#include <iostream>
#include <vector>

#include "Configuration.h"

#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TMath.h"
#include "TFile.h"
#include "TString.h"
#include "TLine.h"
#include "TLatex.h"
#include "RooFit.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"

#include <fstream>

using namespace Configuration;
using namespace RooFit;
using namespace std;

void TryCuts_fit(const string&, const string& output_tag = "");