#pragma once
#include "include.h"

struct duo {
	duo() :
		trainData(Mat()), trainLabel(Mat()), testData(Mat()), testLabel(Mat()) {

	}
	duo(Mat trData, Mat trLabel, Mat teData, Mat teLabel) :
		trainData(trData), trainLabel(trLabel), testData(teData), testLabel(teLabel) {

	}
	Mat trainData, trainLabel, testData, testLabel;
};

class iSplitter {
	// C&D
public:
	virtual ~iSplitter() {}

	// ACC
public:
	virtual bool duoRatio(double dRatio) = 0;
	virtual duo splitDuo(Mat* pmData, Mat* pmLabels) = 0;
};