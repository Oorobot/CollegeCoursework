import os
import time

import numpy as np
import open3d as o3d
import open3d.visualization.gui as gui
import open3d.visualization.rendering as rendering
import tensorflow as tf
import trimesh
import threading

from .geometry_diy import Geometry_DIY
from .settings import Settings
from .tools import from_o3d_to_numpy, from_numpy_to_o3d, get_open3d_geometry_info
from ..script.tools import Obj, remesh
from ..model.Loss import ChamferLossLayer, ConvergenceDetector
from ..model.Mesh import Mesh
from ..model.PointToMeshModel import PointToMeshModel, get_vertex_features


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

    WINDOW_TRAIN = 1
    WINDOW_GEOMETRY = 2

    MENU_FILE = 21
    MENU_EXPORTFILE = 22
    MENU_ABOUT = 23
    MENU_CONVEX_HULL = 24
    MENU_REMESH = 25
    MENU_CLOSE_ALL = 26
    MENU_SHOW_TRAIN = 27
    MENU_SHOW_GEOMETRY = 28

    def __init__(self):
        # 渲染
        self.settings = Settings()
        # 参数
        self.options = None
        # 三维模型
        self.geometry = None
        self.train_geometry = None
        self.geometry_test = []
        # 消息面板-显示消息
        self.message = None
        # 训练状态：True-训练中，False-未训练或训练暂停
        self.train_status = False

        self.window = gui.Application.instance.create_window(
            "Point To Mesh", 1600, 800)

        em = self.window.theme.font_size

        # 菜单栏以及菜单设置
        if gui.Application.instance.menubar is None:
            menubar = gui.Menu()
            file_menu = gui.Menu()
            file_menu.add_item("improt Mesh", MainWindow.MENU_FILE)
            file_menu.add_item("export Mesh as ...",
                               MainWindow.MENU_EXPORTFILE)
            menubar.add_menu("File", file_menu)

            operation_menu = gui.Menu()
            operation_menu.add_item(
                "Create convex hull", MainWindow.MENU_CONVEX_HULL)
            operation_menu.add_item(
                "Remesh:Manifold and Simplify", MainWindow.MENU_REMESH)
            menubar.add_menu("Operations", operation_menu)

            view_menu = gui.Menu()
            view_menu.add_item("Close All Windows", MainWindow.MENU_CLOSE_ALL)
            view_menu.add_separator()
            view_menu.add_item("Show train panel", MainWindow.MENU_SHOW_TRAIN)
            view_menu.set_checked(MainWindow.MENU_SHOW_TRAIN, True)
            view_menu.add_item("Show Geometry panel",
                               MainWindow.MENU_SHOW_GEOMETRY)
            menubar.add_menu("View", view_menu)

            help_menu = gui.Menu()
            help_menu.add_item("About", MainWindow.MENU_ABOUT)
            menubar.add_menu("Help", help_menu)
            gui.Application.instance.menubar = menubar

        self.window.set_on_menu_item_activated(
            MainWindow.MENU_FILE, self._on_menu_file)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_EXPORTFILE, self._on_menu_export_file)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_CONVEX_HULL, self._on_menu_convex_hull)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_REMESH, self._on_menu_remesh)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_ABOUT, self._on_menu_about)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_CLOSE_ALL, self._on_menu_close_all)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SHOW_TRAIN, self._on_menu_show_train)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SHOW_GEOMETRY, self._on_menu_show_geometry)

        # 显示“三维模型”部分
        self.display_panel = gui.SceneWidget()
        self.display_panel.scene = rendering.Open3DScene(self.window.renderer)
        self.display_panel.scene.set_background([1, 1, 1, 1])
        self.display_panel.scene.scene.set_sun_light(
            [-1, -1, -1],  # direction
            [1, 1, 1],  # color
            100000)
        self.display_panel.scene.scene.enable_sun_light(True)

        # 训练模型操作面板
        self.options_panel = gui.Vert(
            0, gui.Margins(em*0.5, em*0.5, em*0.5, em*0.5))
        option_collapsablevert = gui.CollapsableVert(
            "Train Model", 0, gui.Margins(0.5*em, 0, 0.5*em, 0))
        option_collapsablevert.set_is_open(False)

        # 点云文件布局（选择点云文件）
        self._point_cloud = gui.TextEdit()
        point_cloud_button = gui.Button("...")
        point_cloud_button.horizontal_padding_em = 0.5
        point_cloud_button.vertical_padding_em = 0

        point_cloud_button.set_on_clicked(self._on_point_cloud_button)

        self.point_cloud_layout = gui.Horiz()
        self.point_cloud_layout.add_child(gui.Label("Point Cloud:"))
        self.point_cloud_layout.add_child(self._point_cloud)
        self.point_cloud_layout.add_child(point_cloud_button)

        option_collapsablevert.add_child(self.point_cloud_layout)

        # 训练结果布局（由选择点云文件位置产生）
        self._result_folder = gui.TextEdit()

        result_folder_layout = gui.Horiz()
        result_folder_layout.add_child(gui.Label("Result file Folder:"))
        result_folder_layout.add_child(self._result_folder)

        option_collapsablevert.add_child(result_folder_layout)

        # 初始网格布局（有->选择初始网格；无->略）
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

        # 参数布局1（基础）
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

        # 参数布局2（面数）
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

        # 参数布局3（保存结果）
        self._save_obj = gui.NumberEdit(gui.NumberEdit.INT)
        self._save_obj.int_value = Options["obj_save_modulo"]
        self._save_obj.set_limits(1, 100)

        save_parameters_layout = gui.Horiz()
        save_parameters_layout.add_child(gui.Label("how often to save:"))
        save_parameters_layout.add_child(self._save_obj)

        option_collapsablevert.add_child(save_parameters_layout)

        # 参数布局4（采样）
        self._min_samples_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._min_samples_num.int_value = Options["min_num_samples"]
        self._max_samples_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._max_samples_num.int_value = Options["max_num_samples"]

        option_collapsablevert.add_child(
            gui.Label("---range to lineralyinterp between when computing samples---"))
        min_sample_layout = gui.Horiz()
        min_sample_layout.add_child(
            gui.Label("The minimum number of samples:"))
        min_sample_layout.add_child(self._min_samples_num)
        max_sample_layout = gui.Horiz()
        max_sample_layout.add_child(
            gui.Label("The maximum number of samples:"))
        max_sample_layout.add_child(self._max_samples_num)

        option_collapsablevert.add_child(min_sample_layout)
        option_collapsablevert.add_child(max_sample_layout)

        # pooling

        self.options_panel.add_child(option_collapsablevert)

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

        self.options_panel.add_child(Train_button_layout)

        # 消息面板
        self.info_panel = gui.Horiz(em, gui.Margins(em, 0, em, 0))
        self.message_label = gui.Label("Welcome.")
        self.info_panel.add_child(self.message_label)

        # 三维模型面板
        self.geometry_panel = gui.Vert(
            0, gui.Margins(em*0.5, em*0.5, em*0.5, em*0.5))
        self.geometry_treeview = gui.TreeView()
        self.geometry_treeview.can_select_items_with_children = False
        self.geometry_panel.add_child(gui.Label("Geometries:"))
        self.geometry_treeview.set_on_selection_changed(self._on_tree)
        self.update_geometry_treeview(0)

        self.geometry_panel.add_child(self.geometry_treeview)

        self.window.set_on_layout(self._on_layout)
        self.window.add_child(self.display_panel)
        # self.window.add_child(self.options_panel)
        self.window.add_child(self.info_panel)
        self.window.add_child(self.geometry_panel)

    # 主窗口布局设定
    def _on_layout(self, theme):
        r = self.window.content_rect
        self.display_panel.frame = r
        # if gui.Application.instance.menubar.is_checked(MainWindow.MENU_SHOW_GEOMETRY):
        #     pass
        # if gui.Application.instance.menubar.is_checked(MainWindow.MENU_SHOW_TRAIN):
        #     width = min(
        #         r.width, self.options_panel.calc_preferred_size(theme).width)
        #     height = min(r.height,
        #                  self.options_panel.calc_preferred_size(theme).height)
        #     self.options_panel.frame = gui.Rect(
        #         r.get_right() - width, r.y, width, height)

        self.geometry_panel.frame = gui.Rect(
            r.get_right()*0.8, r.y, r.get_right()*0.2, r.height*0.33)

        self.info_panel.frame = gui.Rect(
            r.x, r.y+r.height -
            self.info_panel.calc_preferred_size(theme).height,
            r.width, self.info_panel.calc_preferred_size(theme).height
        )
    
    def _on_tree(self,id):
        print(id)
        print(self.geometry_treeview.selected_item)

    def update_geometry_treeview(self, type):
        if type == 0:
            for g in self.geometry_test:
                id = self.geometry_treeview.add_item(
                    self.geometry_treeview.get_root_item(), self.each_geometry_layout(g))
                g.id = id
        if type == 1:
            id = self.geometry_treeview.add_item(
                    self.geometry_treeview.get_root_item(), self.each_geometry_layout(self.geometry_test[-1]))
            self.geometry_test[-1].id = id

    def each_geometry_layout(self, geometry_diy):
        em = self.window.theme.font_size
        g_unit = gui.CollapsableVert(
            geometry_diy.name, 0, gui.Margins(0, 0, 0, 0))
        g_unit.set_is_open(False)
        vs, fs = get_open3d_geometry_info(geometry_diy.geometry)
        g_unit.add_child(gui.Label("vertices:"+str(vs)))
        g_unit.add_child(gui.Label("faces:"+str(fs)))
        return g_unit


    # 菜单栏 --> 载入文件
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
        self._print_message("[info] file: " + path)

    # 菜单栏 --> 关于
    def _on_menu_about(self):
        self.window.show_message_box(
            "About", "Welcome to Point2Mesh Model Visualization!!!")

    # 菜单栏 --> 保存文件
    def _on_menu_export_file(self):
        file_dialog = gui.FileDialog(
            gui.FileDialog.SAVE, "Choose File to open:", self.window.theme
        )
        file_dialog.add_filter(".obj", "Wavefront OBJ files (.obj)")
        file_dialog.add_filter(".off", "Object file format (.off)")
        file_dialog.add_filter(".ply", "Polygon files (.ply)")
        file_dialog.add_filter(".stl", "Stereolithography files (.stl)")
        file_dialog.add_filter(".fbx", "Autodesk Filmbox files (.fbx)")
        file_dialog.add_filter(".gltf", "OpenGL transfer files (.gltf)")
        file_dialog.add_filter(".glb", "OpenGL binary transfer files (.glb)")
        file_dialog.add_filter(".xyz", "ASCII point cloud files (.xyz)")
        file_dialog.add_filter(
            ".xyzn", "ASCII point cloud with normals (.xyzn)")
        file_dialog.add_filter(".xyzrgb",
                               "ASCII point cloud files with colors (.xyzrgb)")
        file_dialog.add_filter(".pcd", "Point Cloud Data files (.pcd)")
        file_dialog.add_filter(".pts", "3D Points files (.pts)")
        file_dialog.set_on_cancel(self._dialog_cancel)
        file_dialog.set_on_done(self._export_file_dialog_done)
        self.window.show_dialog(file_dialog)

    def _export_file_dialog_done(self, path):
        self.window.close_dialog()
        self.write(path)

    # 菜单栏 --> 创建凸包
    def _on_menu_convex_hull(self):
        if self.geometry is not None:
            convex_hull, _ = self.geometry.compute_convex_hull()
            self.display_panel.scene.add_geometry(
                "convex_hull", convex_hull, self.settings.material)
            vertices, faces = from_o3d_to_numpy(convex_hull)
            self._print_message("[INFO] already create convex_hull, vertices: " +
                                str(vertices.shape[0]) + ", faces: " + str(faces.shape[0]))
        else:
            self._print_message("[WARNING] There is no Mesh.")

    def _on_menu_remesh(self):
        self.show_remesh_dialog("remesh")

    # 菜单栏 --> 关闭子窗口
    def _on_menu_close_all(self):
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_TRAIN, False)
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_GEOMETRY, False)

    # 菜单栏 --> 显示训练面板
    def _on_menu_show_train(self):
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_TRAIN,
            not gui.Application.instance.menubar.is_checked(
                MainWindow.MENU_SHOW_TRAIN)
        )

    def _on_menu_show_geometry(self):
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_GEOMETRY,
            not gui.Application.instance.menubar.is_checked(
                MainWindow.MENU_SHOW_GEOMETRY)
        )

    # 训练面板 --> 添加点云文件按钮
    def _on_point_cloud_button(self):
        point_cloud_dialog = gui.FileDialog(
            gui.FileDialog.OPEN, "Select point cloud file:", self.window.theme)
        point_cloud_dialog.add_filter(
            ".txt .pwn", "fitted point cloud files(.txt .pwn)")
        point_cloud_dialog.add_filter(
            ".xyz .xyzn .xyzrgb .ply .pcd .pts",
            "Point cloud files (.xyz, .xyzn, .xyzrgb, .ply, "
            ".pcd, .pts)")
        point_cloud_dialog.set_on_cancel(self._dialog_cancel)
        point_cloud_dialog.set_on_done(self._on_p_c_d_done)
        self.window.show_dialog(point_cloud_dialog)

    def _on_p_c_d_done(self, path):
        self._point_cloud.text_value = path
        path_list = path.split("/")
        path_list.pop()
        path_list.append("results")
        result_path = "/".join(path_list)
        self._result_folder.text_value = result_path

        self.window.close_dialog()
        self.load_other_format_pc(path)

    # 训练面板 --> 有无初始网格
    def _on_initial_mesh_status(self, is_checked):
        if is_checked:
            self.initial_mesh_layout.visible = True
        else:
            self.initial_mesh_layout.visible = False

    # 训练面板 --> 添加初始网格按钮
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

    # 训练面板 --> 训练按钮
    def _on_train(self):
        self.options = Options.copy()
        self.options["num_subdivisions"] = self._num_epoch.int_value
        self.options["num_iterations"] = self._num_iterations.int_value
        self.options["subdivision_multiplier"] = self._epoch_multiplier.double_value
        self.options["initial_num_faces"] = self._initial_faces_num.int_value
        self.options["max_num_faces"] = self._max_faces_num.int_value
        self.options["obj_save_modulo"] = self._save_obj.int_value
        self.options["min_num_samples"] = self._min_samples_num.int_value
        self.options["max_num_samples"] = self._max_samples_num.int_value
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

    # 训练面板 --> 暂停按钮
    def _on_stop(self):
        if self.train_status:
            self.stop_button.text = "Continue"
            self.train_status = False
        else:
            self.stop_button.text = "Stop"
            self.train_status = True
            self._print_message("recovery the training.")

    def load(self, path):
        # self.display_panel.scene.clear_geometry()
        geometry = None
        geometry_type = o3d.io.read_file_geometry_type(path)
        geometry_name = os.path.basename(path).split(".")[0]
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
                self.display_panel.scene.add_geometry(
                    geometry_name, geometry, self.settings.material)
                bounds = geometry.get_axis_aligned_bounding_box()
                self.display_panel.setup_camera(
                    60, bounds, bounds.get_center())
                self.geometry_test.append(
                    Geometry_DIY(-1, geometry_name, geometry))
                self.update_geometry_treeview(1)
                print(self.geometry_test)
            except Exception as e:
                print(e)

    def load_other_format_pc(self, path):
        self.display_panel.scene.clear_geometry()
        suffix = path.split(".")[1]
        self.geometry = None
        try:
            if suffix == "txt" or suffix == "pwn":
                self.geometry = o3d.io.read_point_cloud(path, format='xyz')
            else:
                self.geometry = o3d.io.read_point_cloud(path)
        except Exception:
            pass
        if self.geometry is not None:
            self._print_message("[Info] Successfully read " + path)
            print("[Info] Successfully read", path)
            try:
                self.display_panel.scene.add_geometry(
                    "__cloud__", self.geometry, self.settings.material)
                bounds = self.geometry.get_axis_aligned_bounding_box()
                self.display_panel.setup_camera(
                    60, bounds, bounds.get_center())
            except Exception as e:
                print(e)
        else:
            self._print_message("[WARNING] Failed to read points " + path)
            print("[WARNING] Failed to read points", path)

    def write(self, path):
        suffix = path.split(".")[1]
        point_cloud_file_extension_name = [
            "xyz", "xyzn", "xyzrgb", "ply", "pcd", "pts"]
        mesh_file_extension_name = [
            "ply", "stl", "fbx", "obj", "off", "gltf", "glb"]
        if self.geometry is None:
            self._print_message("[WARNING] There is no Mesh.")
        else:
            vertices, faces = from_o3d_to_numpy(self.geometry)
            if faces is None:
                if suffix in point_cloud_file_extension_name:
                    try:
                        o3d.io.write_point_cloud(
                            path, self.geometry, write_ascii=True)
                    except Exception as e:
                        print(e)
                else:
                    faces = np.array([]).reshape(-1, 3)
                    try:
                        o3d.io.write_triangle_mesh(
                            path, from_numpy_to_o3d(vertices, faces), write_ascii=True)
                    except Exception as e:
                        print(e)
            else:
                if suffix in mesh_file_extension_name:
                    try:
                        o3d.io.write_triangle_mesh(
                            path, self.geometry, write_ascii=True)
                    except Exception as e:
                        print(e)
                else:
                    try:
                        o3d.io.write_point_cloud(
                            path, from_numpy_to_o3d(vertices, None), write_ascii=True)
                    except Exception as e:
                        print(e)
            self._print_message("[Info] file is saved at: " + path)

    def train_model(self):
        point_cloud = np.loadtxt(
            fname=self.options["point_cloud"], usecols=(0, 1, 2))
        point_cloud_tf = tf.convert_to_tensor(point_cloud, dtype=tf.float32)

        def save_mesh(filename, vertices, faces):
            Obj.save(os.path.join(
                self.options["save_location"], filename), vertices, faces)

        def print_message(message: str):
            self.message = message
            print(self.message)
            gui.Application.instance.post_to_main_thread(
                self.window, self._print_message_on_child_thread)

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

                print_message(f"Remeshing to {int(new_face_num)} faces")

                remeshed_vertices, remeshed_faces = remesh(
                    new_vertices.numpy(), remeshed_faces, new_face_num
                )
            else:
                print_message(f"Starting with {remeshed_faces.shape[0]} faces")

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
                if self.train_status:
                    print_message(" ".join(message))
                else:
                    print_message(" ".join(message) +
                                  " [INFO] already stop the training")

                # 画布替换
                self.geometry = from_numpy_to_o3d(
                    new_vertices.numpy(), remeshed_faces)
                gui.Application.instance.post_to_main_thread(
                    self.window, self._change_scene_on_child_thread)

                # 暂停训练
                while not self.train_status:
                    time.sleep(1)

                if converged:
                    print_message(
                        f"Converged at iteration {iteration + 1}/{num_iterations}.")
                    break

        print_message("Done")
        self.train_status = False
        self.Train_button.enabled = True
        self.stop_button.visible = False

    def _change_scene_on_child_thread(self):
        self.display_panel.scene.clear_geometry()
        self.display_panel.scene.add_geometry(
            "__model__", self.geometry, self.settings.material)

    # 消息面板
    def _print_message_on_child_thread(self):
        self.message_label.text = self.message

    def _print_message(self, message: str):
        self.message_label.text = message

    # 校验训练参数
    def _check_train_parameters(self) -> bool:
        errors = []
        if self.options["point_cloud"] == None or len(self.options["point_cloud"]) == 0:
            errors.append("point cloud is empty.")
        if self.options["num_subdivisions"] <= 0:
            errors.append("num of epochs must > 0.")
        if self.options["num_iterations"] <= 0:
            errors.append("num of iterations must > 0.")
        if self.options["subdivision_multiplier"] <= 1:
            errors.append(
                "the multiple of face number of two adjacent epoch must > 1.0.")
        if self.options["initial_num_faces"] < 1000:
            errors.append("suggest The initial face number > 1000.")
        if self.options["max_num_faces"] < self.options["initial_num_faces"]:
            errors.append(
                "The maximum face number must > the initial face number.")
        if self.options["obj_save_modulo"] <= 0:
            errors.append("the number how often to save must > 0")
        if self.options["min_num_samples"] <= 0:
            errors.append("The minimum number of samples must > 0. ")
        if self.options["max_num_samples"] <= self.options["min_num_samples"]:
            errors.append(
                "The maxinum number of samples must > The mininum one")

        if len(errors) == 0:
            return True
        else:
            self.show_message_box("errors", errors)
            return False

    def show_message_box(self, title: str, message: list):
        message_box = gui.Dialog(title)
        em = self.window.theme.font_size
        message_box_layout = gui.Vert(0, gui.Margins(em, 0.5*em, em, 0.5*em))
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

    def show_remesh_dialog(self, title: str):
        remesh_dialog = gui.Dialog(title)
        em = self.window.theme.font_size
        layout = gui.Vert(0, gui.Margins(0.2*em, 0.5*em, 0.2*em, 0.5*em))
        layout.add_child(
            gui.Label("take a triangle mesh and generate a manifold mesh."))
        layout.add_child(
            gui.Label("Then,for efficiency purpose,simplify the mesh."))

        self._remesh_face_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._remesh_face_num.int_value = 2000
        face_num_layout = gui.Horiz(0, gui.Margins(0, 0.5*em, 0, 0.5*em))
        face_num_layout.add_child(gui.Label("simplified mesh's face number:"))
        face_num_layout.add_child(self._remesh_face_num)
        layout.add_child(face_num_layout)

        button = gui.Button("apply")
        button.set_on_clicked(self._remesh)
        button_layout = gui.Horiz()
        button_layout.add_stretch()
        button_layout.add_child(button)
        layout.add_child(button)

        remesh_dialog.add_child(layout)
        self.window.show_dialog(remesh_dialog)

    def _remesh(self):
        self._dialog_cancel()
        if self.geometry is None or self.geometry.get_geometry_type() == o3d.geometry.Geometry.PointCloud:
            self._print_message(
                "[WARNING] There is no mesh or the geometry have no face.")
        else:
            vertices, faces = from_o3d_to_numpy(self.geometry)
            new_vertices, new_faces = remesh(
                vertices, faces, self._remesh_face_num.int_value)
            self.geometry = from_numpy_to_o3d(new_vertices, new_faces)
            self._change_scene_on_child_thread()
            self._print_message("[INFO] already remesh, vertices: " +
                                str(new_vertices.shape[0]) + ", faces: " + str(new_faces.shape[0]))


def main():
    gui.Application.instance.initialize()
    MainWindow()
    gui.Application.instance.run()


if __name__ == '__main__':
    main()
