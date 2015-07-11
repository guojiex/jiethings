/************************************************************************/
/*	自定义用于最短编辑距离列表的 矩阵类
	内部用一维vector实现*/
/************************************************************************/
#pragma once
#include "myLog.h"

#include "coutColor.h"


typedef struct MATRIXTYPE
{
	vector<int> array;
	int rowSize;
	int colSize;
}MATRIX;
typedef struct myPOINT
{
	int row;
	int col;
}myPoint;
//************************************
// Method:    getLengthOfAnInt
// FullName:  getLengthOfAnInt
// Access:    public 
// Returns:   int
// Parameter: int value
// like 1000 return 4
//************************************
int getLengthOfAnInt(int value);

//************************************
// Method:    getValueFromMatrix
// FullName:  getValueFromMatrix
// Access:    public 
// Returns:   int
// Parameter: const MATRIX mat 要读取的matrix对象
// Parameter: int row 
// Parameter: int col 
//************************************
int getValueFromMatrix(const MATRIX &mat,int row,int col);
int getValueFromMatrix(const MATRIX &mat,myPoint point);
void setValueFromMatrix(MATRIX &mat,int row,int col,int value);

void showMeTheMatrix(const MATRIX mat);
void showMeTheMatrixWithXYString(const MATRIX mat,string left,string bottom);
//************************************
// Method:    在屏幕上输出整个计算矩阵，最左边是origin字符串，最下面是textGet字符串，因为两个字符串都会加一个?作为对齐，所以不显示
//			  第0列和最下面一行colSize-1
// FullName:  showMeTheMatrixWithXYString
// Access:    public 
// Returns:   void
// Parameter: const MATRIX mat
// Parameter: string left
// Parameter: string bottom
// Parameter: vector<myPoint> path
//************************************
void showMeTheMatrixWithXYString(const MATRIX mat,string left,string bottom,vector<myPoint> path);
//************************************
// Method:    create a rowSize*colSize matrix that all zeros 
// FullName:  zeros
// Access:    public 
// Returns:   MATRIX
// Qualifier:
// Parameter: int rowSize
// Parameter: int colSize
//************************************
MATRIX zeros(int rowSize,int colSize);
vector<myPoint> getShortestPathFromLeftBottomToRightTop( MATRIX mat,bool isOutputOnscreen );
void print(int ** ptr,int row,int col,bool showMax ,bool ** best,string origin,string textGet);
void print(vector<vector<int>> ptr,int row,int col,bool showMax ,bool ** best,string origin,string textGet);
void print(int ** ptr,int row,int col,bool showMax ,bool ** best);