from typing import List, Optional

import tensorflow as tf
from tensorflow.keras.layers import BatchNormalization, Layer, LeakyReLU

from .Mesh import Mesh, features_valid
from .MeshConvolution import MeshConvolution
from .MeshPool import MeshPool
from .MeshUnpool import MeshUnpool
from .Method import CollapseSnapshot


class ConvolutionSequence(Layer):
    """A ConvolutionSequence is simply a number of sequential convolutions."""

    convolutions: List[MeshConvolution]

    def __init__(self, out_channels: int, num_convolutions: int):
        Layer.__init__(self)

        # Create num_convolutions MeshConvolution layers.
        self.convolutions = [
            MeshConvolution(out_channels) for _ in range(num_convolutions)
        ]

    def call(self, mesh: Mesh, features: tf.Tensor) -> tf.Tensor:
        assert features_valid(mesh, features)

        for convolution in self.convolutions:
            features = convolution(mesh, features)

        return features


class DownConvolution(Layer):
    """A DownConvolution combines several skip-connected ConvolutionSequence
    blocks separated by nonlinearities and batch normalization. It can
    optionally include a pooling step at the end.
    """

    convolutions: List[ConvolutionSequence]
    mesh_pool: Optional[MeshPool]
    batch_normalizations: List[BatchNormalization]
    leaky_relu: LeakyReLU

    def __init__(
        self,
        out_channels: int,
        convolutions_per_sequence: int,
        num_sequences: int,
        leaky_relu_alpha: float,
        pool_target: Optional[int],
    ):
        Layer.__init__(self)
        self.convolutions = [
            ConvolutionSequence(out_channels, convolutions_per_sequence)
            for _ in range(num_sequences)
        ]
        self.leaky_relu = LeakyReLU(leaky_relu_alpha)
        self.batch_normalizations = [
            BatchNormalization() for _ in range(num_sequences)]
        self.mesh_pool = None if pool_target is None else MeshPool(pool_target)

    def call(self, mesh: Mesh, features: tf.Tensor) -> tf.Tensor:
        assert features_valid(mesh, features)

        # Run through the first ConvolutionSequence.
        out_features = self.convolutions[0](mesh, features)
        out_features = self.leaky_relu(out_features)
        out_features = self.batch_normalizations[0](
            out_features, training=True)

        # Run the remaining ConvolutionSequences with skip connections.
        in_features = out_features
        for i in range(1, len(self.convolutions)):
            # Create out_features using in_features.
            out_features = self.convolutions[i](mesh, in_features)
            out_features = self.leaky_relu(out_features)
            out_features = self.batch_normalizations[i](
                out_features, training=True)

            # Add the skip connections.
            out_features += in_features
            in_features = out_features

        # Optionally run pooling.
        if self.mesh_pool is not None:
            return self.mesh_pool(mesh, out_features)
        return out_features, None


class UpConvolution(Layer):
    """An UpConvolution combines several skip-connected ConvolutionSequence
    blocks separated by nonlinearities and batch normalization. It can
    optionally include an unpooling step at the end.
    """

    convolutions: List[ConvolutionSequence]
    mesh_unpool: Optional[MeshUnpool]
    batch_normalizations: List[BatchNormalization]
    leaky_relu: LeakyReLU

    def __init__(
        self,
        out_channels: int,
        convolutions_per_sequence: int,
        num_sequences: int,
        leaky_relu_alpha: float,
        unpool: bool,
    ):
        Layer.__init__(self)
        self.convolutions = [
            ConvolutionSequence(out_channels, convolutions_per_sequence)
            for _ in range(num_sequences)
        ]
        self.leaky_relu = LeakyReLU(leaky_relu_alpha)
        self.batch_normalizations = [
            BatchNormalization() for _ in range(num_sequences)]
        self.mesh_unpool = MeshUnpool() if unpool else None

    def call(
        self, mesh: Mesh, features: tf.Tensor, snapshot: CollapseSnapshot
    ) -> tf.Tensor:
        assert features_valid(mesh, features)

        # Run through the first ConvolutionSequence.
        out_features = self.convolutions[0](mesh, features)
        out_features = self.leaky_relu(out_features)
        out_features = self.batch_normalizations[0](
            out_features, training=True)

        # Run the remaining ConvolutionSequences with skip connections.
        in_features = out_features
        for i in range(1, len(self.convolutions)):
            # Create out_features using in_features.
            out_features = self.convolutions[i](mesh, in_features)
            out_features = self.leaky_relu(out_features)
            out_features = self.batch_normalizations[i](
                out_features, training=True)

            # Add the skip connections.
            out_features += in_features
            in_features = out_features

        # Optionally run unpooling.
        if self.mesh_unpool is not None:
            return self.mesh_unpool(mesh, out_features, snapshot)
        return out_features


class Encoder(Layer):
    """This combines several DownConvolution layers."""

    down_convolutions: List[DownConvolution]

    def __init__(
        self,
        out_channels: List[int],
        convolutions_per_sequence: int,
        num_sequences_per_down_convolution: int,
        leaky_relu_alpha: float,
        pool_targets: List[Optional[int]],
    ):
        Layer.__init__(self)

        assert len(out_channels) == len(pool_targets)
        for i in range(1, len(pool_targets)):
            assert pool_targets[i] is None or pool_targets[i] < pool_targets[i - 1]

        self.down_convolutions = [
            DownConvolution(
                out_channels[i],
                convolutions_per_sequence,
                num_sequences_per_down_convolution,
                leaky_relu_alpha,
                pool_targets[i],
            )
            for i in range(len(pool_targets))
        ]

    def call(self, mesh: Mesh, features: tf.Tensor) -> tf.Tensor:
        assert features_valid(mesh, features)

        snapshots = []
        for down_convolution in self.down_convolutions:
            features, snapshot = down_convolution(mesh, features)
            snapshots.append(snapshot)

        return features, snapshots


class Decoder(Layer):
    """This combines several UpConvolution layers."""

    up_convolutions: List[UpConvolution]

    def __init__(
        self,
        out_channels: List[int],
        convolutions_per_sequence: int,
        num_sequences_per_down_convolution: int,
        leaky_relu_alpha: float,
        encoder_pool_targets: List[Optional[int]],
    ):
        Layer.__init__(self)

        assert len(out_channels) == len(encoder_pool_targets)

        needs_unpool = [x != None for x in reversed(encoder_pool_targets)]
        self.up_convolutions = [
            UpConvolution(
                out_channels[i],
                convolutions_per_sequence,
                num_sequences_per_down_convolution,
                leaky_relu_alpha,
                needs_unpool[i],
            )
            for i in range(len(encoder_pool_targets))
        ]

    def call(
        self, mesh: Mesh, features: tf.Tensor, snapshots: CollapseSnapshot
    ) -> tf.Tensor:
        assert features_valid(mesh, features)

        snapshots = reversed(snapshots)
        for up_convolution, snapshot in zip(self.up_convolutions, snapshots):
            features = up_convolution(mesh, features, snapshot)

        return features
