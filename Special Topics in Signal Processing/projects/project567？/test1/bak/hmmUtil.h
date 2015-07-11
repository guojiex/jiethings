#pragma once
//#include "HMM3.h"
#include "myTemplate.h"
#include "DTW.h"

#include "templateList.h"
double DTWminCostFromHMM(HMM3 &model,myTemplate &myTemp);
void multTestHMM(const DTW &test,HMM3 models[10], int dataIndexLeft, int dataIndexRight,int left,int right);
void HMMTest();
double doResegmentTemplateByHMM(HMM3 &model,myTemplate &myTemp);
DTW::DTW_result getMinCostFromHMMModels(HMM3 models[10],myTemplate &myTemp);
double doViterbiSearch(HMM3& hmmModel, myTemplate& singleTemplate);