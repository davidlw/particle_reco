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
#include "compareTrk.h"

using namespace std;
using namespace ple_definition;
using namespace calcKinInfo;
using namespace compareTrk_definition;

namespace recoData_constants {

//particle information
const double dau1mass = 0.497611;	//Ks
const double dau2mass = 0.9382720813;	//proton

}	//namespace recoData_constants

//function prototypes
void recoData(const vector<Float_t>&, TNtuple*, TNtuple*, TNtuple*);
void recoMass(const ple&, const ple&, ple&);

#endif