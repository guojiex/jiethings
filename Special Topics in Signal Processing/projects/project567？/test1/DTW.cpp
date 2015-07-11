#include "DTW.h"
#include <stdio.h>
#include <vector>
#include "mathUtil.h"
#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <minmax.h>
#include <time.h>  
DTW::DTW(void)
{
	this->initMatrix(1000,250);
	templates.resize(10);
	for(int i=0;i<templates.size();i++)
		this->getAllInstancesOfTemplate(templates[i],i);
}

double DTW::P(int i,int j)
{
	if(i<0)
		return -1;
	if(j<0)
		return -1;
	return matrixScore[i][j];
}

//************************************
// Method:    初始化DTW矩阵，化成一个row行col列的二维数组
// FullName:  DTW::initMatrix
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: int row
// Parameter: int col
//************************************
void DTW::initMatrix(int row,int col)
{
	this->matrixScore.resize(row);
	for(unsigned int i=0;i<matrixScore.size();i++)
		matrixScore[i].resize(col);	
}
std::vector<std::vector<double>> averageTemplate(std::vector<std::vector<double>> &leftTemplate,std::vector<std::vector<double>> &data)
{
	std::vector<std::vector<double>> newtemplate;
	for(int i=0;i<leftTemplate.size();++i)
		newtemplate.push_back(leftTemplate[i]);
	std::vector<std::vector<double>> matrix;
	matrix.resize(leftTemplate.size());
	for(int i=0;i<leftTemplate.size();++i)
		matrix[i].resize(data.size());
	double mintemp=0;	
	for(int j=0;j<data.size();j++)
	{
		for(int i=0;i<leftTemplate.size();i++)
		{			
			matrix[i][j]=vectorEuclideanDistance(leftTemplate[i],data[j]);
			if(j!=0)
			{			
				if(i==0)
				{
					matrix[i][j]+=matrix[i][j-1];
				}
				else if(i==1)
				{
					matrix[i][j]+=min(matrix[i][j-1],matrix[i-1][j-1]);
				}else
				{
					matrix[i][j]+=min(matrix[i-2][j-1],min(matrix[i][j-1],matrix[i-1][j-1]));
				}
			}
		}
	}
	int frame=0;
	for(int j=0;j<data.size()-1;++j)
	{
		for(int i=0;i<39;i++)
		{
			newtemplate[frame][i]+=data[j][i];
			newtemplate[frame][i]=newtemplate[frame][i]/2;
		}
		if(frame+2<leftTemplate.size())
		{
			if(matrix[frame+2][j+1]<matrix[frame+1][j+1]&&matrix[frame+2][j+1]<matrix[frame][j+1])
			{
				frame+=2;
			}else if(matrix[frame+1][j+1]<matrix[frame+2][j+1]&&matrix[frame+1][j+1]<matrix[frame][j+1])
			{
				frame+=1;
			}
		}else if(frame+1<leftTemplate.size())
		{
			if(matrix[frame+1][j+1]<matrix[frame][j+1])
			{
				frame+=1;
			}
		}
	}
	return newtemplate;
}
void DTW::getAllInstancesOfTemplate(std::vector<DTW_frame> &toadd,int numberOfTemplate)
{
	for(int i=1;i<16;i++)
		toadd.push_back(this->readAdeltaFromFile(
		createTemplatePath(numberOfTemplate,i).append("delta.txt")));
	//std::cout<<toadd.size()<<std::endl;
	//system("pause");
}


double DTW::DTWforCost(std::vector<std::vector<double>> &leftTemplate,std::vector<std::vector<double>> &data)
{
	double mintemp=0;	
	for(int j=0;j<data.size();j++)
	{
		for(int i=0;i<leftTemplate.size();i++)
		{			
			matrixScore[i][j]=vectorEuclideanDistance(leftTemplate[i],data[j]);
			if(j!=0)
			{			
				if(i==0)
				{
					matrixScore[i][j]+=matrixScore[i][j-1];
				}
				else if(i==1)
				{
					matrixScore[i][j]+=min(matrixScore[i][j-1],matrixScore[i-1][j-1]);
				}else
				{
					matrixScore[i][j]+=min(matrixScore[i-2][j-1],min(matrixScore[i][j-1],matrixScore[i-1][j-1]));
				}
			}
		}
	}

	return matrixScore[leftTemplate.size()-1][data.size()-1];
}
void DTW::testMultiTemplateTimeSynchronousDTW(int dataIndexLeft, int dataIndexRight,int left,int right)
{
	int a[]={0,1,2,13,14};	
	int a2[]={0,1,2,3,4};
	size_t count=sizeof(a)/sizeof(int);
	size_t count2=sizeof(a2)/sizeof(int);
	std::vector<std::vector<double>> res;
	/*for(int i=0;i<templates.size();i++)
	{
		res.clear();
		for(int j=1;j<count;++j)
		{
			if(j==1)
				res=averageTemplate(templates[i][a[0]],templates[i][a[1]]);
			else
			{
				res=averageTemplate(res,templates[i][a[j]]);
			}
		}
		templates[i].push_back(res);
	}*/
	std::vector<int> templateArray(a2,a2+count2);
	std::string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	printf("\nStart DTW------------\n");
	int correct=0;
	int errorNumber=0;
	DTW::DTW_result result;
	DTW::DTW_result best;
	double rate=0;
	time_t start ,end ;  
	double cost;  
	time(&start); 
	for(int j=dataIndexLeft;j<=dataIndexRight;j++)
	{
		for(int i2=left;i2<=right;i2++)
		{		
			best.minIndex=-1;
			for(int i=0;i<templateArray.size();i++)
			{
				result=timeSynchronousDTWforCost(templateArray[i],templates[j][i2]);
				if(best.minIndex==-1||best.minRes>result.minRes)
					best=result;				
			}
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
void DTW::testMultiTemplateDTW( int dataIndexLeft, int dataIndexRight,int left,int right )
{
	int a[]={0,1,2,13,14};
	size_t count=sizeof(a)/sizeof(int);
	std::vector<int> templateArray(a,a+count);
	std::string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	printf("\nStart DTW------------\n");
	int correct=0;
	int errorNumber=0;
	DTW::DTW_result result;
	DTW::DTW_result best;
	double rate=0;
	time_t start ,end ;  
	double cost;  
	time(&start); 	
	for(int j=dataIndexLeft;j<=dataIndexRight;j++)
	{
		for(int i2=left;i2<=right;i2++)
		{		
			best.minIndex=-1;
			for(int i=0;i<templateArray.size();i++)
			{
				result=getBestCostDTWForOne(templateArray[i],templates[j][i2]);
				if (best.minIndex==-1)
				{
					best=result;
				}else if(best.minRes>result.minRes)
				{
					best=result;
				}				
			}
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

DTW::DTW_result DTW::getBestCostDTWCorrectRateFromSingleTemplate( int templateIndex, int dataIndexLeft, int dataIndexRight,int left,int right )
{
	std::string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	printf("\nStart DTW------------\n");
	int correct=0;
	int errorNumber=0;
	DTW::DTW_result result;
	result.minIndex=-1;
	result.minRes=-1;
	double rate=0;
	time_t start ,end ;  
	double cost;  
	time(&start);  	

	for(int j=dataIndexLeft;j<=dataIndexRight;j++)
	{
		for(int i=left;i<=right;i++)
		{			
			result=getBestCostDTWForOne(templateIndex,templates[j][i]);
			printf("%d %d data:%s reg:%s\n",j,i,sampleName[j].c_str(),sampleName[result.minIndex].c_str());			
			if(result.minIndex==j)
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
	printf("using the %dth template\n",templateIndex);
	printf("time spend %.3lf seconds.\n",cost); 
	printf("correct:%d error:%d correct rate:%f \n",correct,errorNumber,rate);
	printf("End of DTW---------------\n");
	return result;
}
DTW::DTW_result DTW::getBestCostDTWForOne( int templateIndex,DTW_frame &data )
{	
	DTW::DTW_result result;
	result.minIndex=-1;
	result.minRes=-1;
	double temp=0;
	for(int i=0;i<this->templates.size();i++)
	{
		temp=DTWforCost(templates[i][templateIndex],data);
		if(i==0)
		{
			result.minIndex=0;
			result.minRes=temp;
		}else if(temp<result.minRes)
		{
			result.minIndex=i;
			result.minRes=temp;
		}
	}	
	//printf("res:%f ",result.minRes);
	return result;
}
DTW::DTW_result DTW::timeSynchronousDTWPruning(const double threshold ,int templatesIndex,std::vector<std::vector<double>> &data )
{
	DTW_result result;
	int line=0;
	result.minIndex=0;
	result.minRes=0;
	int jsize=data.size();
	int templatesSize=templates.size();
	int rowSize=0;
	double colMin=50000;
	for(int j=0;j<jsize;j++)
	{
		for(int i2=0;i2<templatesSize;i2++)
		{	
			for(int i=0;i<templates[i2][templatesIndex].size();i++)
			{
				if(j==0)
				{
					matrixScore[line][j]=vectorEuclideanDistance(templates[i2][templatesIndex][i],data[j]);
					if(matrixScore[line][j]<colMin)
						colMin=matrixScore[line][j];
				}
				else if(j!=0)
				{
					if(i==0)
					{
						matrixScore[line][j]=matrixScore[line][j-1]+vectorEuclideanDistance(templates[i2][templatesIndex][i],data[j]);
					}
					else if(i==1)
					{
						matrixScore[line][j]=min(matrixScore[line][j-1],matrixScore[line-1][j-1])+vectorEuclideanDistance(templates[i2][templatesIndex][i],data[j]);
					}else
					{
						matrixScore[line][j]=min(matrixScore[line-2][j-1],min(matrixScore[line][j-1],matrixScore[line-1][j-1]))+vectorEuclideanDistance(templates[i2][templatesIndex][i],data[j]);
					}
					if(matrixScore[line][j]<colMin)
						colMin=matrixScore[line][j];
				}					
				++line;
			}
		}		
		if(j==0)
			rowSize=line;
		//printf("col %lf\n",colMin);
		for(int i=0;i<rowSize;++i)
		{
			if(matrixScore[i][j]>colMin+threshold)
				matrixScore[i][j]=10000;
		}
		colMin=50000;
		line=0;
	}
	line=0;
	for(int i2=0;i2<templatesSize;i2++)
	{	
		line+=templates[i2][templatesIndex].size();
		if(i2==0)
		{
			result.minRes=matrixScore[line-1][data.size()-1];
			result.minIndex=i2;
		}
		else if(matrixScore[line-1][data.size()-1]<result.minRes)
		{
			result.minRes=matrixScore[line-1][data.size()-1];
			result.minIndex=i2;
		}		
	}
	return result;
}

DTW::DTW_result DTW::timeSynchronousDTWforCost( int templatesIndex,std::vector<std::vector<double>> &data )
{
	DTW_result result;
	int line=0;
	result.minIndex=0;
	result.minRes=0;
	int jsize=data.size();
	int templatesSize=templates.size();
	for(int j=0;j<jsize;j++)
	{
		for(int i2=0;i2<templatesSize;i2++)
		{	
			for(int i=0;i<templates[i2][templatesIndex].size();i++)
			{
				matrixScore[line][j]=vectorEuclideanDistance(templates[i2][templatesIndex][i],data[j]);
				if(j!=0)
				{
					if(i==0)
					{
						matrixScore[line][j]+=matrixScore[line][j-1];
					}
					else if(i==1)
					{
						matrixScore[line][j]+=min(matrixScore[line][j-1],matrixScore[line-1][j-1]);
					}else
					{
						matrixScore[line][j]+=min(matrixScore[line-2][j-1],min(matrixScore[line][j-1],matrixScore[line-1][j-1]));
					}
				}	
				if(j==jsize-1&&i==templates[i2][templatesIndex].size()-1&&i2==0)
				{
					result.minIndex=i2;
					result.minRes=matrixScore[line][j];
				}
				else if(j==jsize-1&&i==templates[i2][templatesIndex].size()-1&&result.minRes>matrixScore[line][j])
				{
					result.minIndex=i2;
					result.minRes=matrixScore[line][j];
				}
				++line;
			}
		}		
		line=0;
	}

	/*for(int i2=0;i2<templates.size();i2++)
	{	
	for(int i=0;i<templates[i2][templatesIndex].size();i++)
	{
	for(int j=0;j<data.size();j++)
	{
	if((i<=1||i-templates[i2][templatesIndex].size())<=1&&(j<=10||data.size()-j<=1))
	printf("%lf ",matrixScore[line][j]);
	}
	++line;
	printf("\n");
	}
	}*/
	/*for (int i = 0; i < templates.size(); i++)
	{
	templates[i][templatesIndex].erase(templates[i][templatesIndex].begin());
	}	
	data.erase(data.begin());*/
	//printf("res:%lf ",result.minRes);
	return result;
}
DTW::DTW_result DTW::testCostTimeSynchronousDTWCorrectRatePrunning(double thre, int dataIndexLeft, int dataIndexRight,int left,int right )
{
	int a[]={0};
	size_t count=sizeof(a)/sizeof(int);
	std::vector<int> templateArray(a,a+count);
	std::string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	printf("\nStart DTW------------\n");
	int correct=0;
	int errorNumber=0;
	DTW::DTW_result result;
	DTW::DTW_result best;
	double rate=0;
	time_t start ,end ;  
	double cost;  
	time(&start); 
	for(int j=dataIndexLeft;j<=dataIndexRight;j++)
	{
		for(int i2=left;i2<=right;i2++)
		{		
			best.minIndex=-1;
			for(int i=0;i<templateArray.size();i++)
			{
				result=timeSynchronousDTWPruning(thre,templateArray[i],templates[j][i2]);
				if(best.minIndex==-1||best.minRes>result.minRes)
					best=result;				
			}
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
	return result;
}
DTW::DTW_result DTW::getCostTimeSynchronousDTWCorrectRateFromSingleTemplate( int templateIndex, int dataIndexLeft, int dataIndexRight,int left,int right )
{
	std::string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	printf("\nstart time synchronous DTW------------\n");
	int correct=0;
	int errorNumber=0;
	DTW_result result;
	result.minIndex=-1;
	result.minRes=-1;
	double rate=0;
	time_t start ,end ;  
	double cost;  
	time(&start);  	
	for(int j=dataIndexLeft;j<=dataIndexRight;j++)
	{
		for(int i=left;i<=right;i++)
		{
			result=timeSynchronousDTWforCost(templateIndex,templates[j][i]);

			printf("%d %d data:%s reg:%s\n",j,i,sampleName[j].c_str(),sampleName[result.minIndex].c_str());			
			if(result.minIndex==j)
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
	printf("using the %dth template\n",templateIndex);
	printf("time spend %.3lf seconds.\n",cost); 
	printf("correct:%d error:%d correct rate:%f \n",correct,errorNumber,rate);
	printf("End of time synchronous DTW---------------\n");
	return result;
}



std::vector<std::vector<double>> DTW::readAdeltaFromFile(const std::string fileName)
{
	std::vector<std::vector<double>> res;
	/*std::ifstream inputFile;
	inputFile.open(fileName);
	std::vector<double> oneline;
	oneline.resize(39);
	while (!inputFile.eof())
	{
	for(int i=0;i<39;i++)
	inputFile>>oneline[i];
	res.push_back(oneline);
	}	
	inputFile.close();*/
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
			res.push_back(oneline);
			index=0;
		}
	}	
	fclose(fq);
	printf("read delta file %s \n",fileName.c_str());
	//printfAVectorVector(res);
	return res;
}
DTW::~DTW(void)
{
	this->matrixScore.clear();
}
