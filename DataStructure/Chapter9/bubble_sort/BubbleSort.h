#ifndef __BUBBLE_SORT_H__
#define __BUBBLE_SORT_H__
#include "Assistance.h"			// ���������

template <class ElemType>
void BubbleSort(ElemType elem[], int n)
// �������:������elem�е�n��Ԫ�ؽ���ð������
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
		cout << "��" << i << "��ð������Ľ����";
		Display(elem, n);		// ��ʾ�м��� 
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
		cout << "��" << i << "��ð������Ľ����";
		Display(elem, n);		// ��ʾ�м��� 
		i++;
		k++;
	}
}

#endif
