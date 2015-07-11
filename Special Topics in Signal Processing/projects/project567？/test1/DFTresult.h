#include "wavReadResult.h"
#include "FFT.h"
#include <iostream>
#define PI 3.1416
#define SAMPLE_RATE  (16000)
#define BANKNUM (40)
using namespace std; 
const double eps=0.1e-10;

class DFTresult
{
public:	
	typedef double ElementType; //原始数据序列的数据类型,可以在这里设置   
	typedef struct complex             //复数结构体,用于实现傅里叶运算   
	{  
		ElementType real,imag;  
	}complex;
	vector<double> datasource;
	vector<double> magnitudes;
	vector<double> powers;	
	complex* values;//dct运算出来的结果
	vector<double> outputOfFilter;
	int resultSize;
	vector<double> delta;
	int windowPoints;//一个window中有多少个点	
	double round(double r)
	{
		return (r>0.0)?floor(r+0.5):ceil(r-0.5);
	}
	DFTresult(vector<double> &v,int left,int right,int windowPoints)//get the DFT of vector v's left to right,not including right
	{	
		for(int i=left;i<right;i++)
			datasource.push_back(v[i]);
		this->init(windowPoints);
		DFT();
	}
	void init(int windowPoints)
	{
		this->windowPoints=windowPoints;
		this->resultSize=windowPoints/2+1;		
		values=new complex[resultSize];
	}
	DFTresult(vector<double> v,int windowPoints)
	{
		datasource=vector<double>(v);	
		this->init(windowPoints);
		DFT();
		this->CalMels(BANKNUM);
	}
	void DFT(void)//DFT离散傅里叶变换
	{		
		//for(int i=0;i<this->datasource.size()/2+1;i++)
		//{			
		//	DFT_oneNode(i);		
		//	magnitudes.push_back(sqrt(values[i].real*values[i].real+values[i].imag*values[i].imag));
		//	powers.push_back(magnitudes[i]*magnitudes[i]);//values[i].real*values[i].real+values[i].imag*values[i].imag;
		//}
		FFT res;
		double* xr=new double[this->windowPoints];
		double* resultR=new double[this->windowPoints];
		double* resultI=new double[this->windowPoints];
		for(int i=0;i<this->windowPoints;i++)
		{
			assert(this->windowPoints==this->datasource.size());
			xr[i]=this->datasource[i];
		}
		res.rFFT_1D(xr,resultR,resultI,this->datasource.size());
		for(int i=0;i<this->resultSize;i++)
		{
			values[i].real=resultR[i];
			values[i].imag=resultI[i];
			magnitudes.push_back(sqrt(values[i].real*values[i].real+values[i].imag*values[i].imag));
			powers.push_back(magnitudes[i]*magnitudes[i]);
		}
		delete[]xr;
		delete[]resultR;
		delete[]resultI;
	}
	typedef struct melBank//复数结构体,用于实现mel bank 
	{  
		vector<int> left;//这三个vector里面是三角形滤波器的下线中线上线对应的点
		vector<int> middle;
		vector<int> right; 
		vector<double> output;
	}melBank;
	vector<double> DCTresult;//DCT离散余弦变换结果
	vector<double> NormalizedDCT;//归一化后的结果
	void DCT(int dctCofNum,int bankNum)
	{
		this->DCTresult.clear();
		for(int i=0;i<dctCofNum;i++)
		{
			this->DCTresult.push_back(sqrt(1.0/bankNum)*DCTforOne(i,bankNum));
		}
	}
	double DCTforOne(int n,int bankNum)
	{
		double result=0;
		double temp=0;
		double b=0;
		for(int i=0;i<bankNum;i++)
		{			
			temp=(2*i+1)*(PI*n/(2*bankNum));
			b=log(outputOfFilter[i]);
			if(outputOfFilter[i]==0)
				b=0;
			result=result+b*cos(temp);				
		}
		return result;
	}
	double Woutput(double k,melBank mel,int l)
	{		
		double result=0;
		double result2=0;

		if(getPointFre(mel.left[l])<=k&&k<=getPointFre(mel.middle[l]))
		{
			result=k-getPointFre(mel.left[l]);
			result2=getPointFre(mel.middle[l])-getPointFre(mel.left[l]);	
			if(result2<eps&&result2>-eps)
				return 0;
			result=result/result2;	
		}
		if(getPointFre(mel.middle[l])<=k && k<=getPointFre(mel.right[l]))
		{
			result=getPointFre(mel.right[l])-k;
			result2=getPointFre(mel.right[l])-getPointFre(mel.middle[l]);
			if(result2<eps&&result2>-eps)
				return 0;
			result=result/result2;
		}
		else
			result=0;		
		return result;
	}
	//void CalcFilterBankOutputAndLog(melBank mel,int bankNum)
	//{		
	//	double sum=0;
	//	for(int l=0;l<bankNum;l++)
	//	{
	//		sum=0;			
	//		for(int k=0;k<powers.size();k++)
	//		{	
	//			if(k<mel.left[l])
	//				continue;
	//			if(k>mel.right[l])
	//				break;
	//			sum=sum+Woutput(getPointFre(k),mel,l)*powers[k];	
	//		}			
	//		/*for(int k=mel.left[l];k<=mel.right[l];k++)
	//		{
	//		sum+=Woutput(k,mel,l)*powers[k];
	//		}*/
	//		this->outputOfFilter.push_back(sum);
	//	}		
	//}
	bool writeNormalAfterDCTtoFile()
	{
		if(this->NormalizedDCT.size()!=0)
		{
			double* output=this->convertVectorDoubleToArray(this->NormalizedDCT);
			this->writeArrayToFile(output,"Normal.txt",this->DCTresult.size());
			delete []output;
			return true;
		}else
			return false;
	}
	void writeDCTresultToFile()
	{
		double* output=this->convertVectorDoubleToArray(this->DCTresult);
		this->writeArrayToFile(output,"DCT.txt",this->DCTresult.size());
		delete []output;
	}
	void writeLogMelToFile()
	{
		double* output=this->convertVectorDoubleToArray(this->outputOfFilter);
		for(unsigned int i=0;i<this->outputOfFilter.size();i++)
		{
			output[i]=log(output[i]);
		}		
		this->writeArrayToFile(output,"LogoutputFilterBank.txt",this->outputOfFilter.size());
		delete[] output;
	}
	void writeFilterBankOutputToFile()
	{
		double* output=this->convertVectorDoubleToArray(this->outputOfFilter);
		this->writeArrayToFile(output,"outputFilterBank.txt",this->outputOfFilter.size());
		delete[] output;
	}

	void CalMels(int bankNum)//计算mel滤波器相关
	{		
		vector<double> dftFreqs;
		for(int i=0;i<=this->windowPoints/2;i++)
		{
			dftFreqs.push_back((double)i/windowPoints*SAMPLE_RATE);
		}
		double minmel=freToMel(133.33);
		double maxmel=freToMel(6855.4976);
		double wts[BANKNUM+1][512];
		vector<double> binfrqs;
		double melWidth=(maxmel-minmel)/(bankNum+1);
		for(int i=0;i<=bankNum+1;i++)
		{
			double melF=(minmel+(double)i/(bankNum+1)*(maxmel-minmel));
			melF=melTofreq(melF);
			binfrqs.push_back(melF);
		}
		vector<double> binbin;		
		for(unsigned int i=0;i<binfrqs.size();i++)
			binbin.push_back(round(binfrqs[i]/SAMPLE_RATE*(windowPoints-1)));		
		for(int i=0;i<bankNum;i++)
		{			
			double fs1=binfrqs[i];
			double fs2=binfrqs[i+1];
			double fs3=binfrqs[i+2];		

			vector <double> r1;
			vector <double> r2;
			for(unsigned int j=0;j<dftFreqs.size();j++)
			{				
				if(dftFreqs[j]<fs1||dftFreqs[j]>fs3)
					wts[i][j]=0;
				else if(fs1<=dftFreqs[j]&&dftFreqs[j]<=fs2)
				{
					double temp=(dftFreqs[j]-fs1)/(fs2-fs1);
					wts[i][j]=temp;
				}
				else if(fs2<dftFreqs[j]&&dftFreqs[j]<=fs3)
				{
					double temp=(fs3-dftFreqs[j])/(fs3-fs2);
					wts[i][j]=temp;
				}
			}
		}
		for(int i=0;i<bankNum;i++)
		{
			for(int j=258;j<512;j++)
			{
				wts[i][j]=0;
			}			
		}
		for(int i=0;i<bankNum;i++)
		{
			for(int j=0;j<512;j++)
			{
				if(wts[i][j]<0)
					wts[i][j]=0;
			}			
		}
		/*ofstream  outputFile;
		outputFile.open("wts.txt");	
		for(int i=0;i<bankNum;i++)
		{
		for(int j=0;j<512;j++)
		{
		outputFile<<wts[i][j]<<" ";
		}
		outputFile<<endl;
		}
		outputFile<<flush;
		outputFile.close();*/
		double result=0;
		for(int i=0;i<BANKNUM;i++)
		{
			result=0;
			for(int k=0;k<powers.size();k++)
			{
				result+=this->powers[k]*wts[i][k];
			}
			this->outputOfFilter.push_back(result);
		}		
	}

	void writeMagnitudesToFile()
	{
		double* output=this->magnitudes.data();
		this->writeArrayToFile(output,"magnitudes.txt",this->resultSize);
	}
	double* convertVectorDoubleToArray(vector<double> &v)
	{
		double* res=new double[v.size()];
		for(unsigned int i=0;i<v.size();i++)
			res[i]=v[i];
		return res;
	}
	void writePowersToFile()
	{
		cout<<this->resultSize<<endl;
		double* output=this->powers.data();
		this->writeArrayToFile(output,"powers.txt",this->resultSize);
	}
	bool writeArrayToFile(double* array,string fileName,int size)
	{
		ofstream  outputFile;
		outputFile.open(fileName);

		for(int i=0;i<size;i++)
		{			
			outputFile<<array[i]<<endl;
		}
		outputFile<<flush;
		outputFile.close();
		return true;
	}
	double getPointFre(int pointPos)//得到dft结果x轴各点的代表频率值
	{
		return SAMPLE_RATE/(this->windowPoints-1)*pointPos;
	}
	void DFT_oneNode(int k)
	{
		complex ResultThisNode;
		ResultThisNode.real = 0;  
		ResultThisNode.imag = 0;
		for(unsigned int n=0; n<this->datasource.size(); n++)  
		{  		
			complex temp;
			temp.real=cos(2*PI/this->datasource.size()*k*n)*datasource[n];//运用欧拉公式把复数拆分成实部和虚部
			temp.imag=sin(2*PI/this->datasource.size()*k*n)*datasource[n];          
			ResultThisNode.real += temp.real;  
			ResultThisNode.imag += temp.imag;  
		}  
		values[k].real=ResultThisNode.real;
		values[k].imag=ResultThisNode.imag;
	}
	~DFTresult(void){
		//delete[]this->values;
	}
	int getMaxMelFre()//得到最大mel频率
	{
		return 2595*log10(SAMPLE_RATE/2/700+1);
	}	
	double melTofreq(double mel)
	{
		double res=700*((pow(10,mel/2595))-1);
		return res;
	}
	double freToMel(double f)
	{
		return 2595*log10(f/700+1);
	}	

	void CalcFilterBankOutputAndLog2(melBank mel, int bankNum)
	{

	}
};

