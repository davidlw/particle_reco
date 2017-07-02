/*
 * IOconfig.h
 *
 *  Created on: Jun 13, 2017
 *      Author: johanngan
 *
 *  Class for reading file configuration from an .xml file for the readxml macro
 */

//============================================================================

#ifndef IOCONFIG_H_
#define IOCONFIG_H_

//============================================================================

// Dependencies
#include <string>
#include <vector>
#include <iostream>
    
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "strToVect.h"

//============================================================================

namespace IOconfig_readxml {

class IOconfig {
public:
    // readxml output path
    string histoOutPathBase;

    // readxml input tree files
    vector<string> inpath_bases;
    vector< vector<int> > idx_lims;
    string directory_name;
    string tree_name;


    // Constructor
    IOconfig(const string& file_info)
    {
        // Read in configuration parameters from an xml file
        boost::property_tree::ptree cfg;
        boost::property_tree::xml_parser::read_xml(file_info, cfg);

        histoOutPathBase = cfg.get<string>("Files.out_files.histo_outpath_base");


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
};

} /* namespace IOconfig_readxml */

#endif /* IOCONFIG_H_ */
