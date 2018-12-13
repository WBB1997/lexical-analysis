#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <stdio.h>
using namespace std;

//保留符 37
static vector<string> ReserveWord = {
	"auto", "break", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "int", "long",
	"register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "void",
	"volatile", "while", "int", "double", "float", "char","long"
};

//运算符和界符 38
static vector<string> OperatorOrDelimiter = {
   "+","++","-", "--", "*", "/", "<", "<=", ">", ">=", "=", "==",
   "!=",";","(",")","^",",","\"","\'","#","&", "&&"
   "|","||","%","~","<<",">>","[","]","{",
   "}","\\",".","?",":","!"
};
// 种别码
unordered_map<string, int> SYN = unordered_map<string, int>();
// 输出
vector<pair<string, int>> token;

void errorOutput(string message, int row, int col) {
	cout << message << "  row:" << row << "  col:" << col << endl;
}

int main() {
	freopen("in.txt", "r", stdin);

	// 初始化种别码
	for (unsigned i = 0; i < ReserveWord.size(); i++)
		SYN.insert(pair<string, int>(ReserveWord[i], i + 1));
	for (unsigned i = 0; i < OperatorOrDelimiter.size(); i++)
		SYN.insert(pair<string, int>(OperatorOrDelimiter[i], i + 1 + ReserveWord.size()));

	// 输入程序
	ifstream resourceProject;
	resourceProject.open("in.txt", ios::in);
	int row = 1, col = 0;
	while (resourceProject.peek() != EOF) {
		char ch;
		string tmp;
		// 抛弃前面的空格，注意文件是否结束
		do {
			resourceProject.get(ch), col++;
			if (ch == '\n')
				row++, col = 1;
		} while (resourceProject.peek() != EOF && (ch == ' ' || ch == '\r' || ch == '\n'));
		int index = col; // 错误信息列
		ios::pos_type pos_index = resourceProject.tellg();
		// 如果开头为字母或‘_’
		if (isalpha(ch) || ch == '_') {
			// 先添加进入当前字符串
			// 如果文件已经结束
			if (resourceProject.peek() == EOF) {
				tmp.push_back(ch);
				if (ch == '_')
					errorOutput(tmp, row, col);
				else
					token.push_back(pair<string, int>(tmp, 1000));
			}
			// 如果未结束
			else {
				bool flag = false;
				while (resourceProject.peek() != EOF && (isalnum(ch) || ch == '_')) {
					tmp.push_back(ch);
					pos_index = resourceProject.tellg();
					if (ch != '_')
						flag = true;
					resourceProject.get(ch), col++;
				}
				//// 错误
				//if (!flag)
				//	errorOutput(tmp, row, index);
				//else
					// 当前字符串判断结束
					// 获取种别码,如果不是保留字就是用户自定义的标识符，1000
				token.push_back(pair<string, int>(tmp, SYN.find(tmp) != SYN.end() ? SYN[tmp] : 1000));
				resourceProject.seekg(pos_index), col--;
			}
		}
		// 如果开头为数字
		else if (isalnum(ch)) {
			// 如果文件已经结束
			if (resourceProject.peek() == EOF) {
				tmp.push_back(ch);
				token.push_back(pair<string, int>(tmp, 999));
			}
			// 如果未结束
			else {
				while (resourceProject.peek() != EOF && isdigit(ch)) {
					tmp.push_back(ch);
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
				}
				// 错误处理
				if (isalpha(ch))
					errorOutput(tmp, row, index);
				else
					// 当前字符串判断结束
					// 获取种别码999
					token.push_back(pair<string, int>(tmp, 999));
				resourceProject.seekg(pos_index), col--;
			}
		}
		// 如果是运算符
		else {
			// 如果为单运算符
			if (ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^' || ch == ',' ||
				ch == '\"' || ch == '\'' || ch == '~' || ch == '#' || ch == '%' || ch == '[' || ch == ']' ||
				ch == '{' || ch == '}' || ch == '\\' || ch == '.' || ch == '?' || ch == ':') {
				tmp.push_back(ch);
				token.push_back(pair<string, int>(tmp, SYN[tmp]));
			}
			// -, --
			else if (ch == '-') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					tmp.push_back(ch);
					if (ch != '-')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
			// +, ++
			else if (ch == '+') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					if (ch != '+')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
			// <, <=, <<
			else if (ch == '<') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					tmp.push_back(ch);
					if (ch != '=' && ch != '<')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
			// >, >= , >>
			else if (ch == '>') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					tmp.push_back(ch);
					if (ch != '=' && ch != '>')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
			// =，==
			else if (ch == '=') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					tmp.push_back(ch);
					if (ch != '=')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
			// &, &&
			else if (ch == '&') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					tmp.push_back(ch);
					if (ch != '&')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
			// !, !=
			else if (ch == '!') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					tmp.push_back(ch);
					if (ch != '=')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
			// |, ||
			else if (ch == '|') {
				tmp.push_back(ch);
				if (resourceProject.peek() == EOF)
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				else {
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
					tmp.push_back(ch);
					if (ch != '|')
						resourceProject.seekg(pos_index), col--;
					else
						tmp.push_back(ch);
					token.push_back(pair<string, int>(tmp, SYN[tmp]));
				}
			}
		}
		// TODO 错误处理
	}
	resourceProject.close();

	// 输出
	freopen("out.txt", "w", stdout);
	cout << "Token   SYN" << endl;
	for (unsigned i = 0; i < token.size(); i++)
		cout << token[i].first << "   " << token[i].second << endl;
	return 0;
}