//
// cutTree.h
// Utilities/
// 
// Created by Johann Gan on 6/5/17
// 
// 
// Template Function. Cuts a specified subset of a tree by a given set of cuts.
//

//////////////////////////////////////

#ifndef CUTTREE_H_
#define CUTTREE_H_

//////////////////////////////////////

// Dependencies
#include <string>
#include <memory>

#include <TTree.h>
#include <TNtuple.h>
#include <TChain.h>

//////////////////////////////////////

using namespace std;


template <class T>
unique_ptr<T> cutTree(const unique_ptr<T>& original, const string& cuts, Long64_t nEntries = -1)
{
	unique_ptr<T> newtree( (T*)(original->CloneTree(nEntries)->CopyTree(cuts.c_str())) );

	return newtree;
}


#endif
