#pragma once
class Segment
{
public:
	bool exist;
	int beginIndex;
	int endIndex;
	Segment(void)
	{
		exist=false;
		beginIndex=0;
		endIndex=0;
	}
	Segment(int beginIndex,int endIndex,bool exist):beginIndex(beginIndex),endIndex(endIndex),exist(exist){}
	int getLength()
	{
		if(!exist)
			return 0;
		return endIndex-beginIndex+1;
	}
	~Segment(void)
	{
	}
};

