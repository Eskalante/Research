//#ifndef _FEATURE_FR_
#pragma once
#define _FEATURE_FR_

#include <math.h>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iFeature.h"

using namespace cv;

int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

class conture {
public:
	conture() {}
	~conture() {
		contures.clear();
	}
	void operator()(cv::Mat in) {
		cv::Mat tmp;
		cv::Mat map;

		map=mapping(in);
		
		contures.push_back(tmp);
	}
	cv::Mat operator[](unsigned int i) {
		return contures[i];
	}
	cv::Mat getConture(unsigned int i) {
		return contures[i];
	}
	unsigned int getCount() const {
		return ((unsigned int)contures.size());
	}
private:
	std::vector<cv::Mat> contures;
	cv::Mat mapping(cv::Mat in) {
		signed int register x=0, y=0;
		unsigned int idx=+1;
		cv::Mat out=cv::Mat::zeros(in.rows, in.cols, CV_8UC1);
		namedWindow("mapping", 2);

		for(y=0; y<in.rows; y++) {
			for(x=0; x<in.cols; x++) {
				cv::Mat tmp;
				if(mark(idx, x, y, &in, &out, tmp))
					idx++;
			}
		}
		return out;
	}
	bool mark(int idx, int x, int y, Mat* in, Mat* map, Mat tmp) {
		unsigned char val=in->at<unsigned char>(Point(x,y));
		unsigned char index=map->at<unsigned char>(Point(x,y));
		if(val>0x00 && index==0x00) {
			map->at<unsigned char>(Point(x,y))=(unsigned char)idx;

			
			cv::threshold(*map, tmp, 0, 255, THRESH_BINARY);
			imshow("mapping", tmp);
			waitKey(0);
			
			//up
			if((y-1)>=0) {
				mark(idx, x, y-1, in, map, tmp);
			}
			//down
			if((y+1)<in->rows) {
				mark(idx, x, y+1, in, map, tmp);
			}
			//left
			if((x-1)>=0) {
				mark(idx, x-1, y, in, map, tmp);
			}
			//right
			if((x+1)<in->cols) {
				mark(idx, x+1, y, in, map, tmp);
			}
			return 1;
		}
		return 0;
	}
};

// test
class feature_fr : public iFeature {
	// C&D
public:
	virtual ~feature_fr() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		Mat out;

		out=extract(input, 0, 30);

		return out;
	}
	virtual Mat extract(Mat input, int from, int to) {
		// init
		Mat out, src_gray, canny_output, com, fourier;
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		
		// steps
		// prerocesing image
		threshold(input, input, 0, 255, THRESH_BINARY);
		blur(input, src_gray, Size(3,3));
		// detect edges using canny
		Canny(src_gray, canny_output, thresh, thresh*2, 3);
		// find contours
		findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		// transform to complex numbers
		com=vecBinToVecCom(contours);
		// dft
		dft(com, fourier);
		// dimension reduction
		fourier=fourier.t();
		out=fourier(Range(0,1), Range(from,to));

		//namedWindow("four", WINDOW_NORMAL);
		//namedWindow("out", WINDOW_NORMAL);
		//imshow("four", plot(fourier));
		//imshow("out", plot(out));

		// exit
		//waitKey(0);
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

//#endif _FEATURE_FR_