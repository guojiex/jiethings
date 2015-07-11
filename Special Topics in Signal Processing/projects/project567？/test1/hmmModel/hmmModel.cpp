#include "hmmModel.h";
#include "singleGaussion.h"
#include "4Gaussion.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>
HMMModel::HMMModel(int count, int type) {
	this->stateCount = count;
	this->stateType = type;
}

HMMModel::HMMModel(HMMModel& preHMMModel) {
	this->stateCount = preHMMModel.stateCount;
	this->stateType = preHMMModel.stateType;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			this->transitionCost[i][j] = preHMMModel.transitionCost[i][j];
		}
	}

	for (int i = 0; i < 10; i++) {
		this->initCost[i] = preHMMModel.initCost[i];
	}
	
	for (int i = 0; i < stateCount; i++) {
		stateModel[i] = preHMMModel.stateModel[i];
	}
}
HMMModel::~HMMModel() {
	/*for(int i=0;i<10;++i)
		if(this->stateModel[i]!=nullptr)
		{
			this->stateModel[i]=nullptr;
		}*/
}

double HMMModel::getEdgeCost(int prevModelNo, int curModelNo) {
	if (_isnan(transitionCost[prevModelNo][curModelNo])) {
		int a = 0;
	}
	return transitionCost[prevModelNo][curModelNo];
}
double HMMModel::getNodeCost(int modelNo, double data[40], int length) {
	return stateModel[modelNo]->getNodeCost(data);
}
double HMMModel::getInitCost(int modelNo) {
	return initCost[modelNo];
}

double HMMModel::getEdgeProb(int prevModelNo, int curModelNo) {
	//return pow(2.718281828,-transitionCost[prevModelNo][curModelNo]);
	return -log(transitionCost[prevModelNo][curModelNo]);
}
double HMMModel::getNodeProb(int modelNo, double data[40], int length) {
	return stateModel[modelNo]->getNodeProb(data)+1e-60;
}
double HMMModel::getInitProb(int modelNo) {
	return pow(2.718281828, -initCost[modelNo]);
}

void HMMModel::setTransitionCost(double cost[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			this->transitionCost[i][j] = cost[i][j];
		}
	}
}

void HMMModel::setInitCost(double cost[10]) {
	for (int i = 0; i < 10; i++) {
		this->initCost[i] = cost[i];
	}
}

void HMMModel::setStateModel(StateModel* stateModel, int modelNo) {
	this->stateModel[modelNo] = stateModel;
}

void HMMModel::readHMMModel(string filename) {
	ifstream in(filename);
	//cout << "File " << filename << endl;
	if (!in.is_open()) {
		exit(1);
	}
	int nCep;
	in >> this->stateCount >> this->stateType >> nCep;
	HMMModel hmmModel(stateCount, stateType);

	for (int i = 0; i < stateCount; i++) {
		in >> initCost[i];
	}

	for (int i = 0; i < stateCount; i++) {
		for (int j = 0; j < stateCount; j++) {
			in >> transitionCost[i][j];
		}
	}

	//delete[] stateModel;
	if (stateType == HMMModel::singleType) {
		double mean[40], sd[40];
		for (int i = 0; i < stateCount; i++) {
			for (int j = 0; j < nCep; j++) {
				in >> mean[j] >> sd[j];
			}

			stateModel[i] = new singleGaussion(mean, sd, nCep);
		}
	}
	
	/*else if (stateType == HMMModel::kmeansType) {
	for (int i = 0; i < stateCount; i++) {
	double mean[5][40], sd[5][40], weight[5];
	for (int j = 0; j < nCluster; j++) {
	for (int k = 0; k < nCep; k++) {
	in >> mean[j][k] >> sd[j][k];
	}

	}
	for (int j = 0; j < nCluster; j++) {
	in >> weight[j];
	cout << weight[j] << " ";
	assert(weight[j] <= 1 && weight[j] >= 0);
	}
	stateModel[i] = new KMeansModel(mean, sd, weight, nCep, nCluster);
	}
	}*/

	int a = 0;
}
void HMMModel::writeHMMModel(string filename) {
	ofstream out(filename);
	int nCep = getnCep();
	out << stateCount << " " << stateType << " " << nCep << "\n";

	for (int i = 0; i < stateCount; i++) {
		out << initCost[i] << " ";
	}
	out << "\n";

	for (int i = 0; i < stateCount; i++) {
		for (int j = 0; j < stateCount; j++) {
			out << transitionCost[i][j] << " ";
		}
	}
	out << "\n";

	if (stateType == HMMModel::singleType||stateType == HMMModel::mutiType) {
		for (int i = 0; i < stateCount; i++) {
			singleGaussion* sgModel = dynamic_cast<singleGaussion*>(stateModel[i]);
			double mean[40], sd[40];
			sgModel->getData(mean, sd);
			for (int j = 0; j < nCep; j++) {
				out << mean[j] << " " << sd[j] << " ";
			}
			out << "\n";
		}
	}
	/*else if (stateType == HMMModel::kmeansType) {
		for (int i = 0; i < stateCount; i++) {
			KMeansModel* kMeansModel = dynamic_cast<KMeansModel*>(stateModel[i]);
			double mean[5][40], sd[5][40], weight[5];
			kMeansModel->getData(mean, sd, weight);
			for (int j = 0; j < nCluster; j++) {
				for (int k = 0; k < nCep; k++) {
					out << mean[j][k] << " " << sd[j][k] << " ";
				}
			}
			out << "\n";
			for (int j = 0; j < nCluster; j++) {
				out << weight[j] << " ";
			}
			out << "\n";
		}
	}*/

	out.flush();
	out.close();
}