#ifndef _HMMTOOL_
#define _HMMTOOL_
#include "hmmModel.h"
#include "template.h"
#include <algorithm>
enum TestType {
	viterbiSearch, forwardAlgo
};
double doViterbiSearchAndResegment(HMMModel& hmmModel, Template& singleTemplate);
double getHMMCompareProb(HMMModel& hmmModel, Template& singleTemplate);
void HMMtest();
template <class T> void safe_deleteCepMatrix(T** matrix,const int &rowNumber);
int testSingleNumber(HMMModel hmmModels[10],int number, int index, int type);
#endif