//#ifndef _FEATURE_HOUGH_
#pragma once
#define _FEATURE_HOUGH_

#include <math.h>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iFeature.h"

using namespace std;


// test
class feature_hough : public iFeature {
	// C&D
public:
	virtual ~feature_hough() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		Mat out;

		out=extract(input, 0, 150);

		return out;
	}
	virtual Mat extract(Mat input, int from, int to) {
		// init
		Mat out, dst, cdst;
		vector<Vec2f> lines;
		// to do
		resize(input, input, Size(from, to), 0, 0, INTER_CUBIC);
		threshold(input, input, 1.0, 255, THRESH_BINARY);
		Canny(input, dst, 100, 200, 3);
		HoughLines(dst, lines, 1, CV_PI/180, 20, 0, 0 );
		// debug
		bool debug=true;
		if(debug) {
			//cvtColor(dst, cdst, CV_GRAY2BGR);
			cvtColor(dst, cdst, COLOR_GRAY2BGR);
			for( size_t i = 0; i < lines.size(); i++ ) {
				float rho = lines[i][0], theta = lines[i][1];
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a*rho, y0 = b*rho;
				pt1.x = cvRound(x0 + 1000*(-b));
				pt1.y = cvRound(y0 + 1000*(a));
				pt2.x = cvRound(x0 - 1000*(-b));
				pt2.y = cvRound(y0 - 1000*(a));
				line( cdst, pt1, pt2, Scalar(0,0,255));
			}
			namedWindow("in", WINDOW_NORMAL);
			namedWindow("canny", WINDOW_NORMAL);
			namedWindow("lines", WINDOW_NORMAL);
			imshow("in", input);
			imshow("canny", dst);
			imshow("lines", cdst);
			waitKey(0);
		}
		// out
		for( size_t i = 0; i < lines.size(); i++ ) {
			float rho = lines[i][0], theta = lines[i][1];
			Mat tmp;
			tmp.push_back(rho);
			tmp.push_back(theta);
			tmp=tmp.t();
			out.push_back(tmp);
		}
		//out=cv::Mat(lines);
		//out=out.t();
		out.convertTo(out, CV_32FC1);
		return out;
	}

	// HELP
private:
	Mat vecBinToVecCom(vector<vector<Point>> contours) {
		Mat out, planes[2];

		for(int i=0; i<contours.size(); i++) {
			if(contours[i].size()>10) {
				for(int j=0; j<contours[i].size(); j++) {
					float x=contours[i][j].x;
					float y=contours[i][j].y;
					planes[0].push_back<float>((float)x);
					planes[1].push_back<float>((float)y);
				}
			}
		}

		cv::merge(planes, 2, out);
		return out;
	}

	cv::Mat plot(cv::Mat vector) {
	register unsigned int i=0;
	float value=0.0f;
	signed short pos=0;
	unsigned int size=(unsigned int)vector.cols;
	double maxVal; 

	cv::Mat out(vector.cols, vector.cols, CV_8UC3);
	out.setTo(cv::Scalar(255, 255, 255));
	minMaxLoc( vector, NULL, &maxVal, NULL, NULL);

	for(i=0; i<size; i++) {
		value=vector.at<float>(0,i)/(float)maxVal;
		pos=(unsigned short)(((float)(size-1))*value);
		if(pos>=maxVal)
			pos=maxVal-1;
		if(pos<0)
			pos=0;
		out.at<Vec3b>(pos, i)[0]=0x00;
		out.at<Vec3b>(pos, i)[1]=0x00;
		out.at<Vec3b>(pos, i)[2]=0x00;
	}

	return out.clone();
}
};

//#endif _FEATURE_HOUGH_