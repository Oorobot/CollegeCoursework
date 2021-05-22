import threading
import time
from typing import Tuple

import numpy as np
import open3d as o3d
import open3d.visualization.gui as gui
import open3d.visualization.rendering as rendering
import tensorflow as tf
import trimesh

from ..model.Loss import ChamferLossLayer, ConvergenceDetector
from ..model.Mesh import Mesh
from ..model.PointToMeshModel import PointToMeshModel, get_vertex_features
from ..script.tools import remesh
from .geometry import GeometryInfo, GeometryInfos
from .options import Options
from .settings import Settings


class MainWindow:

    MENU_ABOUT = 1

    MENU_SHOW_NONE = 2
    MENU_SHOW_TRAIN = 3
    MENU_SHOW_GEOMETRY = 4

    MENU_IMPORT_FILE = 5
    MENU_EXPORT_FILE = 6

    MENU_CONVEX_HULL = 7
    MENU_TRANSFORMATION = 8
    MENU_SUBDIVISION_MIDPOINT = 9
    MENU_SUBDIVISION_LOOP = 10
    MENU_SMOOTH_HUMPHREY = 11
    MENU_SMOOTH_LAPLACIAN = 12
    MENU_SMOOTH_TAUBIN = 13
    MENU_REMESH = 14
    MENU_SIMPLIFICATION_AVERAGE = 15
    MENU_SIMPLIFICATION_QUADRIC_DECIMATION = 16

    STATE_NONE = 101
    STATE_CLOUD = 102
    STATE_MESH = 103
    STATE_TRAIN = 104

    def __init__(self):

        # 渲染
        self.settings = Settings()

        # 训练参数
        self.options = Options()

        # 三维模型
        self.cloud = None  # 训练的点云
        self.geometry_infos = GeometryInfos()

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
            file_menu.add_item("improt Mesh", MainWindow.MENU_IMPORT_FILE)

            file_menu.add_item("export Mesh as ...",
                               MainWindow.MENU_EXPORT_FILE)
            menubar.add_menu("File", file_menu)

            operation_menu = gui.Menu()
            operation_menu.add_item(
                "Create convex hull", MainWindow.MENU_CONVEX_HULL)
            operation_menu.add_item(
                "Translate/Rotation/Scale", MainWindow.MENU_TRANSFORMATION)
            mesh_menu = gui.Menu()
            mesh_menu.add_item(
                "Subdivision: Midpoint", MainWindow.MENU_SUBDIVISION_MIDPOINT)
            # mesh_menu.add_item(
            #     "Subdivision: Loop", MainWindow.MENU_SUBDIVISION_LOOP)
            mesh_menu.add_separator()
            mesh_menu.add_item(
                "Smoothing: Humphrey", MainWindow.MENU_SMOOTH_HUMPHREY)
            mesh_menu.add_item(
                "Smoothing: Laplacian", MainWindow.MENU_SMOOTH_LAPLACIAN)
            mesh_menu.add_item(
                "Smoothing: Taubin", MainWindow.MENU_SMOOTH_TAUBIN)
            mesh_menu.add_separator()
            mesh_menu.add_item(
                "Remesh:Manifold and Simplify", MainWindow.MENU_REMESH)
            mesh_menu.add_item(
                "Simplication: Average", MainWindow.MENU_SIMPLIFICATION_AVERAGE)
            mesh_menu.add_item(
                "Simplication: Quadric Error Metric Decimation",
                MainWindow.MENU_SIMPLIFICATION_QUADRIC_DECIMATION)

            operation_menu.add_menu("mesh layer", mesh_menu)
            menubar.add_menu("Operations", operation_menu)

            view_menu = gui.Menu()
            view_menu.add_item("Close All Windows", MainWindow.MENU_SHOW_NONE)
            view_menu.add_separator()
            view_menu.add_item("Show train panel", MainWindow.MENU_SHOW_TRAIN)
            view_menu.set_checked(MainWindow.MENU_SHOW_TRAIN, True)
            view_menu.add_item("Show Geometry panel",
                               MainWindow.MENU_SHOW_GEOMETRY)
            view_menu.set_checked(MainWindow.MENU_SHOW_GEOMETRY, True)
            menubar.add_menu("View", view_menu)

            help_menu = gui.Menu()
            help_menu.add_item("About", MainWindow.MENU_ABOUT)
            menubar.add_menu("Help", help_menu)
            gui.Application.instance.menubar = menubar

        # gui.Application.instance.menubar.set_enabled(1, False)

        self.window.set_on_menu_item_activated(
            MainWindow.MENU_IMPORT_FILE, self._on_menu_file)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_EXPORT_FILE, self._on_menu_export_file)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_CONVEX_HULL, self._on_menu_convex_hull)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_TRANSFORMATION, self._on_menu_transformation)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_REMESH, self._on_menu_remesh)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_ABOUT, self._on_menu_about)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SHOW_NONE, self._on_menu_show_none)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SHOW_TRAIN, self._on_menu_show_train)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SHOW_GEOMETRY, self._on_menu_show_geometry)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SUBDIVISION_MIDPOINT, self._on_menu_subdivision_midpoint)
        # self.window.set_on_menu_item_activated(
        #     MainWindow.MENU_SUBDIVISION_LOOP, self._on_menu_subdivision_loop)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SMOOTH_HUMPHREY, self._on_menu_smooth_humphrey)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SMOOTH_LAPLACIAN, self._on_menu_smooth_laplacian)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SMOOTH_TAUBIN, self._on_menu_smooth_taubin)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SIMPLIFICATION_AVERAGE, self._on_menu_simplification_average)
        self.window.set_on_menu_item_activated(
            MainWindow.MENU_SIMPLIFICATION_QUADRIC_DECIMATION, self._on_menu_simplification_quadric_decimation)

        # 显示“三维模型”部分
        self.scene_module = gui.SceneWidget()
        self.scene_module.scene = rendering.Open3DScene(self.window.renderer)
        self.scene_module.scene.set_background([1, 1, 1, 1])
        self.scene_module.scene.scene.set_sun_light(
            [-1, -1, -1],  # direction
            [1, 1, 1],  # color
            100000)
        self.scene_module.scene.scene.enable_sun_light(True)
        self.scene_module.scene.show_axes(True)

        # 训练模型操作面板
        self.options_module = gui.Vert(
            0, gui.Margins(em*0.5, em*0.5, em*0.5, em*0.5))
        option_collapsablevert = gui.CollapsableVert(
            "Train Model", 0, gui.Margins(0.5*em, 0, 0.5*em, 0))
        option_collapsablevert.set_is_open(False)

        # 点云文件布局（选择点云文件）
        self._point_cloud = gui.TextEdit()
        self.point_cloud_button = gui.Button("...")
        self.point_cloud_button.horizontal_padding_em = 0.5
        self.point_cloud_button.vertical_padding_em = 0

        self.point_cloud_button.set_on_clicked(self._on_point_cloud_button)

        self.point_cloud_layout = gui.Horiz()
        self.point_cloud_layout.add_child(gui.Label("Point Cloud:"))
        self.point_cloud_layout.add_child(self._point_cloud)
        self.point_cloud_layout.add_child(self.point_cloud_button)

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
        self.initial_mesh_button = gui.Button("...")
        self.initial_mesh_button.horizontal_padding_em = 0.5
        self.initial_mesh_button.vertical_padding_em = 0
        self.initial_mesh_button.set_on_clicked(self._on_initial_mesh_button)

        self.initial_mesh_layout = gui.Horiz()
        self.initial_mesh_layout.add_child(gui.Label("initial mesh:"))
        self.initial_mesh_layout.add_child(self._initial_mesh)
        self.initial_mesh_layout.add_child(self.initial_mesh_button)

        self.initial_mesh_layout.visible = False

        option_collapsablevert.add_child(self.initial_mesh_status)
        option_collapsablevert.add_child(self.initial_mesh_layout)

        # 参数布局1（基础）
        self._num_epoch = gui.NumberEdit(gui.NumberEdit.INT)
        self._num_epoch.int_value = self.options.num_subdivisions
        self._num_epoch.set_limits(1, 99)

        self._num_iterations = gui.NumberEdit(gui.NumberEdit.INT)
        self._num_iterations.int_value = self.options.num_iterations
        self._num_iterations.set_limits(100, 10000)

        self._epoch_multiplier = gui.NumberEdit(gui.NumberEdit.DOUBLE)
        self._epoch_multiplier.double_value = self.options.subdivision_multiplier
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
        self._initial_faces_num.int_value = self.options.initial_num_faces
        self._initial_faces_num.set_limits(500, 5000)
        self._max_faces_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._max_faces_num.int_value = self.options.max_num_faces
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
        self._save_obj.int_value = self.options.obj_save_modulo
        self._save_obj.set_limits(1, 100)

        save_parameters_layout = gui.Horiz()
        save_parameters_layout.add_child(gui.Label("how often to save:"))
        save_parameters_layout.add_child(self._save_obj)

        option_collapsablevert.add_child(save_parameters_layout)

        # 参数布局4（采样）
        self._min_samples_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._min_samples_num.int_value = self.options.min_num_samples
        self._max_samples_num = gui.NumberEdit(gui.NumberEdit.INT)
        self._max_samples_num.int_value = self.options.max_num_samples

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
        self.poolings = []
        self.poolings_status = gui.Checkbox(
            "Use mesh pool.(value in descending order between 0 and 1)")
        self.poolings_status.set_on_checked(self._on_poolings_status)
        option_collapsablevert.add_child(self.poolings_status)

        self.poolings_layout = gui.Horiz()
        self.poolings_layout.add_child(gui.Label("pooling:"))
        for i in range(0, 6):
            pooling = gui.NumberEdit(gui.NumberEdit.DOUBLE)
            pooling.set_preferred_width(2*em)
            pooling.decimal_precision = 1
            self.poolings_layout.add_child(pooling)
            self.poolings_layout.add_fixed(0.33*em)
            self.poolings.append(pooling)
        self.poolings_layout.visible = False
        option_collapsablevert.add_child(self.poolings_layout)

        self.options_module.add_child(option_collapsablevert)

        # “训练”按钮
        Train_button_layout = gui.Horiz()
        self.train_button = gui.Button("Train")
        self.train_button.horizontal_padding_em = 0.5
        self.train_button.vertical_padding_em = 0
        self.train_button.set_on_clicked(self._on_train)
        self.stop_button = gui.Button("Stop")
        self.stop_button.horizontal_padding_em = 0.5
        self.stop_button.vertical_padding_em = 0
        self.stop_button.visible = False
        self.stop_button.set_on_clicked(self._on_stop)
        Train_button_layout.add_stretch()
        Train_button_layout.add_child(self.stop_button)
        Train_button_layout.add_child(self.train_button)

        self.options_module.add_child(Train_button_layout)

        # 消息面板
        self.info_panel = gui.Horiz(0, gui.Margins(em, 0, em, 0))
        self.message_label = gui.Label("Welcome.")
        self.info_panel.add_child(self.message_label)

        # 三维模型面板
        self.geometry_panel = gui.Vert(
            0, gui.Margins(0.1*em, 0.33*em, 0.1*em, 0))

        self.line_button = gui.Button("line:show")
        self.line_button.horizontal_padding_em = 0.5
        self.line_button.vertical_padding_em = 0
        self.line_button.set_on_clicked(self._on_line)
        self.point_button = gui.Button("point:show")
        self.point_button.horizontal_padding_em = 0.5
        self.point_button.vertical_padding_em = 0
        self.point_button.set_on_clicked(self._on_point)

        show_type_horiz = gui.Horiz(0, gui.Margins(0, 0.1*em, 0, 0))
        show_type_horiz.add_stretch()
        show_type_horiz.add_child(self.line_button)
        show_type_horiz.add_child(gui.Label("   "))
        show_type_horiz.add_child(self.point_button)

        self.geometry_panel.add_child(show_type_horiz)

        self.geometry_treeview = gui.TreeView()
        self.geometry_treeview.can_select_items_with_children = False
        self.geometry_treeview.set_on_selection_changed(self._on_tree)

        self.geometry_panel.add_child(self.geometry_treeview)

        self.show_hide_button = gui.Button("hide")
        self.show_hide_button.horizontal_padding_em = 0.5
        self.show_hide_button.vertical_padding_em = 0
        self.show_hide_button.set_on_clicked(self._on_show_hide)

        self.delete_button = gui.Button("delete")
        self.delete_button.horizontal_padding_em = 0.5
        self.delete_button.vertical_padding_em = 0
        self.delete_button.set_on_clicked(self._on_delete)

        geometry_button_horiz = gui.Horiz(
            0, gui.Margins(0, 0.1*em, 0, 0))
        geometry_button_horiz.add_stretch()
        geometry_button_horiz.add_child(self.show_hide_button)
        geometry_button_horiz.add_stretch()
        geometry_button_horiz.add_child(self.delete_button)
        geometry_button_horiz.add_stretch()

        self.geometry_panel.add_child(geometry_button_horiz)

        self.window.set_on_layout(self._on_layout)
        self.window.add_child(self.scene_module)
        self.window.add_child(self.geometry_panel)
        self.window.add_child(self.options_module)
        self.window.add_child(self.info_panel)

        self.state_management(MainWindow.STATE_NONE)

    """
    布局
    """

    def _on_layout(self, theme):
        r = self.window.content_rect
        self.scene_module.frame = r
        panel_show = []
        panel_show.append(gui.Application.instance.menubar.is_checked(
            MainWindow.MENU_SHOW_GEOMETRY))
        panel_show.append(gui.Application.instance.menubar.is_checked(
            MainWindow.MENU_SHOW_TRAIN))
        if panel_show[0] and panel_show[1]:
            width = min(
                r.width, self.options_module.calc_preferred_size(theme).width)
            height = min(r.height,
                         self.options_module.calc_preferred_size(theme).height)
            self.geometry_panel.frame = gui.Rect(
                r.get_right() - width, r.y, width, r.height*0.33)
            self.options_module.frame = gui.Rect(
                r.get_right() - width, r.y + r.height*0.33, width, height)
        elif panel_show[0]:
            self.geometry_panel.frame = gui.Rect(
                r.get_right()*0.8, r.y, r.get_right()*0.2, r.height*0.33)
            self.options_module.frame = gui.Rect(
                r.x + r.width, r.y + r.height, 0, 0)
        elif panel_show[1]:
            width = min(
                r.width, self.options_module.calc_preferred_size(theme).width)
            height = min(
                r.height, self.options_module.calc_preferred_size(theme).height)
            self.geometry_panel.frame = gui.Rect(
                r.x + r.width, r.y + r.height, 0, 0)
            self.options_module.frame = gui.Rect(
                r.get_right() - width, r.y, width, height)
        else:
            self.geometry_panel.frame = gui.Rect(
                r.x + r.width, r.y + r.height, 0, 0)
            self.options_module.frame = gui.Rect(
                r.x + r.width, r.y + r.height, 0, 0)

        self.info_panel.frame = gui.Rect(
            r.x,
            r.y + r.height - self.info_panel.calc_preferred_size(theme).height,
            r.width, self.info_panel.calc_preferred_size(theme).height
        )

    """
    菜单栏功能函数
    """

    # 关于
    def _on_menu_about(self):
        self.window.show_message_box(
            "About", "Welcome to Point2Mesh Model Visualization!!!")

    # 载入文件
    def _on_menu_file(self):
        file_dialog = gui.FileDialog(
            gui.FileDialog.OPEN, "Choose File to open:", self.window.theme
        )
        file_dialog.add_filter(
            ".ply .stl .fbx .obj .off .gltf .glb .xyz .xyzn .xyzrgb .ply .pcd .pts",
            "Mesh or Point cloud (.ply, .stl, .fbx, .obj, .off, "
            ".gltf, .glb .xyz .xyzn .xyzrgb .ply .pcd .pts)")
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

    # 保存文件
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

    def _dialog_cancel(self):
        self.window.close_dialog()

    def _file_dialog_done(self, path):
        self.window.close_dialog()
        self.load(path)

    def _export_file_dialog_done(self, path):
        self.window.close_dialog()
        self.save(path)

    def load(self, path: str):
        # 训练数据-点云
        pc = False
        if path.endswith("__train__"):
            path = path[:-9]
            pc = True

        self.add_geometry_widget_and_others(path=path, setup_camera=True)
        temp = GeometryInfo(path)
        if temp.geometry[0] is not None:
            if pc:
                self.cloud, _ = GeometryInfo.o3d_to_numpy(temp.geometry[0])
                self.cloud = self.cloud.tolist()
            self._print_message("[Info] Successfully read " + path)
        else:
            self._print_message("[ERROR] Failure to read " + path)

    def save(self, path: str):
        id = self.geometry_treeview.selected_item
        temp = self.geometry_infos.get(id)
        if temp.save(path):
            self._print_message("[Info] Successfully save "+path)
        else:
            self._print_message("[ERROR] Failure to save "+path)


    # 关闭子窗口
    def _on_menu_show_none(self):
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_TRAIN, False)
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_GEOMETRY, False)
        self.window.set_needs_layout()

    # 显示训练面板
    def _on_menu_show_train(self):
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_TRAIN,
            not gui.Application.instance.menubar.is_checked(
                MainWindow.MENU_SHOW_TRAIN)
        )
        self.window.set_needs_layout()

    # 显示模型面板
    def _on_menu_show_geometry(self):
        gui.Application.instance.menubar.set_checked(
            MainWindow.MENU_SHOW_GEOMETRY,
            not gui.Application.instance.menubar.is_checked(
                MainWindow.MENU_SHOW_GEOMETRY)
        )
        self.window.set_needs_layout()

    # 创建凸包
    def _on_menu_convex_hull(self):
        id = self.geometry_treeview.selected_item
        geometry_info = self.geometry_infos.get(id)

        convex_hull, _ = geometry_info.geometry[0].compute_convex_hull()
        self.add_geometry_widget_and_others(
            name=geometry_info.name[0] + "_convex_hull", geometry=convex_hull)
        self._print_message("[Info] Successfully create convex_hull.")

    def _on_menu_transformation(self):
        dialog = gui.Dialog("transformation")
        em = self.window.theme.font_size
        vert = gui.Vert(0, gui.Margins(0.2*em, 0.5*em, 0.2*em, 0.5*em))
        vert.add_child(gui.Label("First,Translation [XYZ]"))
        vert.add_child(gui.Label("Then,Rotation [XYZ] Euler angles"))
        vert.add_child(gui.Label("Finally,scale"))
        label = ["X Translation: ", "Y Translation: ", "Z Translation: ",
                 "X Rotation: ", "Y Rotation: ", "Z Rotation: ", "Scale: "
                 ]
        self._transformation_parameter = []
        for i in range(0, len(label)):
            number_edit = gui.NumberEdit(gui.NumberEdit.DOUBLE)
            if i != 6:
                number_edit.double_value = 0.0
            else:
                number_edit.double_value = 1.0
            horiz = gui.Horiz(0, gui.Margins(0, 0.5*em, 0, 0.5*em))
            horiz.add_child(gui.Label(label[i]))
            horiz.add_child(number_edit)
            vert.add_child(horiz)
            self._transformation_parameter.append(number_edit)

        cancel_button = gui.Button("cancel")
        cancel_button.set_on_clicked(self._dialog_cancel)
        apply_button = gui.Button("apply")
        apply_button.set_on_clicked(self._transformation)
        button_layout = gui.Horiz()
        button_layout.add_stretch()
        button_layout.add_child(cancel_button)
        button_layout.add_stretch()
        button_layout.add_child(apply_button)
        button_layout.add_stretch()
        vert.add_child(button_layout)
        dialog.add_child(vert)
        self.window.show_dialog(dialog)

    def _transformation(self):
        self._dialog_cancel()
        id = self.geometry_treeview.selected_item
        geometry_info = self.geometry_infos.get(id)
        # 空间几何变换
        geometry_info.geometry[0].translate(
            (self._transformation_parameter[0].double_value,
                self._transformation_parameter[1].double_value,
                self._transformation_parameter[2].double_value))
        R = geometry_info.geometry[0].get_rotation_matrix_from_xyz(
            ((self._transformation_parameter[3].double_value % 360) / 180 * np.pi,
                (self._transformation_parameter[4].double_value %
                 360) / 180 * np.pi,
                (self._transformation_parameter[5].double_value % 360) / 180 * np.pi))
        geometry_info.geometry[0].rotate(R, center=(0, 0, 0))
        geometry_info.geometry[0].scale(
            self._transformation_parameter[6].double_value,
            center=geometry_info.geometry[0].get_center())
        # 画布中重绘
        self.remove_geometry_in_scene_module(geometry_info)
        geometry_info.geometry = [geometry_info.geometry[0]]
        geometry_info.bulid()
        self.add_geometry_in_scene_module(geometry_info, False)

    # remesh 网格模型
    def _on_menu_remesh(self):
        info = []
        info.append("take a triangle mesh and generate a manifold mesh.")
        info.append("Then,for efficiency purpose,simplify the mesh.")
        self.show_functional_dialog(
            "remesh", info, MainWindow.MENU_REMESH, self._remesh)

    def _on_menu_subdivision_midpoint(self):
        info = []
        info.append("we compute the midpoint of each side per triangle")
        info.append("and divide the triangle into four smaller triangles.")
        self.show_functional_dialog(
            "subdivision", info, MainWindow.MENU_SUBDIVISION_MIDPOINT, self._subdivision)

    def _on_menu_subdivision_loop(self):
        info = []
        info.append("Function subdivide mesh using Loop’s algorithm.")
        info.append(
            "Loop, Smooth subdivision surfaces based on triangles, 1987.")
        self.show_functional_dialog(
            "subdivision", info, MainWindow.MENU_SUBDIVISION_LOOP, self._subdivision)

    def _on_menu_smooth_humphrey(self):
        info = []
        info.append("use laplacian smoothing and Humphrey filtering.")
        info.append(
            "Articles \"Improved Laplacian Smoothing of Noisy Surface Meshes\"")
        info.append("J. Vollmer, R. Mencl, and H. Muller")
        self.show_functional_dialog(
            "humphrey smooth:", info, MainWindow.MENU_SMOOTH_HUMPHREY, self._smooth)

    def _on_menu_smooth_laplacian(self):
        info = []
        info.append("use laplacian smoothing.")
        self.show_functional_dialog(
            "laplacian smooth:", info, MainWindow.MENU_SMOOTH_LAPLACIAN, self._smooth)

    def _on_menu_smooth_taubin(self):
        info = []
        info.append("use laplacian smoothing and taubin filtering.")
        info.append(
            "Articles \"Improved Laplacian Smoothing of Noisy Surface Meshes\"")
        info.append("J. Vollmer, R. Mencl, and H. Muller")
        self.show_functional_dialog(
            "taubin smooth:", info, MainWindow.MENU_SMOOTH_TAUBIN, self._smooth)

    def _on_menu_simplification_average(self):
        info = []
        info.append("simplify mesh using vertex clustering.")
        info.append("The vertex positions are computed by the averaging.")
        self.show_functional_dialog(
            "simplification", info, MainWindow.MENU_SIMPLIFICATION_AVERAGE, self._simplification)

    def _on_menu_simplification_quadric_decimation(self):
        info = []
        info.append("simplify mesh using Quadric Error Metric Decimation.")
        self.show_functional_dialog(
            "simplification", info, MainWindow.MENU_SIMPLIFICATION_QUADRIC_DECIMATION, self._simplification)

    def _remesh(self):
        self._dialog_cancel()
        id = self.geometry_treeview.selected_item
        geometry_info = self.geometry_infos.get(id)

        vertices, faces = GeometryInfo.o3d_to_numpy(geometry_info.geometry[0])
        new_vertices, new_faces = remesh(
            vertices, faces, self._remesh_face_num.int_value)
        g = GeometryInfo.numpy_to_o3d(new_vertices, new_faces)

        self.add_geometry_widget_and_others(
            name=geometry_info.name[0] + "_manifold_simplied", geometry=g)

        self._print_message("[Info] Successfully remesh.")

    def _subdivision(self):
        self._dialog_cancel()
        id = self.geometry_treeview.selected_item
        temp = self.geometry_infos.get(id)

        mesh_out = None
        if self._function_type == MainWindow.MENU_SUBDIVISION_MIDPOINT:
            mesh_out = temp.geometry[0].subdivide_midpoint(
                number_of_iterations=self._subdivision_iteration.int_value)
        elif self._function_type == MainWindow.MENU_SUBDIVISION_LOOP:
            mesh_out = temp.geometry[0].subdivide_loop(
                number_of_iterations=self._subdivision_iteration.int_value)
        # 移除原来的网格
        self._on_delete()
        # 修改后添加
        self.add_geometry_widget_and_others(temp.name[0], mesh_out)

    def _smooth(self):
        self._dialog_cancel()
        id = self.geometry_treeview.selected_item
        temp = self.geometry_infos.get(id)

        # 转换为 trimesh 库中类型，使用trimesh.smoothing
        vertices, faces = GeometryInfo.o3d_to_numpy(temp.geometry[0])
        mesh = trimesh.Trimesh(vertices=vertices, faces=faces)
        name = temp.name[0]
        # 进行平滑操作
        if self._function_type == MainWindow.MENU_SMOOTH_HUMPHREY:
            trimesh.smoothing.filter_humphrey(
                mesh=mesh, iterations=self._smooth_iteration.int_value)
            name += "_humphrey"
        elif self._function_type == MainWindow.MENU_SMOOTH_LAPLACIAN:
            trimesh.smoothing.filter_laplacian(
                mesh=mesh, iterations=self._smooth_iteration.int_value)
            name += "_laplacian"
        else:
            trimesh.smoothing.filter_taubin(
                mesh=mesh, iterations=self._smooth_iteration.int_value)
            name += "_taubin"
        # 将trimesh转换为o3d
        smoothed_mesh = mesh.as_open3d
        # 添加平滑后的网格
        self.add_geometry_widget_and_others(
            name=name, geometry=smoothed_mesh)

        self._print_message("[Info] Successfully smoothen.")

    def _simplification(self):
        self._dialog_cancel()
        id = self.geometry_treeview.selected_item
        temp = self.geometry_infos.get(id)

        mesh = None
        if self._function_type == MainWindow.MENU_SIMPLIFICATION_AVERAGE:
            mesh = temp.geometry[0].simplify_vertex_clustering(
                self._simplification_parameter.double_value)
        elif self._function_type == MainWindow.MENU_SIMPLIFICATION_QUADRIC_DECIMATION:
            mesh = temp.geometry[0].simplify_quadric_decimation(
                self._simplification_parameter.int_value)
            # 移除原来的网格
        self._on_delete()
        # 修改后添加
        self.add_geometry_widget_and_others(temp.name[0], mesh)

    def show_functional_dialog(self, title: str, info: list, type: int, apply_button_function):
        dialog = gui.Dialog(title)
        self._function_type = type
        em = self.window.theme.font_size
        vert = gui.Vert(0, gui.Margins(0.2*em, 0.5*em, 0.2*em, 0.5*em))
        for i in info:
            vert.add_child(gui.Label(i))

        if type == MainWindow.MENU_REMESH:
            self._remesh_face_num = gui.NumberEdit(gui.NumberEdit.INT)
            self._remesh_face_num.int_value = 2000
            horiz = gui.Horiz(0, gui.Margins(0, 0.5*em, 0, 0.5*em))
            horiz.add_child(gui.Label("target face number:"))
            horiz.add_child(self._remesh_face_num)
            vert.add_child(horiz)
        elif type == MainWindow.MENU_SIMPLIFICATION_AVERAGE:
            self._simplification_parameter = gui.NumberEdit(
                gui.NumberEdit.DOUBLE)
            self._simplification_parameter.double_value = 0.028284
            horiz = gui.Horiz(0, gui.Margins(0, 0.5*em, 0, 0.5*em))
            horiz.add_child(gui.Label("voxel size:"))
            horiz.add_child(self._simplification_parameter)
            vert.add_child(horiz)
        elif type == MainWindow.MENU_SIMPLIFICATION_QUADRIC_DECIMATION:
            self._simplification_parameter = gui.NumberEdit(gui.NumberEdit.INT)
            horiz = gui.Horiz(0, gui.Margins(0, 0.5*em, 0, 0.5*em))
            horiz.add_child(gui.Label("target face number:"))
            horiz.add_child(self._simplification_parameter)
            vert.add_child(horiz)
        elif type == MainWindow.MENU_SUBDIVISION_LOOP or type == MainWindow.MENU_SUBDIVISION_MIDPOINT:
            self._subdivision_iteration = gui.NumberEdit(gui.NumberEdit.INT)
            self._subdivision_iteration.int_value = 1
            horiz = gui.Horiz(0, gui.Margins(0, 0.5*em, 0, 0.5*em))
            horiz.add_child(gui.Label("subdivision iterations:"))
            horiz.add_child(self._subdivision_iteration)
            vert.add_child(horiz)
        elif type == MainWindow.MENU_SMOOTH_HUMPHREY or type == MainWindow.MENU_SMOOTH_LAPLACIAN or type == MainWindow.MENU_SMOOTH_TAUBIN:
            self._smooth_iteration = gui.NumberEdit(gui.NumberEdit.INT)
            self._smooth_iteration.int_value = 3
            horiz = gui.Horiz(0, gui.Margins(0, 0.5*em, 0, 0.5*em))
            horiz.add_child(gui.Label("smoothing iterations:"))
            horiz.add_child(self._smooth_iteration)
            vert.add_child(horiz)

        cancel_button = gui.Button("cancel")
        cancel_button.set_on_clicked(self._dialog_cancel)
        apply_button = gui.Button("apply")
        apply_button.set_on_clicked(apply_button_function)
        button_layout = gui.Horiz()
        button_layout.add_stretch()
        button_layout.add_child(cancel_button)
        button_layout.add_stretch()
        button_layout.add_child(apply_button)
        button_layout.add_stretch()
        vert.add_child(button_layout)
        dialog.add_child(vert)
        self.window.show_dialog(dialog)

    """
    模型选择模块——功能函数
    """

    def _on_tree(self, id):
        g_info = self.geometry_infos.get(id)
        if g_info.visible[0]:
            self.show_hide_button.text = "hide"
        else:
            self.show_hide_button.text = "show"
        if len(g_info.geometry) == 3:
            if g_info.visible[1]:
                self.point_button.text = "point:hide"
            else:
                self.point_button.text = "point:show"
            if g_info.visible[2]:
                self.line_button.text = "line:hide"
            else:
                self.line_button.text = "line:show"
        # 状态修改
        if len(g_info.geometry) == 1:
            self.state_management(MainWindow.STATE_CLOUD)
        else:
            self.state_management(MainWindow.STATE_MESH)

    def _on_point(self):
        id = self.geometry_treeview.selected_item
        g_info = self.geometry_infos.get(id)

        g_info.visible[1] = not g_info.visible[1]
        # 如果有点且 mesh 可见
        if len(g_info.geometry) == 3 and g_info.visible[0]:
            self.scene_module.scene.show_geometry(
                g_info.name[1], g_info.visible[1])
        # 修改按钮的文本
        if g_info.visible[1]:
            self.point_button.text = "point:hide"
        else:
            self.point_button.text = "point:show"

    def _on_line(self):
        id = self.geometry_treeview.selected_item
        g_info = self.geometry_infos.get(id)

        g_info.visible[2] = not g_info.visible[2]
        # 如果有线且mesh可见
        if len(g_info.geometry) == 3 and g_info.visible[0]:
            self.scene_module.scene.show_geometry(
                g_info.name[2], g_info.visible[2])
        # 修改按钮的文本
        if g_info.visible[2]:
            self.line_button.text = "line:hide"
        else:
            self.line_button.text = "line:show"

    def add_geometry_widget_and_others(self, name=None, geometry=None, path=None, setup_camera=False):
        # 创建info
        info = None
        if path is None:
            info = GeometryInfo(name=name, geometry=geometry)
        else:
            info = GeometryInfo(path=path)
        # 若三维模型不是None时，执行添加操作
        if info.geometry[0]:
            # 在infos中添加
            self.geometry_infos.push_back(info)
            # 在TreeView中添加部件
            id = self.geometry_treeview.add_item(
                self.geometry_treeview.get_root_item(), self.geometry_infos.infos[-1].widget)
            self.geometry_infos.infos[-1].set_id(id)
            self.geometry_treeview.selected_item = id
            # 在画布中添加
            self.add_geometry_in_scene_module(info, setup_camera)
            # 状态修改
            if len(info.geometry) == 1:
                self.state_management(MainWindow.STATE_CLOUD)
            else:
                self.state_management(MainWindow.STATE_MESH)
        else:
            print("[WARNING] 添加三维模型信息组件失败。")

    def _on_show_hide(self):
        id = self.geometry_treeview.selected_item
        g_info = self.geometry_infos.get(id)
        g_info.visible[0] = not g_info.visible[0]
        if g_info.visible[0]:
            self.show_hide_button.text = "hide"
            self.show_geometry_in_scene_module(g_info, g_info.visible)
        else:
            self.show_hide_button.text = "show"
            self.show_geometry_in_scene_module(
                g_info, [g_info.visible[0], False, False])

    def _on_delete(self):
        if len(self.geometry_infos.infos) != 0:
            id = self.geometry_treeview.selected_item
            # 移除geometry部件
            self.geometry_treeview.remove_item(id)
            # 从画布和geometry_infos中移除
            g = self.geometry_infos.get(id)
            self.remove_geometry_in_scene_module(geometry_info=g)
            self.geometry_infos.remove(id)
            # 设置geometry_panel中选中的项目
            if len(self.geometry_infos.infos) > 0:
                self.geometry_treeview.selected_item = self.geometry_infos.infos[0].id
                if len(self.geometry_infos.infos[0].geometry) == 1:
                    self.state_management(MainWindow.STATE_CLOUD)
                else:
                    self.state_management(MainWindow.STATE_MESH)
            else:
                self.geometry_treeview.selected_item = 0
                self.state_management(MainWindow.STATE_NONE)

    """
    训练面板功能函数
    """

    # 训练面板 --> 添加点云文件按钮
    def _on_point_cloud_button(self):
        point_cloud_dialog = gui.FileDialog(
            gui.FileDialog.OPEN, "Select point cloud file:", self.window.theme)
        point_cloud_dialog.add_filter(
            ".xyz .xyzn .xyzrgb .ply .pcd .pts .txt .pwn",
            "Fitted Point cloud files (.xyz, .xyzn, .xyzrgb, .ply, "
            ".pcd, .pts .txt .pwn)")
        point_cloud_dialog.set_on_cancel(self._dialog_cancel)
        point_cloud_dialog.set_on_done(self._on_p_c_d_done)
        self.window.show_dialog(point_cloud_dialog)

    def _on_p_c_d_done(self, path):
        self._point_cloud.text_value = path
        path_list = path.split("/")
        path_list[-1] = "results"
        self._result_folder.text_value = "/".join(path_list)

        self.window.close_dialog()
        path = path + "__train__"
        self.load(path)

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

    # 训练面板 --> 是否使用pool
    def _on_poolings_status(self, is_checked):
        if is_checked:
            self.poolings_layout.visible = True
        else:
            self.poolings_layout.visible = False

    # 训练面板 --> 训练按钮
    def _on_train(self):

        # 给options赋值
        self.options.init(
            num_subdivisions=self._num_epoch.int_value,
            num_iterations=self._num_iterations.int_value,
            subdivision_multiplier=self._epoch_multiplier.double_value,
            initial_num_faces=self._initial_faces_num.int_value,
            max_num_faces=self._max_faces_num.int_value,
            obj_save_modulo=self._save_obj.int_value,
            min_num_samples=self._min_samples_num.int_value,
            max_num_samples=self._max_samples_num.int_value
        )
        if len(self._point_cloud.text_value) != 0:
            self.options.point_cloud = self._point_cloud.text_value
            self.options.save_location = self._result_folder.text_value
        if self.initial_mesh_status.checked and len(self._initial_mesh.text_value) != 0:
            self.options.initial_mesh = self._initial_mesh.text_value
        if self.poolings_status.checked:
            i = 0
            while i < 6:
                self.options.pooling[i] = self.poolings[i].double_value
                i = i+1

        print(self.options)

        # 校验 options 参数
        warnings = self.options.validate(self.poolings_status.checked)
        if len(warnings) == 0:
            # 关闭部分部件的功能
            self.train_status = True
            self.state_management(MainWindow.STATE_TRAIN)
            # 清除画布，仅显示训练的模型变化
            self.scene_module.scene.clear_geometry()
            threading.Thread(target=self.train_model).start()
        else:
            self.options.reset()
            self.show_message_box("WARNING", warnings)

    # 训练面板 --> 暂停按钮
    def _on_stop(self):
        if self.train_status:
            self.stop_button.text = "Continue"
            self.train_status = False
        else:
            self.stop_button.text = "Stop"
            self.train_status = True
            self._print_message("[Info] recovery the training.")

    def train_model(self):

        point_cloud = self.cloud
        point_cloud_tf = tf.convert_to_tensor(point_cloud, dtype=tf.float32)

        def load(path) -> Tuple[np.ndarray, np.ndarray]:
            temp = GeometryInfo.read(path)
            vertices, faces = GeometryInfo.o3d_to_numpy(temp.geometry)
            return np.float32(vertices), faces

        def save(path, vertices, faces):
            geometry = GeometryInfo.numpy_to_o3d(vertices, faces)
            GeometryInfo.write(geometry, path)

        def print_message(message: str):
            self.message = message
            print(message)
            gui.Application.instance.post_to_main_thread(
                self.window, self._print_message_on_child_thread)

        # 初始网格
        if self.options.initial_mesh:
            remeshed_vertices, remeshed_faces = load(self.options.initial_mesh)
        else:
            convex_hull = trimesh.convex.convex_hull(point_cloud)
            remeshed_vertices, remeshed_faces = remesh(
                convex_hull.vertices, convex_hull.faces, self.options.initial_num_faces
            )
        save("tmp_initial_mesh.obj", remeshed_vertices, remeshed_faces)

        # 画布显示初始网格模型
        self.mesh_in_train = GeometryInfo.numpy_to_o3d(
            remeshed_vertices, remeshed_faces)
        gui.Application.instance.post_to_main_thread(
            self.window, self._change_scene_on_child_thread)

        # 模型
        chamfer_loss = ChamferLossLayer()
        optimizer = tf.keras.optimizers.Adam(learning_rate=0.00005)
        num_subdivisions = self.options.num_subdivisions
        new_vertices = None

        for subdivision_level in range(num_subdivisions):
            chamfer_convergence = ConvergenceDetector()

            if subdivision_level != 0:
                if new_vertices is None:
                    raise Exception("Could not find vertices to subdivide.")
                else:
                    new_face_num = min(
                        self.options.max_num_faces, self.options.subdivision_multiplier *
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
                mesh.edges.shape[0], self.options.pooling)

            # 随机特征值
            in_features = tf.random.uniform(
                (mesh.edges.shape[0], 6), -0.5, 0.5)

            old_vertices = tf.convert_to_tensor(
                remeshed_vertices, dtype=tf.float32)
            num_iterations = self.options.num_iterations
            for iteration in range(num_iterations):
                iteration_start_time = time.time()
                with tf.GradientTape() as tape:
                    # 获取新的点位置
                    features = model(mesh, in_features)
                    new_vertices = old_vertices + \
                        get_vertex_features(mesh, features)
                    # 计算loss值
                    samples_num = int(
                        self.options.min_num_samples
                        + (iteration / self.options.num_iterations)
                        * (self.options.max_num_samples - self.options.min_num_samples)
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
                save_obj = self.options.obj_save_modulo
                if iteration % save_obj == 0 or converged or iteration == num_iterations - 1:
                    save(
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
                                  " [Info] already stop the training")

                # 画布替换
                self.mesh_in_train = GeometryInfo.numpy_to_o3d(
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
        gui.Application.instance.post_to_main_thread(
            self.window, self._reset_after_train)

    def _change_scene_on_child_thread(self):
        self.scene_module.scene.clear_geometry()
        self.scene_module.scene.add_geometry(
            "__result__", self.mesh_in_train, self.settings.material)

    def _reset_after_train(self):
        # 恢复部分部件功能
        self.train_status = False
        self.state_management(MainWindow.STATE_MESH)
        # 重新绘画已有的模型，并不展示出来
        self.scene_module.scene.clear_geometry()
        for g_info in self.geometry_infos.infos:
            g_info.visible = [False, False, False]
            self.add_geometry_in_scene_module(
                geometry_info=g_info, setup_camera=False)
        # 添加训练结果模型，并展示出来
        self.add_geometry_widget_and_others(
            name="__Point2Mesh_result__", geometry=self.mesh_in_train, setup_camera=True)

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

    """
    消息面板功能函数
    """

    def _print_message_on_child_thread(self):
        self.message_label.text = self.message

    def _print_message(self, message: str):
        self.message_label.text = message

    """
    状态管理
    """

    def state_management(self, state: int):

        def train_module_button(enabled: bool, visible: bool):
            self.stop_button.visible = visible
            self.train_button.enabled = enabled
            self.point_cloud_button.enabled = enabled
            self.initial_mesh_button.enabled = enabled

        def model_select_button(p_l_enabled: bool, enabled: bool):
            self.point_button.enabled = p_l_enabled
            self.line_button.enabled = p_l_enabled
            self.delete_button.enabled = enabled
            self.show_hide_button.enabled = enabled

        if state == MainWindow.STATE_NONE:
            gui.Application.instance.menubar.set_enabled(5, True)
            for i in range(6, 17):
                gui.Application.instance.menubar.set_enabled(i, False)
            train_module_button(True, False)
            model_select_button(False, False)
        elif state == MainWindow.STATE_CLOUD:
            for i in range(5, 9):
                gui.Application.instance.menubar.set_enabled(i, True)
            for i in range(9, 17):
                gui.Application.instance.menubar.set_enabled(i, False)
            train_module_button(True, False)
            model_select_button(False, True)
        elif state == MainWindow.STATE_MESH:
            for i in range(5, 17):
                gui.Application.instance.menubar.set_enabled(i, True)
            train_module_button(True, False)
            model_select_button(True, True)
        elif state == MainWindow.STATE_TRAIN:
            for i in range(5, 17):
                gui.Application.instance.menubar.set_enabled(i, False)
            train_module_button(False, True)
            model_select_button(False, False)

    """
    画布模块
    """

    def add_geometry_in_scene_module(self, geometry_info: GeometryInfo, setup_camera: bool):
        # 设置摄像头
        if setup_camera:
            bounds = geometry_info.geometry[0].get_axis_aligned_bounding_box()
            self.scene_module.setup_camera(60, bounds, bounds.get_center())
        # 三维模型
        for i in range(0, len(geometry_info.geometry)):
            self.scene_module.scene.add_geometry(
                geometry_info.name[i], geometry_info.geometry[i], self.settings.material)
            self.scene_module.scene.show_geometry(
                geometry_info.name[i], geometry_info.visible[i])

    def remove_geometry_in_scene_module(self, geometry_info: GeometryInfo):
        for i in range(0, len(geometry_info.geometry)):
            self.scene_module.scene.remove_geometry(geometry_info.name[i])

    def show_geometry_in_scene_module(self, geometry_info: GeometryInfo, visible: list):
        for i in range(0, len(geometry_info.geometry)):
            self.scene_module.scene.show_geometry(
                geometry_info.name[i], visible[i])


def main():
    gui.Application.instance.initialize()
    MainWindow()
    gui.Application.instance.run()


if __name__ == '__main__':
    main()
