//#ifndef _FEATURE_FOURIER2D_
#pragma once
#define _FEATURE_FOURIER2D_

#include <math.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iFeature.h"

using namespace cv;


// test
class feature_fourier2d : public iFeature {
	// C&D
public:
	virtual ~feature_fourier2d() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		Mat out;

		out=extract(input, 70, 80);

		return out;
	}
	virtual Mat extract(Mat input, int from, int to) {
		Mat out, I=input;

		//namedWindow("1in", 1);
		//imshow("1in", input);
		
		Mat padded;                            //expand input image to optimal size
		int m = getOptimalDFTSize( I.rows );
		int n = getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

		//namedWindow("2padded", 1);
		//imshow("2padded", padded);

		Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
		Mat complexI;
		merge(planes, 2, complexI); 

		dft(complexI, complexI);            // this way the result may fit in the source matrix
	
	

		// compute the magnitude and switch to logarithmic scale
		// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
		split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
		Mat magI = planes[0];

		magI += Scalar::all(1);                    // switch to logarithmic scale
		log(magI, magI);

		// crop the spectrum, if it has an odd number of rows or columns
		magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

		//Mat spectrum=magI.clone();
		//spectrum.convertTo(spectrum, CV_8UC3);
		//applyColorMap(spectrum, spectrum, cv::COLORMAP_JET);
		//namedWindow("3spectrum", 1);
		//imshow("3spectrum", spectrum);

		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = magI.cols/2;
		int cy = magI.rows/2;

		Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

		Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);

		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);

		//normalize(magI, magI, 0, 255, CV_MINMAX);
		normalize(magI, magI, 0, 255, NORM_MINMAX); 

		//Mat spectrum2=magI.clone();
		//spectrum2.convertTo(spectrum2, CV_8UC3);
		//applyColorMap(spectrum2, spectrum2, cv::COLORMAP_JET);
		//namedWindow("4spectrum", 1);
		//imshow("4spectrum", spectrum2);

		Mat crop;
		int fromx=75-(from/2);
		int tox=75+(from/2);
		int fromy=75-(to/2);
		int toy=75+(to/2);
		magI(Range(fromx,tox), Range(fromy,toy)).copyTo(crop);

		//Mat crop2=crop.clone();
		//crop2.convertTo(crop2, CV_8UC3);
		//applyColorMap(crop2, crop2, cv::COLORMAP_JET);
		//namedWindow("5crop", 1);
		//imshow("5crop", crop2);
		//waitKey(0);
	
		/*
		Mat spectrum=crop;
		namedWindow("Spatial", WINDOW_NORMAL);
		namedWindow("Spectrum", WINDOW_NORMAL);
		imshow("Spatial"       , I   );    // Show the result
		imshow("Spectrum", spectrum);
		waitKey(0);
		*/

		/*
		namedWindow("in", WINDOW_NORMAL);
		namedWindow("fft", WINDOW_NORMAL);
		namedWindow("fft reduced", WINDOW_NORMAL);
		namedWindow("spatial", WINDOW_NORMAL);
		imshow("in", input);
		imshow("fft", fourier);
		imshow("fft reduced", red);
		imshow("spatial", spat);
		waitKey(0);
		*/

		crop.convertTo(crop, CV_32FC1);
		out=crop.reshape(0, 1);
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

//#endif _FEATURE_FOURIER2D_