// project3.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "LevenshteinDistance.h"
#include "fileReader.h"
#include "sentenceDistance.h"
vector<string> createStrings()
{
	vector<string> result;
	result.push_back("Elephant");
	result.push_back("Elegant");
	result.push_back("Sycophant");
	return result;
}
void testFile(LevenshteinDistance &test,myLog* logFileHandle,bool isFixed)
{
	fileReader fi;
	test.dict=fi.readADict(logFileHandle,"G:/中大/研一上学期/语音识别/project3/project3/dict.txt","\t\n\r .,\":?!",false);
	cout<<test.dict.size()<<endl;
	vector<string> story=fi.readADict(logFileHandle,"G:/中大/研一上学期/语音识别/project3/project3/story.txt","\t\n\r .,\":?!",false);
	vector<int> correctWordInDict;
	//vector<vector<string>> sentences=fi.readSentences("G:/中大/研一上学期/语音识别/project3/project3/story.txt","\t\n\r ,\":",".?!",true);
	if(isFixed)
		correctWordInDict=test.getStringsFromTextAndDict(story);
	else
		correctWordInDict=test.getStringsFromTextAndDictBeamSearch(story,3);
}
void createSpellCheckTxt(LevenshteinDistance &test,myLog* logFileHandle,bool isFixed)
{
	fileReader fi;
	test.printMatrixOnScreen=false;
	vector<vector<string>> sentences=fi.readSentences("G:/中大/研一上学期/语音识别/project3/project3/story.txt","\t\n\r ,\":",".?!",true);
	test.dict=fi.readADict(logFileHandle,"G:/中大/研一上学期/语音识别/project3/project3/dict.txt","\t\n\r .,\":?!",false);
	vector<string> story=fi.readADict(logFileHandle,"G:/中大/研一上学期/语音识别/project3/project3/story.txt","\t\n\r .,\":?!",false);
	vector<int> correctWordInDict=test.getStringsFromTextAndDictBeamSearch(story,10);//test.getStringsFromTextAndDict(story);
	fi.writeSpellCheckVersionToTxt("G:/中大/研一上学期/语音识别/project3/project3/spellcheckBeam.txt",sentences,test,correctWordInDict);	
}
void testPassage(LevenshteinDistance &test,myLog* logFileHandle,bool isFixed)
{
	createSpellCheckTxt(test,logFileHandle,isFixed);
	return;
	fileReader fi;
	test.printMatrixOnScreen=false;
	test.dict=fi.readADict(logFileHandle,"G:/中大/研一上学期/语音识别/project3/project3/dict.txt","\t\n\r .,\":?!",false);	
	vector<vector<string>> sentences=fi.readSentences("G:/中大/研一上学期/语音识别/project3/project3/spellcheck.txt","\t\n\r ,\":",".?!",true);	
	vector<vector<string>> sentences2=fi.readSentences("G:/中大/研一上学期/语音识别/project3/project3/spellcheckBeam.txt","\t\n\r ,\":",".?!",true);
	vector<vector<string>> storyCorrect=fi.readSentences("G:/中大/研一上学期/语音识别/project3/project3/storycorrect.txt","\t\n\r ,\":",".?!",true);	
	sentenceDistance sentenceTest;
	sentenceTest.printMatrixOnScreen=false;
	sentenceDistance::ErrorNum res1= sentenceTest.getAllErrorsFromTwoPassages(storyCorrect,sentences2,3,false);
	printf("ins:%d del:%d sub:%d \n",res1.insertion,res1.deletion,res1.substitution);	
}
void testWordOneOnOne(LevenshteinDistance &test,myLog* logFileHandle)
{
	//cout<<test.calcLeastEditMatrix2("ante","onse",3)<<endl;//evaryt evx，正好同样阈值但是结果不同
	//cout<<test.calcLeastEditMatrix2("once","onse",3)<<endl;
	test.printMatrixOnScreen=true;
	int dis=test.calcLeastEditMatrix2("evaryt","evx",3);
	LevenshteinDistance::ErrorNum errors=test.computeErrors("evaryt","evx",3,true);
	printf("ins:%d del:%d sub:%d \n",errors.insertion,errors.deletion,errors.substitution);	
//	int dis=test.calcLeastEditMatrixUsingBeamSearchNewest("evaryt","evx",3);
	dis=test.calcLeastEditMatrixUsingBeamSearchNewest("elephant","eleaphant",3);
	//errors=test.computeErrors("evaryt","evx",3,false);
	//printf("ins:%d del:%d sub:%d \n",errors.insertion,errors.deletion,errors.substitution);	
}
void testWords(LevenshteinDistance &test,myLog* logFileHandle)
{
	cout<<"result of fix search of strings: "<<test.getMostRelatedStringUsingFixed("Eleaphent",createStrings(),3)<<endl;
	cout<<"result of beamsearch of strings: "<<test.getMostRelatedStringUsingBeamSearch("Eleaphent",createStrings(),3)<<endl;
}
int main()
{	
	myLog logfile;
	myLog* logFileHandle=&logfile;
	//logfile.PrintOnScreen(true);
	LevenshteinDistance test(logFileHandle);	
	test.printMatrixOnScreen=true;//true;

	//*****测试从dict 读取词典
	//testFile(test,logFileHandle,true);//true means using fixed caculation
	//****
	//testPassage(test,logFileHandle,true);
	testWordOneOnOne(test,logFileHandle);

	//*********测试一对多最短距离
	//testWords(test,logFileHandle);		
	//**********

	/*logfile.debug(isCharInString('y',"acd")?"true":"false");
	logfile.debug("hello world.");
	logfile.error("error");
	logfile.warn("warning");
	logfile.finish();*/
	//logfile.finish();
	//logfile.finishNoDebug();
	test.writeLogs();
	system("pause");
	return 0;
}

