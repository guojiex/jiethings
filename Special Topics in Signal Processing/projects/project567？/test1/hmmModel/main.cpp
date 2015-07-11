#include <iostream>
#include <string>
#include <vector>
#include "templateList.h"
#include "hmmModel.h"
#include "hmmTool.h"

using namespace std;

enum TestType {
	viterbiSearch, forwardAlgo
};

#define SEGMENT_NUMBER 7        //no more than 10
#define TEMPLATE_NUMBER_IN_MODEL 5  
#define MODEL_NUMBER 10
#define TEST_CASE_NUMBER 10
#define NCEP 39                // no more than 40
TestType selectAlgo = viterbiSearch;


string filePath = "C:\\Users\\jack\\Documents\\Visual Studio 2013\\Projects\\SRProject1\\SRProject4\\data\\";
string hmmPath = filePath + "one.hmm";
string fileNames[] = { "one", "two","three","four","five","six","seven","eight","nine","ten" };
string indexs[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
HMMModel hmmModels[10];

void initHMMModel() {
	for (int i = 0; i < MODEL_NUMBER; i++) {
		string singleFilePath = filePath + fileNames[i] + ".hmm";
		hmmModels[i].readHMMModel(singleFilePath);
	}
}

int testSingleNumber(int number, int index, int type) {
	Template singleTemplate;
	string singleFilePath = filePath + fileNames[number] + "-" + indexs[index] + ".mfcc";
	singleTemplate.readTemplate(singleFilePath);

	int bestIndex = 0;
	if (type == forwardAlgo) {
		double maxProb = -1e20;
		for (int i = 0; i < MODEL_NUMBER; i++) {
			double prob = getHMMCompareProb(hmmModels[i], singleTemplate);
			if (maxProb < prob) {
				maxProb = prob;
				bestIndex = i;
			}
		}
	}
	else if (type == viterbiSearch) {
		double minCost = 1e20;
		for (int i = 0; i < MODEL_NUMBER; i++) {
			double cost = doViterbiSearch(hmmModels[i], singleTemplate);
			if (minCost > cost) {
				minCost = cost;
				bestIndex = i;
			}
		}
	}

	cout << "Test the number: " << fileNames[number] << endl;
	cout << "The result: " << fileNames[bestIndex] << endl;
	cout << endl;

	if (number == bestIndex) {
		return 1;
	}
	return 0;
}

int main() {

	int cmd;
	cout << "--------------min dist ----------------\n";
	cout << "1. Build the HMM Model, single guassian" << endl;
	cout << "2. Build the HMM Model, k-means" << endl;
	cout << "3. Test one template" << endl;
	cout << "4. Test all templates" << endl;
	cout << "5. quit" << endl;
	cin >> cmd;
	while (true) {
		if (cmd == 1) {
			for (int i = 0; i < MODEL_NUMBER; i++) {
				TemplateList templates(TEMPLATE_NUMBER_IN_MODEL, SEGMENT_NUMBER, NCEP);
				vector<string> files;
				for (int j = 0; j < TEMPLATE_NUMBER_IN_MODEL; j++) {
					files.push_back(filePath + fileNames[i] +  "-" + indexs[j] + ".mfcc");
				}

				templates.readMutiTemplate(files);
				HMMModel hmmmodel = templates.buildHMMModel(HMMModel::singleType);
				hmmmodel.writeHMMModel(filePath + fileNames[i] + ".hmm");
				cout << filePath + fileNames[i] + ".hmm" + " has done!" << endl;
			}
		}
		else if (cmd == 2) {
			for (int i = 0; i < MODEL_NUMBER; i++) {
				TemplateList templates(TEMPLATE_NUMBER_IN_MODEL, SEGMENT_NUMBER, NCEP);
				vector<string> files;
				for (int j = 0; j < TEMPLATE_NUMBER_IN_MODEL; j++) {
					files.push_back(filePath + fileNames[i] + "-" + indexs[j] + ".mfcc");
				}

				templates.readMutiTemplate(files);
				HMMModel hmmmodel = templates.buildHMMModel(HMMModel::kmeansType);
				hmmmodel.writeHMMModel(filePath + fileNames[i] + ".hmm");
				cout << filePath + fileNames[i] + ".hmm" + " has done!" << endl;
			}

		}
		else if (cmd == 3) {
			initHMMModel();

			int testIndex = 0;
			cout << "Please input the number to test" << endl;
			cin >> testIndex;
			testSingleNumber(testIndex, 0, selectAlgo);
		}
		else if (cmd == 4) {
			initHMMModel();

			int right = 0;
			int total = 0;
			for (int i = 0; i < MODEL_NUMBER; i++) {
				for (int j = 0; j < TEST_CASE_NUMBER; j++) {
					right += testSingleNumber(i, j, selectAlgo);
					total += 1;
				}
			}

			cout << endl;
			cout << "Right testcase: " << right << endl;
			cout << "Total testcase: " << total << endl;
			cout << "Right probility: " << right / (double)total << endl;
		}
		else if (cmd == 5) {
			break;
		}

		cin >> cmd;
	};

	return 0;
}