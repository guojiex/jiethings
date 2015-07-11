#pragma once
#include "DTW.h"
#include <vector>
#include "Segment.h"
#define deltaVectorSize 39
class HMM2
{
public:
	
	int states;
	HMM2( DTW input,int st);
	~HMM2(void);
};

