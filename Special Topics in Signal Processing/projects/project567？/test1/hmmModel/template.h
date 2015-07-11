#ifndef _TEMPLATE_H
#define _TEMPLATE_H
#include "segment.h"
#include <vector>
#include <string>
class Template {
private:
	Segment curStage[10];
	Segment prevStage[10];
	double** cepMatrix;
	int frameSize;
	int nCep;
	int segLength;
public:
	Template() { segLength = nCep = 0; cepMatrix=NULL;}
	Template(int segLength, int nCep) :segLength(segLength), nCep(nCep) {cepMatrix=NULL;}
	Template(Template& prevTempalte);
	Template& operator = (const Template &prevTemplate);
	~Template();
	bool readTemplate(std::string filename);
	void readTemplate2(std::string filename);
	bool readVector(const std::vector<std::vector<double>> &data);
	bool isConverge();
	void setStageInfo(Segment stage[10]);
	void setTemplateInfo(int segLength, int nCep);
	int* getCurStageInfo(int& length);
	double** getStageData(int& size, int& nCep, int stageNo);
	void getStageSize(int& size, int& nCep, int stageNo);
	int fillData(double** data, int stageNo, int beginIndex);
	double** getCepMatrix(int& size, int& nCep);
	bool init();
	void safe_deleteCepMatrix();
};

#endif