#pragma once
#include "include.h"
#include "feature_rad2.h"

class parserFileTree{
	// C&D
public:
	parserFileTree() :iPic(0), iMet(0), pcFeat(NULL) {

	}
	~parserFileTree() {
		if (pcFeat) {
			delete pcFeat;
			pcFeat = NULL;
		}
	}
	// MAIN
public:
	virtual bool init(std::string strDataPath, int iNumoPictures, int iNumoMethods) {
		pcFeat = new feature_rad2();
		strPath = strDataPath;
		iPic = iNumoPictures;
		iMet = iNumoMethods;
		return 0;
	}
	virtual bool parseData(cv::Mat* pmTargets, vector<cv::Mat>* pmPredicts) {
		std::ostringstream oss;
		char name[256];
		cv::Mat raw, image;
		cv::Mat fv;
		// run for each target
		for (register unsigned int i = 0; i < iPic; i++) {
			cout << "\tprocessing target " << i;
			// reset stream
			oss.str(std::string());
			// format subfolder and file name
			sprintf_s(name, "\\targets\\%03d", i);
			// create path to file
			oss << strPath << name << ".png";
			// read b&w image and preprocess
			raw = cv::imread(oss.str(), cv::IMREAD_GRAYSCALE);
			if (raw.cols == 0 || raw.rows == 0) {
				cout << "invalid image - SKIPPED" << endl;
			}
			else {
				cout << endl;
				image = preprocess(raw);
				// calculate row feature vector(s)
				fv = pcFeat->extract(image);
				// push vector(s) to databank as new most bottom row(s)
				pmTargets->push_back(fv);
			}
		}
		// run for each method
		for (register unsigned int i = 0; i < iMet; i++) {
			// for each predict
			Mat temp;
			for (register unsigned int j = 0; j < iPic; j++) {
				cout << "\tprocessing method " << i << " for image " << j << endl;
				// reset stream
				oss.str(std::string());
				// format subfolder and file name
				sprintf_s(name, "\\predicts\\%03d\\%03d", i, j);
				// create path to file
				oss << strPath << name << ".png";
				// read b&w image and preprocess
				raw = cv::imread(oss.str(), cv::IMREAD_GRAYSCALE);
				if (raw.cols == 0 || raw.rows == 0) {

				}
				image = preprocess(raw);
				// calculate row feature vector(s)
				fv = pcFeat->extract(image);
				// push vectors to databank as new most bottom row(s)
				temp.push_back(fv);
			}
			pmPredicts->push_back(temp);
			temp = Mat();
		}
		return 0;
	}
	// ACC
public:
	void setPicNum(int iPicNumber) {
		iPic = iPicNumber;
	}
	unsigned int getPicNum() const {
		return iPic;
	}
	void setMetNum(int iMetNumber) {
		iMet = iMetNumber;
	}
	unsigned int getMetNum() const {
		return iMet;
	}
	// ATR
private:
	unsigned int iPic;
	unsigned int iMet;
	std::string strPath;
	feature_rad2* pcFeat;
	// HELP
private:
	Mat preprocess(Mat rawImage) {
		resize(rawImage, rawImage, Size(200, 200));
		return rawImage;
	}
};
