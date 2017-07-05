/*
 * Configuration.h
 *
 *  Created on: Jun 13, 2017
 *      Author: johanngan
 *
 *  Class for reading and using configuration parameters from an .xml configuration file
 */

//============================================================================

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//============================================================================

// Dependencies
#include <string>
#include <vector>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "strToVect.h"

//============================================================================

namespace Configuration {

class Configuration {
public:
	Configuration(const string& cfg_file = "config.xml");
	virtual ~Configuration();

	// Getters
	string get_out_base() const {return out_file_base;}
	vector< vector<double> > get_pt_lims() const {return pt_lims;}
	vector< vector<double> > get_eta_lims() const {return eta_lims;}
	vector< vector<double> > get_ptd1_lims() const {return ptdau1_lims;}
	vector< vector<double> > get_ptd2_lims() const {return ptdau2_lims;}
	vector< vector<double> > get_etad1_lims() const {return etadau1_lims;}
	vector< vector<double> > get_etad2_lims() const {return etadau2_lims;}
	vector<double> get_xyDCA_hi() const {return xyDCA_hi;}
	vector<double> get_zDCA_hi() const {return zDCA_hi;}
	vector<double> get_massd1_zmax() const {return massdau1_zmax;}
	double get_massd1_mean() const {return massdau1_mean;}
	double get_massd1_sigma() const {return massdau1_sigma;}
	vector<double> get_cosPAlo() const {return cosPAlo;}
	vector<double> get_DLsiglo() const {return DLsiglo;}
	bool use_dedx() const {return dedx_flag;}
	double get_dedx_mass() const {return dedx_mass;}
	vector<double> get_kca_lo() const {return kca_low;}
	double get_cutoff_lo() const {return cutoff_low;}
	vector<double> get_kca_hi() const {return kca_high;}
	double get_cutoff_hi() const {return cutoff_high;}
	int get_nhits_min() const {return nhits_min;}
	vector<double> get_hrange() const {return histo_range;}
	double get_binsize() const {return binsize;}

	string get_histoFile() const {return histoFitFile;}
	string get_fitOutPath() const {return fitOutPath;}
	double get_peakMass() const {return peakMass;}
	double get_peakStd() const {return peakStDev;}
	string get_dau1name() const {return dau1name;}
	string get_dau2name() const {return dau2name;}
	string get_imageType() const {return imageType;}

	// Calculate information about the output histograms
	int n_histos() const;
	int n_bins() const;

private:
	// File info
	string out_file_base;

	// Cut info
	vector< vector<double> > pt_lims;
	vector< vector<double> > eta_lims;
	vector< vector<double> > ptdau1_lims;
	vector< vector<double> > ptdau2_lims;
	vector< vector<double> > etadau1_lims;
	vector< vector<double> > etadau2_lims;
	vector<double> xyDCA_hi;
	vector<double> zDCA_hi;
	vector<double> massdau1_zmax;
	double massdau1_mean;
	double massdau1_sigma;
	vector<double> cosPAlo;
	vector<double> DLsiglo;

	bool dedx_flag;
	double dedx_mass;
	vector<double> kca_low;
	double cutoff_low;
	vector<double> kca_high;
	double cutoff_high;
	int nhits_min;

    // Fitting info
    string histoFitFile;
    string fitOutPath;
    double peakMass;
    double peakStDev;
    string dau1name;
    string dau2name;
    string imageType;

	// Histogram info
	vector<double> histo_range;
	double binsize;

	// Supporting function for reading cut info
	void readRangeCuts(const boost::property_tree::ptree&, const string&, vector< vector<double> >&);
};

} /* namespace Configuration */

#endif /* CONFIGURATION_H_ */
