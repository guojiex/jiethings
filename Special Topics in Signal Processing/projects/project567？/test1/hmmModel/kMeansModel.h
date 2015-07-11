#ifndef _K_MEANS_MODEL_H_
#define _K_MEANS_MODEL_H_
#include "stateModel.h"
#include <iostream>
#include <algorithm>
#include <ctime>
# define PI          3.141592653589793238
class KMeansModel : public StateModel {
private:
	double mean[5][40];
	double sd[5][40];
	double weight[5];
	bool isValid[5];
	int length;
	int nCluster;
	double getClusterCost(const double node[40], int index);
public:
	KMeansModel();
	KMeansModel(int length, int nCluster);
	KMeansModel(double mean[5][40], double sd[5][40], double weight[5], int length, int nCluster);
	void getData(double mean[5][40], double sd[5][40], double weight[5]);
	virtual double getNodeProb(const double node[40]);
	virtual double getNodeCost(const double node[40]);
	virtual void setNodeInfo(double** data, int size, int templateCount);
	virtual int getCount() { return length; }
};

#endif