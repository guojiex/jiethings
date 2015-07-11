#include "stdafx.h"
#include "lextreeDTW.h"
#include <bitset>
#include <minmax.h>
#include <limits>
lextreeDTW::~lextreeDTW(void)
{
}
void lextreeDTW::drawCostMatrix2(const std::string &input)
{
	printf("   ");
	for(auto item:input)
	{
		printf("%3c",item);
	}
	printf("\n");
	for(int i=0;i<templateLength;++i)
	{
		for(int j=0;j<input.size();++j)
		{
			if(j==0)
				printf("%3c",templateLeft[i]);
			if(costMatrix2[i][j]==INFINITEVALUE||costMatrix2[i][j]==-1)
			{
				printf("   ");
			}
			else 
				printf("%3d",costMatrix2[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
void lextreeDTW::drawCostMatrix(const std::string input)
{
	printf("   ");
	for(auto item:input)
	{
		printf("%3c",item);
	}
	printf("\n");
	for(int i=0;i<templateLength;++i)
	{
		for(int j=0;j<input.size();++j)
		{
			if(j==0)
				printf("%3c",templateLeft[i]);
			if(costMatrix[i][j]==INFINITEVALUE||costMatrix[i][j]==-1)
			{
				printf("   ");
			}
			else 
				printf("%3d",costMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
std::string lextreeDTW::findBestStringAfterDTW()
{
	template_size_type minIndex=0;
	unsigned int minValue=0;
	unsigned int temp=0;
	for(unsigned int i=1;i<templateLength;++i)//i==0 is '*',will not be the end of a word
	{
		if(endTemplateLeft[i]==0)//if it is not end of a word
			continue;
		temp=costMatrix[i][inputSize-1];
		if(temp==INFINITEVALUE||temp==-1)
			continue;
		else if(minIndex==0||minValue>temp)
		{
			minValue=temp;
			minIndex=i;
		}
	}
	std::string res="";
	while (minIndex!=0)
	{
		//std::cout<<parentIndexOfCurrent[minIndex]<<" "<<minIndex<<" "<<templateLeft[minIndex]<<std::endl;
		res+=templateLeft[minIndex];
		minIndex=parentIndexOfCurrent[minIndex];
	}

	reverse(res.begin(),res.end());
	return res;
}
//bool lextreeDTW::DTW( const std::string input,const int &beamwidth)
//{
//	assert(input.length()<=MAXINPUTDATALENGTH);
//	this->resetCostMatrix();
//
//	costMatrix[0][0]=0;
//	int minValueOfCol=0;
//	for(int i=1;i<templateLength;++i)
//	{
//		costMatrix[i][0]=costMatrix[indexTemplateLeft[i]][0]+1;
//		if(costMatrix[i][0]<minValueOfCol)
//			minValueOfCol=costMatrix[i][0];
//	}
//
//	std::vector<DTW_valid_index_vector_type> validLeft;
//	std::vector<DTW_valid_index_vector_type> validRight;
//
//	for(int i=1;i<templateLength;++i)
//		if(costMatrix[i][0]-minValueOfCol<=beamwidth)
//			validLeft.push_back(i);
//
//	unsigned short inputSize=input.length();
//	int minOfThree=INFINITEVALUE;
//	int validSize;//=validLeft.size();
//	for(unsigned short j=1;j<inputSize;++j)
//	{
//		if(validLeft.empty()&&j!=inputSize-1)
//			return false;
//		validSize=validLeft.size();
//		validRight.clear();
//		for(int i=0;i<validSize;++i)
//	}
//}

void lextreeDTW::resetCostMatrix()
{
	this->costMatrix.clear();
	this->costMatrix.resize(templateLeft.size());
	int size=templateLeft.size();
	//templateLength=size;
	for(int i=0;i<size;++i)
		costMatrix[i].resize(MAXINPUTDATALENGTH,-1);
}

void lextreeDTW::resetMatrixForSegment()
{
	this->costMatrix2.clear();
	this->backPointerMatrix.clear();
	this->costMatrix2.resize(templateLeft.size());
	this->backPointerMatrix.resize(templateLeft.size());
	int size=templateLeft.size();
	for(int i=0;i<size;++i)
	{
		costMatrix2[i].resize(MAXSEGMENTLENGTH,-1);
		backPointerMatrix[i].resize(MAXSEGMENTLENGTH,0);
	}
}

std::string lextreeDTW::getBestCloseForOne( const std::string &input,const unsigned int &beamwidth )
{
	std::string data='*'+input;
	bool haveResult=this->DTW(data,beamwidth);
	if(isPrint)
		drawCostMatrix(data);
	if(!haveResult)
		return "not_valid_for_beam";

	return this->findBestStringAfterDTW();
	//return "not support";
}



void lextreeDTW::getBestCloseForSentencesOutputToFIle(const std::vector<std::vector<std::string>> &inputs,const unsigned int &beamwidth,const std::string &outputFile,const bool &needSpace)
{
	FILE *pfOutput;
	fopen_s(&pfOutput,outputFile.c_str(), "wt");
	if (pfOutput == NULL)
	{
		LOG(INFO) << "open outputfile:"<<outputFile<<" error\n";
	}
	else
		LOG(INFO) << "open outputfile:"<<outputFile<<" success\n";

	LOG(INFO) << "Computing and writing file!\n";

	unsigned int vectorSize=inputs.size();
	unsigned int sentenceSize;
	std::string result;
	clock_t start, finish;
	start = clock();

	double duration = 0;
	for(unsigned int i=0;i<vectorSize;++i)
	{
		sentenceSize=inputs[i].size();
		for(unsigned int j=0;j<sentenceSize;++j)
		{
			result=this->getBestCloseForOne(inputs[i][j],beamwidth);		
			finish = clock();
			duration=(double)(finish - start) / CLOCKS_PER_SEC;
			printf("%d/%d ori:%s check:%s time:%.3lf\n",j+1,sentenceSize,inputs[i][j].c_str(),result.c_str(),duration);
			if(needSpace)
				fprintf(pfOutput, "%s ",result.c_str());
			else
				fprintf(pfOutput, "%s",result.c_str());
		}	
		fprintf(pfOutput, "\n");
	}
	LOG(INFO) << "Write file finished!\n";
	fclose(pfOutput);
}


std::string lextreeDTW::getBestSegmentForOne( const std::string &input,const unsigned int &beamwidth )
{
	std::string data='*'+input;
	bool haveResult=segmentDTW(data,beamwidth);
	if(isPrint)
		drawCostMatrix2(data);
	if(!haveResult)
		return "not_valid_for_beam";
	data=this->findBestStringAfterSegmentDTW();
	printf("%d %s\n%d %s\n",input.size(),input.c_str(),data.size(),data.c_str());
	return data;
}

bool lextreeDTW::DTW( const std::string &input,const int &beamwidth)
{
	assert(input.length()<=MAXINPUTDATALENGTH);
	assert(input.length()<=(std::numeric_limits<unsigned short>::max)());
	//std::cout<<"limit of unsigned int:"<<(std::numeric_limits<unsigned int>::max)()<<std::endl;

	//this->resetCostMatrix();
	costMatrix[0][0]=0;
	int minValueOfCol=0;
	for(int i=1;i<templateLength;++i)
	{
		costMatrix[i][0]=costMatrix[parentIndexOfCurrent[i]][0]+1;
		if(costMatrix[i][0]<minValueOfCol)
			minValueOfCol=costMatrix[i][0];
	}

	std::bitset<MAXTEMPLATELENGTH> validLeft;
	std::bitset<MAXTEMPLATELENGTH> validCurrent;

	for(template_size_type i=0;i<templateLength;++i)
		if(costMatrix[i][0]-minValueOfCol<=beamwidth)
		{
			validLeft.set(i);
		}else
		{
			costMatrix[i][0]=INFINITEVALUE;
		}

		inputSize=input.length();
		unsigned int fromleft,fromleftdown,fromdown;
		for(unsigned int j=1;j<inputSize;++j)
		{
			if(validLeft.none())
				return false;
			validCurrent.reset();
			minValueOfCol=-1;
			for(template_size_type i=0;i<templateLength;++i)
			{			
				if(i==0)
				{
					if(validLeft.at(i))
					{
						costMatrix[i][j]=costMatrix[i][j-1]+1;
						validCurrent.set(i);
					}
				}else
				{
					fromdown=fromleftdown=fromleft=INFINITEVALUE;
					if(validLeft.at(i))
						fromleft=costMatrix[i][j-1]+1;
					if(validLeft.at(parentIndexOfCurrent[i]))
					{
						fromleftdown=costMatrix[parentIndexOfCurrent[i]][j-1];
						if(templateLeft[i]!=input[j])
							fromleftdown+=1;
					}
					if(validCurrent.at(parentIndexOfCurrent[i]))
					{
						fromdown=costMatrix[parentIndexOfCurrent[i]][j]+1;
					}
					costMatrix[i][j]=min(fromdown,min(fromleft,fromleftdown));
					if(costMatrix[i][j]!=INFINITEVALUE||costMatrix[i][j]!=-1)
						validCurrent.set(i);
				}
				if(minValueOfCol==-1||costMatrix[i][j]<minValueOfCol)
					minValueOfCol=costMatrix[i][j];
				//printf("min:%d \n",minValueOfCol);
			}
			for(template_size_type i=0;i<templateLength;++i)
			{
				if(costMatrix[i][j]>beamwidth+minValueOfCol)
				{
					//printf("costij%d min%d",costMatrix[i][j],minValueOfCol);
					costMatrix[i][j]=INFINITEVALUE;
					validCurrent.reset(i);
				}
			}
			validLeft=validCurrent;
		}
		return true;
}
int lextreeDTW::lastColumnminEndOfWord(const template_size_type &j)
{
	template_size_type minIndex=0;
	unsigned int minValue=0;
	unsigned int temp=0;
	for(unsigned int i=1;i<templateLength;++i)//i==0 is '*',will not be the end of a word
	{
		if(endTemplateLeft[i]==0)//if it is not end of a word
			continue;
		temp=costMatrix2[i][j];
		if(temp==INFINITEVALUE||temp==-1)
			continue;
		else if(minIndex==0||minValue>temp)
		{
			minValue=temp;
			minIndex=i;
		}
	}
	return minIndex;
}

bool lextreeDTW::segmentDTW( const std::string &input,const int &beamwidth )
{
	printf("\n%d \n",input.size());
	assert(input.length()<=MAXSEGMENTLENGTH);
	assert(input.length()<=(std::numeric_limits<unsigned short>::max)());

	costMatrix2[0][0]=0;
	int minValueOfCol=0;
	for(int i=1;i<templateLength;++i)
	{
		costMatrix2[i][0]=costMatrix2[parentIndexOfCurrent[i]][0]+1;
		if(costMatrix2[i][0]<minValueOfCol)
			minValueOfCol=costMatrix2[i][0];
	}
	std::bitset<MAXTEMPLATELENGTH> validLeft;
	std::bitset<MAXTEMPLATELENGTH> validCurrent;
	inputSize=input.length();
	backPointerVector.push_back(backPointer(0,0,"*"));
	for(template_size_type i=0;i<templateLength;++i)
	{
		backPointerMatrix[i][0]=0;
		if(costMatrix2[i][0]-minValueOfCol<=beamwidth)
			validLeft.set(i);
		else
			costMatrix2[i][0]=INFINITEVALUE;
	}
		unsigned int fromleft,fromleftdown,fromdown;
		for(unsigned int j=1;j<inputSize;++j)
		{
			printf("computing char:%4d/%4d\n",j,inputSize);
			if(validLeft.none())
				return false;
			validCurrent.reset();
			for(template_size_type i=0;i<templateLength;++i)
			{		
				if(i==0){
					if(j!=1){
						backPointerMatrix[0][j]=lastColumnminEndOfWord(j-1);
						backPointerVector.push_back(backPointer(j,backPointerMatrix[backPointerMatrix[0][j]][j-1],getAWordFromEnd(backPointerMatrix[0][j])));
						backPointerMatrix[0][j]=backPointerVector.size()-1;
						costMatrix2[i][j]=costMatrix2[lastColumnminEndOfWord(j-1)][j-1];
						minValueOfCol=costMatrix2[i][j];
						validCurrent.set(i);		
					}else// when only after *
					{
						backPointerMatrix[0][j]=lastColumnminEndOfWord(j-1);
						backPointerVector.push_back(backPointer(j,backPointerMatrix[backPointerMatrix[0][j]][j-1],getAWordFromEnd(backPointerMatrix[0][j])));
						backPointerMatrix[0][j]=backPointerVector.size()-1;						
						costMatrix2[i][j]=costMatrix2[lastColumnminEndOfWord(j-1)][j-1];
						backPointerMatrix[0][j]=0;
						minValueOfCol=costMatrix2[i][j];
						validCurrent.set(i);	
					}								
				}else if(parentIndexOfCurrent[i]==0)
				{
					fromdown=fromleftdown=fromleft=INFINITEVALUE;
					if(validLeft.at(i))
					{
						fromleft=costMatrix2[i][j-1]+1;
					}
					if(validLeft.at(0))
					{						
						if(j!=1)
						{
							fromleftdown=costMatrix2[0][j];
							if(templateLeft[i]!=input[j])
								fromleftdown+=1;	
						}
						else
						{
							fromleftdown=costMatrix2[0][j-1];
							if(templateLeft[i]!=input[j])
								fromleftdown+=1;
						}			
					}						
					costMatrix2[i][j]=min(fromleft,fromleftdown);
					///setting backpointers
					if(costMatrix2[i][j]==fromleft)
					{
						backPointerMatrix[i][j]=backPointerMatrix[i][j-1];
					}
					if(costMatrix2[i][j]==fromleftdown)
					{
						backPointerMatrix[i][j]=backPointerMatrix[0][j];
					}
					if(costMatrix2[i][j]!=INFINITEVALUE||costMatrix2[i][j]!=-1)
						validCurrent.set(i);
				}else{
					fromdown=fromleftdown=fromleft=INFINITEVALUE;
					if(validLeft.at(i))
					{
						fromleft=costMatrix2[i][j-1]+1;
					}
					if(validLeft.at(parentIndexOfCurrent[i]))
					{						
						fromleftdown=costMatrix2[parentIndexOfCurrent[i]][j-1];
						if(templateLeft[i]!=input[j])
							fromleftdown+=1;						
					}
					if(validCurrent.at(parentIndexOfCurrent[i]))
					{
						fromdown=costMatrix2[parentIndexOfCurrent[i]][j]+1;
						if(parentIndexOfCurrent[i]==0)
							fromdown=INFINITEVALUE;
					}

					costMatrix2[i][j]=min(fromdown,min(fromleft,fromleftdown));
					///setting backpointers
					if(costMatrix2[i][j]==fromdown)
					{
						//backPointerMatrix[i][j].first=parentIndexOfCurrent[i];
						backPointerMatrix[i][j]=backPointerMatrix[parentIndexOfCurrent[i]][j];
					}else if(costMatrix2[i][j]==fromleft)
					{
						backPointerMatrix[i][j]=backPointerMatrix[i][j-1];
					}else if(costMatrix2[i][j]==fromleftdown)
					{
						backPointerMatrix[i][j]=backPointerMatrix[parentIndexOfCurrent[i]][j-1];
						/*backPointerMatrix[i][j].first=parentIndexOfCurrent[i];
						backPointerMatrix[i][j].second=j-1;*/
					}
					if(costMatrix2[i][j]!=INFINITEVALUE||costMatrix2[i][j]!=-1)
						validCurrent.set(i);
				}
				if(minValueOfCol==-1||costMatrix2[i][j]<minValueOfCol)
					minValueOfCol=costMatrix2[i][j];
			}
			for(template_size_type i=0;i<templateLength;++i)
			{
				if(costMatrix2[i][j]>beamwidth+minValueOfCol)
				{					
					costMatrix2[i][j]=INFINITEVALUE;
					validCurrent.reset(i);
				}
			}
			validLeft=validCurrent;
		}
		return true;
}
void lextreeDTW::outputAllSegmentToOutput(std::vector<std::string> &inputs,const std::string &output,const int &beam)
{
	FILE *pfOutput;
	fopen_s(&pfOutput,output.c_str(), "wt");
	for(int i=0;i<inputs.size();++i)
		fprintf(pfOutput, "%s\n",getBestSegmentForOne(inputs[i],beam));
	fclose(pfOutput);
}
std::string lextreeDTW::getAWordFromEnd(const int &end)
{
	int now=end;
	std::string res="";
	while(now!=0)
	{
		res+=templateLeft[now];
		now=parentIndexOfCurrent[now];
	}
	//reverse(res.begin(),res.end());
	return res;
}
std::string lextreeDTW::findBestStringAfterSegmentDTW()
{
	template_size_type minI=0;
	unsigned int minValue=0;
	unsigned int temp=0;
	for(unsigned int i=1;i<templateLength;++i)//i==0 is '*',will not be the end of a word
	{
		if(endTemplateLeft[i]==0)//if it is not end of a word
			continue;
		temp=costMatrix2[i][inputSize-1];
		if(temp==INFINITEVALUE||temp==-1)
			continue;
		else if(minI==0||minValue>temp)
		{
			minValue=temp;
			minI=i;
		}
	}
	template_size_type minJ=inputSize-1;
	std::string res="";
	std::string temp2;
	temp2=getAWordFromEnd(minI);
	res+=temp2;
	minI=backPointerMatrix[minI][inputSize-1];
	/*for(int i=backPointerVector.size()-1;i>=0;--i)
		printf("%d index:%d par:%d word:%s\n",i,backPointerVector[i].StateTime(),backPointerVector[i].ParentBackpointer(),backPointerVector[i].Word().c_str());*/
	while (minI!=0)
	{
		printf("%d index:%d par:%d word:%s\n",minI,backPointerVector[minI].StateTime(),backPointerVector[minI].ParentBackpointer(),backPointerVector[minI].Word().c_str());
		res+=" "+backPointerVector[minI].Word();
		minI=backPointerVector[minI].ParentBackpointer();
	}
	reverse(res.begin(),res.end());
	return res;
}
