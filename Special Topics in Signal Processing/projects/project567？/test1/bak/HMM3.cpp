#include "HMM3.h"
#include <assert.h>
HMM3::HMM3(void)
{
}

HMM3::~HMM3(void)
{
	for(int i=0;i<10;++i)
		if(this->stateModels[i]!=nullptr)
		{
			//delete stateModels[i];
			this->stateModels[i]=nullptr;
		}
}

double HMM3::getEdgeCost(int prevStageNo,int currStageNo)
{
	if(!_isnan(transtionCost[prevStageNo][currStageNo]))
		return transtionCost[prevStageNo][currStageNo];
	printf("transtion cost NAN error!\n");
	system("pause");
	return 0;
}

double HMM3::getInitCost(const int modelNo)
{
	assert(modelNo<this->segmentNumber);
	return initCost[modelNo];
}

void HMM3::setInitCost( const double initcost[10] )
{
	for(int i=0;i<10;++i)
		this->initCost[i]=initCost[i];
}

double HMM3::getNodeProb(const int stageNo,const std::vector<double> &aNodeDelta)
{
	return this->stateModels[stageNo]->getNodeProb(aNodeDelta)+1e-60;
}

double HMM3::getNodeCost(const int stageNo,const std::vector<double> &aNodeDelta)
{
	assert(stateModels[stageNo]!=nullptr);
	return stateModels[stageNo]->getNodeCost(aNodeDelta);
}

double HMM3::getInitProb(const int modelNo)
{	
	return exp(-initCost[modelNo]);
}

double HMM3::getEdgeProb(const int prevStageNo,const int CurrStageNo)
{
	return exp(-transtionCost[prevStageNo][CurrStageNo]);
}

void HMM3::setStateModel( stateModel* model,const int &modelIndex )
{
	/*if(this->stateModels[modelIndex]!=nullptr)
		delete stateModels[modelIndex];*/
	//assert(this->stateModels[modelIndex]==nullptr);
	this->stateModels[modelIndex]=model;
}

void HMM3::setTranstionCost( const double transCost[10][10] )
{
	for(int i=0;i<10;++i)
		for(int j=0;j<10;++j)
			this->transtionCost[i][j]=transCost[i][j];
}
