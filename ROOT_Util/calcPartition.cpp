//
// calcPartition.cpp
// Utilities/
// 
// Created by Johann Gan on 6/3/17
//

//////////////////////////////////////

#include "calcPartition.h"

//////////////////////////////////////


vector<int> calcPartition(int n)
{
	// Calculates the best way to partition a canvas into grids given the number of elements needed to be drawn.
	// Returns a vector with the <x, y> dimension sizes in that order.
	// Returns {-1, -1} if something goes wrong.
	//
	// Criteria:
	// 1. (number of rows) * (number of columns) >= n
	// 2. (number of columns) >= (number of rows)
	// 3. (number of columns) / (number of rows) <= 2 OR (number of columns) - (number of rows) <= 2
	// 4. Minimize the ratio of unused cells to row length ( ((number of rows * number of columns) - n) / (number of columns) )


	double maxr = 2;	// Maximum acceptable ratio between number of columns (x) and number of rows (y)
	int maxd = 2;	// Maximum acceptable difference between x and y


	vector<int> bestxy = {-1, -1};	// Best x, y
	double bestratio = n + 1;	// Best (minimum) ratio of condition (4) found so far

	for(int x = ceil( pow(n, 0.5) ); x <= n; x++)
	{
		for(int y = max( ceil(static_cast<double>(n) / x), min( ceil(x / maxr), static_cast<double>(max(x - maxd, 1)) ) ); \
				y <= x; y++)
			// max(x - maxd, 1) ensures that y is not negative.
			// min( ceil(x / maxr), max(static_cast<double>(x - maxd), 1.0) ) ensures condition (3) is satisfied.
			// max( ceil(n / x), * ) ensures that condition (1) is jointly satisfied with condition (3).
			// y <= x is condition (2).
		{
			double ratio = (x * y - n) / static_cast<double>(x);
			if(ratio < bestratio)
			{
				bestxy = {x, y};
				bestratio = ratio;
			}
		}
	}

	return bestxy;
}