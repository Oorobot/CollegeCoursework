#include "string/Linklist.h"


//1
typedef LinkList<char> String;
typedef Node<char> CNode;

bool find_char(const String &str, unsigned int len, char c)
{
	CNode *head = str.GetHead();
	head = head->next;
	int length = len;
	while (head != nullptr && (length--))
	{
		if (head->data == c)
			return true;
		head = head->next;
	}
	return false;
}

int find(const String &s, const String &t)
{
	CNode *shead = s.GetHead();
	shead = shead->next;
	for (int i = 0; shead != nullptr; shead = shead->next, i++)
	{
		if (find_char(s, i, shead->data))
			continue;
		if (!find_char(t, t.GetLength(), shead->data))
			return i;
	}
	return -1;
}

//2
bool delete_string(char *str, int m, int t)
{
	int s = std::strlen(str);
	if (m < s || t < (s - m))
		return false;
	std::strncpy(str + s, str, m);
	std::strncpy(str + s + m, str + m + t, s - m - t);
	return true;
}

int main()
{
#define STR(arr) String(arr, sizeof(arr) / sizeof(char))
	//find_test78
	assert(find(STR(""), STR("")) == -1);
	assert(find(STR("abcdef"), STR("abcdef")) == -1);
	assert(find(STR(""), STR("aaaaaa")) == -1);
	assert(find(STR("aaabbbccc"), STR("abc")) == -1);

	assert(find(STR("aaaaa"), STR("")) == 0);
	assert(find(STR("abcdef"), STR("ab")) == 2);
	assert(find(STR("abcdef"), STR("abcd")) == 4);
	assert(find(STR("abcdef"), STR("abcde")) == 5);
	//std::cout << find(STR("abcdabcdabcd"), STR("bca"));
	char s[100] = { 0 }, t[100] = { 0 };
	std::cin >> s >> t;
	std::cout << find(String(s, std::strlen(s)), String(t, std::strlen(t)));
	//delete_string_test
	char *ptr = new char[100];
	std::memset(ptr, 0, 100 * sizeof(char));
	std::strncpy(ptr, "abcdefghijklmnopqrstuvwxyz", 27);
	delete_string(ptr, 10, 10);
	std::cout << ptr;
	delete[] ptr;
	system("pause");
	return 0;
}
