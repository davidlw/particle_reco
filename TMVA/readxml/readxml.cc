#include "includes/readxml.h"

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

void readxml(const string& config_file_name, const string& IOfile_list, const string& output_tag = "")
{
  cout << "Loading configurations...\n";
  Configuration cfg(config_file_name);
  IOconfig io_list(IOfile_list);
  cout << "Using general configuration file: " << config_file_name << "\n";
  cout << "Using IO configuration file: " << IOfile_list << "\n";

  // Stuff that was in readxml.h //
  Double_t effS[cfg.nEff], effB[cfg.nEff];

  std::vector<TString> cuts;
  std::vector< std::vector<Double_t> > cutval(cfg.nMaxVar);
  std::vector<TString> varval(cfg.nMaxVar);
  Float_t ptmin, ptmax, raa;
  ////


  Float_t ptmin = cfg.ptBinLimits[0];
  Float_t ptmax = cfg.ptBinLimits[1];

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
  const string filename = cfg.myTMVApath + "/weights/TMVAClassification_CutsSA.weights.xml";
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
    
    TFile *inputS = TFile::Open(cfg.signalFileName.c_str());
    if(!inputS)
    {
        cout<<"Signal file not found."<<endl;
        return;
    }
    
    TTree *signal = (TTree*) inputS->Get(cfg.signalTreePath.c_str());

    // Chain together the background trees
    TChain *background = new TChain("background");
    vector<string> in_bases = io_list.inpath_bases;
    for(unsigned i = 0; i < in_bases.size(); i++)
    {
        int idxlo = io_list.idx_lims[i][0], idxhi = io_list.idx_lims[i][1];
        for(int j = idxlo; j <= idxhi; j++)
        {
            string file = in_bases[i];
            if(file.find("{}") != file.npos)
                file.replace( file.find("{}"), 2, to_string(j) );

            background->Add(file.c_str());
        }
    }

    string histoOutPath = io_list.histoOutPathBase + output_tag + ".root";
    TFile histoOutFile(histoOutPath.c_str(), "recreate");


    TH1D* LCmass[100];
    TH1D* LCmassS = new TH1D("LCmassS", "LCmassS", cfg.nMassBins, cfg.massWindowLo, cfg.massWindowHi);
    TH1D* LCmassB = new TH1D("LCmassB", "LCmassB", cfg.nMassBins, cfg.massWindowLo, cfg.massWindowHi);

    for(int icut=0;icut<100;icut++)
    {
        LCmass[icut] = new TH1D(Form("mass_cut%d", icut), Form("mass_cut%d", icut), cfg.nMassBins, cfg.massWindowLo, cfg.massWindowHi);
    }

    cout<<"Finished histogram construction."<<endl;

    vector<string> bNames = cfg.branchNames;
    vector<Float_t> branches(bNames.size());
    for(unsigned i = 0; i < bNames.size(); i++)
    {
        signal->SetBranchAddress(bNames[i].c_str(), &branches[i]);
    }

    Long64_t nentriesS = signal->GetEntries();
    for (Long64_t i = 0; i < nentriesS; i++)
    {
        signal->GetEntry(i);
        
        //first implement non-tuning cut; must be sychronized with the setting in TMVA tuning (mycutb)
        if(cfg.passesBaseSignalCuts(branches))
            LCmassS->Fill(branches[m]);
    }

    for(unsigned i = 0; i < bNames.size(); i++)
    {
        background->SetBranchAddress(bNames[i].c_str(), &branches[i]);
    }

    
    Long64_t nentries = background->GetEntries();
    vector<Double_t> cutVals(cfg.variableNamesList.size());
    for (Long64_t i = 0; i < nentries; i++)
    {
        background->GetEntry(i);
        
        //first implement non-tuning cut; must be sychronized with the setting in TMVA tuning (mycutb)
        if(cfg.passesBaseBackgroundCuts(branches))
        {
            LCmassB->Fill(branches[m]);
        
            //now implement the tuning cuts
            for(int icut = 0; icut < 100; icut++)
            {
                for(unsigned k = 0; k < cutVals.size(); k++)
                {
                    cutVals[k] = cutval[k].at(icut);
                }

                if(cfg.passesVariableCuts(branches, cutVals))
                    LCmass[icut]->Fill(branches[m]);
            }
        }
    }

    cout << "Finished filling histograms." << endl;

    histoOutFile.Write();


    // Print the cuts
    for(int icut = 0; icut < 100; icut++)
    {
        cout << "icut: " << icut;
        for(unsigned j = 0; j < cfg.variableNamesList.size(); j++)
        {
            cout << ", " << cfg.variableNamesList[j] << " " << cfg.variableComparators[j] << " " << cutval[j].at(icut);
        }
        cout << endl;
    }   
}