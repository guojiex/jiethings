#ifndef _TEMPLATE_LIST_H_
#define _TEMPLATE_H_
#include <vector>
#include "template.h"
#include "hmmModel.h"
using std::vector;
using std::string;
class TemplateList {
private:
	Template templates[100];
	int count;
	int segLength;
	int deltaDimension;
public:
	TemplateList() { count = segLength = 0;}
	TemplateList(int count, int segLength, int nCep);
	bool readMutiTemplate(vector<string>& filename);
	HMMModel buildHMMModel(HMMModel::StateType type);
	HMMModel generateTempModel(HMMModel::StateType type);
};

#endif