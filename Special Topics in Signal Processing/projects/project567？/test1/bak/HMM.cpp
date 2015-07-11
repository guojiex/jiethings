#include "HMM.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <algorithm>
#include <minmax.h>
#include <time.h> 
#define PI 3.1415
HMM::HMM( DTW input,int st):states(st)
{
	//this->states=5;
	this->templates=input.templates;
	printf("\nHMM start------\nread templates from DTW instances of sample:%d frames:%d \n",input.getTotalInstancesOfSample(),input.getTotalFrames());
	int a[]={0,1,2,9,10};
	this->initScoreMatrix(10,250);
	this->templatesArray=std::vector<int>(a,a+5);
	this->initSegment();
	//this->SegmentalK_means(5);
}
HMM::HMM_result HMM::getBestCostDTWCorrectRateFromSingleTemplate( int dataIndexLeft, int dataIndexRight,int left,int right )
{
	std::string sampleName[]={"zero","one","two","three","four","five","six",
		"seven","eight","nine"};
	printf("\nStart HMM------------\n");
	int correct=0;
	int errorNumber=0;
	HMM::HMM_result result;
	result.maxIndex=-1;
	result.maxRes=-1;
	double rate=0;
	time_t start ,end ;  
	double cost;  
	time(&start);  	

	for(int j=dataIndexLeft;j<=dataIndexRight;j++)
	{
		for(int i=left;i<=right;i++)
		{			
			result=getBestScoreDTWForOne(templates[j][i]);
			printf("res:%.3lf %d %d data:%s reg:%s\n",result.maxRes,j,i,sampleName[j].c_str(),sampleName[result.maxIndex].c_str());			
			if(result.maxIndex==j)
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
	printf("End of HMM---------------\n");
	return result;
}
HMM::HMM_result HMM::getBestScoreDTWForOne( std::vector<std::vector<double>> &data )
{
	HMM::HMM_result result;
	result.maxIndex=-1;
	result.maxRes=-1;
	double temp=0;
	for(int i=0;i<this->templates.size();i++)
	{
		temp=DTWforScore(data,i);
		if(i==0)
		{
			result.maxIndex=0;
			result.maxRes=temp;
		}else if(temp>result.maxRes)
		{
			result.maxIndex=i;
			result.maxRes=temp;
		}
	}	
	//printf("res:%f ",result.minRes);
	return result;
}

double HMM::DTWforScore( std::vector<std::vector<double>> &data,const int templateWord )
{

	for(int j=0;j<data.size();j++)
	{
		for(int i=0;i<states;++i)
		{			
			scoreMatrix[i][j]=d(data[j],templateWord,i);			
			if(j==0)
			{
				scoreMatrix[i][j]+=Tprobability[templateWord][0][i];
			}
			else 
			{			
				if(i==0)
				{
					scoreMatrix[i][j]+=scoreMatrix[i][j-1]+Tprobability[templateWord][i+1][i];
				}
				else if(i==1)
				{
					scoreMatrix[i][j]+=max(scoreMatrix[i][j-1]+Tprobability[templateWord][i+1][i],scoreMatrix[i-1][j-1]+Tprobability[templateWord][i-1+1][i]);
				}else
				 {
				 scoreMatrix[i][j]+=max(scoreMatrix[i-2][j-1]+Tprobability[templateWord][i-2+1][i],max(scoreMatrix[i][j-1]+Tprobability[templateWord][i+1][i],scoreMatrix[i-1][j-1]+Tprobability[templateWord][i-1+1][i]));
				 }
			}
			//printf("%.3lf ",scoreMatrix[i][j]);
			//system("pause");
		}
	}
	return scoreMatrix[states-1][data.size()-1];
}
void HMM::initProbabilities()
{
	Tprobability.resize(templates.size());
	for(int j=0;j<templates.size();++j)
	{
		Tprobability[j].resize(states+1);//加上自设的初始化节点，有states+1的初始可能
		for(int i=0;i<states+1;i++)
			Tprobability[j][i].resize(states);
	}	
	//只有0到其他states
	//[i][j]i比j size大1，因为有从0到别的状态
	int temp=0;
	for(int k=0;k<templates.size();++k)
	{
		for(int i=0;i<states+1;i++)
		{
			for(int j=0;j<states;j++)
			{
				if(i==0)
				{
					if(j!=0)
						Tprobability[k][i][j]=log(templatesArray.size());
					else
						Tprobability[k][i][j]=0;
				}else if(i-1==j)
				{
					Tprobability[k][i][j]=log(framesIneachSegment[k][i-1]-templatesArray.size());
				}else if(j==i-1+1)
				{
					Tprobability[k][i][j]=log(templatesArray.size());
				}else
				{
					Tprobability[k][i][j]=0;
				}
			}
		}
	}
	for(int k=0;k<templates.size();++k)
	{
		for(int i=0;i<states+1;i++)
		{
			for(int j=0;j<states;j++)
			{
				printf("%.3lf ",Tprobability[k][i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}
	system("pause");
}
void HMM::initScoreMatrix(const int row,const int col)
{
	this->scoreMatrix.resize(row);
	for(int i=0;i<row;++i)
		this->scoreMatrix[i].resize(col);
}
void HMM::initSegmentSingleTemplate( const std::vector<std::vector<double>> &aTemplate,startOfASegment &seg)
{
	seg.resize(states);
	//第[templateI][templateJ]个template的means有states个状态，每个状态是个39维的向量
	//means[templateI][templateJ].resize(states);
	/*for(int i=0;i<states;i++)
	mean[i].resize(39);*/
	int size=aTemplate.size()/states;
	for(int i=0;i<states;i++)
	{
		seg[i]=i*size;		
		//printf("%d ",seg[i]);
	}
}
void HMM::initCovar()
{	
	covariances.resize(10);
	for(int i=0;i<10;++i)
	{
		covariances[i].resize(states);	
		for(int j=0;j<states;++j)
		{
			covariances[i][j].resize(39);
			for(int dimension=0;dimension<covariances[i][j].size();++dimension)
				covariances[i][j][dimension]=0;
		}
	}	
}
void HMM::initSegment()
{
	int size=templatesArray.size();
	int size2=templates.size();
	segments.resize(size2);
	initCovar();
	initModelVector();
	initFramesIneachSegment();
	for (int i=0;i<size2;i++)
	{
		segments[i].resize(size);	
		for(int j=0;j<size;j++)
		{			
			initSegmentSingleTemplate(templates[i][templatesArray[j]],segments[i][j]);
		}
	}
	this->getModelVector();
	this->getCovarianceFromModelVector();
}
void HMM::SegmentalK_means(int times)
{
	int size=templatesArray.size();
	int size2=templates.size();
	double lasttimelast=-10000;
	for(int times2=0;times2<times;times2++)
	{
		for (int i=0;i<size2;i++)
		{	
			for(int j=0;j<size;j++)
			{			
				SegmentalK_meansOne(templates[i][templatesArray[j]],i,segments[i][j]);			
			}	
		}
		printf("start mean calu\n");
		initCovar();
		initModelVector();
		initFramesIneachSegment();
		this->getModelVector();
		this->getCovarianceFromModelVector();
		if(lasttimelast!=modelVector[templates.size()-1][states-1][39-1])
			lasttimelast=modelVector[templates.size()-1][states-1][39-1];
		else
		{
			printf("running k means for %d times\n",times2);
			break;
		}		
	}	
}
void HMM::getModelVector()
{	
	int size=templates.size();
	int size2=templatesArray.size();	
	int size3=0;
	int transIndex=0;
	int left=0;
	int right=0;	
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size2;j++)
		{
			transIndex=templatesArray[j];
			size3=templates[i][transIndex].size();//样本帧数
			for(int k=0;k<states;k++)
			{
				if(k!=states-1)//除非是最后一段，不然右边（不包括right）就是下一段的开始地址
				{
					right=segments[i][j][k+1];					
				}
				else//最后一段的结束在帧数的size
				{
					right=size3;
				}				
				//printf("lr %d %d\n",left,right);
				if(right==0)
					continue;
				assert(right-left>=0);	
				framesIneachSegment[i][k]+=right-left;
				for(int index=left;index<right;index++)
				{		
					//printf("%d %d\n",index,templates[i][transIndex].size());
					for(int dimension=0;dimension<39;++dimension)
					{
						//printf("%.3lf ",templates[i][transIndex][index][dimension]);
						modelVector[i][k][dimension]+=templates[i][transIndex][index][dimension];
					}
					//printf("\n");
				}				
				left=right;
				//printf("%d %d \n",left,right);
			}
			left=0;
		}
	}
	for(int k=0;k<size;k++)
	{
		for(int i=0;i<states;++i)
		{
			for(int j=0;j<39;++j)
			{				
				modelVector[k][i][j]=modelVector[k][i][j]/framesIneachSegment[k][i];
				//printf("%.3lf ",modelVector[k][i][j]);
			}
			//printf("\n");
		}	
	}	
}
double HMM::individualVarianceTermS2LeftPart( const int templateWord,const int jthState )
{
	double result=0;
	for(int l=0;l<39;++l)
	{
		result+=log(2*PI*covariances[templateWord][jthState][l]);
	}
	return result;
}
double HMM::d( const std::vector<double> &x,const int templateWord,const int jthState )
{
	assert(x.size()==39&&jthState<states);
	double leftPart=0.5*individualVarianceTermS2LeftPart(templateWord,jthState);
	double rightPart=0;
	for(int i=0;i<39;++i)
	{
		rightPart+=pow(x[i]-modelVector[templateWord][jthState][i],2)/covariances[templateWord][jthState][i];
	}
	return leftPart+0.5*rightPart;
}
void HMM::getNewSeg(const std::vector<std::vector<double>> &atemplate,const int templateWord,startOfASegment &newSeg)
{
	int i=0;
	int j=0;
	while(i<states)
	{
		if(j==atemplate.size()-1)
			break;
		if(i==states-1)
		{
			newSeg[i]=j;
			break;
		}
		if(i==states-2)
		{
			if(scoreMatrix[i+1][j+1]>scoreMatrix[i][j+1])
			{
				++i;
				newSeg[i]=j+1;
			}else
			{
				++j;
			}
		}
	}
}
void HMM::SegmentalK_meansOne( const std::vector<std::vector<double>> &atemplate,const int templateWord,startOfASegment &newSeg )
{
	assert(newSeg.size()==5);
	for(int i=0;i<states;++i)
		newSeg[i]=0;
	int frameNumber=atemplate.size();
	/*for(int i=0;i<states;++i)
	{
		for(int j=0;j<frameNumber;++j)
		{
			scoreMatrix[i][j]=d(atemplate[j],templateWord,i);
			if(j!=0)
			{			
				if(i==0)
				{
					scoreMatrix[i][j]+=scoreMatrix[i][j-1];
				}
				else if(i==1)
				{		
					scoreMatrix[i][j]+=max(scoreMatrix[i][j-1],scoreMatrix[i-1][j-1]);
				}else
				{
					scoreMatrix[i][j]+=max(scoreMatrix[i-2][j-1],max(scoreMatrix[i][j-1],scoreMatrix[i-1][j-1]));
				}
			}
		}
	}	
	getNewSeg(atemplate,templateWord,newSeg);*/
	int left=0;
	int right=left+1;
	double leftD=0;
	double rightD=0;
	for(int j=0;j<frameNumber;++j)
	{
		if(left==states-1)
			break;
		leftD=d(atemplate[j],templateWord,left);
		rightD=d(atemplate[j],templateWord,right);
		if(leftD <= rightD)
			++newSeg[right];
		else
		{
			/*if(newSeg[right]==0)
			{
				printf("newseg %d %d",left,right);
				system("pause");
			}*/
			if(newSeg[right]-newSeg[left]==0)
			{
				++newSeg[right];
			}
			++left;
			++right;
			if(right<states)
			{
				newSeg[right]=newSeg[left];
			}
			else
			{			
				break;
			}
		}
	}
	for (int i=0;i<states;++i)
	{
		printf("%d ",newSeg[i]);
	}
	printf("\n");
}
void HMM::getCovarianceFromModelVector()//在modelvector初始化之后执行，framesIneachSegment已经被执行过
{	
	int size=templates.size();
	int size2=templatesArray.size();	
	int size3=0;
	int transIndex=0;
	int left=0;
	int right=0;	
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size2;j++)
		{
			transIndex=templatesArray[j];
			size3=templates[i][transIndex].size();//样本帧数
			for(int k=0;k<states;k++)
			{
				if(k!=states-1)//除非是最后一段，不然右边（不包括right）就是下一段的开始地址
					right=segments[i][j][k+1];
				else//最后一段的结束在帧数的size
				{
					right=size3;
				}
				if(right==0)
					continue;
				assert(right-left>=0);	
				//framesIneachSegment[k]+=right-left;
				for(int index=left;index<right;index++)
				{		
					for(int dimension=0;dimension<39;++dimension)
					{
						//modelVector[k][dimension]+=templates[i][transIndex][index][dimension];
						covariances[i][k][dimension]+=pow(templates[i][transIndex][index][dimension]-modelVector[i][k][dimension],2);
					}
				}				
				left=right;
			}
			left=0;
		}
	}
	for(int k=0;k<size;++k)
	{
		for(int i=0;i<states;++i)
		{
			for(int j=0;j<39;++j)
			{			
				covariances[k][i][j]=covariances[k][i][j]/framesIneachSegment[k][i];
				//printf("%.3lf ",covariances[k][i][j]);
			}
			//printf("\n\n");
		}	
	}	
}
//void HMM::getMeanFromSegment()
//{
//	int size=templates.size();
//	int size2=templatesArray.size();
//	int size3=0;
//	int transIndex=0;
//	int left=0;
//	int right=0;
//	int numbers[5];
//	for(int i=0;i<5;i++)
//		numbers[i]=0;
//	//i用0-9来index,j用templatesArray中的数值来作
//	for(int i=0;i<size;i++)
//	{
//		for(int j=0;j<size2;j++)
//		{
//			transIndex=templatesArray[j];
//			size3=templates[i][transIndex].size();//样本帧数
//			for(int k=0;k<states;k++)
//			{
//				if(k!=states-1)//除非是最后一段，不然右边（不包括right）就是下一段的开始地址
//					right=segments[i][j][k+1];
//				else//最后一段的结束在帧数的size
//				{
//					right=size3;
//				}
//				printf("%d %d %d;",left,right,size3);
//				assert(right-left>0);
//				numbers[k]+=right-left;
//				for(int index=left;index<right;index++)
//				{		
//					for(int dimension=0;dimension<39;dimension++)
//					{
//						means[k][dimension]+=templates[i][transIndex][index][dimension];
//					}
//				}				
//				left=right;
//			}
//			left=0;
//			printf("\n");
//		}
//	}
//	printf("number of frames in state:\n");
//	for(int i=0;i<5;i++)
//		printf("%d ",numbers[i]);
//	printf("\n");
//	for(int k=0;k<states;k++)
//	{
//		for (int dimension=0;dimension<39;dimension++)
//		{
//			//printf("%lf ",means[k][dimension]);
//			means[k][dimension]=means[k][dimension]/numbers[k];
//			//printf("%lf ",means[k][dimension]);
//		}
//		//printf("\n");
//	}
//}
HMM::~HMM(void)
{
}
