#pragma once
#include "iParser.h"

class parserFileTree : public iParser {
	// C&D
public:
	parserFileTree():iClassN(0), iPicN(0), iFlag(0), pcFeat(NULL){

	}
	~parserFileTree() {
		if (pcFeat) {
			delete pcFeat;
			pcFeat = NULL;
		}
	}
	// MAIN
public:
	virtual bool init(std::string strDataPath, string strFeature, unsigned int iFlags = 0) {
		if (!strcmp(strFeature.c_str(), "canny")) {
			pcFeat = new feature_canny();
		}
		else if(!strcmp(strFeature.c_str(), "hough")) {
			pcFeat = new feature_hough();
		}
		else {
			debug::error(2, "Unknow feature extraction method.");
			return 1;
		}
		iFlag = iFlags;
		strPath = strDataPath;
		return 0;
	}
	virtual bool parseData(cv::Mat* pmData, cv::Mat* pmLabels) {
		std::ostringstream oss;
		char name[256];
		cv::Mat image;
		cv::Mat fv;
		// run for each picture
		for (register unsigned int i = 0; i < iClassN; i++) {
			for (register unsigned int j = 0; j < iPicN; j++) {
				// reset stream
				oss.str(std::string());
				// format subfolder and file name
				sprintf_s(name, "%03d\\%03d", i, j);
				// create path to file
				oss << strPath << name << ".png";
				// read b&w image
				image = cv::imread(oss.str(), cv::IMREAD_GRAYSCALE);
				// calculate row feature vector(s)
				fv=pcFeat->extract(image);
				// push vector(s) to databank as new most bottom row(s)
				pmData->push_back(fv);
				// add labels for each new f. vector
				for (register unsigned int l = 0; l<fv.rows; l++) {
					pmLabels->push_back((double)i);
				}
			}
		}
		return 0;
	}
	// ACC
public:
	void setClassNum(int iClassNumber) {
		iClassN = iClassNumber;
	}
	void setPictureNum(int iPictureNum) {
		iPicN = iPictureNum;
	}
	unsigned int getClassNum() const {
		return iClassN;
	}
	unsigned int getPictureNum() const {
		return iPicN;
	}
	// ATR
private:
	unsigned int iClassN;
	unsigned int iPicN;
	unsigned int iFlag;
	std::string strPath;
	iFeature* pcFeat;
};
