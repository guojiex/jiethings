#pragma once
#include <vector>
#include <string>
#include <utility>
#include "backPointer.h"
#define MAXINPUTDATALENGTH 30
#define INFINITEVALUE 10000
#define MAXSEGMENTLENGTH 500
typedef unsigned int DTW_valid_index_vector_type;
typedef unsigned int template_size_type;
typedef unsigned int parent_index_type;
typedef std::pair<unsigned int,unsigned int> backPointerType;
///the template length must less than 65535,or change the template_size_type to larger
class lextreeDTW
{
private:
	std::vector<std::vector<unsigned short>> costMatrix;
	std::vector<std::vector<int>> backPointerMatrix;
	std::vector<std::vector<unsigned short>> costMatrix2;
	std::vector<backPointer> backPointerVector;
	///dict tree words
	std::vector<char> templateLeft;
	///current index's parent index
	std::vector<parent_index_type> parentIndexOfCurrent;
	///use to identify whether this index of the template is an end of a word
	std::vector<bool> endTemplateLeft;
	///use this var for not accessing vector.size() too often
	template_size_type templateLength;
	bool isPrint;	
	unsigned int inputSize;
	void resetCostMatrix();
	void resetMatrixForSegment();
public:
	bool IsPrint() const { return isPrint; }
	void IsPrint(bool val) { isPrint = val; }
	lextreeDTW(const std::vector<char> templateLeft,
		const std::vector<template_size_type> parentIndexOfCurrent,
		const std::vector<bool> endTemplateLeft):templateLeft(templateLeft),parentIndexOfCurrent(parentIndexOfCurrent),endTemplateLeft(endTemplateLeft),templateLength(templateLeft.size())
	{
		assert(!templateLeft.empty());
		this->isPrint=false;
		this->resetCostMatrix();
		this->resetMatrixForSegment();
	}	
	std::string getBestCloseForOne(const std::string &input,const unsigned int &beamwidth);
	std::string findBestStringAfterDTW();
	bool DTW(const std::string &input,const int &beamwidth);
	~lextreeDTW(void);
	void drawCostMatrix(const std::string input);	
	std::string getBestSegmentForOne(const std::string &input,const unsigned int &beamwidth);
	bool segmentDTW(const std::string &input,const int &beamwidth);
	void getBestCloseForSentencesOutputToFIle(const std::vector<std::vector<std::string>> &inputs,const unsigned int &beamwidth,const std::string &outputFile,const bool &needSpace);
	//************************************
	// Method:    得到第j列，单词结束的时候最小的花费的那个词结尾的index
	// FullName:  lextreeDTW::lastColumnminEndOfWord
	// Access:    public 
	// Returns:   int
	// Parameter: const template_size_type & j
	//************************************
	int lastColumnminEndOfWord(const template_size_type &j);
	std::string findBestStringAfterSegmentDTW();
	std::string getAWordFromEnd(const int &end);
	void drawCostMatrix2(const std::string &input);
	std::string getAWordFromSegmentDTW( const std::string &input,int &startOfAWord,const int&beamwidth);
	void outputAllSegmentToOutput(std::vector<std::string> &inputs,const std::string &output,const int &beam);
};

