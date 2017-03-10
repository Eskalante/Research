#pragma once

#include "include.h"
// parts
#include "iParser.h"
#include "parserFileTree.h"
#include "iProcessor.h"
#include "procModel.h"
#include "iEvaluator.h"
#include "evalDefault.h"

// application
class app {
	// C&D
public:
	app() :pmData(0), pmLabels(0), pmResults(0), pcParser(0), pcProc(0), pcEval(0) {

	}
	virtual ~app() {
		term();
	}
	// SYS
public:
	bool init(string strSettingsPath) {
		init();
		return 0;
	}
	bool init() {
		// init databanks
		pmData = new Mat();
		pmLabels = new Mat();
		pmResults = new Mat();
		// setup parser
		pcParser = new parserFileTree();
		if (pcParser->init("S:\\Database\\", "hough")) { //"E:\\1_Praca\\0_vyskum\\0_databazy\\A0_diza\\Database_C\\"
			debug::error(1, "Unable to initialize parser.");
			return 1;
		}
		((parserFileTree*)pcParser)->setClassNum(15);
		((parserFileTree*)pcParser)->setPictureNum(100);
		// setup processor
		pcProc = new procMatch();
		if (pcProc->init("svm", "cross_validation", 10.0)) {
			debug::error(1, "Unable to initialize processor.");
			return 1;
		}
		// setup evaluator
		pcEval = new evalDefault();
		return 0;
	}
	bool cycle() {
		// parse data
		cout << "Parser START" << endl;
		pcParser->parseData(pmData, pmLabels);
		// process data
		cout << "Processor START" << endl;
		pcProc->processData(pmData, pmLabels, pmResults);
		// evaluate data
		cout << "Evaluator START" << endl;
		pcEval->evaluateData(pmResults, "prf");
		return 0;
	}
	void term() {
		if (pcEval) {
			delete pcEval;
			pcEval = NULL;
		}
		if (pcProc) {
			delete pcProc;
			pcProc = NULL;
		}
		if (pcParser) {
			delete pcParser;
			pcParser = NULL;
		}

		if (pmResults) {
			pmResults->release();
			delete pmResults;
			pmResults = NULL;
		}
		if (pmLabels) {
			pmLabels->release();
			delete pmLabels;
			pmLabels = NULL;
		}
		if (pmData) {
			pmData->release();
			delete pmData;
			pmData = NULL;
		}
	}
	// ATR
private:
	// databank
	Mat* pmData;
	Mat* pmLabels;
	Mat* pmResults;
	// parts
	iParser* pcParser;
	iProcessor* pcProc;
	iEvaluator* pcEval;
};