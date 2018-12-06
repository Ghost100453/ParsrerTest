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

	vector<string> inputList;	//输入的字符串集合
	vector<Token> TokenList;	//Token集合
	vector<Token> VarList;		//变量集合
	vector<vector<Token>> TacList;	//四元式集合
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

	//程序主体
	void Program();
	State Sentence();
	//变量声明
	void VarDeclare();
	Token VarDefine();		//变量定义
	Token _VarDefine();
	Token VarChart();		//标志符表
	int tempVar;			//中间变量


	void Statement();		//赋值语句
	//State if_exp();			//if语句
	//State while_exp();		//while语句
	//State repeat_exp();		//repeat语句
	State compose_sentence();//复合语句
	State sentenceList();	//语句表

	Token rop();			//关系符
	Token ch_exp();			//字符表达式

	Token Cal_exp();		//算数表达式
	Token term();			//项
	Token factor();			//因子
	Token arithmetic();		//算术量

	State bool_exp();		//布尔表达式
	State bool_term();		//布尔项
	State bool_factor();	//布尔因子
	State bool_quan();		//布尔量


	int Tacpushback(Token, Token, Token, Token);
	void printTacList();

	void backpatch(vector<int>, int);
	
};

