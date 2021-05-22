class Options():

    DEFAULT_PROFILE_NAME = "DEFAULT"
    OPTIONS_PROFILE = {
        DEFAULT_PROFILE_NAME: {
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

    }

    point_cloud: str
    save_location: str
    initial_mesh: None
    num_subdivisions: int
    num_iterations: int
    subdivision_multiplier: float
    initial_num_faces: int
    max_num_faces: int
    beamgap_modulo: float
    obj_save_modulo: int
    min_num_samples: int
    max_num_samples: int
    pooling: list

    def __init__(self) -> None:
        self.apply_options_profile(Options.DEFAULT_PROFILE_NAME)

    def __str__(self) -> str:
        str = [f"{self.__class__.__name__}:"]
        for name, value in vars(self).items():
            str.append('%s=%s' % (name, value))
        return "[" + (" ").join(str) + "]"

    def init(self,
             num_subdivisions: int,
             num_iterations: int,
             subdivision_multiplier: float,
             initial_num_faces: int,
             max_num_faces: int,
             obj_save_modulo: int,
             min_num_samples: int,
             max_num_samples: int,
             beamgap_modulo=-1,
             point_cloud=None,
             save_location=None,
             initial_mesh=None,
             # pooling=[None, None, None, None, None, None]
             ):
        self.point_cloud = point_cloud
        self.save_location = save_location
        self.initial_mesh = initial_mesh
        self.num_subdivisions = num_subdivisions
        self.num_iterations = num_iterations
        self.subdivision_multiplier = subdivision_multiplier
        self.initial_num_faces = initial_num_faces
        self.max_num_faces = max_num_faces
        self.beamgap_modulo = beamgap_modulo
        self.obj_save_modulo = obj_save_modulo
        self.min_num_samples = min_num_samples
        self.max_num_samples = max_num_samples
        self.pooling = [None, None, None, None, None, None]

    def reset(self):
        self.apply_options_profile(Options.DEFAULT_PROFILE_NAME)

    def apply_options_profile(self, name) -> None:
        profile = Options.OPTIONS_PROFILE[name]
        for key, val in profile.items():
            setattr(self, key, val)

    def validate(self, validate_pooling: bool) -> list:
        warnings = []
        if self.point_cloud is None or len(self.point_cloud) == 0:
            warnings.append("please choose a point cloud file.")
        if self.num_subdivisions <= 0:
            warnings.append("num of epochs must > 0.")
        if self.num_iterations <= 0:
            warnings.append("num of iterations must > 0.")
        if self.subdivision_multiplier <= 1:
            warnings.append(
                "the multiple of face number of two adjacent epoch must > 1.0.")
        if self.initial_num_faces < 1000:
            warnings.append("suggest The initial face number >= 1000.")
        if self.max_num_faces <= self.initial_num_faces:
            warnings.append(
                "The maximum face number must > the initial face number.")
        if self.obj_save_modulo <= 0:
            warnings.append("the number how often to save must > 0")
        if self.min_num_samples <= 0:
            warnings.append("The minimum number of samples must > 0. ")
        if self.max_num_samples <= self.min_num_samples:
            warnings.append(
                "The maxinum number of samples must > The mininum one")
        if validate_pooling:
            for i in range(0, 5):
                if self.pooling[i] <= 0 or self.pooling[i] >= 1 or self.pooling[i] <= self.pooling[i+1]:
                    warnings.append(
                        "pooling value should be in descending order between 0 and 1")
                    break
            if self.pooling[5] <= 0 or self.pooling[5] >= 1:
                warnings.append(
                    "pooling value should be in descending order between 0 and 1")
        return warnings
