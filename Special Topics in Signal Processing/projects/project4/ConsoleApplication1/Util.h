#pragma once
#include <string>
#include <vector>
class Util
{
public:
	static std::string getstring ( const int &n );
	static bool isCharInString(const char &c,const std::string &str);
};
namespace util{
	typedef unsigned int parent_index_vector_type;
	void showAVector(const std::vector<char> &show);
	//void showAVector(const std::vector<int> &show);
	void showAVector(const std::vector<bool> &show);
	void showAVector( const std::vector<parent_index_vector_type> &show );
	int getLengthOfAnInt(const int &value);	
	int compareSentences(const std::vector<std::vector<std::string>> &left,std::vector<std::vector<std::string>> &right);
	int compareSentence(const std::vector<std::string> &left,const std::vector<std::string> &right);
}
