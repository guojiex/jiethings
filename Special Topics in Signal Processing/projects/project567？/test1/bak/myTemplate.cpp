#include "myTemplate.h"
#include <minmax.h>
#include <assert.h>

myTemplate::~myTemplate(void)
{
}

void myTemplate::initTemplateSegment()
{
	int size=frameNumber/segmentNumber;//每个segment有几个帧，初始化的时候平均分整个template
	for(int i=0;i<segmentNumber;++i)
		currStage[i]=Segment(i*size,min(size*(i+1),frameNumber)-1,true);
}

void myTemplate::readFromVector( const std::vector<std::vector<double>> &temp )
{
	this->deltaMatrix.clear();
	this->deltaMatrix=temp;
	this->frameNumber=deltaMatrix.size();
	this->deltaDimension=deltaMatrix[0].size();
}

bool myTemplate::readFromFile(std::string fileName)
{
	this->deltaMatrix.clear();
	FILE *fq;
	fq=fopen(fileName.c_str(),"r+");
	if(fq==NULL)
	{
		printf("fatal:file not found or load error.\n");
		system("pause");
		exit(-1);
	}
	std::vector<double> oneline;
	oneline.resize(39);
	int index=0;
	while (fscanf(fq,"%lf",&oneline[index])!=EOF)
	{
		index++;
		if(index>=39){
			deltaMatrix.push_back(oneline);
			index=0;
		}
	}	
	this->frameNumber=deltaMatrix.size();
	fclose(fq);
	printf("read delta file %s \n",fileName.c_str());
	return true;
}

int myTemplate::getStageSize(const int &stageIndex)
{
	return this->currStage[stageIndex].getLength();
}

std::vector<std::vector<double>> myTemplate::getStageData(const int &stageIndex)
{
	std::vector<std::vector<double>> data;
	int endIndex=currStage[stageIndex].endIndex;
	for(int i=currStage[stageIndex].beginIndex;i<=endIndex;++i)
		data.push_back(this->deltaMatrix[i]);
	return data;
}

int myTemplate::getStageData(std::vector<std::vector<double>> &data,const int stageNo)
{
	if(!currStage[stageNo].exist)
		return 0;
	std::vector<std::vector<double>> temp=this->getStageData(stageNo);
	assert(temp.size()==currStage[stageNo].getLength());
	for(int i=0;i<temp.size();++i)
		data.push_back(temp[i]);
	return temp.size();
}

bool myTemplate::isConverge()
{
	int threshold = 1;
	for(int i=0;i<segmentNumber;++i)
	{
		if(abs(currStage[i].beginIndex-this->prevStage[i].beginIndex)>threshold||abs(currStage[i].endIndex-prevStage[i].endIndex>threshold))
			return false;
	}
	return true;
}

void myTemplate::setStateInfo( Segment seg[10] )
{
	for(int i=0;i<segmentNumber;++i)
	{
		this->prevStage[i]=this->currStage[i];
		this->currStage[i]=seg[i];
	}
}
