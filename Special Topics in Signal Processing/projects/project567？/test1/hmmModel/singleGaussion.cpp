#include "singleGaussion.h"
#include <algorithm>
#include <cmath>
#include <cassert>
#include <vector>
singleGaussion::singleGaussion(double mean[40], double sd[40], int length) {
	this->deltaDimension = length;
	for (int i = 0; i < length; i++) {
		this->mean[i] = mean[i];
		this->covariance [i] = sd[i];
	}
}

double singleGaussion::getNodeCost(const double node[40]) {
	double determint = 1;
	double dist = 0;
	double cost = 0;

	for (int i = 0; i < deltaDimension; i++) {
		assert(covariance [i] != 0.0);
		determint *= covariance [i];
		//printf("%d %d %lf %lf %lf\n",i,length,node[i],mean[i],sd[i]);
		//printf("det%lf mean%lf cov%lf\n",determint,mean[i],covariance[i]);
		dist += (node[i] - mean[i]) * (node[i] - mean[i]) / covariance [i];
	}
	//printf("node cost sg:%lf %lf\n",determint,dist);
	cost = 0.5 * log(pow(2 * PI, deltaDimension)*determint) + 0.5 * dist;
	if (_isnan(cost)) {
		int a = 0;
		printf("cost #inf!");
		system("pause");
	}
	return cost;
}

double singleGaussion::getNodeProb(const double node[40]) {
	double prob = 0;
	double determint = 1;
	double dist = 0;

	for (int i = 0; i < deltaDimension; i++) {
		assert(covariance [i] != 0.0);
		determint *= covariance [i] * 2 * PI;
		dist += -0.5* (node[i] - mean[i]) * (node[i] - mean[i]) / covariance [i];
	}
	double a = 1.0 / sqrt(determint);
	double b = pow(2.718281828, dist);
	prob = 1.0 / sqrt(determint) * pow(2.718281828, dist);

	if (prob == 0) {
		int a = 0;
	}

	return prob;
}
void singleGaussion::setNodeInfo(std::vector<std::vector<double>> &data, int size, int templateCount) {
	assert(data.size()==size);
	for (int i = 0; i < deltaDimension; i++) {
		mean[i] = 0;
		covariance [i] = 0;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < deltaDimension; j++) {
			mean[j] += data[i][j];
		}
	}

	for (int i = 0; i < deltaDimension; i++) {
		mean[i] /= size;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < deltaDimension; j++) {
			covariance [j] += (data[i][j] - mean[j])*(data[i][j] - mean[j]);
		}
	}

	for (int i = 0; i < deltaDimension; i++) {
		covariance [i] /= size;
	}

}
void singleGaussion::setNodeInfo(double** data, int size, int templateCount) {
	for (int i = 0; i < deltaDimension; i++) {
		mean[i] = 0;
		covariance [i] = 0;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < deltaDimension; j++) {
			mean[j] += data[i][j];
		}
	}

	for (int i = 0; i < deltaDimension; i++) {
		mean[i] /= size;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < deltaDimension; j++) {
			covariance [j] += (data[i][j] - mean[j])*(data[i][j] - mean[j]);
		}
	}

	for (int i = 0; i < deltaDimension; i++) {
		covariance [i] /= size;
	}

}

void singleGaussion::getData(double mean[40], double sd[40]) {
	for (int i = 0; i < deltaDimension; i++) {
		mean[i] = this->mean[i];
		sd[i] = this->covariance [i];
	}
}
