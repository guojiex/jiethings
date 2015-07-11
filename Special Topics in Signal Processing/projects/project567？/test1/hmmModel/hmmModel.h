#ifndef _HMM_MODEL_H_
#define _HMM_MODEL_H_
#include "stateModel.h"
#include <string>
using namespace std;

class HMMModel {
private:
	StateModel* stateModel[10];
	int stateCount;
	int stateType;
	double transitionCost[10][10];
	double initCost[10];
public:
	static const int nCluster = 4;
	HMMModel(int count, int type);
	HMMModel() {};
	HMMModel(HMMModel& preHMMModel);
	~HMMModel();
	double getEdgeCost(int prevModelNo, int curModelNo);
	double getNodeCost(int modelNo, double data[40], int length);
	double getInitCost(int modelNo);
	double getEdgeProb(int prevModelNo, int curModelNo);
	double getNodeProb(int modelNo, double data[40], int length);
	double getInitProb(int modelNo);
	void setTransitionCost(double cost[10][10]);
	void setInitCost(double cost[10]);
	void setStateModel(StateModel* stateModel, int modelNo);
	int getstateCount() { return stateCount; }
	int getStateType() { return stateType;  }
	int getnCep() { return stateModel[0]->getCount(); }
	void readHMMModel(string filename);
	void writeHMMModel(string filename);
	enum StateType{
		singleType, mutiType, kmeansType
	};
};

#endif