#include "uti.h"
#include "../../runTMVA_config.h"
#include "../config_util/InputChain.h"
using namespace runTMVA_shared_config;
using namespace readxml_config;

Double_t effS[nEff], effB[nEff];

std::vector<TString> cuts;
std::vector< std::vector<Double_t> > cutval(nMaxVar);
std::vector<TString> varval(nMaxVar);

Float_t ptmin, ptmax, raa;
