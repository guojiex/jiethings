#include "LevenshteinDistance.h"
#include <unordered_map>
#include <map>
//#define printMatrixOnScreen false
#define MAXVALUE 10000
#define MAXMATRIXROW 1200
#define MAXMATRIXCOL 1200
LevenshteinDistance::LevenshteinDistance(string templateString,string comparedString):templateString(templateString),comparedString(comparedString)
{
	myLog logfile;
	this->logFileHandle=&logfile;
	this->init();
}

LevenshteinDistance::LevenshteinDistance(string templateString,string comparedString,myLog* logFileHandler):templateString(templateString),comparedString(comparedString),logFileHandle(logFileHandler)
{	
	this->init();
}

LevenshteinDistance::LevenshteinDistance()
{
	myLog logfile;
	this->logFileHandle=&logfile;
	this->init();
}

LevenshteinDistance::LevenshteinDistance( myLog* logFileHandler2 )
{
	assert(logFileHandler2!=nullptr);
	this->logFileHandle=logFileHandler2;
	this->init();
}

LevenshteinDistance::~LevenshteinDistance(void)
{
	this->logFileHandle=nullptr;
	ptr = safe_delete(ptr,50+1);
}

void LevenshteinDistance::writeLogs(void)
{	
	assert(this->logFileHandle!=NULL);
	this->logFileHandle->finish();	
}


void LevenshteinDistance::init(void)
{
	this->ptr=new_matrix2 (MAXMATRIXROW+1,MAXMATRIXCOL+1,3,3);
	this->logFileHandle->debug("LevenshteinDistanceInit");	
}

vector<int> LevenshteinDistance::initFirstCol( MATRIX &mat,string left,string bottom,int maxDistanceLimit )
{
	vector<int> validRowIndex;
	for(int i=mat.rowSize-1;i>=0;i--)
	{
		if(i==mat.rowSize-1)
		{
			setValueFromMatrix(mat,i,0,0);
			validRowIndex.push_back(i);
		}
		else
		{
			int result=getValueFromMatrix(mat,i+1,0)+1>maxDistanceLimit?10000:getValueFromMatrix(mat,i+1,0)+1;
			setValueFromMatrix(mat,i,0,result);
			if(result!=10000)
				validRowIndex.push_back(i);
		}
	}
	return validRowIndex;
}
void LevenshteinDistance::makeUnvalidToLarge(MATRIX &mat,int col,vector<int> validVect,int maxValue)
{
	cout<<"valid";
	for(int i=0;i<validVect.size();i++)
	cout<<validVect[i]<<" ";
	cout<<endl;
	for(int i=0;i<mat.rowSize;i++)
	{		
		cout<<i;
		if(!intInVector(i,validVect))
		{
			cout<<i;
			setValueFromMatrix(mat,i,col,maxValue);
		}
	}
	cout<<endl;
}
int LevenshteinDistance::calcLeastEditMatrix( string origin, string textGet,int maxDistanceLimit )
{	
	MATRIX mat=zeros(origin.length(),textGet.length());
	initFirstCol(mat,origin,textGet,maxDistanceLimit);
	for(int j=1;j<textGet.length();j++)
	{
		for(int i=0;i<mat.rowSize;i++)
		{
			int realI=origin.length()-i-1;//转化成易于显示的矩阵时的行数index
			if(i==0)//就是每一列最开始的第一行，就是没有对角线和他的down方向的元素，所以只能看他的左边
			{
				setValueFromMatrix(mat,realI,j,getValueFromMatrix(mat,realI,j-1)+1);
			}else//从当前元素的左，对角下，正下 三个点取最小
			{
				int temp1= getValueFromMatrix(mat,realI,j-1);
				int temp2= origin[i]==textGet[j]?getValueFromMatrix(mat,realI+1,j-1):getValueFromMatrix(mat,realI+1,j-1)+1;
				int temp3= getValueFromMatrix(mat,realI+1,j);
				int minVal= min(temp1+1,min(temp2,temp3+1));
				if(minVal>maxDistanceLimit)
					minVal=10000;
				setValueFromMatrix(mat,realI,j,minVal);
			}
		}	
	}
	if(printMatrixOnScreen)
	{		
		cout<<origin<<" "<<textGet<<endl;
		cout<<"arraysize:"<<mat.array.size()<<endl;	
		cout<<"**********************"<<endl;
		showMeTheMatrixWithXYString(mat,origin,textGet,getShortestPathFromLeftBottomToRightTop(mat,false));
		this->getShortestPathLength(mat);
	}	
	return getShortestPathLength(mat);
}

int LevenshteinDistance::calcLeastEditMatrix2( string origin, string textGet,int maxDistanceLimit )
{		
	int max1 = origin.size();
	int max2 = textGet.size();
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
				if(origin[i-1]==textGet[j-1])
					d=0;
				else
					d=1;
				ptr[i][j] = min(temp+1,d+ptr[i-1][j-1]);
				if(ptr[i][j] > maxDistanceLimit)
					ptr[i][j]=MAXVALUE;
			}				
		}
	}
	if(printMatrixOnScreen){
		print(ptr,max1+1,max2+1,true,best_solution(ptr,max1+1,max2+1),origin,textGet);
		print(ptr,max1+1,max2+1,false,best_solution(ptr,max1+1,max2+1),origin,textGet);
	}
	int mindis = ptr[max1][max2];
	return mindis;	
}
int LevenshteinDistance::calcLeastEditMatrix2( string origin, int textGetInDict,int maxDistanceLimit )
{
		string textGet=dict[textGetInDict];
		int max1 = origin.size();
		int max2 = textGet.size();
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
					if(origin[i-1]==textGet[j-1])
						d=0;
					else
						d=1;
					ptr[i][j] = min(temp+1,d+ptr[i-1][j-1]);
					if(ptr[i][j] > maxDistanceLimit)
						ptr[i][j]=MAXVALUE;
				}				
			}
		}
		if(printMatrixOnScreen){
			print(ptr,max1+1,max2+1,true,best_solution(ptr,max1+1,max2+1),origin,textGet);
			print(ptr,max1+1,max2+1,false,best_solution(ptr,max1+1,max2+1),origin,textGet);
		}
		int mindis = ptr[max1][max2];
		return mindis;	
}
int LevenshteinDistance::getShortestPathLength(MATRIX mat)
{
	int result=getValueFromMatrix(mat,0,mat.colSize-1);
	//logFileHandle->debug(string("shortest edit distance result:").append(getstring(result)));
	return result;
}

MATRIX LevenshteinDistance::calcLeastEditMatrixUsingBeamSearch( string origin, string textGet,int maxDistanceLimit )
{
	MATRIX mat=zeros(origin.length(),textGet.length());
	initFirstCol(mat,origin,textGet,maxDistanceLimit);
	for(int j=1;j<textGet.length();j++)
	{
		int minOfCol=10000;
		for(int i=0;i<mat.rowSize;i++)
		{
			int realI=origin.length()-i-1;//转化成易于显示的矩阵时的行数index			
			if(i==0)//就是每一列最开始的第一行，就是没有对角线和他的down方向的元素，所以只能看他的左边
			{
				if(getValueFromMatrix(mat,realI,j-1)+1<minOfCol)
					minOfCol=getValueFromMatrix(mat,realI,j-1)+1;
				setValueFromMatrix(mat,realI,j,getValueFromMatrix(mat,realI,j-1)+1);
			}else//从当前元素的左，对角下，正下 三个点取最小
			{
				int temp1= getValueFromMatrix(mat,realI,j-1);
				int temp2= origin[i]==textGet[j]?getValueFromMatrix(mat,realI+1,j-1):getValueFromMatrix(mat,realI+1,j-1)+1;
				int temp3= getValueFromMatrix(mat,realI+1,j);
				int minVal= min(temp1+1,min(temp2,temp3+1));
				if(minVal<minOfCol)
					minOfCol=minVal;
				setValueFromMatrix(mat,realI,j,minVal);
			}
		}
		for(int i=0;i<mat.rowSize;i++)
		{
			if(getValueFromMatrix(mat,i,j)>minOfCol+maxDistanceLimit)
				setValueFromMatrix(mat,i,j,10000);
		}
	}
	if(printMatrixOnScreen)
	{
		cout<<origin<<" "<<textGet<<endl;
		cout<<"arraysize:"<<mat.array.size()<<endl;	
		cout<<"**********************"<<endl;
		showMeTheMatrixWithXYString(mat,origin,textGet,getShortestPathFromLeftBottomToRightTop(mat,false));
		this->getShortestPathLength(mat);
	}	
	return mat;
}
int LevenshteinDistance::calcLeastEditMatrixUsingBeamSearchNewest(string origin,string textGet,int maxDistance)
{	
	int max1 = origin.size();
	int max2 = textGet.size();
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
				if(origin[i-1]==textGet[j-1])
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
		for(int j = 1;j <max2+1;j++)
		{
			if(ptr[i][j]>mindis+maxDistance)
				ptr[i][j]=MAXVALUE;
		}
	}
	if(printMatrixOnScreen)
	{
		print(ptr,max1+1,max2+1,true,best_solution(ptr,max1+1,max2+1),origin,textGet);
		print(ptr,max1+1,max2+1,false,best_solution(ptr,max1+1,max2+1),origin,textGet);
	}
	int mindistance = ptr[max1][max2];
	return mindistance;
}
int LevenshteinDistance::calcLeastEditMatrixUsingBeamSearch2( string origin, string textGet,int maxDistanceLimit )
{
	int max1 = origin.size();
	int max2 = textGet.size();
	vector<int> temp,temp2;
	for(int i = 0; i < max1+1;i++)
	{
		if(i<=maxDistanceLimit)
			temp.push_back(i);
	}
	for(int i = 1;i < max2+1;i++)
	{
		int mindis=MAXVALUE;
		for(int j = 0;j <temp.size();j++)
		{
			if(ptr[temp[j]][i-1]+1 < ptr[temp[j]][i]||ptr[temp[j]][i]<0)
			{
				ptr[temp[j]][i]=ptr[temp[j]][i-1]+1;
			}
			if(ptr[temp[j]][i]<mindis)
				mindis=ptr[temp[j]][i];
			if(ptr[temp[j]][i]<=maxDistanceLimit&&!intInVector(temp[j],temp2))
			{
				temp2.push_back(temp[j]);
			}
			int d = 0;
			if(origin[temp[j]]==textGet[i-1])
				d=0;
			else
				d=1;
			if(temp[j]<max1){
				ptr[temp[j]+1][i]=min(ptr[temp[j]][i-1]+d,ptr[temp[j]][i]+1);
				if(ptr[temp[j]+1][i]<mindis)
					mindis=ptr[temp[j]+1][i];
				if(ptr[temp[j]+1][i]<=maxDistanceLimit&&!intInVector(temp[j]+1,temp2))
					temp2.push_back(temp[j]+1);
			}
		}
		temp.clear();
		temp=temp2;
		temp2.clear();
		/*for(int k = 0; k<max1+1;k++)
		{
		if(!intInVector(k,temp))
		ptr[k][i]=MAXVALUE;
		}*/
		maxDistanceLimit +=mindis;
	}
	if(printMatrixOnScreen)
	{
		print(ptr,max1+1,max2+1,true,best_solution(ptr,max1+1,max2+1),origin,textGet);
		print(ptr,max1+1,max2+1,false,best_solution(ptr,max1+1,max2+1),origin,textGet);
	}
	int mindistance = ptr[max1][max2];	
	return mindistance;
}

vector<int> LevenshteinDistance::getStringsFromTextAndDict(vector<string> &textGet )
{
	vector<int> result;
	//std::unordered_map <string,int> newDict;	
	//result.resize(textGet.size());
	double duration =0;
	cout<<dict.size()<<" "<<textGet.size()<<endl;
	clock_t start, finish;
	start = clock();
	for(int i=0;i<textGet.size();i++)
	{		
		//std::unordered_map <string,int>::iterator it = newDict.find(textGet[i]); 
		//if(it == newDict.end()){//如果这个词不在已记录字典中
		//	newDict.insert(make_pair(textGet[i],getMostRelatedStringUsingFixed(textGet[i],3)));	
		//	it = newDict.find(textGet[i]);			
		//}
		result.push_back(getMostRelatedStringUsingFixed(textGet[i],3));
		printf("%d:%d %s %s\n",i,textGet.size(),textGet[i].c_str(),dict[result[result.size()-1]].c_str());
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		printf( "%f seconds\n", duration );
		//result.push_back(this->getMostRelatedStringUsingFixed(textGet[i],dict,3));		
		//cout<<i<<":"<<textGet.size()<<" "<<textGet[i]<<" "<<result[result.size()-1]<<endl;
		//result[i]=getMostRelatedStringUsingFixed(textGet[i],dict,3);		
	}
	return result;
}

vector<int> LevenshteinDistance::getStringsFromTextAndDictBeamSearch( vector<string> &textGet,int m )
{
	vector<int> result;
	//std::map <string,int> newDict;	
	cout<<dict.size()<<" "<<textGet.size()<<endl;
	clock_t start, finish;
	start = clock();
	for(int i=0;i<textGet.size();i++)
	{
		//std::map <string,int>::iterator it = newDict.find(textGet[i]); 
		//if(it == newDict.end()){//如果这个词不在已记录字典中
		//	newDict.insert(make_pair(textGet[i],getMostRelatedStringUsingBeamSearch(textGet[i],3)));	
		//	it = newDict.find(textGet[i]);			
		//}
		result.push_back(this->getMostRelatedStringUsingBeamSearch(textGet[i],m));
		cout<<i<<":"<<textGet.size()<<" "<<textGet[i]<<" "<</*dict[it->second]<<endl;*/dict[result[result.size()-1]]<<endl;
		finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
		printf( "%f seconds\n", duration );
	}
	return result;
}int LevenshteinDistance::getMostRelatedStringUsingFixed( string origin,vector<string> dict2,int maxDistance )
{	
	int min=10000;
	int minIndex=-1;
	int temp=0;
	for(int i=0;i<dict2.size();i++)
	{
		if(origin==dict2[i])
			return i;
		if(abs((int)(origin.length()-dict2[i].length()))>maxDistance)
			continue;
		temp=calcLeastEditMatrix2(origin,dict2[i],maxDistance);
		if(temp<min)
		{
			min=temp;
			minIndex=i;
		}
	}
	if(minIndex!=-1)
		return minIndex;
	else//fatal error:出现对于某一个字符串完全找不到匹配的情况
	{				
		min=10000;
		minIndex=-1;
		for(int i=0;i<dict.size();i++)
		{					
			temp=this->calcLeastEditMatrix2(origin,dict2[i],abs((int)(origin.length()-dict2[i].length())));		
			if(temp<min)
			{
				min=temp;
				minIndex=i;
				if(temp==0)
					break;
			}
		}	
		return min;
	}	
}
string LevenshteinDistance::getTheMostRelatedStringUsingFixed( string origin,int maxDistance )
{
	return this->dict[getMostRelatedStringUsingFixed(origin,maxDistance)];
}
int LevenshteinDistance::getMostRelatedStringUsingFixed( string origin,int maxDistance )
{	
	int min=10000;
	int minIndex=-1;
	int temp=0;
	for(int i=0;i<dict.size();i++)
	{
		if(origin==dict[i])
			return i;
		if(abs((int)(origin.length()-dict[i].length()))>maxDistance)
			continue;
		temp=calcLeastEditMatrix2(origin,i,maxDistance);
		if(temp<min)
		{
			min=temp;
			minIndex=i;
		}
	}
	if(minIndex!=-1)
		return minIndex;
	else//fatal error:出现对于某一个字符串完全找不到匹配的情况
	{				
		min=10000;
		minIndex=-1;
		for(int i=0;i<dict.size();i++)
		{					
			temp=this->calcLeastEditMatrixUsingBeamSearchNewest(origin,dict[i],abs((int)(origin.length()-dict[i].length())));
			//temp=this->calcLeastEditMatrixUsingBeamSearch2(origin,dict[i],maxDistance);
			if(temp<min)
			{
				min=temp;
				minIndex=i;
				if(temp==0)
					break;
			}
		}	
		return min;
	}	
}
int LevenshteinDistance::getMostRelatedStringUsingBeamSearch( string origin,vector<string> dict2,int maxDistance )
{
	int min=10000;
	int minIndex=-1;
	int temp=0;
	for(int i=0;i<dict2.size();i++)
	{
		if(origin==dict2[i])
			return i;
		temp=this->calcLeastEditMatrixUsingBeamSearchNewest(origin,dict2[i],maxDistance);
		if(temp<min)
		{
			min=temp;
			minIndex=i;
			if(temp==0)
				break;
		}
	}	
	if(minIndex!=-1)
		return minIndex;
	else
	{
		min=10000;
		minIndex=-1;
		for(int i=0;i<dict2.size();i++)
		{					
			temp=this->calcLeastEditMatrixUsingBeamSearchNewest(origin,dict2[i],abs((int)(origin.length()-dict2[i].length())));
			if(temp<min)
			{
				min=temp;
				minIndex=i;
				if(temp==0)
					break;
			}
		}	
		return min;
	}	
}
int LevenshteinDistance::getMostRelatedStringUsingBeamSearch( string origin,int maxDistance )
{
	int min=10000;
	int minIndex=-1;
	int temp=0;
	for(int i=0;i<dict.size();i++)
	{
		if(origin==dict[i])
			return i;
		/*if(abs((int)(origin.length()-dict[i].length()))>maxDistance)
		continue;*/
		temp=this->calcLeastEditMatrixUsingBeamSearchNewest(origin,dict[i],maxDistance);
		//temp=this->calcLeastEditMatrixUsingBeamSearch2(origin,dict[i],maxDistance);
		if(temp<min)
		{
			min=temp;
			minIndex=i;
			if(temp==0)
				break;
		}
	}	
	if(minIndex!=-1)
		return minIndex;
	else
	{
		//assert(1==0);//fatal error:出现对于某一个字符串完全找不到匹配的情况
		//result="";
		min=10000;
		minIndex=-1;
		for(int i=0;i<dict.size();i++)
		{					
			temp=this->calcLeastEditMatrixUsingBeamSearchNewest(origin,dict[i],abs((int)(origin.length()-dict[i].length())));
			//temp=this->calcLeastEditMatrixUsingBeamSearch2(origin,dict[i],maxDistance);
			if(temp<min)
			{
				min=temp;
				minIndex=i;
				if(temp==0)
					break;
			}
		}	
		return min;
	}
	
}
LevenshteinDistance::ErrorNum LevenshteinDistance::computeErrors( string origin,string textGet,int maxDistanceLimit,bool usingFixed )
{
	bool flag=this->printMatrixOnScreen;
	this->printMatrixOnScreen=false;
	LevenshteinDistance::ErrorNum result;
	result.deletion=0;
	result.insertion=0;
	result.substitution=0;
	int ans;
	if(usingFixed)
		ans=this->calcLeastEditMatrix(origin,textGet,maxDistanceLimit);
	else
	{
		ans=this->calcLeastEditMatrixUsingBeamSearchNewest(origin,textGet,maxDistanceLimit);
	}
	if(ans!=MAXVALUE)
		this->best_solutionLD(ptr,origin.size()+1,textGet.size()+1,result);
	this->printMatrixOnScreen=flag;
	return result;
}
void LevenshteinDistance::best_solutionLD( int **ptr,int max1,int max2 ,LevenshteinDistance::ErrorNum &result)
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
					result.deletion++;
				}
				else if(ptr[i-1][j]+1==ptr[i][j])
				{
					best[i-1][j]=true;
					result.insertion++;
				}
				else if (ptr[i-1][j-1]+1==ptr[i][j])
				{
					best[i-1][j-1]=true;
					result.substitution++;
				}else
				{
					best[i-1][j-1]=true;
				}			
			}
		}
	}
}

//LevenshteinDistance::ErrorNumType LevenshteinDistance::computeTheErrors( int **ptr,int max1,int max2 )
//{
//
//}
