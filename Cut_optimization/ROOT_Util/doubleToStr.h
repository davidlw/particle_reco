// 
// doubleToStr.h
// Utilities/
// 
// Created by Johann Gan on 6/2/17
// 
// 
// Converts a double to a string and removes trailing zeros.
// Replaces the decimal point with a given string if specified.

//////////////////////////////////////

#ifndef DOUBLETOSTR_H_
#define DOUBLETOSTR_H_

//////////////////////////////////////

// Dependencies
#include <string>
#include <sstream>

//////////////////////////////////////

using namespace std;


string doubleToStr(double d, const string& dot = ".");



#endif
