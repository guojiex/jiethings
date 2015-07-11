#pragma once
#include "statemodel.h"
#include <vector>
class singleGaussion :
	public stateModel
{
private:
	std::vector<double> means;//39
	std::vector<double>  covariances;//39
	int deltaDimension;
public:
	singleGaussion(void);
	singleGaussion(const int &deltaDimension):deltaDimension(deltaDimension){}
	~singleGaussion(void);
	virtual double getNodeCost(const std::vector<double> &node/*39维*/);
	virtual double getNodeProb(const std::vector<double> &node);
	virtual void setNodeInfo(const std::vector<std::vector<double>> &data, int size, int nCep);
	virtual int getCount(){return deltaDimension;}
};

