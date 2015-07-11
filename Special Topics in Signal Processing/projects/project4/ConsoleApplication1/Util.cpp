#include "stdafx.h"
#include "Util.h"
#include <string>
#include <sstream>

//************************************
// Method:    get the string form from an integer
// FullName:  Util::getstring
// Access:    public static 
// Returns:   std::string
// Parameter: const int & n
//************************************
std::string Util::getstring( const int &n )
{
	std::stringstream newstr;
	newstr<<n;
	return newstr.str();
}

//************************************
// Method:    judge if a char c is in the string str
// FullName:  Util::isCharInString
// Access:    public static 
// Returns:   bool
// Qualifier:
// Parameter: const char & c
// Parameter: const std::string & str
//************************************
bool Util::isCharInString( const char &c,const std::string &str )
{
	int size=str.length();
	for(int i=0;i<size;i++)
		if(c==str[i])
			return true;
	return false;
}

void util::showAVector( const std::vector<char> &show )
{
	for(int i=0;i<show.size();++i)
		if('a'<=show[i]||show[i]<='z'||show[i]=='\''||show[i]=='*'||show[i]=='-')
			printf(" %2c",show[i]);
		else
		{
			int a=0;
		}
		printf(" %d\n",show.size()-1);
}
void util::showAVector( const std::vector<parent_index_vector_type> &show )
{
	for(int i=0;i<show.size();++i)		
		printf("%3d",show[i]);		
	printf("\n");
}

void util::showAVector( const std::vector<bool> &show )
{
	for(int i=0;i<show.size();++i)		
		printf("%3d",show[i]?1:0);		
	printf("\n");
}

int util::getLengthOfAnInt( const int &value )
{
	return ceil(log10(value+1));
}

int util::compareSentences( const std::vector<std::vector<std::string>> &left,std::vector<std::vector<std::string>> &right )
{
	LOG(INFO)<<"passage size:"<<left.size()<<" passage size:"<<right.size();
	int different=0;
	assert(left.size()==right.size());
	for(int i=0;i<left.size();++i)
		different+=compareSentence(left[i],right[i]);
	return different;
}

int util::compareSentence( const std::vector<std::string> &left,const std::vector<std::string> &right )
{
	std::vector<std::vector<unsigned int>> matrix;
	matrix.resize(left.size());
	for(int i=0;i<left.size();++i)
		matrix[i].resize(right.size(),0);
	int temp=0;
	matrix[0][0]=(left[0]==right[0]?0:1);
	for(int i=1;i<left.size();++i)
	{
		matrix[i][0]=matrix[i-1][0]+1;			
	}
	for(int j=1;j<right.size();++j){
		for(int i=0;i<left.size();++i)
		{
			if (i==0)
			{
				matrix[i][j]=matrix[i][j-1];
			}else
			{
				temp=min(matrix[i-1][j]+1,matrix[i][j-1]+1);
				/*if(left[i]==right[j])
					printf("%s %s\n",left[i].c_str(),right[j].c_str());*/

				temp=(left[i]==right[j]?min(matrix[i-1][j-1],temp):min(matrix[i-1][j-1]+1,temp));
				matrix[i][j]=temp;
			}
		}
	}
	return matrix[left.size()-1][right.size()-1];
}
