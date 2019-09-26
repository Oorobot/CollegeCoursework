#ifndef __BUBBLE_SORT_H__
#define __BUBBLE_SORT_H__
#include "Assistance.h"			// 辅助软件包

template <class ElemType>
void BubbleSort(ElemType elem[], int n)
// 操作结果:对数组elem中的n个元素进行冒泡排序
{
	bool finish = false;
	int i = 1;
	while (i < n && !finish) {
		finish = true;
		for (int j = 0; j < n - i; j++)
			if (elem[j] > elem[j + 1]) {
				Swap(elem[j], elem[j + 1]);
				finish = false;
			}
		cout << "第" << i << "趟冒泡排序的结果：";
		Display(elem, n);		// 显示中间结果 
		i++;
	}
}

template <class ElemType>
void DBubbleSort(ElemType elem[], int n)
{
	bool finish = false;
	int i = 1;
	int k = 0;
	while (k < n - i && !finish) {
		finish = true;
		for (int j = k; j < n - i; j++)
			if (elem[j] > elem[j + 1]) {
				Swap(elem[j], elem[j + 1]);
				finish = false;
			}
		for (int j = n - i - 1; j > k; j--)
			if (elem[j - 1] > elem[j]) {
				Swap(elem[j - 1], elem[j]);
				finish = false;
			}
		cout << "第" << i << "趟冒泡排序的结果：";
		Display(elem, n);		// 显示中间结果 
		i++;
		k++;
	}
}

#endif
