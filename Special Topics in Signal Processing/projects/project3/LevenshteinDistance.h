#pragma once
#include "util.h"
#include "myMatrix.h"
#include "myLog.h"
#include <assert.h>
#include <algorithm>

class LevenshteinDistance
{
private:
	string templateString;
	string comparedString;
	myLog* logFileHandle;	
	int ** ptr;
public:
	typedef struct ErrorNum
	{
		int insertion;
		int deletion;
		int substitution;
	}ErrorNumType;
	vector<ErrorNum> errors;
	vector<string> dict;
	bool printMatrixOnScreen;
	LevenshteinDistance();
	void init(void);
	//ErrorNumType computeTheErrors(int **ptr,int max1,int max2);
	LevenshteinDistance(myLog* logFileHandler2);
	LevenshteinDistance(string templateString,string comparedString);
	LevenshteinDistance(string templateString,string comparedString,myLog* logFileHandler);
	~LevenshteinDistance(void);
	bool ** best_solutionLD(int **ptr,int max1,int max2);
	//************************************
	// Method:    ��logfile�м�¼������д���ļ�ȥ
	// FullName:  LevenshteinDistance::writeLogs
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: void
	//************************************
	void writeLogs(void);

	//void initFirstCol(int* leftCol,int* rightCol,string origin,string textGet,int maxDistanceLimit);
	//************************************
	// Method:    �������ĵ�һ��
	// FullName:  LevenshteinDistance::initFirstCol
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: MATRIX & mat
	// Parameter: string left
	// Parameter: string bottom
	// Parameter: int maxDistanceLimit
	//************************************
	vector<int> initFirstCol(MATRIX &mat,string left,string bottom,int maxDistanceLimit);

	//************************************
	// Method:    ������̱༭�������
	// FullName:  LevenshteinDistance::calcLeastEditMatrix
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: string origin
	// Parameter: string textGet
	// Parameter: int maxDistanceLimit
	//************************************
	int calcLeastEditMatrix(string origin, string textGet,int maxDistanceLimit);

	MATRIX calcLeastEditMatrixUsingBeamSearch(string origin, string textGet,int maxDistanceLimit);

	int getMostRelatedStringUsingFixed(string origin,int maxDistance);
	int getMostRelatedStringUsingFixed( string origin,vector<string> dict2,int maxDistance );
	//************************************
	// Method:    �õ����·�������ж�������·����ֵ������С�༭����
	// FullName:  LevenshteinDistance::getShortestPathLength
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: MATRIX mat
	//************************************
	int getShortestPathLength(MATRIX mat);

	//************************************
	// Method:    �����ֵ�͸�������Ҫƥ����ַ������飬�õ��ֵ���������Ĵ�����
	// FullName:  LevenshteinDistance::getStringsFromTextAndDict
	// Access:    public 
	// Returns:   vector<int> ���ض�Ӧ�ַ������ֵ�dict�е�λ��
	// Qualifier:
	// Parameter: vector<string> & textGet ��Ҫƥ����ַ�������
	//************************************
	vector<int> getStringsFromTextAndDict(vector<string> &textGet);

	int getMostRelatedStringUsingBeamSearch( string origin,int maxDistance );
	int getMostRelatedStringUsingBeamSearch( string origin,vector<string> dict2,int maxDistance );
	void makeUnvalidToLarge(MATRIX &mat,int col,vector<int> validVect,int maxValue);
	vector<int> getStringsFromTextAndDictBeamSearch(vector<string> &textGet,int m);

	//************************************
	// Method:    ȫ������򵥵Ķ�ά�����ܣ�ʹ�ü�֦���ǵ�һ���ٶȵ�10��
	// FullName:  LevenshteinDistance::calcLeastEditMatrix2
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: string origin
	// Parameter: string textGet
	// Parameter: int maxDistanceLimit
	//************************************
	int calcLeastEditMatrix2( string origin, int textGetInDict,int maxDistanceLimit );
	int calcLeastEditMatrix2( string origin, string textGet,int maxDistanceLimit );
	int calcLeastEditMatrixUsingBeamSearch2( string origin, string textGet,int maxDistanceLimit );
	int calcLeastEditMatrixUsingBeamSearchNewest(string s1,string s2,int beam);
	string getTheMostRelatedStringUsingFixed( string origin,int maxDistance );
	LevenshteinDistance::ErrorNum computeErrors(string origin,string textGet,int maxDistanceLimit,bool usingFixed);
	void best_solutionLD( int **ptr,int max1,int max2 ,LevenshteinDistance::ErrorNum &result);
};

