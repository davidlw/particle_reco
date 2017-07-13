//============================================================================
// Name        : TryCuts.cpp
// Author      : Johann Gan
// Version     :
// Copyright   : 
// Description : Reads in trees and outputs mass histograms with various different cuts applied.
//============================================================================

#include "TryCuts.h"

//============================================================================

int main(int argc, char* argv[])
{
    clock_t clock_start = clock();

	// Specify configuration file
	string cfg_file = "config.xml";	// Default
	string infile_config = "infile_list.xml"; // Default
	string output_tag = ""; // Default

	cout << "Loading configuration.\n";
	if(argc > 1)
		cfg_file = argv[1];
	if(argc > 2)
		infile_config = argv[2];
	if(argc > 3)
		output_tag = argv[3];

	Configuration::Configuration params(cfg_file);	// Configuration parameters
	cout << "Using general configuration file: " << cfg_file << endl;
	InputChain::InputChain ic(infile_config);	// File parameters
	cout << "Using input chain configuration file: " << infile_config << endl;
	cout << "n_files: " << ic.get_n_files() << endl;
	cout << "Using output tag: " << output_tag << endl;

	// Readers for the branches
	Float_t mass_read = 0;
	Float_t pt_read = 0;
	Float_t eta_read = 0;
	Float_t ptdau1_read = 0;
	Float_t etadau1_read = 0;
	Float_t ptdau2_read = 0;
	Float_t etadau2_read = 0;
	Float_t xydca_read = 0;
	Float_t zdca_read = 0;
	Float_t massdau1_read = 0;
	Float_t PA_read = 0;
	Float_t DLsig_read = 0;
	Float_t dedx_read = 0;
	Float_t pdau2_read = 0;
	Float_t nhits_read = 0;

	// Get dE/dx cut information (but use only if specified)
	bool use_dedx = params.use_dedx();
	double k_lo = params.get_kca_lo()[0], c_lo = params.get_kca_lo()[1], a_lo = params.get_kca_lo()[2];
	double cutoff_lo = params.get_cutoff_lo();
	double k_hi = params.get_kca_hi()[0], c_hi = params.get_kca_hi()[1], a_hi = params.get_kca_hi()[2];
	double cutoff_hi = params.get_cutoff_hi();
	double dedx_mass = params.get_dedx_mass();
	double nhits_min = params.get_nhits_min();

	// Write default cuts to the text file
	cout << "Generating output files.\n";
	ofstream textout;
	textout.open(params.get_out_base() + output_tag + "_cutinfo.txt");
	textout << "Base cuts:\n";
	if(use_dedx)
	{
		textout << "nhits_dau2 >= " << nhits_min << endl;
		textout << "[ dEdxHarmonic_dau2 >= " << k_lo << " * (" << dedx_mass << " / (p_dau2 + " << -a_lo << "))^2 + " << c_lo << \
				"   OR   p_dau2 >= " << cutoff_lo << " ]\n    AND\n";
		textout << "[ dEdxHarmonic_dau2 < " << k_hi << " * (" << dedx_mass << " / (p_dau2 + " << -a_hi << "))^2 + " << c_hi << \
				"   OR   p_dau2 >= " << cutoff_hi << " ]\n";
	}
	textout << "-----------------------------------------\n";
	textout << "Histogram-specific cuts:\n";

	// Make an output file for the histograms
	TFile rootfile((params.get_out_base() + output_tag + "_histos.root").c_str(), "recreate");

	// Create a histogram vector for each set of cuts
	vector<unique_ptr<TH1D>> histos(params.n_histos());
	int nbins = params.n_bins();	// Number of bins for each histogram

	// Read in configuration cuts for iteration
	double massd1_mean = params.get_massd1_mean(), massd1_sigma = params.get_massd1_sigma();

	vector< vector<double> > pt_lims = params.get_pt_lims();
	vector<double> eta_max = params.get_eta_max(),
			ptd1_min = params.get_ptd1_min(),
			ptd2_min = params.get_ptd2_min(),
			etad1_max = params.get_etad1_max(),
			etad2_max = params.get_etad2_max(),
			xyDCA_max = params.get_xyDCA_max(),
			zDCA_max = params.get_zDCA_max(),
			massd1_z_max = params.get_massd1_zmax(),
			cosPA_min = params.get_cosPA_min(),
			dlsig_min = params.get_DLsig_min();

	// Loop through all the combinations of cuts to fill the text file and form the histograms
	int counter = 0; 	// Counter for iterations
    auto pt_end = pt_lims.end();
    auto eta_end = eta_max.end();
    auto ptd1_end = ptd1_min.end();
    auto ptd2_end = ptd2_min.end();
    auto etad1_end = etad1_max.end();
    auto etad2_end = etad2_max.end();
    auto xyDCA_end = xyDCA_max.end();
    auto zDCA_end = zDCA_max.end();
    auto massd1_z_end = massd1_z_max.end();
    auto cosPA_end = cosPA_min.end();
    auto dlsig_end = dlsig_min.end();
	for(auto pt = pt_lims.begin(); pt < pt_end; ++pt) {
	for(auto eta = eta_max.begin(); eta < eta_end; ++eta) {
	for(auto ptd1 = ptd1_min.begin(); ptd1 < ptd1_end; ++ptd1) {
	for(auto ptd2 = ptd2_min.begin(); ptd2 < ptd2_end; ++ptd2) {
	for(auto etad1 = etad1_max.begin(); etad1 < etad1_end; ++etad1) {
	for(auto etad2 = etad2_max.begin(); etad2 < etad2_end; ++etad2) {
	for(auto xy = xyDCA_max.begin(); xy < xyDCA_end; ++xy) {
	for(auto z = zDCA_max.begin(); z < zDCA_end; ++z) {
	for(auto md1 = massd1_z_max.begin(); md1 < massd1_z_end; ++md1) {
	for(auto agl = cosPA_min.begin(); agl < cosPA_end; ++agl) {
	for(auto dl = dlsig_min.begin(); dl < dlsig_end; ++dl)
	{
		++counter;

		// Set up the histograms
		string nametitle = "cut_" + to_string(counter);
		histos[counter - 1].reset( new TH1D( nametitle.c_str(), nametitle.c_str(), nbins, \
				params.get_hrange()[0], params.get_hrange()[1] ) );
		histos[counter - 1]->SetXTitle("m_{inv} (GeV/c^2)");
		histos[counter - 1]->GetXaxis()->CenterTitle();
		histos[counter - 1]->SetTitleOffset(1.2, "X");

		string ytitle = "Candidates (per " + doubleToStr(histos[counter - 1]->GetBinWidth(1)) + " GeV/c^{2})";
		histos[counter - 1]->SetYTitle(ytitle.c_str());
		histos[counter - 1]->GetYaxis()->CenterTitle();
		histos[counter - 1]->SetTitleOffset(1.5, "Y");

		histos[counter - 1]->SetStats(kFALSE);

		histos[counter - 1]->SetDirectory(&rootfile);

		// Write cut information to the text output file
		textout << endl << nametitle << ":\n";
		iterVarOut(textout, "pT", *pt);
		textout << "\t|eta| < " << *eta << endl;
		textout << "\tpT_dau1 > " << *ptd1 << endl;
		textout << "\tpT_dau2 > " << *ptd2 << endl;
		textout << "\t|eta_dau1| < " << *etad1 << endl;
		textout << "\t|eta_dau2| < " << *etad2 << endl;
		textout << "\t|xyDCA| < " << *xy << endl;
		textout << "\t|zDCA| < " << *z << endl;
		textout << "\t|(mass_dau1 - " << massd1_mean << ") / " << massd1_sigma << "| <= " << *md1 << endl;
		textout << "\tcos(3D pointing angle) > " << *agl << endl;
		textout << "\t3D decay length significance > " << *dl << endl;

	}}}}}}}}}}}	// 11 closing braces

	// Loop through the entries of the chain and divide up the entries according to the different sets of cuts

	// Setup for file reading
	string treePath = "";
	if(ic.get_dir_name().length() > 0)
		treePath += ic.get_dir_name() + "/";

	treePath += ic.get_tree_name();


	vector<string> in_bases = ic.get_in_bases();
	vector< vector<int> > i_lims = ic.get_idx_lims();

	// Setup for progress tracking
	unsigned file_counter = 0;
	unsigned n_files = ic.get_n_files();
	unsigned n_bases = i_lims.size();

	cout << "Begin reading entries.\n";
	for(unsigned f = 0; f < n_bases; ++f)
	{
		int idxlo = i_lims[f][0], idxhi = i_lims[f][1];
		for(int f_idx = idxlo; f_idx <= idxhi; ++f_idx)
		{
			// Progress tracking
			cout << "Opening file " << ++file_counter << "/" << n_files << "." << endl;

			string file = in_bases[f];
            if(file.find("{}") != file.npos)
                file.replace( file.find("{}"), 2, to_string(f_idx) );

            // Open the current file and tree
            TFile* file_i = new TFile(file.c_str());
            TNtuple* tree_i = (TNtuple*)file_i->Get(treePath.c_str());

            // Set up the current tree
			tree_i->SetBranchAddress("mass", &mass_read);
			tree_i->SetBranchAddress("pt", &pt_read);
			tree_i->SetBranchAddress("eta", &eta_read);
			tree_i->SetBranchAddress("pt_dau1", &ptdau1_read);
			tree_i->SetBranchAddress("eta_dau1", &etadau1_read);
			tree_i->SetBranchAddress("pt_dau2", &ptdau2_read);
			tree_i->SetBranchAddress("eta_dau2", &etadau2_read);
			tree_i->SetBranchAddress("xyDCASig_dau2", &xydca_read);
			tree_i->SetBranchAddress("zDCASig_dau2", &zdca_read);
			tree_i->SetBranchAddress("mass_dau1", &massdau1_read);
			tree_i->SetBranchAddress("3DPointingAngle_dau1", &PA_read);
			tree_i->SetBranchAddress("3DDecayLengthSig_dau1", &DLsig_read);
			tree_i->SetBranchAddress("dEdxHarmonic_dau2", &dedx_read);
			tree_i->SetBranchAddress("p_dau2", &pdau2_read);
			tree_i->SetBranchAddress("nhits_dau2", &nhits_read);


			unsigned long long int num_entries = tree_i->GetEntries();	// Total number of entries in tree
			for(unsigned long long int i = 0; i < num_entries; ++i)
			{
				tree_i->GetEntry(i);

				// Check whether the current leaf satisfies each set of cuts, and if so, fill the corresponding histogram
				int index = -1; 	// Index for the current set of cuts
				for(auto pt = pt_lims.begin(); pt < pt_end; ++pt) {
				for(auto eta = eta_max.begin(); eta < eta_end; ++eta) {
				for(auto ptd1 = ptd1_min.begin(); ptd1 < ptd1_end; ++ptd1) {
				for(auto ptd2 = ptd2_min.begin(); ptd2 < ptd2_end; ++ptd2) {
				for(auto etad1 = etad1_max.begin(); etad1 < etad1_end; ++etad1) {
				for(auto etad2 = etad2_max.begin(); etad2 < etad2_end; ++etad2) {
				for(auto xy = xyDCA_max.begin(); xy < xyDCA_end; ++xy) {
				for(auto z = zDCA_max.begin(); z < zDCA_end; ++z) {
				for(auto md1 = massd1_z_max.begin(); md1 < massd1_z_end; ++md1) {
				for(auto agl = cosPA_min.begin(); agl < cosPA_end; ++agl) {
				for(auto dl = dlsig_min.begin(); dl < dlsig_end; ++dl)
				{
					++index;


					if( isWithin(pt_read, *pt) && fabs(eta_read) < *eta && \
						ptdau1_read > *ptd1 && ptdau2_read > *ptd2 && \
						fabs(etadau1_read) < *etad1 && fabs(etadau2_read) < *etad2 && \
						fabs( (massdau1_read - massd1_mean) / massd1_sigma ) <= *md1 && \
						cos(PA_read) > *agl && DLsig_read > *dl && \
						fabs(xydca_read) < *xy && fabs(zdca_read) < *z)
					{
						// Apply dE/dx cuts only if specified
						if(!use_dedx || (
							(nhits_read >= nhits_min) && \
							(dedx_read >= k_lo * pow(dedx_mass / (pdau2_read - a_lo), 2) + c_lo || pdau2_read >= cutoff_lo) && \
							(dedx_read < k_hi * pow(dedx_mass / (pdau2_read - a_hi), 2) + c_hi || pdau2_read >= cutoff_hi) )
						)
						{
							histos[index]->Fill(static_cast<double>(mass_read));
						}
					}

				}}}}}}}}}}}	// 11 closing braces
			}

			delete tree_i;
			delete file_i;
		}
	}

	// Remove the empty histograms from the file
	cout << "\nRemoving empty histograms.\n";

    auto histos_end = histos.end();
	for(auto i = histos.begin(); i < histos_end; ++i)
	{
		if( (*i)->GetMaximum() <= 0 )
			(*i)->SetDirectory(0);
	}

	// Write the histograms to the file
	rootfile.Write();

	// Cleanup
	textout.close();

    clock_t clock_end = clock();

    cout << "Program terminated successfully.\n";
    cout << "Time elapsed: " << (clock_end - clock_start) / (double)CLOCKS_PER_SEC << " seconds.\n";
}

void iterVarOut(ofstream& out, const string& var_name, const vector<double>& var_lims)
{
	// Supporting function. Given an ofstream and a 2d vector<double>, outputs the following line in the file:
	//
	// "\t<var_name>: [<var_lo>, <var_hi>)\n"
	//
	// The vector<double> should contain {var_lo, var_hi}.

    out << "\t" + var_name + ": [" << var_lims[0] << ", " << var_lims[1] << ")\n";
}
