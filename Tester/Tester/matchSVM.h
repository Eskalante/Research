#pragma once

#include "iMatch.h"

using namespace cv;
using namespace cv::ml;

class matchSVM :public iMatcher {
	// C&D
public:
	matchSVM() : iFlag(0) {

	}
	~matchSVM() {

	}
	// SYS
	void	init(unsigned int iFlags = 0) {
		iFlag = iFlags;
		pSVM = SVM::create();
		pSVM->setType(SVM::C_SVC);
		pSVM->setKernel(SVM::LINEAR);
		pSVM->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	}
	bool	train(Mat* pmData, Mat* pmLabel) {
		if (pmData == NULL)
			return 1;
		if (pmLabel == NULL)
			return 1;
		Mat a, b;
		pmData->convertTo(a, CV_32FC1);
		pmLabel->convertTo(b, CV_32SC1);
		pSVM->train(a, ROW_SAMPLE, b);
		return !(pSVM->isTrained());
	}
	float	test(Mat* pmSample) {
		return pSVM->predict(*pmSample);
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
	Ptr<SVM> pSVM;
};
