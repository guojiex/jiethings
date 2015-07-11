#include "templateList.h"
#include "hmmTool.h"
#include <cassert>
#include "singleGaussion.h"
#include "4Gaussion.h"
TemplateList::TemplateList(int count, int segLength, int nCep) {
	//printf("tL2\n");
	//assert(count <= 10);	
	this->count = count;
	printf("TL count:%d\n",this->count);
	this->segLength = segLength;
	this->deltaDimension = nCep;
};

bool TemplateList::readMutiTemplate(vector<string>& filename) {
	//printf("%d %d\n",filename.size(),this->count);
	assert(filename.size() == count);
	for (int i = 0; i < count; i++) {
		templates[i].setTemplateInfo(segLength, deltaDimension);
		templates[i].readTemplate2(filename[i]);//.readTemplate(filename[i]);
	}

	return true;
}
HMMModel TemplateList::buildHMMModel(HMMModel::StateType type) {
	HMMModel hmmModel(segLength, type);
	for (int i = 0; i < count; i++) {
		templates[i].init();
	}
	
	int iteratorCount = 0;
	bool isConverge = false;
	do {
		iteratorCount++;
		printf("Begin iter %d\n",iteratorCount);
		hmmModel = generateTempModel(type);
		isConverge = true;
		for (int i = 0; i < count; i++) {
			doViterbiSearchAndResegment(hmmModel, templates[i]);
			if (!templates[i].isConverge()) {
				isConverge = false;
			}
		}
	} while (!isConverge);
	printf("Finish training\n");
	return hmmModel;
}

HMMModel TemplateList::generateTempModel(HMMModel::StateType type) {
	HMMModel hmmModel(segLength, type);
	double transitionCost[10][10] = {0};
	double initCost[10] = { 0 };

	// deal with the init cost
	int firstCount = 0, nCep;
	for (int i = 0; i < count; i++) {
		int size = 0;
		templates[i].getStageSize(size, nCep, 0);
		firstCount = (size != 0) ? firstCount + 1 : firstCount;		
	}
	for(int i=0;i<segLength;++i)
		initCost[i] = -log(10e-60);

	initCost[0] = -log((firstCount + 10e-60) / (double)count);
	initCost[1] = -log((count - firstCount + 10e-60) / (double)count);

	hmmModel.setInitCost(initCost);

	// 计算边cost和点cost
	for (int i = 0; i < segLength; i++) {
		//计算在当前i段里面有多少帧，和把数据parse到数组里面
		int size = 0;
		int stageSize, nCep;
		for (int j = 0; j < count; j++) {
			templates[j].getStageSize(stageSize, nCep, i);
			size += stageSize;
		}

		double** data = new double*[size];
		for (int j = 0; j < size; j++) {
			data[j] = new double[nCep];
		}

		int beginIndex = 0;
		//int nCluster = HMMModel::nCluster;
		for (int j = 0; j < count; j++) {
			int len = templates[j].fillData(data, i, beginIndex);
			beginIndex += len;
		}
		StateModel * model = NULL;
		if (type == HMMModel::singleType) {
			model = new singleGaussion(nCep);
		}else if(type==HMMModel::mutiType)
		{
			 model=new fourGaussion(nCep);
		}		
		
		if (size == 0) {
			int a = 0;
		}
		model->setNodeInfo(data, size, count);
		hmmModel.setStateModel(model, i);

		int selfCount, nextCount, superNextCount,totalSize = 0;
		selfCount = nextCount = superNextCount = 0;
		for (int j = 0; j < count; j++) {
			templates[j].getStageSize(stageSize, nCep, i);
			totalSize += stageSize;
			selfCount += stageSize - 1;
			if (i + 1 < segLength) {
				templates[j].getStageSize(stageSize, nCep, i+1);
				if (stageSize != 0) {
					nextCount += 1;
				}
				else if (i + 2 < segLength) {
					superNextCount += 1;
				}
				else {
					selfCount += 1;
				}
			}
		}
		if (totalSize != 0) {
			transitionCost[i][i] = -log((selfCount + 10e-60) / (double)totalSize);
			if (i + 1 < segLength) {
				transitionCost[i][i + 1] = -log((nextCount + 10e-60) / (double)totalSize);
			}
			if (i + 2 < segLength) {
				transitionCost[i][i + 2] = -log((superNextCount + 10e-60) / (double)totalSize);
			}
		}
		
		for (int j = 0; j < size; j++) {
			if(data[j]!=nullptr)
				delete[] data[j];
		}
		if(data!=nullptr)
			delete[] data;
		data=nullptr;
	}

	hmmModel.setTransitionCost(transitionCost);

	return hmmModel;
}