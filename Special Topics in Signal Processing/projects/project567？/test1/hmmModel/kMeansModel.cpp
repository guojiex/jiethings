#include "kMeansModel.h"
#include <assert.h>
KMeansModel::KMeansModel() {
	length = nCluster = 0;
}
KMeansModel::KMeansModel(int length, int nCluster) {
	this->length = length;
	this->nCluster = nCluster;
	srand((unsigned)time(0));
	for (int i = 0; i < nCluster; i++) {
		weight[i] = 1 / (double)nCluster;
		isValid[i] = true;
		for (int j = 0; j < length; j++) {
			mean[i][j] = rand() / (double)(RAND_MAX);
			sd[i][j] = 1;
		}
	}
}
KMeansModel::KMeansModel(double mean[5][40], double sd[5][40], double weight[5], int length, int nCluster) {
	this->length = length;
	this->nCluster = nCluster;

	for (int i = 0; i < 5; i++) {
		this->weight[i] = weight[i];
		if (abs(weight[i]) < 10e-10) {
			this->isValid[i] = false;
		}
		else {
			this->isValid[i] = true;
		}
		for (int j = 0; j < length; j++) {
			this->mean[i][j] = mean[i][j];
			this->sd[i][j] = sd[i][j];
		}
	}
}
void KMeansModel::getData(double mean[5][40], double sd[5][40], double weight[5]) {
	for (int i = 0; i < nCluster; i++) {
		weight[i] = this->weight[i];
		for (int j = 0; j < length; j++) {
			mean[i][j] = this->mean[i][j];
			sd[i][j] = this->sd[i][j];
			if (_isnan(mean[i][j])) {
				mean[i][j] = 0;
			}
			if (_isnan(sd[i][j])) {
				sd[i][j] = 1;
			}
		}
	}
}
double KMeansModel::getNodeProb(const double node[40]) {
	double totalProb = 0;
	for (int j = 0; j < nCluster; j++) {
		if (!isValid[j]) {
			continue;
		}
		double prob = 0;
		double determint = 1;
		double dist = 0;

		for (int i = 0; i < length; i++) {
			assert(sd[j][i] != 0.0);
			determint *= sd[j][i] * 2 * PI;
			//std::cout << node[i] << " ";
			dist += -0.5* (node[i] - mean[j][i]) * (node[i] - mean[j][i]) / sd[j][i];
		}
		//std::cout << "\n";
		double a = 1.0 / sqrt(determint);
		double b = pow(2.718281828, dist);
		prob = 1.0 / sqrt(determint) * pow(2.718281828, dist);

		totalProb += weight[j] * prob;
	}

	return totalProb;
	
}
double KMeansModel::getNodeCost(const double node[40]) {
	double totalProb = getNodeProb(node);
	return -log(totalProb);
}
void KMeansModel::setNodeInfo(double** data, int size, int templateCount) {
	double initMean[40] = { 0 };
	double squareMean[40] = { 0 };
	double initSd[40] = { 0 };
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < length; j++) {
			initMean[j] += data[i][j];
			squareMean[j] += data[i][j] * data[i][j];
		}
	}
	for (int i = 0; i < length; i++) {
		initMean[i] /= size;
		squareMean[i] /= size;
		initSd[i] = squareMean[i] - initMean[i] * initMean[i];
	}
	srand((unsigned)time(NULL));
	for (int i = 0; i < nCluster; i++) {
		weight[i] = 1 / (double)nCluster;
		int index = rand() % size;
		for (int j = 0; j < length; j++) {
			this->mean[i][j] = data[index][j];
			this->sd[i][j] = initSd[j];
		}
	}
	bool isConverge = true;
	do {
		double curMean[5][40];
		double curSd[5][40];
		double squareMean[5][40];
		int count[5];
		memset(curMean, 0, sizeof(curMean));
		memset(curSd, 0, sizeof(curSd));
		memset(squareMean, 0, sizeof(squareMean));
		memset(count, 0, sizeof(count));
		int index = 0;
		for (int i = 0; i < size; i++) {
			double minCost = 10e20;
			for (int j = 0; j < nCluster; j++) {
				double cost = getClusterCost(data[i], j);
				//std::cout << cost << " ";
				if (minCost > cost) {
					minCost = cost;
					index = j;
				}
			}
			//std::cout << index << " ";
			//std::cout << std::endl;
			count[index] += 1;
			for (int j = 0; j < length; j++) {
				curMean[index][j] += data[i][j];
				squareMean[index][j] += data[i][j] * data[i][j];
			}
		}

		for (int i = 0; i < nCluster; i++) {
			isValid[i] = count[i] == 0 ? false : true;
			for (int j = 0; j < length; j++) {
				curMean[i][j] /= count[i];
				squareMean[i][j] /= count[i];
				curSd[i][j] = squareMean[i][j] - curMean[i][j] * curMean[i][j];
				if (abs(curSd[i][j]) < 10e-20) {
					curSd[i][j] = 10e-10;
				}
			}
			weight[i] = count[i] / double(size);
		}

		for (int i = 0; i < nCluster; i++) {
			std::cout << count[i] << " ";
		}
		std::cout << std::endl;

		isConverge = true;
		for (int i = 0; i < nCluster; i++) {
			for (int j = 0; j < length; j++) {
				if (abs(curMean[i][j] - mean[i][j]) > 0.01 || abs(curSd[i][j] - sd[i][j]) > 0.01) {
					isConverge = false;
				}
				mean[i][j] = curMean[i][j];
				sd[i][j] = curSd[i][j];
			}
		}
	} while (!isConverge);
	std::cout << std::endl;
}

double KMeansModel::getClusterCost(const double node[40], int index) {
	double determint = 1;
	double dist = 0;
	double cost = 0;
	if (!isValid[index]) {
		return 10e20;
	}
	for (int i = 0; i < length; i++) {
		assert(sd[index][i] != 0.0);
		determint *= sd[index][i];
		dist += (node[i] - mean[index][i]) * (node[i] - mean[index][i]) / sd[index][i];
	}

	cost = 0.5 * log(pow(2 * PI, length)*determint) + 0.5 * dist;
	if (_isnan(cost)) {
		int a = 0;
	}

	return cost;
}
