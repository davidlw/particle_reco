//NOTE: THIS MACRO REQUIRES C++11, AND MUST BE RUN IN ROOT6

//v2 of recoData.cpp. Allows reconstruction on data in multiple files, with results stored in one single file
//This is better than merging the input files, since particles from one sample should not be combined with
//particles from a different sample. (A particle can't have one daughter in one file and the other daughter somewhere else)

/////////////

#ifndef RECODATA_CPP_
#define RECODATA_CPP_

#include "recoData.h"

void recoData(vector<Float_t>& eventInfo, TNtuple* dau1Tree, TNtuple* dau2Tree, TNtuple* recoTree)
{
    using namespace recoData_constants;

    //recoTree should have branches: mass, pt, eta
	//performs reconstruction on data

	//useful numbers
    Long64_t nentries_1 = dau1Tree->GetEntries();
    Long64_t nentries_2 = dau2Tree->GetEntries();
    const double PARENT_MASS = 2.28646;
    double cm_corr = 0;

    //particle structs
    ple parent, dau1, dau2;

    dau1.mass = dau1mass; //mass of Lambda (or Ks)
    dau1.charge = 0;    //charge of Lambda/Ks is 0
    dau2.mass = dau2mass; //mass of Pi (or Proton)


    //readers: used for reading/filling info from trees, not used in reconstruction

    //readers that will be calculated
    Float_t massf = -999.9;
    Float_t n_reco = 0; //number of reconstructed particles added to the output tree
    Float_t yf = -999.9;
    Float_t yd1f = -999.9;
    Float_t yd2f = -999.9;

    //readers that will be read directly from input trees
    Float_t md1f = -999.9;
    Float_t agld1f = -999.9;
    Float_t dlsigd1f = -999.9;
    Float_t statd1f = -999.9;
    Float_t pterrd2f = -999.9;
    Float_t pd2f = -999.9;
    Float_t dcazd2f = -999.9;
    Float_t dcaxyd2f = -999.9;
    Float_t dedxd2f = -999.9;
    Float_t nd2f = -999.9;

    //initialize
    for(Int_t i = 0; i < 4; i++)
    {
    	dau1.p4[i] = -999.9;
    	dau2.p4[i] = -999.9;
    }

    //set daughter branch addresses (energy to be calculated later)

    //daughter 1
    dau1Tree->SetBranchAddress("px", &dau1.p4[0]);
    dau1Tree->SetBranchAddress("py", &dau1.p4[1]);
    dau1Tree->SetBranchAddress("pz", &dau1.p4[2]);
    dau1Tree->SetBranchAddress("pt", &dau1.pt);
    dau1Tree->SetBranchAddress("eta", &dau1.eta);
    dau1Tree->SetBranchAddress("m", &md1f);
    dau1Tree->SetBranchAddress("agl", &agld1f);
    dau1Tree->SetBranchAddress("dlsig", &dlsigd1f);
    dau1Tree->SetBranchAddress("stat", &statd1f);


    //daughter 2
    dau2Tree->SetBranchAddress("px", &dau2.p4[0]);
    dau2Tree->SetBranchAddress("py", &dau2.p4[1]);
    dau2Tree->SetBranchAddress("pz", &dau2.p4[2]);
    dau2Tree->SetBranchAddress("p", &pd2f);
    dau2Tree->SetBranchAddress("pt", &dau2.pt);
    dau2Tree->SetBranchAddress("pterr", &pterrd2f);
    dau2Tree->SetBranchAddress("eta", &dau2.eta);
    dau2Tree->SetBranchAddress("q", &dau2.charge);
    dau2Tree->SetBranchAddress("dedx", &dedxd2f);
    dau2Tree->SetBranchAddress("dcaz", &dcazd2f);
    dau2Tree->SetBranchAddress("dcaxy", &dcaxyd2f);
    dau2Tree->SetBranchAddress("nhits", &nd2f);


    //set parent branch addresses
    recoTree->SetBranchAddress("ntrkoffline", &eventInfo[0]);
    recoTree->SetBranchAddress("vtxX", &eventInfo[1]);
    recoTree->SetBranchAddress("vtxY", &eventInfo[2]);
    recoTree->SetBranchAddress("vtxZ", &eventInfo[3]);
    recoTree->SetBranchAddress("n_reconstructions", &n_reco);

    recoTree->SetBranchAddress("mass", &massf);
    recoTree->SetBranchAddress("pt", &parent.pt);
    recoTree->SetBranchAddress("eta", &parent.eta);
    recoTree->SetBranchAddress("y_cm", &yf);

    recoTree->SetBranchAddress("mass_dau1", &md1f);
    recoTree->SetBranchAddress("pt_dau1", &dau1.pt);
    recoTree->SetBranchAddress("eta_dau1", &dau1.eta);
    recoTree->SetBranchAddress("y_cm_dau1", &yd1f);
    recoTree->SetBranchAddress("3DPointingAngle_dau1", &agld1f);
    recoTree->SetBranchAddress("3DDecayLengthSig_dau1", &dlsigd1f);
    recoTree->SetBranchAddress("status_dau1_matched", &statd1f);

    recoTree->SetBranchAddress("pt_dau2", &dau2.pt);
    recoTree->SetBranchAddress("ptErr_dau2", &pterrd2f);
    recoTree->SetBranchAddress("p_dau2", &pd2f);
    recoTree->SetBranchAddress("eta_dau2", &dau2.eta);
    recoTree->SetBranchAddress("y_cm_dau2", &yd2f);
    recoTree->SetBranchAddress("charge_dau2", &dau2.charge);
    recoTree->SetBranchAddress("zDCASig_dau2", &dcazd2f);
    recoTree->SetBranchAddress("xyDCASig_dau2", &dcaxyd2f);
    recoTree->SetBranchAddress("dEdxHarmonic_dau2", &dedxd2f);
    recoTree->SetBranchAddress("nhits_dau2", &nd2f);


    //perform parent mass reconstruction on each lambda/pi pair
    //THIS IS THE BULK OF THE COMPUTATION!

    //first count the number of good reconstructions there will be (primarily for counting false matches)

    //record the reconstructions that pass all the tests to save on iteration time later
    vector<Int_t> good_dau1;
    vector<Int_t> good_dau2;
    good_dau1.reserve(nentries_1 * nentries_2);
    good_dau2.reserve(nentries_1 * nentries_2);

    for(Int_t i = 0; i < nentries_1; i++)
    {
        dau1Tree->GetEntry(i);
        dau1.p4[3] = calcEnergy(dau1);
    
        if(fabs(dau1.eta) < 2.4)   //***CUT DAU_1***
        {
            for(Int_t j = 0; j < nentries_2; j++)
            {
                dau2Tree->GetEntry(j);
                dau2.p4[3] = calcEnergy(dau2);

                if(fabs(dau2.eta) < 2.4) //***CUT DAU_2***
                {
                    recoMass(dau1, dau2, parent);

                    if(parent.mass > 2 && parent.mass < 2.5 && fabs(parent.eta) < 2.4) //***CUT PARENT***
                    {
                        n_reco++;

                        good_dau1.push_back(i);
                        good_dau2.push_back(j);
                    }
                }
            }
        }
    }


    //now do the good reconstructions again and fill the output tree
    for(unsigned i = 0; i < good_dau1.size(); i++)
    {
        dau1Tree->GetEntry(i);
        dau1.p4[3] = calcEnergy(dau1);
        yd1f = rapidity(dau1.eta, dau1mass, dau1.pt, cm_corr);

        dau2Tree->GetEntry(i);
        dau2.p4[3] = calcEnergy(dau2);
        yd2f = rapidity(dau2.eta, dau2mass, dau2.pt, cm_corr);
    
        recoMass(dau1, dau2, parent);
        yf = rapidity(parent.eta, PARENT_MASS, parent.pt, cm_corr);

        massf = static_cast<Float_t>(parent.mass);
        recoTree->Fill(1);
    }
}


/////////////


void recoMass(const ple& dau1, const ple& dau2, ple& parent)
{
	//take in two ple structs by ref and calculate the members of the struct for the parent ple

	for(int i = 0; i < 4; i++)
	{
		parent.p4[i] = dau1.p4[i] + dau2.p4[i];
	}

	parent.mass = calcMass(parent);
    parent.pt = calcPt(parent);
    parent.eta = calcEta(parent);
}

#endif