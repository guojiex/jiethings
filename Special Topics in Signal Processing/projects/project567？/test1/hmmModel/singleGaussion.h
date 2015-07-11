#ifndef _SG_MODEL_H_
#define _SG_MODEL_H_
#include "stateModel.h"
#include <vector>
# define PI          3.141592653589793238
class singleGaussion : public StateModel {
private :
	double mean[40];
	double covariance [40];	
	int deltaDimension;
public:
	void getMean(double val[40])
	{
		for(int i=0;i<deltaDimension;++i)
			val[i]=mean[i] ;
	}
	void Mean(double val[40])
	{
		for(int i=0;i<deltaDimension;++i)
			mean[i]=val[i];
	}
	void getCovariance(double val[40]){ 
		for(int i=0;i<deltaDimension;++i)
			val[i]=covariance[i] ;
	}
	void Covariance(double val[40]) { 
		for(int i=0;i<deltaDimension;++i)
			covariance[i] = val[i]; 
	}
	singleGaussion() {deltaDimension=39;};
	singleGaussion(int deltaDimension) : deltaDimension(deltaDimension) {};
	singleGaussion(double mean[40], double sd[40], int length);
	void getData(double mean[40], double sd[40]);
	virtual double getNodeProb(const double node[40]);
	virtual double getNodeCost(const double node[40]);
	virtual void setNodeInfo(double** data, int size, int templateCount);
	void singleGaussion::setNodeInfo(std::vector<std::vector<double>> &data, int size, int templateCount);
	virtual int getCount() { return deltaDimension; }
};

#endif