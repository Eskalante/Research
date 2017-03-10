#pragma once
#include "iSplitter.h"

class splitCrossValidation : public iSplitter {
	// C&D
public:
	splitCrossValidation() : dRatio(0) {

	}
	~splitCrossValidation() {

	}
	// MAIN
public:
	virtual bool duoRatio(double dDuoRatio) {
		dRatio = dDuoRatio;
		return 0;
	}
	virtual duo splitDuo(Mat* pmData, Mat* pmLabels) {
		// check if input is valid
		if (!pmData) {
			return duo();
		}
		if (!pmLabels) {
			return duo();
		}
		if (pmData->rows != pmLabels->rows) {
			return duo();
		}
		// find number of pictures per class
		unsigned int picNumber = 0;
		while (!pmLabels->at<double>(picNumber,0)) {
			picNumber++;
		}
		// find number of classes
		double classNum = 0.0;
		for (register unsigned int i = 0; i < pmData->rows; i++) {
			if (classNum < pmLabels->at<double>(i, 0)) {
				classNum = pmLabels->at<double>(i, 0);
			}
		}
		classNum++;
		time_t rawtime;
		time(&rawtime);
		duo ret;
		RNG rng(rawtime);

		for (register unsigned int i = 0; i < classNum; i++) {
			vector<int> transfer;
			std::vector<int>::iterator it;
			while (transfer.size() < (picNumber *(dRatio / 100.0))) {
				int candidate = rng.uniform((int)(i*picNumber), ((i + 1)*picNumber) - 1);
				it = find(transfer.begin(), transfer.end(), candidate);
				if (it == transfer.end()) {
					transfer.push_back(candidate);
					//cout << candidate << "\t";
				}
			}
			//cout << endl;
			for (register unsigned int j = ((int)(i*picNumber)); j < ((i + 1)*picNumber); j++) {
				it = find(transfer.begin(), transfer.end(), j);
				if (it != transfer.end()) {
					ret.testData.push_back(pmData->row(j));
					ret.testLabel.push_back(pmLabels->row(j));
				}
				else {
					ret.trainData.push_back(pmData->row(j));
					ret.trainLabel.push_back(pmLabels->row(j));
				}
			}
		}
		return ret;
	}
	// ATR
private:
	double dRatio;
};
