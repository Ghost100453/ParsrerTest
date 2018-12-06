#include "State.h"



State::State()
{
}


State::~State()
{
}

void State::addTrueChain(State &st) {
	True.insert(True.begin(), st.True.begin(), st.True.end());
}

void State::addFalseChain(State &st) {
	False.insert(False.begin(), st.False.begin(), st.False.end());
}


void State::swapChain() {
	std::vector<int> temp = this->True;
	this->True = this->False;
	this->False = temp;
}

std::vector<int> State::merge(std::vector<int> chainA, std::vector<int> chainB) {
	chainA.insert(chainA.end(), chainB.begin(), chainB.end());
	return chainA;
}