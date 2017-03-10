#pragma once
#include "include.h"

// template for all classification methods
class iExporter {
	// C&D
public:
	virtual ~iExporter() {}

	// ACC
public:
	virtual bool exportData(Mat* pmData, string strPath) = 0;// return value?
};