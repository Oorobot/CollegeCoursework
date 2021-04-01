from pathlib import WindowsPath
import tkinter as tk
import open3d as o3d
import trimesh 
import numpy as np

# main_window = tk.Tk()
# main_window.mainloop()

mesh = o3d.io.read_triangle_mesh("bun_zipper.ply")
print(mesh)
# o3d.visualization.draw_geometries([mesh])
vertices = np.asarray(mesh.vertices)
with open("bunny.txt", 'w') as f:
    for v in vertices:
        f.write(str(v[0])+" "+str(v[1])+" "+str(v[2])+"\n")

# with open("bun_zipper.ply",'r') as f:
#     mesh = trimesh.exchange.ply.load_ply(f)
# mesh.show()
