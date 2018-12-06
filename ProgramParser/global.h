#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef enum {
	HELLO, HELLOAGAIN
}TokenType;

typedef enum {
	INTEGER, BOOL, CHAR, UNDEFINE
}VarType;

typedef enum {
	START, DONE, ISNUM, ISID, ISASSIGN, ISQUOTE,
	ISSLASH, ISLEFTCOMM, ISRIGHTCOMM, ISGT, ISLT, ISDOT
}stateType;
