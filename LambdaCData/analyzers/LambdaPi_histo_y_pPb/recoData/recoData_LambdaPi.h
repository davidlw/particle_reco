#ifndef RECODATA_H_
#define RECODATA_H_

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#include "TFile.h"
#include "TCanvas.h"
#include "TNtuple.h"
#include "TH1.h"
#include "TMath.h"

#include "ple.h"
#include "calcKinInfo.h"

using namespace std;
using namespace ple_definition;
using namespace calcKinInfo;

namespace recoData_constants {

//particle information
const double dau1mass = 1.115683;	//Lambda
const double dau2mass = 0.13957018;	//pion

}	//namespace recoData_constants

//function prototypes
void recoData(TNtuple*, TNtuple*, vector<TH1D*>&, vector<TH1D*>&);
void recoMass(const ple&, const ple&, ple&);

#endif