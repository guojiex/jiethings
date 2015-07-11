#include "templateList.h"
#include "myTemplate.h"
#include <assert.h>
#include "HMM3.h"
#include "stateModel.h"
#include "singleGaussion.h"
#include "hmmUtil.h"
templateList::templateList(void)
{
	this->deltaDimension=39;
	this->segmentNumber=5;
	this->templatesCount=0;
}


templateList::~templateList(void)
{
}

void templateList::readTemplatesFromVector( const std::vector<std::vector<std::vector<double>>> &data )
{
	for(int i=0;i<data.size();++i)
	{
		myTemplate temp=myTemplate(this->segmentNumber,this->deltaDimension);
		temp.readFromVector(data[i]);
		this->templates.push_back(temp);
	}
	this->templatesCount=templates.size();
}

void templateList::readMultiTemplate(const std::vector<std::string> &fileNames)
{	
	for(int i=0;i<fileNames.size();++i)
	{
		myTemplate temp=myTemplate(this->segmentNumber,this->deltaDimension);
		temp.readFromFile(fileNames[i]);
		this->templates.push_back(temp);
	}
	this->templatesCount=templates.size();
}

//************************************
// Method:    根据所有的模板产生一个模型，暂时只能是单高斯模型
// FullName:  templateList::generateTemplateModel
// Access:    public 
// Returns:   HMM3
// Qualifier:
// Parameter: HMM3::StateType type
//************************************
HMM3 templateList::generateTemplateModel( HMM3::StateType type )
{
	HMM3 model=HMM3(this->segmentNumber,type);
	double transCost[10][10];
	double initCost[10]={0};
	assert(templatesCount==templates.size());
	int firstStateNumber=0;
	int segSize=0;
	//计算第一个state中那么多个模板有多少个段
	for(int i=0;i<this->templatesCount;++i)
	{
		segSize=templates[i].getStageSize(0);
		if(0!=segSize)
			++firstStateNumber;		
	}
	for(int i=0;i<10;++i)
	{
		initCost[i]=-log(10e-20);
	}
	initCost[0]=-log(1.0*(firstStateNumber+10e-60)/segmentNumber);
	initCost[1]=-log(1.0*(segmentNumber-firstStateNumber+10e-60)/segmentNumber);
	model.setInitCost(initCost);
	int size=0;//计算位于同一个segment的那么多个模板的帧数和
	for(int stageNo=0;stageNo<segmentNumber;++stageNo)
	{
		std::vector<std::vector<double>> data;
		stateModel* stateModel=nullptr;
		for(int j=0;j<templatesCount;++j)
		{
			size+=templates[j].getStageSize(stageNo);
		}		
		for(int i=0;i<templatesCount;++i)
			templates[i].getStageData(data,stageNo);
		
		if (type==HMM3::singleType)
		{
			stateModel=new singleGaussion(this->deltaDimension);
		}
		assert(size!=0);

		stateModel->setNodeInfo(data,data.size(),deltaDimension);
		model.setStateModel(stateModel,stageNo);

		int stageSize,selfCount, nextCount, superNextCount,totalSize = 0;
		selfCount = nextCount = superNextCount = 0;
		for (int j = 0; j < templatesCount; j++) {
			stageSize=templates[j].getStageSize(stageNo);
			totalSize += stageSize;
			selfCount += stageSize - 1;
			if (stageNo + 1 < segmentNumber) {
				stageSize=templates[j].getStageSize(stageNo+1);
				if (stageSize != 0) {
					nextCount += 1;
				}
				else if (stageNo + 2 < segmentNumber) {
					superNextCount += 1;
				}
				else {
					selfCount += 1;
				}
			}
		}
		if (totalSize != 0) {
			transCost[stageNo][stageNo] = -log((selfCount + 10e-60) / (double)totalSize);
			if (stageNo + 1 < segmentNumber) {
				transCost[stageNo][stageNo + 1] = -log((nextCount + 10e-60) / (double)totalSize);
			}
			if (stageNo + 2 < segmentNumber) {
				transCost[stageNo][stageNo + 2] = -log((superNextCount + 10e-60) / (double)totalSize);
			}
		}
	}
	model.setTranstionCost(transCost);
	return model;
}

//************************************
// Method:    产生一个模型
// FullName:  templateList::generateHMMModel
// Access:    public 
// Returns:   HMM3
// Qualifier:
// Parameter: HMM3::StateType type
//************************************
HMM3 templateList::generateHMMModel( HMM3::StateType type )
{
	HMM3 hmmModel(segmentNumber, type);
	for(int i=0;i<templatesCount;++i)
		templates[i].initTemplateSegment();
	int iterTurn=0;
	bool isConverge=false;
	do{
		printf("generate hmm turn:%d\n",iterTurn);
		hmmModel=generateTemplateModel(type);
		isConverge=true;
		for(int i=0;i<templatesCount;++i)
		{
			doResegmentTemplateByHMM(hmmModel,templates[i]);
			//doViterbiSearch(hmmModel,templates[i]);
			if(!templates[i].isConverge())
				isConverge=false;
		}
		++iterTurn;
	}while(!isConverge);
	return hmmModel;
}
