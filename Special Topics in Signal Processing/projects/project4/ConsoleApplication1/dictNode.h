#pragma once
#include "stdafx.h"
#include <vector>
template <class T>
class dictNode
{
private:
	std::vector<dictNode<T>*> children;
	bool isRoot;	
	T ch;
	bool isEndOfWord;
public:	
	void destroy(dictNode<T>* node);
	bool IsEndOfWord() const { return isEndOfWord; }
	void IsEndOfWord(bool val) { isEndOfWord = val; }
	bool IsRoot() const { return isRoot; }
	void IsRoot(bool val) { isRoot = val; }
	int getChildrenLength(){return children.size();}
	dictNode<T>* childAt(const int &index)
	{
		assert(index<getChildrenLength());
		return this->children[index];
	}
	T getValue(){return ch;}
	dictNode<T>(T ch,bool isRoot,bool isEndOfWord):
		ch(ch),isRoot(isRoot),isEndOfWord(isEndOfWord){}
	dictNode<T>* addAChild(T ch,bool isEnd);
	~dictNode(void);
	//dictNode<T>* addAnEndLeaf( T ch);	
};

template <class T>
void dictNode<T>::destroy(dictNode<T>* node)
{
	if(node->getChildrenLength()!=0)
		for (int i=0;i<node->getChildrenLength();++i)
		{
			destroy(node->children[i]);
		}
	if(node!=nullptr)
	{
		//printf("del:%c ",node->getValue());
		delete node;
	}
}

//template <class T>
//dictNode<T>* dictNode<T>::addAnEndLeaf( T ch)
//{
//	dictNode<T>* temp=new dictNode<T>('~',false,true);
//	this->children.push_back(temp);
//	return temp;
//}

template <class T>
dictNode<T>* dictNode<T>::addAChild( T ch,bool isEnd )
{
	/*if(ch=='\0')
		return this;	*/
	unsigned int size=children.size();
	for(unsigned int i=0;i<size;++i)
		if(children[i]->getValue()==ch)
			return children[i];
	dictNode<T>* temp=new dictNode<T>(ch,false,isEnd);
	this->children.push_back(temp);
	/*if(isEnd)
	{
	temp->addAnEndLeaf('~');
	}*/
	return temp;
}

template <class T>
dictNode<T>::~dictNode( void )
{

}

