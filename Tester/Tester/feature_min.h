//#ifndef _FEATURE_MIN_
#pragma once
#define _FEATURE_MIN_

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

#include "iFeature.h"

// test
class feature_min : public iFeature {
	// C&D
public:
	virtual ~feature_min() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		Mat out;

		out=extract(input, 33, 33);

		return out;
	}
	virtual Mat extract(Mat input, int from, int to) {
		Mat out;
		out=input;

		resize(input, out, Size(from, to), 0, 0, INTER_CUBIC); 
		//blur( out, out, Size(3,3) );
		//Canny( out, out, 100, 300, 3 );

		/*namedWindow( "in", 1 );
		namedWindow( "out", 1 );
		imshow( "in", input );
		imshow( "out", out );
		waitKey(0);*/
		
		out=out.reshape(0, 1);
		out.convertTo(out, CV_32FC1);
		return out;
	}
};

//#endif