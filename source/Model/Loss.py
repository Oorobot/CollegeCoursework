from collections import deque
from typing import Deque

import numpy as np
import scipy
import tensorflow as tf
import tensorflow_graphics.nn.loss.chamfer_distance as chamfer_distance
import tensorflow_probability as tfp
from tensorflow.keras.layers import Layer

from .Mesh import Mesh


class ChamferLossLayer(Layer):
    def __init__(self):
        super(ChamferLossLayer, self).__init__()

    def call(self, cloud1, cloud2, num_samples):
        """
        Arguments:
        cloud1 is a tf tensor of shape (N,P1,D) where:
            N is number of clouds in batch,
            P1 is the max number of points amoung all of the images in the batch
            D is dimensionality of system
        cloud2 is a tf tensor of shape (M,P2,D) where:
            N is number of clouds in batch,
            P2 is the max number of points amoung all of the images in the batch
            D is dimensionality of system
        """

        # Assert that cloud1 and cloud2 are tensors of the correct shape.
        assert tf.is_tensor(cloud1)
        assert tf.shape(cloud1)[-1] == 3
        assert tf.is_tensor(cloud2)
        assert tf.shape(cloud2)[-1] == 3

        def subsample(cloud):
            num_points = tf.shape(cloud)[-2]
            point_sample_probs = tf.ones(num_points) / tf.cast(
                num_points, dtype=tf.float32
            )
            point_distribution = tfp.distributions.Categorical(
                probs=point_sample_probs)
            points_to_sample = point_distribution.sample(num_samples)
            return tf.gather(cloud, points_to_sample)

        subsampled_cloud1 = subsample(cloud1)
        subsampled_cloud2 = subsample(cloud2)

        return chamfer_distance.evaluate(subsampled_cloud1, subsampled_cloud2)


class BeamGapLossLayer(Layer):
    def __init__(self, target_function):
        super(BeamGapLossLayer, self).__init__()
        self.target_function = target_function
        self.points = None
        self.mask = None

    def update_points_mask(self, mesh, vertices, target_point_cloud):
        self.points, self.mask = self.target_function(
            mesh, vertices, target_point_cloud, 0.99
        )

    def call(self, mesh: Mesh, vertices: tf.Tensor):
        affected_faces = mesh.faces[self.mask]
        affected_points = self.points[self.mask]

        face_vertices = tf.gather(vertices, affected_faces)
        face_midpoints = tf.math.reduce_mean(face_vertices, axis=1)
        distances = tf.norm(affected_points - face_midpoints, axis=1)
        return 10.0 * tf.math.reduce_mean(distances)


def discrete_project(mesh, point_cloud, vertices=None, threshold=0.9):
    assert tf.is_tensor(point_cloud)
    if not tf.is_tensor(vertices):
        vertices = mesh.vertices
    else:
        vertices = vertices.numpy()
    assert point_cloud.shape[-1] == 3
    assert isinstance(mesh, Mesh)

    point_cloud = tf.cast(point_cloud, dtype=tf.double)
    point_cloud = point_cloud.numpy()

    mesh_normals, _ = mesh.generate_face_areas_normals(
        tf.convert_to_tensor(vertices)
    )

    mesh_normals = mesh_normals.numpy()

    mid_points = vertices[mesh.faces]

    mid_points = tf.math.reduce_mean(mid_points, axis=1).numpy()

    looping_mesh_points_mask = get_looping_points(mid_points, point_cloud)

    masked_mid_points = mid_points[~looping_mesh_points_mask, :]
    maksed_normals = mesh_normals[~looping_mesh_points_mask, :]

    distance, masked_with_cone = distance_within_cone(
        masked_mid_points, maksed_normals, point_cloud, threshold
    )

    distance[~masked_with_cone] += float("inf")
    min = np.amin(distance, axis=-1)
    arg_min = np.argmin(distance, axis=-1)

    pc_per_masked_face = point_cloud[arg_min, :].copy()
    pc_per_masked_face[min == float("inf"), :] = float("nan")

    pc_per_face = np.zeros(
        shape=(np.shape(mid_points)[0], 3), dtype=pc_per_masked_face.dtype
    )

    pc_per_face[~looping_mesh_points_mask, :] = pc_per_masked_face

    pc_per_face[looping_mesh_points_mask, :] = float("nan")

    pc_is_non_nan = pc_per_face[:, 0] == pc_per_face[:, 0]

    return tf.convert_to_tensor(pc_per_face, dtype=tf.float32), tf.convert_to_tensor(
        pc_is_non_nan, dtype=tf.bool
    )


def get_looping_points(mid_points, point_cloud, k=3):
    KDTree_cloud = scipy.spatial.KDTree(point_cloud)
    KDTree_mesh = scipy.spatial.KDTree(mid_points)

    _, k_closest_cloud_points2mesh = KDTree_cloud.query(
        mid_points, k=k, eps=0
    )
    _, k_closest_mesh_points2cloud = KDTree_mesh.query(
        point_cloud, k=k, eps=0
    )

    neighbors_neighbors = np.reshape(
        k_closest_mesh_points2cloud[k_closest_cloud_points2mesh],
        (np.shape(mid_points)[0], -1),
    )

    looping_mesh_points_mask = (
        np.sum(
            np.equal(
                neighbors_neighbors,
                np.expand_dims(np.arange(0, np.shape(mid_points)[0]), axis=1),
            ),
            axis=1,
        )
        > 0
    )

    return looping_mesh_points_mask


def distance_within_cone(mid_points, normals, point_cloud, threshold):
    displacement = (
        mid_points[:, None, :] - point_cloud
    )  # (num_mid_points, num_points_in_cloud, 3)

    # Compute distance via L2 norm on displacements
    distance = np.linalg.norm(
        displacement, axis=-1
    )  # (num_mid_points, num_points_in_cloud)

    masked_within_cone = np.array(
        np.abs(
            np.sum(
                (displacement / distance[:, :, None]) * normals[:, None, :],
                axis=-1,
            )
        )
        > threshold
    )

    return distance, masked_within_cone


class ConvergenceDetector:
    window_size: int
    threshold: float
    loss_window: Deque[float]
    running_average_window: Deque[float]

    def __init__(self, window_size: int = 25, threshold: float = 0.99) -> None:
        super().__init__()
        self.window_size = window_size
        self.threshold = threshold
        self.loss_window = deque()
        self.running_average_window = deque()

    def step(self, loss_value: float) -> bool:
        # Update the loss window.
        self.loss_window.append(loss_value)
        if len(self.loss_window) > self.window_size:
            self.loss_window.popleft()

        # Calculate the average over the window.
        loss_sum = 0
        for loss in self.loss_window:
            loss_sum += loss
        loss_average = loss_sum / len(self.loss_window)

        # Update the running average window.
        self.running_average_window.append(loss_average)
        if len(self.running_average_window) > self.window_size:
            past_average = self.running_average_window.popleft()

            # Indicate that convergence has been detected if the running average
            # of the loss hasn't been decreasing.
            if loss_average > past_average * self.threshold:
                return True

        return False
