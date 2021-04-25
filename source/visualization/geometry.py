import os
from typing import List, Optional

import numpy as np
import open3d as o3d


class GeometryInfo():
    id: int
    file: str
    name: str
    visible: bool
    geometry: None
    num_vertices: int
    num_faces: int

    def __init__(self, path:str) -> None:
        '''
            path: 文件路径, path = None ,则需在使用init()函数
        '''
        self.id = -1
        self.visible = True
        self.file = ""
        self.name = ""
        self.path = ""
        self.geometry = None
        if path is not None:
            self.read(path)

    def __str__(self) -> str:
        return f"[{self.__class__.__name__}: id={self.id}, name={self.name}, geometry={self.geometry}, visible={self.visible}]"
    
    def init(self, name: str, geometry, visible: bool) -> bool:
        if geometry is None:
            print("[ERROR] geometry is None.")
            return False
        else:
            self.name = name
            self.geometry = geometry
            self.visible = visible
            type = self.geometry.get_geometry_type()
            if type == o3d.geometry.Geometry.PointCloud:
                self.num_vertices = len(geometry.points)
                self.num_faces = 0
            if type == o3d.geometry.Geometry.TriangleMesh:
                self.num_vertices = len(geometry.vertices)
                self.num_faces = len(geometry.triangles)
            return True

    def set_id(self, id: int):
        self.id = id

    def read(self, path: str) -> bool:

        self.path = path
        self.file = os.path.basename(path)
        self.name = self.file.split(".")[0]

        geometry = None
        geometry_type = o3d.io.read_file_geometry_type(path)
        mesh = None
        if geometry_type & o3d.io.CONTAINS_TRIANGLES:
            mesh = o3d.io.read_triangle_mesh(path)
        if mesh is not None:
            if len(mesh.triangles) == 0:
                print("[WARNING] Contains 0 triangles, will read as point cloud")
                mesh = None
            else:
                print("[INFO] Successfully read", path)
                self.num_vertices = len(mesh.vertices)
                self.num_faces = len(mesh.triangles)

                mesh.compute_vertex_normals()
                if len(mesh.vertex_colors) == 0:
                    mesh.paint_uniform_color([1, 1, 1])
                geometry = mesh

                if not mesh.has_triangle_uvs():
                    uv = np.array([[0.0, 0.0]] * (3 * len(mesh.triangles)))
                    mesh.triangle_uvs = o3d.utility.Vector2dVector(uv)
        else:
            print("[INFO]", path, "appears to be a point cloud")
        if geometry is None:
            cloud = None
            suffix = path.split(".")[1]
            try:
                if suffix == "txt" or suffix == "pwn":
                    cloud = o3d.io.read_point_cloud(path, format="xyz")
                else:
                    cloud = o3d.io.read_point_cloud(path)
            except Exception:
                pass
            if cloud is not None:
                print("[INFO] Successfully read", path)
                self.num_vertices = len(cloud.points)
                self.num_faces = 0

                if not cloud.has_normals():
                    cloud.estimate_normals()
                cloud.normalize_normals()
                geometry = cloud
            else:
                print("[WARNING] Failed to read points", path)
        if geometry is not None:
            self.geometry = geometry
            return True
        return False

    def write(self, path: str) -> bool:
        suffix = path.split(".")[1]
        point_cloud_file = ["xyz", "xyzn", "xyzrgb", "ply", "pcd", "pts"]
        mesh_file = ["ply", "stl", "fbx", "obj", "off", "gltf", "glb"]
        if self.geometry is None:
            return False
        else:
            vertices, faces = self.to_numpy()

            def point_cloud(path, geometry) -> bool:
                try:
                    o3d.io.write_point_cloud(path, geometry, write_ascii=True)
                    return True
                except Exception as e:
                    print(e)
                    return False

            def mesh(path, geometry) -> bool:
                try:
                    o3d.io.write_triangle_mesh(
                        path, geometry, write_ascii=True)
                    return True
                except Exception as e:
                    print(e)
                    return False

            if faces is None:
                if suffix in point_cloud_file:
                    return point_cloud(path, self.geometry)
                else:
                    faces = np.array([]).reshape(-1, 3)
                    return mesh(path, GeometryInfo.from_numpy(vertices, faces))
            else:
                if suffix in mesh_file:
                    return mesh(path, self.geometry)
                else:
                    return point_cloud(path, GeometryInfo.from_numpy(vertices, None))

    def to_numpy(self):
        """convert self.geometry(o3d.geometry.Geometry.PointCloud or TriangleMesh) to numpy"""

        assert self.geometry is not None
        type = self.geometry.get_geometry_type()
        assert type == o3d.geometry.Geometry.PointCloud or type == o3d.geometry.Geometry.TriangleMesh

        vertices = None
        faces = None
        if type == o3d.geometry.Geometry.PointCloud:
            vertices = np.asarray(self.geometry.points)
        if type == o3d.geometry.Geometry.TriangleMesh:
            vertices = np.asarray(self.geometry.vertices)
            faces = np.asarray(self.geometry.triangles)
        return vertices, faces

    @staticmethod
    def from_numpy(vertices: np.ndarray, faces: Optional[np.ndarray]):
        """
            vertices: shape(num, 3)
            faces: shape(num, 3) or None
            return: open3d.geometry.Geometry.PointCloud or TriangleMesh
        """
        geometry = None
        if faces is None:
            geometry = o3d.geometry.PointCloud()
            geometry.points = o3d.utility.Vector3dVector(vertices)
        else:
            geometry = o3d.geometry.TriangleMesh()
            geometry.vertices = o3d.utility.Vector3dVector(vertices)
            geometry.triangles = o3d.utility.Vector3iVector(faces)
        return geometry


class GeometryInfos():
    geometrt_infos: List[GeometryInfo]
    geometry_names = List[str]

    def __init__(self) -> None:
        self.geometrt_infos = []
        self.geometry_names = []

    def push_back(self, geometry_info: GeometryInfo):
        if not isinstance(geometry_info, GeometryInfo):
            print("[WARNING] 添加元素类型错误")
        else:
            self.check_name(geometry_info)
            self.geometrt_infos.append(geometry_info)

    # 检测是否重名，若重名则修改
    def check_name(self, geometry_info: GeometryInfo):
        name = geometry_info.name
        while name in self.geometry_names:
            if not name.endswith(")"):
                name = name + "(1)"
            else:
                try:
                    left = name.rindex("(")
                    number = name[left+1:len(name)-1]
                    if number.isdigit():
                        name = name[:left]
                        number = str(int(number)+1)
                        name = name + "(" + number + ")"
                    else:
                        name = name + "(1)"
                except Exception as e:
                    name = name + "(1)"
                    pass
        self.geometry_names.append(name)
        geometry_info.name = name

    def remove(self, id: int):
        geometry_info = None
        for g_info in self.geometrt_infos:
            if g_info.id == id:
                geometry_info = g_info
                break
        if geometry_info is not None:
            self.geometrt_infos.remove(geometry_info)
            self.geometry_names.remove(geometry_info.name)
        else:
            print("[WARNING] no such geometry")

    def get(self, id: int) -> GeometryInfo:
        geometry_info = None
        for g_info in self.geometrt_infos:
            if g_info.id == id:
                geometry_info = g_info
                break
        return geometry_info
    
    def getAll(self) -> List[GeometryInfo]:
        return self.geometrt_infos
