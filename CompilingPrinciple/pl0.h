#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <string>
#include <regex>
#include <map>
using namespace std;

//nul Î´¶¨Òå×Ö·û
enum symbol {
	nul, ident, number, plus, minus, times, slash, eql, neq, lss, leq,
	gtr, geq, lparen, rparen, comma, semicolon, period, becomes,
	beginsym, callsym, constsym, dosym, endsym, ifsym, oddsym,
	procedure, readsym, thensym, varsym, whilesym, writesym,
};
#define symnum 32


vector<string> basicWord = { "begin", "call", "const", "do", "end", "if", "odd",
					   "procedure", "read", "then" ,"var" ,"while", "write", "else" };
vector<string> _operator = { "<=", ">=", ":=", "+", "-", "*", "/", "=", "#", "<", ">" };
vector<string> border = { "(", ")", ";", ",", "." };
vector<string> ob = { "<=", ">=", ":=", "+", "-", "*", "/", "=", "#", "<", ">",
						"(", ")", ";", ",", "." };
map<string, string> map_ob;

struct IdAndNum
{
	string ident;
	int num;
};
struct WordAndCode {
	string code;
	string word;
};


//
void init(map<string, string>& m);
void split(vector<string>& s, const vector<string>& c);
void trim(string& s);
void countIdent(vector<IdAndNum>& ian, string s);
bool isBasicWord(const string& s, const vector<string>& bW);
bool isOperatorOrBorder(const string& s, const vector<string>& ob);
bool isNum(const string& s);
bool isIdent(const string& s);

void outputIdentNum(string infile, string outfile);
vector<WordAndCode> outputWordAndCode(string infile, string outfile);

bool expression(vector<WordAndCode> wac, int& p,int& l);
bool term(vector<WordAndCode> wac, int& p,int& l);
bool factor(vector<WordAndCode> wac, int& p,int& l);

int stringToNum(string num);
void compute(vector<WordAndCode> wac);
WordAndCode computeWithoutParen(vector<WordAndCode> temp);
