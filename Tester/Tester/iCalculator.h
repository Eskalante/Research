#pragma once
#include "include.h"

// template for all classification methods
class iCalculator {
	// C&D
public:
	virtual ~iCalculator() {}

	// ACC
public:
	virtual Mat calculate(Mat* pmResults) = 0;// return value?
};