#ifndef RUNTMVA_CONFIG_H_
#define RUNTMVA_CONFIG_H_

#include <cmath>
#include <vector>
#include <string>
#include "TString.h"
#include "TMath.h"

//========== Shared configuration for TMVAClassification.C and readxml.cc ==========//
namespace runTMVA_shared_config {

// File information
const TString signalFileName = "/home/jg61/store/outputs/AODSIM_NEW/MC_LambdaC_KsProton_gen_match_bkg_merged.root";
const TString signalTreePath = "lcreco/lambdac_match";

// Cutting information
const std::vector<Double_t> ptBinLimits = {2, 4};

// Base cuts (not tuning cuts)
const double absEtaMax = 1;
const double ptD1Min = 0.5;
const double absEtaD1Max = 2.4;
const double cosAngleD1Min = 0.999;
const double DLSigD1Min = 5;
const double absMassD1zScoreMax = 3;
const double ptD2Min = 0.3;
const double absEtaD2Max = 2.4;

const double massWindowLo = 2.1;
const double massWindowHi = 2.5;

// const double massD1Mean = 1.116;
// const double massD1Sigma = 0.0028;
const double massD1Mean = 0.498;
const double massD1Sigma = 0.0067;

const bool use_dedx = true;
const double dedx_mass = 0.9382720813;
const std::vector<double> dedx_kca_hi = {3.5, 4.2, 0.1};
const double dedx_cutoff_hi = 2.5;
const std::vector<double> dedx_kca_low = {2.6, 2.2, -0.1};
const double dedx_cutoff_low = 2.2;
const int nhits_min = 11;    // >= is used

const std::vector<TString> variableNamesList = {
    "pt_dau1",
    "pt_dau2",
    "abs(eta_dau1)",
    "abs(eta_dau2)"
};



// Calculated values
const double massWindowMid = (massWindowLo + massWindowHi) / 2;
const double massWindowHalfLength = massWindowHi - massWindowMid;

} // namespace runTMVA_shared_config


//========== Configuration only for TMVAClassification.C ==========//
namespace TMVAClassification_config {

// File information
const TString outfileName = "TMVA.root";

// Input
const TString backgroundFileName = "/home/jg61/store/outputs/data/data_KsProton_pt_2to4_nomassd1_smaller2_skim.root";
const TString backgroundTreePath = "lambdac";

const Double_t signalWeight = 1.0;
const Double_t backgroundWeight = 1.0;

// Cutting information -- DO NOT INCLUDE THE PARENT PT CUTS (they will be added automatically using the "ptBinLimits" constant defined in the shared configuration)
const TCut baseSignalCuts = (
    "abs(eta) <" + std::to_string(runTMVA_shared_config::absEtaMax) + \
    "&& pt_dau1 >" + std::to_string(runTMVA_shared_config::ptD1Min) + \
    "&& abs(eta_dau1) <" + std::to_string(runTMVA_shared_config::absEtaD1Max) + \
    "&& cos(3DPointingAngle_dau1) >" + std::to_string(runTMVA_shared_config::cosAngleD1Min) + \
    "&& 3DDecayLengthSig_dau1 >" + std::to_string(runTMVA_shared_config::DLSigD1Min) + \
    "&& abs((mass_dau1 -" + std::to_string(runTMVA_shared_config::massD1Mean) + ") /" + \
        std::to_string(runTMVA_shared_config::massD1Sigma) + ") <=" + std::to_string(runTMVA_shared_config::absMassD1zScoreMax) + \
    "&& pt_dau2 >" + std::to_string(runTMVA_shared_config::ptD2Min) + \
    "&& abs(eta_dau2) <" + std::to_string(runTMVA_shared_config::absEtaD2Max) + \

    "&& ( !" + std::to_string(runTMVA_shared_config::use_dedx) + \
        "|| (nhits_dau2 >=" + std::to_string(runTMVA_shared_config::nhits_min) + \
        "&& (dEdxHarmonic_dau2 >=" + std::to_string(runTMVA_shared_config::dedx_kca_low[0]) + "* (" + \
        std::to_string(runTMVA_shared_config::dedx_mass) + "/ (p_dau2 -" + \
        std::to_string(runTMVA_shared_config::dedx_kca_low[2]) + "))^2 +" + \
        std::to_string(runTMVA_shared_config::dedx_kca_low[1]) + "|| p_dau2 >=" + \
        std::to_string(runTMVA_shared_config::dedx_cutoff_low) + \
        ") && (dEdxHarmonic_dau2 <" + std::to_string(runTMVA_shared_config::dedx_kca_hi[0]) + "* (" + \
        std::to_string(runTMVA_shared_config::dedx_mass) + "/ (p_dau2 -" + \
        std::to_string(runTMVA_shared_config::dedx_kca_hi[2]) + "))^2 +" + \
        std::to_string(runTMVA_shared_config::dedx_kca_hi[1]) + "|| p_dau2 >=" + \
        std::to_string(runTMVA_shared_config::dedx_cutoff_hi) + ") ) )"
    ).c_str();

const TCut baseBackgroundCuts = ("abs(mass -" + std::to_string(runTMVA_shared_config::massWindowMid) + ") <" + \
    std::to_string(runTMVA_shared_config::massWindowHalfLength)).c_str() + baseSignalCuts;
const std::string ptBranchName = "pt";    // For adding the pT bin to the cuts

// Direction to vary each variable (corresponds to variableNamesList in the shared configuration)
// "FMax" means vary from the base value (in the base cuts) to infinity.
// "FMin" means vary from the base value (in the base cuts) to zero.
const std::vector<TString> variableProps = {
    "FMax",
    "FMax",
    "FMin",
    "FMin"
};

} // namespace TMVAClassification_config


//========== Configuration only for readxml ==========//
namespace readxml_config {

// File information
const std::string myTMVApath = "../myTMVA_K_2_4";
const TString histoOutPathBase = "./readxml_histos";

// Cutting information

// Comparators ("<", "<=", ">", or ">=") for each tuning variable (corresponds to variableNamesList in the shared configuration) 
const std::vector<TString> variableComparators = {
    ">",
    ">",
    "<",
    "<"
};

// Names of branches to be used in cutting or filling
const std::vector<TString> branchNames = {
    "mass",
    "pt",
    "eta",
    "pt_dau1",
    "eta_dau1",
    "3DPointingAngle_dau1",
    "3DDecayLengthSig_dau1",
    "mass_dau1",
    "pt_dau2",
    "eta_dau2",
    "nhits_dau2",
    "dEdxHarmonic_dau2",
    "p_dau2"
};
enum branch {m, pt, eta, ptd1, etad1, angle, dl, md1, ptd2, etad2, nhits, dedx, pd2};


// Boolean functions that correspond with different sets of cuts.

// Base cut functions
// Take in a vector of branch values (corresponding to branchNames), and return true if all the cuts in the set are satisfied and false if not.
bool passesBaseSignalCuts(const std::vector<Float_t>& branches)
{
    if(!(branches[pt] >= runTMVA_shared_config::ptBinLimits[0] && branches[pt] < runTMVA_shared_config::ptBinLimits[1])) return false;
    if(fabs(branches[eta]) >= runTMVA_shared_config::absEtaMax) return false;
    if(branches[ptd1] <= runTMVA_shared_config::ptD1Min) return false;
    if(fabs(branches[etad1]) >= runTMVA_shared_config::absEtaD1Max) return false;
    if(cos(branches[angle]) <= runTMVA_shared_config::cosAngleD1Min) return false;
    if(branches[dl] <= runTMVA_shared_config::DLSigD1Min) return false;
    if(fabs( (branches[md1] - runTMVA_shared_config::massD1Mean) / runTMVA_shared_config::massD1Sigma ) > \
        runTMVA_shared_config::absMassD1zScoreMax) return false;
    if(branches[ptd2] <= runTMVA_shared_config::ptD2Min) return false;
    if(fabs(branches[etad2]) >= runTMVA_shared_config::absEtaD2Max) return false;

    // Is dedx cuts are not desired, then at this point return true. If not, do one last cut.
    if(!runTMVA_shared_config::use_dedx) return true;
    
    if( !(  (branches[nhits] >= runTMVA_shared_config::nhits_min) && \
            (branches[dedx] >= runTMVA_shared_config::dedx_kca_low[0] * \
                pow(runTMVA_shared_config::dedx_mass / (branches[pd2] - runTMVA_shared_config::dedx_kca_low[2]), 2) + \
                runTMVA_shared_config::dedx_kca_low[1] || branches[pd2] >= runTMVA_shared_config::dedx_cutoff_low) &&
            (branches[dedx] < runTMVA_shared_config::dedx_kca_hi[0] * \
                pow(runTMVA_shared_config::dedx_mass / (branches[pd2] - runTMVA_shared_config::dedx_kca_hi[2]), 2) + \
                runTMVA_shared_config::dedx_kca_hi[1] || branches[pd2] >= runTMVA_shared_config::dedx_cutoff_hi)
        )
    ) return false;

    return true;
}

bool passesBaseBackgroundCuts(const std::vector<Float_t>& branches)
{
    if(fabs(branches[m] - runTMVA_shared_config::massWindowMid) >= runTMVA_shared_config::massWindowHalfLength) return false;

    return passesBaseSignalCuts(branches);
}

// Tuning cut function
// Takes in a vector of branch values (corresponding to branchNames) and a vector of cut values (with same number of elements as the number of tuning variables), 
//     and return true if the cuts are satisifed and false if not.
bool passesVariableCuts(const std::vector<Float_t>& branches, const std::vector<Double_t>& cutVals)
{
    if(branches[ptd1] <= cutVals[0]) return false;
    if(branches[ptd2] <= cutVals[1]) return false;
    if(fabs(branches[etad1]) >= cutVals[2]) return false;
    if(fabs(branches[etad2]) >= cutVals[3]) return false;

    return true;
}

// Histogram information
const Double_t targetBinSize = 5e-3;  // Desired bin size (may not work exactly)

// Misc. information
const int nEff = 100;
const int nMaxVar = runTMVA_shared_config::variableNamesList.size();



// Calculated values
const Int_t nMassBins = ceil( (runTMVA_shared_config::massWindowHi - runTMVA_shared_config::massWindowLo) / targetBinSize );
const Double_t massBinSize = (runTMVA_shared_config::massWindowHi - runTMVA_shared_config::massWindowLo) / nMassBins; // Actual bin size

} // namespace readxml_config



//========== Configuration only for readxml_fit ==========//
namespace readxml_fit_config {

// File information
const TString imageType = ".png";

// Fitting information
const Double_t peakStDev = 2;
const Double_t peakMass = 2.28646;

// Labeling information
const TString dau1name = "K_{s}^{0}";
const TString dau2name = "p";


} // namespace readxml_fit_config


#endif
