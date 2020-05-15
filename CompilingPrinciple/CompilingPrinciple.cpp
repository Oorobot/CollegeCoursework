#include "pl0.h"

int main()
{
	init(map_ob);
	//outputIdentNum("case01.txt", "output1-1.txt");
	//outputIdentNum("case02.txt", "output1-2.txt");
	//outputIdentNum("case03.txt", "output1-3.txt");
	//outputIdentNum("case04.txt", "output1-4.txt");
	//outputIdentNum("case05.txt", "output1-5.txt");
	//outputWordAndCode("case01.txt", "output2-1.txt");
	//outputWordAndCode("case02.txt", "output2-2.txt");
	//outputWordAndCode("case03.txt", "output2-3.txt");
	//outputWordAndCode("case04.txt", "output2-4.txt");
	//outputWordAndCode("case05.txt", "output2-5.txt");
	/*int p = 0, l = 0;
	vector<WordAndCode> wac = outputWordAndCode("3-8.txt", "3.txt");
	if (expression(wac, p, l))
		cout << "语法正确" << endl;*/
		/*string s = "3-";
		char j = '0';
		for (int i = 0; i <= 9; i++) {
			int p = 0,l=0;
			printf("\n");
			vector<WordAndCode> wac = outputWordAndCode(s + j + ".txt", "3.txt");
			j = j + 1;
			if (expression(wac, p, l))
				cout << "语法正确" << endl;
			else
				cout << "语法错误" << endl;
		}*/
	vector<WordAndCode> wac = outputWordAndCode("./txt/4-2.txt", "./txt/4.txt");
	compute(wac);
}


void init(map<string, string>& m)
{
	m.insert(pair<string, string>("+", "plus"));
	m.insert(pair<string, string>("-", "minus"));
	m.insert(pair<string, string>("*", "times"));
	m.insert(pair<string, string>("/", "slash"));
	m.insert(pair<string, string>("=", "eql"));
	m.insert(pair<string, string>("#", "neq"));
	m.insert(pair<string, string>("<", "lss"));
	m.insert(pair<string, string>("<=", "leq"));
	m.insert(pair<string, string>(">", "gtr"));
	m.insert(pair<string, string>(">=", "geq"));
	m.insert(pair<string, string>(":=", "becomes"));
	m.insert(pair<string, string>("(", "lparen"));
	m.insert(pair<string, string>(")", "rparen"));
	m.insert(pair<string, string>(",", "comma"));
	m.insert(pair<string, string>(";", "semicolon"));
	m.insert(pair<string, string>(".", "period"));
}

void split(vector<string>& s, const vector<string>& c) {
	for (string cstr : c) {
		size_t size = s.size();
		for (int i = 0; i < size; i++) {
			string str = s.front();
			s.erase(s.begin());
			size_t pos1 = 0, pos2;
			pos2 = str.find(cstr);
			while (string::npos != pos2) {
				s.push_back(str.substr(pos1, pos2 - pos1));
				pos1 = pos2 + cstr.size();
				pos2 = str.find(cstr, pos1);
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
	if (ident.size() != 0 && !isBasicWord(ident, basicWord) && isIdent(ident)) {
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
		split(word, _operator);
		split(word, border);
		vector<string> _split = { " " };
		split(word, _split);
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

vector<WordAndCode> outputWordAndCode(string infile, string outfile)
{
	vector<WordAndCode> wac;
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
				if (isBasicWord(t, basicWord)) {
					cout << "(" << t + "sym" << "," << t << ")" << endl;
					out << "(" << t + "sym" << "," << t << ")" << endl;
					wac.push_back({ t + "sym",t });
				}
				else {
					cout << "(" << "ident" << "," << t << ")" << endl;
					out << "(" << "ident" << "," << t << ")" << endl;
					wac.push_back({ "iden",t });
				}
			}
			else if (s[i] >= '0' && s[i] <= '9') {
				token[count++] = s[i];
				i++;
				while (s[i] >= '0' && s[i] <= '9') {
					token[count++] = s[i];
					i++;
				}
				token[count] = '\0';
				cout << "(" << "number" << "," << token << ")" << endl;
				out << "(" << "number" << "," << token << ")" << endl;
				wac.push_back({ "number", token });
			}
			else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '=' ||
				s[i] == '#' || s[i] == '(' || s[i] == ')' || s[i] == ';' || s[i] == '.' || s[i] == ',') {
				token[0] = s[i];
				i++;
				token[1] = '\0';
				string t(token);
				cout << "(" << map_ob.find(t)->second << "," << token << ")" << endl;
				out << "(" << map_ob.find(t)->second << "," << token << ")" << endl;
				wac.push_back({ map_ob.find(t)->second, token });
			}
			else if (s[i] == '>' || s[i] == '<' || s[i] == ':') {
				token[0] = s[i];
				i++;
				if (s[i] == '=') {
					token[1] = s[i];
					i++;
					token[2] = '\0';
					string t(token);
					cout << "(" << map_ob.find(t)->second << "," << token << ")" << endl;
					out << "(" << map_ob.find(t)->second << "," << token << ")" << endl;
					wac.push_back({ map_ob.find(t)->second, token });
				}
				else {
					token[1] = '\0';
					string t(token);
					auto search = map_ob.find(t);
					if (search != map_ob.end()) {
						cout << "(" << search->second << "," << token << ")" << endl;
						out << "(" << search->second << "," << token << ")" << endl;
						wac.push_back({ search->second, token });
					}
					else {
						cout << "(  " << "未定义字符" << "  ,  " << token << "  )  " << endl;
						out << "(  " << "未定义字符" << "  ,  " << token << "  )  " << endl;
						wac.push_back({ "nul", token });
					}
				}
			}
			else if (s[i] == ' ' || s[i] == '\t') i++;
			else {
				token[0] = s[i];
				token[1] = '\0';
				i++;
				cout << "(  " << "未定义字符" << "  ,  " << token << "  )  " << endl;
				out << "(  " << "未定义字符" << "  ,  " << token << "  )  " << endl;
				wac.push_back({ "nul", token });
			}
		}
	}
	out.close();
	return wac;
}

bool isBasicWord(const string& s, const vector<string>& bW)
{
	for (string o : bW) {
		if (o == s) return true;
	}
	return false;
}

bool isOperatorOrBorder(const string& s, const vector<string>& ob)
{
	for (string o : ob) {
		if (o == s) return true;
	}
	return false;
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


bool expression(vector<WordAndCode> wac, int& p, int& l)
{
	if (wac[p].code == "minus" || wac[p].code == "plus") {
		cout << wac[p].word << "   ";
		p++;
	}
	if (!term(wac, p, l))
		return false;
	while (p < wac.size() && (wac[p].code == "minus" || wac[p].code == "plus")) {
		cout << wac[p].word << " ";
		p++;
		if (!term(wac, p, l))
			return false;
	}
	if (p > 0 && p < wac.size() && (wac[p].code == "iden" || wac[p].code == "number") && (wac[p - 1].code == "iden" || wac[p - 1].code == "number")) {
		cout << wac[p].word << endl;
		cout << "缺乏运算符" << endl;
		return false;
	}
	p++;
	if (p > 0 && p < wac.size() && (wac[p].code == "iden" || wac[p].code == "number") && wac[p - 1].code == "rparen") {
		cout << wac[p].word << endl;
		cout << "缺乏运算符" << endl;
		return false;
	}
	p--;
	if (p > 0 && p < wac.size() && wac[p].code == "lparen" && (wac[p - 1].code == "iden" || wac[p - 1].code == "number")) {
		cout << wac[p].word << endl;
		cout << "缺乏运算符" << endl;
		return false;
	}
	if (p < wac.size() && wac[p].code == "rparen") {
		l--;
		if (l < 0) {
			cout << wac[p].word << endl;
			cout << "多出一个右括号" << endl;
			return false;
		}
		l++;
	}
	if (p > 0 && p < wac.size() && wac[p].code == "nul") {
		cout << "非法字符" << endl;
		return false;
	}
	return true;
}

bool term(vector<WordAndCode> wac, int& p, int& l) {
	if (!factor(wac, p, l))
		return false;
	while (p < wac.size() && (wac[p].code == "times" || wac[p].code == "slash"))
	{
		cout << wac[p].word << " ";
		p++;
		if (!factor(wac, p, l))
			return false;
	}
	return true;
}

bool factor(vector<WordAndCode> wac, int& p, int& l) {
	while (p < wac.size()) {
		if (wac[p].code == "iden") {
			cout << wac[p].word << " ";
			p++;
			return true;
		}
		else if (wac[p].code == "number") {
			cout << wac[p].word << " ";
			p++;
			return true;
		}
		else if (wac[p].code == "lparen") {
			cout << wac[p].word << " ";
			p++;
			l++;
			if (!expression(wac, p, l))
				return false;
			if (p < wac.size() && wac[p].code == "rparen") {
				cout << wac[p].word << " ";
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
			if (wac[p].code == "nul")
				cout << wac[p].word << "非法字符" << endl;
			else if (wac[p].code == "rparen") {
				cout << wac[p].word << "()不能为空" << endl;
			}
			else
				cout << wac[p].word << "缺乏标识符或数字" << endl;
			return false;
		}
	}
	return true;
}

int stringToNum(string num)
{
	return atoi(num.c_str());
}

void compute(vector<WordAndCode> wac)
{
	while (!wac.empty()) {
		int lp = -1, rp = -1;
		for (int i = 0; i < wac.size(); i++) {
			if (wac[i].code == "lparen")
				lp = i;
			if (wac[i].code == "rparen") {
				rp = i;
				break;
			}
		}
		if (lp > -1 && rp > -1) {
			int len = (int)rp - (int)lp;
			vector<WordAndCode> temp;
			for (int i = 1; i < len; i++) {
				temp.push_back(wac[lp + i]);
			}
			wac.erase(wac.begin() + lp + 1, wac.begin() + rp + 1);
			for (WordAndCode w : wac) {
				cout << w.word << endl;
			}
			WordAndCode result = computeWithoutParen(temp);
			wac[lp].code = result.code;
			wac[lp].word = result.word;
			if (result.code == "nul") {
				wac.clear();
				cout << "表达式有误无法计算" << endl;
			}
		}
		int flag = 0;
		for (auto x : wac) {
			if (x.code == "lparen") {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			WordAndCode result = computeWithoutParen(wac);
			wac.clear();
			if (result.code == "nul") {
				wac.clear();
				cout << "表达式有误无法计算" << endl;
			}
			else {
				cout << result.word << endl;
			}
		}
	}
}

WordAndCode computeWithoutParen(vector<WordAndCode> temp)
{
	vector<WordAndCode> op, num;
	WordAndCode result;
	result.code = "nul";
	WordAndCode begin = temp.front();
	if (begin.code == "minus") {
		temp.erase(temp.begin());
		begin = temp.front();
		int x = stringToNum(begin.word);
		x = 0 - x;
		temp[0].word = to_string(x);
	}
	for (auto t : temp) {
		if (t.code == "number")
			num.push_back(t);
		if (t.code == "plus" || t.code == "minus" || t.code == "times" || t.code == "slash")
			op.push_back(t);
	}
	if (op.size() != num.size() - 1)
		return result;
	for (size_t i = 0; i < num.size() - 1; i++) {
		if (!op.empty() && op[i].code == "times") {
			int x = stringToNum(num[i].word);
			int y = stringToNum(num[i + 1].word);
			num.erase(num.begin() + i);
			num[i].word = to_string(x * y);
			op.erase(op.begin() + i);
			i--;
		}
		else if (!op.empty() && op[i].code == "slash") {
			int x = stringToNum(num[i].word);
			int y = stringToNum(num[i + 1].word);
			num.erase(num.begin() + i);
			if (y == 0) {
				return result;
			}
			num[i].word = to_string(x / y);
			op.erase(op.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < num.size() - 1; i++) {
		if (!op.empty() && op[i].code == "plus") {
			int x = stringToNum(num[i].word);
			int y = stringToNum(num[i + 1].word);
			num.erase(num.begin() + i);
			num[i].word = to_string(x + y);
			op.erase(op.begin() + i);
			i--;
		}
		else if (!op.empty() && op[i].code == "minus") {
			int x = stringToNum(num[i].word);
			int y = stringToNum(num[i + 1].word);
			num.erase(num.begin() + i);
			num[i].word = to_string(x - y);
			op.erase(op.begin() + i);
			i--;
		}
	}
	result = num.front();
	return result;
}
