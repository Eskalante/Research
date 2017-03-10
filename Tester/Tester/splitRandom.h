#pragma once
#include "iSplitter.h"

class splitRandom : public iSplitter {
	// C&D
public:
	splitRandom(): dRatio(0) {

	}
	~splitRandom() {

	}
	// MAIN
public:
	virtual bool duoRatio(double dDuoRatio) {
		dRatio = dDuoRatio;
		return 0;
	}
	virtual duo splitDuo(Mat* pmData, Mat* pmLabels) {
		if (!pmData) {
			return duo();
		}
		if (!pmLabels) {
			return duo();
		}
		if (pmData->rows != pmLabels->rows) {
			return duo();
		}
		duo ret;
		RNG rng(5485246);
		for (register unsigned int i = 0; i < pmData->rows; i++) {
			if (rng.uniform(0, pmData->rows) <= (pmData->rows / dRatio)) {
				ret.testData.push_back(pmData->row(i));
				ret.testLabel.push_back(pmLabels->row(i));
			}
			else {
				ret.trainData.push_back(pmData->row(i));
				ret.trainLabel.push_back(pmLabels->row(i));
			}
		}
		return ret;
	}
	// ATR
private:
	double dRatio;
};
