//#ifndef _FEATURE_FCOMPLEX_
#pragma once
#define _FEATURE_FCOMPLEX_

#include <math.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iFeature.h"

using namespace cv;


// test
class feature_fcomplex : public iFeature {
	// C&D
public:
	virtual ~feature_fcomplex() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		Mat out;

		out=extract(input, 0, 300);

		return out;
	}
	Mat extract(Mat input, int from, int to) {
		pi=3.1415926535897931;
		Mat bin, com, fourier, spectrum, out;
		//namedWindow("in", 1);
		//namedWindow("bin", 1);
		//namedWindow("spectrum", 3);
		//

		// 1 input to binary
		threshold(input, bin, 10, 255, THRESH_BINARY);
		cv::Sobel(bin, bin, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(bin, bin);
		bin=polarToCartImage(bin, Point(((double)(150/2)), ((double)(150/2))));
		
		//imshow("in", input);
		//imshow("bin", bin);
		//waitKey(0);
		// 2 bin. img. to complex plane
		com=binToComplex(bin);
		// 3 dft
		Mat planes[] = {Mat_<float>(input), Mat::zeros(input.size(), CV_32F)};

		//dft(com, fourier);
		dft(com, fourier);

		split(fourier, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
		Mat magI = planes[0];
		fourier=magI;
		/*
		if(fourier.rows>=300) {
			fourier=fourier.t();
		}
		else {
			int marker=300-fourier.rows;
			for(int i=0; i<marker; i++) {
				fourier.push_back<float>((float)0.0);
			}
			fourier=fourier.t();
		}*/

		out=fourier(Range(0,1), Range(from,to));

		//out=out.reshape(0, 1);
		//out.convertTo(out, CV_32FC1);
		return out;
	}

	// HELP
private:
	double pi;
	Mat binToComplex(Mat in) {
		Mat planes[2], out=Mat::zeros(1, in.cols, CV_32FC1);
		planes[0]=Mat::zeros(1, in.cols, CV_32FC1);
		planes[1]=Mat::zeros(1, in.cols, CV_32FC1);

		for(double x=0; x<in.cols; x++) {
			for(double y=in.rows-1; y>=0; y--) {
				unsigned char val=in.at<unsigned char>(Point(x,y));
				if(val>=1) {
					planes[0].at<float>(0,x)=((float)x);
					planes[1].at<float>(0,x)=((float)y);
					break;
				}
			}
		}
		
		cv::merge(planes, 2, out);
		return out;
	}
	Mat complexToBin(Mat in, int size) {
		Mat planes[2], image=Mat::zeros(size, size, CV_8UC1);

		cv::split(in, planes);

		for(double i=0; i<in.rows; i++) {
			signed int x=(signed int)(planes[0].at<float>(i,0));
			if(x>=size)
				x=size-1;
			if(x<0)
				x=0;
			signed int y=(signed int)(planes[1].at<float>(i,0));
			if(y>=size)
				y=size-1;
			if(y<0)
				y=0;
			if(i>2447)
				int test=0;
			image.at<unsigned char>(Point(x,y))=0x255;
		}
		
		return image;
	}

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

//#endif _FEATURE_FCOMPLEX_