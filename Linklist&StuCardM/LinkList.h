#ifndef LINKLIST_H
#define LINKLIST_H
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

template <typename T> class LinkList;

template <typename T> class Node
{
public:
	Node(const T &t = 0) : data(t), next(NULL) {}
	friend class LinkList<T>;
	T data;
private:
	Node *next;
};

template <typename T> class LinkList
{
private:
	Node<T> *head, *cur_node;                 //����ָ�롢��ǰ���ָ��
	int num;                                 //�����еĽ����

public:
	LinkList();                              //Ĭ�Ϲ��캯��
	LinkList(int n, const T *t = NULL);              //��������Ԫ�ش�������
	LinkList(const LinkList &list);          //������������
	LinkList & operator=(const LinkList &list);      //��ֵ����
	virtual ~LinkList();                     //�����������麯����



	//��λ��ǰ���
	Node<T> *GoTop();                        //����λ�ã��׽��
	Node<T> *Go(int n);                      //����λ�ã���n����0��ʼ�������
	Node<T> *GoBottom();                     //����λ�ã�β���
	Node<T> *Skip(int n = 1);                  //��Զ�λ��ƫ��n����Ϊ�����������
	template <typename T1>
	Node<T> *Locate(const T1 &t, bool restart = false)
	{
		if (restart != 0)
		{
			for (Node<T> *p = head; p != NULL; p = p->next)
			{
				if (p->data == (T)t) { cur_node = p; return cur_node; }
			}
		}
		else
		{
			for (Node<T> *p = cur_node->next; p != NULL; p = p->next)
			{
				if (p->data == (T)t) { cur_node = p; return cur_node; }
			}
		}
		cur_node = head;
		return NULL;
	}
	//�������������������ǿ��ת����T1���ͣ���λ/������λ��restart��=0ʱ�����׿�ʼ

	//�жϵ�ǰ���λ�ü�����
	bool isEmpty() const;                     //��ǰ�����Ƿ�Ϊ������
	bool isBegin() const;                     //��ǰ����Ƿ�Ϊ�׽��
	bool isLast() const;                      //��ǰ����Ƿ�Ϊβ���
	Node<T> *Cur_node() const;                //���ص�ǰ���ĵ�ַ
	T & CurData() const;                      //���÷��ص�ǰ��㱾��
	int CurPos() const;                       //���ص�ǰ�����š�0λ�׽�㣬-1Ϊ�޵�ǰ���
	int NumNodes() const;                     //���ص�ǰ��������
	//������
	void InsbeforeHeadNode(const T &t);       //���׽���ǰ����Ϊ������
	void InsbeforeCurNode(const T &t);        //��ǰ���֮ǰ
	void InsAfterCurNode(const T &t);         //��ǰ���֮��
	void Append(const T &t);                  //β���󣬳�Ϊ�µ�β���
	//ɾ�����
	void DeleteCurNode();                     //��ǰ���
	void FreeList();                          //�ͷ����н��
	//�޸�
	void Change(const T &t);                  //�޸ĵ�ǰ����
	//���
	void ShowCurData() const                //�����ǰ�������
	{
		if (cur_node != nullptr) cout << cur_node->data;
	}
	void ShowList(ostream &out, int LinePerNode = 0) const;   //�����ǰ���н��
	//�����������
	void Reverse();                           //����
	template<typename Compare>
	void sort(Compare cmp)                    //����
	{
		unsigned int i;
		if (isEmpty()) return;
		Node<T>** a = new Node<T>*[num], *p = head;
		for (i = 0; p != NULL; p = p->next, i++)
			a[i] = p;
		std::sort(a, a + num, [&](const Node<T>* lhs, const Node<T>* rhs)
		{
			return cmp(lhs->data, rhs->data);
		});
		for (i = 0; i < num - 1; i++)
			a[i]->next = a[i + 1];
		a[num - 1]->next = NULL;
		head = a[0];
		delete[] a;
	}


	void Save(const char *filename, ios_base::openmode OpenMode = ios::out)
	{
		ofstream outfile(filename);
		ShowList(outfile, 0);
		outfile.close();
	}
	//  �����������ݴ�������ļ�

};
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
LinkList<T>::LinkList()
{
	head = NULL;
	cur_node = NULL;
	num = 0;
}

template <typename T>
LinkList<T>::LinkList(int n, const T *t) :num(n), head(NULL), cur_node(NULL)
{
	Node<T> *p = NULL;
	for (int i = n - 1; i >= 0; i--)
	{
		p = new Node<T>;
		if (t != NULL)
			p->data = t[i];
		p->next = head;
		head = p;
	}
	cur_node = head;
}

template <typename T>
LinkList<T>::LinkList(const LinkList &list) :head(NULL), cur_node(NULL), num(0)
{
	*this = list;
}

template <typename T>
LinkList<T>::~LinkList()
{
	FreeList();
}

template <typename T>
LinkList<T> & LinkList<T>::operator=(const LinkList &list)
{
	Node<T> *curNode = NULL;
	if (this == &list) return *this;
	FreeList();
	for (Node<T> *p = list.head; p != NULL; p = p->next)
	{
		Append(p->data);
		if (p == list.cur_node)
			curNode = cur_node;
	}
	cur_node = curNode;
	num = list.num;
	return *this;
}

//��λ��ǰָ��
template <typename T>
Node<T>* LinkList<T>::GoTop()
{
	cur_node = head;
	return cur_node;
}

template <typename T>
Node<T>* LinkList<T>::Go(int n)
{
	if (n > num) throw std::out_of_range("���볬���ڵ�����");
	Node<T> *p;
	p = head;
	for (int i = 0; i < n - 1; i++)
		p = p->next;
	cur_node = p;
	return cur_node;
}

template <typename T>
Node<T>* LinkList<T>::GoBottom()
{
	Node<T> *p;
	for (p = head; p->next != NULL; p = p->next);
	cur_node = p;
	return cur_node;
}

template <typename T>
Node<T>* LinkList<T>::Skip(int n)
{
	Node<T> *p, *q; int i = 0, j, x;
	if (n >= 0)
	{
		j = CurPos() + 1;
		if (n + j > num + 1) throw std::out_of_range("���볬������Χ��");
		for (i = n; i > 0 && cur_node != NULL; i--)
			cur_node = cur_node->next;
	}
	else
	{
		j = CurPos() + 1;
		if (n + j < 1) throw std::out_of_range("���볬������Χ��");
		if (n + j == 1) { cur_node = head; return cur_node; }
		for (x = 1, q = head; x <= n + j; x++, q = q->next)
			cur_node = q;
	}
	return cur_node;
}



template <typename T>
bool LinkList<T>::isEmpty() const
{
	return num == 0;
}

template <typename T>
bool LinkList<T>::isBegin() const
{
	return cur_node == head;

}

template <typename T>
bool LinkList<T>::isLast() const
{
	return cur_node->next == NULL;
}

template <typename T>
Node<T>* LinkList<T>::Cur_node() const
{
	return this->cur_node;
}

template <typename T>
T & LinkList<T>::CurData() const
{
	if (cur_node != nullptr) return this->cur_node->data;
}

template <typename T>
int LinkList<T>::CurPos() const
{
	Node<T> *p; int i = 0;
	int flag = -1;
	for (p = head, i = 0; p != NULL; p = p->next, i++)
	{
		if (p == cur_node) { flag = 0; return i; }
	}
	if (flag == -1)  return flag;
}

template <typename T>
int LinkList<T>::NumNodes() const
{
	/*return num;*/
	int i; Node<T> *p;
	if (head == NULL) { i = 0; return i; }
	for (p = head, i = 0; p != NULL; p = p->next, i++)
		;
	return i;
}

template <typename T>
void LinkList<T>::InsbeforeHeadNode(const T& t)
{
	Node<T> *p = new Node<T>(t);
	p->next = head;
	head = p;
	num++;
}

template <typename T>
void LinkList<T>::InsbeforeCurNode(const T& t)
{
	Node<T> *p = new Node<T>;
	p->data = t;
	Node<T> *q = NULL;
	int j = CurPos();
	if (j == -1) throw std::out_of_range("�޵�ǰָ�룬�޷�����");
	for (q = head; q->next != cur_node; q = q->next);
	q->next = p;
	p->next = cur_node;
	num++;
}

template <typename T>
void LinkList<T>::InsAfterCurNode(const T& t)
{
	Node<T> *p = new Node<T>;
	p->data = t;
	Node<T> *q = NULL;
	int j = CurPos();
	if (j == -1) throw std::out_of_range("�޵�ǰ�ڵ�");
	cur_node->next = p;
	p->next = q;
	num++;
}

template <typename T>
void LinkList<T>::Append(const T& t)
{
	Node<T> *p;
	Node<T> *q = new Node<T>(t);
	q->next = NULL;
	if (head == NULL)
	{
		head = q;
		num++;
	}
	else
	{
		for (p = head; p->next != NULL; p = p->next);
		p->next = q;
		num++;
	}
}

template <typename T>
void LinkList<T>::DeleteCurNode()
{
	int j = CurPos();
	Node<T> *p;
	Node<T>* q = cur_node;
	if (j == -1)  cur_node = NULL;
	if (cur_node == head)
	{
		Node<T>*p = head; head = head->next; cur_node = NULL; delete p;
	}
	else
	{
		for (p = head; p->next != cur_node; p = p->next);
		p->next = q->next;
		delete q; cur_node = NULL;
	}
}

template <typename T>
void LinkList<T>::FreeList()
{
	Node<T> *p;
	while (head != NULL)
	{
		p = head;
		head = head->next;
		delete p;
	}
	num = 0;
	cur_node = NULL;
}

template <typename T>
void LinkList<T>::Change(const T& t)
{
	if (cur_node != nullptr)
	{
		int j = CurPos();
		if (j == -1) throw std::out_of_range("�޵�ǰָ�룬�޷��޸�");
		else cur_node->data = t;
	}
}

template <typename T>
void LinkList<T>::ShowList(ostream &out, int LinePerNode) const
{
	if (LinePerNode == 0)
	{
		out << "head";
		for (Node<T> *p = head; p != NULL; p = p->next)
			out << (p == cur_node ? " ->*" : " -> ") << p->data;
		out << " -> NULL.";
	}
	else
	{
		for (Node<T> *p = head; p != NULL; p = p->next)
			out << (p == cur_node ? "*\t" : " \t") << p->data << endl;
	}
}

//template <typename T>
//void LinkList<T>::Reverse()          //����
//{
//	unsigned int i;
//	if (isEmpty())  return;
//	Node<T>** a = new Node<T>*[num],*p = head;
//	for (i = 0; p != NULL; p = p->next, i++)
//		a[i] = p;
//	std::reverse(a, a + num);
//	for (i = 0; i < num - 1; i++)
//		a[i]->next = a[i + 1];
//	a[num - 1]->next = NULL;
//	head = a[0];
//	delete[] a;
//}   //���õ���һ�ֽ�Ϊ���ӵĿ��е�д����

template <typename T>
void LinkList<T>::Reverse()
{

	Node<T>* p = NULL, *_head = NULL, *q = head;
	for (int i = 0; i <= num - 1; i++, q = q->next)
	{
		p = new Node<T>;
		if (q != NULL)
			p->data = q->data;
		p->next = _head;
		_head = p;
	}
	head = p;
	cur_node = head;
}

template <typename T> ostream & operator<<(ostream &out, const LinkList<T> &link)
{
	link.ShowList(out, 0);
	return out;
}




#endif // LINKLIST_H
