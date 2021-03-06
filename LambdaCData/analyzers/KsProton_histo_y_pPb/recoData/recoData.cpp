//NOTE: THIS MACRO REQUIRES C++11, AND MUST BE RUN IN ROOT6

//v2 of recoData.cpp. Allows reconstruction on data in multiple files, with results stored in one single file
//This is better than merging the input files, since particles from one sample should not be combined with
//particles from a different sample. (A particle can't have one daughter in one file and the other daughter somewhere else)

/////////////

#ifndef RECODATA_CPP_
#define RECODATA_CPP_

#include "recoData.h"

void recoData(TNtuple* dau1Tree, TNtuple* dau2Tree, vector<TH1D*>& recoHistos)
{
    using namespace recoData_constants;

    //recoTree should have branches: mass, pt, eta
	//performs reconstruction on data

	//useful numbers
    Long64_t nentries_1 = dau1Tree->GetEntries();
    Long64_t nentries_2 = dau2Tree->GetEntries();
    double xmin = recoHistos[0]->GetXaxis()->GetXmin(), xmax = recoHistos[0]->GetXaxis()->GetXmax();
    double ymax = 1;

    double cm_corr = 0.465; // pPb correction
    const double PARENT_MASS = 2.28646;

    //particle structs
    ple parent, dau1, dau2;

    dau1.mass = dau1mass; //mass of Lambda (or Ks)
    dau1.charge = 0;    //charge of Lambda/Ks is 0
    dau2.mass = dau2mass; //mass of Pi (or Proton)


    //readers: used for reading/filling info from trees, not used in reconstruction

    //readers that will be read directly from input trees
    Float_t md1f = -999.9;
    Float_t agld1f = -999.9;
    Float_t dlsigd1f = -999.9;
    Float_t pterrd2f = -999.9;
    Float_t pd2f = -999.9;
    Float_t dcazd2f = -999.9;
    Float_t dcaxyd2f = -999.9;
    Float_t dedxd2f = -999.9;

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


    //perform parent mass reconstruction on each lambda/pi pair
    //THIS IS THE BULK OF THE COMPUTATION!
    for(Int_t i = 0; i < nentries_1; i++)
    {
    	dau1Tree->GetEntry(i);
        dau1.p4[3] = calcEnergy(dau1);
	
    	if(fabs(dau1.eta) < 2.4 && dau1.pt >= 0.5)	//***CUT DAU_1***
    	{
	    	for(Int_t j = 0; j < nentries_2; j++)
	    	{
    		   	dau2Tree->GetEntry(j);
                dau2.p4[3] = calcEnergy(dau2);

    		   	if(fabs(dau2.charge) == 1 && fabs(dau2.eta) < 2.4 && dau2.pt >= 0.3 && \
                ( (dedxd2f >= 2.6 * pow(dau2mass / (pd2f + 0.1), 2) + 2.2 || pd2f >= 2.2) && \
                  (dedxd2f < 3.5 * pow(dau2mass / (pd2f - 0.1), 2) + 4.2 || pd2f >= 2.5) ) )	//***CUT DAU_2***
    		   	{
			    	recoMass(dau1, dau2, parent);

			    	if(parent.mass >= xmin && parent.mass < xmax && \
                        fabs( rapidity(parent.eta, PARENT_MASS, parent.pt, cm_corr) ) < ymax)	//***CUT PARENT***
			    	{
                        // Now do optimized cuts based on the pT bin
                        if(parent.pt >= 1 && parent.pt < 2)
                        {
                            if( !(dau1.pt > 1) ) continue;
                            if( !(dau2.pt > 0.4) ) continue;
                            if( !(fabs(dau1.eta) < 1.4) ) continue;
                            if( !(fabs(dau2.eta) < 1) ) continue;
                            if( !( fabs((md1f - 0.498) / 0.0067) <= 2 ) ) continue;

                            recoHistos[0]->Fill(parent.mass);
                        }
                        else if(parent.pt >= 2 && parent.pt < 4)
                        {
                            if( !(dau1.pt > 1.4) ) continue;
                            if( !(dau2.pt > 0.3) ) continue;
                            if( !(fabs(dau1.eta) < 1.4) ) continue;
                            if( !(fabs(dau2.eta) < 1) ) continue;
                            if( !( fabs((md1f - 0.498) / 0.0067) <= 2 ) ) continue;

                            if(parent.pt < 3)
                                recoHistos[1]->Fill(parent.mass);
                            else
                                recoHistos[2]->Fill(parent.mass);
                        }
                        else if(parent.pt >= 4 && parent.pt < 6)
                        {
                            if( !(dau1.pt > 0.5) ) continue;
                            if( !(dau2.pt > 0.5) ) continue;
                            if( !(fabs(dau1.eta) < 1.4) ) continue;
                            if( !(fabs(dau2.eta) < 1.9) ) continue;
                            if( !( fabs((md1f - 0.498) / 0.0067) <= 2 ) ) continue;

                            if(parent.pt < 5)
                                recoHistos[3]->Fill(parent.mass);
                            else
                                recoHistos[4]->Fill(parent.mass);
                        }
                        else if(parent.pt >= 6 && parent.pt < 8)
                        {
                            if( !(dau1.pt > 0.5) ) continue;
                            if( !(dau2.pt > 1.5) ) continue;
                            if( !(fabs(dau1.eta) < 1.4) ) continue;
                            if( !(fabs(dau2.eta) < 1.4) ) continue;
                            if( !( fabs((md1f - 0.498) / 0.0067) <= 2 ) ) continue;

                            if(parent.pt < 7)
                                recoHistos[5]->Fill(parent.mass);
                            else
                                recoHistos[6]->Fill(parent.mass);
                        }
                        else if(parent.pt >= 8 && parent.pt < 10)
                        {
                            if( !(dau1.pt > 1.6) ) continue;
                            if( !(dau2.pt > 2.3) ) continue;
                            if( !(fabs(dau1.eta) < 1.1) ) continue;
                            if( !(fabs(dau2.eta) < 1.1) ) continue;
                            if( !( fabs((md1f - 0.498) / 0.0067) <= 2 ) ) continue;

                            if(parent.pt < 9)
                                recoHistos[7]->Fill(parent.mass);
                            else
                                recoHistos[8]->Fill(parent.mass);
                        }
                        else if(parent.pt >= 10 && parent.pt < 20)
                        {
                            if( !(dau1.pt > 2.5) ) continue;
                            if( !(dau2.pt > 3.5) ) continue;
                            if( !(fabs(dau1.eta) < 1.1) ) continue;
                            if( !(fabs(dau2.eta) < 1.1) ) continue;
                            if( !( fabs((md1f - 0.498) / 0.0067) <= 2 ) ) continue;

                            recoHistos[9]->Fill(parent.mass);
                        }
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