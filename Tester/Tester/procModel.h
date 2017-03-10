#pragma once
#include "iProcessor.h"

class procMatch : public iProcessor {
	// C&D
public:
	procMatch() :pcMatch(NULL), pcSplit(NULL), iFlag(0) {

	}
	~procMatch() {
		if (pcSplit) {
			delete pcSplit;
			pcSplit = NULL;
		}
		if (pcMatch) {
			delete pcMatch;
			pcMatch = NULL;
		}
	}
	// MAIN
public:
	virtual bool init(string strMatcher, string strSplitter, unsigned int iFlags = 0, string strModelPath = "") {
		if (!strcmp(strMatcher.c_str(), "svm")) {
			pcMatch = new matchSVM();
			pcMatch->init(0);
		}
		else if (!strcmp(strMatcher.c_str(), "nb")) {
			pcMatch = new matchNB();
			pcMatch->init(0);
		}
		else if (!strcmp(strMatcher.c_str(), "knn")) {
			pcMatch = new matchKNN();
			pcMatch->init(0);
		}
		else if (!strcmp(strMatcher.c_str(), "ann")) {
			// do not use
			pcMatch = new matchANN();
			pcMatch->init(0);
		}
		else if (!strcmp(strMatcher.c_str(), "lr")) {
			pcMatch = new matchLR();
			pcMatch->init(0);
		}
		else if (!strcmp(strMatcher.c_str(), "flann")) {
			pcMatch = new matchFLANN();
			pcMatch->init(0);
		}
		else {
			debug::error(2, "Unknow matcher.");
			return 1;
		}
		if (!strcmp(strSplitter.c_str(), "cross_validation")) {
			pcSplit = new splitCrossValidation();
			pcSplit->duoRatio((double)iFlags);
		}
		else if (!strcmp(strSplitter.c_str(), "random")) {
			pcSplit = new splitRandom();
			pcSplit->duoRatio((double)iFlags);
		}
		else if (!strcmp(strSplitter.c_str(), "begin")) {
			pcSplit = new splitBegin();
			pcSplit->duoRatio((double)iFlags);
		}
		else {
			debug::error(2, "Unknow splitter.");
			return 1;
		}
		iFlag = iFlags;
		return 0;
	}
	virtual bool processData(cv::Mat* pmData, cv::Mat* pmLabels, cv::Mat* pmResults) {
		duo input = pcSplit->splitDuo(pmData, pmLabels);
		if (pcMatch->train(&(input.trainData), &(input.trainLabel)))
			return 1;
		for (register unsigned int i = 0; i < input.testData.rows; i++) {
			Mat row;
			row.push_back((double)input.testLabel.at<double>(i,0));
			row.push_back((double)(pcMatch->test(&(input.testData.row(i)))));
			transpose(row, row);
			pmResults->push_back(row);
		}
		return 0;
	}
	// ATR
private:
	iMatcher* pcMatch;
	iSplitter* pcSplit;
	unsigned int iFlag;
};
