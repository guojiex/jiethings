#include "singleGaussion.h"
#include <assert.h>
#define PI 3.14159
singleGaussion::singleGaussion(void)
{
	this->deltaDimension=39;
	this->means.resize(deltaDimension);
	this->covariances.resize(deltaDimension);
}


singleGaussion::~singleGaussion(void)
{
}

double singleGaussion::getNodeCost(const std::vector<double> &node/*39维*/)
{
	int size=node.size();
	assert(size==39);
	double cost=0;
	double leftPart=1;
	for(int i=0;i<size;++i)
	{
		leftPart*=covariances[i];
	}
	double rightPart=0;
	for(int i=0;i<size;++i)
	{
		rightPart+=(node[i]-means[i])*(node[i]-means[i])/covariances[i];
	}
	cost=0.5*log(pow(2*PI,size)*leftPart)+0.5*rightPart;
	if(_isnan(cost))
	{
		printf("cost is nan ");
		printf("%lf %lf\n",covariances[0],rightPart);
		system("pause");
		cost=0;
	}
	return cost;
}

void singleGaussion::setNodeInfo(const std::vector<std::vector<double>> &data, int size, int nCep)
{
	means.clear();
	this->deltaDimension=nCep;
	means.resize(nCep);
	covariances.clear();
	covariances.resize(nCep);
	assert(size==data.size());
	printf("size %d %d %d \n",size,data.size(),nCep);
	for(int j=0;j<size;++j)
	{
		for(int i=0;i<nCep;++i)
		{
			printf("%d %d %d",j,i,nCep);
			means[i]+=data[j][i];
		}
	}
	/*printf("start a new\n");
	for(int j=0;j<data.size();++j)
	{
		for(int i=0;i<this->deltaDimension;++i)
		{
			printf("%lf ",data[j][i]);
		}
		printf("\n");
	}	
	printf("\n");
	for(int i=0;i<this->deltaDimension;++i)
	{
		printf("%lf ",means[i]);
	}
	printf("\n");*/
	//system("pause");
	for(int i=0;i<this->deltaDimension;++i)
		means[i]/=size;
	for(int i=0;i<size;++i)
		for(int dimension=0;dimension<deltaDimension;++dimension)
		{
			covariances[dimension]+=(data[i][dimension]-means[dimension])*
				(data[i][dimension]-means[dimension]);
		}	
	for(int i=0;i<this->deltaDimension;++i)
	{
		covariances[i]/=size;
		//printf("%lf %lf",covariances[i],means[i]);
	}
	//printf("\n");
}

double singleGaussion::getNodeProb(const std::vector<double> &node)//lecture6 p79
{
	double prob=0,leftpart=1,rightpart=0;
	for(int i=0;i<deltaDimension;++i)
	{
		leftpart*=2*PI*covariances[i];
		rightpart+=pow((node[i]-means[i]),2)/covariances[i];
	}
	leftpart=1.0/sqrt(leftpart);
	rightpart=exp(0.5*rightpart);
	prob=leftpart*rightpart;
	assert(prob!=0);
	return prob;
}
