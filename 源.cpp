#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <stdio.h>
using namespace std;

//������ 37
static vector<string> ReserveWord = {
	"auto", "break", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "int", "long",
	"register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "void",
	"volatile", "while", "int", "double", "float", "char","long"
};

//������ͽ�� 38
static vector<string> OperatorOrDelimiter = {
   "+","++","-", "--", "*", "/", "<", "<=", ">", ">=", "=", "==",
   "!=",";","(",")","^",",","\"","\'","#","&", "&&"
   "|","||","%","~","<<",">>","[","]","{",
   "}","\\",".","?",":","!"
};
// �ֱ���
unordered_map<string, int> SYN = unordered_map<string, int>();
// ���
vector<pair<string, int>> token;

void errorOutput(string message, int row, int col) {
	cout << message << "  row:" << row << "  col:" << col << endl;
}

int main() {
	freopen("in.txt", "r", stdin);

	// ��ʼ���ֱ���
	for (unsigned i = 0; i < ReserveWord.size(); i++)
		SYN.insert(pair<string, int>(ReserveWord[i], i + 1));
	for (unsigned i = 0; i < OperatorOrDelimiter.size(); i++)
		SYN.insert(pair<string, int>(OperatorOrDelimiter[i], i + 1 + ReserveWord.size()));

	// �������
	ifstream resourceProject;
	resourceProject.open("in.txt", ios::in);
	int row = 1, col = 0;
	while (resourceProject.peek() != EOF) {
		char ch;
		string tmp;
		// ����ǰ��Ŀո�ע���ļ��Ƿ����
		do {
			resourceProject.get(ch), col++;
			if (ch == '\n')
				row++, col = 1;
		} while (resourceProject.peek() != EOF && (ch == ' ' || ch == '\r' || ch == '\n'));
		int index = col; // ������Ϣ��
		ios::pos_type pos_index = resourceProject.tellg();
		// �����ͷΪ��ĸ��_��
		if (isalpha(ch) || ch == '_') {
			// ����ӽ��뵱ǰ�ַ���
			// ����ļ��Ѿ�����
			if (resourceProject.peek() == EOF) {
				tmp.push_back(ch);
				if (ch == '_')
					errorOutput(tmp, row, col);
				else
					token.push_back(pair<string, int>(tmp, 1000));
			}
			// ���δ����
			else {
				bool flag = false;
				while (resourceProject.peek() != EOF && (isalnum(ch) || ch == '_')) {
					tmp.push_back(ch);
					pos_index = resourceProject.tellg();
					if (ch != '_')
						flag = true;
					resourceProject.get(ch), col++;
				}
				//// ����
				//if (!flag)
				//	errorOutput(tmp, row, index);
				//else
					// ��ǰ�ַ����жϽ���
					// ��ȡ�ֱ���,������Ǳ����־����û��Զ���ı�ʶ����1000
				token.push_back(pair<string, int>(tmp, SYN.find(tmp) != SYN.end() ? SYN[tmp] : 1000));
				resourceProject.seekg(pos_index), col--;
			}
		}
		// �����ͷΪ����
		else if (isalnum(ch)) {
			// ����ļ��Ѿ�����
			if (resourceProject.peek() == EOF) {
				tmp.push_back(ch);
				token.push_back(pair<string, int>(tmp, 999));
			}
			// ���δ����
			else {
				while (resourceProject.peek() != EOF && isdigit(ch)) {
					tmp.push_back(ch);
					pos_index = resourceProject.tellg();
					resourceProject.get(ch), col++;
				}
				// ������
				if (isalpha(ch))
					errorOutput(tmp, row, index);
				else
					// ��ǰ�ַ����жϽ���
					// ��ȡ�ֱ���999
					token.push_back(pair<string, int>(tmp, 999));
				resourceProject.seekg(pos_index), col--;
			}
		}
		// ����������
		else {
			// ���Ϊ�������
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
			// =��==
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
		// TODO ������
	}
	resourceProject.close();

	// ���
	freopen("out.txt", "w", stdout);
	cout << "Token   SYN" << endl;
	for (unsigned i = 0; i < token.size(); i++)
		cout << token[i].first << "   " << token[i].second << endl;
	return 0;
}