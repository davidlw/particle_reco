/*
 * InputChain.h
 *
 *  Created on: Jun 13, 2017
 *      Author: johanngan
 *
 *  Class for reading files input information (for a TChain) from an .xml file
 */

//============================================================================

#ifndef INPUTCHAIN_H_
#define INPUTCHAIN_H_

//============================================================================

// Dependencies
#include <string>
#include <vector>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "strToVect.h"

//============================================================================

namespace InputChain {

class InputChain {
public:
    InputChain(const string& input_file)
    {
        // Read in configuration parameters
        boost::property_tree::ptree cfg;
        boost::property_tree::xml_parser::read_xml(input_file, cfg);

        // File information
        directory_name = cfg.get<string>("Files.in_files.directory_name");
        tree_name = cfg.get<string>("Files.in_files.tree_name");

        int n_infiles = cfg.get_child("Files.in_files").size() - 2; // - 2 discounts directory_name and tree_name
        inpath_bases.reserve(n_infiles);
        idx_lims.reserve(n_infiles);
        for(int i = 1; i <= n_infiles; i++)
        {
            string in_file = "Files.in_files.in_file_" + to_string(i);
            string in = cfg.get<string>(in_file + ".name_base");

            // If the directory_name node is empty, don't use a directory in the path
            if(directory_name.length() > 0)
                in += "/" + directory_name;

            in += "/" + tree_name;

            inpath_bases.push_back(in);
            idx_lims.push_back( strToVect<int>( cfg.get<string>(in_file + ".index_limits") ) );
        }
    }

    virtual ~InputChain() {}

    // Getters
    vector<string> get_in_bases() const {return inpath_bases;}
    vector< vector<int> > get_idx_lims() const {return idx_lims;}
    string get_dir_name() const {return directory_name;}
    string get_tree_name() const {return tree_name;}

private:
    // File info
    vector<string> inpath_bases;
    vector< vector<int> > idx_lims;
    string directory_name;
    string tree_name;
};

} /* namespace InputChain */

#endif /* INPUTCHAIN_H_ */
