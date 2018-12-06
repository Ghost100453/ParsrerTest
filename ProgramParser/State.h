#pragma once
#include <vector>
class State
{
public:
	State();
	~State();
	std::vector<int> True;
	std::vector<int> False;
	int codebegin;
	void addTrueChain(State &st);
	void addFalseChain(State &st);
	void swapChain();
	std::vector<int> merge(std::vector<int> chainA, std::vector<int> chainB);
};