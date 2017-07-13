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
	vector<double> get_eta_max() const {return eta_max;}
	vector<double> get_ptd1_min() const {return ptdau1_min;}
	vector<double> get_ptd2_min() const {return ptdau2_min;}
	vector<double> get_etad1_max() const {return etadau1_max;}
	vector<double> get_etad2_max() const {return etadau2_max;}
	vector<double> get_xyDCA_max() const {return xyDCA_max;}
	vector<double> get_zDCA_max() const {return zDCA_max;}
	vector<double> get_massd1_zmax() const {return massdau1_zmax;}
	double get_massd1_mean() const {return massdau1_mean;}
	double get_massd1_sigma() const {return massdau1_sigma;}
	vector<double> get_cosPA_min() const {return cosPA_min;}
	vector<double> get_DLsig_min() const {return DLsig_min;}
	bool use_dedx() const {return dedx_flag;}
	double get_dedx_mass() const {return dedx_mass;}
	vector<double> get_kca_lo() const {return kca_low;}
	double get_cutoff_lo() const {return cutoff_low;}
	vector<double> get_kca_hi() const {return kca_high;}
	double get_cutoff_hi() const {return cutoff_high;}
	int get_nhits_min() const {return nhits_min;}
	vector<double> get_hrange() const {return histo_range;}
	double get_binsize() const {return binsize;}
	string get_histoFile() const {return histoFile;}
	string get_fitOutPath() const {return fitOutPath;}
	string get_imageType() const {return imageType;}
	double get_peakStd() const {return peakStDev;}
	double get_peakMass() const {return peakMass;}
	string get_dau1name() const {return dau1name;}
	string get_dau2name() const {return dau2name;}

	// Calculate information about the output histograms
	int n_histos() const;
	int n_bins() const;

private:
	// File info
	string out_file_base;

	// Cut info
	vector< vector<double> > pt_lims;
	vector<double> eta_max;
	vector<double> ptdau1_min;
	vector<double> ptdau2_min;
	vector<double> etadau1_max;
	vector<double> etadau2_max;
	vector<double> xyDCA_max;
	vector<double> zDCA_max;
	vector<double> massdau1_zmax;
	double massdau1_mean;
	double massdau1_sigma;
	vector<double> cosPA_min;
	vector<double> DLsig_min;

	bool dedx_flag;
	double dedx_mass;
	vector<double> kca_low;
	double cutoff_low;
	vector<double> kca_high;
	double cutoff_high;
	int nhits_min;

	// Histogram info
	vector<double> histo_range;
	double binsize;

	// Fit info
	string histoFile;
	string fitOutPath;
	string imageType;

	double peakStDev;
	double peakMass;

	string dau1name;
	string dau2name;

	// Supporting function for reading cut info
	void readRangeCuts(const boost::property_tree::ptree&, const string&, vector< vector<double> >&);
};

} /* namespace Configuration */

#endif /* CONFIGURATION_H_ */
