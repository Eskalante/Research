#pragma once
#include "include.h"

#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>

using namespace cv;

// template for all classification methods
class iMatcher {
	// C&D
public:
	virtual ~iMatcher() {}

	// ACC
public:
	virtual void	init(unsigned int iFlags = 0) = 0;
	virtual bool	train(Mat* pmData, Mat* pmLabel) = 0;
	virtual float	test(Mat* pmSample) = 0;
	virtual bool	loadModel(char* path) = 0;
	virtual bool	saveModel(char* path) = 0;
};