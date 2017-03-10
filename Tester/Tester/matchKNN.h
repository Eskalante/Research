#pragma once

#include "iMatch.h"

using namespace cv;
using namespace cv::ml;

class matchKNN :public iMatcher {
	// C&D
public:
	matchKNN() : iFlag(0) {

	}
	~matchKNN() {

	}
	// SYS
	void	init(unsigned int iFlags = 0) {
		iFlag = iFlags;
		pKNN = KNearest::create();
	}
	bool	train(Mat* pmData, Mat* pmLabel) {
		if (pmData == NULL)
			return 1;
		if (pmLabel == NULL)
			return 1;
		Mat a, b;
		pmData->convertTo(a, CV_32FC1);
		pmLabel->convertTo(b, CV_32SC1);
		pKNN->train(a, ROW_SAMPLE, b);
		return !(pKNN->isTrained());
	}
	float	test(Mat* pmSample) {
		return pKNN->predict(*pmSample);
	}
	bool	loadModel(char* path) {
		return 0;
	}
	bool	saveModel(char* path) {
		return 0;
	}
	// ATR
private:
	unsigned int iFlag;
	Ptr<KNearest> pKNN;
};
