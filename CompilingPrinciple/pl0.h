#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <regex>
using namespace std;

//nul Î´¶¨Òå×Ö·û
enum symbol {
	nul, ident, number,
	leq, geq, becomes, plus, minus, times, slash, lss, gtr, eql, neq,
	lparen, rparen, comma, semicolon, period,
	beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym, oddsym,
	procedure, readsym, thensym, varsym, whilesym, writesym
};
#define symnum 33
const string symbol[symnum] = {
	"nul","ident","number",
	"leq", "geq", "becomes", "plus", "minus", "times", "slash", "lss", "gtr", "eql", "neq",
	"lparen", "rparen", "comma", "semicolon", "period",
	"beginsym", "callsym", "constsym", "dosym", "elsesym", "endsym", "ifsym", "oddsym",
	"proceduresym", "readsym", "thensym", "varsym", "whilesym", "writesym"
};
const string code[symnum] = {
	"", "", "",
	"<=", ">=", ":=", "+", "-", "*", "/", "<",  ">", "=", "#",
	"(", ")", ",", ";", ".",
	"begin", "call", "const", "do", "else", "end", "if", "odd",
	"procedure", "read", "then" ,"var" ,"while", "write"
};

struct IdAndNum
{
	string ident;
	int num;
};
struct CodeAndWord {
	int code;
	string word;
};
struct four {
	int code;
	string opr1;
	string opr2;
	string res;
};

void split(vector<string>& s, int start, int end);
void trim(string& s);
void countIdent(vector<IdAndNum>& ian, string s);
int keyword(const string& s);
int delimiterAndOperator(const char& c);
bool isNum(const string& s);
bool isIdent(const string& s);


void outputIdentNum(string infile, string outfile);
vector<CodeAndWord> outputCodeAndWord(string infile, string outfile);

bool expression(vector<CodeAndWord> wac, size_t& p, int& l);
bool term(vector<CodeAndWord> wac, size_t& p, int& l);
bool factor(vector<CodeAndWord> wac, size_t& p, int& l);

void compute(vector<CodeAndWord> caw);
CodeAndWord computeWithoutParen(vector<CodeAndWord> temp);

vector<four> Intermediate(vector<CodeAndWord> caw);
CodeAndWord IntermediateWithoutParen(vector<CodeAndWord> temp, vector<four>& f);
