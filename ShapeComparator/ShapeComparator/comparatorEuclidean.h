#pragma once
#include "include.h"

class comparatorEuclidean {
	// C&D
public:
	comparatorEuclidean() {
	}
	~comparatorEuclidean() {
	}
	// MAIN
public:
	bool process(cv::Mat* pmTargets, vector<cv::Mat>* pmPredicts, Mat* pvvResults) {
		if (!pmTargets) {
			cout << "At FILE: comparatorEuclidean.cpp\tLINE:14\t\t";
			cout << "Data lost" << endl;
			return 1;
		}
		if (!pmPredicts) {
			cout << "At FILE: comparatorEuclidean.cpp\tLINE:19\t\t";
			cout << "Data lost" << endl;
			return 1;
		}
		if (!pvvResults) {
			cout << "At FILE: comparatorEuclidean.cpp\tLINE:24\t\t";
			cout << "Data lost" << endl;
			return 1;
		}
		int numoPic = pmTargets->rows;
		int numo = pmPredicts->size();
		for (register unsigned int i = 0; i < numoPic; i++) {
			// for each predict
			for (register unsigned int j = 0; j < numo; j++) {
				double result = norm(pmTargets->row(i), pmPredicts->at(j).row(i), CV_L2);
				pvvResults->at<float>(i, j) = (float)result;
			}
		}
		return 0;
	}
	// ACC
public:
	// ATR
private:
};
