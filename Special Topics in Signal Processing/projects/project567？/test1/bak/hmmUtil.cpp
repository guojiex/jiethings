#pragma once
#include "hmmUtil.h"
#include <minmax.h>
#include <time.h>  
double doViterbiSearch(HMM3& hmmModel, myTemplate& singleTemplate) {
	int size, nCep, stateCount;
	double** cepMatrix = singleTemplate.getDeltaTo2DArray();
	stateCount = hmmModel.getSegmentNumber();
	std::vector<std::vector<double>> data=singleTemplate.getDeltaMatrix();
	size=data.size();
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
			double nodeCost = hmmModel.getNodeCost(j,data[i]);//.getNodeCost(j, cepMatrix[i], nCep);
			if (_isnan(nodeCost)) {
				int a = 0;
			}
			//nodeCost = hmmModel.getNodeCost(j, cepMatrix[i], nCep);
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
			endIndex = currentFrame;
		}
		else if (forward == index - 1) {
			state[index] = Segment(currentFrame, endIndex, true);
			endIndex = currentFrame;
		}

		index = forward;
		currentFrame--;
	}
	state[index] = Segment(0, endIndex, true);
	singleTemplate.setStateInfo(state);//.setStageInfo(state);

	return minCost;
}
double DTWminCostFromHMM(HMM3 &model,myTemplate &myTemp)
{
	//double minCost=0;
	std::vector<std::vector<double>> data=myTemp.getDeltaMatrix();
	int frameNumber=data.size();
	int segmentNumber=model.getSegmentNumber();
	std::vector<std::vector<double>> costMatrix;
	costMatrix.resize(segmentNumber);
	//一共有帧数那么多列，分段+1那么多行（人工添加一个初始状态）
	for(int i=0;i<costMatrix.size();++i)
	{
		costMatrix[i].resize(frameNumber);
	}	
	double current,low,superlow,minValue,nodecost;
	for(int j=0;j<frameNumber;++j)
	{
		for(int i=0;i<segmentNumber;++i)
		{						
			if(j==0)
			{
				current=model.getInitCost(i);//+model.getNodeCost(i,data[j]);
				costMatrix[i][j]=current+model.getNodeCost(i,data[j]);
			}
			else
			{
				current = model.getEdgeCost(i,i) + costMatrix[i][j-1];
				low = superlow = current + 10;
				if(i>1)
				{
					superlow=model.getEdgeCost(i-2,i) + costMatrix[i-2][j-1];
					low=model.getEdgeCost(i-1,i) + costMatrix[i-1][j-1];
				}else if(i>0)
					low=model.getEdgeCost(i-1,i) + costMatrix[i-1][j-1];

				minValue=min(current,min(low,superlow));
				nodecost=model.getNodeCost(i,data[j]);
				if(_isnan(nodecost))
				{
					printf("node cost Nan error!\n");
					system("pause");
				}
				costMatrix[i][j]=minValue+nodecost;
			}
		}
	}//到这里就把cost矩阵，和最佳路径矩阵(前指针，代表前一个状态)
	double minCost1 = costMatrix[0][frameNumber-1];
	//int index = 0;
	for (int i = 0; i < segmentNumber; i++) {
		if (costMatrix[i][frameNumber-1] < minCost1) {
			minCost1 = costMatrix[i][frameNumber-1];
			//index = i;
		}
	}
	return minCost1;
}
DTW::DTW_result getMinCostFromHMMModels(HMM3 models[10],myTemplate &myTemp)
{
	DTW::DTW_result res;
	res.minRes =doViterbiSearch(models[0],myTemp);
	res.minIndex=0;
	double temp=0;
	for(int i=1;i<10;++i)
	{
		temp=doViterbiSearch(models[i],myTemp);
		if(temp<res.minRes)
		{
			res.minIndex=i;
			res.minRes=temp;
		}
	}
	return res;
}
void multTestHMM(const DTW &test,HMM3 models[10], int dataIndexLeft, int dataIndexRight,int left,int right)
{
	int correct=0;
	int errorNumber=0;
	DTW::DTW_result best;
	double rate=0;
	time_t start ,end ;  
	double cost;  
	time(&start);
	std::string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	printf("\nStart DTW------------\n");
	for(int j=dataIndexLeft;j<=dataIndexRight;j++)
	{
		for(int i2=left;i2<=right;i2++)
		{		
			myTemplate temp(test.templates[j][i2]);
			best=getMinCostFromHMMModels(models,temp);
			printf("res:%.3lf %d %d data:%s reg:%s\n",best.minRes,j,i2,sampleName[j].c_str(),sampleName[best.minIndex].c_str());			
			if(best.minIndex==j)
			{			
				correct++;
			}
			else
				errorNumber++;
		}
	}
	rate=1.0*correct/(correct+errorNumber);
	time(&end);  
	cost=difftime(end,start);  	
	printf("time spend %.3lf seconds.\n",cost); 
	printf("correct:%d error:%d correct rate:%f \n",correct,errorNumber,rate);
	printf("End of DTW---------------\n");
}
void HMMTest()
{
	DTW test;
	int a[]={0,1,2,3,4,5,6,7,8,9,10,11};
	size_t count=sizeof(a)/sizeof(int);
	std::vector<std::vector<std::vector<double>>> temp;
	templateList list1;
	HMM3 wordModels[10];
	for(int i=0;i<10;++i)
	{
		temp.clear();
		for(int j=0;j<count;++j)
			temp.push_back(test.templates[i][a[j]]);
		list1.readTemplatesFromVector(temp);
		wordModels[i]= list1.generateHMMModel(HMM3::singleType);
	}
	multTestHMM(test,wordModels,0,9,10,14);
}
double doResegmentTemplateByHMM(HMM3 &model,myTemplate &myTemp)
{
	DTW::DTW_result best;
	std::vector<std::vector<double>> data=myTemp.getDeltaMatrix();
	int frameNumber=data.size();
	int segmentNumber=model.getSegmentNumber();
	std::vector<std::vector<double>> costMatrix;
	std::vector<std::vector<int>> pathMatrix;
	costMatrix.resize(segmentNumber);
	pathMatrix.resize(segmentNumber);
	//一共有帧数那么多列，分段+1那么多行（人工添加一个初始状态）
	for(int i=0;i<costMatrix.size();++i)
	{
		costMatrix[i].resize(frameNumber);
		pathMatrix[i].resize(frameNumber);
	}
	//for(int i=0;i<frameNumber;++i)
	//{
	//	costMatrix[0][i]=0;
	//}
	//for(int i=1;i<segmentNumber/*多了一个人工添加的初始状态啊*/;++i)
	//	costMatrix[i][0]=model.getInitCost(i-1);//比如原本的第0个segment就应该放在第1的位置，因为第0的位置已经被占用了啊
	//for(int j=0;j<frameNumber;++j)
	//{
	//	for(int i=1;i<segmentNumber;++i)//注意这里调用edgecost的时候，如果使用i，调用的stage应该是i-1
	//	{
	//		double current,low,superlow;
	//		current=model.getEdgeCost(i-1,i-1)+costMatrix[i-1][j];
	//	}
	//}
	double current,low,superlow,minValue,nodecost;
	for(int j=0;j<frameNumber;++j)
	{
		for(int i=0;i<segmentNumber;++i)
		{						
			if(j==0)
			{
				minValue=model.getInitCost(i);//+model.getNodeCost(i,data[j]);
				printf("minvalue %lf \n",minValue);
				nodecost=model.getNodeCost(i,data[j]);
				costMatrix[i][j]=minValue+nodecost;
				//pathMatrix[i][j]=0;
			}else
			{
				current = model.getEdgeCost(i,i) + costMatrix[i][j-1];
				low = superlow = current + 10;
				if(i>1)
				{
					superlow=model.getEdgeCost(i-2,i) + costMatrix[i-2][j-1];
					low=model.getEdgeCost(i-1,i) + costMatrix[i-1][j-1];
				}else if(i>0)
					low=model.getEdgeCost(i-1,i) + costMatrix[i-1][j-1];

				minValue=min(current,min(low,superlow));
				nodecost=model.getNodeCost(i,data[j]);
				if(_isnan(nodecost))
				{
					printf("node cost Nan error!\n");
					system("pause");
				}
				costMatrix[i][j]=minValue+nodecost;
				if(minValue==current)		
					pathMatrix[i][j]=i;
				else if(minValue==low)
					pathMatrix[i][j]=i-1;
				else
				{
					pathMatrix[i][j]=i-2;
				}
			}
		}
	}//到这里就把cost矩阵，和最佳路径矩阵(前指针，代表前一个状态)
	double minCost2 = costMatrix[0][frameNumber-1];
	printf("mincost:%d %d %lf\n",0,frameNumber-1,costMatrix[0][frameNumber-1]);
	int index = 0;
	for (int i = 0; i < segmentNumber; i++) {
		//printf("mincost:%d %d %lf\n",i,frameNumber-1,costMatrix[i][frameNumber-1]);
		if (costMatrix[i][frameNumber-1] < minCost2) {
			minCost2 = costMatrix[i][frameNumber-1];			
			index = i;
		}
	}
	//printf("index:%d\n",index);
	Segment state[10];
	int forward = index;
	int endIndex = frameNumber-1;
	int currentFrame = frameNumber - 1;
	while (currentFrame > 0) {
		forward = pathMatrix[index][currentFrame];
		if (forward == index - 2) {
			state[index] = Segment(currentFrame, endIndex, true);
			state[index - 1] = Segment(currentFrame, currentFrame, false);
			endIndex = currentFrame-1;
		}
		else if (forward == index - 1) {
			state[index] = Segment(currentFrame, endIndex, true);
			endIndex = currentFrame-1;
		}

		index = forward;
		--currentFrame;
	}
	for(int i=0;i<10;++i)
		printf("%d %d %d\n",i,state[i].beginIndex,state[i].endIndex);
	state[index] = Segment(0, endIndex, true);
	myTemp.setStateInfo(state);
	return minCost2;
}