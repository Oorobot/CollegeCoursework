#include "BubbleSort.h"			// ð�������㷨

int main(void)
{
	int a[] = { 55, 63, 38, 21, 63, 92, 16, 46, 87, 72 };
	int n = 10;
	cout << "����ǰ:";
	Display(a, n);				// ��ʾ����a������ 
	BubbleSort(a, n);			// ð������
	cout << "�����:";
	Display(a, n);				// ��ʾa����a������ 

	//int b[] = { 55, 63, 38, 21, 63, 92, 16, 46, 87, 72 };
	int b[] = { 9,8,7,6,5,4,3,2,1,0 };
	cout << endl << "����ǰ:";
	Display(b, n);				// ��ʾ����a������ 
	DBubbleSort(b, n);			// ð������
	cout << "�����:";
	Display(b, n);				// ��ʾa����a������ 
	system("PAUSE");			// ���ÿ⺯��system()
	return 0;					// ����ֵ0, ���ز���ϵͳ
}
