// 
// drawMany.cpp
// Utilities/
// 
// Created by Johann Gan on 6/3/17
//

//////////////////////////////////////

#include "drawMany.h"

//////////////////////////////////////


unique_ptr<TCanvas> drawMany(const vector< unique_ptr<TCanvas> >& pads, int cols, int rows, \
	const string& name, const string& title, int width, int height)
{
	// Draws multiple pads onto one single canvas. Returns the combined canvas.
	//
	// The pads to draw are passed in as a vector.
	//
	// The number of rows and columns in the canvas partition can be specified manually
	// or calculated automatically based on the size of the vector.
	//
	// Pass in -1 for rows and columns for automatic calculation.
	

	// Set up the combined canvas
	unique_ptr<TCanvas> c;

	if(name.length() == 0)
		c.reset( new TCanvas );
	else if(width < 0 || height < 0)
		c.reset( new TCanvas(name.c_str(), title.c_str()) );
	else
		c.reset( new TCanvas(name.c_str(), title.c_str(), width, height) );


	if(cols == -1 || rows == -1)
	{
		vector<int> dims = calcPartition(pads.size());
		cols = dims[0];
		rows = dims[1];
	}

	c->Divide(cols, rows);


	// Draw the pads onto one canvas and return the final canvas
	for(int i = 0; i < pads.size(); i++)
	{
		c->cd(i + 1);
		pads[i]->DrawClonePad();
	}

	return c;
}