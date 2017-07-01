#include "includes/readxml.h"
using namespace runTMVA_shared_config;
using namespace readxml_config;

#include "includes/Tools.h"

#include "TGraph.h"
#include <iostream>
#include <vector>
#include "TCanvas.h"
#include "TH1.h"
#include "TMath.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"

#include <fstream>

using namespace std;

void readxml(const string& backgroundTreeFileList, const string& outIndex)
{
  Float_t ptmin = ptBinLimits[0];
  Float_t ptmax = ptBinLimits[1];

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0);
  gStyle->SetMarkerStyle(20);
  gStyle->SetTextSize(0.05);
  gStyle->SetTextFont(42);
  gStyle->SetPadRightMargin(0.043);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.145);
  gStyle->SetTitleX(.0f);

  //read weight file
  const string filename = myTMVApath + "/weights/TMVAClassification_CutsSA.weights.xml";
  void *doc = TMVA::gTools().xmlengine().ParseFile(filename.c_str(),TMVA::gTools().xmlenginebuffersize());
  void* rootnode = TMVA::gTools().xmlengine().DocGetRootElement(doc); // node "MethodSetup"
  TString fullMethodName("");  
  TMVA::gTools().ReadAttr(rootnode, "Method", fullMethodName);

  cout<<endl;
  cout<<" ╒══════════════════════════════════════════════════╕"<<endl;
  cout<<" |               Cut Opt Configuration              |"<<endl;
  cout<<" ├────────────┬────────────────────────────┬────────┤"<<endl;
  cout<<" | "<<setiosflags(ios::left)<<setw(10)<<"Method"<<" | "<<setiosflags(ios::left)<<setw(26)<<fullMethodName<<" | "<<setiosflags(ios::left)<<setw(6)<<" "<<" |"<<endl;

  void *opts = TMVA::gTools().GetChild(rootnode,"Options");
  void* opt = TMVA::gTools().GetChild(opts,"Option");

  TString varProp("");
  while (opt)
    {
      TString optname("");
      TMVA::gTools().ReadAttr(opt, "name", optname);
      if (optname=="VarProp") varProp = TMVA::gTools().GetContent(opt);
      opt = TMVA::gTools().GetNextChild(opt);
    }

  TObjArray *marginclass = varProp.Tokenize(" ");
  std::vector<TString> margins;//avoid objarrays
  for(int i=0;i<marginclass->GetEntries();i++)
    {
      margins.push_back(((TObjString *)(marginclass->At(i)))->String());
    }
  void* variables = TMVA::gTools().GetChild(rootnode,"Variables");
  UInt_t nVar=0;
  std::vector<TString> varnames;
  TMVA::gTools().ReadAttr(variables, "NVar", nVar);

  void* var = TMVA::gTools().GetChild(variables,"Variable");
  for(unsigned int k=0;k<nVar;k++)
    {
      TString varname("");
      TMVA::gTools().ReadAttr(var, "Expression", varname);
      TString tem = Form("Variable%i",k);
      varval[k] = varname;
      cout<<" ├────────────┼────────────────────────────┼────────┤"<<endl;
      cout<<" | "<<setiosflags(ios::left)<<setw(10)<<tem<<" | "<<setiosflags(ios::left)<<setw(26)<<varname<<" | "<<setiosflags(ios::left)<<setw(6)<<margins[k]<<" |"<<endl;
      var = TMVA::gTools().GetNextChild(var);
      varnames.push_back(varname);
    }
  cout<<" ╞════════════╪════════════════════════════╪════════╡"<<endl;
    TString ptstring = Form("(%.1f,%.1f)",ptmin,ptmax);
    cout<<" | "<<setiosflags(ios::left)<<setw(10)<<"Pt"<<" | "<<setiosflags(ios::left)<<setw(26)<<ptstring<<" | "<<setiosflags(ios::left)<<setw(6)<<" "<<" |"<<endl;
    cout<<" ╘════════════╧════════════════════════════╧════════╛"<<endl;
    cout<<endl;
    
  void* weight = TMVA::gTools().GetChild(rootnode,"Weights");
  void* eff = TMVA::gTools().GetChild(weight,"Bin");
  int n=0;
  while(eff)
    {
      TMVA::gTools().ReadAttr(eff, "effS", effS[n]);
      TMVA::gTools().ReadAttr(eff, "effB", effB[n]);
      void* cutsnode = TMVA::gTools().GetChild(eff,"Cuts");

      TString cut;
      for(ULong_t l=0;l<varnames.size();l++)
    {
      Double_t min,max;
      TMVA::gTools().ReadAttr(cutsnode, TString("cutMin_")+l, min);
      TMVA::gTools().ReadAttr(cutsnode, TString("cutMax_")+l, max);
      TString lessmax = "<"; lessmax+=max;
      TString moremin = ">"; moremin+=min;
      if(margins[l]=="FMin")
        {
          cut+=" && "+varnames[l]+lessmax;
          cutval[l].push_back(max);
        }
      if(margins[l]=="FMax")
        {
          cut+=" && "+varnames[l]+moremin;
          cutval[l].push_back(min);
        }
    }
      cuts.push_back(cut);
      eff = TMVA::gTools().GetNextChild(eff);
      n++;
    }
    TMVA::gTools().xmlengine().FreeDoc(doc);
    
    cout<<"Finished reading cuts."<<endl;
    //construct histos with TMVA cuts
    
    TFile *inputS = TFile::Open(signalFileName);
    if(!inputS)
    {
        cout<<"Signal file not found."<<endl;
        return;
    }
    
    TTree *signal = (TTree*) inputS->Get(signalTreePath);

    // Chain together the background trees
    InputChain::InputChain ic(backgroundTreeFileList);
    TChain *background = new TChain("background");
    for(unsigned i = 0; i < ic.get_in_bases().size(); i++)
    {
        int idxlo = ic.get_idx_lims()[i][0], idxhi = ic.get_idx_lims()[i][1];
        for(int j = idxlo; j <= idxhi; j++)
        {
            string file = ic.get_in_bases()[i];
            if(file.find("{}") != file.npos)
                file.replace( file.find("{}"), 2, to_string(j) );

            background->Add(file.c_str());
        }
    }

    TString histoOutFileName = histoOutPathBase + outIndex.c_str() + ".root";
    TFile histoOutFile(histoOutFileName, "recreate");

    TH1D* LCmass[100];
    TH1D* LCmassS = new TH1D("LCmassS", "LCmassS", nMassBins, massWindowLo, massWindowHi);
    TH1D* LCmassB = new TH1D("LCmassB", "LCmassB", nMassBins, massWindowLo, massWindowHi);

    for(int icut=0;icut<100;icut++)
    {
        LCmass[icut] = new TH1D(Form("mass_cut%d", icut), Form("mass_cut%d", icut), nMassBins, massWindowLo, massWindowHi);
    }

    cout<<"Finished histogram construction."<<endl;

    vector<Float_t> branches(branchNames.size());
    for(unsigned i = 0; i < branchNames.size(); i++)
    {
        signal->SetBranchAddress(branchNames[i], &branches[i]);
    }

    Long64_t nentriesS = signal->GetEntries();
    for (Long64_t i = 0; i < nentriesS; i++)
    {
        signal->GetEntry(i);
        
        //first implement non-tuning cut; must be sychronized with the setting in TMVA tuning (mycutb)
        if(passesBaseSignalCuts(branches))
            LCmassS->Fill(branches[m]);        
    }

    for(unsigned i = 0; i < branchNames.size(); i++)
    {
        background->SetBranchAddress(branchNames[i], &branches[i]);
    }

    
    Long64_t nentries = background->GetEntries();
    vector<Double_t> cutVals(variableNamesList.size());
    for (Long64_t i = 0; i < nentries; i++)
    {
        background->GetEntry(i);
        
        //first implement non-tuning cut; must be sychronized with the setting in TMVA tuning (mycutb)
        if(passesBaseBackgroundCuts(branches))
        {
            LCmassB->Fill(branches[m]);
        
            //now implement the tuning cuts
            for(int icut = 0; icut < 100; icut++)
            {
                for(unsigned k = 0; k < cutVals.size(); k++)
                {
                    cutVals[k] = cutval[k].at(icut);
                }

                if(passesVariableCuts(branches, cutVals))
                    LCmass[icut]->Fill(branches[m]);
            }
        }
    }

    cout<<"Finished filling histograms."<<endl;

    histoOutFile.Write();


    // Print the cuts
    for(int icut = 0; icut < 100; icut++)
    {
        cout << "icut: " << icut;
        for(unsigned j = 0; j < variableNamesList.size(); j++)
        {
            cout << ", " << variableNamesList[j] << " " << variableComparators[j] << " " << cutval[j].at(icut);
        }
        cout << endl;
    }   
}

void readxml(const string& backgroundTreeFileList, int outIndex)
{
    readxml(backgroundTreeFileList, to_string(outIndex));
}
