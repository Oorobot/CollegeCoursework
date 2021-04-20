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


def o3d_info(geometry) -> str:
    info = {}
    infomation = None
    if geometry.get_geometry_type() == o3d.geometry.Geometry.PointCloud:
        info["vertices"] = np.asarray(geometry.points)
        # if geometry.has_colors():
        #     info["vertex_colors"] = np.asarray(geometry.colors)
        # if geometry.has_normals():
        #     info["vertex_normals"] = np.asarray(geometry.normals)
        infomation = "Tpye: PointCloud, Vertices: " + \
            str(info["vertices"].shape[0])
    if geometry.get_geometry_type() == o3d.geometry.Geometry.TriangleMesh:
        info["vertices"] = np.asarray(geometry.vertices)
        info["faces"] = np.asarray(geometry.triangles)
        infomation = "Type: TriangleMesh, Vertices: " + \
            str(info["vertices"].shape[0]) + \
            " faces: " + str(info["faces"].shape[0])
    return infomation
