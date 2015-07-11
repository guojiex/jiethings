#include "hmmTool.h"
#include "templateList.h"
#include "../mathUtil.h"
#include <iostream>
#include "../wavReadResult.h"
//#define SEGMENT_NUMBER 7
#define TEMPLATE_NUMBER_IN_MODEL 5  
#define MODEL_NUMBER 10
#define TEST_CASE_NUMBER 10
#define DELTADIMENSION 39                
template <class T>
void safe_deleteCepMatrix(T** matrix,const int &rowNumber)
{
	if(matrix!=nullptr)
	{
		for (int i = 0; i < rowNumber; i++) {
			if(matrix[i]!=nullptr)
				delete[] matrix[i];
		}
		delete[] matrix;
	}
}
double doViterbiSearchAndResegment( HMMModel& hmmModel, Template& singleTemplate )
{
	int size, nCep, stateCount;
	double** cepMatrix = singleTemplate.getCepMatrix(size, nCep);
	stateCount = hmmModel.getstateCount();
	double** map = new double*[size];
	int** path = new int*[size];
	for (int i = 0; i < size; i++) {
		map[i] = new double[stateCount+1];
		path[i] = new int[stateCount+1];
	}
	for (int i = 0; i < size; i++) {
		map[i][0] = 0;
	}
	for (int i = 0; i < stateCount; i++) {
		map[0][i] = hmmModel.getInitCost(i);
	}
	for (int i = 1; i < size; i++) {
		for (int j = 0; j < stateCount; j++) {
			double current, low, superlow;
			current = hmmModel.getEdgeCost(j,j) + map[i-1][j];
			low = superlow = current + 10;
			if (j > 1) {
				low = hmmModel.getEdgeCost(j - 1, j) + map[i - 1][j - 1];
				superlow = hmmModel.getEdgeCost(j - 2, j) + map[i - 1][j - 2];
			}
			else if (j > 0) {
				low = hmmModel.getEdgeCost(j - 1, j) + map[i - 1][j - 1];
			}

			double minValue = min(current, min(superlow, low));
			double nodeCost = hmmModel.getNodeCost(j, cepMatrix[i], nCep);
			if (_isnan(nodeCost)) {
				int a = 0;
			}
			nodeCost = hmmModel.getNodeCost(j, cepMatrix[i], nCep);
			if (minValue == current) {
				map[i][j] = current + nodeCost;
				if (_isnan(map[i][j])) {
					//cout << map[i][j] << " ";
					int a = 0;
				}
				path[i][j] = j;
			}
			else if (minValue == low) {
				map[i][j] = low + nodeCost;
				path[i][j] = j-1;
			}
			else {
				map[i][j] = superlow + nodeCost;
				path[i][j] = j - 2;
			}
		}
	}

	double minCost = map[size-1][0];
	int index = 0;
	for (int i = 0; i < stateCount; i++) {
		//cout << map[size - 1][i] << " ";
		if (map[size - 1][i] < minCost) {
			minCost = map[size - 1][i];
			index = i;
		}
		assert(index<stateCount);
	}
	//cout << "\n";

	Segment state[10];
	int forward = index;
	int endIndex = size;
	int currentFrame = size - 1;
	while (currentFrame > 0) {
		int forward = path[currentFrame][index];
		if (forward == index - 2) {
			state[index] = Segment(currentFrame, endIndex, true);
			state[index - 1] = Segment(currentFrame, currentFrame, false);
			//printf("%d %d\n",currentFrame,endIndex);
			
			endIndex = currentFrame;
		}
		else if (forward == index - 1) {
			state[index] = Segment(currentFrame, endIndex, true);
			//printf("%d %d\n",currentFrame,endIndex);
			endIndex = currentFrame;
		}

		index = forward;
		currentFrame--;
	}
	state[index] = Segment(0, endIndex, true);
	//printf("%d %d\n",0,endIndex);
	singleTemplate.setStageInfo(state);
	//safe_deleteCepMatrix(cepMatrix,size);
	safe_deleteCepMatrix(path,size);
	safe_deleteCepMatrix(map,size);
	return minCost;
}

double getHMMCompareProb(HMMModel& hmmModel, Template& singleTemplate) {
	int size, nCep, stateCount;
	double** cepMatrix = singleTemplate.getCepMatrix(size, nCep);
	stateCount = hmmModel.getstateCount();
	double** map = new double*[size];
	for (int i = 0; i < size; i++) {
		map[i] = new double[stateCount + 1];
		for (int j = 0; j < stateCount; j++) {
			map[i][j] = 0;
		}
	}
	for (int i = 0; i <= stateCount; i++) {
		map[0][i] = log(hmmModel.getInitProb(i));
	}
	
	for (int i = 1; i < size; i++) {
		for (int j = 0; j < stateCount; j++) {
			double logCurrent, logLow, logSuperlow;
			logCurrent = logLow = logSuperlow = 1e-60;
			double logEdgeProb = hmmModel.getEdgeProb(j, j);
			double logNodeProb = hmmModel.getNodeProb(j, cepMatrix[i], nCep);
			double prev = map[i - 1][j];
			logCurrent = log(logEdgeProb) + log(logNodeProb) + prev;
			//logCurrent=logEdgeProb+logNodeProb+ prev
			if (_isnan(logCurrent)) {
				int a = 0;
			}
			if (j > 1) {
				logLow = log(hmmModel.getEdgeProb(j - 1, j)) + log(hmmModel.getNodeProb(j, cepMatrix[i], nCep)) + map[i - 1][j - 1];
				logSuperlow = log(hmmModel.getEdgeProb(j - 2, j)) + log(hmmModel.getNodeProb(j, cepMatrix[i], nCep)) + map[i - 1][j - 2];
				double logSum = logLow + log(1 + pow(2.718281828, logSuperlow - logLow));
				map[i][j] = logCurrent + log(1 + pow(2.718281828, logSum - logCurrent));
			}
			else if (j > 0) {
				logLow = log(hmmModel.getEdgeProb(j - 1, j)) + log(hmmModel.getNodeProb(j, cepMatrix[i], nCep)) + map[i - 1][j - 1];
				map[i][j] = logCurrent + log(1 + pow(2.718281828, logLow - logCurrent));
			}
			else {
				map[i][j] = logCurrent;
			}

		}
	}
	double totalProb = 0;
	for (int i = 0; i < stateCount; i++) {
		totalProb += map[size - 1][i];
	}
	std::cout << "total: " << totalProb << " " << totalProb / pow(size, stateCount) << endl;
	safe_deleteCepMatrix(map,size);
	//return totalProb / pow(size,stateCount);
	return totalProb;
}
void initHMMModel(HMMModel hmmModels[10]) {
	string filePath = ".\\template\\";
	string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	for (int i = 0; i < MODEL_NUMBER; i++) {
		string singleFilePath = filePath + sampleName[i] + ".hmm";
		hmmModels[i].readHMMModel(singleFilePath);
	}
}
int testSingleNumber(HMMModel hmmModels[10],const vector<vector<double>> &data,int type)
{
	string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	Template singleTemplate;
	singleTemplate.readVector(data);
	int bestIndex = 0;
	double maxProb = -1e20;
	double minCost = 1e20;
	if (type == viterbiSearch) {
		for (int i = 0; i < MODEL_NUMBER; i++) {
			double cost = doViterbiSearchAndResegment(hmmModels[i], singleTemplate);
			if (minCost > cost) {
				minCost = cost;
				bestIndex = i;
			}
		}
	}else if(type==forwardAlgo)
	{
		for (int i = 0; i < MODEL_NUMBER; i++) {
			double cost = getHMMCompareProb(hmmModels[i], singleTemplate);
			if (minCost < cost) {
				minCost = cost;
				bestIndex = i;
			}
		}
	}
	std::cout << minCost;	
	std::cout << " result: " << sampleName[bestIndex] << std::endl;
	return 0;
}
int testSingleNumber(HMMModel hmmModels[10],int number, int index, int type) {
	string filePath = ".\\template\\";
	string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	Template singleTemplate;
	string singleFilePath = filePath + sampleName[number] +  "\\" + getstring(index) + "\\delta.txt";
	singleTemplate.readTemplate2(singleFilePath);//.readTemplate(singleFilePath);
	int bestIndex = 0;
	double maxProb = -1e20;
	double minCost = 1e20;
	if (type == viterbiSearch) {
		//double minCost = 1e20;
		for (int i = 0; i < MODEL_NUMBER; i++) {
			double cost = doViterbiSearchAndResegment(hmmModels[i], singleTemplate);
			if (minCost > cost) {
				minCost = cost;
				bestIndex = i;
			}
		}
	}else if(type==forwardAlgo)
	{
		for (int i = 0; i < MODEL_NUMBER; i++) {
			double cost = getHMMCompareProb(hmmModels[i], singleTemplate);
			if (minCost < cost) {
				minCost = cost;
				bestIndex = i;
			}
		}
	}
	std::cout << minCost;
	std::cout << " number: " << sampleName[number];
	std::cout << " result: " << sampleName[bestIndex] << std::endl;

	if (number == bestIndex) {
		return 1;
	}
	return 0;
}
void HMMtest(wavReadResult &result)
{
	string filePath = ".\\template\\";
	string hmmPath = filePath + "one.hmm";
	string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	string indexs[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

	int cmd;	
	int right = 0;
	int total = 0;
	TestType selectAlgo = viterbiSearch;//TestType::forwardAlgo;
	int indexLeft=1;
	int indexRight=6;
	int segmentNumber=7;
	HMMModel hmmModels[10];
	HMMModel::StateType type=HMMModel::singleType;//::singleType;
	HMMModel hmmmodel(indexRight-indexLeft+1,type);
	while(1)
	{
		printf("1.build HMM to file.\n");
		printf("2.read HMM from file.\n");
		printf("3.test single.\n");
		printf("4.test all.\n");
		printf("5.exit.\n");
		std::cin >> cmd;
		switch (cmd)
		{
		case 1:
			for (int i = 0; i < 10; i++) {
				TemplateList templates(indexRight-indexLeft+1,segmentNumber, DELTADIMENSION);
				vector<string> files;
				for (int j = indexLeft; j <=indexRight; j++) {
					files.push_back(filePath + sampleName[i] +  "\\" + getstring(j) + "\\delta.txt");
				}
				templates.readMutiTemplate(files);

				hmmmodel= templates.buildHMMModel(type);
				hmmmodel.writeHMMModel(filePath + sampleName[i] + ".hmm");
				printf("%s%s.hmm write done!\n",filePath.c_str(),sampleName[i].c_str());
			}
			break;
		case 2:
			initHMMModel(hmmModels);
			break;
		case 3:
			initHMMModel(hmmModels);
			testSingleNumber(hmmModels,result.getMFCCDelta(),selectAlgo);
			break;
		case 5:
			system("pause");
			exit(0);
			break;
		default:
			system("pause");
			exit(0);
			break;
		}
	}
}
void HMMtest()
{
	string filePath = ".\\template\\";
	string hmmPath = filePath + "one.hmm";
	string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	//string indexs[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };	
	int cmd;	
	int right = 0;
	int total = 0;
	TestType selectAlgo = viterbiSearch;
	int indexLeft=1;
	int indexRight=11;
	int segmentNumber=5;
	HMMModel hmmModels[10];
	HMMModel::StateType type=HMMModel::singleType;
	HMMModel hmmmodel(indexRight-indexLeft+1,type);
	while(1)
	{
		printf("1.build HMM to file.\n");
		//printf("2.read HMM from file.\n");
		//printf("3.test single.\n");
		printf("4.test all.\n");
		printf("5.exit.\n");
		std::cin >> cmd;
		switch (cmd)
		{
		case 1:
			for (int i = 0; i < 10; i++) {

				TemplateList templates(indexRight-indexLeft+1,segmentNumber, DELTADIMENSION);
				vector<string> files;
				for (int j = indexLeft; j <=indexRight; j++) {
					files.push_back(filePath + sampleName[i] +  "\\" + getstring(j) + "\\delta.txt");
					//printf("%s \n",files[files.size()-1].c_str());
				}
				templates.readMutiTemplate(files);
			
				hmmmodel= templates.buildHMMModel(type);
				hmmmodel.writeHMMModel(filePath + sampleName[i] + ".hmm");
				//cout << filePath<<  sampleName[i] <<  ".hmm" <<  " has done!" << endl;
				printf("%s%s.hmm write done!\n",filePath.c_str(),sampleName[i].c_str());
			}
			break;
		case 2:
			initHMMModel(hmmModels);
			break;
		case 3:
			break;
		case 4:
			initHMMModel(hmmModels);
			for (int i = 0; i < 10; i++) {
				for (int j = 12; j < 16; j++) {
					right += testSingleNumber(hmmModels,i, j, selectAlgo);
					total += 1;
				}
			}
			std::cout << std::endl;
			std::cout << " Right " << right;
			std::cout << " Total " << total;
			std::cout << " Rate: " << right / (double)total << std::endl;
			break;
		case 5:
			system("pause");
			exit(0);
			break;
		default:
			system("pause");
			exit(0);
			break;
		}
	}
}

