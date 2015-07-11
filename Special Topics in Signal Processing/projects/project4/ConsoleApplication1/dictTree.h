#pragma once
#include "stdafx.h"
#include "dictNode.h"
typedef unsigned int parent_index_vector_type;

template<class T>
///字典树，成员变量有根节点地址，词数量记录
class dictTree
{
private:
	dictNode<T>* _Root;		
	int wordCount;	
	void getDictTemplateVector(std::vector<T> &templateVector,dictNode<T>* node);
	void getIndexTemplateVector(std::vector<parent_index_vector_type> &intTemplateVector,dictNode<T>* node,const int &parent,const int &current);
	void getEndTemplateVector(std::vector<bool> &endTemplateVector,dictNode<T>* node);
	void drawStorageTree(dictNode<T>* node);
public:
	int size() const { return wordCount; }
	dictNode<T>* Root() const { return _Root; }
	void Root(dictNode<T>* val) { _Root = val; }
	dictTree<T>(void);
	~dictTree<T>(void);
	void addAWord(T* word,const int size);	
	void addAWord(const T* word,const int &size);
	void drawStorageTree();	
	std::vector<T> getDictTemplateVector();	
	std::vector<parent_index_vector_type> getIndexTemplateVector();
	std::vector<bool> getEndTemplateVector();
	int readWordsFromStringVector(const std::vector<std::string> &dict);
};

template<class T>
std::vector<bool> dictTree<T>::getEndTemplateVector()
{
	std::vector<bool> res;
	this->getEndTemplateVector(res,_Root);
	return res;
}

//************************************
// Method:    得到整棵树里面某一个字符是否为结束的一个bool vector
// FullName:  dictTree<T>::getEndTemplateVector
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: std::vector<bool> & endTemplateVector
// Parameter: dictNode<T> * node
//************************************
template<class T>
void dictTree<T>::getEndTemplateVector(std::vector<bool> &endTemplateVector,dictNode<T>* node)
{
	if(node->IsEndOfWord())
		endTemplateVector.push_back(true);
	else
	{
		endTemplateVector.push_back(false);
	}
	if (node->IsEndOfWord())
	{				
		if(node->getChildrenLength()==0)
			return;
		else
		{
			for(int i=0;i<node->getChildrenLength();++i)
				getEndTemplateVector(endTemplateVector,node->childAt(i));
		}
	}else
	{
		for(int i=0;i<node->getChildrenLength();++i)
			getEndTemplateVector(endTemplateVector,node->childAt(i));
	}
}

//************************************
// Method:   从字符串数组中读取所有的词到dict tree
// FullName:  dictTree<>::readWordsFromStringVector
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const std::vector<std::string> & dict
//************************************
template<>
int dictTree<char>::readWordsFromStringVector( const std::vector<std::string> &dict )
{
	assert(!dict.empty());
	if(dict.empty())
		return 0;
	int size=dict.size();	
	for(int i=0;i<size;++i)
		this->addAWord(dict[i].c_str(),dict[i].size());
	return this->wordCount;
}

template<class T>
void dictTree<T>::getDictTemplateVector(std::vector<T> &templateVector,dictNode<T>* node)
{
	templateVector.push_back(node->getValue());
	if (node->IsEndOfWord())
	{				
		if(node->getChildrenLength()==0)
			return;
		else
		{
			for(int i=0;i<node->getChildrenLength();++i)
				getDictTemplateVector(templateVector,node->childAt(i));
		}
	}else
	{
		for(int i=0;i<node->getChildrenLength();++i)
			getDictTemplateVector(templateVector,node->childAt(i));
	}
}

template<class T>
std::vector<T> dictTree<T>::getDictTemplateVector()
{
	std::vector<T> res;
	getDictTemplateVector(res,_Root);
	return res;
}

template<class T>
std::vector<parent_index_vector_type> dictTree<T>::getIndexTemplateVector()
{
	std::vector<parent_index_vector_type> res;
	getIndexTemplateVector(res,_Root,0,0);
	return res;
}

//************************************
// Method:    get the parent index of the template vector from recusion
// FullName:  dictTree<T>::getIndexTemplateVector
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: std::vector<int> & intTemplateVector
// Parameter: dictNode<T> * node
// Parameter: const int & parent
// Parameter: const int & current
//************************************
template<class T>
void dictTree<T>::getIndexTemplateVector(std::vector<parent_index_vector_type> &intTemplateVector,dictNode<T>* node,const int &parent,const int &current)
{
	intTemplateVector.push_back(parent);
	if (node->IsEndOfWord())
	{				
		if(node->getChildrenLength()==0)
			return;
		else
		{
			for(int i=0;i<node->getChildrenLength();++i)
				getIndexTemplateVector(intTemplateVector,node->childAt(i),current,intTemplateVector.size());
		}
	}else
	{
		for(int i=0;i<node->getChildrenLength();++i)
			getIndexTemplateVector(intTemplateVector,node->childAt(i),current,intTemplateVector.size());
	}
}

//************************************
// Method:    print out every node's value of a tree
// FullName:  dictTree<>::drawStorageTree
// Access:    public 
// Returns:   void
//************************************
template<>
void dictTree<char>::drawStorageTree()
{
	drawStorageTree(this->_Root);
}

template<>
void dictTree<char>::drawStorageTree(dictNode<char>* node)
{
	printf("%c ",node->getValue());
	if (node->IsEndOfWord())
	{		
		printf("\n");
		if(node->getChildrenLength()==0)
			return;
		else
		{
			for(int i=0;i<node->getChildrenLength();++i)
				drawStorageTree(node->childAt(i));
		}
	}else
	{
		for(int i=0;i<node->getChildrenLength();++i)
			drawStorageTree(node->childAt(i));
	}
}

template<class T>
void dictTree<T>::addAWord( T* word,const int size )
{
	if(size==0)
		return;
	++this->wordCount;
	dictNode<T>* temp=_Root;
	for(int i=0;i<size;++i)
	{
		//if(word[i]!='\0')
		temp=temp->addAChild(word[i],i==size-1?true:false);
	}
}

//************************************
// Method:    add a word to the tree
// FullName:  dictTree<T>::addAWord
// Access:    public 
// Returns:   void
// Parameter: const T * word
// Parameter: const int & size
//************************************
template<class T>
void dictTree<T>::addAWord( const T* word,const int &size )
{
	if(size==0)
		return;
	++this->wordCount;
	dictNode<T>* temp=_Root;
	for(int i=0;i<size;++i)
	{
		temp=temp->addAChild(word[i],i==size-1?true:false);
	}
}

template<class T>
dictTree<T>::dictTree( void )
{
	this->_Root=new dictNode<T>('*',true,false);
	this->wordCount=0;
}
template<class T>
dictTree<T>::~dictTree( void )
{
	_Root->destroy(_Root);
}

