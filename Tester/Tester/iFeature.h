#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

class iFeature {
	// C&D
public:
	virtual ~iFeature() {}

	// ACC
public:
	virtual Mat extract(Mat input) = 0;
	virtual Mat extract(Mat input, int from, int to) = 0;
};
