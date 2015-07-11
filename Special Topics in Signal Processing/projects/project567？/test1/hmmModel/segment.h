#ifndef _SEGMENT_H_
#define _SEGMENT_H

class Segment {
public:
	Segment() { beginIndex = endIndex = 0; isExist = false; }
	Segment(int beginIndex, int endIndex, bool isExist) : beginIndex(beginIndex), endIndex(endIndex), isExist(isExist) {}
	int beginIndex;
	int endIndex; // the next node about the end node.
	bool isExist;
	int getLength() {
		if (!isExist) {
			return 0;
		}
		return endIndex - beginIndex;
	}
};
#endif