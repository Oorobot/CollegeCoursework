import scipy
import numpy as np
import tensorflow as tf
import tensorflow_probability as tfp
import tensorflow_graphics.nn.loss.chamfer_distance as chamfer_distance
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
        self.points,self.mask = self.target_function(
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

    distance,masked_with_cone = distance_within_cone(
        masked_mid_points, maksed_normals, point_cloud, threshold
    )

    distance[~masked_with_cone] += float("inf")


def get_looping_points(mid_points, point_cloud):
    pass


def distance_within_cone(mid_points, mesh_normals, point_cloud, threshold):
    pass
