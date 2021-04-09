import os.path
import numpy as np


import open3d as o3d
import open3d.visualization.gui as gui
import open3d.visualization.rendering as rendering

basedir = os.path.dirname(os.path.realpath(__file__))

Options = {
    # The point cloud that's fitted.
    "point_cloud": "bunny.txt",
    # The number of times remeshing/subdivision happens.
    "num_subdivisions": 6,
    # The number of iterations between each remeshing/subdivision.
    "num_iterations": 1000,
    # Each subdivision multiplies the number of faces by this.
    "subdivision_multiplier": 1.5,
    # The maximum number of faces that subdivision is allowed to yield.
    "max_num_faces": 10000,
    # The initial number of faces used for optimization.
    "initial_num_faces": 1000,
    # An optional initial mesh.
    "initial_mesh": None,
    # The folder where the results are saved.
    "save_location": "results/bunny",
    # how often to run beamgap loss if -1 then no beam gap loss
    "beamgap_modulo": -1,
    # how often to save objs
    "obj_save_modulo": 5,
    # range to lineralyinterp between when computing samples
    "min_num_samples": 10000,
    "max_num_samples": 16000,
    "pooling": [None, None, None, None, None, None],
}


class Settings:
    UNLIT = "defaultUnlit"
    LIT = "defaultLit"
    NORMALS = "normals"
    DEPTH = "depth"

    DEFAULT_PROFILE_NAME = "Bright day with sun at +Y [default]"
    POINT_CLOUD_PROFILE_NAME = "Cloudy day (no direct sun)"
    CUSTOM_PROFILE_NAME = "Custom"
    LIGHTING_PROFILES = {
        DEFAULT_PROFILE_NAME: {
            "ibl_intensity": 45000,
            "sun_intensity": 45000,
            "sun_dir": [0.577, -0.577, -0.577],
            # "ibl_rotation":
            "use_ibl": True,
            "use_sun": True,
        },
        "Bright day with sun at -Y": {
            "ibl_intensity": 45000,
            "sun_intensity": 45000,
            "sun_dir": [0.577, 0.577, 0.577],
            # "ibl_rotation":
            "use_ibl": True,
            "use_sun": True,
        },
        "Bright day with sun at +Z": {
            "ibl_intensity": 45000,
            "sun_intensity": 45000,
            "sun_dir": [0.577, 0.577, -0.577],
            # "ibl_rotation":
            "use_ibl": True,
            "use_sun": True,
        },
        "Less Bright day with sun at +Y": {
            "ibl_intensity": 35000,
            "sun_intensity": 50000,
            "sun_dir": [0.577, -0.577, -0.577],
            # "ibl_rotation":
            "use_ibl": True,
            "use_sun": True,
        },
        "Less Bright day with sun at -Y": {
            "ibl_intensity": 35000,
            "sun_intensity": 50000,
            "sun_dir": [0.577, 0.577, 0.577],
            # "ibl_rotation":
            "use_ibl": True,
            "use_sun": True,
        },
        "Less Bright day with sun at +Z": {
            "ibl_intensity": 35000,
            "sun_intensity": 50000,
            "sun_dir": [0.577, 0.577, -0.577],
            # "ibl_rotation":
            "use_ibl": True,
            "use_sun": True,
        },
        POINT_CLOUD_PROFILE_NAME: {
            "ibl_intensity": 60000,
            "sun_intensity": 50000,
            "use_ibl": True,
            "use_sun": False,
            # "ibl_rotation":
        },
    }

    DEFAULT_MATERIAL_NAME = "Polished ceramic [default]"
    PREFAB = {
        DEFAULT_MATERIAL_NAME: {
            "metallic": 0.0,
            "roughness": 0.7,
            "reflectance": 0.5,
            "clearcoat": 0.2,
            "clearcoat_roughness": 0.2,
            "anisotropy": 0.0
        },
        "Metal (rougher)": {
            "metallic": 1.0,
            "roughness": 0.5,
            "reflectance": 0.9,
            "clearcoat": 0.0,
            "clearcoat_roughness": 0.0,
            "anisotropy": 0.0
        },
        "Metal (smoother)": {
            "metallic": 1.0,
            "roughness": 0.3,
            "reflectance": 0.9,
            "clearcoat": 0.0,
            "clearcoat_roughness": 0.0,
            "anisotropy": 0.0
        },
        "Plastic": {
            "metallic": 0.0,
            "roughness": 0.5,
            "reflectance": 0.5,
            "clearcoat": 0.5,
            "clearcoat_roughness": 0.2,
            "anisotropy": 0.0
        },
        "Glazed ceramic": {
            "metallic": 0.0,
            "roughness": 0.5,
            "reflectance": 0.9,
            "clearcoat": 1.0,
            "clearcoat_roughness": 0.1,
            "anisotropy": 0.0
        },
        "Clay": {
            "metallic": 0.0,
            "roughness": 1.0,
            "reflectance": 0.5,
            "clearcoat": 0.1,
            "clearcoat_roughness": 0.287,
            "anisotropy": 0.0
        },
    }

    def __init__(self):
        self.mouse_model = gui.SceneWidget.Controls.ROTATE_CAMERA
        self.bg_color = gui.Color(1, 1, 1)
        self.show_skybox = False
        self.show_axes = False
        self.use_ibl = True
        self.use_sun = True
        self.new_ibl_name = None  # clear to None after loading
        self.ibl_intensity = 45000
        self.sun_intensity = 45000
        self.sun_dir = [0.577, -0.577, -0.577]
        self.sun_color = gui.Color(1, 1, 1)

        self.apply_material = True  # clear to False after processing
        self._materials = {
            Settings.LIT: rendering.Material(),
            Settings.UNLIT: rendering.Material(),
            Settings.NORMALS: rendering.Material(),
            Settings.DEPTH: rendering.Material()
        }
        self._materials[Settings.LIT].base_color = [0.9, 0.9, 0.9, 1.0]
        self._materials[Settings.LIT].shader = Settings.LIT
        self._materials[Settings.UNLIT].base_color = [0.9, 0.9, 0.9, 1.0]
        self._materials[Settings.UNLIT].shader = Settings.UNLIT
        self._materials[Settings.NORMALS].shader = Settings.NORMALS
        self._materials[Settings.DEPTH].shader = Settings.DEPTH

        # Conveniently, assigning from self._materials[...] assigns a reference,
        # not a copy, so if we change the property of a material, then switch
        # to another one, then come back, the old setting will still be there.
        self.material = self._materials[Settings.LIT]


class MainWindow:
    MENU_FILE = 1
    MENU_QUIT = 3
    MENU_ABOUT = 21

    def __init__(self):

        self.settings = Settings()

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
        # result_folder_button = gui.Button("...")
        # result_folder_button.horizontal_padding_em = 0.5
        # result_folder_button.vertical_padding_em = 0
        # result_folder_button.set_on_clicked(self._on_result_folder_button)
        result_folder_layout = gui.Horiz()
        result_folder_layout.add_child(gui.Label("Result file Folder:"))
        result_folder_layout.add_child(self._result_folder)
        # result_folder_layout.add_child(result_folder_button)

        self.control_layout.add_child(result_folder_layout)

        # 其他详细的参数设置
        option_collapsablevert = gui.CollapsableVert(
            "Setting", 0.33*em, gui.Margins(0.5*em, 0, 0.5*em, 0))

        option_collapsablevert.set_is_open(False)

        num_epoch = gui.NumberEdit(gui.NumberEdit.INT)
        num_epoch.int_value = 6
        num_epoch.set_limits(1, 99)

        num_iterations = gui.NumberEdit(gui.NumberEdit.INT)
        num_iterations.int_value = 1000
        num_iterations.set_limits(100, 10000)

        epoch_multiplier = gui.NumberEdit(gui.NumberEdit.DOUBLE)
        epoch_multiplier.double_value = 1.5
        epoch_multiplier.set_limits(1.1, 10.0)

        train_parameters_layout = gui.Horiz()
        train_parameters_layout.add_child(gui.Label("number of epoch:"))
        train_parameters_layout.add_child(num_epoch)
        train_parameters_layout.add_fixed(em)
        train_parameters_layout.add_child(gui.Label("number of iterations:"))
        train_parameters_layout.add_child(num_iterations)

        option_collapsablevert.add_child(train_parameters_layout)

        train_parameters_layout_1 = gui.Horiz()
        train_parameters_layout_1.add_child(
            gui.Label("the multiple of face number of two adjacent epoch:"))
        train_parameters_layout_1.add_child(epoch_multiplier)

        option_collapsablevert.add_child(train_parameters_layout_1)

        # 与面的数量相关的参数
        initial_faces_num = gui.NumberEdit(gui.NumberEdit.INT)
        initial_faces_num.int_value = 1000

        max_faces_num = gui.NumberEdit(gui.NumberEdit.INT)
        max_faces_num.int_value = 10000

        face_parameters_layout = gui.Horiz()
        face_parameters_layout.add_child(gui.Label("The initial face number:"))
        face_parameters_layout.add_child(initial_faces_num)
        face_parameters_layout.add_fixed(em)
        face_parameters_layout.add_child(gui.Label("Maximum number of faces:"))
        face_parameters_layout.add_child(max_faces_num)

        option_collapsablevert.add_child(face_parameters_layout)

        # 其他参数
        save_obj = gui.NumberEdit(gui.NumberEdit.INT)
        save_obj.int_value = 5

        # min_num_samples
        # max_num_samples
        # pooling

        other_parameters_layout = gui.Horiz()
        other_parameters_layout.add_child(gui.Label("how often to save:"))
        other_parameters_layout.add_child(save_obj)

        option_collapsablevert.add_child(other_parameters_layout)

        self.control_layout.add_child(option_collapsablevert)

        Train_button_layout = gui.Horiz()
        Train_button = gui.Button("Train")
        Train_button.set_on_clicked(self._on_train)
        Train_button_layout.add_stretch()
        Train_button_layout.add_child(Train_button)

        self.control_layout.add_child(Train_button_layout)

        self.window.set_on_layout(self._on_layout)
        self.window.add_child(self._scene)
        self.window.add_child(self.control_layout)

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
            gui.FileDialog.OPEN, "Select point cloud file:", self.window.theme
        )
        point_cloud_dialog.add_filter(
            ".txt .pwn", "fitted point cloud files(.txt .pwn)")
        point_cloud_dialog.set_on_cancel(self._on_p_c_d_cancel)
        point_cloud_dialog.set_on_done(self._on_p_c_d_done)
        self.window.show_dialog(point_cloud_dialog)

    def _on_p_c_d_cancel(self):
        self.window.close_dialog()

    def _on_p_c_d_done(self, path):
        self._point_cloud.text_value = path
        path_list = path.split("/")
        filename = path_list.pop()
        path_list.append("results")
        result_path = "/".join(path_list)
        self._result_folder.text_value = result_path

        point_cloud = o3d.io.read_point_cloud(path, format='xyz')

        # mesh = o3d.io.read_triangle_mesh(path)

        self._scene.scene.clear_geometry()

        self._scene.scene.add_geometry(filename, point_cloud, self.settings.material)
        bounds = point_cloud.get_axis_aligned_bounding_box()
        self._scene.setup_camera(60, bounds, bounds.get_center())

        self.window.close_dialog()

    def _on_train(self):
        pass

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


def main():
    gui.Application.instance.initialize()
    MainWindow()
    gui.Application.instance.run()


if __name__ == '__main__':
    main()
