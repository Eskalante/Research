#pragma once

#include <string>
#include <iostream>
#include <time.h>

using namespace std;

typedef unsigned int uint;

class debug {
	// UTIL
public:
	static void error(uint iLevel, string strReport) {
		if (siDebugLevel >= iLevel) {
			cout << getTimeString();
			for (register uint i = 0; i < iLevel; i++) {
				cout << "\t";
			}
			cout << "Error:\t" << strReport << endl;
		}
	}
	// ACC
	static void setDebugLevel(uint iDebugLevel) {
		siDebugLevel = iDebugLevel;
	}
	static uint getDebugLevel() {
		return siDebugLevel;
	}
	// ATR
private:
	static uint siDebugLevel;
	// HELP
private:
	static string getTimeString() {
		time_t rawtime;
		struct tm * timeinfo = new tm();
		time(&rawtime);
		localtime_s(timeinfo, &rawtime);
		char text[256];
		sprintf_s(text, "%04d-%02d-%02d %02d:%02d:%02d ", timeinfo->tm_year+1900, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		return string(text);
	}
};