//
// rooFitHisto_error.cpp
// 
// Created by Johann Gan on 7/12/17.
//
// Fits a histogram from a .root file to a curve made by summing a double Gaussian with a shared mean and a polynomial function.
// Retries the fit until one of the following conditions is satisfied:
//     1. The fit converges, the fit errors converge, AND the estimated distance to minimum is below a user-specified threshold.
//     2. The number of fit attempts exceeds a user-specified maximum.
//
//////////////////////////////////////
//
// --- USAGE INSTRUCTIONS ---
//
// ~~ Running ~~
// Assuming ROOT is installed, to run, go into the directory containing this macro, and run "root -l -b -q rooFitHisto_error.cpp"
// through the command line. Options on the root command can be adjusted as desired.
//
//
// ~~ Reading the output ~~
// When the program terminates, the last few
// lines of output will look something like the following:
//
//     Fit Status: 0
//     Estimated Distance to Minimum: 6.29858e-08
//     Covariance Matrix Quality: 3
// 
//     Fit was successful.
//     Signal yield HAS asymmetric error.
//
//     Total yield: 73.5035  +-  0.216491
//
//     Info in <TCanvas::Print>: png file rooFitHisto_error_fit.png has been created
//
// Make sure that the program outputs the line "Fit was successful." If the output is instead "Fit was UNSUCCESSFUL.", check
// the 3 lines above the aforementioned line to see what went wrong, change the fit configuration, then rerun the macro.
// Examples of changes include:
//     1. Raising EDM_TOLERANCE.
//     2. Increasing MAX_TRIES.
//     3. Changing the initial guesses and limits on the fit parameters.
//     4. Changing the polynomial background order.
//     5. Changing the fitting range (massWindowHi/massWindowLo).
//
//     Fit status 0 is desired. If this number is not 0, then the fit did not converge.
//     Estimated Distance to Minimum should be under a user-specified value (see below). Ideally, it should be very small (<< 1).
//     Covariance matrix quality code 3 is desired. If this number is different from 3, then the errors of the fit did not converge.
//
// Note: Even if the output states that the signal yield has asymmetric error, this is just a warning.
// The output will still use symmetric error. To use asymmetric error, modify the code.
//
// To read the total yield and error of the double Gaussian signal curve, one can look at either the console output or the image
// output. Note that one should always look at the image output to make sure the fit is reasonable, regardless of whether or not
// the fit status is 0.
//
// 
// ~~ Configuration ~~
// Most of the configuration is listed before the function definitions, and are enclosed between the "// CONFIGURATION //" and
// "// END CONFIGURATION //" lines. However, some of the configuration for the fitting parameters must be done inside the macro.
// These sections can be found in the sections labeled "// FIT PARAMETERS //" and "// POLYNOMIAL BACKGROUND ORDER //" near the
// beginning of the doRooFit() function definition.
//
// // CONFIGURATION //
// peakMass - The initial guess for the mean of the double Gaussians. The mean is allowed to vary around this initial guess.
// massWindowLo and massWindowHi - Define the x-axis range for the input histogram to use for fitting.
// peakStDev - The number of standard deviations away from the mean with which to define the "peak" region for the calculation
//     of signal significance and the signal yield fraction. Not used for total yield calculation.
// dau1name and dau2name - TLatex string for the names of the daughters of the reconstructed candidates. Used for histogram labeling.
//
// infileName - The name of the input .root file containing the histogram object to be fit.
// pathToHisto - Internal path within the .root file to the histogram object to be fit.
// outfileName - Path to the desired output image file for the fit.
//
// MAX_TRIES - The maximum number of time to attempt a fit before giving up.
// EDM_TOLERANCE - The upper bound for acceptable values of estimated distance to minimum.
//
// // FIT PARAMETERS //
// mean - The shared mean of both Gaussians.
// sigma1 and sigma 2 - The standard deviations of the first and second Gaussians, respectively.
// sig1prop - Proportion of the total yield that comes from the first Gaussian curve. Must be between 0 and 1.
// sig1yield - Total yield of the double Gaussian curve. This is the variable that is read when outputting the total yield.
// 
// // POLYNOMIAL BACKGROUND ORDER //
// a, b, cp, d, e, f - Polynomial coefficients. Not all are used, and more can be created if necessary.
// poly - Polynomial function object. Can be instantiated to be whatever order is desired, using the coefficients mentioned above.
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

//////////////////////////////////////

// CONFIGURATION //

// Lambda information
// const double peakMass = 1.115683;
// const double massWindowLo = 1.08;
// const double massWindowHi = 1.16;

// Kshort information
// const double peakMass = 0.497611;
// const double massWindowLo = 0.43;
// const double massWindowHi = 0.56;

// LambdaC information
const double peakMass = 2.28646;
const double massWindowLo = 2.19;
const double massWindowHi = 2.39;
const double peakStDev = 2;
const TString dau1name = "#Lambda^{0}";
const TString dau2name = "#pi";
// const TString dau1name = "K_{s}^{0}";
// const TString dau2name = "p";


// File configuration
const TString infileName = "MC_LambdaC_L_histos_merged.root";
const TString pathToHisto = "lcreco/lambdac_5";
const TString outfileName = "rooFitHisto_error_fit.png";


// Process parameters
const unsigned MAX_TRIES = 20;
const double EDM_TOLERANCE = 1e-5;

// END CONFIGURATION //


void doRooFit(TH1D*);

void rooFitHisto_error()
{
    TFile f(infileName);

    TH1D* h = (TH1D*)f.Get(pathToHisto);
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
    double massBinSize = h->GetBinWidth(1);

    RooRealVar x("x", "mass", massWindowLo, massWindowHi);
    RooDataHist data("data", "dataset", x, h);
    RooPlot* xframe = x.frame(nMassBins);
    data.plotOn(xframe,Name("data"));


    // FIT PARAMETERS //
    // Change only the last three arguments in the constructors. From left to right, these three arguments are:
    // 1. Initial guess for the parameter
    // 2. Lower bound for the parameter.
    // 3. Upper bound for the parameter.

    RooRealVar mean("mean", "mean", peakMass, peakMass - 0.01, peakMass + 0.01); // shared mean of both Gaussians
    RooRealVar sigma1("sigma1", "sigma1", 0.015, 0.005, 0.015); // sigma of the first Gaussian
    RooRealVar sigma2("sigma2", "sigma2", 0.015, 0.005, 0.015); // sigma of the second Gaussian

    RooRealVar sig1prop("sig1prop", "signal1prop", 0.5, 0, 1);  // weight of the first Gaussian
    RooRealVar sigyield("sigyield", "sigyield", 10, 0, 1e9);    // total combined yield of the two Gaussians

    // END FIT PARAMETERS //


    RooFormulaVar sig1("sig1", "@0 * @1", RooArgList(sigyield, sig1prop));  // total yield of the first Gaussian
    RooFormulaVar sig2("sig2", "@0 * (1 - @1)", RooArgList(sigyield, sig1prop)); // total yield of the second Gaussian

    // The two Gaussians
    RooGaussian gaus1("gaus1", "gaus1", x, mean, sigma1);
    RooGaussian gaus2("gaus2", "gaus2", x, mean, sigma2);


    // POLYNOMIAL BACKGROUND ORDER //
    // Uncomment whichever definition of the "poly" object provides the desired polynomial background order.
    // If higher orders are desired, add new RooRealVars after "f" (these are the coefficients of poly), and
    // add a new line instantiating poly with whatever order is desired.

    RooRealVar a("a", "a", 0, -100000, 100000);
    RooRealVar b("b", "b", 0, -100000, 100000);
    RooRealVar cp("cp", "cp", 0, -100000, 100000);
    RooRealVar d("d", "d", 0, -100000, 100000);
    RooRealVar e("e", "e", 0, -100000, 100000);
    RooRealVar f("f", "f", 0, -100000, 100000);
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b));                 // 1st order background
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp));             // 2nd order background
    RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp, d));             // 3rd order background
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp, d, e));       // 4th order background
    // RooPolynomial poly("poly", "poly", x, RooArgList(a, b, cp, d, e, f));    // 5th order background

    RooRealVar polysig("polysig", "polysig", 10, 0, 1e9);   // Weight of the polynomial

    // END POLYNOMIAL BACKGROUND ORDER //


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

    bool yield_asym_err = sigyield.hasAsymError();
    if(yield_asym_err)
        cout << "Signal yield HAS asymmetric error.\n";
    else
        cout << "Signal yield DOES NOT HAVE asymmetric error.\n";

    cout << "\nTotal yield: " << yield_val << "  +-  " << yield_err << "\n\n";

    c->Print(outfileName);
}