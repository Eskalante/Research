#pragma once

#include "include.h"
// parts
#include "parserFileTree.h"
#include "comparatorEuclidean.h"

// application
class app {
	// C&D
public:
	app() :iPic(0), iMet(0), pmTargets(0), pmPredicts(0), pvvResults(0), pcParser(0), pcComparator(0) {

	}
	virtual ~app() {
		term();
	}
	// SYS
public:
	bool init(string strSettingsPath, int iNumoPictures, int iNumoMethods) {
		iPic = iNumoPictures;
		iMet = iNumoMethods;
		// init databanks
		pmTargets = new Mat();
		pmPredicts = new vector<Mat>();
		pvvResults = new Mat(iPic, iMet, CV_32F);
		// setup parser
		pcParser = new parserFileTree();
		if (pcParser->init(strSettingsPath, iPic, iMet)) {
			cout << "At FILE: app.cpp\tLINE:29\t\t";
			cout << "Unable to initialize parser." << endl;
			return 1;
		}
		// setup processor
		pcComparator = new comparatorEuclidean();
		// setup evaluator
		/*
		pcEval = new evalDefault();
		*/
		return 0;
	}
	bool cycle() {
		// parse data
		cout << "Parser START" << endl;
		pcParser->parseData(pmTargets, pmPredicts);
		// process data
		cout << "Processor START" << endl;
		pcComparator->process(pmTargets, pmPredicts, pvvResults);
		// print results
		Mat res;
		reduce(*pvvResults, res, 0, CV_REDUCE_AVG);
		cout << res << endl;
		// evaluate data
		exportData(*pvvResults);
		return 0;
	}
	void term() {
		if (pcComparator) {
			delete pcComparator;
			pcComparator = NULL;
		}
		if (pcParser) {
			delete pcParser;
			pcParser = NULL;
		}
		if (pvvResults) {
			pvvResults->release();
			delete pvvResults;
			pvvResults = NULL;
		}
		if (pmPredicts) {
			pmPredicts->clear();
			delete pmPredicts;
			pmPredicts = NULL;
		}
		if (pmTargets) {
			pmTargets->release();
			delete pmTargets;
			pmTargets = NULL;
		}
	}
	// ATR
private:
	int iPic;
	int iMet;
	// databank
	Mat* pmTargets;
	vector<cv::Mat>* pmPredicts;
	Mat* pvvResults;
	// parts
	parserFileTree* pcParser;
	comparatorEuclidean* pcComparator;
	//iProcessor* pcProc;
	//iEvaluator* pcEval;

	// HELP
private:
	void exportData(Mat data) {
		string name = generateFileName("txt");
		cout << "Saving to file " << name << " START" << endl;
		ofstream file;
		file.open(name, std::ofstream::out | std::ofstream::trunc);
		if (file.is_open()) {
			for (register unsigned int i = 0; i < iPic; i++) {
				for (register unsigned int j = 0; j < iMet; j++) {
					file << pvvResults->at<float>(i, j) << "\t";
				}
				file << endl;
			}
			file.close();
		}
		else {
			cout << "At FILE: app.cpp\tLINE:99\t\t";
			cout << "Unable to open export file" << endl;
		}
	}
	string generateFileName(string extension) {
		// get time and date
		time_t t = time(0);
		struct tm now;
		localtime_s(&now, &t);
		// create filename
		string name;
		name += to_string(now.tm_year + 1900);
		if (now.tm_mon < 10) {
			name += "0";
		}
		name += to_string(now.tm_mon + 1);
		if (now.tm_mday < 10) {
			name += "0";
		}
		name += to_string(now.tm_mday);
		name += "_";
		if (now.tm_hour < 10) {
			name += "0";
		}
		name += to_string(now.tm_hour);
		name += "_";
		if (now.tm_min < 10) {
			name += "0";
		}
		name += to_string(now.tm_min);
		name += "_";
		if (now.tm_sec < 10) {
			name += "0";
		}
		name += to_string(now.tm_sec);
		name += ".";
		name += extension;
		return name;
	}
};