//
// isWithin.h
//
// Created by Johann Gan on 6/2/17
//
// Supporting function for TryCuts. Given a number and a 2-d vector<double> defining an interval, checks if the number is
// 		within the interval.
//
// The vector<double> should contain {var_lo, var_hi}. The interval is defined as [var_lo, var_hi)

//////////////////////////////////////

#ifndef ISWITHIN_H_
#define ISWITHIN_H_

//////////////////////////////////////

// Dependencies
#include <vector>

//////////////////////////////////////

using namespace std;


bool isWithin(double n, const vector<double>& interval)
{
    if(n >= interval[0] && n < interval[1])
    	return true;

    return false;
}

#endif /* ISWITHIN_H_ */
