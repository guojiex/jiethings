#pragma once
#include <vector>
#include "DTW.h"
typedef std::vector<std::vector<double>> DTW_frame;
typedef std::vector<double> vectorOfAMeanOrCovariance;//39维变量用于存放对应维度的分段的平均值或者方差
typedef std::vector<int> startOfASegment;//分成五段，里面存放每个段的起始下标
typedef vectorOfAMeanOrCovariance covarianceOfASegment;//分成五段，里面存放每个段的covariance方差（协方差）
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
	std::vector<std::vector<startOfASegment>> segments;//第一维0-9代表是哪个词的模板分段，第二维是根据templateArray确定的需要使用的具体模板的instance，第三维就是在一个模板里面要分的states那么多段
	std::vector<std::vector<std::vector<double>>> modelVector;//modelVector,对应每个状态state里面只有一帧
	std::vector<std::vector<covarianceOfASegment>> covariances;
	std::vector<std::vector<int>> framesIneachSegment; 
	std::vector<std::vector<std::vector<double>>> Tprobability;//从第i个状态到j状态的可能性
	std::vector<std::vector<double>> M;
public:
	HMM(DTW input,int st);
	~HMM(void);
	void initSegment();
	
	//************************************
	// Method:    对一个模板进行分段初始化
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
	// Method:    根据分段得到model vector，在里面同时进行frames of segment的统计，计算每个状态有多少帧
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
	// Parameter: const std::vector<double> & x 是一帧的delta，39维
	// Parameter: const int jthState 与第j个状态比
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

