#pragma once

#include "iMatch.h"

using namespace cv;
using namespace cv::ml;

class matchANN :public iMatcher {
	// C&D
public:
	matchANN() : iFlag(0) {

	}
	~matchANN() {

	}
	// SYS
	void	init(unsigned int iFlags = 0) {
		iFlag = iFlags;
		pANN = ANN_MLP::create();
		pANN->setActivationFunction(ANN_MLP::SIGMOID_SYM);
		pANN->setTrainMethod(ANN_MLP::RPROP);
		pANN->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.01));
		pANN->setBackpropWeightScale(0.1);
		Mat layers;
		layers.push_back(1089);
		layers.push_back(2000);
		layers.push_back(1089);
		pANN->setLayerSizes(layers);
		pANN->create();
	}
	bool	train(Mat* pmData, Mat* pmLabel) {
		if (pmData == NULL)
			return 1;
		if (pmLabel == NULL)
			return 1;
		Mat a, b;
		pmData->convertTo(a, CV_32FC1);
		pmLabel->convertTo(b, CV_32SC1);
		pANN->train(a, ROW_SAMPLE, b);
		return !(pANN->isTrained());
	}
	float	test(Mat* pmSample) {
		return pANN->predict(*pmSample);
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
	Ptr<ANN_MLP> pANN;
};