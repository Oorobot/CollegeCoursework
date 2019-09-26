#include <iostream>
#include "LinkList.h"
#include "studentMcard.h"

using namespace std;


//测试定位当前节点函数   测试可行。
int test0()
{
	int array[] = { 10, 1, 2, 3, 4, 5, 6, 0, 1, 2 };
	int n = sizeof(array) / sizeof(*array);
	LinkList<int> iLink0, iLink1(n, array), iLink2(iLink1);
	cout << "默认构造结点数为0的链表" << endl;
	cout << "iLink0: " << iLink0 << endl;
	cout << "是否为空链表？" << iLink0.isEmpty();
	cout << "\niLink0当前节点个数:" << iLink0.NumNodes();
	cout << "\n返回iLink0当前节点序号：" << iLink0.CurPos();

	cout << "\n利用数组元素构造的链表" << endl;
	cout << "iLink1: " << iLink1 << endl;

	cout << "\n拷贝构造的链表" << endl;
	cout << "iLink2: " << iLink2 << endl;
	cout << "返回iLink2当前节点序号：" << iLink2.CurPos();

	iLink0 = iLink1; iLink0.GoTop(); iLink0.Skip(4);
	iLink0.DeleteCurNode();
	cout << "\n赋值运算 iLink0 = iLink1\n" << "iLink0: " << iLink0 << endl;
	iLink0.InsbeforeHeadNode(999);
	cout << "\n头指针前插入\n" << "iLink0: " << iLink0 << endl;

	iLink1.GoBottom();
	cout << "\n cur_node在尾节点" << endl;
	cout << "iLink1: " << iLink1 << endl;
	cout << "当前指针是否为首节点：" << iLink1.isBegin();
	cout << "   当前指针是否为尾节点：" << iLink1.isLast();
	cout << "   返回iLink1当前节点序号：" << iLink1.CurPos();
	iLink1.InsAfterCurNode(9999);
	cout << "\n在当前指针后插入" << "iLink1:" << iLink1 << endl;
	iLink1.InsbeforeCurNode(8888);
	cout << "\n在当前指针前插入" << "iLink1:" << iLink1 << endl;
	iLink1.Skip(-3);
	cout << "\n cur_node向后移动3个位置" << endl;
	cout << "iLink1: " << iLink1 << endl;
	cout << "返回iLink1当前节点序号：" << iLink1.CurPos();
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
	cout << "\n 超出范围，回到尾指针" << endl;
	cout << "iLink1: " << iLink1 << endl;
	iLink1.GoTop();
	cout << "\n cur_node回到头指针" << endl;
	cout << "iLink1: " << iLink1 << endl;
	int a = 1;
	iLink1.Locate(a, 1);
	cout << "\n cur_node定位1" << endl;
	cout << "iLink1: " << iLink1 << endl;
	iLink1.Locate(a, 0);
	cout << "\n cur_node从现指针位置开始继续定位1" << endl;
	cout << "iLink1: " << iLink1 << endl;
	iLink1.Locate(11, 0);
	cout << "\n 无法定位到所需的节点时,返回到头节点" << endl;
	cout << "iLink1: " << iLink1 << endl;


	cout << "当前节点数据为：";
	iLink1.ShowCurData();
	cout << "  当前节点地址为：";
	cout << iLink1.Cur_node();
	cout << "  引用返回当前节点数据：";
	cout << iLink1.CurData();
	iLink1.Change(90);
	cout << "\n当前节点数据为：";
	iLink1.ShowCurData();
	cout << "\n当前节点个数:" << iLink0.NumNodes();

	return 0;
}

int test1()
{
	LinkList<double> dLink(3);										// 创建一条链表（三个结点，数据域不可预知）
	cout << "\n转换构造的链表（数据域的值不可预知）\n" << dLink << endl;

	char cArray[] = { 'a', 'b', 'c', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	LinkList<char> cLinkArray[] = { LinkList<char>(3, cArray), LinkList<char>(2, cArray + 3), LinkList<char>(6, cArray + 5) };
	// 创建链表的数组，有3个元素（即3条链表）
	cout << cLinkArray[1] << endl;
	cout << "\n输出链表数组各元素\n";
	for (int i = 0; i < 3; i++)
		cout << cLinkArray[i] << endl;

	LinkList<LinkList<char> > cLLink0(3);							// 创建一条链表，有3个结点（每个结点也是一条链表）
	cout << "\n3条空链表的链表\n" << cLLink0 << endl;

	LinkList<LinkList<char> > cLLink1(3, cLinkArray);				// 创建链表的链表，有3个结点
	cout << "\n3条链表的链表\n" << cLLink1 << endl;

	cout << "\n将链表数据存入文件output.txt" << endl;
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
