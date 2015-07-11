#ifndef _STATE_MODEL_H_
#define _STATE_MODEL_H_
class StateModel {
public:
	StateModel() {}
	virtual double getNodeCost(const double node[40]) = 0;
	virtual double getNodeProb(const double node[40]) = 0;
	virtual void setNodeInfo(double** data, int size, int nCep) = 0;
	virtual int getCount() = 0;
};

#endif