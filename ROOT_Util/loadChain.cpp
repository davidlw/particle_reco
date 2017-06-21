//
// loadChain.cpp
// Utilities/
// 
// Created by Johann Gan on 6/9/17

//////////////////////////////////////

#include "loadChain.h"

//////////////////////////////////////


void loadChain(unique_ptr<TChain>& chain, const string& path_base, const vector<string>& indices)
{
	// Loads a TChain with multiple trees given a file name skeleton and an index list (strings).
	//
	// The name skeleton should be a full path to the trees to be retrieved, with a "{}" in place of the index.
	//
	// Note: Modifies the input chain.

	for(auto i = indices.begin(); i < indices.end(); i++)
	{
		string path = path_base;

		if(path.find("{}") < path.length())
			path.replace( path.find("{}"), 2, *i);

		chain->Add(path.c_str());
	}
}

void loadChain(unique_ptr<TChain>& chain, const string& path_base, const vector<int>& indices)
{
	// Loads a TChain with multiple trees given a file name skeleton and an index list (ints).
	//
	// The name skeleton should be a full path to the trees to be retrieved, with a "{}" in place of the index.
	//
	// Note: Modifies the input chain.

	for(auto i = indices.begin(); i < indices.end(); i++)
	{
		string path = path_base;
		path.replace( path.find("{}"), 2, to_string(*i) );

		chain->Add(path.c_str());
	}
}

void loadChain(unique_ptr<TChain>& chain, const string& path_base, int idxlo, int idxhi)
{
	// Loads a TChain with multiple trees given a file name skeleton and index limits.
	//
	// The name skeleton should be a full path to the trees to be retrieved, with a "{}" in place of the index.
	//
	// Note: Modifies the input chain.

	// Generates an index list from the limits and calls the other function overload.
	vector<int> indices;
	indices.reserve(idxhi - idxlo + 1);

	for(int i = idxlo; i <= idxhi; i++)
	{
		indices.push_back(i);
	}

	loadChain(chain, path_base, indices);
}
