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

    //particle structs
    ple parent, dau1, dau2;

    dau1.mass = dau1mass; //mass of Lambda (or Ks)
    dau1.charge = 0;    //charge of Lambda/Ks is 0
    dau2.mass = dau2mass; //mass of Pi (or Proton)


    //readers: used for reading/filling info from trees, not used in reconstruction

    //readers that will be calculated
    Float_t massf = -999.9;

    //readers that will be read directly from input trees
    Float_t md1f = -999.9;
    Float_t agld1f = -999.9;
    Float_t dlsigd1f = -999.9;
    Float_t isantid1f = -999.9;
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
    dau1Tree->SetBranchAddress("is_anti", &isantid1f);


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
    // recoTree->SetBranchAddress("vtxX", &eventInfo[1]);
    // recoTree->SetBranchAddress("vtxY", &eventInfo[2]);
    // recoTree->SetBranchAddress("vtxZ", &eventInfo[3]);

    recoTree->SetBranchAddress("mass", &massf);
    recoTree->SetBranchAddress("pt", &parent.pt);
    recoTree->SetBranchAddress("eta", &parent.eta);

    recoTree->SetBranchAddress("mass_dau1", &md1f);
    recoTree->SetBranchAddress("pt_dau1", &dau1.pt);
    recoTree->SetBranchAddress("eta_dau1", &dau1.eta);
    recoTree->SetBranchAddress("3DPointingAngle_dau1", &agld1f);
    recoTree->SetBranchAddress("3DDecayLengthSig_dau1", &dlsigd1f);
    // recoTree->SetBranchAddress("is_anti_dau1", &isantid1f);

    recoTree->SetBranchAddress("pt_dau2", &dau2.pt);
    recoTree->SetBranchAddress("ptErr_dau2", &pterrd2f);
    recoTree->SetBranchAddress("p_dau2", &pd2f);
    recoTree->SetBranchAddress("eta_dau2", &dau2.eta);
    // recoTree->SetBranchAddress("charge_dau2", &dau2.charge);
    recoTree->SetBranchAddress("zDCASig_dau2", &dcazd2f);
    recoTree->SetBranchAddress("xyDCASig_dau2", &dcaxyd2f);
    recoTree->SetBranchAddress("dEdxHarmonic_dau2", &dedxd2f);
    recoTree->SetBranchAddress("nhits_dau2", &nd2f);


    //perform parent mass reconstruction on each lambda/pi pair
    //THIS IS THE BULK OF THE COMPUTATION!
    for(Int_t i = 0; i < nentries_1; i++)
    {
    	dau1Tree->GetEntry(i);
        dau1.p4[3] = calcEnergy(dau1);
	
    	if(fabs(dau1.eta) < 2.4 && dau1.pt >= 1)	//***CUT DAU_1***
    	{
	    	for(Int_t j = 0; j < nentries_2; j++)
	    	{
    		   	dau2Tree->GetEntry(j);
                dau2.p4[3] = calcEnergy(dau2);

                //only reconstruct if it's Lambda + pi+ or anti-Lambda pi-
                bool signmatch = (isantid1f == 0 && dau2.charge == 1) || (isantid1f == 1 && dau2.charge == -1);
    		   	if(signmatch && fabs(dau2.eta) < 2.4 && dau2.pt >= 0.7)	//***CUT DAU_2***
    		   	{
			    	recoMass(dau1, dau2, parent);

			    	if(parent.mass > 2 && parent.mass < 2.5 && fabs(parent.eta) < 2.4)	//***CUT PARENT***
			    	{
                        massf = static_cast<Float_t>(parent.mass);
                        recoTree->Fill(1);
				    }
			    }
	    	}
	    }
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