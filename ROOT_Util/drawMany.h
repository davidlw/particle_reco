// 
// drawMany.h
// Utilities/
// 
// Created by Johann Gan on 6/3/17
// 
// 
// Draws multiple pads onto one single canvas. Returns the combined canvas.
//
// The pads to draw are passed in as a vector.
//
// The number of rows and columns in the canvas partition can be specified manually
// or calculated automatically based on the size of the vector.
//
// Pass in -1 for rows and columns for automatic calculation.

//////////////////////////////////////

#ifndef DRAWMANY_H_
#define DRAWMANY_H_

//////////////////////////////////////

// Dependencies
#include <string>
#include <vector>
#include <memory>

#include <TCanvas.h>

#include "calcPartition.h"

//////////////////////////////////////

using namespace std;


unique_ptr<TCanvas> drawMany(const vector< unique_ptr<TCanvas> >&, int cols = -1, int rows = -1, \
	const string& name = "", const string& title = "", int width = -1, int height = -1);


#endif