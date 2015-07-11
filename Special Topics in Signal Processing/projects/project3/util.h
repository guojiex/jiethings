#pragma once
#include <string>
#include "myLog.h"
#include <stdlib.h> //要将string类和int类型直接转换最好有这些包含，
#include <sstream>
//************************************
// Method:    isCharInString 判断字符c是否在字符串str中
// FullName:  isCharInString
// Access:    public 
// Returns:   bool true如果c在str中
// Qualifier:
// Parameter: char c 
// Parameter: string str 
//************************************
bool isCharInString(const char c,const string &str);

//************************************
// Method:    把整数n转换成字符串
// FullName:  getstring
// Access:    public 
// Returns:   std::string
// Qualifier:
// Parameter: const int n
//************************************
string getstring ( const int n );

bool intInVector(const int value,const vector<int> &vect);
//int ** new_matrix(int row,int col,int c_col=-1,int c_row=-1);
vector<vector<int>> new_matrix(int row,int col,int c_col,int c_row);
int ** safe_delete(int ** ptr, int row);
int ** safe_delete(vector<vector<int>> ptr, int row);
bool ** best_solution(int **ptr,int max1,int max2);
bool ** best_solution(vector<vector<int>> ptr,int max1,int max2);
int ** new_matrix2(int row,int col,int c_col,int c_row);
//int inlineCalcLeastEditMatrix2( string origin, string textGet,int maxDistanceLimit ,bool isPrintOnScreen );