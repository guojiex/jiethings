// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "dictTree.h"
#include <string.h>
#include <vector>
#include "fileReader.h"
#include "lextreeDTW.h"
#include "Util.h"
#include <iostream>
_INITIALIZE_EASYLOGGINGPP
	/*
	char a[10]="abcd";
	strcat_s(a,10,"efgh");
	printf("%d\n",strlen(a));
	输出8，
	*/

void testAword(const std::string &fileName,const std::string &word,const bool &isPrint,const unsigned int &beam)
{
	LOG(INFO) << "test index and template Vector start";	
	std::vector<std::string> dict;
	dict=fileReader::readAdict(fileName,"\n",false);
	LOG(INFO) << "dict read, read size:"<<dict.size();
	dictTree<char> tree;	
	tree.readWordsFromStringVector(dict);
	LOG(INFO) << "dict tree build, tree size:"<<tree.size();
	std::vector<char> show;
	show=tree.getDictTemplateVector();
	LOG(INFO) << "dict template vector size:"<<show.size();	
	std::vector<unsigned int> show2;
	show2=tree.getIndexTemplateVector();
	LOG(INFO) << "dict template index vector size:"<<show2.size();
	std::vector<bool> show3;
	show3=tree.getEndTemplateVector();
	LOG(INFO) << "dict end template index vector size:"<<show3.size();
	if(isPrint)
	{
		util::showAVector(show);
		util::showAVector(show2);
		util::showAVector(show3);
	}	
	lextreeDTW dtwTree(show,show2,show3);
	dtwTree.IsPrint(isPrint);
	LOG(INFO) << "start dtw one word";
	printf("%s \n",dtwTree.getBestCloseForOne(word,beam).c_str());
	LOG(INFO) << "finish dtw one word";
}
void testAPassage(const std::string &fileName,const std::string &passageName,const bool &isPrint,const unsigned int &beam)
{
	LOG(INFO) << "test index and template Vector start";	
	std::vector<std::string> dict;
	dict=fileReader::readAdict(fileName,"\n",false);
	LOG(INFO) << "dict read, read size:"<<dict.size();
	dictTree<char> tree;	
	tree.readWordsFromStringVector(dict);
	LOG(INFO) << "dict tree build, tree size:"<<tree.size();
	std::vector<char> show;
	show=tree.getDictTemplateVector();
	LOG(INFO) << "dict template vector size:"<<show.size();	
	std::vector<unsigned int> show2;
	show2=tree.getIndexTemplateVector();
	LOG(INFO) << "dict template index vector size:"<<show2.size();
	std::vector<bool> show3;
	show3=tree.getEndTemplateVector();
	LOG(INFO) << "dict end template index vector size:"<<show3.size();
	if(isPrint)
	{
		util::showAVector(show);
		util::showAVector(show2);
		util::showAVector(show3);
	}	
	lextreeDTW dtwTree(show,show2,show3);
	dtwTree.IsPrint(isPrint);	
	//std::vector<std::string> words;
	//words=fileReader::readAdict(passageName," \t\n",false);
	std::vector<std::vector<std::string>> sentences;
	sentences=fileReader::readSentences(passageName," \t","\n",false);
	dtwTree.getBestCloseForSentencesOutputToFIle(sentences,beam,"../../checkTypo.txt",true);
	//dtwTree.getBestCloseForMulti(words,beam,"../../checkTypo.txt");
}
void testAPassageWithSegment(const std::string &fileName,const std::string &passageName,const std::string &output,const bool &isPrint,const unsigned int &beam)
{
	LOG(INFO) << "test segmentation start";	
	std::vector<std::string> dict;
	dict=fileReader::readAdict(fileName,"\n",false);
	LOG(INFO) << "dict read, read size:"<<dict.size();
	dictTree<char> tree;	
	tree.readWordsFromStringVector(dict);
	LOG(INFO) << "dict tree build, tree size:"<<tree.size();
	std::vector<char> show;
	show=tree.getDictTemplateVector();
	LOG(INFO) << "dict template vector size:"<<show.size();	
	std::vector<unsigned int> show2;
	show2=tree.getIndexTemplateVector();
	LOG(INFO) << "dict template index vector size:"<<show2.size();
	std::vector<bool> show3;
	show3=tree.getEndTemplateVector();
	LOG(INFO) << "dict end template index vector size:"<<show3.size();
	if(isPrint)
	{
		util::showAVector(show);
		util::showAVector(show2);
		util::showAVector(show3);
	}	
	lextreeDTW dtwTree(show,show2,show3);
	dtwTree.IsPrint(isPrint);	
	std::vector<std::string> sentences;
	sentences=fileReader::readAdict(passageName,"\n",false);
	sentences[0]="onceuponatimewhile";
	//dtwTree.outputAllSegmentToOutput(sentences,output,beam);
	dtwTree.getBestSegmentForOne(sentences[0],beam);
	/*for(int i=0;i<sentences.size();++i)
		printf("%s\n",sentences[i].c_str());*/
}
int wordCount(std::vector<std::vector<std::string>> &inputs)	
{
	int total=0;
	for(auto item:inputs)
		for(auto item2:item)
			total+=item2.size();
	return total;
}
void testPassageSimilarity(const std::string &left,const std::string &right)
{
	std::vector<std::vector<std::string>> sentences2=fileReader::readSentences(left," \t,.?!\"","\n",false);
	std::vector<std::vector<std::string>> storyCorrect=fileReader::readSentences(right," \t,.?!\"","\n",false);
	LOG(INFO)<<"word count:"<<wordCount(sentences2)<<" word count:"<<wordCount(storyCorrect);
	int diff=util::compareSentences(sentences2,storyCorrect);
	LOG(INFO)<<"different words of two passage:"<<diff;
}
void test()
{
	//testAword("../../dict_2.txt","dood",true,3);//true);	
	//testAPassage("../../dict_2.txt","../../typos.txt",true,3);
	testAPassageWithSegment("../../dict_2.txt","../../unsegmented0.txt","../../checkTypoSegmented.txt",true,15);
	//testPassageSimilarity("../../segmented.txt","../../checkTypoSegmented.txt");
	//testindexandtemplateVector("../../dict_1.txt",true);
	system("pause");
}
int _tmain(int argc, _TCHAR* argv[])
{
	el::Configurations conf("./defaultlog.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	LOG(INFO)<<" ";
	LOG(INFO)<<"console application start!";
	test();	
	return 0;
	/*dictTree<char> tree;	
	tree.addAWord("a",1);
	tree.addAWord("ac",2);
	tree.addAWord("ade",3);
	std::vector<char> show;
	show=tree.getDictTemplateVector();
	std::vector<int> show2;
	std::vector<bool> show3;
	show2=tree.getIndexTemplateVector();
	show3=tree.getEndTemplateVector();
	util::showAVector(show);
	util::showAVector(show2);
	util::showAVector(show3);
	system("pause");
	return 0;*/
}

