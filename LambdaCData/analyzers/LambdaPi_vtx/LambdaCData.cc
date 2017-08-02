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

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"


//struct definition
#include "recoData/recoData.h"


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
        TNtuple* lcrecoTree;
        TH1D* eventCount;

        //for internal use
        edm::Service<TFileService> fs;
        double mass_lo, mass_hi;    // For cutting on LambdaC mass

        //for RECO particles
        edm::EDGetTokenT<reco::VertexCollection> tok_offlinePV_;
        edm::EDGetTokenT<reco::TrackCollection> tok_generalTrk_;

        edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> tok_lc_;

        edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > tok_dedx_;

        //for GEN particles
        edm::EDGetTokenT<reco::GenParticleCollection> tok_genParticle_;
        vector< vector<double> >* parentVect;    // vector of <pt, eta, phi, charge> vectors for LambdaC genParticles
        vector< vector<double> >* dau1Vect; // for LambdaC dau1s
        vector< vector<double> >* dau2Vect; // for LambdaC dau2s

        //vectors for <pt. eta, phi, charge> to be stored in parentVect, dau1Vect, dau2Vect
        vector<double>* Pvector;
        vector<double>* Dvector1;
        vector<double>* Dvector2;
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

    tok_lc_ = consumes<reco::VertexCompositeCandidateCollection>(iConfig.getUntrackedParameter<edm::InputTag>("LambdaCCollection"));

    tok_dedx_ = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxHarmonic2"));


    tok_genParticle_ = consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
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
    using std::vector;
    using namespace edm;

    //matched reco LambdaC


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
    
    //inner parts of the detector  
    if(bestvz < -15.0 || bestvz > 15.0) return;
    
    // Fill the counter
    eventCount->Fill(0);



    //gen particle info
    edm::Handle<reco::GenParticleCollection> genpars;
    iEvent.getByToken(tok_genParticle_, genpars);

    parentVect = new vector< vector<double> >;   //see class members for comments
    dau1Vect = new vector< vector<double> >;
    dau2Vect = new vector< vector<double> >;

    for(unsigned i = 0; i < genpars->size(); i++)
    {
        const reco::GenParticle& trk = (*genpars)[i];

        int id = trk.pdgId();
        if(fabs(id) != 4122) continue; //check for LambdaC
        if(trk.numberOfDaughters() != 2) continue; //check for 2-particle decay

        const reco::Candidate* Ld1 = trk.daughter(0);
        const reco::Candidate* Ld2 = trk.daughter(1);

        //(There shouldn't be any KsProton daughters, but just in case...)
        //do a swap if the Pi daughter is first in a LambdaPi pair, or the Proton daughter is first in a KsProton pair
        if( (fabs(Ld1->pdgId()) == 211 && fabs(Ld2->pdgId()) == 3122) || (fabs(Ld1->pdgId()) == 2212 && fabs(Ld2->pdgId()) == 310) )
        {
            const reco::Candidate* temp = Ld1;
            Ld1 = Ld2;
            Ld2 = temp;
            // std::cout << "SWAP! (" << Ld1->pdgId() << ")\n";
        }

        //now check the daughter identites...
        if( !(fabs(Ld1->pdgId()) == 3122 && fabs(Ld2->pdgId()) == 211) && !(fabs(Ld1->pdgId()) == 310 && fabs(Ld2->pdgId()) == 2212) )
            continue;

        //check for stable pi/proton daughter
        if(Ld2->status() != 1) continue;

        //eta cuts
        if(fabs(trk.eta()) > 2.4) continue;

        //y cuts
        if(fabs(trk.y()) >= 1) continue;

        //kinematics info for matching
        Pvector = new vector<double>;
        Dvector1 = new vector<double>;
        Dvector2 = new vector<double>;

        Pvector->push_back(trk.pt());
        Pvector->push_back(trk.eta());
        Pvector->push_back(trk.phi());
        Pvector->push_back(trk.charge());

        Dvector1->push_back(Ld1->pt());
        Dvector1->push_back(Ld1->eta());
        Dvector1->push_back(Ld1->phi());
        Dvector1->push_back(Ld1->charge());
        
        Dvector2->push_back(Ld2->pt());
        Dvector2->push_back(Ld2->eta());
        Dvector2->push_back(Ld2->phi());
        Dvector2->push_back(Ld2->charge());
        
        parentVect->push_back(*Pvector);
        dau1Vect->push_back(*Dvector1);
        dau2Vect->push_back(*Dvector2);
        
        delete Pvector;
        delete Dvector1;
        delete Dvector2;
    }



    // Now look at the reco level

    // Set output tree branches
    float mass_lc = -999.9, pt_lc = -999.9, eta_lc = -999.9, y_cm_lc = -999.9, \
        angle_lc = -999.9, dlos_lc = -999.9, vtx_prob_lc = -999.9, is_matched = -999.9;

    float mass_dau1 = -999.9, pt_dau1 = -999.9, eta_dau1 = -999.9, y_cm_dau1 = -999.9, angle_dau1 = -999.9, dlos_dau1 = -999.9;
    
    float pt_dau2 = -999.9, ptErr_dau2 = -999.9, p_dau2 = -999.9, eta_dau2 = -999.9, y_cm_dau2 = -999.9, \
        dcaz_dau2 = -999.9, dcaxy_dau2 = -999.9, H2dedx_dau2 = -999.9;

    lcrecoTree->SetBranchAddress("mass", &mass_lc);
    lcrecoTree->SetBranchAddress("pt", &pt_lc);
    lcrecoTree->SetBranchAddress("eta", &eta_lc);
    lcrecoTree->SetBranchAddress("y_cm", &y_cm_lc);
    lcrecoTree->SetBranchAddress("3DPointingAngle", &angle_lc);
    lcrecoTree->SetBranchAddress("3DDecayLengthSig", &dlos_lc);
    lcrecoTree->SetBranchAddress("vtx_prob", &vtx_prob_lc);
    lcrecoTree->SetBranchAddress("is_matched", &is_matched);

    lcrecoTree->SetBranchAddress("mass_dau1", &mass_dau1);
    lcrecoTree->SetBranchAddress("pt_dau1", &pt_dau1);
    lcrecoTree->SetBranchAddress("eta_dau1", &eta_dau1);
    lcrecoTree->SetBranchAddress("y_cm_dau1", &y_cm_dau1);
    lcrecoTree->SetBranchAddress("3DPointingAngle_dau1", &angle_dau1);
    lcrecoTree->SetBranchAddress("3DDecayLengthSig_dau1", &dlos_dau1);

    lcrecoTree->SetBranchAddress("pt_dau2", &pt_dau2);
    lcrecoTree->SetBranchAddress("ptErr_dau2", &ptErr_dau2);
    lcrecoTree->SetBranchAddress("p_dau2", &p_dau2);
    lcrecoTree->SetBranchAddress("eta_dau2", &eta_dau2);
    lcrecoTree->SetBranchAddress("y_cm_dau2", &y_cm_dau2);
    lcrecoTree->SetBranchAddress("xyDCASig_dau2", &dcaxy_dau2);
    lcrecoTree->SetBranchAddress("zDCASig_dau2", &dcaz_dau2);
    lcrecoTree->SetBranchAddress("dEdxHarmonic_dau2", &H2dedx_dau2);



    // Read the variables

    edm::Handle<reco::VertexCompositeCandidateCollection> lc_candidates;
    iEvent.getByToken(tok_lc_, lc_candidates);
    if(!lc_candidates.isValid()) return;

    edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxHandle;
    iEvent.getByToken(tok_dedx_, dEdxHandle);

    int nGen = (int)parentVect->size(); // Size of parentVect, dau1Vect, and dau2Vect

    for(unsigned i = 0; i < lc_candidates->size(); ++i)
    {
        const reco::VertexCompositeCandidate & lc = (*lc_candidates)[i];

        const reco::Candidate* dau1 = lc.daughter(0); // First is always Lambda/Kshort
        const reco::Candidate* dau2 = lc.daughter(1);

        reco::TrackRef trk_ref = dau2->get<reco::TrackRef>();  // Get the track reference from the track candidate pointer

        // Standard cuts
        eta_dau1 = dau1->eta();
        eta_dau2 = trk_ref->eta();
        eta_lc = lc.eta();
        if(fabs(eta_dau1) >= 2.4 || fabs(eta_dau2) >= 2.4 || fabs(eta_lc) >= 2.4) continue;


        mass_lc = lc.mass();
        if(mass_lc < mass_lo || mass_lc > mass_hi) continue;

        y_cm_lc = lc.y();
        if(fabs(y_cm_lc) >= 1) continue;


        if(!trk_ref->quality(reco::TrackBase::highPurity)) continue;
        if(fabs(trk_ref->charge()) != 1) continue;

        // Read values
        pt_dau1 = dau1->pt();
        mass_dau1 = dau1->mass();
        y_cm_dau1 = dau1->y();        

        pt_dau2 = trk_ref->pt();
        ptErr_dau2 = trk_ref->ptError();
        p_dau2 = trk_ref->p();
        y_cm_dau2 = dau2->y();

        pt_lc = lc.pt();


        // Apply optimized cuts
        if( !passesCuts(pt_lc, mass_dau1, pt_dau1, eta_dau1, pt_dau2, eta_dau2) ) continue;



        // Calculate dau1 values //
        double px_dau1 = dau1->px(), py_dau1 = dau1->py(), pz_dau1 = dau1->pz();

        // 3D pointing angle
        double tervx = -999.9, tervy = -999.9, tervz = -999.9;
        tervx = dau1->vx(); tervy = dau1->vy(); tervz = dau1->vz();

        double dxvtx_dau1 = tervx - bestvx;
        double dyvtx_dau1 = tervy - bestvy;
        double dzvtx_dau1 = tervz - bestvz;

        TVector3 ptotvec(dxvtx_dau1, dyvtx_dau1, dzvtx_dau1);
        TVector3 tervec(px_dau1, py_dau1, pz_dau1);

        angle_dau1 = tervec.Angle(ptotvec);    //to store

        if(cos(angle_dau1) <= 0.999) continue;  //cut on the cosine of the pointing angle

        // 3D Decay length significance
        typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
        typedef ROOT::Math::SVector<double, 3> SVector3;
        typedef ROOT::Math::SVector<double, 6> SVector6;
        
        SMatrixSym3D totalCov_dau1 = vtx.covariance() + dau1->vertexCovariance();
        SVector3 distanceVector_dau1(dxvtx_dau1, dyvtx_dau1, dzvtx_dau1);
        
        double dl_dau1 = ROOT::Math::Mag(distanceVector_dau1);
        double dlerror_dau1 = sqrt(ROOT::Math::Similarity(totalCov_dau1, distanceVector_dau1)) / dl_dau1;
        
        dlos_dau1 = dl_dau1 / dlerror_dau1;   //to store

        if(dlos_dau1 <= 5) continue; //cut on the decay length significance





        // Calculate dau2 values //

        // DCA significance
        math::XYZPoint bestvtx(bestvx, bestvy, bestvz);

        double dzvtx_dau2 = trk_ref->dz(bestvtx);
        double dxyvtx_dau2 = trk_ref->dxy(bestvtx);
        double dzerror_dau2 = sqrt(trk_ref->dzError()*trk_ref->dzError() + bestvzError*bestvzError);
        double dxyerror_dau2 = sqrt(trk_ref->d0Error()*trk_ref->d0Error() + bestvxError*bestvyError);

        //to store
        dcaz_dau2 = dzvtx_dau2 / dzerror_dau2;
        dcaxy_dau2 = dxyvtx_dau2 / dxyerror_dau2;

        // dE/dx

        if(dEdxHandle.isValid())
        {
            const edm::ValueMap<reco::DeDxData> dEdxTrack = *dEdxHandle.product();
            H2dedx_dau2 = dEdxTrack[trk_ref].dEdx();
        }

        // HERE IS WHERE TO APPLY DE/DX CUTS FOR KS+PROTON




        // Calculate parent values //

        // 3D pointing angle
        double secvx = -999.9, secvy = -999.9, secvz = -999.9;
        secvx = lc.vx(); secvy = lc.vy(); secvz = lc.vz();

        double dxvtx_lc = secvx - bestvx;
        double dyvtx_lc = secvy - bestvy;
        double dzvtx_lc = secvz - bestvz;

        TVector3 ptosvec(dxvtx_lc, dyvtx_lc, dzvtx_lc);
        TVector3 secvec(lc.px(), lc.py(), lc.pz());

        angle_lc = secvec.Angle(ptosvec);    //to store

        // 3D Decay length significance        
        SMatrixSym3D totalCov_lc = vtx.covariance() + lc.vertexCovariance();
        SVector3 distanceVector_lc(dxvtx_lc, dyvtx_lc, dzvtx_lc);
        
        double dl_lc = ROOT::Math::Mag(distanceVector_lc);
        double dlerror_lc = sqrt(ROOT::Math::Similarity(totalCov_lc, distanceVector_lc)) / dl_lc;
        
        dlos_lc = dl_lc / dlerror_lc;   //to store

        // Vertex probability
        vtx_prob_lc = TMath::Prob(lc.vertexChi2(), lc.vertexNdof());



        // delta R matching on LambdaC and both daughters
        bool match_bool = false;
        for(int j = 0; j < nGen; ++j)
        {
            match_bool = compareTrk(lc, (*parentVect)[j]) && compareTrk(*dau1, (*dau1Vect)[j]) && compareTrk(*dau2, (*dau2Vect)[j]);

            if(match_bool) break;
        }

        is_matched = static_cast<float>(match_bool);

        // Fill output tree
        lcrecoTree->Fill(1);
    }

    // Cleanup
    delete parentVect;
    delete dau1Vect;
    delete dau2Vect;
}

// ------------ method called once each job just before starting event loop  ------------
void 
LambdaCData::beginJob()
{
    TH1D::SetDefaultSumw2();

    // Create the output tree
    string LambdaCInfo = "mass:pt:eta:y_cm:3DPointingAngle:3DDecayLengthSig:vtx_prob:is_matched";
    string dau1Info = "mass_dau1:pt_dau1:eta_dau1:y_cm_dau1:3DPointingAngle_dau1:3DDecayLengthSig_dau1";
    string dau2Info = "pt_dau2:ptErr_dau2:p_dau2:eta_dau2:y_cm_dau2:zDCASig_dau2:xyDCASig_dau2:dEdxHarmonic_dau2";

    string branchesStr = LambdaCInfo + ":" + dau1Info + ":" + dau2Info;

    lcrecoTree = fs->make<TNtuple>("lambdac", "lambdac_reco", branchesStr.c_str());

    eventCount = fs->make<TH1D>("event_count" ,"event_count", 1, -1, 1);    // Event counter

    mass_lo = 2;
    mass_hi = 2.5;
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
