#include "util.h"
#include "myMatrix.h"
#include "LevenshteinDistance.h"
#define MAXVALUE 10000
//int inlineCalcLeastEditMatrix2( string origin, string textGet,int maxDistanceLimit ,bool isPrintOnScreen )
//{
//	int max1 = origin.size();
//	int max2 = textGet.size();
//
//	//int ** ptr = new_matrix (max1+1,max2+1,3,3);
//	vector<vector<int>> ptr= new_matrix (max1+1,max2+1,3,3);
//	int temp = 0;
//	int d = 0;
//	for(int i = 1;i < max1+1;i++)
//	{
//		for(int j = 1;j< max2+1;j++)
//		{
//			if(ptr[i-1][j]==MAXVALUE&&ptr[i][j-1]==MAXVALUE&&ptr[i-1][j-1]==MAXVALUE)
//				ptr[i][j]=MAXVALUE;
//			else{
//				temp=min(ptr[i-1][j],ptr[i][j-1]);
//				if(origin[i-1]==textGet[j-1])
//					d=0;
//				else
//					d=1;
//				ptr[i][j] = min(temp+1,d+ptr[i-1][j-1]);
//				if(ptr[i][j] > maxDistanceLimit)
//					ptr[i][j]=MAXVALUE;
//			}				
//		}
//	}
//	if(isPrintOnScreen){
//		print(ptr,max1+1,max2+1,true,best_solution(ptr,max1+1,max2+1),'?'+origin,'?'+textGet);
//		print(ptr,max1+1,max2+1,false,best_solution(ptr,max1+1,max2+1),'?'+origin,'?'+textGet);
//	}
//	int mindis = ptr[max1][max2];
//	//ptr = safe_delete(ptr,max1+1);
//	return mindis;	
//}

string getstring ( const int n )
{
	stringstream newstr;
	newstr<<n;
	return newstr.str();
}

bool isCharInString( const char c,const string &str )
{
	for(int i=0;i<str.length();i++)
		if(c==str[i])
			return true;
	return false;
}

bool intInVector( const int value,const vector<int> &vect )
{
	for(int i=0;i<vect.size();i++)
		if(vect[i]==value)
			return true;
	return false;
}
int ** new_matrix2(int row,int col,int c_col,int c_row){
	int ** ptr = new int * [row];
	//vector<vector<int>> ptr;
	//ptr.resize(row);
	for(int i = 0;i<row;i++){
		//ptr[i].resize(col);
		ptr[i]=new int[col];
	}
	for(int i = 0;i<row;i++){
		if(i>c_col && c_col>=0)
			ptr[i][0] = MAXVALUE;
		else{
			ptr[i][0] = i;
		}
	}
	for(int i = 0;i<col;i++){
		if(i>c_row && c_row>=0 ){
			ptr[0][i] = MAXVALUE;
		}
		else ptr[0][i] = i;
	}
	return ptr;
}
vector<vector<int>> new_matrix(int row,int col,int c_col,int c_row){
	//int ** ptr = new int * [row];
	vector<vector<int>> ptr;
	ptr.resize(row);
	for(int i = 0;i<row;i++){
		ptr[i].resize(col);
	}
	for(int i = 0;i<row;i++){
		if(i>c_col && c_col>=0)
			ptr[i][0] = MAXVALUE;
		else{
			ptr[i][0] = i;
		}
	}
	for(int i = 0;i<col;i++){
		if(i>c_row && c_row>=0 ){
			ptr[0][i] = MAXVALUE;
		}
		else ptr[0][i] = i;
	}
	return ptr;
}
int ** safe_delete(int ** ptr, int row){

	for(int i=0;i<row;i++)
	{
		delete[] ptr[i];
		ptr[i]=NULL;
	}
	delete[] ptr;
	ptr=NULL;
	return NULL;
}
bool ** best_solution(vector<vector<int>> ptr,int max1,int max2)
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
					//continue;
				}
				else if(ptr[i-1][j]+1==ptr[i][j])
				{
					best[i-1][j]=true;
					//continue;
				}
				else
				{
					best[i-1][j-1]=true;
					//continue;
				}
			}
		}
	}
	return best;
}
bool ** best_solution(int **ptr,int max1,int max2){
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
					//continue;
				}
				else if(ptr[i-1][j]+1==ptr[i][j])
				{
					best[i-1][j]=true;
										//continue;
				}
				else if (ptr[i-1][j-1]+1==ptr[i][j])
				{
					best[i-1][j-1]=true;
				}else
				{
					best[i-1][j-1]=true;
				}			
			}
		}
	}
	return best;
}