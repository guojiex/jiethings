#pragma once
#include <string>
#include <time.h>  
#include <fstream>
#include <vector>
#include <math.h>  
#include <assert.h>
#include "DFTresult.h"

#define DEBUG   (0)
/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (16000)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (500)
#define NUM_CHANNELS    (1)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) /**/
/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE   (1)

/* Select sample format. */
#if 0
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif
typedef struct paTestData
{
    int          frameIndex;  /* Index into sample array. */
    int          maxFrameIndex;
    SAMPLE      *recordedSamples;
	int			silenceFrames;
}paTestData;

class wavReadResult
{
public:		
	wavReadResult(string fileName);
	wavReadResult(SAMPLE *recordedSamples2);
	//wavReadResult(SAMPLE* samples,int size);
	wavReadResult(SAMPLE* samples,int size,string folder,string times);
	~wavReadResult(void);
private:
	vector<double> d;
	int sample_rate;//����Ƶ��	
public:		
	static const int WINDOW_LENGTH=16000/1000*25;//���ڳ��ȣ���λ:�㣩
	static const int featuresLengthForcepstra=13;//DCT��ȡ���ٸ�ϵ����Ҳ����������ά��
	
	void init(void);//��ʼ��
	int readAWav(string fileName);	
	
	vector<DFTresult> resultsOfMFCC;//������¼MFCC����������������
	//��v�����left��right��dft
	//DFTresult DFT(vector<double> v,int left,int right);	
	DFTresult DFT(vector<double> &v);	
	vector<double> Preemphasizing(vector<double> v);
	//���ò���Ƶ��
	bool setSampleRate(int rate);
	//Ԥ�Ŵ󣬴�����left��right
	//vector<double> Preemphasizing(vector<double> v, int left, int right, double alpha);
	//��left��right��v��window
	vector<double> windowing(vector<double> &v, int &left, int &right);

	static vector<double> readVectorFromFile(string fileName)
	{		 
		vector<double> v;
		string text;
		ifstream infile;
		infile.open(fileName,ios::in); 		
		int time=0;
		while(infile.good() && !infile.eof())
		{
			getline(infile, text, '\n');
			v.push_back(atof(text.data()));		
			if(v.at(v.size()-1)==0)
				v[v.size()-1]=0;			
		}
		infile.close();
		return v;
	}
	//bool writeVectorToFile(vector<double> v,string fileName)
	//{
	//	ofstream  outputFile;
	//	outputFile.open(fileName);
	//	
	//	for(int i=0;i<v.size();i++)
	//	{			
	//		outputFile<<v[i]<<endl;
	//	}
	//	outputFile<<flush;
	//	outputFile.close();
	//	return true;
	//}


	vector<double> zeroPadding(vector<double> &origin);
	void DCTNormalize(void);
	void writeAllNormalToFile(string fileName);
	void writeAllLogMelTofile(string fileName);
	void writeDFTPowerResultToFile(string fileName);
	void writeBeforeNormalizeToFile(string fileName);
	void writeMelToFile(string fileName);
	void varianceNormalization(vector<double> &vrecording);
	void calcDelta(void);
	void writeDeltaTofile(string fileName);
	vector<vector<double>> getMFCCDelta();
};
int CheckDir(const char* Dir);
