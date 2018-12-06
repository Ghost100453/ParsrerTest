#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "global.h"
using namespace std;

class Token
{
public:
	Token();
	Token(string, int, int = 0);
	~Token();

	int rowNum;			//行号
	int columnNum;		//列号
	int type;			//参照编码表
	int constant_id;	//变量或常量id

	string content;		//内容
	VarType var_type;	//变量类型，有integer、bool、char三种
	string error_msg;

	void setVarType(VarType);
	void setContent(string);
	void setInfo(int row, int column, string err_msg = ""); //设置Token信息，主要信息是Token处于内容中第几行，第几列，用于报错

	string printInfo();	//打印信息，用于输出二元式

	int getType();
	string getContent();
	VarType getVarType();
};

