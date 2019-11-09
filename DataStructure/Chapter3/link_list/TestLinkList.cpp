#include "LinkList.h"		// ��������

int main(void)
{
	char c = '*';
	LinkList<double> la;
	double e;
	int i;

	while (c != '0')
	{
		cout << endl << "1. ���ɵ�����.";
		cout << endl << "2. ��ʾ������.";
		cout << endl << "3. ȡָ��λ�õ�Ԫ��.";
		cout << endl << "4. ����Ԫ��ֵ.";
		cout << endl << "5. ɾ��Ԫ��.";
		cout << endl << "6. ����Ԫ��.";
		cout << endl << "7. Ԫ�ض�λ";
		cout << endl << "8. ȡ��������";
		cout << endl << "9. ����";
		cout << endl << "a.ɾ������Ԫ�ص���e�Ľ��";
		cout << endl << "0. �˳�";
		cout << endl << "ѡ����(0~8):";
		cin >> c;
		switch (c)
		{
		case '1':
			la.Clear();
			cout << endl << "����e(e = 0ʱ�˳�):";
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
			cout << endl << "����Ԫ��λ��:";
			cin >> i;
			if (la.GetElem(i, e) == RANGE_ERROR)
				cout << "Ԫ�ز�����." << endl;
			else
				cout << "Ԫ��:" << e << endl;
			break;
		case '4':
			cout << endl << "����λ��:";
			cin >> i;
			cout << endl << "����Ԫ��ֵ:";
			cin >> e;
			if (la.SetElem(i, e) == RANGE_ERROR)
				cout << "λ�÷�Χ��." << endl;
			else
				cout << "���óɹ�." << endl;
			break;
		case '5':
			cout << endl << "����λ��:";
			cin >> i;
			if (la.DeleteElem(i, e) == RANGE_ERROR)
				cout << "λ�÷�Χ��." << endl;
			else
				cout << "��ɾ��Ԫ��ֵ:" << e << endl;
			break;
		case '6':
			la.Traverse(Write<double>);
			cout << endl << "����λ��:";
			cin >> i;
			cout << endl << "����Ԫ��ֵ:";
			cin >> e;
			if (la.InsertElem(i, e) == RANGE_ERROR)
				cout << "λ�÷�Χ��." << endl;
			else
				cout << "�ɹ�:" << e << endl;
			la.InsertElem(e);
			la.Traverse(Write<double>);
			break;
		case '7':
			cout << endl << "����Ԫ�ص�ֵ:";
			cin >> e;
			i = la.LocateElem(e);
			if (i == 0)
				cout << "Ԫ�ز�����." << endl;
			else
				cout << "Ԫ��" << e << "�����Ϊ��" << i << endl;
			break;
		case '8':
			cout << endl << "������ĳ���Ϊ:" << la.GetLength() << endl;
			break;
		case '9':
			cout << endl << "���ã�";
			la.Reverse();
			la.Traverse(Write<double>);
			cout << endl;
			break;
		case 'a':
			cout << endl << "������Ҫɾ����Ԫ��e��";
			double e;
			cin >> e;
			la.DeleteE(e);
			la.Traverse(Write<double>);
			cout << endl;
			break;
		case 'b':
			cout << endl << "����Ԫ��ֵ:";
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
	system("PAUSE");        // ���ÿ⺯��system()
	return 0;               // ����ֵ0, ���ز���ϵͳ
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