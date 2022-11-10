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
	Node<T> *head, *cur_node;                 //链首指针、当前结点指针
	int num;                                 //链表中的结点数

public:
	LinkList();                              //默认构造函数
	LinkList(int n, const T *t = NULL);              //利用连续元素创建链表
	LinkList(const LinkList &list);          //拷贝构造链表
	LinkList & operator=(const LinkList &list);      //赋值运算
	virtual ~LinkList();                     //析构函数（虚函数）



	//定位当前结点
	Node<T> *GoTop();                        //绝对位置：首结点
	Node<T> *Go(int n);                      //绝对位置：第n（从0开始）个结点
	Node<T> *GoBottom();                     //绝对位置：尾结点
	Node<T> *Skip(int n = 1);                  //相对定位：偏移n（可为负数）个结点
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
	//依条件（将结点数据域强制转换成T1类型）定位/继续定位。restart！=0时从链首开始

	//判断当前结点位置及其他
	bool isEmpty() const;                     //当前链表是否为空链表
	bool isBegin() const;                     //当前结点是否为首结点
	bool isLast() const;                      //当前结点是否为尾结点
	Node<T> *Cur_node() const;                //返回当前结点的地址
	T & CurData() const;                      //引用返回当前结点本身
	int CurPos() const;                       //返回当前结点序号。0位首结点，-1为无当前结点
	int NumNodes() const;                     //返回当前链表结点数
	//插入结点
	void InsbeforeHeadNode(const T &t);       //链首结束前，成为新链首
	void InsbeforeCurNode(const T &t);        //当前结点之前
	void InsAfterCurNode(const T &t);         //当前结点之后
	void Append(const T &t);                  //尾结点后，成为新的尾结点
	//删除结点
	void DeleteCurNode();                     //当前结点
	void FreeList();                          //释放所有结点
	//修改
	void Change(const T &t);                  //修改当前数据
	//输出
	void ShowCurData() const                //输出当前结点数据
	{
		if (cur_node != nullptr) cout << cur_node->data;
	}
	void ShowList(ostream &out, int LinePerNode = 0) const;   //输出当前所有结点
	//其他链表操作
	void Reverse();                           //倒置
	template<typename Compare>
	void sort(Compare cmp)                    //排序
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
	//  链表各结点数据存入磁盘文件

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

//定位当前指针
template <typename T>
Node<T>* LinkList<T>::GoTop()
{
	cur_node = head;
	return cur_node;
}

template <typename T>
Node<T>* LinkList<T>::Go(int n)
{
	if (n > num) throw std::out_of_range("输入超出节点数。");
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
		if (n + j > num + 1) throw std::out_of_range("输入超出链表范围。");
		for (i = n; i > 0 && cur_node != NULL; i--)
			cur_node = cur_node->next;
	}
	else
	{
		j = CurPos() + 1;
		if (n + j < 1) throw std::out_of_range("输入超出链表范围。");
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
	if (j == -1) throw std::out_of_range("无当前指针，无法插入");
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
	if (j == -1) throw std::out_of_range("无当前节点");
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
		if (j == -1) throw std::out_of_range("无当前指针，无法修改");
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
//void LinkList<T>::Reverse()          //倒置
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
//}   //倒置的另一种较为复杂的可行的写法。

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
