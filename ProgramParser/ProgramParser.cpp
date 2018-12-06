#include "stdafx.h"
#include "ProgramParser.h"

ProgramParser::ProgramParser(){
	this->init();
	tempVar = 0;
}

ProgramParser::~ProgramParser(){
}

void ProgramParser::init() {
	keywords["and"] = 1;	keywords["array"] = 2;	keywords["begin"] = 3;
	keywords["bool"] = 4;	keywords["call"] = 5;	keywords["case"] = 6;
	keywords["char"] = 7;	keywords["constant"] = 8;	keywords["dim"] = 9;
	keywords["do"] = 10;	keywords["else"] = 11;	keywords["end"] = 12;
	keywords["false"] = 13;	keywords["for"] = 14;	keywords["if"] = 15;
	keywords["input"] = 16;	keywords["integer"] = 17;	keywords["not"] = 18;
	keywords["of"] = 19;	keywords["or"] = 20;	keywords["output"] = 21;
	keywords["procedure"] = 22;	keywords["program"] = 23;	keywords["read"] = 24;
	keywords["real"] = 25;	keywords["repeat"] = 26;	keywords["set"] = 27;
	keywords["stop"] = 28;	keywords["then"] = 29;	keywords["to"] = 30;
	keywords["true"] = 31;	keywords["until"] = 32;	keywords["var"] = 33;
	keywords["while"] = 34;	keywords["write"] = 35;	keywords["("] = 39;
	keywords[")"] = 40;		keywords["*"] = 41;		keywords["*/"] = 42;
	keywords["+"] = 43;		keywords[","] = 44;		keywords["-"] = 45;
	keywords["."] = 46;		keywords[".."] = 47;	keywords["/"] = 48;
	keywords["/*"] = 49;	keywords[":"] = 50;		keywords[":="] = 51;
	keywords[";"] = 52;		keywords["<"] = 53;		keywords["<="] = 54;
	keywords["<>"] = 55;	keywords["="] = 56;		keywords[">"] = 57;
	keywords[">="] = 58;	keywords["["] = 59;		keywords["]"] = 60;
}

void ProgramParser::inputFile(string filename) {
	this->inputList.resize(0);
	this->TokenList.resize(0);
	ifstream fin(filename,  ios::in);
	if (!fin) {
		cout << "File not exist!" << endl;
		system("pause");
		exit(0);
	}
	string temp;
	while (getline(fin, temp)) {
		inputList.push_back(temp + "\n");
	}
	for (string s : inputList) {
		cout << s;
	}
}

int ProgramParser::getToken() {
	int currTokenType = 0;
	stateType nowState = START;
	string currToken = "";
	string error_msg = "";
	bool is_comment = false;
	bool is_error = false;
	while (nowState != DONE) {
		char currChar = getNextChar();
		bool add_flag = true;
		switch (nowState) 
		{
			case START: {
				if (isdigit(currChar)) {
					nowState = ISNUM;
				}
				else if (isalpha(currChar)) {
					nowState = ISID;
				}
				else if (currChar == '\'') {
					nowState = ISQUOTE;
				}
				else if (currChar == '/') {
					nowState = ISSLASH;
				}
				else if (currChar == ':') {
					nowState = ISASSIGN;
				}
				else if (currChar == '.') {
					nowState = ISDOT;
				}
				else if (currChar == '<') {
					nowState = ISLT;
				}
				else if (currChar == '>') {
					nowState = ISGT;
				}
				else if (currChar == 0) {
					add_flag = false;
					nowState = DONE;
					currTokenType = 0;
				}
				else if (currChar == '\n' || currChar == ' ' || currChar == '\t') {
					add_flag = false;
				}
				else { //说明是 + -  [ ] ( ) , ; 这些
					nowState = DONE;
					string tempkey = "";
					tempkey += currChar;
					if (keywords.find(tempkey) != keywords.end()) { //说明找到了
						currTokenType = keywords[tempkey];
					}
					else {
						currTokenType = -1;
						cout << "输入错误" << endl;
					}
				}
				break;
			}
			case ISLT: { //<
				nowState = DONE;
				if (currChar == '=' || currChar == '>') { // <=  or <>
					currTokenType = keywords[currToken + currChar];
				}
				else {
					currTokenType = keywords["<"];
					add_flag = false;
					backTrack();
				}
				break;
			}
			case ISGT: { // >
				nowState = DONE;
				if (currChar == '=') {
					currTokenType = keywords[">="];
				}
				else {
					nowState = DONE;
					currTokenType = keywords[">"];
					add_flag = false;
					backTrack();
				}
				break;
			}
			case ISDOT: {
				nowState = DONE;
				if (currChar == '.') {
					currTokenType = keywords[".."];
				}
				else {
					currTokenType = keywords["."];
					add_flag = false;
					backTrack();
				}
				break;
			}
			case ISASSIGN: {
				nowState = DONE;
				if (currChar == '=') {
					currTokenType = keywords[":="];
				}
				else {
					currTokenType = keywords[":"];
					add_flag = false;
					backTrack();
				}
				break;
			}
			case ISSLASH: {
				if (currChar == '*') {
					nowState = ISLEFTCOMM;
					currTokenType = keywords["*/"];
				}
				else {
					nowState = DONE;
					currTokenType = keywords["/"];
					add_flag = false;
					backTrack();
;				}
				break;
			}
			case ISLEFTCOMM: {
				if (currChar == '*') {
					nowState = ISRIGHTCOMM;
					is_comment = true;
				}
				break;
			}
			case ISRIGHTCOMM: {
				if (currChar == '/') {
					is_comment = true;
					nowState = DONE;
					currTokenType = keywords["*/"];
				}
				else {
					nowState = ISLEFTCOMM;
				}
				break;
			}
			case ISNUM: {
				if (!isdigit(currChar)) {
					if (!isalpha(currChar)) { //既不是数字也不是字母，代表数字的输入完成，进入下一个token
						nowState = DONE;
						backTrack();
						add_flag = false;
						currTokenType = 37;
					}
					else { //数字开头加字母 错误输入
						nowState = DONE;
						add_flag = false;
						currTokenType = -1;  //代表错误
						cout << "变量以字母开头！" << endl;
					}
				}
				break;
			}
			case ISID: {
				if (isdigit(currChar) || isalpha(currChar)) {
					nowState = ISID;
				}
				else {
					nowState = DONE;
					add_flag = false;
					backTrack();
					currTokenType = 36;  //统一做标识符处理，后续再判断是否是保留字
				}
				break;
			}
			case ISQUOTE: {
				if (currChar == '\'') { //字符常数
					nowState = DONE;
					currTokenType = 38;
				}
				else if (currChar == '\n') {
					nowState = DONE;
					currTokenType = -1;
					is_error = true;
				}
				break;
			}
		}

		if (add_flag) {
			currToken += currChar;
		}

		if (nowState == DONE) {
			if (currTokenType == 36) {
				if (keywords.find(currToken) != keywords.end()) {
					currTokenType = keywords[currToken];
				}
			}
			Token token = Token(currToken, currTokenType);
			token.setInfo(rowNum + 1, columnNum + 1 - currToken.length());
			if (currTokenType >= 36 && currTokenType <= 38) {
				if (constantList.find(currToken) == constantList.end()) {
					token.constant_id = constantList.size() + 1;
					constantList[currToken] = token.constant_id;
				}
				else {
					token.constant_id = constantList[currToken];
				}
			}
			if (!is_comment && !is_error && currTokenType != 0) {  //先不处理错误输入
				TokenList.push_back(token);
			}
		}
	}
	return currTokenType;
}

char ProgramParser::getNextChar() {
	if (columnNum >= inputList[rowNum].length()) {
		rowNum += 1;
		columnNum = 0;
	}
	if (rowNum >= inputList.size()) {
		currChar = 0;
		return 0;
	}
	currChar = inputList[rowNum][columnNum++];
	return currChar;
}

void ProgramParser::backTrack() {
	columnNum--;
}

void ProgramParser::createTokenList() {
	rowNum = 0;
	columnNum = 0;
	while (getToken() != 0);
	cout << TokenList.size() << endl;

	for (auto ele : TokenList) {
		cout<<ele.printInfo()<< " "<<ele.getContent()<<endl;
	}
}

void ProgramParser::printTokenList() {
	int i = 0;
	for (int index = 0; index < TokenList.size(); ++index) {
		cout << setw(15) << TokenList[index].printInfo();
		i++;
		if (i == 5){
			cout << endl;
			i = 0;
		}
	}
	cout << endl;
}

int ProgramParser::getNextToken() {
	if (Token_index < TokenList.size()) {
		nowToken = TokenList[Token_index];
		Token_index++;
	}
	for (auto e : this->VarList) {
		if (e.getContent() == nowToken.getContent())
			e.setVarType(nowToken.getVarType());
	}
	return Token_index-1;
}

void ProgramParser::parse() {
	Token_index = 0;
	getNextToken();
	Program();
}

void ProgramParser::Program() {
	Token temp = nowToken;
	if (nowToken.getType() != 23) { //判断第一个输入是否是"Program"
		cout << "Missing Program!" << endl;
		system("pause");
		exit(0);
	}	
	getNextToken();
	if (nowToken.getType() != 36) { //判断是否是标识符
		cout << "Missing Program Name!" << endl;
		system("pause");
		exit(0);
	}
	else{
		Tacpushback(temp, nowToken, Token("-", 0), Token("-", 0));
	}
	getNextToken();
	if (nowToken.getType() != 52) { //;
		cout << "Missing keyword ';' !" << endl;
		system("pause");
		exit(0);
	}
	getNextToken();
	VarDeclare();			//变量声明
	Sentence();			//复合句
	if (nowToken.getType() != 46) {	//结束点
		cout << "Missing keyword '.' !" << endl;
		system("pause");
		exit(0);
	}
	temp = Token("sys", 0);
	Tacpushback(temp, Token("-",0), Token("-", 0), Token("-", 0));
	cout << "Parse complete" << endl;
}

State ProgramParser::Sentence() {
	State temp;
	getNextToken();
	switch (nowToken.getType())
	{
		case 3: { //begin 复合语句
			compose_sentence();
			break;
		}
		case 36: {  //赋值句
			Statement();
			temp.chain.clear();
			temp.chain.push_back(0);
			break;
		}
		case 15: { //if语句
			getNextToken();
			int a = address;
			temp = bool_exp();
			backpatch(temp.True, address);
			temp.chain = temp.False;
			if (nowToken.getType() != 29) { // then
				cout << "Missing keyword 'then'!" << endl;
				system("pause");
				exit(0);
			}
			else {

				State temp2 = Sentence();
				Tacpushback(Token("j", 0), Token("-", 0), Token("-", 0), Token("-", 0));
				temp.chain = temp.merge(temp.chain, temp2.chain);
				if (nowToken.getType() == 11) { //else
					backpatch(temp.chain, address);
					temp2.chain.push_back(address-1);
					temp.chain = temp2.chain;
					temp2 = Sentence();
					temp.chain = temp.merge(temp.chain, temp2.chain);
					backpatch(temp.chain, address);
				}
				
			}		
			//backpatch(temp.True,address);
			break;
		}
		case 34: { //while 语句
			getNextToken();
			temp.codebegin = address;
			temp = bool_exp();
			
			
			if (nowToken.getType() == 10) { // do
				backpatch(temp.True, address);
				temp.chain = temp.False;
				State temp2 = Sentence();
				backpatch(temp2.chain, temp.codebegin);
			}
			Tacpushback(Token("j", 0), Token("-", 0), Token("-", 0), Token(to_string(temp.codebegin), 0));
			backpatch(temp.False, address);
			break;
		}
		case 26: {//repeat语句
			temp.codebegin = address;
			State t = Sentence();
			backpatch(t.chain, address);
			if (nowToken.getType() == 32) { // until
				getNextToken();
				State temp_state = bool_exp();
				backpatch(temp_state.False, temp.codebegin);
				temp.chain = temp_state.True;
			}
			else {
				cout << "error" << endl;
				system("pause");
				exit(0);
			}
			break;
		}
		default:
			break;
	}
	return temp;
}

void ProgramParser::VarDeclare() {
	if (nowToken.getType() == 33) {  //var
		getNextToken();
		VarDefine();
	}
}

Token ProgramParser::VarDefine() {
	Token temp;
	VarChart();
	if (nowToken.getType() != 50) { // ':'
		cout << "Missing keywords ':' !" << endl;
		system("pause");
		exit(0);
	}
	getNextToken();
	switch (nowToken.getType()) {
		case 17: { // integer
			nowToken.setVarType(INTEGER);
			for (int i = 0; i < VarList.size(); ++i) {
				if (VarList[i].getVarType() == UNDEFINE) {
					VarList[i].setVarType(INTEGER);
				}
			}
			break;
		}
		case 7: { // char
			nowToken.setVarType(CHAR);
			for (int i = 0; i < VarList.size(); ++i) {
				if (VarList[i].getVarType() == UNDEFINE) {
					VarList[i].setVarType(CHAR);
				}
			}
			break;
		}
		case 4: { // bool
			nowToken.setVarType(BOOL);
			for (int i = 0; i < VarList.size(); ++i) {
				if (VarList[i].getVarType() == UNDEFINE) {
					VarList[i].setVarType(BOOL);
				}
			}
			break;
		}
		default: {
			cout << "变量类型定义错误！" << endl;
			system("pause");
			exit(0);
		}
	}
	getNextToken();
	if (nowToken.getType() != keywords[";"]) {
		cout << "Missing keyword ';' ！" << endl;
		system("pause");
		exit(0);
	}
	getNextToken();
	if (nowToken.getType() == 36) {
		temp = VarDefine();
	}
	else {
		Token_index--;
	}
	return temp;
}
Token ProgramParser::VarChart() {
	Token temp;
	if (nowToken.getType() != 36) { //判断是否是标识符
		cout << "Missing identity!" << endl;
		system("pause");
		exit(0);
	}
	else {
		VarList.push_back(nowToken);
		getNextToken();
		if (nowToken.getType() == 44) {// ,
			getNextToken();
			temp = VarChart();
		}
	}
	return temp;
}

void ProgramParser::Statement() {
	Token temp = nowToken;
	if (nowToken.getType() != 36) {
		cout << "Missing assign!" << endl;
		system("pause");
		exit(0);
	}
	getNextToken();
	if (nowToken.getType() != 51) {
		cout << "Missing keyword := !" << endl;
		system("pause");
		exit(0);
	}
	else {
		Tacpushback(Token(":=", 0), Cal_exp(), Token("-",0), temp);
	}
}

Token ProgramParser::rop() {
	Token temp;
	int rop_j = nowToken.getType();
	if (rop_j >= 53 && rop_j <= 58) {
		temp = nowToken;
	}
	return temp;
}

Token ProgramParser::ch_exp() {
	Token temp;
	if (nowToken.getType() == 36 || nowToken.getType() == 38) { // 标识符或字符常量
		temp = nowToken;
	}
	return temp;
}

Token ProgramParser::Cal_exp() {
	Token temp;
	getNextToken();
	temp = term();
	if (nowToken.getType() == 43) { //加号
		Token temp_cal = Cal_exp();
		tempVar++;
		string str = "T" + to_string(tempVar);
		Token temp2 = Token(str,36);
		constantList.insert({ str, constantList.size() });
		Tacpushback(Token("+", 0), temp, temp_cal, temp2);
		temp = temp2;

	}
	else if (nowToken.getType() == 45) { // 减号
		Token temp_cal = Cal_exp();
		tempVar++;
		string str = "T" + to_string(tempVar);
		Token temp2 = Token(str, 36);
		constantList.insert({ str, constantList.size() });
		Tacpushback(Token("-", 0), temp, temp_cal, temp2);
		temp = temp2;
	}
	return temp;
}
Token ProgramParser::term() {
	Token temp;
	temp = factor();
	getNextToken();
	if (nowToken.getType() == 41) { // 乘
		tempVar++;
		string str = "T" + to_string(tempVar);
		Token temp2 = Token(str, 36);
		constantList.insert({ str, constantList.size() });
		getNextToken();
		Tacpushback(Token("*", 0), temp, term(), temp2);
		temp = temp2;
	}
	else if (nowToken.getType() == 48) { // 除
		tempVar++;
		string str = "T" + to_string(tempVar);
		Token temp2 = Token(str, 36);
		constantList.insert({ str, constantList.size() });
		getNextToken();
		Tacpushback(Token("/", 0), temp, term(), temp2);
		temp = temp2;
	}
	return temp;
}
Token ProgramParser::factor() {
	Token temp;
	if (nowToken.getType() == 45) { //负号
		getNextToken();
		tempVar++;
		string str = "T" + to_string(tempVar);
		Token temp2 = Token(str, 36);
		constantList.insert({ str, constantList.size() });
		Tacpushback(Token("-", 0), nowToken, Token("-", 0), temp2);
		temp = temp2;
	}
	else {
		temp = arithmetic();
	}
	return temp;
}
Token ProgramParser::arithmetic() {
	Token temp;
	if (nowToken.getType() == 37 ) { // 整数
		temp = nowToken;
	}
	else if (nowToken.getType() == 36) { // 标识符
		temp = nowToken;
	}
	else if (nowToken.getType() == 37) { //数字
		temp = nowToken;
	}
	else if (nowToken.getType() == 39) { //如果是括号，判断中间算术表达式
		getNextToken();
		temp = Cal_exp();
	}
	return temp;
}

State ProgramParser::bool_exp() {
	State temp;
	temp.codebegin = address;
	State temp1 = bool_term();
	getNextToken();
	if (nowToken.getType() == 20) { //or
		State temp2;
		temp2 = bool_exp();
		backpatch(temp1.False, temp2.codebegin);
		temp.codebegin = temp1.codebegin;
		temp.True = temp.merge(temp1.True, temp2.True);
		temp.False = temp2.False;
	}
	else {
		temp = temp1;
	}
	backpatch(temp.True, address);
	return temp;
}
State ProgramParser::bool_term() {
	State temp;
	State temp1 = bool_factor();
	if (nowToken.getType() == 1) {  //and
		State temp2;
		getNextToken();
		getNextToken();
		temp2 = bool_term();
		backpatch(temp1.True, temp2.codebegin);
		temp.codebegin = temp1.codebegin;
		temp.True = temp2.True;
		temp.False = temp.merge(temp1.False, temp2.False);
	}
	else {
		temp = temp1;
	}
	return temp;
}
State ProgramParser::bool_factor() {
	State temp;
	if (nowToken.getType() == 18) { //bool_factor -> not bool_factor
		State temp1 = bool_factor();
		temp.True = temp1.False;
		temp.False = temp1.True;
		temp.codebegin = temp1.codebegin;
	}
	else {
		temp = bool_quan();
	}
	return temp;

}
State ProgramParser::bool_quan() {
	State temp;
	if (nowToken.getVarType() == BOOL) {
		Tacpushback(Token("jnz", 0), Token("-", 0), Token("-", 0), Token("-", 0));
		Tacpushback(Token("j", 0), Token("-", 0), Token("-", 0), Token("-", 0));
		getNextToken();
		return temp;
	}
	else {
		switch (nowToken.getType()) {
			case 31: { //true
				Tacpushback(Token("j", 0), Token("-", 0), Token("-", 0), Token("-", 0));
				temp.True.clear();
				temp.True.push_back(address);
				temp.codebegin = address;
				break;
			}
			case 13: { //false
				Tacpushback(Token("j", 0), Token("-", 0), Token("-", 0), Token("-", 0));
				temp.True.clear();
				temp.False.push_back(address);
				temp.codebegin = address;
				break;
			}
			case 36: { //标识符
				Token_index--;
				Token t1;
				int a = address;
				t1 = Cal_exp();
				if (nowToken.getType() >= 53 && nowToken.getType() <= 58) {
					Token op = nowToken;
					Token t2;
					t2 = Cal_exp();
					Token_index--;
					Tacpushback(Token("j" + op.getContent(), 0), t1, t2, Token("-", 0));
					Tacpushback(Token("j", 0), Token("-", 0), Token("-", 0), Token("-", 0));
					temp.True.clear();
					temp.True.push_back(a);
					temp.codebegin = a;
					temp.False.clear();
					temp.False.push_back(a+1);
					return temp;
				}
				else {
					Token_index--;
				}
				
				break;
			}
			case 37: {  //数字
				Token_index--;
				Token t1,t2;
				int a = address;
				t1 = Cal_exp();
				getNextToken();
				if (nowToken.getType() >= 53 && nowToken.getType() <= 58) {
					Token op = nowToken;
					Token t2;
					t2 = Cal_exp();
					Token_index--;
					Tacpushback(Token("j" + op.getContent(), 0), t1, t2, Token("-", 0));
					Tacpushback(Token("j", 0), Token("-", 0), Token("-", 0), Token("-", 0));
					temp.True.clear();
					temp.True.push_back(a);
					temp.codebegin = a;
					temp.False.clear();
					temp.False.push_back(a + 1);
				}
				else {
					cout << "error" << endl;
				}
				break;
			}
			case 39: { //(  bool_quan->(bool_exp)
				temp = bool_exp();
				getNextToken();
				if (nowToken.getType() != 40) { //括号不匹配
					cout << "Mising keyword ')' !" << endl;
					cout << "in: L" << setw(5) << nowToken.rowNum + 1 << "C:" << nowToken.columnNum + 1 << endl;
					system("pause");
					exit(0);
				}
				break;
			}
		}
	}
	return temp;	
}


State ProgramParser::compose_sentence() {
	State temp;
	if (nowToken.getType() == 3) {// begin
		temp = sentenceList();
		if (nowToken.getType() != 12) { //end
			cout << "Mising keyword end!" << endl;
			system("pause");
			exit(0);
		}
		getNextToken();
	}
	return temp;
}

State ProgramParser::sentenceList() {
	State temp;
	temp = Sentence();
	if (nowToken.getType() == 52) {
		sentenceList();
	}
	return temp;
}

int ProgramParser::Tacpushback(Token t1, Token t2, Token t3, Token t4) {
	vector<Token> newTac= { t1,t2,t3,t4 };
	TacList.push_back(newTac);
	address++;
	return address - 1;
}

void ProgramParser::printTacList() {

	/*for (auto &element : TacList) {
		int idx;
		auto nextJump = element;
		while (nextJump[0].getContent() == "j") {
			stringstream ss;
			ss << nextJump[3].getContent();
			ss >> idx;
			nextJump = TacList[idx];
		}
		if (element[0].getContent() == "j") {
			element[3].setContent(to_string(idx));
		}
	}*/

	cout << TacList.size() << endl;
	for (int i = 0; i < TacList.size(); ++i) {
		cout << "(" << setw(2) << i << ") ";
		if (TacList[i][0].getType() == 23) { //program
			cout << "(" << TacList[i][0].getContent() << "," << TacList[i][1].getContent() << ",-,-)" << endl;
		}
		else {
			cout << "(" <<setw(4) << TacList[i][0].getContent() << "," << setw(4) << TacList[i][1].getContent() << "," 
				<< setw(4) << TacList[i][2].getContent() << "," << setw(4) << TacList[i][3].getContent() << ")" << endl;
		}
	}
}

void ProgramParser::backpatch(vector<int> num, int codebegin) {
	for (int i = 0; i < num.size(); ++i) {
		TacList[num[i]][3].setContent(to_string(codebegin));
	}
}