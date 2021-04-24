from typing import Optional
import open3d as o3d
import numpy as np


def from_o3d_to_numpy(geometry):
    vertices = None
    faces = None
    if geometry.get_geometry_type() == o3d.geometry.Geometry.PointCloud:
        vertices = np.asarray(geometry.points)
    if geometry.get_geometry_type() == o3d.geometry.Geometry.TriangleMesh:
        vertices = np.asarray(geometry.vertices)
        faces = np.asarray(geometry.triangles)
    return vertices, faces


def from_numpy_to_o3d(vertices: np.ndarray, faces: Optional[np.ndarray]):
    geometry = None
    if faces is None:
        geometry = o3d.geometry.PointCloud()
        geometry.points = o3d.utility.Vector3dVector(vertices)
    else:
        geometry = o3d.geometry.TriangleMesh()
        geometry.vertices = o3d.utility.Vector3dVector(vertices)
        geometry.triangles = o3d.utility.Vector3iVector(faces)
    return geometry


def get_open3d_geometry_info(geometry):
    vertices = None
    faces = np.array([]).reshape(-1, 3)
    if geometry.get_geometry_type() == o3d.geometry.Geometry.PointCloud:
        vertices = np.asarray(geometry.points)
    if geometry.get_geometry_type() == o3d.geometry.Geometry.TriangleMesh:
        vertices = np.asarray(geometry.vertices)
        faces = np.asarray(geometry.triangles)
    return vertices.shape[0], faces.shape[0]
