// 
// doubleToStr.cpp
// Utilities/
// 
// Created by Johann Gan on 6/2/17
// 

//////////////////////////////////////

#include "doubleToStr.h"

//////////////////////////////////////



string doubleToStr(double d, const string& dot)
{
	// Converts a double to a string and removes trailing zeros.
	// Replaces the decimal point with a given string if specified.


	// Convert to string
	stringstream ss;
	ss << d;
	string dstr = ss.str();

	// Replace decimal point
	int dot_pos = dstr.find(".");
	if(dot_pos < dstr.length())
		dstr.replace( dot_pos, 1, dot );

    return dstr;
}
