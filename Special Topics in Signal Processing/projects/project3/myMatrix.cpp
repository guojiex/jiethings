#include "myMatrix.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;
#define MAXVALUE 10000
void print(int ** ptr,int row,int col,bool showMax ,bool ** best){
	printf("%s\n",ptr[row-1][col-1]!=MAXVALUE?"have":"no");
	cout << "********************************" << endl;
	cout<<setw(getLengthOfAnInt(10000)+1)<<" ";
	for(int i = 0;i < row;i++)
	{
		for(int j=0;j<col;j++)
		{			

			if(ptr[i][j]!=MAXVALUE&&ptr[i][j]>=0 &&best[i][j]==true)
				cout<<greencolor<<setw(5)<<ptr[i][j]<<" "<<defcolor;
			else if(ptr[i][j]!=MAXVALUE&&ptr[i][j]>=0 || showMax)
				cout << defcolor << setw(5) << ptr[i][j] << " "; 
			else
				cout << setw(5) << " " << " ";
		}
		cout << endl;
	}
	cout << "********************************" << endl;
}
void print(int ** ptr,int row,int col,bool showMax ,bool ** best,string origin,string textGet){
	origin="?"+origin;	
	textGet="?"+textGet;
	/*for(int i=0;i<row;i++)
	{
	for(int j=0;j<col;j++)
	printf("%s   ",best[i][j]==true?"true":"false");
	cout<<endl;
	}*/
	printf("%s\n",ptr[row-1][col-1]!=MAXVALUE?"have":"no");
	cout << "********************************" << endl;
	cout<<setw(getLengthOfAnInt(10000)+1)<<" ";
	for(int i=0;i<textGet.length();i++)
	{
		cout<<setw(getLengthOfAnInt(10000)+1)<<textGet[i];
	}
	cout<<endl;
	for(int i = 0;i < row;i++)
	{
		for(int j=0;j<col;j++)
		{
			if(j==0)				
				cout<<setw(getLengthOfAnInt(10000)+1)<<origin[i]<<" ";

			if(ptr[i][j]!=MAXVALUE&&ptr[i][j]>=0 &&best[i][j]==true)
				cout<<greencolor<<setw(5)<<ptr[i][j]<<" "<<defcolor;
			else if(ptr[i][j]!=MAXVALUE&&ptr[i][j]>=0 || showMax)
				cout << defcolor << setw(5) << ptr[i][j] << " "; 
			else
				cout << setw(5) << " " << " ";
		}
		cout << endl;
	}
	cout << "********************************" << endl;
}
void print(vector<vector<int>> ptr,int row,int col,bool showMax ,bool ** best,string origin,string textGet){
	cout << "********************************" << endl;
	origin="?"+origin;	
	textGet="?"+textGet;
	cout<<setw(getLengthOfAnInt(10000)+1)<<" ";
	for(int i=0;i<textGet.length();i++)
	{
		cout<<setw(getLengthOfAnInt(10000)+1)<<textGet[i];
	}
	cout<<endl;
	for(int i = 0;i < row;i++)
	{
		for(int j=0;j<col;j++)
		{
			if(j==0)				
				cout<<setw(getLengthOfAnInt(10000)+1)<<origin[i]<<" ";

			if(ptr[i][j]!=MAXVALUE&&ptr[i][j]>=0 &&best[i][j]==true)
				cout<<greencolor<<setw(5)<<ptr[i][j]<<" "<<defcolor;
			else if(ptr[i][j]!=MAXVALUE&&ptr[i][j]>=0 || showMax)
				cout << defcolor << setw(5) << ptr[i][j] << " "; 
			else
				cout << setw(5) << " " << " ";
		}
		cout << endl;
	}
	cout << "********************************" << endl;
}
int getLengthOfAnInt(int value)
{
	return ceil(log10(value+1));
}
//row from 0,col from 0
int getValueFromMatrix(const MATRIX &mat,int row,int col)
{
	return mat.array[mat.colSize*row+col];
}
int getValueFromMatrix(const MATRIX &mat,myPoint point)
{
	return getValueFromMatrix(mat,point.row,point.col);
}
void setValueFromMatrix(MATRIX &mat,int row,int col,int value){
	mat.array[mat.colSize*row+col]=value;
}
void showMeTheMatrix(const MATRIX mat)
{
	int max=*max_element(mat.array.begin(),mat.array.end());
	for(int i=0;i<mat.rowSize;i++)
	{
		for(int j=0;j<mat.colSize;j++)
			cout<<setw(getLengthOfAnInt(max)+1)<<getValueFromMatrix(mat,i,j)<<" ";
		cout<<endl;
	}
}

MATRIX zeros( int rowSize,int colSize )
{
	MATRIX mat;
	mat.rowSize=rowSize;
	mat.colSize=colSize;
	for(int i=0;i<mat.rowSize;i++)
		for(int j=0;j<mat.colSize;j++)
			mat.array.push_back(0);
	return mat;
}

void showMeTheMatrixWithXYString( const MATRIX mat,string left,string bottom )
{
	int max=*max_element(mat.array.begin(),mat.array.end());
	for(int i=0;i<mat.rowSize-1;i++)
	{
		for(int j=1;j<mat.colSize;j++)
		{
			if(j==1)
				cout<<setw(getLengthOfAnInt(max)+1)<<left[left.length()-i-1]<<" ";

			cout<<setw(getLengthOfAnInt(max)+1)<<getValueFromMatrix(mat,i,j)<<" ";
		}
		cout<<endl;
		if(i==mat.rowSize-2)
		{
			cout<<setw(getLengthOfAnInt(max)+2)<<" ";
			for(int i=1;i<bottom.length();i++)
			{
				cout<<setw(getLengthOfAnInt(max)+1)<<bottom[i]<<" ";
			}
			cout<<endl;
		}
	}
}

void showMeTheMatrixWithXYString( const MATRIX mat,string left,string bottom,vector<myPoint> path )
{
	int max=*max_element(mat.array.begin(),mat.array.end());
	MATRIX setColor = zeros(mat.rowSize,mat.colSize);
	for(int i=0;i<path.size();i++)
		setValueFromMatrix(setColor,path[i].row,path[i].col,1);
	for(int i=0;i<mat.rowSize-1;i++)
	{
		for(int j=1;j<mat.colSize;j++)
		{
			if(j==1)
				cout<<setw(getLengthOfAnInt(max)+1)<<left[left.length()-i-1]<<" ";

			if (1==getValueFromMatrix(setColor,i,j))
			{
				cout<<greencolor<<setw(getLengthOfAnInt(max)+1)<<getValueFromMatrix(mat,i,j)<<" "<<defcolor;
			}else
			{
				if(getValueFromMatrix(mat,i,j)!=10000)
					cout<<setw(getLengthOfAnInt(max)+1)<<getValueFromMatrix(mat,i,j)<<" ";
				else
				{
					cout<<setw(getLengthOfAnInt(max)+1)<<"     "<<" ";
				}
			}
		}
		cout<<endl;
		if(i==mat.rowSize-2)
		{
			cout<<setw(getLengthOfAnInt(max)+2)<<" ";
			for(int i=1;i<bottom.length();i++)
			{
				cout<<setw(getLengthOfAnInt(max)+1)<<bottom[i]<<" ";
			}
			cout<<endl;
		}
	}
}

vector<myPoint> getShortestPathFromLeftBottomToRightTop( MATRIX mat,bool isOutputOnscreen )
{
	vector<myPoint> path;
	myPoint current;
	current.row=0;
	current.col=mat.colSize-1;
	if(getValueFromMatrix(mat,current)!=10000)
		path.push_back(current);
	else
	{
		return path;
	}
	while(current.row!=mat.rowSize-1-1&&current.col!=0+1)
	{
		int down= (current.row+1<mat.rowSize-1)?getValueFromMatrix(mat,current.row+1,current.col):10000;
		int left= (current.col-1>=0)?getValueFromMatrix(mat,current.row,current.col-1):10000;
		int dignal= (current.row+1<mat.rowSize-1&&current.col-1>=0)?getValueFromMatrix(mat,current.row+1,current.col-1):10000;
		int minVal=min(down,min(left,dignal));
		if(minVal==10000)
			break;
		if(minVal==dignal)
		{
			myPoint temp;
			temp.row=current.row+1;
			temp.col=current.col-1;
			path.push_back(temp);
			current.row=current.row+1;
			current.col=current.col-1;
		}else if(minVal==down)
		{
			myPoint temp;
			temp.row=current.row+1;
			temp.col=current.col;
			path.push_back(temp);
			current.row=current.row+1;
			current.col=current.col;
		}else if(minVal==left)
		{
			myPoint temp;
			temp.row=current.row;
			temp.col=current.col-1;
			path.push_back(temp);
			current.row=current.row;
			current.col=current.col-1;
		}
	}
	reverse(path.begin(),path.end());
	/*while(current.row!=0||current.col!=mat.colSize-1)
	{
	int up= (current.row-1>=0)?getValueFromMatrix(mat,current.row-1,current.col):10000;
	int right= (current.col+1<mat.colSize)?getValueFromMatrix(mat,current.row,current.col+1):10000;
	int dignal= (current.row-1>=0&&current.col+1<mat.colSize)?getValueFromMatrix(mat,current.row-1,current.col+1):10000;
	int minVal=min(up,min(right,dignal));
	if(minVal==10000)
	break;
	if(minVal==dignal)
	{
	myPoint temp;
	temp.row=current.row-1;
	temp.col=current.col+1;
	path.push_back(temp);
	current.row=current.row-1;
	current.col=current.col+1;
	}else if(minVal==up)
	{
	myPoint temp;
	temp.row=current.row-1;
	temp.col=current.col;
	path.push_back(temp);
	current.row=current.row-1;
	current.col=current.col;
	}else if(minVal=right)
	{
	myPoint temp;
	temp.row=current.row;
	temp.col=current.col+1;
	path.push_back(temp);
	current.row=current.row;
	current.col=current.col+1;
	}
	}*/
	if(isOutputOnscreen)
		for(int i=0;i<path.size();i++)
			cout<<path[i].row<<" "<<path[i].col<<" "<<getValueFromMatrix(mat,path[i])<<endl;
	return path;
}