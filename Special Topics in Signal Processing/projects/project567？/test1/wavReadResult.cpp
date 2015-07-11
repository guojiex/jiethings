#include "wavReadResult.h"
#include <iostream>
#include <sstream>
#include <complex>
#include <stdio.h>  
#include <direct.h> 
#include <stdlib.h>  
#include <memory> 
wavReadResult::wavReadResult(string fileName)
{	
	this->init();
	d=readVectorFromFile(fileName);
	printf("Successfully input %d double.\n",d.size());
}

wavReadResult::~wavReadResult(void){d.clear();}

int wavReadResult::readAWav(string fileName)
{
	string text;
	ifstream infile;
	infile.open(fileName,ios::in); 		
	int time=0;
	while(infile.good() && !infile.eof())
	{
		//todo:读取特别的raw文件
	}
	infile.close();
	return this->d.size();
}
///按照dir路径生成文件夹（只能生成某个已有文件夹的下一层，不能直接生成下两层）
int CheckDir(const char* Dir)  
{  
	FILE *fp = NULL;  
	char TempDir[200];  
	memset(TempDir,'\0',sizeof(TempDir));  
	sprintf(TempDir,Dir);  
	strcat(TempDir,"\\");  
	strcat(TempDir,".temp.fortest");  
	fp = fopen(TempDir,"w");  
	if (!fp)  
	{  
		if(_mkdir(Dir)==0)  
		{  
			return 1;//文件夹创建成功  
		}  
		else  
		{  
			return -1;//can not make a dir;  
		}  
	}  
	else  
	{  
		fclose(fp);  
	}  
	return 0;  
}  
wavReadResult::wavReadResult(SAMPLE* samples,int size,string folder,string times)
{
	this->init();//init();
	for(int i=0;i<size;i++)
		d.push_back(samples[i]);
	int left=0;
	int right=left+this->WINDOW_LENGTH;
	d=Preemphasizing(d);
	cout<<"now start DFT with point:"<<size<<endl;
	time_t start ,end ;  
    double cost;  
    time(&start);  	
	while(left<d.size())
	{
		if(right+1>=d.size())//最后一个窗口有可能right已经超过数组大小，要恢复right到数组d的大小
		{
			right=d.size();
		}		
		
		vector<double> source=this->windowing(d,left,right);		
		DFTresult res=this->DFT(source);
		res.DCT(this->featuresLengthForcepstra,BANKNUM);	
		this->resultsOfMFCC.push_back(res);				
	}
	time(&end);  
	cost=difftime(end,start);  
	printf("%f\n",cost); 
	cout<<"FFT and mel and DCT end,with windows number:"<<this->resultsOfMFCC.size()<<endl;
	this->DCTNormalize();
	this->calcDelta();
	/*int index=10;	
	this->resultsOfMFCC[index].writePowersToFile();
	this->resultsOfMFCC[index].writeFilterBankOutputToFile();
	this->resultsOfMFCC[index].writeLogMelToFile();
	this->resultsOfMFCC[index].writeDCTresultToFile();*/
	folder+=times;
	/*CheckDir(folder.c_str());*/
	string mel=folder+"mel.txt";
	string power=folder+"power.txt";
	//string mellog="mellog.txt";
	string dctresultName=folder+"DCTresult.txt";
	string normalFilename=folder+"dctnormalized.txt";
	string deltaFilename=folder+"delta.txt";
	this->writeMelToFile(mel);
	this->writeDFTPowerResultToFile(power);
	this->writeAllNormalToFile(normalFilename);
	//this->writeAllLogMelTofile(mellog);
	this->writeBeforeNormalizeToFile(dctresultName);
	this->writeDeltaTofile(deltaFilename);
}

///对v数组从left到right做dft
//DFTresult wavReadResult::DFT(vector<double> v,int left,int right)
//{	
//	DFTresult res=DFTresult(v,left,right,this->WINDOW_LENGTH);
//	res.windowPoints=this->WINDOW_LENGTH;
//	return res;
//}
///对v数组做dft
DFTresult wavReadResult::DFT(vector<double> &v)
{	
	DFTresult res=DFTresult(v,v.size());
	res.windowPoints=v.size();
	return res;
}

vector<double> wavReadResult::Preemphasizing(vector<double> v)//全数组预放大
{
	vector<double> result;
	for(unsigned int i=0;i<v.size();i++)
	{
		if(i!=0)
			result.push_back(v[i]-0.95*v[i-1]);
		else
			result.push_back(v[i]);
	}
	return result;
}

bool wavReadResult::setSampleRate(int rate)//设置采样频率
{
	if(rate!=0){
		this->sample_rate=rate;
		return true;
	}else{
		return false;
	}
}

void wavReadResult::init(void)
{
	this->sample_rate=16000;
}

vector<double> wavReadResult::windowing(vector<double> &v, int &left, int &right)
{
	vector<double> result;
	int index=0;	
	for(int i=left;i<right;i++)
	{		
		result.push_back(v[i]*(0.5-0.5*cos(2*PI*index/WINDOW_LENGTH)));
		index++;
	}
	if(right-left<this->WINDOW_LENGTH)
	{
		for(int i=0;i<right-left;i++)
			result.push_back(0);
	}
	result=this->zeroPadding(result);
	left+=16000/1000*10;
	right=left+WINDOW_LENGTH;	
	return result;
}
//记录2的次方的表
int twoPowerTable[14]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192};
vector<double> wavReadResult::zeroPadding(vector<double> &origin)
{
	if(origin.size()>twoPowerTable[13])
		{
			//fatal:origin's size is too large
			assert(1==0);
		}
	for(int i=0;i<14;i++)
	{		
		if(twoPowerTable[i]<origin.size()&&twoPowerTable[i+1]>origin.size())
			while(origin.size()<twoPowerTable[i+1])
				origin.push_back(0);
	}
	return origin;
}


void wavReadResult::DCTNormalize(void)
{
	vector<double> Mrecording(this->featuresLengthForcepstra,0);
	for(int i=0;i<this->resultsOfMFCC.size();i++)
		for(int j=0;j<this->featuresLengthForcepstra;j++)
			Mrecording[j]+=this->resultsOfMFCC[i].DCTresult[j];
	double temp=0;
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{
		for(int j=0;j<this->featuresLengthForcepstra;j++)
		{
			temp=this->resultsOfMFCC[i].DCTresult[j]-Mrecording[j]/this->resultsOfMFCC.size
				();
			this->resultsOfMFCC[i].NormalizedDCT.push_back(temp);
		}
	}
	this->varianceNormalization(Mrecording);
}
void wavReadResult::calcDelta(void)
{
	for(int i=2;i<resultsOfMFCC.size()-2;i++)
	{
		resultsOfMFCC[i].delta.resize(featuresLengthForcepstra*3);
		for(int j=0;j<featuresLengthForcepstra;j++)
		{
			resultsOfMFCC[i].delta[j]=resultsOfMFCC[i].NormalizedDCT[j];
			resultsOfMFCC[i].delta[j+featuresLengthForcepstra]=resultsOfMFCC[i+1].NormalizedDCT[j]
			-resultsOfMFCC[i-1].NormalizedDCT[j];
			resultsOfMFCC[i].delta[j+featuresLengthForcepstra*2]=resultsOfMFCC[i+2].NormalizedDCT[j]
			-resultsOfMFCC[i-2].NormalizedDCT[j];
		}
	}
}
void wavReadResult::varianceNormalization(vector<double> &vrecording)
{
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{
		for(int j=0;j<this->featuresLengthForcepstra;j++)
		{
			if(i==0)
				vrecording[j]=pow(resultsOfMFCC[i].NormalizedDCT[j],2);
			else
				vrecording[j]+=pow(resultsOfMFCC[i].NormalizedDCT[j],2);

			if(i==this->resultsOfMFCC.size()-1)//已经是加完所有结果了
			{
				vrecording[j]=sqrt(1.0*vrecording[j]/resultsOfMFCC.size());
			}
		}
	}
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{
		for(int j=0;j<this->featuresLengthForcepstra;j++)
		{			
			resultsOfMFCC[i].NormalizedDCT[j]=resultsOfMFCC[i].NormalizedDCT[j]/vrecording[j];
		}
	}
}

void wavReadResult::writeAllNormalToFile(string fileName)
{
	ofstream  outputFile;
	outputFile.open(fileName);		
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{			
		for(int j=0;j<this->resultsOfMFCC[i].NormalizedDCT.size();j++)
		{
			outputFile<<this->resultsOfMFCC[i].NormalizedDCT[j]<<" ";	
		}
		outputFile<<endl;	
	}
	outputFile<<flush;
	outputFile.close();
}
void wavReadResult::writeDeltaTofile(string fileName)
{
	ofstream  outputFile;
	outputFile.open(fileName);	
	for(int i=2;i<resultsOfMFCC.size()-2;i++)
	{
		for(int j=0;j<resultsOfMFCC[i].delta.size();j++)
			outputFile<<resultsOfMFCC[i].delta[j]<<" ";
		outputFile<<endl;
	}
	outputFile<<flush;
	outputFile.close();
}

void wavReadResult::writeAllLogMelTofile(string fileName)
{
	ofstream  outputFile;
	outputFile.open(fileName);	
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{
		//cout<<"here"<<endl;
		for(int j=0;j<this->resultsOfMFCC[i].outputOfFilter.size();j++)
		{
			//cout<<log(this->resultsOfMFCC[i].outputOfFilter[j])<<endl;
			outputFile<<log(this->resultsOfMFCC[i].outputOfFilter[j])<<" ";
		}
		outputFile<<endl;
	}
	outputFile<<flush;
	outputFile.close();
}


void wavReadResult::writeDFTPowerResultToFile(string fileName)
{
	ofstream  outputFile;
	outputFile.open(fileName);	
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{
		for(int j=0;j<this->resultsOfMFCC[i].powers.size();j++)
		{
			outputFile<<this->resultsOfMFCC[i].powers[j]<<",";//.magnitudes[j]<<" ";
		}
		outputFile<<endl;
	}
	outputFile<<flush;
	outputFile.close();
}


void wavReadResult::writeBeforeNormalizeToFile(string fileName)
{
	ofstream  outputFile;
	outputFile.open(fileName);	
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{
		for(int j=0;j<this->resultsOfMFCC[i].DCTresult.size();j++)
		{
			outputFile<<this->resultsOfMFCC[i].DCTresult[j]<<" ";
		}
		outputFile<<endl;
	}
	outputFile<<flush;
	outputFile.close();
}


void wavReadResult::writeMelToFile(string fileName)
{
	ofstream  outputFile;
	outputFile.open(fileName);	
	for(int i=0;i<this->resultsOfMFCC.size();i++)
	{
		for(int j=0;j<this->resultsOfMFCC[i].outputOfFilter.size();j++)
		{
			outputFile<<this->resultsOfMFCC[i].outputOfFilter[j]<<" ";
		}
		outputFile<<endl;
	}
	outputFile<<flush;
	outputFile.close();
}

vector<vector<double>> wavReadResult::getMFCCDelta()
{
	vector<vector<double>> res;
	for(int i=0;i<resultsOfMFCC.size();i++)
		if(resultsOfMFCC[i].delta.size()!=0)
			res.push_back(resultsOfMFCC[i].delta);
	return res;
}
