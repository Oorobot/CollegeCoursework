import os
import time
import uuid
import trimesh
import numpy as np
from typing import Tuple

MANIFOLD_SOFTWARE_DIR = "MANIFOLD/build/Release"


# 根据点云创建初始网格(凸包网格)
def create_convex_hull(filename_in: str, filename_out: str) -> None:
    points = np.loadtxt(filename_in)
    convex_hull = trimesh.convex.convex_hull(points[:, :3])
    obj = trimesh.exchange.obj.export_obj(convex_hull)
    with open(filename_out, "w") as f:
        f.write(obj)


# 将网格保持为manifold watertight网格
def remesh(vertices: np.ndarray, faces: np.ndarray, faces_num=2000):

    original_file = random_filename("obj")
    with open(original_file, "w") as f:
        mesh = trimesh.Trimesh(vertices=vertices, faces=faces)
        f.write(trimesh.exchange.obj.export_obj(mesh))

    # 创建manifold文件
    manifold_file = random_filename("obj")
    manifold_software_path = os.path.join(MANIFOLD_SOFTWARE_DIR, "manifold")
    cmd = f"start {manifold_software_path} {original_file} {manifold_file}"
    os.system(cmd)

    while(not os.path.exists(manifold_file)):
        time.sleep(2)
        print("正在生成manifold……")

    # 简化manifold文件
    simplified_file = random_filename("obj")
    simplify_software_path = os.path.join(MANIFOLD_SOFTWARE_DIR, "simplify")
    cmd = f"start {simplify_software_path} -i {manifold_file} -o {simplified_file} -f {faces_num}"
    os.system(cmd)

    while(not os.path.exists(simplified_file)):
        time.sleep(2)
        print("正在简化manifold……")

    # 此处起开始单步调试，程序正常运行
    with open(simplified_file, "r") as f:
        mesh = trimesh.exchange.obj.load_obj(f)

    # 删除多余文件
    os.remove(original_file)
    os.remove(manifold_file)
    os.remove(simplified_file)

    return mesh["vertices"], mesh["faces"]


def random_filename(suffix: str, prefix='temp_'):
    return f"{prefix}{uuid.uuid4()}.{suffix}"


def save_obj(filename: str, vertices: np.ndarray, faces: np.ndarray) -> None:
    with open(filename, 'w') as f:
        mesh = trimesh.Trimesh(vertices=vertices, faces=faces)
        f.write(trimesh.exchange.obj.export_obj(mesh))


def load_obj(filename: str) -> Tuple[np.ndarray, np.ndarray]:
    with open(filename, 'r') as f:
        mesh = trimesh.exchange.obj.load_obj(f)
    return mesh["vertices"], mesh["faces"]
