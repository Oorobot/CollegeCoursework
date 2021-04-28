import os
from typing import List, Optional, Tuple

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
    line_set: None
    line_set_visible: bool
    point_cloud: None
    point_cloud_visible: bool

    def __init__(self, path=None, name="", geometry=None) -> None:
        """ 使用时，仅用 path 参数 或 用 name，geometry 参数
            path: 文件路径或None。 
            name: 名称
            geometry: Point Cloud or Triangle Mesh
        """
        self.id = -1
        self.visible = True
        self.line_set_visible = False
        self.point_cloud_visible = False
        if path is None:
            self.file = ""
            self.name = name
            self.geometry = geometry
        else:
            self.file = os.path.basename(path)
            self.name = self.file.split(".")[0]
            self.geometry = GeometryInfo.read(path)
        self.bulid()
    
    def bulid(self):
        if self.geometry:
            if self.geometry.get_geometry_type() == o3d.geometry.Geometry.PointCloud:
                self.num_vertices = len(self.geometry.points)
                self.num_faces = 0
                self.line_set = None
                self.point_cloud = None
            else:  # o3d.geometry.Geometry.TriangleMesh
                self.num_vertices = len(self.geometry.vertices)
                self.num_faces = len(self.geometry.triangles)
                self.line_set = o3d.geometry.LineSet.create_from_triangle_mesh(
                    self.geometry)
                self.line_set.paint_uniform_color((0.5098, 0.5098, 0.5098))
                vertices, _ = GeometryInfo.o3d_to_numpy(self.geometry)
                self.point_cloud = GeometryInfo.numpy_to_o3d(vertices, None)
                self.point_cloud.paint_uniform_color((1, 0.7569, 0.1451))
        else:
            self.num_vertices = 0
            self.num_faces = 0
            self.line_set = None
            self.point_cloud = None

    def set_geometry(self, geometry):
        self.geometry = geometry

    def set_id(self, id: int):
        self.id = id
    
    def save(self, path):
        return GeometryInfo.write(self.geometry, path)

    @staticmethod
    def read(path: str):
        geometry = None
        mesh = None
        # 尝试作为 triangle mesh 读取
        try:
            mesh = o3d.io.read_triangle_mesh(path)
        except Exception as e:
            pass
        if len(mesh.vertices) != 0:
            if len(mesh.triangles) == 0:
                print("[Info] Contains 0 triangles, will convert to point cloud")
                # 将 triangle mesh 转换为 point cloud
                vertices = np.asarray(mesh.vertices)
                cloud = o3d.geometry.PointCloud()
                cloud.points = o3d.utility.Vector3dVector(vertices)
                # 计算法向量
                cloud.estimate_normals()
                cloud.normalize_normals()
                geometry = cloud
            else:
                # 计算点的法向量
                mesh.compute_vertex_normals()
                # 添加点的颜色属性
                if len(mesh.vertex_colors) == 0:
                    mesh.paint_uniform_color([1, 1, 1])
                geometry = mesh
            print("[Info] Successfully read", path)
        else:
            print("[Info]", path, "try to read as a point cloud")
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
            if len(cloud.points) != 0:
                # 计算法向量
                if not cloud.has_normals():
                    cloud.estimate_normals()
                cloud.normalize_normals()
                geometry = cloud
                print("[Info] Successfully read", path)
            else:
                print("[WARNING] Failed to read points", path)
        return geometry        

    @staticmethod
    def write(geometry, path: str) -> bool:
        """save o3d.geometry.Geometry.PointCloud or TriangleMesh as a file"""

        # geometry 不能为空且必须为open3d的指定类型
        assert geometry is not None
        type = geometry.get_geometry_type()
        assert type == o3d.geometry.Geometry.PointCloud or type == o3d.geometry.Geometry.TriangleMesh

        # 获取文件扩展名
        suffix = path.split(".")[1]
        point_cloud_file = ["xyz", "xyzn", "xyzrgb", "ply", "pcd", "pts"]
        mesh_file = ["ply", "stl", "fbx", "obj", "off", "gltf", "glb"]

        vertices, faces = GeometryInfo.o3d_to_numpy(geometry)

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
        # 没有面的数据时
        if faces is None:
            if suffix in point_cloud_file:
                return point_cloud(path, geometry)
            else:
                faces = np.array([]).reshape(-1, 3)
                print("[WARNING] point cloud should not save as a mesh file.")
                return mesh(path, GeometryInfo.numpy_to_o3d(vertices, faces))
        # 有面的数据时
        else:
            if suffix in mesh_file:
                return mesh(path, geometry)
            else:
                print("[Info] only save the vertices of the mesh.")
                return point_cloud(path, GeometryInfo.numpy_to_o3d(vertices, None))

    @staticmethod
    def o3d_to_numpy(geometry) -> Tuple[np.ndarray, np.ndarray]:
        """convert geometry(o3d.geometry.Geometry.PointCloud or TriangleMesh) to numpy"""

        # geometry 不能为空且必须为open3d的指定类型
        assert geometry is not None
        type = geometry.get_geometry_type()
        assert type == o3d.geometry.Geometry.PointCloud or type == o3d.geometry.Geometry.TriangleMesh

        vertices = None
        faces = None
        if type == o3d.geometry.Geometry.PointCloud:
            vertices = np.asarray(geometry.points)
        if type == o3d.geometry.Geometry.TriangleMesh:
            vertices = np.asarray(geometry.vertices)
            faces = np.asarray(geometry.triangles)
        return vertices, faces

    @staticmethod
    def numpy_to_o3d(vertices: np.ndarray, faces: Optional[np.ndarray]):
        """convert numpy to open3d.geometry.Geometry.PointCloud or TriangleMesh

            vertices: shape(num, 3)
            faces: shape(num, 3) or None
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
    geometry_infos: List[GeometryInfo]
    geometry_names: List[str]

    def __init__(self) -> None:
        self.geometry_infos = []
        self.geometry_names = []

    def get(self, id=-1, name=None) -> GeometryInfo:
        """ 根据 id 或 name 找到 GeometryInfo """
        geometry_info = None
        if name == None:
            for g_info in self.geometry_infos:
                if g_info.id == id:
                    geometry_info = g_info
                    break
        if id == -1:
            for g_info in self.geometry_infos:
                if g_info.name == name:
                    geometry_info = g_info
                    break
        return geometry_info

    def pop(self):
        if len(self.geometry_infos) > 0:
            self.geometry_infos.pop()
            self.geometry_names.pop()
        else:
            print("[WARNING] no item to pop")

    def push_back(self, geometry_info: GeometryInfo):
        if not isinstance(geometry_info, GeometryInfo):
            print("[WARNING] 添加元素类型错误")
        else:
            # 检查是否重名，若重名则修改名字
            name = geometry_info.name
            while name in self.geometry_names:
                # 名字后缀无编号，直接添加编号
                if not name.endswith(")"):
                    name = name + "(1)"
                else:
                    try:
                        # 找名字中最后的“(”的位置
                        right = name.rindex("(")
                        # 截取括号间的字符串
                        number = name[right + 1: len(name) - 1]
                        # 编号加1
                        if number.isdigit():
                            name = name[:right]
                            name = name + "(" + str(int(number)+1) + ")"
                        # 直接添加编号
                        else:
                            name = name + "(1)"
                    # 名字中无“(”时，直接添加编号
                    except Exception as e:
                        name = name + "(1)"
            # 修改名字
            geometry_info.name = name
            # 添加 info
            self.geometry_names.append(geometry_info.name)
            self.geometry_infos.append(geometry_info)

    def remove(self, id: int):
        for g_info in self.geometry_infos:
            if g_info.id == id:
                self.geometry_infos.remove(g_info)
                self.geometry_names.remove(g_info.name)
                break

