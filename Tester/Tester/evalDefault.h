#pragma once
// c++
#include "include.h"
// components
#include "iEvaluator.h"

class evalDefault: public iEvaluator {
	// D
public:
	evalDefault() {

	}

	// MAIN
public:
	virtual bool evaluateData(Mat* pmResults, string strCalculator) {
		iCalculator* pcCalculator;
		if (!strcmp(strCalculator.c_str(), "confusion")) {
			pcCalculator = new calcConfusion();
		}
		else if (!strcmp(strCalculator.c_str(), "prf")) {
			pcCalculator = new calcPRF();
		}
		else {
			debug::error(2, "Unknow calculator.");
			return 1;
		}
		Mat out = pcCalculator->calculate(pmResults);
		cout << out << endl;
		delete pcCalculator;
		return 0;
	}
	virtual bool evaluateAndExport(Mat* pmResults, string strCalculator, string strExport, string strPath) {
		if (evaluateData(pmResults, strCalculator)) {
			return 1;
		}
		iExporter* pcExport;
		if (!strcmp(strExport.c_str(), ""))
			;// new exporter
		else {
			debug::error(2, "Unknow exporter.");
			return 1;
		}
		// export
		return 0;
	}
};