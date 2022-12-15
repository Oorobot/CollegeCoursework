import datetime
import random
import sys

from PBS import IN_OPERATION, OUT_OPERATION, PBS

if __name__ == "__main__":
    sys.stdout = open("log.txt", "w", encoding="utf-8")
    print("\n", datetime.datetime.now())
    pbs = PBS("附件1.xlsx")
    pbs.print_pic()
    while True:
        pbs.current_situation()
        in_flag, out_flag = False, False
        in_op, out_op = "", ""
        while True:
            # 可执行的操作 in_operations, out_operations
            in_operations = pbs.executable_operations["IN"] if not in_flag else []
            out_operations = pbs.executable_operations["OUT"] if not out_flag else []

            # 输入可执行操作
            if not in_flag and 0 != len(in_operations):
                in_op = in_operations[random.randint(0, len(in_operations) - 1)]
            if not out_flag and 0 != len(out_operations):
                # ops = []
                # for op in out_operations:
                #     if "71" not in op:
                #         ops.append(op)
                # out_op = ops[random.randint(0, len(ops) - 1)]
                out_op = out_operations[random.randint(0, len(out_operations) - 1)]

            if not in_flag:
                for op in in_operations:
                    print("==> 代码 {} -- {}".format(op, IN_OPERATION[op]["name"]))
                in_flag = pbs.get_in_operation(in_op)
            if not out_flag:
                for op in out_operations:
                    print("==> 代码 {} -- {}".format(op, OUT_OPERATION[op]["name"]))
                out_flag = pbs.get_out_operation(out_op)

            # 无可执行的操作时, 进入下一秒进行更新s
            if in_flag and out_flag:
                break

        # # 人工输入
        # pbs.operate()
        if pbs.next_second():
            break
