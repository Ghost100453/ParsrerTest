#include "stdafx.h"
#include "Token.h"

Token::Token()
{
	content = "";
	type = -1;
	var_type = UNDEFINE;
}


Token::~Token()
{
}

Token::Token(string t_content, int t_type, int t_constantid) {
	content = t_content;
	type = t_type;
	constant_id = t_constantid;
	var_type = UNDEFINE;
}

void Token::setContent(string t_content) {
	content = t_content;
}

void Token::setVarType(VarType v) {
	var_type = v;
}

void Token::setInfo(int row, int column, string err_msg) {
	rowNum = row;
	columnNum = column;
	error_msg = err_msg;
}

string Token::printInfo() {
	string temp = "";
	if (constant_id == 0) {
		//cout << "(" << setw(2) << type << ",-)";
		temp = "(" + to_string(type) + ",-)";
	}
	else {
		//cout << "(" << setw(2) << type << ","<<constant_id<<")";
		temp = "(" + to_string(type) + "," + to_string(constant_id) + ")";
	}
	return temp;
}

VarType Token::getVarType() {
	return var_type;
}

int Token::getType() {
	return type;
}

string Token::getContent() {
	return content;
}
