#pragma once
#include "include.h"
// components
#include "iSplitter.h"
#include "splitCrossValidation.h"
#include "splitRandom.h"
#include "splitBegin.h"
#include "iMatch.h"
#include "matchSVM.h"
#include "matchNB.h"
#include "matchKNN.h"
#include "matchANN.h"
#include "matchLR.h"
#include "matchFLANN.h"

class iProcessor {
	// D
public:
	iProcessor() {

	}

	// MAIN
public:
	virtual bool init(string strMatcher, string strSplitter, unsigned int iFlags = 0, string strModelPath = "") = 0;
	virtual bool processData(cv::Mat* pmData, cv::Mat* pmLabels, cv::Mat* pmResults) = 0;
};