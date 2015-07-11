#pragma once
#include "myTemplate.h"
#include "HMM3.h"
#include <vector>
class templateList
{
private:
	std::vector<myTemplate> templates;
	int templatesCount;
	int deltaDimension;
	int segmentNumber;
public:
	templateList(void);
	templateList(int deltaDimension,int segmentNumber)
		:deltaDimension(deltaDimension),segmentNumber(segmentNumber){};
	void readMultiTemplate(const std::vector<std::string> &fileNames);
	void readTemplatesFromVector(const std::vector<std::vector<std::vector<double>>> &data);
	HMM3 generateTemplateModel(HMM3::StateType type);
	HMM3 generateHMMModel(HMM3::StateType type);
	~templateList(void);
};

