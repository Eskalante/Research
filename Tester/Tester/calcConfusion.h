#pragma once
#include "iCalculator.h"

class calcConfusion : public iCalculator {
	// C&D
public:
	calcConfusion() {

	}
	~calcConfusion() {

	}
	// MAIN
public:
	//virtual bool init(std::vector<double> vParametes, unsigned int iFlags = 0) {}
	virtual Mat calculate(Mat* pmResults) {
		if (!pmResults)
			return Mat();
		unsigned int classNum = 0;
		for (register unsigned int i = 0; i < pmResults->rows; i++) {
			if (pmResults->at<double>(i, 0) > (double)classNum) {
				classNum = (unsigned int)(pmResults->at<double>(i, 0));
			}
		}
		if (classNum == 0)
			return Mat();
		// calculate confusion matrix
		Mat ret = Mat::zeros(classNum+1, classNum+1, CV_8UC1);
		for (register unsigned int i = 0; i < pmResults->rows; i++) {
			unsigned int x = pmResults->at<double>(i, 0);
			unsigned int y = pmResults->at<double>(i, 1);
			ret.at<unsigned char>(x, y)++;
		}
		return ret;
	}
};
