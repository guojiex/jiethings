#include "mathUtil.h"
#include <vector>
#include <sstream>
#include <assert.h>
#include "wavReadResult.h"
#include "readwave.h"
double vectorEuclideanDistance(const std::vector<double> &x,const std::vector<double> &y)
{
	double distance=0;
	assert(x.size()==y.size());
	int maxSize=x.size();
	for(int i=0;i<maxSize;i++)
	{
		distance+=(x[i]-y[i])*(x[i]-y[i]);
		//distance+=pow(x[i]-y[i],2);
		//distance+=(x[i]-y[i]);
		//printf("%f %f %f\n",x[i],y[i],distance);
	}
	distance=sqrt(distance);
	return distance;
}
std::vector<std::vector<short>> getIsolatewordFromSampleAndisspeech()
{
	std::vector<std::vector<short>> isolatedWords;
	return isolatedWords;
}
std::string getstring ( const int n )
{	
	std::stringstream newstr;
	newstr<<n;
	return newstr.str();
}
std::string sampleName[]={"zero","one","two","three","four","five","six",
	"seven","eight","nine"};
std::string createTemplatePath(int sampleNumber,int index)
{
	std::string base="./template/";
	base.append(sampleName[sampleNumber]).append("/").append(getstring(index)).append("/");
	//printf("%s\n",base.c_str());
	return base;
}

std::vector<std::vector<double>> readDeltaFromWavFile( std::string fileName )
{
	int num=0;
	int samplerate=0;
	short* data2=ReadWave(fileName.c_str(),&num,&samplerate);
	for(int i=0;i<num;++i)
		printf("%d ",data2[i]);
	wavReadResult resulttest=wavReadResult(data2,num,"test","1");
	free(data2);
	return resulttest.getMFCCDelta();
}
