import os
import time

import numpy as np
import open3d as o3d
import open3d.visualization.gui as gui
import open3d.visualization.rendering as rendering
import tensorflow as tf
import Tools
import trimesh

from .settings import Settings
from model.Loss import ChamferLossLayer, ConvergenceDetector
from model.Mesh import Mesh
from model.PointToMeshModel import PointToMeshModel, get_vertex_features

# 训练参数
Options = {
    # The point cloud that's fitted.
    "point_cloud": "bunny.txt",
    # The folder where the results are saved.
    "save_location": "results/bunny",
    # An optional initial mesh.
    "initial_mesh": None,
    # The number of times remeshing/subdivision happens.

    "num_subdivisions": 6,
    # The number of iterations between each remeshing/subdivision.
    "num_iterations": 1000,
    # Each subdivision multiplies the number of faces by this.
    "subdivision_multiplier": 1.5,
    # The initial number of faces used for optimization.

    "initial_num_faces": 1000,
    # The maximum number of faces that subdivision is allowed to yield.
    "max_num_faces": 10000,
    # how often to run beamgap loss if -1 then no beam gap loss

    "beamgap_modulo": -1,
    # how often to save objs
    "obj_save_modulo": 5,

    # range to lineralyinterp between when computing samples
    "min_num_samples": 10000,
    "max_num_samples": 16000,
    "pooling": [None, None, None, None, None, None],
}


class MainWindow:
    MENU_FILE = 1
    MENU_QUIT = 3
    MENU_ABOUT = 21

    def __init__(self):

        self.settings = Settings()
        self.options = None

        self.window = gui.Application.instance.create_window(
            "Point To Mesh", 1600, 800)

        em = self.window.theme.font_size

        # 菜单栏以及菜单设置
        if gui.Application.instance.menubar is None:
            menubar = gui.Menu()
            menu = gui.Menu()
            menu.add_item("File", MainWindow.MENU_FILE)
            menu.add_item("About", MainWindow.MENU_ABOUT)
            menu.add_separator()
            menu.add_item("Quit", MainWindow.MENU_QUIT)
            menubar.add_menu("View", menu)
            gui.Application.instance.menubar = menubar

        self.window.set_on_menu_item_activated(
            MainWindow.MENU_FILE, self._on_menu_file)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_ABOUT, self._on_menu_about)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_QUIT, self._on_menu_quit)

        # 显示“三维模型”部分
        self._scene = gui.SceneWidget()
        self._scene.scene = rendering.Open3DScene(self.window.renderer)
        self._scene.scene.set_background([1, 1, 1, 1])
        self._scene.scene.scene.set_sun_light(
            [-1, -1, -1],  # direction
            [1, 1, 1],  # color
            100000)
        self._scene.scene.scene.enable_sun_light(True)

        # 设置参数部分
        self.control_layout = gui.Vert(
            0, gui.Margins(em*0.5, em*0.5, em*0.5, em*0.5))

        # 选择使用的点云文件
        self._point_cloud = gui.TextEdit()
        point_cloud_button = gui.Button("...")
        point_cloud_button.horizontal_padding_em = 0.5
        point_cloud_button.vertical_padding_em = 0

        point_cloud_button.set_on_clicked(self._on_point_cloud_button)

        point_cloud_layout = gui.Horiz()
        point_cloud_layout.add_child(gui.Label("Point Cloud:"))
        point_cloud_layout.add_child(self._point_cloud)
        point_cloud_layout.add_child(point_cloud_button)

        self.control_layout.add_child(point_cloud_layout)

        # 训练结果存放的文件夹
        self._result_folder = gui.TextEdit()
        result_folder_layout = gui.Horiz()

        result_folder_layout.add_child(gui.Label("Result file Folder:"))
        result_folder_layout.add_child(self._result_folder)

        self.control_layout.add_child(result_folder_layout)

        # 是否有初始网格
        self.initial_mesh_status = gui.Checkbox("Add initial mesh model")
        self.initial_mesh_status.set_on_checked(self._on_initial_mesh_status)
        self._initial_mesh = gui.TextEdit()
        initial_mesh_button = gui.Button("...")
        initial_mesh_button.horizontal_padding_em = 0.5
        initial_mesh_button.vertical_padding_em = 0
        initial_mesh_button.set_on_clicked(self._on_initial_mesh_button)

        self.initial_mesh_layout = gui.Horiz()
        self.initial_mesh_layout.add_child(gui.Label("initial mesh:"))
        self.initial_mesh_layout.add_child(self._initial_mesh)
        self.initial_mesh_layout.add_child(initial_mesh_button)

        self.initial_mesh_layout.visible = False

        self.control_layout.add_child(self.initial_mesh_status)
        self.control_layout.add_child(self.initial_mesh_layout)

        # 详细的参数设置
        option_collapsablevert = gui.CollapsableVert(
            "Options", 0.33*em, gui.Margins(0.5*em, 0, 0.5*em, 0))

        option_collapsablevert.set_is_open(False)

        self._num_epoch = gui.NumberEdit(gui.NumberEdit.INT)
        self._num_epoch.int_value = Options["num_subdivisions"]
        self._num_epoch.set_limits(1, 99)

        self._num_iterations = gui.NumberEdit(gui.NumberEdit.INT)
        self._num_iterations.int_value = Options["num_iterations"]
        self._num_iterations.set_limits(100, 10000)

        self._epoch_multiplier = gui.NumberEdit(gui.NumberEdit.DOUBLE)
        self._epoch_multiplier.double_value = Options["subdivision_multiplier"]
        self._epoch_multiplier.set_limits(1.1, 10.0)

        train_parameters_layout = gui.Horiz()
        train_parameters_layout.add_child(gui.Label("number of epoch:"))
        train_parameters_layout.add_child(self._num_epoch)
        train_parameters_layout.add_fixed(em)
        train_parameters_layout.add_child(gui.Label("number of iterations:"))
        train_parameters_layout.add_child(self._num_iterations)

        option_collapsablevert.add_child(train_parameters_layout)

        train_parameters_layout_1 = gui.Horiz()
        train_parameters_layout_1.add_child(
            gui.Label("the multiple of face number of two adjacent epoch:"))
        train_parameters_layout_1.add_child(self._epoch_multiplier)

        option_collapsablevert.add_child(train_parameters_layout_1)

        # 与面的数量相关的参数
        self._initial_faces_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._initial_faces_num.int_value = Options["initial_num_faces"]
        self._initial_faces_num.set_limits(500, 5000)

        self._max_faces_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._max_faces_num.int_value = Options["max_num_faces"]
        self._max_faces_num.set_limits(10000, 1000000)

        face_parameters_layout_0 = gui.Horiz()
        face_parameters_layout_0.add_child(
            gui.Label("The initial face number:"))
        face_parameters_layout_0.add_child(self._initial_faces_num)

        face_parameters_layout_1 = gui.Horiz()
        face_parameters_layout_1.add_child(
            gui.Label("The maximum face number:"))
        face_parameters_layout_1.add_child(self._max_faces_num)

        option_collapsablevert.add_child(face_parameters_layout_0)
        option_collapsablevert.add_child(face_parameters_layout_1)

        # 其他参数
        self._save_obj = gui.NumberEdit(gui.NumberEdit.INT)
        self._save_obj.int_value = Options["obj_save_modulo"]
        self._save_obj.set_limits(1, 100)

        # min_num_samples
        # max_num_samples
        # pooling

        other_parameters_layout = gui.Horiz()
        other_parameters_layout.add_child(gui.Label("how often to save:"))
        other_parameters_layout.add_child(self._save_obj)

        option_collapsablevert.add_child(other_parameters_layout)

        self.control_layout.add_child(option_collapsablevert)

        # “训练”按钮
        Train_button_layout = gui.Horiz()
        Train_button = gui.Button("Train")
        Train_button.set_on_clicked(self._on_train)
        Train_button_layout.add_stretch()
        Train_button_layout.add_child(Train_button)

        self.control_layout.add_child(Train_button_layout)

        self.window.set_on_layout(self._on_layout)
        self.window.add_child(self._scene)
        self.window.add_child(self.control_layout)

    # 布局设置
    def _on_layout(self, theme):
        r = self.window.content_rect
        self._scene.frame = r
        width = min(
            r.width, self.control_layout.calc_preferred_size(theme).width)
        height = min(r.height,
                     self.control_layout.calc_preferred_size(theme).height)
        self.control_layout.frame = gui.Rect(
            r.get_right() - width, r.y, width, height)

    # 菜单栏-载入文件
    def _on_menu_file(self):
        file_dialog = gui.FileDialog(
            gui.FileDialog.OPEN, "Choose File to open:", self.window.theme
        )
        file_dialog.add_filter(
            ".ply .stl .fbx .obj .off .gltf .glb",
            "Triangle mesh files (.ply, .stl, .fbx, .obj, .off, "
            ".gltf, .glb)")
        file_dialog.add_filter(
            ".xyz .xyzn .xyzrgb .ply .pcd .pts",
            "Point cloud files (.xyz, .xyzn, .xyzrgb, .ply, "
            ".pcd, .pts)")
        file_dialog.add_filter(".ply", "Polygon files (.ply)")
        file_dialog.add_filter(".stl", "Stereolithography files (.stl)")
        file_dialog.add_filter(".fbx", "Autodesk Filmbox files (.fbx)")
        file_dialog.add_filter(".obj", "Wavefront OBJ files (.obj)")
        file_dialog.add_filter(".off", "Object file format (.off)")
        file_dialog.add_filter(".gltf", "OpenGL transfer files (.gltf)")
        file_dialog.add_filter(".glb", "OpenGL binary transfer files (.glb)")
        file_dialog.add_filter(".xyz", "ASCII point cloud files (.xyz)")
        file_dialog.add_filter(
            ".xyzn", "ASCII point cloud with normals (.xyzn)")
        file_dialog.add_filter(".xyzrgb",
                               "ASCII point cloud files with colors (.xyzrgb)")
        file_dialog.add_filter(".pcd", "Point Cloud Data files (.pcd)")
        file_dialog.add_filter(".pts", "3D Points files (.pts)")
        file_dialog.add_filter("", "All files")
        file_dialog.set_on_cancel(self._dialog_cancel)
        file_dialog.set_on_done(self._file_dialog_done)
        self.window.show_dialog(file_dialog)

    def _dialog_cancel(self):
        self.window.close_dialog()

    def _file_dialog_done(self, path):
        self.window.close_dialog()
        self.load(path)

    # 菜单栏-关于
    def _on_menu_about(self):
        pass

    # 菜单栏-退出
    def _on_menu_quit(self):
        gui.Application.instance.quit()

    # 添加点云文件按钮
    def _on_point_cloud_button(self):
        point_cloud_dialog = gui.FileDialog(
            gui.FileDialog.OPEN, "Select point cloud file:", self.window.theme)
        point_cloud_dialog.add_filter(
            ".txt .pwn", "fitted point cloud files(.txt .pwn)")
        point_cloud_dialog.set_on_cancel(self._dialog_cancel)
        point_cloud_dialog.set_on_done(self._on_p_c_d_done)
        self.window.show_dialog(point_cloud_dialog)

    def _on_p_c_d_done(self, path):
        self._point_cloud.text_value = path
        path_list = path.split("/")
        filename = path_list.pop()
        path_list.append("results")
        result_path = "/".join(path_list)
        self._result_folder.text_value = result_path
        point_cloud = o3d.io.read_point_cloud(path, format='xyz')

        self._scene.scene.clear_geometry()

        self._scene.scene.add_geometry(
            filename, point_cloud, self.settings.material)
        bounds = point_cloud.get_axis_aligned_bounding_box()
        self._scene.setup_camera(60, bounds, bounds.get_center())

        self.window.close_dialog()

    def _on_initial_mesh_status(self, is_checked):
        if is_checked:
            self.initial_mesh_layout.visible = True
        else:
            self.initial_mesh_layout.visible = False

    # 添加初始网格文件
    def _on_initial_mesh_button(self):
        initial_mesh_dialog = gui.FileDialog(
            gui.FileDialog.OPEN, "Select initial mesh file:", self.window.theme)
        initial_mesh_dialog.add_filter(".obj", "Wavefront OBJ files (.obj)")
        initial_mesh_dialog.set_on_cancel(self._dialog_cancel)
        initial_mesh_dialog.set_on_done(self._i_m_d_done)
        self.window.show_dialog(initial_mesh_dialog)

    def _i_m_d_done(self, path):
        self.window.close_dialog()
        self._initial_mesh.text_value = path

    def _on_train(self):
        self.options = Options.copy()
        self.options["num_subdivisions"] = self._num_epoch.int_value
        self.options["num_iterations"] = self._num_iterations.int_value
        self.options["subdivision_multiplier"] = self._epoch_multiplier.double_value
        self.options["initial_num_faces"] = self._initial_faces_num.int_value
        self.options["max_num_faces"] = self._max_faces_num.int_value
        self.options["obj_save_modulo"] = self._save_obj.int_value
        if len(self._point_cloud.text_value) != 0:
            self.options["point_cloud"] = self._point_cloud.text_value
            self.options["save_location"] = self._result_folder.text_value
        if self.initial_mesh_status.checked and len(self._initial_mesh.text_value) != 0:
            self.options["initial_mesh"] = self._initial_mesh.text_value

        print(self.options)

    def load(self, path):
        self._scene.scene.clear_geometry()
        geometry = None
        geometry_type = o3d.io.read_file_geometry_type(path)
        mesh = None
        if geometry_type & o3d.io.CONTAINS_TRIANGLES:
            mesh = o3d.io.read_triangle_mesh(path)
        if mesh is not None:
            if len(mesh.triangles) == 0:
                print(
                    "[WARNING] Contains 0 triangles, will read as point cloud")
                mesh = None
            else:
                mesh.compute_vertex_normals()
                if len(mesh.vertex_colors) == 0:
                    mesh.paint_uniform_color([1, 1, 1])
                geometry = mesh
            # Make sure the mesh has texture coordinates
            if not mesh.has_triangle_uvs():
                uv = np.array([[0.0, 0.0]] * (3 * len(mesh.triangles)))
                mesh.triangle_uvs = o3d.utility.Vector2dVector(uv)
        else:
            print("[Info]", path, "appears to be a point cloud")

        if geometry is None:
            cloud = None
            try:
                cloud = o3d.io.read_point_cloud(path)
            except Exception:
                pass
            if cloud is not None:
                print("[Info] Successfully read", path)
                if not cloud.has_normals():
                    cloud.estimate_normals()
                cloud.normalize_normals()
                geometry = cloud
            else:
                print("[WARNING] Failed to read points", path)

        if geometry is not None:
            try:
                self._scene.scene.add_geometry("__model__", geometry,
                                               self.settings.material)
                bounds = geometry.get_axis_aligned_bounding_box()
                self._scene.setup_camera(60, bounds, bounds.get_center())
            except Exception as e:
                print(e)

    def train_model(self):

        point_cloud = np.loadtxt(
            fname=self.options["point_cloud"], usecols=(0, 1, 2))
        point_cloud_tf = tf.convert_to_tensor(point_cloud, dtype=tf.float32)

        def save_mesh(filename, vertices, faces):
            Tools.Obj.save(os.path.join(
                self.options["save_location"], filename), vertices, faces)

        # 初始网格
        if self.options["initial_mesh"]:
            remeshed_vertices, remeshed_faces = Tools.Obj.load(
                self.options["initial_mesh"])
        else:
            convex_hull = trimesh.convex.convex_hull(point_cloud)
            remeshed_vertices, remeshed_faces = Tools.remesh(
                convex_hull.vertices, convex_hull.faces, self.options["initial_num_faces"]
            )
        save_mesh("tem_initial_mesh.obj", remeshed_vertices, remeshed_faces)

        # 模型
        chamfer_loss = ChamferLossLayer()
        optimizer = tf.keras.optimizers.Adam(learning_rate=0.00005)
        num_subdivisions = self.options["num_subdivisions"]
        new_vertices = None

        for subdivision_level in range(num_subdivisions):
            chamfer_convergence = ConvergenceDetector()

            if subdivision_level != 0:
                if new_vertices is None:
                    raise Exception("Could not find vertices to subdivide.")
                else:
                    new_face_num = min(
                        self.options["max_num_faces"], self.options["subdivision_multiplier"] *
                        remeshed_faces.shape[0]
                    )
                print(
                    f"Remeshing to {int(new_face_num)} faces"
                )
                remeshed_vertices, remeshed_faces = Tools.remesh(
                    new_vertices.numpy(), remeshed_faces, new_face_num
                )
            else:
                print(
                    f"Starting with {remeshed_faces.shape[0]} faces"
                )
            mesh = Mesh(remeshed_vertices, remeshed_faces)
            model = PointToMeshModel(
                mesh.edge.shape[0], self.options["pooling"])

            # 随机特征值
            in_features = tf.random.uniform((mesh.edge.shape[0], 6), -0.5, 0.5)
            old_vertices = tf.convert_to_tensor(
                remeshed_vertices, dtype=tf.float32)
            num_iterations = self.options["num_iterations"]
            for iteration in range(num_iterations):
                iteration_start_time = time.time()
                with tf.GradientTape() as tape:
                    # 获取新的点位置
                    features = model(mesh, in_features)
                    new_vertices = old_vertices + \
                        get_vertex_features(mesh, features)
                    #计算loss值
                    samples_num = int(
                        self.options["min_num_samples"]
                        + (iteration / self.options["num_iterations"])
                        * (self.options["max_num_samples"] - self.options["min_num_samples"])
                    )
                    surface_sample = mesh.surface_sample(
                        new_vertices, samples_num
                    )
                    loss = chamfer_loss(
                        surface_sample[0], point_cloud_tf, samples_num
                    )
                    converged = chamfer_convergence.step(loss.numpy().item())

                gradients = tape.gradient(loss, model.trainable_variables)
                optimizer.apply_gradients(
                    zip(gradients, model.trainable_variables))

                # 保存模型
                save_obj = self.options["obj_save_modulo"]
                if iteration % save_obj == 0 or converged or iteration == num_iterations - 1:
                    save_mesh(
                        f"tmp_{str(subdivision_level).zfill(2)}_{str(iteration).zfill(3)}.obj",
                        new_vertices.numpy(),
                        remeshed_faces,
                    )

                message = [
                    f"{subdivision_level+1}/{num_subdivisions} & {iteration+1}/{num_iterations}",
                    f"Chamfer Loss:{loss.numpy().item()}",
                    f"Time:{time.time()-iteration_start_time}"
                ]
                print(" ".join(message))

                # 画布替换
                o3d_mesh = o3d.geometry.TriangleMesh()
                o3d_mesh.vertices = o3d.utility.Vector3dVector(
                    new_vertices.numpy())
                o3d_mesh.triangles = o3d.utility.Vector3iVector(remeshed_faces)

                self._scene.scene.clear_geometry()
                self._scene.scene.add_geometry("__model__", o3d_mesh,
                                               self.settings.material)
                bounds = o3d_mesh.get_axis_aligned_bounding_box()
                self._scene.setup_camera(60, bounds, bounds.get_center())

                if converged:
                    print(
                        f"Converged at iteration {iteration + 1}/{num_iterations}."
                    )
                    break


def main():
    gui.Application.instance.initialize()
    MainWindow()
    gui.Application.instance.run()


if __name__ == '__main__':
    main()
