#pragma once
#include <vector>
#include <string>
class sentenceDistance
{	
public:
	typedef struct ErrorNum
	{
		int insertion;
		int deletion;
		int substitution;
	}ErrorNumType;
	sentenceDistance::ErrorNumType errors;
	int ** ptr;
	bool printMatrixOnScreen;
	sentenceDistance(void);
	ErrorNumType computeTheErrors(int **ptr,int max1,int max2);
	int computeSentenceDistanceFixed(std::vector<std::string> &left,std::vector<std::string> &top,int maxDistanceLimit );
	int computeSentenceDistanceBeamSearch(std::vector<std::string> &left,std::vector<std::string> &top,int maxDistanceLimit );
	ErrorNum getAllErrorsFromTwoPassages(std::vector<std::vector<std::string>> correctLeft,std::vector<std::vector<std::string>> comparedTop,int maxDistanceLimit,bool useFix);
	~sentenceDistance(void);
	void reset();
};

