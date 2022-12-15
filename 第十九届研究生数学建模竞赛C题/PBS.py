# 接车->进车道：放车时间
IN_DROP_CAR_TIME = [9, 6, 3, 0, 6, 9]

# 返回道->进车道：放车时间
IN_RETURN_DROP_CAR_TIME = [15, 12, 9, 6, 6, 9]

# 接车横移机操作
IN_OPERATION = {
    "-1": {"name": "接车:空闲"},
    # time = [总时间, 拿上车的时间, 放下车的时间]
    "0-110": {"name": "接车:涂装出车口至进车道1", "time": [18, 0, 9]},
    "0-210": {"name": "接车:涂装出车口至进车道2", "time": [12, 0, 6]},
    "0-310": {"name": "接车:涂装出车口至进车道3", "time": [6, 0, 3]},
    "0-410": {"name": "接车:涂装出车口至进车道4", "time": [0, 0, 0]},
    "0-510": {"name": "接车:涂装出车口至进车道5", "time": [12, 0, 6]},
    "0-610": {"name": "接车:涂装出车口至进车道6", "time": [18, 0, 9]},
    # time = [总时间, 拿上车的时间, 放下车的时间]
    "710-110": {"name": "接车:返回道至进车道1", "time": [24, 3, 15]},
    "710-210": {"name": "接车:返回道至进车道2", "time": [18, 3, 12]},
    "710-310": {"name": "接车:返回道至进车道3", "time": [12, 3, 9]},
    "710-410": {"name": "接车:返回道至进车道4", "time": [6, 3, 6]},
    "710-510": {"name": "接车:返回道至进车道5", "time": [12, 3, 6]},
    "710-610": {"name": "接车:返回道至进车道6", "time": [18, 3, 9]},
}

# 送车->进车道：拿车时间
OUT_PICK_CAR_TIME = [9, 6, 3, 0, 6, 9]

# 返回道->进车道：拿车时间
OUT_RETURN_PICK_CAR_TIME = [9, 6, 3, 0, 6, 9]

# 送车横移机操作
OUT_OPERATION = {
    "-1": {"name": "送车:空闲"},
    # time = [总时间, 拿上车的时间, 放下车的时间]
    "11-3": {"name": "送车:进车道1至总装接车口", "time": [18, 9, 18]},
    "21-3": {"name": "送车:进车道2至总装接车口", "time": [12, 6, 12]},
    "31-3": {"name": "送车:进车道3至总装接车口", "time": [6, 3, 6]},
    "41-3": {"name": "送车:进车道4至总装接车口", "time": [0, 0, 0]},
    "51-3": {"name": "送车:进车道5至总装接车口", "time": [12, 6, 12]},
    "61-3": {"name": "送车:进车道6至总装接车口", "time": [18, 9, 18]},
    # time = [总时间, 拿上车的时间, 放下车的时间]
    "11-71": {"name": "送车:进车道1至返回道", "time": [24, 9, 21]},
    "21-71": {"name": "送车:进车道2至返回道", "time": [18, 6, 15]},
    "31-71": {"name": "送车:进车道3至返回道", "time": [12, 3, 9]},
    "41-71": {"name": "送车:进车道4至返回道", "time": [6, 0, 3]},
    "51-71": {"name": "送车:进车道5至返回道", "time": [12, 6, 9]},
    "61-71": {"name": "送车:进车道6至返回道", "time": [18, 9, 15]},
}


import random

import numpy as np
import pandas as pd


class PBS:
    def __init__(self, car_in_order: str, constraint_6_7: bool = True) -> None:
        # 约束6和7
        self.constraint_6_7 = constraint_6_7
        # 汇总记录
        self.summary_record = []
        # 准备入场的车的序号
        self.current_car_number = 0
        # 入场的车
        self.cars = pd.read_excel(car_in_order)
        # 区域计时
        self.area = {}
        for _ in pd.read_excel("附件3.xlsx")[["区域", "代码"]].values:
            self.area[str(_[1])] = {
                # 公共属性
                "name": _[0],  # 区域中文名
                "car": None,  # 车的进程顺序号
                # 车道停车位使用的属性
                "time": -1,  # -1: 停止; >=0: 正在移动
                # 接送横移车使用的属性
                "total_time": -1,  # 若有操作则表示完成该操作的全部时间，无则为-1。
                "take_car_time": -1,  # 拿到车的时间
                "take_car_area": None,  # 拿车的地方
                "drop_car_time": -1,  # 放下车的时间
                "drop_car_area": None,  # 放下车的地方
            }
        # 使用返回道的次数
        self.num_return = 0
        # 记录每次可执行的操作
        self.executable_operations = {"IN": [], "OUT": []}
        # 车在进车道到达 1 号停车位的顺序
        self.car_to_parking_one_order = []
        # 出车顺序
        self.car_out_order = []
        # 将第一辆车放置涂装出车口
        self.car2area0()

    # 当前时间
    def current_time(self):
        return len(self.summary_record)

    # 将车入场放入涂装出车口
    def car2area0(self):
        if self.current_car_number < len(self.cars):
            car = self.cars.loc[self.current_car_number].to_dict()
            if self.area["0"]["car"] is None:
                self.area["0"]["car"] = car
                self.current_car_number += 1

    # 检查总装入车口
    def check_area3(self):
        car = self.area["3"]["car"]
        if car:
            if 0 == len(self.car_out_order):
                self.car_out_order.append(car)
            # 未出车加入出车队列
            elif car["进车顺序"] != self.car_out_order[-1]["进车顺序"]:
                self.car_out_order.append(car)
            # 已出车则清空
            else:
                car = None

    def finish(self):
        if len(self.car_out_order) == len(self.cars):
            pd.DataFrame(data=np.array(self.summary_record).T).to_excel("result.xlsx")
            return True
        else:
            return False

    # 当前情况汇总
    def current_situation(self):
        self.executable_operations = {"IN": [], "OUT": []}

        # 可执行的IN操作
        # 接车横移机状态
        in_status = True if self.area["1"]["total_time"] == -1 else False
        # 如果没车了, 且涂装出车口也没车
        if self.current_car_number == 318:
            print(0)
        has_car = True
        if self.area["0"]["car"] is None and self.current_car_number == len(self.cars):
            has_car = False
        # 返回道10号停车位有车或9号停车位在3秒内将移动到10号停车位, 则可使用
        return_ten_status = False
        if self.area["710"]["car"] or (
            self.area["79"]["car"] and self.area["79"]["time"] >= 6
        ):
            return_ten_status = True
        # 查看1-6进车道10号停车位
        for i in range(1, 7):
            area_ten = self.area[str(i) + "10"]
            if in_status and has_car:
                if area_ten["car"] is None or (
                    area_ten["car"]
                    and area_ten["time"] != -1
                    and 9 - area_ten["time"] <= IN_DROP_CAR_TIME[i - 1]
                ):
                    self.executable_operations["IN"].append(
                        "-".join(["0", str(i) + "10"])
                    )
            if in_status and return_ten_status:
                if area_ten["car"] is None or (
                    area_ten["car"]
                    and area_ten["time"] != -1
                    and 9 - area_ten["time"] <= IN_RETURN_DROP_CAR_TIME[i - 1]
                ):
                    self.executable_operations["IN"].append(
                        "-".join(["710", str(i) + "10"])
                    )

        # 可执行的OUT操作
        # 送车横移机状态
        out_status = True if self.area["2"]["total_time"] == -1 else False
        # 返回道1号停车位无车或1号停车位的车将在3秒内移动到2号停车位, 则可使用
        return_one_status = False
        if self.area["71"]["car"] is None or (
            self.area["71"]["car"] and self.area["71"]["time"] >= 6
        ):
            return_one_status = True
        # 查看1-6进车道1号停车位
        for i in range(1, 7):
            area_one = self.area[str(i) + "1"]
            area_two = self.area[str(i) + "2"]
            if out_status:
                if area_one["car"] or (
                    area_one["car"] is None
                    and area_two["car"]
                    and 9 - area_two["time"] <= OUT_PICK_CAR_TIME[i - 1]
                ):
                    self.executable_operations["OUT"].append(
                        "-".join([str(i) + "1", "3"])
                    )
            if out_status and return_one_status:
                if area_one["car"] or (
                    area_one["car"] is None
                    and area_two["car"]
                    and 9 - area_two["time"] <= OUT_RETURN_PICK_CAR_TIME[i - 1]
                ):
                    self.executable_operations["OUT"].append(
                        "-".join([str(i) + "1", "71"])
                    )
        # 如果710没有车且接车横移机空闲, 则接车横移机可以空闲
        if self.area["710"]["car"] is None and in_status:
            self.executable_operations["IN"].append("-1")
        # 如果任意车道1停车位没有车且送车横移机空闲, 则送车横移机可以空闲
        if len(self.car_to_parking_one_order) == 0 and out_status:
            self.executable_operations["OUT"].append("-1")

        # 添加约束条件6和7
        if self.constraint_6_7:
            if self.area["710"]["car"] and in_status:
                in_op = []
                for op in self.executable_operations["IN"]:
                    if "710" in op:
                        in_op.append(op)
                print(
                    "==>[IN]约束6和7: {} => {}".format(
                        self.executable_operations["IN"], in_op
                    )
                )
                self.executable_operations["IN"] = in_op
            if len(self.car_to_parking_one_order) != 0 and out_status:
                out_op = []
                no = self.car_to_parking_one_order[0]
                for op in self.executable_operations["OUT"]:
                    if no in op:
                        out_op.append(op)
                print(
                    "==>[OUT]约束6和7: {} => {}".format(
                        self.executable_operations["OUT"], out_op
                    )
                )
                self.executable_operations["OUT"] = out_op

    def get_in_operation(self, in_op):
        in_operation = self.executable_operations["IN"]
        if len(in_operation) == 0:
            return True
        if in_op not in in_operation:
            print("重新输入操作代码")
            return False
        elif in_op == "-1":
            return True
        else:
            # 查看是否能够出车
            self.car2area0()
            # 更新掉 0s 的操作, 初始化接车横移机
            from_p, to_p = in_op.split("-")
            area = self.area["1"]
            area["time"] = 0
            area["total_time"] = IN_OPERATION[in_op]["time"][0]
            area["take_car_area"] = from_p
            area["take_car_time"] = IN_OPERATION[in_op]["time"][1]
            area["drop_car_area"] = to_p
            area["drop_car_time"] = IN_OPERATION[in_op]["time"][2]
            self.update_dilivery_area("1")
            self.print_pic()
            # 非0s的过程, 无需在输入代码
            if in_op != "0-410":
                return True
            # 重新调用 current_situation获取可执行的操作
            self.current_situation()
            return False

    def get_out_operation(self, out_op):
        out_operations = self.executable_operations["OUT"]
        if len(out_operations) == 0:
            return True
        if len(out_operations) == 1 and out_operations[0] == "-1":
            return True
        if out_op not in out_operations:
            print("重新输入操作代码")
            return False
        elif out_op == "-1":
            return True
        else:
            # 更新掉需要 0s 的操作, 初始化送车横移机任务
            from_p, to_p = out_op.split("-")
            area = self.area["2"]
            area["time"] = 0
            area["total_time"] = OUT_OPERATION[out_op]["time"][0]
            area["take_car_area"] = from_p
            area["take_car_time"] = OUT_OPERATION[out_op]["time"][1]
            area["drop_car_area"] = to_p
            area["drop_car_time"] = OUT_OPERATION[out_op]["time"][2]
            self.update_dilivery_area("2")
            self.print_pic()
            # 如果使用返回车道, 计数
            if out_op and "71" in out_op:
                self.num_return += 1
            # 非0s的过程, 无需在输入代码
            if out_op != "41-3":
                return True
            # 重新调用 current_situation获取可执行的操作
            self.current_situation()
            return False

    # 当前操作，操作选择直接调用 next_second
    def operate(self):
        in_operations = self.executable_operations["IN"]
        out_operations = self.executable_operations["OUT"]
        # 接车的操作
        if 0 != len(in_operations):
            while True:
                for op in in_operations:
                    print("==> 代码 {} -- {}".format(op, IN_OPERATION[op]["name"]))
                # input_operation = input("请输入PBS【接】车横移机要执行的操作代码: ")
                # 随机选择操作
                input_operation = in_operations[
                    random.randint(0, len(in_operations) - 1)
                ]
                print("输入的代码", input_operation)
                if input_operation not in in_operations:
                    print("请重新输入操作代码！")
                elif input_operation == "-1":
                    break
                else:
                    # 查看是否能够出车
                    self.car2area0()
                    # 更新掉 0s 的操作, 初始化接车横移机
                    from_p, to_p = input_operation.split("-")
                    area = self.area["1"]
                    area["time"] = 0
                    area["total_time"] = IN_OPERATION[input_operation]["time"][0]
                    area["take_car_area"] = from_p
                    area["take_car_time"] = IN_OPERATION[input_operation]["time"][1]
                    area["drop_car_area"] = to_p
                    area["drop_car_time"] = IN_OPERATION[input_operation]["time"][2]
                    self.update_dilivery_area("1")
                    self.print_pic()
                    # 非0s的过程, 无需在输入代码
                    if input_operation != "0-410":
                        break
                    # 重新调用 current_situation获取可执行的操作
                    self.current_situation()
                    in_operations = self.executable_operations["IN"]
                    if len(in_operations) == 0:
                        break
        # 送车的操作
        if 0 != len(out_operations):
            while True:
                # 如果仅有空闲选项直接跳过
                if len(out_operations) == 1 and out_operations[0] == "-1":
                    break
                for op in out_operations:
                    print("==> 代码 {} -- {}".format(op, OUT_OPERATION[op]["name"]))
                # input_operation = input("请输入PBS【送】车横移机要执行的操作代码: ")
                input_operation = out_operations[
                    random.randint(0, len(out_operations) - 1)
                ]
                print("输入的代码", input_operation)
                if input_operation not in out_operations:
                    print("请重新输入操作代码！")
                elif input_operation == "-1":
                    break
                else:
                    # 更新掉需要 0s 的操作, 初始化送车横移机任务
                    from_p, to_p = input_operation.split("-")
                    area = self.area["2"]
                    area["time"] = 0
                    area["total_time"] = OUT_OPERATION[input_operation]["time"][0]
                    area["take_car_area"] = from_p
                    area["take_car_time"] = OUT_OPERATION[input_operation]["time"][1]
                    area["drop_car_area"] = to_p
                    area["drop_car_time"] = OUT_OPERATION[input_operation]["time"][2]
                    self.update_dilivery_area("2")
                    self.print_pic()
                    # 如果使用返回车道, 计数
                    if input_operation and "71" in input_operation:
                        self.num_return += 1
                    # 非0s的过程, 无需在输入代码
                    if input_operation != "41-3":
                        break
                    # 重新调用 current_situation获取可执行的操作
                    self.current_situation()
                    out_operations = self.executable_operations["OUT"]
                    if len(out_operations) == 0:
                        break

    # 更新到下一秒, 该修改区域的修改区域，该挪动的挪动
    def next_second(self):

        # 更新接送车横移机的时间
        for code in ["1", "2"]:
            if self.area[code]["total_time"] != -1:
                self.area[code]["time"] += 1

        # 更新快车道的时间
        for i in range(1, 7):
            for j in range(1, 11):
                area = self.area[str(i) + str(j)]
                # 停车位 1 号有车, 则 time = -1 该车停止移动
                if j == 1:
                    if area["car"]:
                        area["time"] = -1
                    continue
                # 非停车位 1 号有车
                next_area = self.area[str(i) + str(j - 1)]
                if area["car"]:
                    # 下一个车位有车且停止移动, 则 time = -1 该车停止移动
                    if next_area["car"] and next_area["time"] == -1:
                        area["time"] = -1
                    else:
                        if area["time"] == -1:
                            area["time"] = 0
                        area["time"] += 1
        # 更新返回道的时间
        for j in range(10, 0, -1):
            area = self.area["7" + str(j)]
            if j == 10:
                if area["car"]:
                    area["time"] = -1
                continue
            next_area = self.area["7" + str(j + 1)]
            if area["car"]:
                if next_area["car"] and next_area["time"] == -1:
                    next_area["time"] = -1
                else:
                    if area["time"] == -1:
                        area["time"] = 0
                    area["time"] += 1

        # 更新进车道停车位
        for i in range(1, 7):
            for j in range(2, 11):
                area = self.area[str(i) + str(j)]
                next_area = self.area[str(i) + str(j - 1)]
                # 下一个停车位为空且时间为9, 则移动车位
                if next_area["car"] is None and area["time"] == 9:
                    next_area["car"] = area["car"]
                    next_area["time"] = 0
                    self.reset_area(str(i) + str(j))
                    # 添加进入1号停车位的顺序, 仅记录进车道
                    if j == 2:
                        self.car_to_parking_one_order.append(str(i) + "1")

        # 更新返回车道的停车位
        for j in range(9, 0, -1):
            area = self.area["7" + str(j)]
            next_area = self.area["7" + str(j + 1)]
            # 下一个停车位为空且时间为9, 则移动车位
            if next_area["car"] is None and area["time"] == 9:
                next_area["car"] = area["car"]
                next_area["time"] = 0
                self.reset_area("7" + str(j))

        # 更新接送车横移机操作
        for code in ["1", "2"]:
            if self.area[code]["total_time"] != -1:
                self.update_dilivery_area(code)

        # 出车到涂装出车口
        self.car2area0()

        # 查看总装出车口
        self.check_area3()

        # 更新 record
        record = np.zeros(shape=len(self.cars), dtype=np.uint16)
        for k, v in self.area.items():
            if v["car"]:
                car_no = v["car"]["进车顺序"]
                record[car_no - 1] = int(k)
        self.summary_record.append(record)

        # 打印图像化界面
        self.print_pic()
        return self.finish()

    # 清空area
    def reset_area(self, code):
        self.area[code] = {
            "name": self.area[code]["name"],  # 区域中文名
            "car": None,  # 车的进程顺序号
            "time": -1,  # -1: 停止; >=0: 正在移动
            "total_time": -1,  # 若有操作则表示完成该操作的全部时间，无则为-1.
            "take_car_time": -1,  # 拿到车的时间
            "take_car_area": None,  # 拿车的地方
            "drop_car_time": -1,  # 放下车的时间
            "drop_car_area": None,  # 放下车的地方
        }

    # 更新接送横移车区域
    def update_dilivery_area(self, code: str):
        assert code == "1" or code == "2"
        area = self.area[code]
        if area["time"] == area["take_car_time"]:
            assert self.area[area["take_car_area"]]["car"] is not None
            area["car"] = self.area[area["take_car_area"]]["car"]
            # 从进车道1号停车位拿车, 将该车从记录的进入1号停车位的顺序中移出
            if len(area["take_car_area"]) == 2 and area["take_car_area"][1] == "1":
                self.car_to_parking_one_order.remove(area["take_car_area"])
            self.reset_area(area["take_car_area"])
        if area["time"] == area["drop_car_time"]:
            if area["drop_car_area"] != "3":
                assert self.area[area["drop_car_area"]]["car"] is None
            self.area[area["drop_car_area"]]["car"] = area["car"]
            self.area[area["drop_car_area"]]["time"] = 0
            area["car"] = None
        if area["time"] == area["total_time"]:
            self.reset_area(code)

    # 打印图形化说明
    def print_pic(self):
        print(
            "----------------------------------------------------------------------------------------------------------------------------------"
        )
        print("==> 当前时间: {} 秒".format(len(self.summary_record)))
        print(
            "----------------------------------------------------------------------------------------------------------------------------------"
        )
        # 涂装进车口和总装出车口
        print_str = ""
        for code in ["0", "3"]:
            area = self.area[code]
            if area["car"]:
                str_car = "_".join([str(c) for c in area["car"].values()])
                print_str += "{:<10} {:<30}".format(area["name"], str_car)
            else:
                print_str += "{:<10} {:<30}".format(area["name"], "空闲")
            print_str += "\t"
        print(print_str)
        print(
            "----------------------------------------------------------------------------------------------------------------------------------"
        )
        # 接送车状态
        print_str = ""
        for code in ["1", "2"]:
            area = self.area[code]
            if area["total_time"] == -1:
                print_str += "{:<10} {:<30}".format(area["name"], "空闲")
            elif area["time"] < area["take_car_time"]:
                area_name = (
                    "进车道" + area["take_car_area"][0]
                    if area["take_car_area"][0] != "7"
                    else "返回道"
                )
                print_str += "{:<10} ({:>2}/{:>2}) {:<22}".format(
                    area["name"], area["time"], area["total_time"], "前往 " + area_name,
                )
            elif (
                area["time"] >= area["take_car_time"]
                and area["time"] < area["drop_car_time"]
            ):
                str_car = "".join([str(c) for c in area["car"].values()])
                area_name = (
                    "进车道" + area["drop_car_area"][0]
                    if area["drop_car_area"][0] != "7"
                    else "返回道"
                )
                print_str += "{:<10} ({:>2}/{:>2}) {:<22}".format(
                    area["name"],
                    area["time"],
                    area["total_time"],
                    "运【" + str_car + "】-> " + area_name,
                )
            else:
                print_str += "{:<10} ({:>2}/{:>2}) {:<22}".format(
                    area["name"], area["time"], area["total_time"], "返回"
                )
            print_str += "\t"
        print(print_str)
        print(
            "----------------------------------------------------------------------------------------------------------------------------------"
        )
        # 打印分数
        print(
            "总分: {:.2f}, [1] {:.2f}, [2] {:.2f}, [3] {:.2f}, [4] {:.2f}.".format(
                *self.compute_score()
            )
        )
        print(
            "=================================================================================================================================="
        )

        def print_dirveway(no: int):
            str_direction, str_head = None, None
            if no == 7:
                str_direction = "<-"
                str_head = "{:<4}".format("返回道")
            else:
                str_direction = "->"
                str_head = "{:<4}".format("进车道" + str(no))
            print_str = str_head
            print_car = "{:<7}".format(" ")
            str_info = []
            str_car = []
            for j in range(10, 0, -1):
                area = self.area[str(no) + str(j)]
                if area["car"]:
                    car_info = "".join([str(c) for c in area["car"].values()])
                    str_info.append(
                        "{:<10}".format(car_info[:-4] + "-" + str(area["time"]) + "/9")
                    )
                    str_car.append(f"{car_info[-4:]:<6}")
                else:
                    str_info.append("{:<10}".format(" "))
                    str_car.append("{:<10}".format(" "))
            print(print_str + "\t" + str_direction.join(str_info))
            print(print_car + "\t" + str_direction.join(str_car))
            print(
                "=================================================================================================================================="
            )

        # 打印车道
        print_dirveway(6)
        print_dirveway(5)
        print_dirveway(7)
        print_dirveway(4)
        print_dirveway(3)
        print_dirveway(2)
        print_dirveway(1)

    # 计算得分
    def compute_score(self):
        target1 = 100
        first, second = -1, -1
        power = [car["动力"] for car in self.car_out_order]
        for i, p in enumerate(power):
            if p == "混动":
                if first == -1 and second == -1:
                    first = i
                elif first != -1 and second == -1:
                    second = i
                    if second - first != 3:
                        target1 -= 1
                else:
                    first = second
                    second = i
                    if second - first != 3:
                        target1 -= 1
            else:
                continue
        target2 = 100
        drive = [car["驱动"] for car in self.car_out_order]
        num1, num2 = 0, 0
        flag = 1
        for j in range(len(drive)):
            if flag == 1:
                if drive[j] == drive[0]:
                    num1 += 1
                else:
                    flag = 2
            if flag == 2:
                if drive[j] != drive[0]:
                    num2 += 1
                else:
                    if num1 != num2:
                        target2 -= 2
                    flag = 1
                    num1, num2 = 1, 0
        target3 = 100 - self.num_return
        target4 = 100 - 0.01 * (self.current_time() - 9 * len(self.cars) - 72)
        total_target = 0.4 * target1 + 0.3 * target2 + 0.2 * target3 + 0.1 * target4
        return total_target, target1, target2, target3, target4

