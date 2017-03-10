#pragma once
// c++
#include "include.h"
// components
#include "iCalculator.h"
#include "calcConfusion.h"
#include "calcPRF.h"
#include "iExporter.h"

class iEvaluator {
	// D
public:
	iEvaluator() {

	}

	// MAIN
public:
	virtual bool evaluateData(Mat* pmResults, string strCalculator) = 0;
	virtual bool evaluateAndExport(Mat* pmResults, string strCalculator, string strExport, string strPath) = 0;
};