//#ifndef _FEATURE_FOURIER_
#pragma once
#define _FEATURE_FOURIER_

#include <math.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iFeature.h"

using namespace cv;


// test
class feature_fourier : public iFeature {
	// C&D
public:
	virtual ~feature_fourier() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		Mat out;

		out=extract(input, 0, 30);

		return out;
	}
	virtual Mat extract(Mat input, int from, int to) {
		pi=3.1415926535897931;
		Mat bin, polar, sobel, values, fourier, out, conts[10];
		int nc=0;
		//namedWindow("1in", 1);
		//namedWindow("2bin", 1);
		//namedWindow("4polar", 1);
		//namedWindow("3sobel", 1);
		
		//imshow("1in", input);
		// 1 input to binary
		threshold(input, bin, 10, 255, THRESH_BINARY);
		//imshow("2bin", bin);
		
		//cont(&nc, conts, bin);

		// 3 sobel to find edge
		cv::Sobel(bin, sobel, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(sobel, sobel);
		//imshow("3sobel", sobel);
		// 2 polar transform
		polar=polarToCartImage(sobel, Point(((double)(150/2)), ((double)(150/2))));
		//imshow("4polar", polar);
		//out=out.reshape(0, 1);
		//out.convertTo(out, CV_32FC1);
		// 4 vector
		values=val(polar);
		Mat plo;
		plo=plotToImage(values);
		//namedWindow("5plot", 10);
		//imshow("5plot", plo);
		// 5 fft
		dft(values, fourier);
		// 6 erase details
		out=fourier(Range(0,1), Range(from,to));// 30 87.4167
		//out=fourier;// 30 87.4167

		//waitKey(0);
		return out;
	}

	// HELP
private:
	double pi;
	Mat polarToCartImage(Mat polImg, Point center) {
		Mat out=Mat::zeros(polImg.rows, polImg.cols, polImg.type());
		double mag, angle;

		for(double y=0; y<polImg.rows/2; y++) {
			for(double x=0; x<polImg.cols; x++) {
				unsigned char val=polImg.at<unsigned char>(Point(x,y));
				if(val>=1) {
					mag=sqrt(double(((x-center.x)*(x-center.x))+((y-center.y)*(y-center.y))));
					if(x==75 && y==75) {
						angle=0;
					}
					else {
						angle=ang((x-center.x)/polImg.cols, (y-center.y)/polImg.rows);
					}
					angle=(((angle)/(pi*2))*polImg.cols);
					if(angle>polImg.cols)
						angle=(polImg.cols)-1;
					if(angle<0)
						angle=0;
					if(mag>polImg.rows)
						mag=polImg.rows-1;
					if(mag<0)
						mag=0;
					out.at<unsigned char>(Point(angle,mag))=polImg.at<unsigned char>(Point(x,y));
				}
			}
		}
		for(double y=polImg.rows/2; y<polImg.rows; y++) {
			for(double x=0; x<polImg.cols; x++) {
				unsigned char val=polImg.at<unsigned char>(Point(x,y));
				if(val>=1) {
					mag=sqrt(double(((x-center.x)*(x-center.x))+((y-center.y)*(y-center.y))));
					if(x==75 && y==75) {
						angle=0;
					}
					else {
						angle=ang((x-center.x)/polImg.cols, (y-center.y)/polImg.rows);
					}
					angle=(((angle)/(pi*2))*polImg.cols)+(polImg.cols/2)-1;
					if(angle>polImg.cols)
						angle=(polImg.cols)-1;
					if(angle<0)
						angle=0;
					if(mag>polImg.rows)
						mag=polImg.rows-2;
					if(mag<0)
						mag=0;
					out.at<unsigned char>(Point(angle,mag))=polImg.at<unsigned char>(Point(x,y));
				}
			}
		}

		return out;
	}
	Mat plotToImage(Mat in) {
		int size=(in.rows>in.cols)?in.rows:in.cols;
		Mat out=Mat::zeros(size, size, CV_8UC1);
		//normalize(in, in, 0, size-1, NORM_MINMAX, CV_32FC1);
		for(unsigned int x=0; x<size; x++) {
			signed int y=(unsigned int)in.at<float>(0,x);
			if(y>=size) {
				y=size-1;
			}
			else {
				if(y<=0) {
					y=0;
				}
				else {
					y=y;
				}
			}
			out.at<unsigned char>(y,x)=0x255;
		}
		return out;
	}
	Mat cartToPolarImage(Mat in) {
		int size=(in.rows>in.cols)?in.rows:in.cols;
		Mat out=Mat::zeros(size, size, CV_8UC1);
		

		return out;
	}
	double ang(double x, double y) {
		Point_<double> p;
		Point_<double> ref;
		ref.x=1;
		ref.y=0;
		p.x=x;
		p.y=y;
		double dot=(p.x*ref.x)+(p.y*ref.y);
		double plen=sqrt((p.x*p.x)+(p.y*p.y));
		double reflen=sqrt((ref.x*ref.x)+(ref.y*ref.y));
		double out=std::acos(dot/(plen*reflen));
		return out;
	}
	Mat val(Mat in) {
		Mat out=Mat::zeros(1, in.cols, CV_32FC1);

		for(double x=0; x<in.cols; x++) {
			for(double y=in.rows-1; y>=0; y--) {
				if(in.at<unsigned char>(Point(x,y))>0) {
					out.at<float>(0,x)=(float)y;
					break;
				}
			}
		}

		return out;
	}

	void cont(int* numContures, Mat* contures, Mat in) {
		for(double y=0; y<in.rows; y++) {
			for(double x=0; x<in.cols; x++) {
				unsigned char val=in.at<unsigned char>(Point(x,y));
				if(val>0x00) {
					val=0x00;
				}
			}
		}
	}
};

//#endif _FEATURE_FOURIER_