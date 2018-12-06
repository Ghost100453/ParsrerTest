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

	int rowNum;			//�к�
	int columnNum;		//�к�
	int type;			//���ձ����
	int constant_id;	//��������id

	string content;		//����
	VarType var_type;	//�������ͣ���integer��bool��char����
	string error_msg;

	void setVarType(VarType);
	void setContent(string);
	void setInfo(int row, int column, string err_msg = ""); //����Token��Ϣ����Ҫ��Ϣ��Token���������еڼ��У��ڼ��У����ڱ���

	string printInfo();	//��ӡ��Ϣ�����������Ԫʽ

	int getType();
	string getContent();
	VarType getVarType();
};

