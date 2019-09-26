#include "LinkQueue.h"	// ��������

int main(void)
{
	char c = '1';
	LinkQueue<int> qa;
	int x;

	while (c != '0')
	{
		cout << endl << "1. ���ɶ���.";
		cout << endl << "2. ��ʾ����.";
		cout << endl << "3. �����.";
		cout << endl << "4. ������.";
		cout << endl << "5. ȡ����ͷ.";
		cout << endl << "6.ǰn�����������.";
		cout << endl << "0. �˳�";
		cout << endl << "ѡ����(0~5):";
		cin >> c;
		switch (c) {
		case '1':
			qa.Clear();
			cout << endl << "����e(e = 0ʱ�˳�)";
			cin >> x;
			while (x != 0) {
				qa.EnQueue(x);
				cin >> x;
			}
			break;
		case '2':
			cout << endl;
			qa.Traverse(Write<int>);
			break;
		case '3':
			cout << endl << "����Ԫ��ֵ:";
			cin >> x;
			qa.EnQueue(x);
			break;
		case '4':
			qa.DelQueue(x);
			cout << endl << "��ͷԪ��ֵΪ " << x << " ." << endl;
			break;
		case '5':
			qa.GetHead(x);
			cout << endl << "��ͷԪ��ֵΪ " << x << " ." << endl;
			break;
		case '6':
			int n;
			LinkQueue<int> que;
			cout << endl << "����n��";
			cin >> n;
			cout << endl;

			int pre = 0, cur = 0;
			que.EnQueue(1);
			for (int i = 1; i <= n; i++)
			{
				cout << setw(2 * n-i) << " ";
				que.EnQueue(1);
				for (int j = 0; j < i; j++)
				{
					if (j)
						que.EnQueue(cur + pre);
					que.DelQueue(pre);
					que.GetHead(cur);
					std::cout << pre << ' ';
				}
				que.EnQueue(1);
				cout << endl;				
			}
		}
	}

	system("PAUSE");
	return 0;
}


