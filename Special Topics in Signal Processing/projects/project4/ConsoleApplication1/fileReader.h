#pragma once
#include <string>
#include <vector>
#include "lextreeDTW.h"
class fileReader
{
public:
	static std::vector<std::string> readAdict(const std::string &fileName,const  std::string &seperator,const bool &isPrint );
	static std::vector<std::vector<std::string>> readSentences(const std::string &fileName,const std::string &seperator,const std::string &SentenceEndSeperator,const bool &isPrint );
};

