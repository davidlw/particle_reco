//
// loadChain.h
// Utilities/
// 
// Created by Johann Gan on 6/9/17
// 
// Loads a TChain with multiple trees given a file name skeleton and index limits/an index list.
//
// Note: Modifies the input chain.

//////////////////////////////////////

#ifndef LOADCHAIN_H_
#define LOADCHAIN_H_

//////////////////////////////////////

// Dependencies
#include <vector>
#include <string>
#include <memory>

#include <TChain.h>

//////////////////////////////////////

using namespace std;


void loadChain(unique_ptr<TChain>&, const string&, const vector<string>&);
void loadChain(unique_ptr<TChain>&, const string&, const vector<int>&);
void loadChain(unique_ptr<TChain>&, const string&, int, int);

#endif