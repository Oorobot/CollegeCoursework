#include "SeqList.h"	// 顺序表类



int main(void)
{
	char c = '*';
	SeqList<int> la(6);
	int e, i;
	Status sta;

	while (c != '0')
	{
		cout << endl << "1. 生成线性表.";
		cout << endl << "2. 显示线性表.";
		cout << endl << "3. 取指定元素.";
		cout << endl << "4. 设置元素值.";
		cout << endl << "5. 删除元素.";
		cout << endl << "6. 插入元素.";
		cout << endl << "7. 元素定位.";
		cout << endl << "8. 求线性表长度.";
		cout << endl << "9. 倒置线性表.";
		cout << endl << "a. 删除表中多余的重复元素.";
		cout << endl << "b. 排序（1-升序，0-降序）.";
		cout << endl << "0. 退出";
		cout << endl << "选择功能(0~a):";
		cin >> c;
		switch (c) 	{
			case '1':
				la.Clear();
				sta = SUCCESS;
				cout << endl << "输入e(e = 0时退出):";
				cin >> e;
				while (e != 0 && sta != OVER_FLOW)	{
					sta = la.InsertElem(e);
					if (sta == OVER_FLOW)
						cout << "线性表已满." << endl;
					else
						cin >> e;
				}
				break;
			case '2':
				la.Traverse(Write<int>);
				break;
			case '3':
				cout << endl << "输入元素位置:";
				cin >> i;
				if (la.GetElem(i, e) == NOT_PRESENT)
					cout << "元素不存储." << endl;
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
				cout << endl << "输入位置:";
				cin >> i;
				cout << endl << "输入元素值:";
				cin >> e;
				sta = la.InsertElem(i, e);

				if (sta == RANGE_ERROR)
					cout << "位置范围错." << endl;
				else if (sta == OVER_FLOW)
					cout << "线性表已满." << endl;
				else
					cout << "插入成功." << endl;
				break;
			case '7':
				cout << endl << "输入元素值:";
				cin >> e;
				i = la.LocateElem(e);
				if (i != 0)
					cout << "元素" << e << "的序号为：" << i << endl;
				else
					cout << "元素" << e << "不存在。"  << endl;
				break;
			case '8':
				cout << endl << "线性表的长度为:" << la.GetLength() << endl;
				break;
			case '9':
				la.Reverse();
				cout << endl << "线性表倒置：";
				la.Traverse(Write<int>);
				break;
			case 'a':
				la.DeleteDuplicateElem();
				cout << endl << "删除多余的重复元素：";
				la.Traverse(Write<int>);
				break;
			case 'b':
				int b = 1;
				cout << endl << "请选择排序方式（0或1）:";
				cin >> b;
				la.Sort(b);
				if(b!=0) cout << endl << "升序：";
				else cout << endl << "降序：";
				la.Traverse(Write<int>);
				break;
		}
	}

	system("PAUSE");        		// 调用库函数system()，暂停程序运行 



	int a[5] = { 1,2,5,6,-1 };
	int b[6] = { 99,-99,152,36,0,3 };
	int s, t;
	SeqList<int> l(a, 5, 10), L(b, 6, 9);
	l.Sort(1);
	L.Sort(1);
	l.Merge(L);
	l.Traverse(Write<int>);
	cout << endl << "输出删除的范围：";
	cin >> s >> t;
	l.DeleteRange(s, t);
	if (l.DeleteRange(s, t) == RANGE_ERROR) {
		cout << "选择的删除范围错误(左<右),无法进行。" << endl;
	}
	else l.Traverse(Write<int>);
	cout << endl << 0;


	return 0;               		// 返回操作系统
}


