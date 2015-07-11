#pragma once
#include <string>
class backPointer
{
private:
	int stateTime;
	int parentBackpointer;	
	std::string word;	
public:
	backPointer(const int& t,const int &parentState,const std::string &word):stateTime(t),parentBackpointer(parentState),word(word){}
	std::string Word() const { return word; }
	//void Word(std::string val) { word = val; }
	int StateTime() const { return stateTime; }
	//void StateTime(int val) { stateTime = val; }
	int ParentBackpointer() const { return parentBackpointer; }
	//void ParentBackpointer(int val) { parentBackpointer = val; }
	~backPointer(void)
	{
	}
};

