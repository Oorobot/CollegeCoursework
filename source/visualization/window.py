import os
import time

import numpy as np
import open3d as o3d
import open3d.visualization.gui as gui
import open3d.visualization.rendering as rendering
import tensorflow as tf
import trimesh
import threading

from .settings import Settings
from ..script.tools import Obj, remesh
from ..model.Loss import ChamferLossLayer, ConvergenceDetector
from ..model.Mesh import Mesh
from ..model.PointToMeshModel import PointToMeshModel, get_vertex_features

# 训练参数
Options = {
    # 点云文件
    "point_cloud": None,
    # 结果保存位置
    "save_location": None,
    # 初始网格模型（可选）
    "initial_mesh": None,

    # 训练轮次 或 网格细分等级
    "num_subdivisions": 6,
    # 每轮或每个等级迭代次数
    "num_iterations": 1000,
    # 相邻训练轮次之间网格面的倍数
    "subdivision_multiplier": 1.5,

    # 初始网格的面数
    "initial_num_faces": 1000,
    # 最大的网格模型面数
    "max_num_faces": 10000,

    # 使用beamgap的频率（使用次数/每轮的迭代数），-1表示不使用
    "beamgap_modulo": -1,
    # 保存结果的频率（保存次数/每轮迭代数）
    "obj_save_modulo": 5,

    # 对模型进行采样的数量（与当前迭代数有关、线性关系）
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
        self.geometry = None
        self.message = None
        self.train_status = False

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

        # 训练模型参数设置
        self.control_layout = gui.Vert(
            0, gui.Margins(em*0.5, em*0.5, em*0.5, em*0.5))

        option_collapsablevert = gui.CollapsableVert(
            "Train Model", 0, gui.Margins(0.5*em, 0, 0.5*em, 0))
        option_collapsablevert.set_is_open(False)

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

        option_collapsablevert.add_child(point_cloud_layout)

        # 训练结果存放的文件夹
        self._result_folder = gui.TextEdit()
        result_folder_layout = gui.Horiz()

        result_folder_layout.add_child(gui.Label("Result file Folder:"))
        result_folder_layout.add_child(self._result_folder)

        option_collapsablevert.add_child(result_folder_layout)

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

        option_collapsablevert.add_child(self.initial_mesh_status)
        option_collapsablevert.add_child(self.initial_mesh_layout)

        # 详细的参数设置

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
        self.Train_button = gui.Button("Train")
        self.Train_button.set_on_clicked(self._on_train)
        self.stop_button = gui.Button("Stop")
        self.stop_button.visible = False
        self.stop_button.set_on_clicked(self._on_stop)
        Train_button_layout.add_stretch()
        Train_button_layout.add_child(self.stop_button)
        Train_button_layout.add_child(self.Train_button)

        self.control_layout.add_child(Train_button_layout)

        # 消息框
        self.message_layout = gui.Horiz(em, gui.Margins(em, 0, em, 0))
        self.message_label = gui.Label("Welcome.")
        self.message_layout.add_child(self.message_label)

        self.window.set_on_layout(self._on_layout)
        self.window.add_child(self._scene)
        self.window.add_child(self.control_layout)
        self.window.add_child(self.message_layout)

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
        self.message_layout.frame = gui.Rect(
            r.x, r.y+r.height -
            self.message_layout.calc_preferred_size(theme).height,
            r.width, self.message_layout.calc_preferred_size(theme).height
        )

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
        self.message_label.text = "file: " + path

    # 菜单栏-关于
    def _on_menu_about(self):
        self.window.show_message_box(
            "About", "Point2Mesh Model Visualization!!!")

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
        self.message_label.text = "showing point cloud."

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

        if self._check_train_parameters():
            self.train_status = True
            self.stop_button.visible = True
            self.Train_button.enabled = False
            threading.Thread(target=self.train_model).start()

    def _on_stop(self):
        if self.train_status:
            self.stop_button.text = "Continue"
            self.train_status = False
        else:
            self.stop_button.text = "Stop"
            self.train_status = True
            self.message_label.text = "recovery the training."

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
        # GPU.
        physical_devices = tf.config.list_physical_devices("GPU")
        if len(physical_devices) > 0:
            tf.config.experimental.set_memory_growth(physical_devices[0], True)

        point_cloud = np.loadtxt(
            fname=self.options["point_cloud"], usecols=(0, 1, 2))
        point_cloud_tf = tf.convert_to_tensor(point_cloud, dtype=tf.float32)

        def save_mesh(filename, vertices, faces):
            Obj.save(os.path.join(
                self.options["save_location"], filename), vertices, faces)

        # 初始网格
        if self.options["initial_mesh"]:
            remeshed_vertices, remeshed_faces = Obj.load(
                self.options["initial_mesh"])
        else:
            convex_hull = trimesh.convex.convex_hull(point_cloud)
            remeshed_vertices, remeshed_faces = remesh(
                convex_hull.vertices, convex_hull.faces, self.options["initial_num_faces"]
            )
        save_mesh("tmp_initial_mesh.obj", remeshed_vertices, remeshed_faces)

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

                self.message = f"Remeshing to {int(new_face_num)} faces"
                print(self.message)
                gui.Application.instance.post_to_main_thread(
                    self.window, self._message_label_change)

                remeshed_vertices, remeshed_faces = remesh(
                    new_vertices.numpy(), remeshed_faces, new_face_num
                )
            else:
                self.message = f"Starting with {remeshed_faces.shape[0]} faces"
                print(self.message)
                gui.Application.instance.post_to_main_thread(
                    self.window, self._message_label_change)

            mesh = Mesh(remeshed_vertices, remeshed_faces)
            model = PointToMeshModel(
                mesh.edges.shape[0], self.options["pooling"])

            # 随机特征值
            in_features = tf.random.uniform(
                (mesh.edges.shape[0], 6), -0.5, 0.5)

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
                    surface_sample = mesh.sample_surface(
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
                self.message = " ".join(message)
                print(self.message)
                if not self.train_status:
                    self.message += "  already stop the training."
                gui.Application.instance.post_to_main_thread(
                    self.window, self._message_label_change)

                # 画布替换
                o3d_mesh = o3d.geometry.TriangleMesh()
                o3d_mesh.vertices = o3d.utility.Vector3dVector(
                    new_vertices.numpy())
                o3d_mesh.triangles = o3d.utility.Vector3iVector(remeshed_faces)

                self.geometry = o3d_mesh
                gui.Application.instance.post_to_main_thread(
                    self.window, self.on_train_change_scene)

                # 暂停训练
                while not self.train_status:
                    pass

                if converged:
                    self.message = f"Converged at iteration {iteration + 1}/{num_iterations}."
                    print(self.message)
                    gui.Application.instance.post_to_main_thread(
                        self.window, self._message_label_change)
                    break

        self.message = "Done"
        print(self.message)
        gui.Application.instance.post_to_main_thread(
            self.window, self._message_label_change)
        self.train_status = False
        self.Train_button.enabled = True
        self.stop_button.visible = False

    def on_train_change_scene(self):
        self._scene.scene.clear_geometry()
        self._scene.scene.add_geometry("__model__", self.geometry,
                                       self.settings.material)
        # bounds = self.geometry.get_axis_aligned_bounding_box()
        # self._scene.setup_camera(60, bounds, bounds.get_center())

    def _message_label_change(self):
        self.message_label.text = self.message

    def _check_train_parameters(self) -> bool:
        errors = []
        if self.options["point_cloud"] == None or len(self.options["point_cloud"]) == 0:
            errors.append("point cloud is empty.")
        if self.options["num_subdivisions"] <= 0:
            errors.append("num of epochs need > 0.")
        if self.options["num_iterations"] <= 0:
            errors.append("num of iterations need > 0.")
        if self.options["subdivision_multiplier"] <= 1:
            errors.append(
                "the multiple of face number of two adjacent epoch need > 1.0.")
        if self.options["initial_num_faces"] < 1000:
            errors.append("suggest The initial face number > 1000.")
        if self.options["max_num_faces"] < self.options["initial_num_faces"]:
            errors.append(
                "The maximum face number need greater than initial_num_faces.")
        if self.options["obj_save_modulo"] <= 0:
            errors.append("the number how often to save need > 0")

        if len(errors) == 0:
            return True
        else:
            self.show_message_box("errors", errors)
            return False

    def show_message_box(self, title: str, message: list):
        message_box = gui.Dialog(title)
        em = self.window.theme.font_size
        message_box_layout = gui.Vert(em, gui.Margins(em, em, em, em))
        for m in message:
            message_box_layout.add_child(gui.Label(m))
        ok_button = gui.Button("OK")
        ok_button.set_on_clicked(self._dialog_cancel)
        button_layout = gui.Horiz()
        button_layout.add_stretch()
        button_layout.add_child(ok_button)

        message_box_layout.add_child(button_layout)

        message_box.add_child(message_box_layout)
        self.window.show_dialog(message_box)


def main():
    gui.Application.instance.initialize()
    MainWindow()
    gui.Application.instance.run()


if __name__ == '__main__':
    main()
