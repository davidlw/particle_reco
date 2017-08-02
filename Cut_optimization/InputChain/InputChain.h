/*
 * InputChain.h
 *
 *  Created on: Jul 3, 2017
 *      Author: johanngan
 */
// For reading input tree files in TryCuts and readxml.

//============================================================================

#ifndef INPUTCHAIN_H_
#define INPUTCHAIN_H_

//============================================================================

// Dependencies
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../ROOT_Util/strToVect.h"

//============================================================================


using namespace std;

namespace InputChain {

class InputChain {
public:
	// Ctor
	InputChain(const string& infile_config = "infile_list.xml")
	{
		// Read in configuration parameters
		boost::property_tree::ptree incfg;
		boost::property_tree::xml_parser::read_xml(infile_config, incfg);


		directory_name = incfg.get<string>("Files.in_files.directory_name");
		tree_name = incfg.get<string>("Files.in_files.tree_name");

		int n_infiles = incfg.get_child("Files.in_files").size() - 2;	// - 2 discounts directory_name and tree_name
		n_files = 0;
		inpath_bases.reserve(n_infiles);
		idx_lims.reserve(n_infiles);
		for(int i = 1; i <= n_infiles; i++)
		{
			string in_file = "Files.in_files.in_file_" + to_string(i);
			string in = incfg.get<string>(in_file + ".name_base");

			inpath_bases.push_back(in);
			vector<int> lim_i = strToVect<int>( incfg.get<string>(in_file + ".index_limits") );
			idx_lims.push_back( lim_i );

			n_files += lim_i[1] - lim_i[0] + 1;
		}

		y_cm_correction = incfg.get<double>("Files.y_cm_correction");
	}

	// Getters
	vector<string> get_in_bases() const {return inpath_bases;}
	vector< vector<int> > get_idx_lims() const {return idx_lims;}
	string get_dir_name() const {return directory_name;}
	string get_tree_name() const {return tree_name;}
	unsigned get_n_files() const {return n_files;}
	double get_y_cm_corr() const {return y_cm_correction;}


private:
	vector<string> inpath_bases;
	vector< vector<int> > idx_lims;
	string directory_name;
	string tree_name;

	unsigned n_files;

	double y_cm_correction;
};

} /* namespace InputChain */

#endif /* INPUTCHAIN_H_ */
