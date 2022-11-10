#ifndef STUDENTMCARD_H
#define STUDENTMCARD_H
#include "LinkList.h"
#include <string>
#include <fstream>

using namespace std;

struct Student
{
	int _num;  //学号
	string _name;
	int _money;   //余额
	int _state;  //正常或挂失
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
	void ShowMenu();  //1.主界面
	int GetChoice(); //2.获取用户选择的编号
	Student CreateStudent();//3.创建一个新的学生信息
	int AddStudent(const Student& student);//4.把学生信息加入到链表中
	void ShowAll();//5.所有学生信息
	int ShowStudentCount();//6.学生数量
	void ModityStudent(int num);//7.修改学号为num的学生信息
	int Question(const char *pstr);//8.获取用户的选择
	int GetInputNum();//9.获取用户输入的学生编号
	void DelStudent(int num);//10.删除学号为num的学生信息
	void DelAll();//11.删除所有的学生信息
	void SaveToFile();//12.把学生信息保存到文件当中
	void LoadFromFile();//13.从文件中读取学生信息
	int Loop();


private:
	LinkList<Student> _data;
	string _filename;
};



#endif // STUDENTMCARD_H
