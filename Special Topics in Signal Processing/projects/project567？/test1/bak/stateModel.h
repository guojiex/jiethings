#pragma once
#include <vector>
class stateModel
{
public:

	stateModel(void)
	{
	}

	~stateModel(void)
	{
	}
	virtual double getNodeCost(const std::vector<double> &node/*39ά*/) = 0;
	virtual double getNodeProb(const std::vector<double> &node) = 0;
	virtual void setNodeInfo(const std::vector<std::vector<double>> &data, int size, int nCep) = 0;
	virtual int getCount() = 0;
};

