#pragma once
#include "include.h"
// components
#include "iFeature.h"
#include "feature_canny.h"
#include "feature_hough.h"

class iParser {
	// D
public:
	iParser() {

	}

	// MAIN
public:
	virtual bool init(std::string strPath, string strFeature, unsigned int iFlags=0) = 0;
	virtual bool parseData(cv::Mat* pmData, cv::Mat* pmLabels) = 0;
};
