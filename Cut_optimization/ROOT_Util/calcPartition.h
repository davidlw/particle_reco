//
// calcPartition.h
// Utilities/
// 
// Created by Johann Gan on 6/3/17
// 
// 
// Calculates the best way to partition a grid into cells given the number of elements to be placed in the cells.
// Returns a vector with the <x, y> dimension sizes in that order.
// Returns {-1, -1} if something goes wrong.
//
// Criteria:
// 1. (number of rows) * (number of columns) >= n
// 2. (number of columns) >= (number of rows)
// 3. (number of columns) / (number of rows) <= 2 OR (number of columns) - (number of rows) <= 2
// 4. Minimize the ratio of unused cells to row length ( (n - (number of rows * number of columns)) / (number of columns) )

//////////////////////////////////////

#ifndef CALCPARTITION_H_
#define CALCPARTITION_H_

//////////////////////////////////////

// Dependencies
#include <vector>
#include <cmath>
#include <algorithm>

//////////////////////////////////////

using namespace std;


vector<int> calcPartition(int);

#endif