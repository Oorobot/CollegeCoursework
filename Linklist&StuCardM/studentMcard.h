#ifndef STUDENTMCARD_H
#define STUDENTMCARD_H
#include "LinkList.h"
#include <string>
#include <fstream>

using namespace std;

struct Student
{
	int _num;  //ѧ��
	string _name;
	int _money;   //���
	int _state;  //�������ʧ
	bool operator==(const Student& r)
	{
		return _num == r._num;
	}
	bool operator!=(const Student& r)
	{
		return !(*this == r);
	}
	Student(int num = 0) :_num(num) {}
};

class studentMcard
{
public:
	studentMcard(const string& file_name);
	void ShowMenu();  //1.������
	int GetChoice(); //2.��ȡ�û�ѡ��ı��
	Student CreateStudent();//3.����һ���µ�ѧ����Ϣ
	int AddStudent(const Student& student);//4.��ѧ����Ϣ���뵽������
	void ShowAll();//5.����ѧ����Ϣ
	int ShowStudentCount();//6.ѧ������
	void ModityStudent(int num);//7.�޸�ѧ��Ϊnum��ѧ����Ϣ
	int Question(const char *pstr);//8.��ȡ�û���ѡ��
	int GetInputNum();//9.��ȡ�û������ѧ�����
	void DelStudent(int num);//10.ɾ��ѧ��Ϊnum��ѧ����Ϣ
	void DelAll();//11.ɾ�����е�ѧ����Ϣ
	void SaveToFile();//12.��ѧ����Ϣ���浽�ļ�����
	void LoadFromFile();//13.���ļ��ж�ȡѧ����Ϣ
	int Loop();


private:
	LinkList<Student> _data;
	string _filename;
};



#endif // STUDENTMCARD_H
