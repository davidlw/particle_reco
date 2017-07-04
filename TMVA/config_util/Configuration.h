/*
 * Configuration.h
 *
 *  Created on: Jun 13, 2017
 *      Author: johanngan
 *
 *  Class for reading configuration information from an .xml file
 */

//============================================================================

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//============================================================================

// Dependencies
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
    
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

#include "TCut.h"
#include "TMath.h"

#include "strToVect.h"

//============================================================================

enum branch {m, pt, eta, ptd1, etad1, angle, dl, md1, ptd2, etad2, xydca, zdca, nhits, dedx, pd2};

namespace Configuration_TMVA {

class Configuration {
public:
    // *** Shared configuration *** //
    string signalFileName;
    string signalTreePath;

    // Cutting information
    vector<double> ptBinLimits;

    // Base cuts (not tuning cuts)
    double absEtaMax;
    double ptD1Min;
    double absEtaD1Max;
    double cosAngleD1Min;
    double DLSigD1Min;
    double absMassD1zScoreMax;
    double ptD2Min;
    double absEtaD2Max;
    double absxyDCASigD2Max;
    double abszDCASigD2Max;

    double massWindowLo;
    double massWindowHi;
    // Calculated
    double massWindowMid;
    double massWindowHalfLength;


    double massD1Mean;
    double massD1Sigma;

    bool use_dedx;
    double dedx_mass;
    vector<double> dedx_kca_hi;
    double dedx_cutoff_hi;
    vector<double> dedx_kca_lo;
    double dedx_cutoff_lo;
    int nhits_min;

    vector<string> variableNamesList;


    // *** Configuration only for TMVAClassification.C *** //

    // File information
    string outfileName;

    string backgroundFileName;
    string backgroundTreePath;

    double signalWeight;
    double backgroundWeight;

    TCut baseSignalCuts;
    TCut baseBackgroundCuts;
    string ptBranchName;

    vector<string> variableProps;


    // *** Configuration only for readxml *** //

    // File information
    string myTMVApath;
    string histoOutPathBase;

    // Names of branches to be used in cutting
    vector<string> branchNames;

    // Deduced
    vector<bool> isVariableCut;
    vector<string> variableComparators;

    // Histogram information
    double targetBinSize;  // Desired bin size (may not work exactly)

    // Misc. information
    int nEff;
    int nMaxVar;


    // Boolean functions that correspond with different sets of cuts.

    // Base cut functions
    // Take in a vector of branch values (corresponding to branchNames), and return true if all the cuts in the set are satisfied and false if not.
    bool passesBaseSignalCuts(const vector<Float_t>& branches)
    {
        if(!(branches[pt] >= ptBinLimits[0] && branches[pt] < ptBinLimits[1])) return false;
        if(fabs(branches[eta]) >= absEtaMax) return false;
        if(branches[ptd1] <= ptD1Min) return false;
        if(fabs(branches[etad1]) >= absEtaD1Max) return false;
        if(cos(branches[angle]) <= cosAngleD1Min) return false;
        if(branches[dl] <= DLSigD1Min) return false;
        if(fabs( (branches[md1] - massD1Mean) / massD1Sigma ) > absMassD1zScoreMax) return false;
        if(branches[ptd2] <= ptD2Min) return false;
        if(fabs(branches[etad2]) >= absEtaD2Max) return false;
        if(fabs(branches[xydca]) >= absxyDCASigD2Max) return false;
        if(fabs(branches[zdca]) >= abszDCASigD2Max) return false;

        // If dedx cuts are not desired, then at this point return true. If not, do one last cut.
        if(!use_dedx) return true;
        
        if( !(  (branches[nhits] >= nhits_min) && \
                (branches[dedx] >= dedx_kca_lo[0] * \
                    pow(dedx_mass / (branches[pd2] - dedx_kca_lo[2]), 2) + dedx_kca_lo[1] || branches[pd2] >= dedx_cutoff_lo) && \
                (branches[dedx] < dedx_kca_hi[0] * pow(dedx_mass / (branches[pd2] - dedx_kca_hi[2]), 2) + \
                    dedx_kca_hi[1] || branches[pd2] >= dedx_cutoff_hi)
            )
        ) return false;

        return true;
    }


    bool passesBaseBackgroundCuts(const std::vector<Float_t>& branches)
    {
        if(fabs(branches[m] - massWindowMid) >= massWindowHalfLength) return false;

        return passesBaseSignalCuts(branches);
    }

    // Tuning cut function
    // Takes in a vector of branch values (corresponding to branchNames) and a vector of cut values (with same number of elements as branches), 
    //     and return true if the cuts are satisifed and false if not. Only look at the cuts if that variable is specified to be for cutting
    //     in isVariableCut.
    bool passesVariableCuts(const std::vector<Float_t>& branches, const std::vector<Double_t>& cutVals)
    {
        if( isVariableCut[eta] && fabs(branches[eta]) >= cutVals[eta] ) return false;
        if( isVariableCut[ptd1] && branches[ptd1] <= cutVals[ptd1] ) return false;
        if( isVariableCut[etad1] && fabs(branches[etad1]) >= cutVals[etad1] ) return false;
        if( isVariableCut[md1] && fabs(branches[md1] - massD1Mean) / massD1Sigma > cutVals[md1] ) return false;
        if( isVariableCut[ptd2] && branches[ptd2] <= cutVals[ptd2] ) return false;
        if( isVariableCut[etad2] && fabs(branches[etad2]) >= cutVals[etad2] ) return false;
        if( isVariableCut[angle] && cos(branches[angle]) <= cutVals[angle] ) return false;
        if( isVariableCut[dl] && branches[dl] <= cutVals[dl] ) return false;
        if( isVariableCut[xydca] && fabs(branches[xydca]) >= cutVals[xydca] ) return false;
        if( isVariableCut[zdca] && fabs(branches[zdca]) >= cutVals[zdca] ) return false;

        return true;
    }

    // Calculated values
    Int_t nMassBins;
    double massBinSize; // Actual bin size


    // *** Configuration only for readxml_fit *** //

    // File information
    string histoFitFile;
    string imageType;

    // Fitting information
    vector<int> entryIdxLims;    // First and last mass_cut histos to fit
    double peakStDev;
    double peakMass;

    // Labeling information
    string dau1name;
    string dau2name;



    // Constructor
    Configuration(const string& input_config_file)
    {
        // Read in configuration parameters from an xml file
        boost::property_tree::ptree cfg;
        boost::property_tree::xml_parser::read_xml(input_config_file, cfg);

        // *** Shared configuration *** //

        // Files
        signalFileName = cfg.get<string>("Shared.Files.signal_file_name");
        signalTreePath = cfg.get<string>("Shared.Files.signal_tree_path");

        // Cuts
        ptBinLimits = strToVect<double>(cfg.get<string>("Shared.Cuts.pt_bin_limits"));

        absEtaMax = cfg.get<double>("Shared.Cuts.abs_eta_max");
        ptD1Min = cfg.get<double>("Shared.Cuts.pt_dau1_min");
        absEtaD1Max = cfg.get<double>("Shared.Cuts.abs_eta_dau1_max");
        cosAngleD1Min = cfg.get<double>("Shared.Cuts.cos_angle_dau1_min");
        DLSigD1Min = cfg.get<double>("Shared.Cuts.decay_length_sig_dau1_min");
        absMassD1zScoreMax = cfg.get<double>("Shared.Cuts.abs_mass_dau1_zscore_max");
        ptD2Min = cfg.get<double>("Shared.Cuts.pt_dau2_min");
        absEtaD2Max = cfg.get<double>("Shared.Cuts.abs_eta_dau2_max");
        absxyDCASigD2Max = cfg.get<double>("Shared.Cuts.abs_xyDCASig_dau2_max");
        abszDCASigD2Max = cfg.get<double>("Shared.Cuts.abs_zDCASig_dau2_max");

        massWindowLo = cfg.get<double>("Shared.Cuts.mass_window_low");
        massWindowHi = cfg.get<double>("Shared.Cuts.mass_window_high");
        massWindowMid = (massWindowLo + massWindowHi) / 2;
        massWindowHalfLength = massWindowHi - massWindowMid;

        massD1Mean = cfg.get<double>("Shared.Cuts.mass_dau1_mean");
        massD1Sigma = cfg.get<double>("Shared.Cuts.mass_dau1_sigma");

        string use_dedx_str = cfg.get<string>("Shared.Cuts.dEdx.use_dedx");
        if(use_dedx_str == "true")
        {
            use_dedx = true;
        }
        else if(use_dedx_str == "false")
        {
            use_dedx = false;
        }
        else
        {
            cout << "Warning: use_dedx was neither \"true\" nor \"false\". Setting to false.\n";
            use_dedx = false;
        }

        dedx_mass = cfg.get<double>("Shared.Cuts.dEdx.dedx_mass");
        dedx_kca_hi = strToVect<double>(cfg.get<string>("Shared.Cuts.dEdx.dedx_kca_high"));
        dedx_cutoff_hi = cfg.get<double>("Shared.Cuts.dEdx.x_cutoff_high");
        dedx_kca_lo = strToVect<double>(cfg.get<string>("Shared.Cuts.dEdx.dedx_kca_low"));
        dedx_cutoff_lo = cfg.get<double>("Shared.Cuts.dEdx.x_cutoff_low");
        nhits_min = cfg.get<double>("Shared.Cuts.dEdx.nhits_dau2_min");

        variableNamesList = strToVect<string>(cfg.get<string>("Shared.Cuts.variable_names_list"));


        // *** TMVAClassification.C configuration *** //

        // Files
        outfileName = cfg.get<string>("myTMVA.Files.out_file_name");
        backgroundFileName = cfg.get<string>("myTMVA.Files.background_file_name");
        backgroundTreePath = cfg.get<string>("myTMVA.Files.background_tree_path");

        signalWeight = cfg.get<double>("myTMVA.Vars.signal_weight");
        backgroundWeight = cfg.get<double>("myTMVA.Vars.background_weight");

        baseSignalCuts = (
            "abs(eta) <" + to_string(absEtaMax) + "&& pt_dau1 >" + to_string(ptD1Min) + \
            "&& abs(eta_dau1) <" + to_string(absEtaD1Max) + "&& cos(3DPointingAngle_dau1) >" + to_string(cosAngleD1Min) + \
            "&& 3DDecayLengthSig_dau1 >" + to_string(DLSigD1Min) + "&& abs((mass_dau1 -" + to_string(massD1Mean) + ") /" + \
                to_string(massD1Sigma) + ") <=" + to_string(absMassD1zScoreMax) + \
            "&& pt_dau2 >" + to_string(ptD2Min) + "&& abs(eta_dau2) <" + to_string(absEtaD2Max) + \
            "&& abs(xyDCASig_dau2) <" + to_string(absxyDCASigD2Max) + "&& abs(zDCASig_dau2) <" + to_string(abszDCASigD2Max) + \

            "&& ( !" + to_string(use_dedx) + \
                "|| (nhits_dau2 >=" + to_string(nhits_min) + \
                "&& (dEdxHarmonic_dau2 >=" + to_string(dedx_kca_lo[0]) + "* (" + to_string(dedx_mass) + "/ (p_dau2 -" + \
                    to_string(dedx_kca_lo[2]) + "))^2 +" + to_string(dedx_kca_lo[1]) + "|| p_dau2 >=" + to_string(dedx_cutoff_lo) + \
                ") && (dEdxHarmonic_dau2 <" + to_string(dedx_kca_hi[0]) + "* (" + to_string(dedx_mass) + "/ (p_dau2 -" + \
                    to_string(dedx_kca_hi[2]) + "))^2 +" + to_string(dedx_kca_hi[1]) + "|| p_dau2 >=" + \
                    to_string(dedx_cutoff_hi) + ") ) )"
            ).c_str();
        baseBackgroundCuts = ("abs(mass -" + to_string(massWindowMid) + ") <" + to_string(massWindowHalfLength)).c_str() + baseSignalCuts;
        ptBranchName = cfg.get<string>("myTMVA.Vars.pt_branch_name");

        variableProps = strToVect<string>(cfg.get<string>("myTMVA.Vars.variable_properties"));


        // *** readxml configuration *** //

        // File information
        myTMVApath = cfg.get<string>("readxml.myTMVA_path");
        histoOutPathBase = cfg.get<string>("readxml.histo_outpath_base");


        branchNames = {
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
            "xyDCASig_dau2",
            "zDCASig_dau2",
            "nhits_dau2",
            "dEdxHarmonic_dau2",
            "p_dau2"
        };

        isVariableCut.resize(branchNames.size());
        variableComparators.reserve(variableNamesList.size());
        for(auto i = variableNamesList.begin(); i < variableNamesList.end(); i++)
        {
            int currentBranch = whichBranch(*i);

            isVariableCut[currentBranch] = true;
            variableComparators.push_back( getComparator(currentBranch) );
        }


        // Histograms
        targetBinSize = cfg.get<double>("readxml.target_bin_size");
        nMassBins = ceil( (massWindowHi - massWindowLo) / targetBinSize );
        massBinSize = (massWindowHi - massWindowLo) / nMassBins; // Actual bin size

        // Misc.
        nEff = cfg.get<double>("readxml.n_eff");
        nMaxVar = variableNamesList.size();



        // *** readxml_fit configuration *** //

        // Files
        histoFitFile = cfg.get<string>("Fit.in_file");
        imageType = cfg.get<string>("Fit.image_type");

        // Fitting
        entryIdxLims = strToVect<int>(cfg.get<string>("Fit.entry_index_limits"));
        peakStDev = cfg.get<double>("Fit.peak_stdev");
        peakMass = cfg.get<double>("Fit.peak_mass");

        // Labeling
        dau1name = cfg.get<string>("Fit.dau1_name");
        dau2name = cfg.get<string>("Fit.dau2_name");
    }


    int whichBranch(string str)
    {
        // Parses a string and returns an enum corresponding to the proper branch (hard coded).
        // If the string doesn't correspond to any branch, return -1.

        // Will only ever return the index of certain variables that may possibly be varied. (No mass, no pt, no nhits, etc.)
        boost::erase_all(str, " ");


        if(str == "abs(eta)") return eta;
        if(str == "pt_dau1") return ptd1;
        if(str == "abs(eta_dau1)") return etad1;
        if(str == "cos(3DPointingAngle_dau1)") return angle;
        if(str == "3DDecayLengthSig_dau1") return dl;
        if(str.find("mass_dau1") != string::npos) return md1;
        if(str == "pt_dau2") return ptd2;
        if(str == "abs(eta_dau2)") return etad2;
        if(str == "abs(xyDCASig_dau2)") return xydca;
        if(str == "abs(zDCASig_dau2)") return zdca;

        cout << "ERROR in Configuration_TMVA::Configuration::whichBranch(). Variable name does not match with any presets.\n";
        return -1;
    }

private:
    string getComparator(int b)
    {
        // Get the comparator for the specified branch (hard coded). Returns empty if the branch shouldn't be being compared.

        switch(b)
        {
            case eta:
                return "<";
            case ptd1:
                return ">";
            case etad1:
                return "<";
            case angle:
                return ">";
            case dl:
                return ">";
            case md1:
                return "<=";
            case ptd2:
                return ">";
            case etad2:
                return "<";
            case xydca:
                return "<";
            case zdca:
                return "<";
            default:
                cout << "ERROR in Configuration_TMVA::Configuration::getComparator(). Branch is not comparable.\n";
                return "";
        }
    }
};

} /* namespace Configuration_TMVA */

#endif /* Configuration_H_ */
