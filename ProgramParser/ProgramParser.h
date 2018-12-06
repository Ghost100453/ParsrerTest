#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <fstream>
#include <sstream>
#include "Token.h"
#include "global.h"
#include "State.h"
using namespace std;

class ProgramParser
{
public:

	map<string, int> keywords;
	map<string, int> constantList;

	vector<string> inputList;	//������ַ�������
	vector<Token> TokenList;	//Token����
	vector<Token> VarList;		//��������
	vector<vector<Token>> TacList;	//��Ԫʽ����
	int address = 0;
	int rowNum;
	int columnNum;
	char currChar;
	int Token_index;
	Token nowToken;	

	ProgramParser();
	~ProgramParser();

	void init();

	void inputFile(string filename);
	int getToken();

	char getNextChar();
	void backTrack();
	void createTokenList();
	void printTokenList();
	int getNextToken();
	void parse();

	//��������
	void Program();
	State Sentence();
	//��������
	void VarDeclare();
	Token VarDefine();		//��������
	Token _VarDefine();
	Token VarChart();		//��־����
	int tempVar;			//�м����


	void Statement();		//��ֵ���
	//State if_exp();			//if���
	//State while_exp();		//while���
	//State repeat_exp();		//repeat���
	State compose_sentence();//�������
	State sentenceList();	//����

	Token rop();			//��ϵ��
	Token ch_exp();			//�ַ����ʽ

	Token Cal_exp();		//�������ʽ
	Token term();			//��
	Token factor();			//����
	Token arithmetic();		//������

	State bool_exp();		//�������ʽ
	State bool_term();		//������
	State bool_factor();	//��������
	State bool_quan();		//������


	int Tacpushback(Token, Token, Token, Token);
	void printTacList();

	void backpatch(vector<int>, int);
	
};

