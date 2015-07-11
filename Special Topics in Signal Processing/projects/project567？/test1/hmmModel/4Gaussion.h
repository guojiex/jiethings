#pragma once
#include "stateModel.h"
#include "singleGaussion.h"
#include <vector>
# define PI          3.141592653589793238
# define GaussionNumber 4
class fourGaussion:public StateModel
{
public:
	void initVectors(){
	}
	fourGaussion(int deltaDimension):deltaDimension(deltaDimension){
		this->initVectors();
	}
	~fourGaussion();
	virtual double getNodeCost( const double node[40] );
	virtual double getNodeProb( const double node[40] );
	virtual void setNodeInfo(double** data, int size, int templateCount);
	virtual int getCount();
	void selectALargeAndSpilt(int numberOfGaussion);
	void spiltGaussionToTwo(const int from,const int to);
	void resegment(const int numberOfGaussion);
	void renewWeight(const int guassionNumberNow);
private:
	int deltaDimension;
	singleGaussion sg[GaussionNumber];
	double weight[GaussionNumber];
	std::vector<std::vector<double>> dataForGausssion;
	std::vector<std::vector<std::vector<double>>> framesForPerGaussion;
};
void spiltAGaussionTOTwo(const int from,const int to);