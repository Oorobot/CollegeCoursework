#include <iostream>
#include "LinkList.h"
#include "studentMcard.h"

using namespace std;


//���Զ�λ��ǰ�ڵ㺯��   ���Կ��С�
int test0()
{
	int array[] = { 10, 1, 2, 3, 4, 5, 6, 0, 1, 2 };
	int n = sizeof(array) / sizeof(*array);
	LinkList<int> iLink0, iLink1(n, array), iLink2(iLink1);
	cout << "Ĭ�Ϲ�������Ϊ0������" << endl;
	cout << "iLink0: " << iLink0 << endl;
	cout << "�Ƿ�Ϊ������" << iLink0.isEmpty();
	cout << "\niLink0��ǰ�ڵ����:" << iLink0.NumNodes();
	cout << "\n����iLink0��ǰ�ڵ���ţ�" << iLink0.CurPos();

	cout << "\n��������Ԫ�ع��������" << endl;
	cout << "iLink1: " << iLink1 << endl;

	cout << "\n�������������" << endl;
	cout << "iLink2: " << iLink2 << endl;
	cout << "����iLink2��ǰ�ڵ���ţ�" << iLink2.CurPos();

	iLink0 = iLink1; iLink0.GoTop(); iLink0.Skip(4);
	iLink0.DeleteCurNode();
	cout << "\n��ֵ���� iLink0 = iLink1\n" << "iLink0: " << iLink0 << endl;
	iLink0.InsbeforeHeadNode(999);
	cout << "\nͷָ��ǰ����\n" << "iLink0: " << iLink0 << endl;

	iLink1.GoBottom();
	cout << "\n cur_node��β�ڵ�" << endl;
	cout << "iLink1: " << iLink1 << endl;
	cout << "��ǰָ���Ƿ�Ϊ�׽ڵ㣺" << iLink1.isBegin();
	cout << "   ��ǰָ���Ƿ�Ϊβ�ڵ㣺" << iLink1.isLast();
	cout << "   ����iLink1��ǰ�ڵ���ţ�" << iLink1.CurPos();
	iLink1.InsAfterCurNode(9999);
	cout << "\n�ڵ�ǰָ������" << "iLink1:" << iLink1 << endl;
	iLink1.InsbeforeCurNode(8888);
	cout << "\n�ڵ�ǰָ��ǰ����" << "iLink1:" << iLink1 << endl;
	iLink1.Skip(-3);
	cout << "\n cur_node����ƶ�3��λ��" << endl;
	cout << "iLink1: " << iLink1 << endl;
	cout << "����iLink1��ǰ�ڵ���ţ�" << iLink1.CurPos();
	int index = 11;
	try
	{
		iLink1.Go(index);
	}
	catch (const std::out_of_range& e)
	{
		index = 10;
		iLink1.Go(index);
	}
	cout << "\n ������Χ���ص�βָ��" << endl;
	cout << "iLink1: " << iLink1 << endl;
	iLink1.GoTop();
	cout << "\n cur_node�ص�ͷָ��" << endl;
	cout << "iLink1: " << iLink1 << endl;
	int a = 1;
	iLink1.Locate(a, 1);
	cout << "\n cur_node��λ1" << endl;
	cout << "iLink1: " << iLink1 << endl;
	iLink1.Locate(a, 0);
	cout << "\n cur_node����ָ��λ�ÿ�ʼ������λ1" << endl;
	cout << "iLink1: " << iLink1 << endl;
	iLink1.Locate(11, 0);
	cout << "\n �޷���λ������Ľڵ�ʱ,���ص�ͷ�ڵ�" << endl;
	cout << "iLink1: " << iLink1 << endl;


	cout << "��ǰ�ڵ�����Ϊ��";
	iLink1.ShowCurData();
	cout << "  ��ǰ�ڵ��ַΪ��";
	cout << iLink1.Cur_node();
	cout << "  ���÷��ص�ǰ�ڵ����ݣ�";
	cout << iLink1.CurData();
	iLink1.Change(90);
	cout << "\n��ǰ�ڵ�����Ϊ��";
	iLink1.ShowCurData();
	cout << "\n��ǰ�ڵ����:" << iLink0.NumNodes();

	return 0;
}

int test1()
{
	LinkList<double> dLink(3);										// ����һ������������㣬�����򲻿�Ԥ֪��
	cout << "\nת������������������ֵ����Ԥ֪��\n" << dLink << endl;

	char cArray[] = { 'a', 'b', 'c', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	LinkList<char> cLinkArray[] = { LinkList<char>(3, cArray), LinkList<char>(2, cArray + 3), LinkList<char>(6, cArray + 5) };
	// ������������飬��3��Ԫ�أ���3������
	cout << cLinkArray[1] << endl;
	cout << "\n������������Ԫ��\n";
	for (int i = 0; i < 3; i++)
		cout << cLinkArray[i] << endl;

	LinkList<LinkList<char> > cLLink0(3);							// ����һ��������3����㣨ÿ�����Ҳ��һ������
	cout << "\n3�������������\n" << cLLink0 << endl;

	LinkList<LinkList<char> > cLLink1(3, cLinkArray);				// ���������������3�����
	cout << "\n3�����������\n" << cLLink1 << endl;

	cout << "\n���������ݴ����ļ�output.txt" << endl;
	cLLink1.Save("output.txt");
	return 0;
}

int test2()
{
    char a[] = { 'n','i','e','l','i','a','n','g','b','i','n','g' };
	LinkList<char> llist(12, a);
	cout << llist << endl;
	llist.Reverse();
	cout << llist << endl;
	int x[] = { 9,1,2,3,7,23,24,1241,52,51,2 };
	LinkList<int> xlist(11, x);
	xlist.sort(std::less<int>());
	cout << xlist << endl;

	int array[] = { 10, 1, 2, 3, 4, 5, 6, 0, 1, 2 };
	int n = sizeof(array) / sizeof(*array);
	LinkList<int> iLink0, iLink1(n, array);
	iLink0 = iLink1; iLink0.GoTop();
	cout << iLink0 << endl;
	iLink0.Skip(1);
	cout << iLink0 << endl;
	iLink0.Skip(2);
	cout << iLink0 << endl;
	iLink0.Skip(-1);
	cout << iLink0 << endl;
	cout << iLink0.NumNodes() << endl;
	return 0;
}

int main()
{
	return studentMcard("student.txt").Loop();
	/*test0();
	test1();
	test2();*/
}
