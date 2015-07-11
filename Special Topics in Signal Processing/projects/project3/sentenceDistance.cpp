#include "sentenceDistance.h"
#include "util.h"
#include "myMatrix.h"
#include <assert.h>
#define MAXVALUE 10000
#define MAXMATRIXROW 100
#define MAXMATRIXCOL 100
sentenceDistance::sentenceDistance(void)
{
	this->ptr=new_matrix2 (MAXMATRIXROW+1,MAXMATRIXCOL+1,20,20);
	errors.deletion=0;
	errors.insertion=0;
	errors.substitution=0;
}


sentenceDistance::~sentenceDistance(void)
{
}

sentenceDistance::ErrorNumType sentenceDistance::computeTheErrors( int **ptr,int max1,int max2 )
{		
	bool ** best = new bool * [max1];
	for(int i = 0;i<max1;i++){
		best[i] = new bool[max2];
	}
	best[max1-1][max2-1]=true;
	for(int i = max1-1;i>0;i--)
	{
		for(int j = max2-1;j>0;j--){
			if(best[i][j]==true){
				if(ptr[i][j-1]+1==ptr[i][j])
				{
					best[i][j-1]=true;
					errors.deletion++;
				}
				else if(ptr[i-1][j]+1==ptr[i][j])
				{
					best[i-1][j]=true;
					errors.insertion++;
				}
				else if (ptr[i-1][j-1]+1==ptr[i][j])
				{
					errors.substitution++;
					best[i-1][j-1]=true;
				}else
				{
					best[i-1][j-1]=true;
				}				
			}
		}
	}
	return errors;
}

int sentenceDistance::computeSentenceDistanceFixed( std::vector<std::string> &left,std::vector<std::string> &top,int maxDistanceLimit )
{
	int max1 = left.size();
	int max2 = top.size();
	int temp = 0;
	int d = 0;
	for(int i = 1;i < max1+1;i++)
	{
		for(int j = 1;j< max2+1;j++)
		{
			if(ptr[i-1][j]==MAXVALUE&&ptr[i][j-1]==MAXVALUE&&ptr[i-1][j-1]==MAXVALUE)
				ptr[i][j]=MAXVALUE;
			else{
				temp=min(ptr[i-1][j],ptr[i][j-1]);
				if(left[i-1]==top[j-1])
					d=0;
				else
					d=1;
				ptr[i][j] = min(temp+1,d+ptr[i-1][j-1]);
				if(ptr[i][j] > maxDistanceLimit)
					ptr[i][j]=MAXVALUE;
			}				
		}
	}
	this->computeTheErrors(ptr,max1+1,max2+1);
	//print(ptr,max1+1,max2+1,true,best_solution(ptr,max1+1,max2+1));
	int mindis = ptr[max1][max2];
	return mindis;	
}
int sentenceDistance::computeSentenceDistanceBeamSearch( std::vector<std::string> &left,std::vector<std::string> &top,int maxDistanceLimit )
{
	int max1 = left.size();
	int max2 = top.size();
	int mindis=0;
	for(int i = 1;i < max1+1;i++)
	{
		int mindis = ptr[i][0];
		for(int j = 1;j< max2+1;j++)
		{
			if(ptr[i-1][j]==MAXVALUE&&ptr[i][j-1]==MAXVALUE&&ptr[i-1][j-1]==MAXVALUE)
			{
				ptr[i][j]=MAXVALUE;
			}
			else{
				int temp = min(ptr[i-1][j],ptr[i][j-1]);
				int d = 0;
				if(left[i-1]==top[j-1])
				{
					d=0;
				}
				else
					d=1;
				ptr[i][j] = min(temp+1,d+ptr[i-1][j-1]);
				if(ptr[i][j] < mindis)
					mindis = ptr[i][j];
			}

		}
		for(int j = 0;j <max2+1;j++)
		{
			if(ptr[i][j]>mindis+maxDistanceLimit)
				ptr[i][j]=MAXVALUE;
		}
	}
	this->computeTheErrors(ptr,max1+1,max2+1);
	if(printMatrixOnScreen)
	{
		print(ptr,max1+1,max2+1,true,best_solution(ptr,max1+1,max2+1));
		print(ptr,max1+1,max2+1,false,best_solution(ptr,max1+1,max2+1));
	}
	int mindistance = ptr[max1][max2];
	return mindistance;
}

void sentenceDistance::reset()
{
	this->errors.deletion=0;
	this->errors.insertion=0;
	this->errors.substitution=0;
}

sentenceDistance::ErrorNum sentenceDistance::getAllErrorsFromTwoPassages( std::vector<std::vector<std::string>> correctLeft,std::vector<std::vector<std::string>> comparedTop,int maxDistanceLimit,bool useFix )
{
	ErrorNum result;
	//this->reset();
	assert(correctLeft.size()==comparedTop.size());
	if(useFix)
		for(int i=0;i<correctLeft.size();i++)
			this->computeSentenceDistanceFixed(correctLeft[i],comparedTop[i],maxDistanceLimit);
	else
	{
		for(int i=0;i<correctLeft.size();i++)
			this->computeSentenceDistanceBeamSearch(correctLeft[i],comparedTop[i],maxDistanceLimit);
	}
	result.insertion=errors.insertion;
	result.deletion=errors.deletion;
	result.substitution=errors.substitution;
	this->reset();
	return result;
}
