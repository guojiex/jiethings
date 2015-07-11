#include "4Gaussion.h"
#include <assert.h>
#include <time.h>
fourGaussion::~fourGaussion(){}

double fourGaussion::getNodeCost( const double node[40] )
{
	double cost=0;
	for(int i=0;i<GaussionNumber;++i)
	{
		cost+=weight[i]*sg[i].getNodeCost(node);
	}
	return cost;
}

double fourGaussion::getNodeProb( const double node[40] )
{
	throw std::exception("The method or operation is not implemented.");
}

void fourGaussion::setNodeInfo(double** data, int size, int templateCount)
{
	sg[0]=singleGaussion(deltaDimension);
	sg[0].setNodeInfo(data,size,templateCount);
	dataForGausssion.resize(size);
	framesForPerGaussion.resize(GaussionNumber);
	framesForPerGaussion[0].resize(size);
	for(int i=0;i<size;++i)
	{				
		dataForGausssion[i].resize(deltaDimension);
		for(int j=0;j<deltaDimension;++j)
		{
			dataForGausssion[i][j]=data[i][j];
		}		
	}
	framesForPerGaussion[0].resize(size);
	for(int i=0;i<size;++i)
	{
		framesForPerGaussion[0][i].resize(deltaDimension);
		for(int j=0;j<deltaDimension;++j)
			framesForPerGaussion[0][i][j]=dataForGausssion[i][j];
	}
	weight[0]=1;
	weight[1]=0;
	weight[2]=0;
	weight[3]=0;
	int numberOfGaussion=1;
	while (numberOfGaussion!=GaussionNumber)
	{
		selectALargeAndSpilt(numberOfGaussion);
		++numberOfGaussion;
		renewWeight(numberOfGaussion);
	}	
}
void fourGaussion::renewWeight(const int guassionNumberNow)
{
	int total=0;
	for(int i=0;i<guassionNumberNow;++i)
		total+=framesForPerGaussion[i].size();
	for(int i=0;i<guassionNumberNow;++i)
		weight[i]=(double)framesForPerGaussion[i].size()/total;
}
void fourGaussion::selectALargeAndSpilt( int numberOfGaussion )
{
	double maxCov=-1;
	int maxIndex=-1;
	double temp=0;
	double cov[40];
	printf("%d numbergaussion",numberOfGaussion);
	for(int i=0;i<numberOfGaussion;++i)
	{
		temp=0;
		sg[i].getCovariance(cov);
		if(numberOfGaussion==1)
		{
			maxIndex=0;
			break;
		}else
		{
			for(int j=0;j<deltaDimension;++j)
				temp+=cov[j];
			if(maxCov==-1)
			{
				maxCov=temp;
				maxIndex=i;
				continue;
			}
			if(temp>maxCov)
			{
				maxCov=temp;
				maxIndex=i;
			}
		}
	}
	spiltGaussionToTwo(maxIndex,numberOfGaussion);
	++numberOfGaussion;
	int iter=0;
	int framesOld=-1;
	int framesNew=framesForPerGaussion[0].size();
	double sg0=0;
	double val[40];
	sg[0].getMean(val);
	double sg01=val[0];
	while(1)
	{
		printf("guassion iter %d\n",iter);
		//framesOld=framesNew;
		sg0=sg01;
		resegment(numberOfGaussion);
		sg[0].getMean(val);
		//framesNew=framesForPerGaussion[0].size();
		sg01=val[0];
		printf("old frames %d new frames %d\n",framesOld,framesNew);
		if(abs(sg0-sg01)<0.1)
			break;
		++iter;
	}	
	printf("guassion resegment finish\n");
}
void fourGaussion::resegment(const int numberOfGaussion)
{
	for(int i=0;i<GaussionNumber;++i)
		framesForPerGaussion[i].clear();
	int maxIndex=-1;
	double maxProb=-1;
	double temp=0;
	double node[40];
	for(int i=0;i<dataForGausssion.size();++i)
	{
		for(int z=0;z<deltaDimension;++z)
		{
			node[z]=dataForGausssion[i][z];
		}
		for(int j=0;j<numberOfGaussion;++j)
		{
			temp=sg[j].getNodeCost(node);
			if(maxIndex==-1)
			{
				maxIndex=j;
				maxProb=temp;
			}else if(temp>maxProb)
			{
				maxProb=temp;
				maxIndex=j;
			}
		}
		framesForPerGaussion[maxIndex].push_back(dataForGausssion[i]);
	}
	for(int i=0;i<numberOfGaussion;++i)
	{
		printf("%d num:%d, ",i,framesForPerGaussion[i].size());
		sg[i].setNodeInfo(framesForPerGaussion[i],framesForPerGaussion[i].size(),10);
	}
	printf("\n");
}
void fourGaussion::spiltGaussionToTwo(const int from,const int to)
{
	double meanTemp[40];
	double meanTemp2[40];
	double cov[40];
	double sigma=1;
	sg[from].getCovariance(cov);
	srand((unsigned)time(NULL));
	int index= rand() % framesForPerGaussion[from].size();
	int index2=rand() % framesForPerGaussion[from].size();
	while (index==index2)
	{
		index2=rand() % framesForPerGaussion[from].size();
	}
	for(int i=0;i<deltaDimension;++i)
	{
		meanTemp[i]=framesForPerGaussion[from][index][i];		
		meanTemp2[i]=framesForPerGaussion[from][index2][i];		
	}
	sg[from].Mean(meanTemp);
	sg[to].Mean(meanTemp2);
	sg[to].Covariance(cov);
	//sg[to].getMean(meanTemp2);	
}
int fourGaussion::getCount()
{
	return deltaDimension;
}
