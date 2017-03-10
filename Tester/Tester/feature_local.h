//#ifndef _FEATURE_LOCAL_
#pragma once
#define _FEATURE_LOCAL_

#include <stdio.h>
#include <iostream>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/ml/ml.hpp"

using namespace cv;

#include "iFeature.h"

bool sorter (int i,int j) { return (i<j); }

// test
class feature_local : public iFeature {
	// C&D
public:
	feature_local() {}
	feature_local(string det, string des) {
		sdetector=det;
		sdescriptor=des;
	}
	virtual ~feature_local() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		return extract(input, 0, 150);
	}
	virtual Mat extract(Mat input, int from, int to) {
		Mat out;
		vector<KeyPoint> kp;
		vector<KeyPoint> new_kp;
		
		initModule_nonfree(); // co to je?

		detector = FeatureDetector::create(sdetector);
		extractor = DescriptorExtractor::create(sdescriptor);
		detector->detect(input, kp);

		float size=(float)from;
		float respo=(float)(((float)to)/1000.0f);
		
		// filter
		for(int i=0; i<kp.size(); i++) {
			if(kp[i].size>=size) {
				if(kp[i].response>=respo) {
					new_kp.push_back(kp[i]);
				}
			}
		}
		//std::sort(new_kp.begin()->response, new_kp.end()->response, sorter); 
		
		/*Mat output;
		//drawKeypoints(input, new_kp, output);
		output=draw(input, new_kp);
		namedWindow( "int", WINDOW_NORMAL );
		namedWindow( "out", WINDOW_NORMAL );
		imshow( "in", input );
		imshow( "out", output );
		waitKey(0);*/
		
		extractor->compute(input, new_kp, out);

		clean_up();
		out.convertTo(out, CV_32FC1);
		return out;
	}

	// ATR
private:
	Ptr<FeatureDetector> detector;
	Ptr<DescriptorExtractor> extractor;
	string sdetector;
	string sdescriptor;

	// HELP
private:
	Mat draw(Mat in, vector<KeyPoint> k) {
		Mat out;
		RNG rng( 0xFFFFFFFF );
		out=in.clone();

		cvtColor(out, out, COLOR_GRAY2BGR);
		for(int i=0; i<k.size(); i++) {
			circle(out, k[i].pt, k[i].size, randomColor(rng));
		}

		//resize(out, out, Size(500, 500), 0, 0, INTER_CUBIC); // INTER_LINEAR INTER_CUBIC

		return out;
	}
	static Scalar randomColor(RNG& rng) {
		int icolor = (unsigned) rng;
		return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
	}
	void clean_up()
	{
		if(!detector->empty()){
			detector.release();
		}
		if(!extractor->empty()){
			extractor.release();
		}

	}
};

//#endif _FEATURE_LOCAL_