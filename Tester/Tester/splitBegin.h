#pragma once
#include "iSplitter.h"

class splitBegin : public iSplitter {
	// C&D
public:
	splitBegin() : dRatio(0) {

	}
	~splitBegin() {

	}
	// MAIN
public:
	virtual bool duoRatio(double dDuoRatio) {
		dRatio = dDuoRatio;
		return 0;
	}
	virtual duo splitDuo(Mat* pmData, Mat* pmLabels) {
		// check if input is valid
		if (!pmData) {
			return duo();
		}
		if (!pmLabels) {
			return duo();
		}
		if (pmData->rows != pmLabels->rows) {
			return duo();
		}
		// find number of pictures per class
		unsigned int picNumber = 0;
		while (!pmLabels->at<double>(picNumber, 0)) {
			picNumber++;
		}
		duo ret;
		RNG rng(5485246);

		// uniform distribution of samples, first of ratio to test
		for (register unsigned int i = 0; i < pmData->rows;) {
			for (register unsigned int j = 0; j < picNumber; j++, i++) {
				if (j < (picNumber *(dRatio / 100.0))) {
					ret.testData.push_back(pmData->row(i));
					ret.testLabel.push_back(pmLabels->row(i));
					cout << "test: " << i << endl;
				}
				else {
					ret.trainData.push_back(pmData->row(i));
					ret.trainLabel.push_back(pmLabels->row(i));
				}
			}
		}
		return ret;
	}
	// ATR
private:
	double dRatio;
};
