#include "pl0.h"

int main()
{
	/*outputIdentNum("./txt/case01.txt", "./txt/1-1.txt");
	outputIdentNum("./txt/case02.txt", "./txt/1-2.txt");
	outputIdentNum("./txt/case03.txt", "./txt/1-3.txt");
	outputIdentNum("./txt/case04.txt", "./txt/1-4.txt");
	outputIdentNum("./txt/case05.txt", "./txt/1-5.txt");
	outputCodeAndWord("./txt/case01.txt", "./txt/2-1.txt");
	outputCodeAndWord("./txt/case02.txt", "./txt/2-2.txt");
	outputCodeAndWord("./txt/case03.txt", "./txt/2-3.txt");
	outputCodeAndWord("./txt/case04.txt", "./txt/2-4.txt");
	outputCodeAndWord("./txt/case05.txt", "./txt/2-5.txt");*/
	/*string str3 = "./txt/3-";
	string str4 = "./txt/4-";
	char j = '0';
	for (int i = 0; i <= 9; i++) {
		size_t p = 0;
		int l = 0;
		printf("\n");
		vector<CodeAndWord> caw = outputCodeAndWord(str3 + j + ".txt", "./txt/output.txt");
		j = j + 1;
		if (expression(caw, p, l)) {
			cout << "语法正确" << endl;
			compute(caw);
		}
		else
			cout << "语法错误" << endl;
	}*/
	/*for (int i = 0; i < symnum; i++) {
		cout << symbol[i] << "  " << code[i] << endl;
	}*/
}

void split(vector<string>& s, int start, int end) {
	string blank = " ";
	for (int j = start; j <= end + 1; j++) {
		size_t size = s.size();
		for (int i = 0; i < size; i++) {
			string str = s.front();
			s.erase(s.begin());
			size_t pos1 = 0, pos2;
			if (j == end + 1) {
				pos2 = str.find(blank);
				while (string::npos != pos2) {
					s.push_back(str.substr(pos1, pos2 - pos1));
					pos1 = pos2 + blank.size();
					pos2 = str.find(blank, pos1);
				}
			}
			else {
				pos2 = str.find(code[j]);
				while (string::npos != pos2) {
					s.push_back(str.substr(pos1, pos2 - pos1));
					pos1 = pos2 + code[j].size();
					pos2 = str.find(code[j], pos1);
				}
			}
			if (pos1 != str.size())
				s.push_back(str.substr(pos1));
		}
	}
}

void trim(string& s) {
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
}

void countIdent(vector<IdAndNum>& ian, string ident) {
	trim(ident);
	if (ident.size() != 0 && keyword(ident) == symbol::ident && isIdent(ident)) {
		if (ian.size() == 0) {
			IdAndNum i = { ident,1 };
			ian.push_back(i);
		}
		else {
			int flag = 0;
			for (IdAndNum& i : ian) {
				if (i.ident == ident) {
					i.num++;
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				IdAndNum i = { ident,1 };
				ian.push_back(i);
			}
		}
	}
}

void outputIdentNum(string infile, string outfile) {
	vector<IdAndNum> ian;
	string s;
	ifstream in(infile);
	if (!in.is_open()) {
		cout << "Error opening file";
		exit(1);
	}
	while (!in.eof()) {
		getline(in, s);
		cout << s << endl;
		trim(s);
		if (!s.empty() && s.back() == ';') {
			s = s.substr(0, s.size() - 1);
		}
		for (char& c : s) {
			c = tolower(c);
		}
		vector<string> word;
		word.push_back(s);
		split(word, symbol::leq, symbol::period);
		for (string w : word) {
			countIdent(ian, w);
		}
	}
	ofstream out(outfile);
	if (out.is_open()) {
		for (IdAndNum i : ian) {
			out << "(" << i.ident << "," << i.num << ")" << endl;
		}
		out.close();
	}
	for (IdAndNum i : ian) {
		cout << "(" << i.ident << "," << i.num << ")" << endl;
	}
}

vector<CodeAndWord> outputCodeAndWord(string infile, string outfile)
{
	vector<CodeAndWord> caw;
	string s;
	ifstream in(infile);
	if (!in.is_open()) {
		cout << "Error opening file";
		exit(1);
	}
	ofstream out(outfile);

	while (!in.eof()) {
		getline(in, s);
		cout << s << endl;
		trim(s);
		char* token = new char[50];
		int count = 0;
		size_t i = 0;
		for (char& ch : s)
			ch = tolower(ch);
		while (i < s.size()) {
			count = 0;
			for (int j = 0; j < 20; j++)
				token[j] = '\0';
			if (s[i] >= 'a' && s[i] <= 'z') {
				token[count++] = s[i];
				i++;
				while (s[i] >= 'a' && s[i] <= 'z' || s[i] >= '0' && s[i] <= '9') {
					token[count++] = s[i];
					i++;
				}
				token[count] = '\0';
				string t(token);
				out << "(" << symbol[keyword(t)] << "," << t << ")" << endl;
				caw.push_back({ keyword(t), t });
			}
			else if (s[i] >= '0' && s[i] <= '9') {
				token[count++] = s[i];
				i++;
				while (s[i] >= '0' && s[i] <= '9') {
					token[count++] = s[i];
					i++;
				}
				token[count] = '\0';
				out << "(" << symbol[number] << "," << token << ")" << endl;
				caw.push_back({ number, token });
			}
			else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '=' ||
				s[i] == '#' || s[i] == '(' || s[i] == ')' || s[i] == ';' || s[i] == '.' || s[i] == ',') {
				token[0] = s[i];
				i++;
				token[1] = '\0';
				string t(token);
				out << "(" << symbol[delimiterAndOperator(t[0])] << "," << token << ")" << endl;
				caw.push_back({ delimiterAndOperator(t[0]), token });
			}
			else if (s[i] == '>' || s[i] == '<' || s[i] == ':') {
				token[0] = s[i];
				i++;
				if (s[i] == '=') {
					token[1] = s[i];
					i++;
					token[2] = '\0';
					string t(token);
					out << "(" << symbol[delimiterAndOperator(t[0]) + 1] << "," << token << ")" << endl;
					caw.push_back({ delimiterAndOperator(t[0]) + 1, token });
				}
				else {
					token[1] = '\0';
					string t(token);
					if (delimiterAndOperator(t[0]) != becomes - 1) {
						out << "(" << symbol[delimiterAndOperator(t[0])] << "," << token << ")" << endl;
						caw.push_back({ delimiterAndOperator(t[0]), token });
					}
					else {
						out << "(  " << symbol[nul] << "  ,  " << token << "  )  " << endl;
						caw.push_back({ nul, token });
					}
				}
			}
			else if (s[i] == ' ' || s[i] == '\t') i++;
			else {
				token[0] = s[i];
				token[1] = '\0';
				i++;
				out << "(  " << symbol[nul] << "  ,  " << token << "  )  " << endl;
				caw.push_back({ nul, token });
			}
		}
	}
	out.close();
	return caw;
}

int keyword(const string& s)
{
	int i = beginsym, j = writesym;
	int k = 0;
	do {
		k = (i + j) / 2;
		if (s.compare(code[k]) <= 0) {
			j = k - 1;
		}
		if (s.compare(code[k]) >= 0) {
			i = k + 1;
		}

	} while (i <= j);
	if (i - 1 > j) {
		return k;
	}
	else {
		return ident;
	}
}

int delimiterAndOperator(const char& c)
{
	switch (c)
	{
	case '+':
		return symbol::plus;
	case '-':
		return symbol::minus;
	case '*':
		return symbol::times;
	case '/':
		return symbol::slash;
	case '=':
		return symbol::eql;
	case '#':
		return symbol::neq;
	case '<':
		return symbol::lss;
	case '>':
		return symbol::gtr;
	case ':':
		return symbol::becomes - 1;
	case '(':
		return symbol::lparen;
	case ')':
		return symbol::rparen;
	case ',':
		return symbol::comma;
	case ';':
		return symbol::semicolon;
	case '.':
		return symbol::period;
	default:
		break;
	}
	return symbol::nul;
}

bool isNum(const string& str)
{
	regex num("\\d*");
	return regex_match(str, num);
}

bool isIdent(const string& s) {
	regex pattern("([a-z]){1}\\w*");
	return regex_match(s, pattern);
}


bool expression(vector<CodeAndWord> caw, size_t& p, int& l)
{
	if (caw[p].code == symbol::minus || caw[p].code == symbol::plus) {
		cout << caw[p].word << "   ";
		p++;
	}
	if (!term(caw, p, l))
		return false;
	while (p < caw.size() && (caw[p].code == symbol::minus || caw[p].code == symbol::plus)) {
		cout << caw[p].word << " ";
		p++;
		if (!term(caw, p, l))
			return false;
	}
	if (p > 0 && p < caw.size() && (caw[p].code == ident || caw[p].code == number) && (caw[p - 1].code == ident || caw[p - 1].code == number)) {
		cout << caw[p].word << endl;
		cout << "缺乏运算符" << endl;
		return false;
	}
	p++;
	if (p > 0 && p < caw.size() && (caw[p].code == ident || caw[p].code == number) && caw[p - 1].code == rparen) {
		cout << caw[p].word << endl;
		cout << "缺乏运算符" << endl;
		return false;
	}
	p--;
	if (p > 0 && p < caw.size() && caw[p].code == lparen && (caw[p - 1].code == ident || caw[p - 1].code == number)) {
		cout << caw[p].word << endl;
		cout << "缺乏运算符" << endl;
		return false;
	}
	if (p < caw.size() && caw[p].code == rparen) {
		l--;
		if (l < 0) {
			cout << caw[p].word << endl;
			cout << "多出一个右括号" << endl;
			return false;
		}
		l++;
	}
	if (p > 0 && p < caw.size() && caw[p].code == nul) {
		cout << "非法字符" << endl;
		return false;
	}
	return true;
}

bool term(vector<CodeAndWord> caw, size_t& p, int& l) {
	if (!factor(caw, p, l))
		return false;
	while (p < caw.size() && (caw[p].code == times || caw[p].code == slash))
	{
		cout << caw[p].word << " ";
		p++;
		if (!factor(caw, p, l))
			return false;
	}
	return true;
}

bool factor(vector<CodeAndWord> caw, size_t& p, int& l) {
	while (p < caw.size()) {
		if (caw[p].code == ident || caw[p].code == number) {
			cout << caw[p].word << " ";
			p++;
			return true;
		}
		else if (caw[p].code == lparen) {
			cout << caw[p].word << " ";
			p++;
			l++;
			if (!expression(caw, p, l))
				return false;
			if (p < caw.size() && caw[p].code == rparen) {
				cout << caw[p].word << " ";
				p++;
				l--;
				return true;
			}
			else
			{
				cout << "缺少右括号" << endl;
				return false;
			}
		}
		else
		{
			if (caw[p].code == nul)
				cout << caw[p].word << "非法字符" << endl;
			else if (caw[p].code == rparen) {
				cout << caw[p].word << "()不能为空" << endl;
			}
			else
				cout << caw[p].word << "缺乏标识符或数字" << endl;
			return false;
		}
	}
	return true;
}

void compute(vector<CodeAndWord> caw)
{
	while (!caw.empty()) {
		int lp = -1, rp = -1;
		for (int i = 0; i < caw.size(); i++) {
			if (caw[i].code == lparen)
				lp = i;
			if (caw[i].code == rparen) {
				rp = i;
				break;
			}
		}
		if (lp > -1 && rp > -1) {
			int len = rp - lp;
			vector<CodeAndWord> temp;
			for (int i = 1; i < len; i++) {
				int j = lp + i;
				temp.push_back(caw[j]);
			}
			caw.erase(caw.begin() + lp + 1, caw.begin() + rp + 1);
			CodeAndWord result = computeWithoutParen(temp);
			caw[lp].code = result.code;
			caw[lp].word = result.word;
			if (result.code == nul) {
				caw.clear();
				cout << "表达式有误无法计算" << endl;
			}
		}
		int flag = 0;
		for (auto x : caw) {
			if (x.code == lparen) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			CodeAndWord result = computeWithoutParen(caw);
			caw.clear();
			if (result.code == nul) {
				caw.clear();
				cout << "表达式有误无法计算" << endl;
			}
			else {
				cout << result.word << endl;
			}
		}
	}
}

CodeAndWord computeWithoutParen(vector<CodeAndWord> temp)
{
	vector<CodeAndWord> op, num;
	CodeAndWord result;
	result.code = nul;
	CodeAndWord begin = temp.front();
	if (begin.code == symbol::minus) {
		temp.erase(temp.begin());
		begin = temp.front();
		int x = stoi(begin.word);
		x = 0 - x;
		temp[0].word = to_string(x);
	}
	for (auto t : temp) {
		if (t.code == number)
			num.push_back(t);
		if (t.code == symbol::plus || t.code == symbol::minus ||
			t.code == symbol::times || t.code == symbol::slash)
			op.push_back(t);
	}
	if (op.size() != num.size() - 1)
		return result;
	for (size_t i = 0; i < num.size() - 1; i++) {
		if (!op.empty() && op[i].code == symbol::times) {
			int x = stoi(num[i].word);
			int y = stoi(num[i + 1].word);
			num.erase(num.begin() + i);
			num[i].word = to_string(x * y);
			op.erase(op.begin() + i);
			i--;
		}
		else if (!op.empty() && op[i].code == symbol::slash) {
			int x = stoi(num[i].word);
			int y = stoi(num[i + 1].word);
			num.erase(num.begin() + i);
			if (y == 0) {
				return result;
			}
			num[i].word = to_string(x / y);
			op.erase(op.begin() + i);
			i--;
		}
	}
	for (size_t i = 0; i < num.size() - 1; i++) {
		if (!op.empty() && op[i].code == symbol::plus) {
			int x = stoi(num[i].word);
			int y = stoi(num[i + 1].word);
			num.erase(num.begin() + i);
			num[i].word = to_string(x + y);
			op.erase(op.begin() + i);
			i--;
		}
		else if (!op.empty() && op[i].code == symbol::minus) {
			int x = stoi(num[i].word);
			int y = stoi(num[i + 1].word);
			num.erase(num.begin() + i);
			num[i].word = to_string(x - y);
			op.erase(op.begin() + i);
			i--;
		}
	}
	result = num.front();
	return result;
}