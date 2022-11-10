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
		case 1:cout << "���ѧ��\n"; AddStudent(CreateStudent()); break;
		case 2:cout << "ɾ��ѧ��\n"; DelStudent(GetInputNum()); break;
		case 3:cout << "�޸���Ϣ\n"; ModityStudent(GetInputNum()); break;
		case 4:cout << "ɾ��ȫ����Ϣ\n"; DelAll(); break;
		case 5:cout << "��ʾȫ����Ϣ\n"; ShowAll(); break;
		case 6:cout << "ѧ��������ѯ\n"; ShowStudentCount(); break;
		case 7:cout << "��ȡ�ļ�\n"; LoadFromFile(); break;
		case 8:cout << "�����ļ�\n"; SaveToFile(); break;
		}
		system("pause");
	}
	return 0;
}

void studentMcard::ShowMenu()
{
	system("cls");
	cout << "-------------------------�Ϻ���ѧѧ��������ϵͳ--------------------------\n";
	cout << "\n\t1.���ѧ����Ϣ        2.ɾ��ѧ����Ϣ       3.�޸�ѧ����Ϣ\n";
	cout << "\t4.ɾ������ѧ����Ϣ    5.��ʾ����ѧ����Ϣ    6.��ʾ��Ϣ����\n";
	cout << "\t7.��ȡ�ļ�ѧ����Ϣ    8.����ѧ����Ϣ���ļ�\n";
	cout << "\t                      0.�˳�ϵͳ\n";
	cout << "\n-------------------------------------------------------------------------\n";
}

int studentMcard::GetChoice()//
{
	int num = -1;//�����û�ѡ��Ĳ˵����
	ShowMenu();
	cout << "��ѡ��˵�(0-8):";
	while (!(std::cin >> num) || num < 0 || num > 8)
	{
		ShowMenu();
		cout << "ѡ��˵�����������ѡ��(0-8):";
		cin.clear();
	}
	return num;
}

Student studentMcard::CreateStudent()
{
	Student student;
	Student* pstu = &student;
	cout << "������ѧ����ѧ��(����)��";
	std::cin >> pstu->_num;
	cout << "������ѧ��������(С��10�ַ�)��";
	cin >> pstu->_name;
	cout << "������ѧ�����н�";
	std::cin >> pstu->_money;
	cout << "������ѧ����״̬��0��1��0Ϊ��ʧ��1Ϊ������";
	while (!(std::cin >> pstu->_state) || pstu->_state < 0 || pstu->_state> 1)
	{
		cout << "״̬����������������루0��1����";
		cin.clear();
	}
	cout << "��ӳɹ�!\n";
	return  student;
}

int studentMcard::AddStudent(const Student& student)//4.��ѧ����Ϣ�ڵ���뵽������
{
	_data.Append(student);
	return 1;
}


void studentMcard::ShowAll()//6.��ʾ����ѧ����Ϣ
{
	_data.GoTop();
	Node<Student> *pstu;
	pstu = _data.Cur_node();
	cout << "---------------------------------------\n";
	cout << "  ѧ��      ����       ���       ״̬\n";
	cout << "---------------------------------------\n";
	while (_data.Cur_node())
	{
		cout << setw(8) << pstu->data._num;
		cout << setw(10) << pstu->data._name;
		cout << setw(8) << pstu->data._money << "Ԫ";
		if(pstu->data._state == 0) cout<<setw(10)<<"��ʧ";
		else cout<<setw(10)<<"����";
		//cout << setw(10) << pstu->data._state;
		_data.Skip(1);
		pstu = _data.Cur_node();
		cout << "\n";
	}
	printf("---------------------------------------\n");
}

int studentMcard::ShowStudentCount()//8.��ʾ��Ϣ����
{
	cout << "��ǰ�� " << _data.NumNodes() << " λѧ��\n";
	return _data.NumNodes();
}

void studentMcard::ModityStudent(int num)//9.�޸�ѧ����Ϣ������ΪҪ�޸ĵ�ѧ���ı��
{
	Student stu;
	stu._num = num;
	_data.Locate(stu, 1);
	Node<Student> *pstu = _data.Cur_node();
	if (!pstu)
	{
		cout << "û��ѧ��Ϊ" << num << "��ѧ����Ϣ��\n";
		return;
	}
	cout << "��ǰѧ��������Ϊ" << pstu->data._name;
	if (Question("\nȷ��Ҫ�޸���"))
	{
		cout << "������ѧ��������(С��10�ַ�)��";
		std::cin >> setw(10) >> pstu->data._name;
	}
	cout << "��ǰѧ�������Ϊ" << pstu->data._money;
	if (Question("\nȷ��Ҫ�޸���"))
	{
		cout << "������ѧ������";
		cin >> pstu->data._money;
	}
	cout << "��ǰѧ����״̬Ϊ(0����ʧ��1��������:" << pstu->data._state;
	if (Question("\nȷ��Ҫ�޸���"))
	{
		cout << "������ѧ����״̬��0����ʧ��1����������";
		while (!(cin >> pstu->data._state) || pstu->data._state < 0 || pstu->data._state > 1)
		{
			cout << "״̬����������������루0����ʧ��1����������";
			cin.clear();
		}
	}
	cout << "�޸���ϣ�\n";
}

int studentMcard::Question(const char *pstr)//��ȡ�û���ѡ��
{
	char answer;
	cout << pstr << "��ѡ��y or n����";
	while (!(std::cin >> answer) || (answer != 'y' && answer != 'n'))
	{
		cout << "�������������ѡ��y or n����";
		cin.clear();
	}
	if ('y' == answer)
		return 1;
	else
		return 0;
}

int studentMcard::GetInputNum()//��ȡ�û������ѧ���ı��
{
	int num;
	cout << "������ѧ����ѧ��(����)��";
	while (!(cin >> num))
	{
		cout << "ѧ�������������������ѧ����ѧ��(����)��";
		cin.clear();
	}
	return num;
}

void studentMcard::DelStudent(int num)//ɾ�����Ϊnum��ѧ����Ϣ
{
	_data.Locate(num, 1);
	if (_data.Cur_node())
	{
		if (!Question("ȷ��Ҫɾ����ѧ����Ϣ��"))
		{
			return;
		}
		_data.DeleteCurNode();
		cout << "ɾ����ѧ��Ϊ" << num << "��ѧ����Ϣ��\n";
	}
	else
	{
		cout << "ɾ����ѧ��Ϊ" << num << "��ѧ����Ϣ��\n";
	}
}

void studentMcard::DelAll()//13.ɾ�����е�ѧ����Ϣ
{
	_data.FreeList();
}

void studentMcard::SaveToFile()//14.��ѧ����Ϣ���浽�ļ�����
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
		cout << "�ɹ���д����" << count <<"��ѧ����Ϣ��\n";
	else
		cout << "Ӧд��"<< count << "��ѧ����Ϣ��ʵ��д��"<< i <<"��ѧ����Ϣ��\n";
}


void studentMcard::LoadFromFile()
{
	_data.FreeList();
	std::ifstream ifs(_filename, std::ios::in);
	Student student;
	while (ifs >> student._num >> student._name >> student._money >> student._state)
		_data.Append(student);
	cout << "�ļ���ȡ��ϣ�ѧ����Ϣ " << _data.NumNodes() << " ����\n";
}
