//
// rooFitHisto_error.cpp
// 
// Created by Johann Gan on 7/12/17.
//
// ---INSERT DESCRIPTION HERE---
//
// ---INSERT USAGE INSTRUCTIONS HERE---
//
// Fit status 0 is desired
// Covariance matrix quality code 3 is desired
//
//////////////////////////////////////

// Dependencies
#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLine.h"
#include "TLatex.h"
#include "TMath.h"
#include "TString.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooFormulaVar.h"

//////////////////////////////////////


using namespace RooFit;
using namespace std;

// const double peakMass = 1.115683;
// const double massWindowLo = 1.08;
// const double massWindowHi = 1.16;

// const double peakMass = 0.497611;
// const double massWindowLo = 0.43;
// const double massWindowHi = 0.56;

const double peakMass = 2.28646;
const double massWindowLo = 2.19;
const double massWindowHi = 2.39;
const double peakStDev = 2;
const TString dau1name = "#Lambda^{0}";
const TString dau2name = "#pi";
// const TString dau1name = "K_{s}^{0}";
// const TString dau2name = "p";
const TString imageType = ".png";


const unsigned MAX_TRIES = 20;
const double EDM_TOLERANCE = 1e-5;


void doRooFit(TH1D*);

void rooFitHisto_error()
{
    TFile f("/Users/johanngan/Desktop/research/blc/RESULTS/MC/eff_analysis/MC_LambdaC_L_histos_merged.root");

    TH1D* h = (TH1D*)f.Get("lcreco/lambdac_5");
    h->SetStats(kFALSE);
    h->Rebin(1);

    doRooFit(h);
}

void doRooFit(TH1D* h)
{
    RooMsgService::instance().setStreamStatus(0,kFALSE);
    RooMsgService::instance().setStreamStatus(1,kFALSE);

    TCanvas *c = new TCanvas("c", "c", 1600, 1600);
    c->cd();

    unsigned nMassBins = h->GetNbinsX();
    double massBinSize = (massWindowHi - massWindowLo) / nMassBins;

    RooRealVar x("x", "mass", massWindowLo, massWindowHi);
    RooDataHist data("data", "dataset", x, h);
    RooPlot* xframe = x.frame(nMassBins);
    data.plotOn(xframe,Name("data"));
    RooRealVar mean("mean", "mean", peakMass, peakMass - 0.01, peakMass + 0.01);
    RooRealVar sigma1("sigma1", "sigma1", 0.015, 0.005, 0.015);
    RooRealVar sigma2("sigma2", "sigma2", 0.015, 0.005, 0.015);

    //***NEW***
    RooRealVar sig1prop("sig1prop", "signal1prop", 0.5, 0, 1);  // weight of the first Gaussian
    RooRealVar sigyield("sigyield", "sigyield", 10, 0, 1e9);    // total combined yield of the two Gaussians
    RooFormulaVar sig1("sig1", "@0 * @1", RooArgList(sigyield, sig1prop));  // total yield of the first Gaussian
    RooFormulaVar sig2("sig2", "@0 * (1 - @1)", RooArgList(sigyield, sig1prop)); // total yield of the second Gaussian

    RooRealVar a("a", "a", 0, -100000, 100000);
    RooRealVar b("b", "b", 0, -100000, 100000);
    RooRealVar cp("cp", "cp", 0, -100000, 100000);
    RooRealVar d("d", "d", 0, -100000, 100000);
    RooRealVar e("e", "e", 0, -100000, 100000);
    RooRealVar f("f", "f", 0, -100000, 100000);
    RooGaussian gaus1("gaus1", "gaus1", x, mean, sigma1);
    RooGaussian gaus2("gaus2", "gaus2", x, mean, sigma2);
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b));
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp));
    RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp, d));
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp, d, e));
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp, d, e, f));
    RooRealVar polysig("polysig", "polysig", 10, 0, 1e9);
    RooAddPdf sum("sum", "sum", RooArgList(gaus1, gaus2, poly), RooArgList(sig1, sig2, polysig));

    RooFitResult* r;
    // for(int i = 0; i < 7; ++i)
    // {
    //     r = sum.fitTo(data, Save(), Minos(kTRUE), PrintLevel(-1));
    // }
    // r = sum.fitTo(data, Save(), Minos(kTRUE));

    // Keep retrying the fit until both the fit and the error matrix converge, or the limit on tries is exceeded

    unsigned tries = 0;
    do
    {
        ++tries;
        r = sum.fitTo(data, Save(), Minos(kTRUE), PrintLevel(-1));
    } while( !(r->status() == 0 && r->edm() < EDM_TOLERANCE && r->covQual() == 3) && tries <= MAX_TRIES );

    sum.plotOn(xframe, Name("sum"));
    sum.plotOn(xframe, Components(poly), LineStyle(kDashed));

    // r->Print();

    double chi2 = xframe->chiSquare("sum", "data");
    double meanf = mean.getVal();
    double meanfe = mean.getError();
    double sigmaf1 = sigma1.getVal();
    double sigmaf2 = sigma2.getVal();
    double bkgf = polysig.getVal();
    double sigf1 = sig1.getVal();
    double sigf2 = sig2.getVal();
    double sigwf1 = sigf1 / (sigf1 + sigf2);
    double sigwf2 = sigf2 / (sigf1 + sigf2);
    double c1 = a.getVal();
    double c2 = b.getVal();

    double sigmaf = sqrt(sigmaf1 * sigmaf1 * sigwf1 + sigmaf2 * sigmaf2 * sigwf2);
    double massmin = meanf - peakStDev * sigmaf;
    double massmax = meanf + peakStDev * sigmaf;

    int nmin = h->GetXaxis()->FindBin(massmin);
    int nmax = h->GetXaxis()->FindBin(massmax);
    int anmin = h->GetXaxis()->FindBin(massWindowLo);
    int anmax = h->GetXaxis()->FindBin(massWindowHi);

    double awyh1 = h->Integral(anmin, nmin);
    double awyh2 = h->Integral(nmax, anmax);
    double awyh = awyh1 + awyh2;
    double totyh = h->Integral(nmin, nmax);

    x.setRange("cut", massmin, massmax);
    RooAbsReal* ibkg = poly.createIntegral(x, NormSet(x), Range("cut"));
    RooAbsReal* isig1 = gaus1.createIntegral(x, NormSet(x), Range("cut"));
    RooAbsReal* isig2 = gaus2.createIntegral(x, NormSet(x), Range("cut"));
    double ibkgf = ibkg->getVal();
    // double bkgfe = polysig.getError();
    double isig1f = isig1->getVal();
    double isig2f = isig2->getVal();
    
    double bkgy = ibkgf * bkgf;
    // double bkgye = ibkgf*bkgfe;
    double sigy1 = isig1f * sigf1;
    double sigy2 = isig2f * sigf2;
    double sigy = sigy1 + sigy2;
    double toty = bkgy + sigy;
    
    double abkgy = (1 - ibkgf) * bkgf;
    double asigy1 = (1 - isig1f) * sigf1;
    double asigy2 = (1 - isig2f) * sigf2;
    double asigy = asigy1 + asigy2;
    // double awy = abkgy + asigy;
    
    double sigfrac = sigy / toty;
    double bkgfrac = bkgy / toty;
    
    double sigyh = totyh * sigfrac;
    double sigfrach = sigyh / totyh;
    double bkgyh = totyh * bkgfrac;
    double bkgfrach = bkgyh / totyh;
    
    double signif = sigy / sqrt(toty);

    double signifh = sigyh / sqrt(totyh);

    //***NEW***
    double yield_val = sigyield.getVal();   // value of total yield
    double yield_err = sigyield.getError(); // value of uncertainty in yield (+-)

    xframe->SetXTitle( "m(" + dau1name + dau2name + ") [GeV/c^{2}]" );
    xframe->GetXaxis()->CenterTitle();
    xframe->GetXaxis()->SetTitleOffset(1.2);
    xframe->GetXaxis()->SetLabelSize(0.02);
    xframe->SetYTitle(("Events(per " + to_string(massBinSize) + "GeV/c^{2})").c_str());
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

    TLine* l1 = new TLine(massmin, uymin, massmin, uymax);
    TLine* l2 = new TLine(massmax, uymin, massmax, uymax);
    l1->SetLineStyle(2);
    l2->SetLineStyle(2);
    l1->Draw("same");
    l2->Draw("same");

    TLatex latex;
    latex.SetTextAlign(12);
    latex.SetTextSize(0.03);
    latex.SetNDC();

    latex.DrawLatex(0.70, 0.85, Form("#frac{s}{#sqrt{s+b}} = %.3f", signif));
    latex.DrawLatex(0.70, 0.80, Form("#frac{s}{s+b} = %.3f", sigfrac));
    latex.DrawLatex(0.70, 0.75, Form("s+b = %.1f", toty));
    latex.DrawLatex(0.70, 0.70, Form("#mu = %.3f", meanf));
    latex.DrawLatex(0.70, 0.65, Form("#sigma = %.4f", sigmaf));

    //***NEW***
    latex.DrawLatex(0.70, 0.60, Form("yield = %.4f", yield_val));
    latex.DrawLatex(0.79, 0.57, Form("#pm %.4f", yield_err));

    // gMinuit->mnmatu(1);
    cout << "\n\n\n";
    r->Print("v");
    cout << "\n\n\n";
    cout << "Fit Status: " << r->status() << endl;
    cout << "Estimated Distance to Minimum: " << r->edm() << endl;
    cout << "Covariance Matrix Quality: " << r->covQual() << endl;

    cout << endl;

    if(r->status() == 0 && r->edm() < EDM_TOLERANCE && r->covQual() == 3)
        cout << "Fit was successful.\n";
    else
        cout << "Fit was UNSUCCESSFUL.\n";

    //***NEW***
    bool yield_asym_err = sigyield.hasAsymError();
    if(yield_asym_err)
        cout << "Signal yield HAS asymmetric error.\n";
    else
        cout << "Signal yield DOES NOT HAVE asymmetric error.\n";

    //***NEW***
    cout << endl;
    c->Print("rooFitHisto_fit_error" + imageType);
}