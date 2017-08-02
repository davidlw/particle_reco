//============================================================================
// Name        : TryCuts.h
// Author      : Johann Gan
// Version     :
// Copyright   :
// Description : Reads in trees into a TChain and outputs mass histograms with various different cuts applied.
//============================================================================

#ifndef TRYCUTS_H_
#define TRYCUTS_H_

//============================================================================

// Dependencies
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <ctime>

#include "TNtuple.h"
#include "TH1D.h"
#include "TFile.h"
#include "TMath.h"

#include "Configuration.h"
#include "InputChain.h"
#include "doubleToStr.h"
#include "isWithin.h"
#include "rapidity.h"

//============================================================================

using namespace std;

void iterVarOut(ofstream& out, const string& var_name, const vector<double>& var_lims);


#endif /* TRYCUTS_H_ */
