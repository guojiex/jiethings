#pragma once
#include "myLog.h"
#include <string>
#include "LevenshteinDistance.h"
class fileReader
{
public:
	fileReader(void);
	~fileReader(void);
	vector<string> readADict(myLog* logFileHandler,std::string fileName, std::string seperator,bool isPrint);	
	vector<vector<string>> readSentences(string fileName, string seperator,string SentenceEndSeperator,bool isPrint);
	void writeSpellCheckVersionToTxt( string fileName,vector<vector<string>>& sentences,LevenshteinDistance &test,vector<int> &correctWordInDict);
};

