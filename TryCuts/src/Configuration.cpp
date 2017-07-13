/*
 * Configuration.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: johanngan
 */

#include "Configuration.h"

namespace Configuration {

Configuration::Configuration(const string& cfg_file)
{
	// Read in configuration parameters
	boost::property_tree::ptree cfg;
	boost::property_tree::xml_parser::read_xml(cfg_file, cfg);

	// File information
	out_file_base = cfg.get<string>("Config.Files.out_file_base");

	// Cut information
	readRangeCuts(cfg, "Config.Cuts.pt", pt_lims);

	eta_max = strToVect<double>( cfg.get<string>("Config.Cuts.eta_max"));
	ptdau1_min = strToVect<double>( cfg.get<string>("Config.Cuts.pt_dau1_min"));
	ptdau2_min = strToVect<double>( cfg.get<string>("Config.Cuts.pt_dau2_min"));
	etadau1_max = strToVect<double>( cfg.get<string>("Config.Cuts.eta_dau1_max"));
	etadau2_max = strToVect<double>( cfg.get<string>("Config.Cuts.eta_dau2_max"));

	xyDCA_max = strToVect<double>( cfg.get<string>("Config.Cuts.xyDCA_max") );
	zDCA_max = strToVect<double>( cfg.get<string>("Config.Cuts.zDCA_max") );

	massdau1_zmax = strToVect<double>( cfg.get<string>("Config.Cuts.mass_dau1_zscore_max") );
	massdau1_mean = cfg.get<double>("Config.Cuts.mass_dau1_mean");
	massdau1_sigma = cfg.get<double>("Config.Cuts.mass_dau1_sigma");
	cosPA_min = strToVect<double>(cfg.get<string>("Config.Cuts.cosPA_min") );
	DLsig_min = strToVect<double>(cfg.get<string>("Config.Cuts.DLsig_min") );

	string use_dedx = cfg.get<string>("Config.Cuts.dedx.use_dedx");
	if(use_dedx == "true")
	{
		dedx_flag = true;
	}
	else if(use_dedx == "false")
	{
		dedx_flag = false;
	}
	else
	{
		cout << "Warning: node \"use_dedx\" is neither \"true\" nor \"false\". Setting use_dedx to false.\n";
		dedx_flag = false;
	}
	dedx_mass = cfg.get<double>("Config.Cuts.dedx.mass");
	kca_low = strToVect<double>( cfg.get<string>("Config.Cuts.dedx.kca_low") );
	cutoff_low = cfg.get<double>("Config.Cuts.dedx.x_cutoff_low");
	kca_high = strToVect<double>( cfg.get<string>("Config.Cuts.dedx.kca_high") );
	cutoff_high = cfg.get<double>("Config.Cuts.dedx.x_cutoff_high");
	nhits_min = cfg.get<int>("Config.Cuts.dedx.nhits_dau2_min");

	// Histogram information
	histo_range = strToVect<double>( cfg.get<string>("Config.Histo.range") );
	binsize = cfg.get<double>("Config.Histo.bin_size");

	// Fit information
	histoFile = cfg.get<string>("Config.Fit.in_file");
	fitOutPath = cfg.get<string>("Config.Fit.out_path");
	imageType = cfg.get<string>("Config.Fit.image_type");

	peakStDev = cfg.get<double>("Config.Fit.peak_stdev");
	peakMass = cfg.get<double>("Config.Fit.peak_mass");

	dau1name = cfg.get<string>("Config.Fit.dau1_name");
	dau2name = cfg.get<string>("Config.Fit.dau2_name");
}

Configuration::~Configuration() {}


int Configuration::n_histos() const
{
	// Returns the number of histograms specified in the configuration
	return pt_lims.size() * eta_max.size() * ptdau1_min.size() * ptdau2_min.size() * etadau1_max.size() * etadau2_max.size() \
			* xyDCA_max.size() * zDCA_max.size() * massdau1_zmax.size() * cosPA_min.size() * DLsig_min.size();
}

int Configuration::n_bins() const
{
	// Returns the number of bins in each histogram
	return round( (histo_range[1] - histo_range[0]) / binsize );
}


void Configuration::readRangeCuts(const boost::property_tree::ptree& cfg, const string& node_name, vector< vector<double> >& cuts)
{
	// Supporting function. Reads a boost property tree for given cut ranges into a vector<vector<double>>.
	// The property tree cut paths are assumed to follow the convention: <node_name>.cut<index>.
	// 		<index> should start from 1.

	int n_cuts = cfg.get_child(node_name).size();
	cuts.clear();
	cuts.reserve(n_cuts);
	for(int i = 1; i <= n_cuts; i++)
	{
		cuts.push_back( strToVect<double>( cfg.get<string>(node_name + ".cut" + to_string(i)) ) );
	}
}


} /* namespace Configuration */
