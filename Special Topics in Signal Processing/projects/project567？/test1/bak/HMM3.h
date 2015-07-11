#pragma once
#include "stateModel.h"
class HMM3
{
private:
	stateModel* stateModels[10];
	int segmentNumber;
	int stateType;
	double transtionCost[10][10];
	double initCost[10];
public:
	static const int nCluster = 4;
	enum StateType{
		singleType, mutiType
	};
	HMM3(void);
	HMM3(const int &segmentNumber,const int &stateType)
		:segmentNumber(segmentNumber),stateType(stateType){}
	void setStateModel(stateModel* model,const int &modelIndex);
	double getEdgeCost(const int prevStageNo,const int currStageNo);
	double getEdgeProb(const int prevStageNo,const int CurrStageNo);
	double getInitCost(const int modelNo);
	double getInitProb(const int modelNo);
	double getNodeCost(const int stageNo,const std::vector<double> &aNodeDelta);
	double getNodeProb(const int stageNo,const std::vector<double> &aNodeDelta);
	void setInitCost(const double initcost[10]);
	void setTranstionCost(const double transCost[10][10]);
	int getSegmentNumber(){return segmentNumber;}//得到分多少段的数字
	~HMM3(void);
};

