#pragma once
#include "iCalculator.h"

class calcPRF : public iCalculator {
	// C&D
public:
	calcPRF() {

	}
	~calcPRF() {

	}
	// MAIN
public:
	//virtual bool init(std::vector<double> vParametes, unsigned int iFlags = 0) {}
	virtual Mat calculate(Mat* pmResults) {
		if (!pmResults)
			return Mat();
		unsigned int classNum = 0;
		for (register unsigned int i = 0; i < pmResults->rows; i++) {
			if (pmResults->at<double>(i, 0) >(double)classNum) {
				classNum = (unsigned int)(pmResults->at<double>(i, 0));
			}
		}
		if (classNum == 0)
			return Mat();
		classNum++;
		// calculate confusion matrix
		Mat conf = Mat::zeros(classNum, classNum, CV_8UC1);
		for (register unsigned int i = 0; i < pmResults->rows; i++) {
			unsigned int x = pmResults->at<double>(i, 0);
			unsigned int y = pmResults->at<double>(i, 1);
			conf.at<unsigned char>(x, y)++;
		}
		cout << conf << endl;
		// calc support
		Mat precisions = Mat::zeros(1, classNum, CV_32FC1);
		Mat recalls = Mat::zeros(1, classNum, CV_32FC1);
		Mat rows, cols;
		reduce(conf, rows, 1, REDUCE_SUM, CV_32FC1);
		reduce(conf, cols, 0, REDUCE_SUM, CV_32FC1);
		cout << rows << endl;
		cout << cols << endl;
		for (register unsigned int i = 0; i < classNum; i++) {
			double TP = (double)conf.at<unsigned char>(i, i);
			double FP = ((double)rows.at<float>(i, 0)) - TP;
			double FN = ((double)cols.at<float>(0, i)) - TP;
			precisions.at<float>(0, i) = TP / (TP + FP);
			recalls.at<float>(0, i) = TP / (TP + FN);
		}
		// calc overall Precision Recall F1
		Mat ret;
		double precision = ((double)sum(precisions)[0]) / ((double)classNum);
		ret.push_back(precision);
		double recall = ((double)sum(recalls)[0]) / ((double)classNum);
		ret.push_back(recall);
		double f1 = 2 * ((precision*recall) / (precision + recall));
		ret.push_back(f1);
		cout << precision << "\t" << recall << "\t" << f1 << endl;
		return ret;
	}
};
