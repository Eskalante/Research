//#ifndef _FEATURE_CANNY_
#pragma once
#define _FEATURE_CANNY_

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

#include "iFeature.h"

// test
class feature_canny : public iFeature {
	// C&D
public:
	virtual ~feature_canny() {}

	// ACC
public:
	virtual Mat extract(Mat input, int from, int to) {
		Mat out, blured;
		out=input;
		resize(input, out, Size(from, to), 0, 0, INTER_CUBIC); 
		blur( out, blured, Size(3,3) );
		Canny( blured, out, 100, 300, 3 );

		/*namedWindow( "in", CV_WINDOW_NORMAL );
		namedWindow( "blur", CV_WINDOW_NORMAL );
		namedWindow( "out", CV_WINDOW_NORMAL );
		imshow( "in", input );
		imshow( "blur", blured );
		imshow( "out", out );
		waitKey(0);*/
		
		out=out.reshape(0, 1);
		out.convertTo(out, CV_32FC1);
		return out;
	}
	virtual Mat extract(Mat input) {
		Mat out;
		
		out=extract(input, 33, 33);

		return out;
	}
};

//#endif