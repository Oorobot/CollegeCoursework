from typing import List

import tensorflow as tf
from tensorflow.keras import Model

from .CompositeLayer import Decoder, Encoder
from .Mesh import Mesh, features_valid
from .MeshConvolution import MeshConvolution


class PointToMeshModel(Model):
    def __init__(self, initial_num_edges: int, pooling: List[float]) -> None:
        super(PointToMeshModel, self).__init__()
        assert len(pooling) == 6
        scaled_pooling = [
            None if p is None else initial_num_edges * p for p in pooling]
        self.encoder = Encoder((6, 16, 32, 64, 64, 128),
                               1, 3, 0.01, scaled_pooling)
        self.decoder = Decoder((64, 64, 32, 16, 6, 6),
                               1, 1, 0.01, scaled_pooling)
        self.batch_normalization = tf.keras.layers.BatchNormalization()
        initializer = tf.keras.initializers.RandomUniform(-1e-8, 1e-8)
        self.final_convolution = MeshConvolution(6, initializer, initializer)

    def call(self, mesh: Mesh, fixed_input_features: tf.Tensor):
        assert tf.rank(
            fixed_input_features) == 2 and fixed_input_features.shape[1] == 6

        encoding, snapshots = self.encoder(mesh, fixed_input_features)
        decoding = self.decoder(mesh, encoding, snapshots)
        normalized = self.batch_normalization(decoding, training=True)
        output = self.final_convolution(mesh, normalized)

        return output


def get_vertex_features(mesh: Mesh, features: tf.Tensor):
    """Convert edge features in shape (num_edges, 6) to vertex features in shape
    (num_vertices, 3).
    """
    assert features_valid(mesh, features)
    assert features.shape[1] == 6

    # Reshape features into the following shape:
    # (num_edges, num_vertices_per_edge = 2, dimensions_per_vertex = 3)
    edge_features = tf.reshape(features, (-1, 2, 3))

    # Now, pad the first two dimensions with slices of zeros.
    # This will be needed to handle variable vertex degrees using gather_nd.
    edge_features = tf.pad(edge_features, ((0, 1), (0, 1), (0, 0)))

    # Since TensorFlow doesn't have NumPy like indexing, it's a bit harder to do
    # this than in PyTorch.
    gathered_vertex_features = tf.gather_nd(
        edge_features, mesh.vertex_to_edges_tensor)
    gathered_vertex_features = tf.reshape(
        gathered_vertex_features, (
            mesh.vertices.shape[0], mesh.max_vertex_degree, 3)
    )

    # Get a weighted average of the features accumulated from the edges.
    vertex_features = tf.math.reduce_sum(gathered_vertex_features, axis=1)
    vertex_features /= mesh.vertex_to_degree[:, None]

    return vertex_features
