#pragma once
#include <vector>
#include <string>
typedef std::vector<std::vector<double>> DTW_frame;
class DTW
{
private:
	std::vector<std::vector<double>> matrixScore;	
	void initMatrix(int row,int col);
	
	DTW_frame templatesForSynchronous;
public:		
	std::vector<std::vector<DTW_frame>> templates;
	struct DTW_result
	{
		int minIndex;
		double minRes;
	};
	DTW(void);
	~DTW(void);
	double DTWforCost(std::vector<std::vector<double>> &leftTemplate,std::vector<std::vector<double>> &data);	
	DTW_frame readAdeltaFromFile(const std::string fileName);
	double P(int i,int j);

	//************************************
	// Method:    把某一个数字的模板delta文件全部读入到数组toadd中，如numberOfTemplate是0时，就把
	//				zero的所有例子读入toadd
	// FullName:  DTW::getAllInstancesOfTemplate
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: std::vector<DTW_frame> & toadd
	// Parameter: int numberOfTemplate
	//************************************
	void getAllInstancesOfTemplate(std::vector<DTW_frame> &toadd,int numberOfTemplate);

	//************************************
	// Method:    使用第templateIndex个模板列表对数据进行识别
	//				数据行从dataIndexeLeft到dataIndexRight(zeros ones等)
	//				数据列从left到right（0 1 2 等）
	// FullName:  DTW::getBestCostDTWCorrectRateFromSingleTemplate
	// Access:    public 
	// Returns:   double
	// Qualifier:
	// Parameter: int templateIndex
	// Parameter: int dataIndexLeft
	// Parameter: int dataIndexRight
	// Parameter: int left
	// Parameter: int right
	//************************************
	DTW_result getBestCostDTWCorrectRateFromSingleTemplate(int templateIndex, int dataIndexLeft, int dataIndexRight,int left,int right);
	
	DTW_result getBestCostDTWForOne(int templateIndex,DTW_frame &data);

	//************************************
	// Method:    把所有模板叠在一起的DTW
	// FullName:  DTW::timeSynchronousDTWforCost
	// Access:    public 
	// Returns:   double
	// Qualifier:
	// Parameter: int templatesIndex 使用的模板序号
	// Parameter: std::vector<std::vector<double>> & data 要对比的数据Input
	//************************************
	DTW_result timeSynchronousDTWforCost(int templatesIndex,std::vector<std::vector<double>> &data);	
	DTW_result getCostTimeSynchronousDTWCorrectRateFromSingleTemplate(int templateIndex, int dataIndexLeft, int dataIndexRight,int left,int right);
	void getSynchronousDTWTemplate(int templatesIndex);
	double timeSynchronousDTWforCost2(int templatesIndex,std::vector<std::vector<double>> &data);
	void removeAlignmentVectorFromTemplate(int templatesIndex);
	void testMultiTemplateDTW(int dataIndexLeft, int dataIndexRight,int left,int right);
	void testMultiTemplateTimeSynchronousDTW(int dataIndexLeft, int dataIndexRight,int left,int right);
	inline void testTimeSynchronousDTWforCost( int templateIndex,int dataIndexLeft, int dataIndexRight,int left,int right )
	{
		this->getCostTimeSynchronousDTWCorrectRateFromSingleTemplate(templateIndex,dataIndexLeft,dataIndexRight,left,right);
	}
	inline void testSingleTemplateDTWforCost(int templateIndex,int dataIndexLeft, int dataIndexRight,int left,int right)
	{	
		this->getBestCostDTWCorrectRateFromSingleTemplate(templateIndex,dataIndexLeft,dataIndexRight,left,right);
	}
	inline int getTotalFrames()
	{
		int size=0;
		int total=0;
		for(int i=0;i<10;++i)
		{
			size=templates[i].size();
			for(int j=0;j<size;++j)
				total+=templates[i][j].size();
		}
		return total;
	}
	inline int getTotalInstancesOfSample()
	{
		int size=0;
		int total=0;
		for(int i=0;i<10;++i)
		{
			total+=templates[i].size();
		}
		return total;
	}
	DTW::DTW_result timeSynchronousDTWPruning(const double threshold ,int templatesIndex,std::vector<std::vector<double>> &data );
	DTW::DTW_result testCostTimeSynchronousDTWCorrectRatePrunning(double thre, int dataIndexLeft, int dataIndexRight,int left,int right );	
};
std::vector<std::vector<double>> averageTemplate(std::vector<std::vector<double>> &left,std::vector<std::vector<double>> &data);