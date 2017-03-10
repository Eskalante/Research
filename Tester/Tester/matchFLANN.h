#pragma once

#include "iMatch.h"

using namespace cv;
using namespace cv::ml;

class matchFLANN :public iMatcher {
	// C&D
public:
	matchFLANN() : iFlag(0) {

	}
	~matchFLANN() {

	}
	// SYS
	void	init(unsigned int iFlags = 0) {
		iFlag = iFlags;
		pFLANN = new FlannBasedMatcher();
	}
	bool	train(Mat* pmData, Mat* pmLabel) {
		if (pmData == NULL)
			return 1;
		if (pmLabel == NULL)
			return 1;
		Mat a, b;
		pmData->convertTo(a, CV_32FC1);
		pmLabel->convertTo(b, CV_32SC1);

		return 0;
	}
	float	test(Mat* pmSample) {
		Mat distribution;
		for (register unsigned int i = 0; i < vmTrains.size(); i++) {
			std::vector< DMatch > matches;
			pFLANN->match(vmTrains[i], *pmSample, matches);
			double max_dist = 0; double min_dist = 100;
			for (int j = 0; j < matches.size(); j++)
			{
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}
			std::vector< DMatch > good_matches;
			for (int i = 0; i < matches.size(); i++)
			{
				if (matches[i].distance <= max(2 * min_dist, 0.02))
				{
					good_matches.push_back(matches[i]);
				}
			}
			distribution.push_back((double)good_matches.size());
		}
		Point max;
		minMaxLoc(distribution, NULL, NULL, NULL, &max);
		return ((float)max.x);
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
	Ptr<FlannBasedMatcher> pFLANN;
	vector<Mat> vmTrains;
};
