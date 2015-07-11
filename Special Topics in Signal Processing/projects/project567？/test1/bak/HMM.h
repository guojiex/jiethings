#pragma once
#include <vector>
#include "DTW.h"
typedef std::vector<std::vector<double>> DTW_frame;
typedef std::vector<double> vectorOfAMeanOrCovariance;//39ά�������ڴ�Ŷ�Ӧά�ȵķֶε�ƽ��ֵ���߷���
typedef std::vector<int> startOfASegment;//�ֳ���Σ�������ÿ���ε���ʼ�±�
typedef vectorOfAMeanOrCovariance covarianceOfASegment;//�ֳ���Σ�������ÿ���ε�covariance���Э���
class HMM
{
public:
	struct HMM_result
	{
		int maxIndex;
		double maxRes;
	};
	int states;
	std::vector<std::vector<double>> scoreMatrix;
	std::vector<std::vector<DTW_frame>> templates;
	std::vector<int> templatesArray;
	std::vector<std::vector<startOfASegment>> segments;//��һά0-9�������ĸ��ʵ�ģ��ֶΣ��ڶ�ά�Ǹ���templateArrayȷ������Ҫʹ�õľ���ģ���instance������ά������һ��ģ������Ҫ�ֵ�states��ô���
	std::vector<std::vector<std::vector<double>>> modelVector;//modelVector,��Ӧÿ��״̬state����ֻ��һ֡
	std::vector<std::vector<covarianceOfASegment>> covariances;
	std::vector<std::vector<int>> framesIneachSegment; 
	std::vector<std::vector<std::vector<double>>> Tprobability;//�ӵ�i��״̬��j״̬�Ŀ�����
	std::vector<std::vector<double>> M;
public:
	HMM(DTW input,int st);
	~HMM(void);
	void initSegment();
	
	//************************************
	// Method:    ��һ��ģ����зֶγ�ʼ��
	// FullName:  HMM::initSegmentSingleTemplate
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: std::vector<std::vector<double>
	// Parameter: startOfASegment seg> & aTemplate
	// Parameter: int states
	//************************************
	void initSegmentSingleTemplate(const std::vector<std::vector<double>> &aTemplate,startOfASegment &seg);	

	//************************************
	// Method:    ���ݷֶεõ�model vector��������ͬʱ����frames of segment��ͳ�ƣ�����ÿ��״̬�ж���֡
	// FullName:  HMM::getModelVector
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void getModelVector();

	void getCovarianceFromModelVector();

	//************************************
	// Method:    use a negative log Gaussian instead, the modified score (with the
	//				diagonal covariance) 
	// FullName:  HMM::d
	// Access:    public 
	// Returns:   double
	// Qualifier:
	// Parameter: const std::vector<double> & x ��һ֡��delta��39ά
	// Parameter: const int jthState ���j��״̬��
	//************************************
	double d(const std::vector<double> &x,const int templateWord,const int jthState);

	double individualVarianceTermS2LeftPart(const int templateWord,const int jthState);

	inline void initFramesIneachSegment()
	{		
		framesIneachSegment.clear();
		framesIneachSegment.resize(templates.size());
		for(int j=0;j<templates.size();++j)
		{
			framesIneachSegment[j].resize(states);		
			for(int i=0;i<states;++i)
				framesIneachSegment[j][i]=0;
		}		
	}
	inline void initModelVector()
	{
		modelVector.clear();
		modelVector.resize(10);
		for(int j=0;j<10;++j)
		{
			modelVector[j].resize(states);
			for(int i=0;i<states;i++)
			{
				modelVector[j][i].resize(39);
				for(int dimension=0;dimension<modelVector[j][i].size();++dimension)
					modelVector[j][i][dimension]=0;
			}
		}		
	}		
	void SegmentalK_meansOne(const std::vector<std::vector<double>> &atemplate,const int templateWord,startOfASegment &newSeg);
	void initScoreMatrix(const int row,const int rol);
	void SegmentalK_means(int times);
	void initCovar();
	void initProbabilities();
	double DTWforScore(std::vector<std::vector<double>> &data,const int templateWord);
	HMM::HMM_result getBestScoreDTWForOne(std::vector<std::vector<double>> &data );
	HMM::HMM_result getBestCostDTWCorrectRateFromSingleTemplate( int dataIndexLeft, int dataIndexRight,int left,int right );
	void getNewSeg(const std::vector<std::vector<double>> &atemplate,const int templateWord,startOfASegment &newSeg);
};

