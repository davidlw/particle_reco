//============================================================================
// Name        : TryCuts.cpp
// Author      : Johann Gan
// Version     :
// Copyright   : 
// Description : Reads in trees into a TChain and outputs mass histograms with various different cuts applied.
//============================================================================

#include "TryCuts.h"

//============================================================================

int main(int argc, char* argv[])
{
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
	cout << "Using output tag: " << output_tag << endl;

	// Read in the chain
	unique_ptr<TChain> chain( new TChain("chain") );

	cout << "Reading in files.\n";
	for(unsigned i = 0; i < ic.get_in_bases().size(); i++)
	{
		int idxlo = ic.get_idx_lims()[i][0], idxhi = ic.get_idx_lims()[i][1];
		for(int j = idxlo; j <= idxhi; j++)
		{
			string file = ic.get_in_bases()[i];
			if(file.find("{}") != file.npos)
				file.replace( file.find("{}"), 2, to_string(j) );

			chain->Add(file.c_str());
		}
	}


	// Set readers for the branches
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

	chain->SetBranchAddress("mass", &mass_read);
	chain->SetBranchAddress("pt", &pt_read);
	chain->SetBranchAddress("eta", &eta_read);
	chain->SetBranchAddress("pt_dau1", &ptdau1_read);
	chain->SetBranchAddress("eta_dau1", &etadau1_read);
	chain->SetBranchAddress("pt_dau2", &ptdau2_read);
	chain->SetBranchAddress("eta_dau2", &etadau2_read);
	chain->SetBranchAddress("xyDCASig_dau2", &xydca_read);
	chain->SetBranchAddress("zDCASig_dau2", &zdca_read);
	chain->SetBranchAddress("mass_dau1", &massdau1_read);
	chain->SetBranchAddress("3DPointingAngle_dau1", &PA_read);
	chain->SetBranchAddress("3DDecayLengthSig_dau1", &DLsig_read);
	chain->SetBranchAddress("dEdxHarmonic_dau2", &dedx_read);
	chain->SetBranchAddress("p_dau2", &pdau2_read);
	chain->SetBranchAddress("nhits_dau2", &nhits_read);


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

	vector< vector<double> > pt_lims = params.get_pt_lims(),
			eta_lims = params.get_eta_lims(),
			ptd1_lims = params.get_ptd1_lims(),
			ptd2_lims = params.get_ptd2_lims(),
			etad1_lims = params.get_etad1_lims(),
			etad2_lims = params.get_etad2_lims();
	vector<double> xyDCA_hi = params.get_xyDCA_hi(),
			zDCA_hi = params.get_zDCA_hi(),
			massd1_z_hi = params.get_massd1_zmax(),
			cosPA_lo = params.get_cosPAlo(),
			dlsig_lo = params.get_DLsiglo();

	// Loop through all the combinations of cuts to fill the text file and form the histograms
	int counter = 0; 	// Counter for iterations
	for(auto pt = pt_lims.begin(); pt < pt_lims.end(); pt++) {
	for(auto eta = eta_lims.begin(); eta < eta_lims.end(); eta++) {
	for(auto ptd1 = ptd1_lims.begin(); ptd1 < ptd1_lims.end(); ptd1++) {
	for(auto ptd2 = ptd2_lims.begin(); ptd2 < ptd2_lims.end(); ptd2++) {
	for(auto etad1 = etad1_lims.begin(); etad1 < etad1_lims.end(); etad1++) {
	for(auto etad2 = etad2_lims.begin(); etad2 < etad2_lims.end(); etad2++) {
	for(auto xy = xyDCA_hi.begin(); xy < xyDCA_hi.end(); xy++) {
	for(auto z = zDCA_hi.begin(); z < zDCA_hi.end(); z++) {
	for(auto md1 = massd1_z_hi.begin(); md1 < massd1_z_hi.end(); md1++) {
	for(auto agl = cosPA_lo.begin(); agl < cosPA_lo.end(); agl++) {
	for(auto dl = dlsig_lo.begin(); dl < dlsig_lo.end(); dl++)
	{
		counter++;

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
		iterVarOut(textout, "|eta|", *eta);
		iterVarOut(textout, "pT_dau1", *ptd1);
		iterVarOut(textout, "pT_dau2", *ptd2);
		iterVarOut(textout, "|eta_dau1|", *etad1);
		iterVarOut(textout, "|eta_dau2|", *etad2);
		textout << "\t|xyDCA| < " << *xy << endl;
		textout << "\t|zDCA| < " << *z << endl;
		textout << "\t|(mass_dau1 - " << massd1_mean << ") / " << massd1_sigma << "| <= " << *md1 << endl;
		textout << "\tcos(3D pointing angle) > " << *agl << endl;
		textout << "\t3D decay length significance > " << *dl << endl;

	}}}}}}}}}}}	// 11 closing braces

	// Loop through the entries of the chain and divide up the entries according to the different sets of cuts
	cout << "Begin reading entries.\n";

	int next_update = 1;	// Progress tracker
	unsigned long long int num_entries = chain->GetEntries();	// Total number of entries in chain
	for(unsigned long long int i = 0; i < num_entries; i++)
	{
		chain->GetEntry(i);

		// Check whether the current leaf satisfies each set of cuts, and if so, fill the corresponding histogram
		int index = -1; 	// Index for the current set of cuts
		for(auto pt = pt_lims.begin(); pt < pt_lims.end(); pt++) {
		for(auto eta = eta_lims.begin(); eta < eta_lims.end(); eta++) {
		for(auto ptd1 = ptd1_lims.begin(); ptd1 < ptd1_lims.end(); ptd1++) {
		for(auto ptd2 = ptd2_lims.begin(); ptd2 < ptd2_lims.end(); ptd2++) {
		for(auto etad1 = etad1_lims.begin(); etad1 < etad1_lims.end(); etad1++) {
		for(auto etad2 = etad2_lims.begin(); etad2 < etad2_lims.end(); etad2++) {
		for(auto xy = xyDCA_hi.begin(); xy < xyDCA_hi.end(); xy++) {
		for(auto z = zDCA_hi.begin(); z < zDCA_hi.end(); z++) {
		for(auto md1 = massd1_z_hi.begin(); md1 < massd1_z_hi.end(); md1++) {
		for(auto agl = cosPA_lo.begin(); agl < cosPA_lo.end(); agl++) {
		for(auto dl = dlsig_lo.begin(); dl < dlsig_lo.end(); dl++)
		{
			index++;


			if( isWithin(pt_read, *pt) && isWithin(fabs(eta_read), *eta) && \
				isWithin(ptdau1_read, *ptd1) && isWithin(ptdau2_read, *ptd2) && \
				isWithin(fabs(etadau1_read), *etad1) && isWithin(fabs(etadau2_read), *etad2) && \
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

		}}}}}}}}}}}	// 8 closing braces

		// Progress tracking in percentage. Report every 1%.
		int progress = static_cast<int>( 100.0 * (i + 1) / num_entries );
		if(progress >= next_update)
		{
			cout << progress << "% finished.\n";
			next_update++;
		}
	}

	// Remove the empty histograms from the file
	cout << "\nRemoving empty histograms.\n";

	for(auto i = histos.begin(); i < histos.end(); i++)
	{
		if( (*i)->GetMaximum() <= 0 )
			(*i)->SetDirectory(0);
	}

	// Write the histograms to the file
	rootfile.Write();

	// Cleanup
	textout.close();

	cout << "\nProgram terminated successfully.\n";
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
