#include "fileReader.h"
#include "LevenshteinDistance.h"
#include <ctype.h>

fileReader::fileReader(void)
{
}


fileReader::~fileReader(void)
{
}

vector<vector<string>> fileReader::readSentences( string fileName, string seperator,string SentenceEndSeperator,bool isPrint )
{
	vector<vector<string>> sentences;
	vector<string> words;
	fstream f(fileName,ios::in);
	string temp="";
	char inputChar;
	while((inputChar=f.get())!=EOF)
	{
		if(isCharInString(inputChar,seperator))
		{
			if(temp.length()!=0)
			{
				words.push_back(temp);				
				temp=string("");
			}
			continue;
		}
		else if(isCharInString(inputChar, SentenceEndSeperator))
		{
			if(temp.length()!=0)
			{
				words.push_back(temp);
				temp=string("");
			}
			if(words.size()==0)
				continue;
			sentences.push_back(words);
			if(isPrint)
			{
				for(int i=0;i<sentences[sentences.size()-1].size();i++)				
					cout<<sentences[sentences.size()-1][i]<<" ";
				cout<<"."<<endl;
			}
			words.clear();
		}else
		{
			temp+=inputChar;
		}
	}	
	if(temp.length()!=0)
		sentences[sentences.size()-1].push_back(temp);
	cout<<sentences.size()<<endl;
	return sentences;
}

vector<string> fileReader::readADict( myLog* logFileHandler,string fileName, string seperator,bool isPrint )
{
	vector<string> dict;
	fstream f(fileName,ios::in);
	string temp="";
	char inputChar;
	while((inputChar=f.get())!=EOF)
	{
		if(isCharInString(inputChar,seperator))
		{
			if(temp.length()!=0)
			{
				dict.push_back(temp);
				if(isPrint)
					cout<<temp<<endl;
				temp=string("");
			}
		}else{	
			temp+= tolower(inputChar);
		}		
	}
	if(temp.length()!=0)
		dict.push_back(temp);
	cout<<"read dict size:"<<dict.size()<<endl;
	f.close();
	return dict;
}

//************************************
// Method:    把spell checked版本写到文本文件
// FullName:  fileReader::writeSpellCheckVersionToTxt
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: string fileName
// Parameter: vector<vector<string>> & sentences
// Parameter: LevenshteinDistance & test
// Parameter: vector<int> & correctWordInDict
//************************************
void fileReader::writeSpellCheckVersionToTxt( string fileName,vector<vector<string>>& sentences,LevenshteinDistance &test,vector<int> &correctWordInDict )
{
	FILE *pfOutput;
	fopen_s(&pfOutput,fileName.c_str(), "wt");
	if (pfOutput == NULL)
		cout << "Error!" << endl;
	else
		printf("open file %s success!\n",fileName.c_str());
	printf("Computing and writing file!\n");
	int index=0;
	for(int i=0;i<sentences.size();i++)
	{
		for(int j=0;j<sentences[i].size();j++)
		{
			if(j!=sentences[i].size()-1)
				fprintf(pfOutput, "%s ",test.dict[correctWordInDict[index]].c_str());
			else
			{
				fprintf(pfOutput, "%s.\n",test.dict[correctWordInDict[index]].c_str());
			}
			index++;
		}
	}
	printf("Write file finished!\n");
	fclose(pfOutput);
}
