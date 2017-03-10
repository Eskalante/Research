#pragma once

#include "iMatch.h"

using namespace cv;
using namespace cv::ml;

class matchLR :public iMatcher {
	// C&D
public:
	matchLR() : iFlag(0) {

	}
	~matchLR() {

	}
	// SYS
	void	init(unsigned int iFlags = 0) {
		iFlag = iFlags;
		pLR = LogisticRegression::create();
	}
	bool	train(Mat* pmData, Mat* pmLabel) {
		if (pmData == NULL)
			return 1;
		if (pmLabel == NULL)
			return 1;
		Mat a, b;
		pmData->convertTo(a, CV_32FC1);
		pmLabel->convertTo(b, CV_32FC1);
		pLR->train(a, ROW_SAMPLE, b);
		return !(pLR->isTrained());
	}
	float	test(Mat* pmSample) {
		return pLR->predict(*pmSample);
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
	Ptr<LogisticRegression> pLR;
};

