//#ifndef _FEATURE_RADW_
#pragma once
#define _FEATURE_RADW_

#include <math.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iFeature.h"

using namespace cv;

// test
class feature_radW : public iFeature {
	// C&D
public:
	virtual ~feature_radW() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		return extract(input, 0, 150);
	}
	virtual Mat extract(Mat input, int from, int to) {
		pi=3.14159265;
		double dfrom=0;
		double dto=360;
		Mat rad, radd;

		//double btest=cv::getTickCount();
		 		
		
		rad=radon(input, dfrom, dto);
		resize(rad, rad, Size(from, to), 0, 0, CV_INTER_LINEAR);
		/*
		Mat surf;
		rad.convertTo(rad, CV_8UC3);
		applyColorMap(rad, surf, COLORMAP_HOT);
		//resize(surf, surf, Size(), 4, 4, INTER_CUBIC); 
		namedWindow( "rad", 1 );
		imshow( "rad", surf );
		radd.convertTo(radd, CV_8UC3);
		applyColorMap(radd, radd, COLORMAP_HOT);
		//resize(radd, radd, Size(), 4, 4, INTER_CUBIC); 
		namedWindow( "radd", 2 );
		imshow( "radd", radd );
		waitKey(0);
		*/

		//cv::reduce(radd, radd, 0, CV_REDUCE_AVG);
		//resize(rad, radd, Size(50,50), 0, 0, INTER_CUBIC);
		//rad=radd;
		rad=rad.reshape(0, 1);
		rad.convertTo(rad, CV_32FC1);

		//double atest=cv::getTickCount();
		//double time=(atest-btest)/getTickFrequency();
		//printf("FVC %f\n", time);

		return rad;
	}

	// HELP
private:
	double pi;
	Mat spin(Mat in, double angle, bool debug=false) {
		// number of rows in image
		unsigned int nrows=in.rows;
		// number of collumns in image
		unsigned int ncols=in.cols;
		// diagonal length of image
		double a=nrows*nrows;
		double b=ncols*ncols;
		double diagonal=std::sqrt(a+b);
		// output image is bigger, so the angled input will fit in
		Mat out=Mat::zeros(diagonal, diagonal, CV_8U);
		// transform to radians
		double rad=(2*pi)/(360/angle);
		// calculate cosinus
		double c=std::cos(rad);
		// calculate sinus
		double s=std::sin(rad);
		// time to calculate
		double t;

		if(debug)
			t=getTickCount();

		// for each input pixel, calculate its new rotated position
		// range is shifted so the center of rotation is in centre of pictures
		for(signed int x=(-(nrows/2)); x<(signed int)((nrows/2)-1); x++) {		// x    - centered position of IN row
			for(signed int y=(-(ncols/2)); y<(signed int)((ncols/2)-1); y++) {	// y    - center. pos. of IN col
				// calculation
				double or=(x*c-y*s);							// or   - center. pos. of OUT row
				double oc=(y*c+x*s);							// oc   - center. pos. of OUT col
				// transform coordinates to image space
				unsigned int row=(unsigned int)(x+nrows/2);							// row  - original pos. of IN row
				unsigned int col=(unsigned int)(y+ncols/2);							// col  - original pos. of IN col
				unsigned int orow=(unsigned int)(or+diagonal/2);				// orow - original pos. of OUT row
				unsigned int ocol=(unsigned int)(oc+diagonal/2);				// ocol - original pos. of OUT col
				// copy pixel value from 
				// IN image on position to 
				// OUT image on rotated position
				out.at<unsigned char>(orow,ocol)=in.at<unsigned char>(row,col);          
			}
		}

		if(debug)
			printf("%f", getTickCount()-t);
		

		// show in and out
		if (debug) {
			namedWindow( "in", 1 );
			namedWindow( "out", 1 );
			imshow( "in", in );
			imshow( "out", out );
			waitKey(0);
		}
		
		return out;
	}
	Mat rotate(Mat in, double angle, bool debug=false){
		// time to calculate
		double t;
		Mat out;

		if(debug)
			t=getTickCount();
		int len = std::max(in.cols, in.rows);
		cv::Point2f pt(len/2., len/2.);
		cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);
		cv::warpAffine(in, out, r, cv::Size(len, len));
		if(debug)
			printf("%f", getTickCount()-t);

		// show in and out
		if (debug) {
			namedWindow( "in", 1 );
			namedWindow( "out", 1 );
			imshow( "in", in );
			imshow( "out", out );
			waitKey(0);
		}

		return out;
	}
	Mat radon(Mat in, double from_angle, double to_angle, bool debug=false) {
		//Mat radon_image=Mat::zeros(to_angle-from_angle, std::max(in.cols, in.rows), CV_32FC1);
		Mat radon_image=Mat::zeros((to_angle-from_angle)/10, std::max(in.cols, in.rows), CV_32FC1);
		
		Mat rot;
		//Mat out;
		// time to calculate
		double t;

		if(debug)
			t=getTickCount();
		for(register int i=from_angle; i<to_angle; i+=10) {
			rot=rotate(in, i, debug);
			for(register int j=0; j<rot.cols; j++) {
				radon_image.at<cv::Vec<float,1>>(i/10, j)=(sum(rot.col(j))[0]);
			}
			//radon_image.push_back(tmp);
		}
		int max=std::max(radon_image.rows, radon_image.cols);	
		//cv::resize(radon_image, radon_image, cv::Size(max, max));
		if (debug)
			printf("%f", getTickCount()-t);

		

		return radon_image;
	}
};

//#endif