#include "myLog.h"

myLog::myLog(void)
{
	this->outputFile.open("myLog.txt",ios::app);
	outputFile<<endl<<"start writing log file."<<endl;
	this->printOnScreen=false;
}


myLog::myLog(string fileName)
{
	this->outputFile.open(fileName,ios::app);
	outputFile<<endl<<"start writing log file."<<endl;
	this->printOnScreen=false;
}


myLog::~myLog(void)
{
}


logMessage myLog::createAMessage(string text, logLevel level)
{
	logMessage logM;
	time_t lt; 
	lt=time(NULL);
	localtime_s(&logM.ptr, &lt);
	logM.content=text;
	logM.level=level;
	return logM;
}


string myLog::createMessageOutString(logMessage message)
{
	string res;
	char str[100]; 
	strftime(str,100,"%Y%m%d %H:%M:%S:",&message.ptr);
	res.append(str);
	if(message.level==logLevel::debug)
		res.append("debug:");//.append(message.content);
	else if(message.level==logLevel::warn)
		res.append("warn: ");
	else if(message.level==logLevel::error)
		res.append("error:");
	else
	{
		res.append("error log type error");
	}
	res.append(message.content);
	return res;
}


bool myLog::debug(string text)
{
	this->message.push_back(this->createAMessage(text,logLevel::debug));
	return false;
}


bool myLog::warn(string text)
{
	this->message.push_back(this->createAMessage(text,logLevel::warn));
	return true;
}


bool myLog::error(string text)
{
	this->message.push_back(this->createAMessage(text,logLevel::error));
	return false;
}


bool myLog::finish(void)
{
	if(!this->outputFile.is_open())
		return false;
	for(int i=0;i<this->message.size();i++)
		outputFile<<createMessageOutString(this->message[i]).c_str()<<endl;
	if(this->printOnScreen)
		for(int i=0;i<this->message.size();i++)
			cout<<createMessageOutString(this->message[i]).c_str()<<endl;
	outputFile<<"end of writing log file."<<endl;
	this->outputFile.close();
	return true;
}


void myLog::finishNoDebug(void)
{
	if(!this->outputFile.is_open())
		return;
	for(int i=0;i<this->message.size();i++)
	{
		if(this->message[i].level!=logLevel::debug)
			outputFile<<createMessageOutString(this->message[i]).c_str()<<endl;
		else
			continue;
	}
	if(this->printOnScreen)
		for(int i=0;i<this->message.size();i++)
		{
			if(this->message[i].level!=logLevel::debug)
				cout<<createMessageOutString(this->message[i]).c_str()<<endl;
			else
				continue;
		}
	outputFile<<"end of writing log file."<<endl;
	this->outputFile.close();
	//return false;
}
