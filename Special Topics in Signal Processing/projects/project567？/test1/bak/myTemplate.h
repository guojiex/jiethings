#pragma once
#include "Segment.h"
#include <vector>
#include <string.h>
class myTemplate
{
private:
	Segment prevStage[10];
	Segment currStage[10];
	std::vector<std::vector<double>> deltaMatrix;
	int frameNumber;//这一个模板有多少帧一共
	int deltaDimension;//delta向量有多少维，默认39
	int segmentNumber;///分多少段
public:
	void initTemplateSegment();
	int getStageSize(const int &stageIndex);
	std::vector<std::vector<double>> getStageData(const int &stageIndex);
	myTemplate(void):frameNumber(0),deltaDimension(39),segmentNumber(5){}
	myTemplate(int segmentNumber,int deltaDimension):segmentNumber(segmentNumber),deltaDimension(deltaDimension){}
	double** getDeltaTo2DArray()
	{
		double** value=new double* [deltaMatrix.size()];
		for(int i=0;i<deltaMatrix.size();++i)
			value[i]=new double [deltaDimension];
		for(int i=0;i<deltaMatrix.size();++i)
			for(int j=0;j<deltaDimension;++j)
				value[i][j]=deltaMatrix[i][j];
		return value;
	}
	myTemplate(const std::vector<std::vector<double>> &temp)
	{
		this->readFromVector(temp);
		this->segmentNumber=5;
	}
	bool readFromFile(std::string fileName);
	void readFromVector(const std::vector<std::vector<double>> &temp);
	std::vector<std::vector<double>> getDeltaMatrix()
	{
		return this->deltaMatrix;
	}
	void setTemplateConfiguration(const int &Dimension,const int &segmentNum)
	{
		this->deltaDimension=Dimension;
		this->segmentNumber=segmentNum;
	}
	int getStageData(std::vector<std::vector<double>> &data,const int stageNo);
	bool isConverge();
	void setStateInfo(Segment seg[10]);
	~myTemplate(void);
};

