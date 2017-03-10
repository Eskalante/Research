#include <iostream>
#include <Windows.h>
#include "radon.h"

using namespace std;

// from min-max to 0-255
Mat eqHist(Mat src) {
	Mat ret = Mat::zeros(src.rows, src.cols, CV_8UC1);
	double min, max;
	minMaxLoc(src, &min, &max, NULL, NULL);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			//cout << "i " << i << "\t\tj " << j << endl;
			double value = (double)(src.at<float>(i, j));
			unsigned char newValue = (unsigned int)(255.0*((value - min) / (max - min)));
			ret.at<unsigned char>(i, j) = newValue;
		}
	}
	return ret;
}
// get Mat.type() as string
string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

int main() {
	// declarations
	Radon drt;
	Mat img, imgGray, spectrumGray, a, b, c, d, e, f;
	// load image
	img = imread("Untitled.png");
	// convert to gray
	if (img.type() == CV_8UC3) {
		cvtColor(img, imgGray, CV_RGB2GRAY);
	}
	else{
		img.copyTo(imgGray);
	}
	// show original
	imshow("Original", imgGray);
	// extract spectrum
	spectrumGray = drt.extract(imgGray, 0.0, 179.0);
	// stretch histogram
	b = eqHist(spectrumGray);
	// convert to 3 channels
	cvtColor(b, c, CV_GRAY2RGB);
	// set data type in Mat to unsigned char
	c.convertTo(d, CV_8UC3);
	// show spectrum
	imshow("Spektrum", d);
	// save spectrum image
	imwrite("test.png", d);
	// end
	waitKey(0);

	return 0;
}