#include "studentMcard.h"
#include <iomanip>
using namespace std;

studentMcard::studentMcard(const string& file_name) :_filename(file_name)
{}

int studentMcard::Loop()
{
	while (true)
	{
		switch (GetChoice())
		{
		case 0:exit(0);
		case 1:cout << "添加学生\n"; AddStudent(CreateStudent()); break;
		case 2:cout << "删除学生\n"; DelStudent(GetInputNum()); break;
		case 3:cout << "修改信息\n"; ModityStudent(GetInputNum()); break;
		case 4:cout << "删除全部信息\n"; DelAll(); break;
		case 5:cout << "显示全部信息\n"; ShowAll(); break;
		case 6:cout << "学生数量查询\n"; ShowStudentCount(); break;
		case 7:cout << "读取文件\n"; LoadFromFile(); break;
		case 8:cout << "存入文件\n"; SaveToFile(); break;
		}
		system("pause");
	}
	return 0;
}

void studentMcard::ShowMenu()
{
	system("cls");
	cout << "-------------------------上海大学学生卡管理系统--------------------------\n";
	cout << "\n\t1.添加学生信息        2.删除学生信息       3.修改学生信息\n";
	cout << "\t4.删除所有学生信息    5.显示所有学生信息    6.显示信息数量\n";
	cout << "\t7.读取文件学生信息    8.保存学生信息至文件\n";
	cout << "\t                      0.退出系统\n";
	cout << "\n-------------------------------------------------------------------------\n";
}

int studentMcard::GetChoice()//
{
	int num = -1;//保存用户选择的菜单编号
	ShowMenu();
	cout << "请选择菜单(0-8):";
	while (!(std::cin >> num) || num < 0 || num > 8)
	{
		ShowMenu();
		cout << "选择菜单错误，请重新选择(0-8):";
		cin.clear();
	}
	return num;
}

Student studentMcard::CreateStudent()
{
	Student student;
	Student* pstu = &student;
	cout << "请输入学生的学号(整型)：";
	std::cin >> pstu->_num;
	cout << "请输入学生的姓名(小于10字符)：";
	cin >> pstu->_name;
	cout << "请输入学生卡中金额：";
	std::cin >> pstu->_money;
	cout << "请输入学生卡状态（0或1）0为挂失，1为正常：";
	while (!(std::cin >> pstu->_state) || pstu->_state < 0 || pstu->_state> 1)
	{
		cout << "状态输入错误，请重新输入（0或1）：";
		cin.clear();
	}
	cout << "添加成功!\n";
	return  student;
}

int studentMcard::AddStudent(const Student& student)//4.把学生信息节点加入到链表中
{
	_data.Append(student);
	return 1;
}


void studentMcard::ShowAll()//6.显示所有学生信息
{
	_data.GoTop();
	Node<Student> *pstu;
	pstu = _data.Cur_node();
	cout << "---------------------------------------\n";
	cout << "  学号      姓名       余额       状态\n";
	cout << "---------------------------------------\n";
	while (_data.Cur_node())
	{
		cout << setw(8) << pstu->data._num;
		cout << setw(10) << pstu->data._name;
		cout << setw(8) << pstu->data._money << "元";
		if(pstu->data._state == 0) cout<<setw(10)<<"挂失";
		else cout<<setw(10)<<"正常";
		//cout << setw(10) << pstu->data._state;
		_data.Skip(1);
		pstu = _data.Cur_node();
		cout << "\n";
	}
	printf("---------------------------------------\n");
}

int studentMcard::ShowStudentCount()//8.显示信息数量
{
	cout << "当前有 " << _data.NumNodes() << " 位学生\n";
	return _data.NumNodes();
}

void studentMcard::ModityStudent(int num)//9.修改学生信息，参数为要修改的学生的编号
{
	Student stu;
	stu._num = num;
	_data.Locate(stu, 1);
	Node<Student> *pstu = _data.Cur_node();
	if (!pstu)
	{
		cout << "没有学号为" << num << "的学生信息。\n";
		return;
	}
	cout << "当前学生的姓名为" << pstu->data._name;
	if (Question("\n确定要修改吗？"))
	{
		cout << "请输入学生的姓名(小于10字符)：";
		std::cin >> setw(10) >> pstu->data._name;
	}
	cout << "当前学生卡余额为" << pstu->data._money;
	if (Question("\n确定要修改吗？"))
	{
		cout << "请输入学生卡余额：";
		cin >> pstu->data._money;
	}
	cout << "当前学生卡状态为(0：挂失，1：正常）:" << pstu->data._state;
	if (Question("\n确定要修改吗？"))
	{
		cout << "请输入学生卡状态（0：挂失，1：正常）：";
		while (!(cin >> pstu->data._state) || pstu->data._state < 0 || pstu->data._state > 1)
		{
			cout << "状态输入错误！请重新输入（0：挂失，1：正常）：";
			cin.clear();
		}
	}
	cout << "修改完毕！\n";
}

int studentMcard::Question(const char *pstr)//获取用户的选择
{
	char answer;
	cout << pstr << "请选择（y or n）：";
	while (!(std::cin >> answer) || (answer != 'y' && answer != 'n'))
	{
		cout << "输入错误！请重新选择（y or n）：";
		cin.clear();
	}
	if ('y' == answer)
		return 1;
	else
		return 0;
}

int studentMcard::GetInputNum()//获取用户输入的学生的编号
{
	int num;
	cout << "请输入学生的学号(整型)：";
	while (!(cin >> num))
	{
		cout << "学号输入错误！请重新输入学生的学号(整型)：";
		cin.clear();
	}
	return num;
}

void studentMcard::DelStudent(int num)//删除编号为num的学生信息
{
	_data.Locate(num, 1);
	if (_data.Cur_node())
	{
		if (!Question("确定要删除该学生信息吗？"))
		{
			return;
		}
		_data.DeleteCurNode();
		cout << "删除了学号为" << num << "的学生信息。\n";
	}
	else
	{
		cout << "删除了学号为" << num << "的学生信息。\n";
	}
}

void studentMcard::DelAll()//13.删除所有的学生信息
{
	_data.FreeList();
}

void studentMcard::SaveToFile()//14.把学生信息保存到文件当中
{
	int i = 0, count = _data.NumNodes();
	std::ofstream ofs(_filename, std::ios::out);
	_data.GoTop();
	for (Node<Student> *pstu = _data.Cur_node(); pstu != NULL; _data.Skip(1), pstu = _data.Cur_node(),i++)
	{
		Student stu = pstu->data;
		ofs << stu._num << " " << stu._name << " " << stu._money << " " << stu._state <<std::endl;
	}
	if (i == count)
		cout << "成功的写入了" << count <<"条学生信息。\n";
	else
		cout << "应写入"<< count << "条学生信息，实际写入"<< i <<"条学生信息。\n";
}


void studentMcard::LoadFromFile()
{
	_data.FreeList();
	std::ifstream ifs(_filename, std::ios::in);
	Student student;
	while (ifs >> student._num >> student._name >> student._money >> student._state)
		_data.Append(student);
	cout << "文件读取完毕！学生信息 " << _data.NumNodes() << " 条。\n";
}
