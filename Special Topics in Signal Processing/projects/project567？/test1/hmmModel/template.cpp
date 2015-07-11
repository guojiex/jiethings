#include "template.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
Template::Template(Template& prevTemplate) {
	this->frameSize = prevTemplate.frameSize;
	this->nCep = prevTemplate.nCep;
	this->segLength = prevTemplate.segLength;
	for (int i = 0; i < this->segLength; i++) {
		this->curStage[i] = prevTemplate.curStage[i];
		this->prevStage[i] = prevTemplate.prevStage[i];
	}
	safe_deleteCepMatrix();
	if (prevTemplate.cepMatrix != NULL) {
		this->cepMatrix = new double*[frameSize];
		for (int i = 0; i < frameSize; i++) {
			this->cepMatrix[i] = new double[nCep];
			for (int j = 0; j < nCep; j++) {
				this->cepMatrix[i][j] = prevTemplate.cepMatrix[i][j];
			}
		}
	}
	//printf("t %d\n",cepMatrix);
}

Template& Template::operator = (const Template &prevTemplate) {
	this->frameSize = prevTemplate.frameSize;
	this->nCep = prevTemplate.nCep;
	this->segLength = prevTemplate.segLength;
	for (int i = 0; i < this->segLength; i++) {
		this->curStage[i] = prevTemplate.curStage[i];
		this->prevStage[i] = prevTemplate.prevStage[i];
	}
	safe_deleteCepMatrix();
	if (prevTemplate.cepMatrix != NULL) {
		this->cepMatrix = new double*[frameSize];
		for (int i = 0; i < frameSize; i++) {
			this->cepMatrix[i] = new double[nCep];
			for (int j = 0; j < nCep; j++) {
				this->cepMatrix[i][j] = prevTemplate.cepMatrix[i][j];
			}
		}
	}/*
	printf("= %d\n",cepMatrix);
*/
	return *this;
}

Template::~Template() {	
	safe_deleteCepMatrix();
}
void Template::safe_deleteCepMatrix()
{
	if(cepMatrix!=nullptr)
	{
		for (int i = 0; i < frameSize; i++) {
			if(cepMatrix[i]!=nullptr)
				delete[] cepMatrix[i];
		}
		delete[] cepMatrix;
	}
}
bool Template::readVector( const vector<vector<double>> &data )
{	
	assert(data[0].size()==39);
	frameSize=data.size();
	nCep=data[0].size();
	assert(nCep==39);
	safe_deleteCepMatrix();
	cepMatrix = new double*[frameSize];
	for(int i=0;i<frameSize;++i)
		cepMatrix[i]=new double[nCep];
	for(int i=0;i<frameSize;++i)
	{
		for (int j=0;j<nCep;++j)
		{
			cepMatrix[i][j]=data[i][j];
		}
	}
	return true;
}

void Template::readTemplate2( string fileName )
{
	std::vector<std::vector<double>> res;
	FILE *fq;
	fq=fopen(fileName.c_str(),"r+");
	if(fq==NULL)
	{
		printf("fatal:file not found or load error.\n");
		system("pause");
		exit(-1);
	}
	std::vector<double> oneline;
	oneline.resize(39);
	int index=0;
	while (fscanf(fq,"%lf",&oneline[index])!=EOF)
	{
		index++;
		if(index>=39){
			res.push_back(oneline);
			index=0;
		}
	}	
	fclose(fq);
	//printf("read delta file %s \n",fileName.c_str());
	frameSize=res.size();
	nCep=39;
	assert(nCep==39);
	//safe_deleteCepMatrix();
	cepMatrix = new double*[frameSize];
	for(int i=0;i<frameSize;++i)
		cepMatrix[i]=new double[nCep];
	for(int i=0;i<frameSize;++i)
		for (int j=0;j<nCep;++j)
		{
			cepMatrix[i][j]=res[i][j];
		}
	int i=0;
}

bool Template::readTemplate(string filename) {
	assert(1==0);//abonded
	ifstream in(filename);
	string line;
	if (!in.is_open()) {
		return false;
	}
	getline(in, line);
	istringstream linestream(line);
	linestream >> frameSize >> nCep;
	safe_deleteCepMatrix();
	while (getline(in, line)) {
		cepMatrix = new double*[frameSize];
		istringstream linestream(line);
		for (int i = 0; i < frameSize; i++) {
			cepMatrix[i] = new double[nCep];
			for (int j = 0; j < nCep; j++) {
				linestream >> cepMatrix[i][j];
			}
		}
	}
	return true;
}

bool Template::isConverge() {
	int threshold = 1;
	for (int i = 0; i < segLength; i++) {
		if (abs(curStage[i].beginIndex - prevStage[i].beginIndex) > threshold || abs(curStage[i].endIndex - prevStage[i].endIndex) > threshold) {
			return false;
		}
	}

	return true;
}
void Template::setStageInfo(Segment stage[10]) {
	for (int i = 0; i < segLength; i++) {
		prevStage[i] = curStage[i];
		curStage[i] = stage[i];
	}
}
int* Template::getCurStageInfo(int& length) {
	return NULL;
}

double** Template::getStageData(int& size, int& nCep, int stageNo) {
	int beginIndex = curStage[stageNo].beginIndex;
	int endIndex = curStage[stageNo].endIndex;
	int len = endIndex - beginIndex;
	size = len;
	this->nCep = nCep;
	double** data = new double*[size];
	for (int i = 0; i < size; i++) {
		data[i] = new double[nCep];
		for (int j = 0; j < nCep; j++) {
			data[i][j] = cepMatrix[i + beginIndex][j];
		}
	}

	return data;
}

bool Template::init() {
	int size = frameSize / segLength;
	for (int i = 0; i < segLength; i++) {
		curStage[i] = Segment(size * i, min(size*(i+1), frameSize), true);
	}

	return true;
}

void Template::getStageSize(int& size, int& nCep, int stageNo) {
	size = curStage[stageNo].getLength();
	nCep = this->nCep;
}

int Template::fillData(double** data, int stageNo, int beginIndex) {
	if (!curStage[stageNo].isExist) {
		return 0;
	}
	int firstIndex = curStage[stageNo].beginIndex;
	int endIndex = curStage[stageNo].endIndex;
	int len = endIndex - firstIndex;
	//printf("fill %d %lf\n",cepMatrix,cepMatrix[0+firstIndex][0]);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < nCep; j++) {
			data[beginIndex+i][j] = cepMatrix[i + firstIndex][j];
		}
	}

	return len;
}

double** Template::getCepMatrix(int& size, int& nCep) {
	size = this->frameSize;
	nCep = this->nCep;

	return cepMatrix;
}

void Template::setTemplateInfo(int segLength, int nCep) {
	this->segLength = segLength;
	this->nCep = nCep;
}
