#include "stdafx.h"
#include "fileReader.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Util.h"
#include "easylogging++.h"
#include <algorithm>
std::vector<std::string> fileReader::readAdict( const std::string &fileName,const std::string &seperator,const bool &isPrint )
{
	LOG(INFO)<<"start read dict"<<fileName;
	std::vector<std::string> dict;
	std::fstream f(fileName,std::ios::in);
	std::string temp="";
	char inputChar;
	while((inputChar=f.get())!=EOF)
	{
		if(Util::isCharInString(inputChar,seperator))
		{
			if(temp.length()!=0)
			{
				dict.push_back(temp);
				if(isPrint)
					printf("%s\n",temp.c_str());
				temp=std::string("");
			}
		}else{	
			temp+= tolower(inputChar);
		}		
	}
	if(temp.length()!=0)
		dict.push_back(temp);
	LOG(INFO)<<"finish reading dict"<<fileName<<" size:"<<dict.size();
	f.close();
	return dict;
}

std::vector<std::vector<std::string>> fileReader::readSentences( const std::string &fileName,const std::string &seperator,const std::string &SentenceEndSeperator,const bool &isPrint )
{
	std::vector<std::vector<std::string>> sentences;
	std::vector<std::string> words;
	std::fstream f(fileName,std::ios::in);
	std::string temp="";
	char inputChar;
	while((inputChar=f.get())!=EOF)
	{
		if(Util::isCharInString(inputChar,seperator))
		{
			if(temp.length()!=0)
			{
				transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				words.push_back(temp);				
				temp=std::string("");
			}
			continue;
		}
		else if(Util::isCharInString(inputChar, SentenceEndSeperator))
		{
			if(temp.length()!=0)
			{
				transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				words.push_back(temp);
				temp=std::string("");
			}
			if(words.size()==0)
				continue;
			sentences.push_back(words);
			if(isPrint)
			{
				for(int i=0;i<sentences[sentences.size()-1].size();i++)				
					std::cout<<sentences[sentences.size()-1][i]<<" ";
				std::cout<<"."<<std::endl;
			}
			words.clear();
		}else
		{
			temp+=inputChar;
		}
	}	
	if(temp.length()!=0)
		sentences[sentences.size()-1].push_back(temp);
	std::cout<<sentences.size()<<std::endl;
	return sentences;
}
