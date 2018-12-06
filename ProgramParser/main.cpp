#include <iostream>
#include <string>
#include "ProgramParser.h"
using namespace std;

int main() {
	ProgramParser parser = ProgramParser();
	string filename;
	cout << "Please input file name:";
	cin >> filename;
	parser.inputFile(filename);
	parser.createTokenList();
	parser.printTokenList();
	parser.parse();
	parser.printTacList();
	//cout << "helloworld" << endl;
	system("pause");
}