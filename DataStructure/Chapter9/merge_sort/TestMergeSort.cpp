#include "MergeSort.h"				// πÈ≤¢≈≈–ÚÀ„∑®

int main(void)
{
	int a[] = {55, 63, 38, 21, 63, 92, 16, 46, 87, 72};
	int n = 10;
	cout << "≈≈–Ú«∞:";
	Display(a, n);					
	MergeSort(a, n);				// πÈ≤¢≈≈–Ú
	cout << "≈≈–Ú∫Û:";
	Display(a, n);					

	system("PAUSE");				
	return 0;						
}
