#pragma once
#include "stdio.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
enum logLevel {debug,warn,error};

typedef struct messageType
{
	struct tm	ptr; 
	string		content;
	logLevel	level;
}logMessage;
class myLog
{
private:	
	vector<logMessage> message;	
	logMessage createAMessage(string text, logLevel level);	
	string createMessageOutString(logMessage message);
	ofstream outputFile;
	bool printOnScreen;	
public:			
	bool PrintOnScreen() const { return printOnScreen; }
	void PrintOnScreen(bool val) { printOnScreen = val; }
	myLog(void);
	myLog(string fileName);
	~myLog(void);	
	bool warn(string text);
	bool debug(string text);
	bool error(string text);
	bool finish(void);
	void finishNoDebug(void);	
};

