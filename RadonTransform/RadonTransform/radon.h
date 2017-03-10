#pragma once

#include <math.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

// test
class Radon {
	// C&D
public:
	virtual ~Radon() {}

	// ACC
public:
	// extract radon spectrum by angle (in degrees) range
	virtual Mat extract(Mat input, double from, double to) {
		// declarations
		Mat r, ra, rad, radd, raddd, in;
		// stretch image to by rectangle
		if (input.rows > input.cols) {
			int expand = (input.rows - input.cols) / 2;
			copyMakeBorder(input, in, 0, 0, expand, expand, BORDER_CONSTANT, 0);
		}
		else {
			int expand = (input.cols - input.rows) / 2;
			copyMakeBorder(input, in, expand, expand, 0, 0, BORDER_CONSTANT, 0);
		}
		// do radon in range of angles
		raddd = radon(in, from, to);
		// rotate 90 degree clockwise
		radd = rotate(raddd, 90.0);
		// flip by y-axis
		flip(radd, rad, 1);
		// crop black borders
		ra=crop(rad);
		// convert to float
		ra.convertTo(r, CV_32FC1);
		// return
		return r;
	}

	// HELP
private:
	double pi;
	Mat rotate(Mat in, double angle, bool debug = false) {
		// time to calculate
		double t;
		Mat out;

		if (debug)
			t = getTickCount();
		int len = std::max(in.cols, in.rows);
		cv::Point2f pt(len / 2., len / 2.);
		cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);
		cv::warpAffine(in, out, r, cv::Size(len, len));
		if (debug)
			printf("%f", getTickCount() - t);

		// show in and out
		if (debug) {
			namedWindow("in", 1);
			namedWindow("out", 1);
			imshow("in", in);
			imshow("out", out);
			waitKey(0);
		}

		return out;
	}
	Mat radon(Mat in, double from_angle, double to_angle, bool debug = false) {
		//Mat radon_image = Mat::zeros(to_angle - from_angle, std::max(in.cols, in.rows), CV_32FC1);
		//Mat tmp(1, std::max(in.cols, in.rows), CV_32S);
		Mat rot, vec, radon_image;
		//Mat out;
		// time to calculate
		double t;

		if (debug)
			t = getTickCount();
		for (register int i = from_angle; i<to_angle; i++) {
			rot = rotate(in, (double)i, debug);
			reduce(rot, vec, 0, CV_REDUCE_SUM, CV_32FC1);
			/*for (register int j = 0; j<rot.cols; j++) {
				radon_image.at<cv::Vec<float, 1>>(i, j) = (sum(rot.col(j))[0]);
			}*/
			radon_image.push_back(vec);
		}
		//int max = std::max(radon_image.rows, radon_image.cols);
		//cv::resize(radon_image, radon_image, cv::Size(max, max));
		//if (debug)
		//	printf("%f", getTickCount() - t);



		return radon_image;
	}
	Mat crop(Mat in) {
		Mat ret;
		int rf, rt, cf, ct;
		
		for (int i = 1; i < in.rows; i++) {
			if (countNonZero(in.row(i))) {
				rf = i-1;
				break;
			}
		}
		for (int i = ((in.rows)-1); i != 0; i--) {
			if (countNonZero(in.row(i))) {
				rt = i+1;
				break;
			}
		}

		for (int i = 1; i < in.cols; i++) {
			if (countNonZero(in.col(i))) {
				cf = i-1;
				break;
			}
		}
		for (int i = (in.cols) - 1; i != 0; i--) {
			if (countNonZero(in.col(i))) {
				ct = i+1;
				break;
			}
		}

		in(Range(rf, rt), Range(cf, ct)).copyTo(ret);
		return ret;
	}
};