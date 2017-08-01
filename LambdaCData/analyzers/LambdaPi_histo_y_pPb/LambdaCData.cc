// -*- C++ -*-
//
// Package:    LambdaCData/LambdaCData
// Class:      LambdaCData
// 
/**\class LambdaCData LambdaCData.cc LambdaCData/LambdaCData/plugins/LambdaCData.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Johann Gan
//         Created:  Wed, 24 May 2017 21:01:03 GMT
//
//


// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <TMath.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TVector3.h>

#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>

//user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"


#include "recoData/recoData.cpp"


//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class LambdaCData : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
    public:
        explicit LambdaCData(const edm::ParameterSet&);
        ~LambdaCData();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        // ----------member data ---------------------------

        //for output
        vector<TH1D*> lcrecoHistos;
        vector<TH1D*> wrongSignHistos;
        TH1D* eventCount;

        //for internal use
        TNtuple* lambdaTree;
        TNtuple* piTree;

        edm::Service<TFileService> fs;

        edm::EDGetTokenT<reco::VertexCollection> tok_offlinePV_;
        edm::EDGetTokenT<reco::TrackCollection> tok_generalTrk_;
      
        edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> tok_lambda_;

        edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > tok_dedx_;
};

//
// constructors and destructor
//
LambdaCData::LambdaCData(const edm::ParameterSet& iConfig)

{
    //now do what ever initialization is needed
    usesResource("TFileService");

    //position, number of tracks, total energy, resolution (errors)
    tok_offlinePV_ = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));

    //like genParticles, p, E, NO id/mass
    tok_generalTrk_ = consumes<reco::TrackCollection>(edm::InputTag("generalTracks"));

    //use standard cuts (relatively high purity)
    tok_lambda_ = consumes<reco::VertexCompositeCandidateCollection>(iConfig.getUntrackedParameter<edm::InputTag>("LambdaCollection"));

    tok_dedx_ = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxHarmonic2"));
}


LambdaCData::~LambdaCData()
{
 
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
LambdaCData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    //reset daughter info collections
    lambdaTree = new TNtuple("lambdaTree", "lambdaTree", "px:py:pz:pt:eta:m:agl:dlsig:is_anti");
    piTree = new TNtuple("piTree", "piTree", "px:py:pz:p:pt:pterr:eta:q:dedx:dcaz:dcaxy");

    lambdaTree->SetDirectory(0);
    piTree->SetDirectory(0);



    //standard event selection
    // select on requirement of valid vertex
    edm::Handle<reco::VertexCollection> vertices;
    iEvent.getByToken(tok_offlinePV_, vertices);
    double bestvz = -999.9, bestvx = -999.9, bestvy = -999.9;
    double bestvzError = -999.9, bestvxError = -999.9, bestvyError = -999.9;


    //we assume there's only one vertex, as we configured most events to have only one collision
    //Vertex collections are sorted by the sum of Pt^2 of tracks associated with the vertex. The vertex with the highest sum
    //and therefore most likely to be the signal vertex, appears first
    const reco::Vertex & vtx = (*vertices)[0];
    bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
    bestvzError = vtx.zError(); bestvxError = vtx.xError(); bestvyError = vtx.yError();
    
    //cut to inner parts of the detector  
    if(bestvz < -15.0 || bestvz > 15.0) return;
      
    // Fill the counter
    eventCount->Fill(0);

    //more reading
      
    edm::Handle<reco::VertexCompositeCandidateCollection> v0candidates_la;
    iEvent.getByToken(tok_lambda_, v0candidates_la);
    if(!v0candidates_la.isValid()) return;

    edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxHandle;
    iEvent.getByToken(tok_dedx_, dEdxHandle);
      
    edm::Handle<reco::TrackCollection> tracks;
    iEvent.getByToken(tok_generalTrk_, tracks);


    // Iterate through tracks
    // int ntrk = 0;   //counter for ntrkoffline
    for(unsigned i = 0; i < tracks->size(); i++)
    {
        const reco::Track & trk = (*tracks)[i];

        //standard cuts
        double eta_trk = trk.eta();

        //CMS standard quality bit (set to filter by highPurity)
        if(!trk.quality(reco::TrackBase::highPurity)) continue;
        
        if(fabs(eta_trk) > 2.4) continue;
        //end standard cuts


        //calculate values//

        //DCA significance
        math::XYZPoint bestvtx(bestvx, bestvy, bestvz);

        double dzvtx = trk.dz(bestvtx);
        double dxyvtx = trk.dxy(bestvtx);
        double dzerror = sqrt(trk.dzError()*trk.dzError() + bestvzError*bestvzError);
        double dxyerror = sqrt(trk.d0Error()*trk.d0Error() + bestvxError*bestvyError);

        //to store
        double dcaz_trk = dzvtx / dzerror;
        double dcaxy_trk = dxyvtx / dxyerror;

        //dE/dx
        reco::TrackRef trkref = reco::TrackRef(tracks, i);    //get a reference to the current track

        double H2dedx = -999.9; //to store

        if(dEdxHandle.isValid())
        {
            const edm::ValueMap<reco::DeDxData> dEdxTrack = *dEdxHandle.product();
            H2dedx = dEdxTrack[trkref].dEdx();
        }


        //fill pi tree
        piTree->Fill(trk.px(), trk.py(), trk.pz(), trk.p(), trk.pt(), trk.ptError(), \
            eta_trk, trk.charge(), H2dedx, dcaz_trk, dcaxy_trk);

        // ntrk++;
    }


    //from Lambda
    for(unsigned i = 0; i < v0candidates_la->size(); i++)
    {
        const reco::VertexCompositeCandidate & la = (*v0candidates_la)[i];

        double eta_la = la.eta();
        if(fabs(eta_la) > 2.4) continue;  //standard cuts

        //calculate values
        double px_la = la.px(), py_la = la.py(), pz_la = la.pz();

        //3D pointing angle
        double secvz = -999.9, secvx = -999.9, secvy = -999.9;
        secvz = la.vz(); secvx = la.vx(); secvy = la.vy();

        double dzvtx = secvz - bestvz;
        double dyvtx = secvy - bestvy;
        double dxvtx = secvx - bestvx;

        TVector3 ptosvec(dxvtx, dyvtx, dzvtx);
        TVector3 secvec(px_la, py_la, pz_la);

        double angle = secvec.Angle(ptosvec);    //to store

        if(cos(angle) <= 0.999) continue;  //cut on the cosine of the pointing angle

        //3D Decay length significance
        typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
        typedef ROOT::Math::SVector<double, 3> SVector3;
        typedef ROOT::Math::SVector<double, 6> SVector6;
        
        SMatrixSym3D totalCov = vtx.covariance() + la.vertexCovariance();
        SVector3 distanceVector(dxvtx, dyvtx, dzvtx);
        
        double dl = ROOT::Math::Mag(distanceVector);
        double dlerror = sqrt(ROOT::Math::Similarity(totalCov, distanceVector))/dl;
        
        double dlos = dl/dlerror;   //to store

        if(dlos <= 5) continue; //cut on the decay length significance

        //Determine if this is a Lambda or an anti-Lambda
        const reco::Candidate* daudau1 = la.daughter(0);
        const reco::Candidate* daudau2 = la.daughter(1);

        const reco::Candidate* la_proton;
        if( fabs(daudau1->p()) >= fabs(daudau2->p()) )
            la_proton = daudau1;
        else
            la_proton = daudau2;

        double is_anti_la = -999.9;
        if(la_proton->charge() == 1)
            is_anti_la = 0;
        else if(la_proton->charge() == -1)
            is_anti_la = 1;

        //fill the tree
        lambdaTree->Fill(px_la, py_la, pz_la, la.pt(), \
            eta_la, la.mass(), angle, dlos, is_anti_la);
    }

    //analysis
    recoData(lambdaTree, piTree, lcrecoHistos, wrongSignHistos);  //reconstructs the LambdaC and fill the histos


    //cleanup
    delete lambdaTree;
    delete piTree;
}


// ------------ method called once each job just before starting event loop  ------------
void 
LambdaCData::beginJob()
{
    TH1D::SetDefaultSumw2();

    double masslo = 2, masshi = 2.5;
    double binsize = 5e-3;

    int nbins = round( (masshi - masslo) / binsize );

    
    //create the histos
    lcrecoHistos.reserve(9);
    wrongSignHistos.reserve(lcrecoHistos.capacity());
    for(unsigned i = 0; i < lcrecoHistos.capacity(); i++)
    {
        lcrecoHistos.push_back( fs->make<TH1D>(("lambdac_" + to_string(i + 1)).c_str(), \
            ("lambdac_reco" + to_string(i + 1)).c_str(), nbins, masslo, masshi) );
        wrongSignHistos.push_back( fs->make<TH1D>(("wrong_sign_" + to_string(i + 1)).c_str(), \
            ("wrong_sign" + to_string(i + 1)).c_str(), nbins, masslo, masshi) );
    }
    eventCount = fs->make<TH1D>("event_count" ,"event_count", 1, -1, 1);    // Event counter
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LambdaCData::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LambdaCData::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(LambdaCData);
