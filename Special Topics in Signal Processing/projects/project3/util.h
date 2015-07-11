#pragma once
#include <string>
#include "myLog.h"
#include <stdlib.h> //Ҫ��string���int����ֱ��ת���������Щ������
#include <sstream>
//************************************
// Method:    isCharInString �ж��ַ�c�Ƿ����ַ���str��
// FullName:  isCharInString
// Access:    public 
// Returns:   bool true���c��str��
// Qualifier:
// Parameter: char c 
// Parameter: string str 
//************************************
bool isCharInString(const char c,const string &str);

//************************************
// Method:    ������nת�����ַ���
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