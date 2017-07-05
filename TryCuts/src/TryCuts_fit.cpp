#include "TryCuts_fit.h"

int main(int argc, char* argv[])
{
    string cfg_file_name = "config.xml";
    string out_tag = "";

    if(argc > 1)
        cfg_file_name = argv[1];
    if(argc > 2)
        out_tag = argv[2];

    TryCuts_fit(cfg_file_name, out_tag);
}



void TryCuts_fit(const string& config_file_name, const string& output_tag)
{
    cout << "Loading configurations...\n";

    Configuration::Configuration cfg(config_file_name);
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



    // Open the histos output file generated by readxml
    TFile histoFile(cfg.get_histoFile().c_str());

    
    // Fit the histos to get signal significance
    vector<double> sigsig(cfg.n_histos(), -1);
    vector<double> sigeff(cfg.n_histos(), -1);

    double massWindowLo = cfg.get_hrange()[0], massWindowHi = cfg.get_hrange()[1];
    string dau1name = cfg.get_dau1name(), dau2name = cfg.get_dau2name();
    string imageType = cfg.get_imageType();
    double peakMass = cfg.get_peakMass(), peakStDev = cfg.get_peakStd();


    TH1D* h;

    for(int icut = 1; icut <= cfg.n_histos(); icut++)
    {
        RooMsgService::instance().setStreamStatus(0,kFALSE);
        RooMsgService::instance().setStreamStatus(1,kFALSE);

        TCanvas *c = new TCanvas("c","c",1600,1600);
        c->cd();
        h = (TH1D*)histoFile.Get(Form("cut_%d", icut));      

        RooRealVar x("x", "mass", massWindowLo, massWindowHi);
        RooDataHist data("data","dataset", x, h);
        RooPlot* xframe = x.frame(cfg.n_bins());
        data.plotOn(xframe,Name("data"));
        RooRealVar mean("mean", "mean", peakMass, peakMass - 0.01, peakMass + 0.01);
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
        
        double chi2 = xframe->chiSquare("sum","data");
        double meanf = mean.getVal();
        double meanfe = mean.getError();
        double sigmaf1 = sigma1.getVal();
        double sigmaf2 = sigma2.getVal();
        double bkgf = polysig.getVal();
        double sigf1 = sig1.getVal();
        double sigf2 = sig2.getVal();
        double sigwf1 = sigf1/(sigf1 + sigf2);
        double sigwf2 = sigf2/(sigf1 + sigf2);
        double c1 = a.getVal();
        double c2 = b.getVal();
        
        double sigmaf = sqrt(sigmaf1 * sigmaf1 * sigwf1 + sigmaf2 * sigmaf2 * sigwf2);
        double massmin = meanf - peakStDev * sigmaf;
        double massmax = meanf + peakStDev * sigmaf;

        int nmin = h->GetXaxis()->FindBin(massmin);
        int nmax = h->GetXaxis()->FindBin(massmax);
        int anmin = h->GetXaxis()->FindBin(massWindowLo);
        int anmax = h->GetXaxis()->FindBin(massWindowHi);
        
        double awyh1 = h->Integral(anmin,nmin);
        double awyh2 = h->Integral(nmax,anmax);
        double awyh = awyh1 + awyh2;
        double totyh = h->Integral(nmin,nmax);
        
        x.setRange("cut",massmin,massmax);
        RooAbsReal* ibkg = poly.createIntegral(x,NormSet(x),Range("cut"));
        RooAbsReal* isig1 = gaus1.createIntegral(x,NormSet(x),Range("cut"));
        RooAbsReal* isig2 = gaus2.createIntegral(x,NormSet(x),Range("cut"));
        double ibkgf = ibkg->getVal();
        double bkgfe = polysig.getError();
        double isig1f = isig1->getVal();
        double isig2f = isig2->getVal();
        
        double bkgy = ibkgf*bkgf;
        double bkgye = ibkgf*bkgfe;
        double sigy1 = isig1f*sigf1;
        double sigy2 = isig2f*sigf2;
        double sigy = sigy1 + sigy2;
        double toty = bkgy + sigy;
        
        double abkgy = (1-ibkgf)*bkgf;
        double asigy1 = (1-isig1f)*sigf1;
        double asigy2 = (1-isig2f)*sigf2;
        double asigy = asigy1 + asigy2;
        double awy = abkgy + asigy;
        
        double sigfrac = sigy/toty;
        double bkgfrac = bkgy/toty;
        
        double sigyh = totyh - bkgy;
        double sigfrach = sigy/totyh;
        double bkgfrach = bkgy/totyh;
        
        double signif = sigy/sqrt(totyh);
        sigsig[icut] = signif;
        sigeff[icut] = icut;
        xframe->SetXTitle( ("m(" + dau1name + dau2name + ") [GeV/c^{2}]").c_str() );
        xframe->GetXaxis()->CenterTitle();
        xframe->GetXaxis()->SetTitleOffset(1.2);
        xframe->GetXaxis()->SetLabelSize(0.02);
        xframe->SetYTitle(("Events(per " + to_string(cfg.get_binsize()) + "GeV/c^{2})").c_str());
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

        c->Print(Form(("./fits/cut_%d" + output_tag + imageType).c_str(), icut));

        delete h;
    }


    // Print the cuts' signal significances
    for(int icut = 1; icut <= cfg.n_histos(); icut++)
    {
        cout << "cut " << icut << ": ss = " << sigsig[icut] << endl;
    }
}