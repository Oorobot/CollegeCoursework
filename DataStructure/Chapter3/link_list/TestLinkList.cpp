#include "LinkList.h"		// 单链表类

int main(void)
{
	char c = '*';
	LinkList<double> la;
	double e;
	int i;

	while (c != '0')
	{
		cout << endl << "1. 生成单链表.";
		cout << endl << "2. 显示单链表.";
		cout << endl << "3. 取指定位置的元素.";
		cout << endl << "4. 设置元素值.";
		cout << endl << "5. 删除元素.";
		cout << endl << "6. 插入元素.";
		cout << endl << "7. 元素定位";
		cout << endl << "8. 取单链表长度";
		cout << endl << "9. 倒置";
		cout << endl << "a.删除数据元素等于e的结点";
		cout << endl << "0. 退出";
		cout << endl << "选择功能(0~8):";
		cin >> c;
		switch (c)
		{
		case '1':
			la.Clear();
			cout << endl << "输入e(e = 0时退出):";
			cin >> e;
			while (e != 0) {
				la.InsertElem(e);
				cin >> e;
			}
			break;
		case '2':
			la.Traverse(Write<double>);
			break;
		case '3':
			cout << endl << "输入元素位置:";
			cin >> i;
			if (la.GetElem(i, e) == RANGE_ERROR)
				cout << "元素不存在." << endl;
			else
				cout << "元素:" << e << endl;
			break;
		case '4':
			cout << endl << "输入位置:";
			cin >> i;
			cout << endl << "输入元素值:";
			cin >> e;
			if (la.SetElem(i, e) == RANGE_ERROR)
				cout << "位置范围错." << endl;
			else
				cout << "设置成功." << endl;
			break;
		case '5':
			cout << endl << "输入位置:";
			cin >> i;
			if (la.DeleteElem(i, e) == RANGE_ERROR)
				cout << "位置范围错." << endl;
			else
				cout << "被删除元素值:" << e << endl;
			break;
		case '6':
			la.Traverse(Write<double>);
			cout << endl << "输入位置:";
			cin >> i;
			cout << endl << "输入元素值:";
			cin >> e;
			if (la.InsertElem(i, e) == RANGE_ERROR)
				cout << "位置范围错." << endl;
			else
				cout << "成功:" << e << endl;
			la.InsertElem(e);
			la.Traverse(Write<double>);
			break;
		case '7':
			cout << endl << "输入元素的值:";
			cin >> e;
			i = la.LocateElem(e);
			if (i == 0)
				cout << "元素不存在." << endl;
			else
				cout << "元素" << e << "的序号为：" << i << endl;
			break;
		case '8':
			cout << endl << "单链表的长度为:" << la.GetLength() << endl;
			break;
		case '9':
			cout << endl << "倒置：";
			la.Reverse();
			la.Traverse(Write<double>);
			cout << endl;
			break;
		case 'a':
			cout << endl << "输入想要删除的元素e：";
			double e;
			cin >> e;
			la.DeleteE(e);
			la.Traverse(Write<double>);
			cout << endl;
			break;
		case 'b':
			cout << endl << "输入元素值:";
			cin >> e;
			la.insert(e);
			la.Traverse(Write<double>);
			break;
		}
	}

	/*int a[5] = { 2,2,6,8,10 };
	int b[6] = { 2,4,6,8,10,12 };
	LinkList<int> M(a,5),N(b,6);
	M.merge10(N);
	M.Traverse(Write<int>);
	cout << endl;*/
	system("PAUSE");        // 调用库函数system()
	return 0;               // 返回值0, 返回操作系统
}













//void merge10(LinkList L)
//{
//	Node<ElemType> *p = head;
//	Node<ElemType> *l = L.head;
//	int i = 1;
//	for (; p != nullptr; p = p->next, i++)
//	{
//		for (; p->data > l->data; l = l->next, i++) {
//			this->InsertElem(i, l->data);
//		}
//		if (i > length) break;
//	}
//	if (p == nullptr&&l != nullptr)
//	{
//		for (; l != nullptr; l = l->next)
//			this->InsertElem(l->data);
//	}
//	//Reverse();
//}
//void Reverse()
//{
//	Node<ElemType> *p, *q, *t;
//	p = this->head;
//	if (p->next != NULL) {
//		q = p->next; t = q->next;
//		p->next = NULL;
//		for (; q != NULL; t = t->next) {
//			q->next = p;
//			p = q;
//			q = t;
//			if (t == NULL) break;
//		}
//		head->next = p;
//	}
//}