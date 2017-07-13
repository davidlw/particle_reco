#include "readxml.h"
#include "Tools.h"

#include <cmath>
#include <fstream>

#include "TGraph.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TMath.h"
#include "TFile.h"
#include "TString.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"

using namespace RooFit;
using namespace std;


void readxml_fit(const string& config_file_name, const string& output_tag = "")
{
    clock_t clock_start = clock();

    cout << "Loading configurations...\n";

    Configuration cfg(config_file_name);
    cout << "Using general configuration file: " << config_file_name << "\n";

    cout << "Using output tag: " << output_tag << "\n";

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


    // Stuff that was in readxml.h //
    Double_t effS[cfg.nEff], effB[cfg.nEff];

    std::vector<TString> cuts;
    std::vector< std::vector<Double_t> > cutval(cfg.nMaxVar);
    std::vector<TString> varval(cfg.nMaxVar);
    ////


     // Read in cuts
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
    const string filename = cfg.weight_file_path + "/" + cfg.jobName + "_CutsSA.weights.xml";
    void *doc = TMVA::gTools().xmlengine().ParseFile(filename.c_str(), TMVA::gTools().xmlenginebuffersize());
    void* rootnode = TMVA::gTools().xmlengine().DocGetRootElement(doc); // node "MethodSetup"
    TString fullMethodName("");  
    TMVA::gTools().ReadAttr(rootnode, "Method", fullMethodName);

    cout << endl;
    cout << " ╒══════════════════════════════════════════════════╕" << endl;
    cout << " |               Cut Opt Configuration              |" << endl;
    cout << " ├────────────┬────────────────────────────┬────────┤" << endl;
    cout << " | " << setiosflags(ios::left) << setw(10) << "Method" << " | " << setiosflags(ios::left) << setw(26) << \
        fullMethodName << " | " << setiosflags(ios::left) << setw(6) << " " << " |" << endl;

    void* opts = TMVA::gTools().GetChild(rootnode, "Options");
    void* opt = TMVA::gTools().GetChild(opts, "Option");

    TString varProp("");
    while(opt)
    {
        TString optname("");
        TMVA::gTools().ReadAttr(opt, "name", optname);
        if(optname == "VarProp") varProp = TMVA::gTools().GetContent(opt);
        opt = TMVA::gTools().GetNextChild(opt);
    }

    TObjArray* marginclass = varProp.Tokenize(" ");
    std::vector<TString> margins;//avoid objarrays
    for(int i = 0; i < marginclass->GetEntries(); i++)
    {
        margins.push_back(((TObjString*)(marginclass->At(i)))->String());
    }
    void* variables = TMVA::gTools().GetChild(rootnode, "Variables");
    UInt_t nVar = 0;
    std::vector<TString> varnames;
    TMVA::gTools().ReadAttr(variables, "NVar", nVar);

    void* var = TMVA::gTools().GetChild(variables, "Variable");
    for(unsigned int k = 0; k < nVar; k++)
    {
        TString varname("");
        TMVA::gTools().ReadAttr(var, "Expression", varname);
        TString tem = Form("Variable%i", k);
        varval[k] = varname;
        cout << " ├────────────┼────────────────────────────┼────────┤" << endl;
        cout << " | " << setiosflags(ios::left) << setw(10) << tem << " | " << setiosflags(ios::left) << setw(26) << varname << \
            " | " << setiosflags(ios::left) << setw(6) << margins[k] << " |" << endl;
        var = TMVA::gTools().GetNextChild(var);
        varnames.push_back(varname);
    }
    cout << " ╞════════════╪════════════════════════════╪════════╡" << endl;
    TString ptstring = Form("(%.1f, %.1f)", ptmin, ptmax);
    cout << " | " << setiosflags(ios::left) << setw(10) << "Pt" << " | " << setiosflags(ios::left) << setw(26) << ptstring << \
        " | " << setiosflags(ios::left) << setw(6) << " " << " |" << endl;
    cout << " ╘════════════╧════════════════════════════╧════════╛" << endl;
    cout << endl;
    
    void* weight = TMVA::gTools().GetChild(rootnode, "Weights");
    void* eff = TMVA::gTools().GetChild(weight, "Bin");
    int n = 0;
    while(eff)
    {
        TMVA::gTools().ReadAttr(eff, "effS", effS[n]);
        TMVA::gTools().ReadAttr(eff, "effB", effB[n]);
        void* cutsnode = TMVA::gTools().GetChild(eff, "Cuts");

        TString cut;
        for(ULong_t l = 0; l < varnames.size(); l++)
        {
            Double_t min, max;
            TMVA::gTools().ReadAttr(cutsnode, TString("cutMin_") + l, min);
            TMVA::gTools().ReadAttr(cutsnode, TString("cutMax_") + l, max);
            TString lessmax = "<"; lessmax += max;
            TString moremin = ">"; moremin += min;
            if(margins[l] == "FMin")
            {
                cut += " && " + varnames[l] + lessmax;
                cutval[l].push_back(max);
            }
            if(margins[l] == "FMax")
            {
                cut += " && " + varnames[l] + moremin;
                cutval[l].push_back(min);
            }
        }
        cuts.push_back(cut);
        eff = TMVA::gTools().GetNextChild(eff);
        n++;
    }
    TMVA::gTools().xmlengine().FreeDoc(doc);
    
    cout << "Finished reading cuts." << endl;



    // Open the histos output file generated by readxml
    TFile histoFile(cfg.histoFitFile.c_str());

    
    // Fit the histos to get signal significance
    double sigsig[100];
    double sigeff[100];
    // Initialize
    for(int icut = 0; icut < 100; icut++)
    {
        sigsig[0] = -1;
        sigeff[0] = -1;
    }


    TH1D* h;

    for(int icut = cfg.entryIdxLims[0]; icut <= cfg.entryIdxLims[1]; icut++)
    {
        RooMsgService::instance().setStreamStatus(0,kFALSE);
        RooMsgService::instance().setStreamStatus(1,kFALSE);

        TCanvas *c = new TCanvas("c","c",1600,1600);
        c->cd();
        h = (TH1D*)histoFile.Get(Form("mass_cut%d", icut));      

        RooRealVar x("x", "mass", cfg.massWindowLo, cfg.massWindowHi);
        RooDataHist data("data","dataset", x, h);
        RooPlot* xframe = x.frame(cfg.nMassBins);
        data.plotOn(xframe,Name("data"));
        RooRealVar mean("mean", "mean", cfg.peakMass, cfg.peakMass - 0.01, cfg.peakMass + 0.01);
        RooRealVar sigma1("sigma1","sigma1",0.015,0.005,0.015);
        RooRealVar sigma2("sigma2","sigma2",0.015,0.005,0.015);
        RooRealVar sig1("sig1","signal1",10,0,10000000);
        RooRealVar sig2("sig2","signal2",10,0,10000000);
        RooRealVar a("a","a",0,-100000,100000);
        RooRealVar b("b","b",0,-100000,100000);
        RooRealVar cp("cp","cp",0,-100000,100000);
        RooRealVar d("d","d",0,-100000,100000);
        RooGaussian gaus1("gaus1","gaus1",x,mean,sigma1);
        RooGaussian gaus2("gaus2","gaus2",x,mean,sigma2);
        RooPolynomial poly("poly","poly",x,RooArgList(a,b,cp,d));
        RooRealVar polysig("polysig","polysig",10,0,1e9);
        RooAddPdf sum("sum","sum",RooArgList(gaus1,gaus2,poly),RooArgList(sig1,sig2,polysig));
        
        RooFitResult *r = sum.fitTo(data,Save(),Minos(kTRUE),PrintLevel(-1));
        r = sum.fitTo(data,Save(),Minos(kTRUE),PrintLevel(-1));
        r = sum.fitTo(data,Save(),Minos(kTRUE),PrintLevel(-1));
        r = sum.fitTo(data,Save(),Minos(kTRUE),PrintLevel(-1));
        r = sum.fitTo(data,Save(),Minos(kTRUE),PrintLevel(-1));
        
        sum.plotOn(xframe,Name("sum"));
        sum.plotOn(xframe,Components(poly),LineStyle(kDashed));

        // r->Print();
        
        // double chi2 = xframe->chiSquare("sum","data");
        double meanf = mean.getVal();
        // double meanfe = mean.getError();
        double sigmaf1 = sigma1.getVal();
        double sigmaf2 = sigma2.getVal();
        double bkgf = polysig.getVal();
        double sigf1 = sig1.getVal();
        double sigf2 = sig2.getVal();
        double sigwf1 = sigf1/(sigf1 + sigf2);
        double sigwf2 = sigf2/(sigf1 + sigf2);
        // double c1 = a.getVal();
        // double c2 = b.getVal();
        
        double sigmaf = sqrt(sigmaf1 * sigmaf1 * sigwf1 + sigmaf2 * sigmaf2 * sigwf2);
        double massmin = meanf - cfg.peakStDev * sigmaf;
        double massmax = meanf + cfg.peakStDev * sigmaf;

        int nmin = h->GetXaxis()->FindBin(massmin);
        int nmax = h->GetXaxis()->FindBin(massmax);
        int anmin = h->GetXaxis()->FindBin(cfg.massWindowLo);
        int anmax = h->GetXaxis()->FindBin(cfg.massWindowHi);
        
        double awyh1 = h->Integral(anmin,nmin);
        double awyh2 = h->Integral(nmax,anmax);
        double awyh = awyh1 + awyh2;
        double totyh = h->Integral(nmin,nmax);
        
        x.setRange("cut",massmin,massmax);
        RooAbsReal* ibkg = poly.createIntegral(x,NormSet(x),Range("cut"));
        RooAbsReal* isig1 = gaus1.createIntegral(x,NormSet(x),Range("cut"));
        RooAbsReal* isig2 = gaus2.createIntegral(x,NormSet(x),Range("cut"));
        double ibkgf = ibkg->getVal();
        // double bkgfe = polysig.getError();
        double isig1f = isig1->getVal();
        double isig2f = isig2->getVal();
        
        double bkgy = ibkgf*bkgf / cfg.massBinSize; // Normalize by bin width
        // double bkgye = ibkgf*bkgfe;
        double sigy1 = isig1f*sigf1;
        double sigy2 = isig2f*sigf2;
        double sigy = (sigy1 + sigy2) / cfg.massBinSize; // Normalize by bin width
        double toty = bkgy + sigy;
        
        double abkgy = (1-ibkgf)*bkgf;
        double asigy1 = (1-isig1f)*sigf1;
        double asigy2 = (1-isig2f)*sigf2;
        double asigy = asigy1 + asigy2;
        // double awy = abkgy + asigy;
        
        double sigfrac = sigy/toty;
        double bkgfrac = bkgy/toty;
        
        double sigyh = totyh - bkgy;
        double sigfrach = sigy/totyh;
        double bkgfrach = bkgy/totyh;
        
        double signif = sigy/sqrt(totyh);
        sigsig[icut] = signif;
        sigeff[icut] = icut;
        xframe->SetXTitle( ("m(" + cfg.dau1name + cfg.dau2name + ") [GeV/c^{2}]").c_str() );
        xframe->GetXaxis()->CenterTitle();
        xframe->GetXaxis()->SetTitleOffset(1.2);
        xframe->GetXaxis()->SetLabelSize(0.02);
        xframe->SetYTitle(("Events(per " + to_string(cfg.massBinSize) + "GeV/c^{2})").c_str());
        xframe->GetYaxis()->CenterTitle();
        xframe->GetYaxis()->SetTitleOffset(1.3);
        xframe->GetYaxis()->SetLabelSize(0.02);
        xframe->SetTitle("");

        h->Draw();
        c->Update();
        double uymin = gPad->GetUymin(), uymax = gPad->GetUymax();
        c->Clear();

        xframe->GetYaxis()->SetRangeUser(uymin, uymax);
        xframe->Draw();
        h->Draw("PEsame");

        TLine* l1 = new TLine(massmin,uymin,massmin,uymax);
        TLine* l2 = new TLine(massmax,uymin,massmax,uymax);
        l1->SetLineStyle(2);
        l2->SetLineStyle(2);
        l1->Draw("same");
        l2->Draw("same");

        TLatex latex;
        latex.SetTextAlign(12);
        latex.SetTextSize(0.03);
        latex.SetNDC();

        latex.DrawLatex(0.70,0.85,Form("#frac{s}{#sqrt{s+b}} = %.3f", signif));
        latex.DrawLatex(0.70,0.80,Form("#frac{s}{s+b} = %.3f", sigfrach));
        latex.DrawLatex(0.70,0.75,Form("s+b = %.1f", totyh));
        latex.DrawLatex(0.70,0.70,Form("#mu = %.3f", meanf));
        latex.DrawLatex(0.70,0.65,Form("#sigma = %.4f", sigmaf));

        c->Print(Form(("./plots/fits/cut%d" + output_tag + cfg.imageType).c_str(), icut));

        delete h;
    }


    // Print the cuts with signal significance
    for(int icut = cfg.entryIdxLims[0]; icut <= cfg.entryIdxLims[1]; icut++)
    {
        cout << "icut: " << icut;
        cout << ", ss = " << sigsig[icut];
        for(unsigned j = 0; j < cfg.variableNamesList.size(); j++)
        {
            cout << ", " << cfg.variableNamesList[j] << " " << cfg.variableComparators[j] << " " << cutval[j].at(icut);
        }
        cout << endl;
    }  



    // Draw signal significance vs. signal efficiency plot
    TCanvas* cg = new TCanvas("cg","cg",800,800);
    cg->cd();
    TGraph* g = new TGraph (100, sigeff, sigsig);
    // Remove invalid points
    for(int i = 0; i < 100; i++)
    {
        if(isnan(sigeff[i]) || isnan(sigsig[i]) || isinf(sigeff[i]) || isinf(sigsig[i]))
            g->RemovePoint(i);
    }

    g->Draw();
    cg->Print(("./plots/sig_v_eff" + output_tag + cfg.imageType).c_str());


    // Draw signal and background histograms with only base cuts.
    TH1D* LCmassS = (TH1D*)histoFile.Get("LCmassS");
    TH1D* LCmassB = (TH1D*)histoFile.Get("LCmassB");

    TCanvas *cS = new TCanvas("cS","cS",1600,1600);
    cS->cd();
    LCmassS->Draw("E");
    cS->Print(("./plots/signal" + output_tag + cfg.imageType).c_str());

    TCanvas *cB = new TCanvas("cB","cB",1600,1600);
    cB->cd();
    LCmassB->Draw("E");
    cB->Print(("./plots/background" + output_tag + cfg.imageType).c_str());

    clock_t clock_end = clock();

    cout << "Program terminated successfully.\n";
    cout << "Time elapsed: " << (clock_end - clock_start) / (double)CLOCKS_PER_SEC << " seconds.\n";
}