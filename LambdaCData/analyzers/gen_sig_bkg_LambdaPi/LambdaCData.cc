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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"

//struct definition
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
        TNtuple* lcMatchTree;
        TNtuple* lcBkgTree; //matched background
        TNtuple* lcGenTree;
        TH1D* eventCount;

        //for internal use

        //for matched reconstruction
        TNtuple* matchedLambdaTree;
        TNtuple* matchedPiTree;
        vector<Float_t> matchedEventInfo;  //vector for event info: has elements: ntrkoffline, vtxX, vtxY, vtxZ in that order

        //for matched background reconstruction
        TNtuple* bkgLambdaTree;
        TNtuple* bkgPiTree;
        vector<Float_t> bkgEventInfo;

        edm::Service<TFileService> fs;

        //for RECO particles
        edm::EDGetTokenT<reco::VertexCollection> tok_offlinePV_;
        edm::EDGetTokenT<reco::TrackCollection> tok_generalTrk_;
      
        // edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> tok_kshort_;
        edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> tok_lambda_;

        edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > tok_dedx_;

        //for GEN particles
        edm::EDGetTokenT<reco::GenParticleCollection> tok_genParticle_;
        vector< vector<double> >* pVect;    //vector of <pt, eta, phi, charge> vectors for daughters of LambdaC genParticles
        //vectors for <pt. eta, phi, charge> to be stored in pVect
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

    //use standard cuts (relatively high purity)
    tok_lambda_ = consumes<reco::VertexCompositeCandidateCollection>(iConfig.getUntrackedParameter<edm::InputTag>("LambdaCollection"));

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

    pVect = new vector< vector<double> >;   //see class members for comments

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


        //if all the checks were passed, store the genParticle/daughter info (store Lambda/Ks information FIRST)

        //raw gen info for output
        lcGenTree->Fill(    trk.pt(), trk.eta(), trk.y(), \
                            Ld1->pdgId(), Ld1->status(), \
                            Ld2->pdgId() );


        //kinematics info for matching
        Dvector1 = new vector<double>;
        Dvector2 = new vector<double>;

        Dvector1->push_back(Ld1->pt());
        Dvector1->push_back(Ld1->eta());
        Dvector1->push_back(Ld1->phi());
        Dvector1->push_back(Ld1->charge());
        Dvector1->push_back(Ld1->status()); //will be stored directly in the output tree if matched
        
        Dvector2->push_back(Ld2->pt());
        Dvector2->push_back(Ld2->eta());
        Dvector2->push_back(Ld2->phi());
        Dvector2->push_back(Ld2->charge());
        
        pVect->push_back(*Dvector1);
        pVect->push_back(*Dvector2);
        
        delete Dvector1;
        delete Dvector2;
    }



    // Now look at the reco level
      
    edm::Handle<reco::VertexCompositeCandidateCollection> v0candidates_la;
    iEvent.getByToken(tok_lambda_, v0candidates_la);
    if(!v0candidates_la.isValid()) return;


    edm::Handle<reco::TrackCollection> tracks;
    iEvent.getByToken(tok_generalTrk_, tracks);

    edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxHandle;
    iEvent.getByToken(tok_dedx_, dEdxHandle);




    // Go through each genParticle daughter pair

    // Find the matches and do reconstruction on them

    int nGenDau = (int)pVect->size();
    for(int i = 0; i < nGenDau; i+=2)
    {
        vector<double> Dvect1 = (*pVect)[i];
        vector<double> Dvect2 = (*pVect)[i + 1];

        //reset event/daughter info collections for each new genParticle pair
        matchedEventInfo.erase(matchedEventInfo.begin(), matchedEventInfo.end());
        matchedEventInfo.reserve(4);

        matchedLambdaTree = new TNtuple("matchedLambdaTree", "matchedLambdaTree", "px:py:pz:pt:eta:m:agl:dlsig:is_anti:stat");
        matchedPiTree = new TNtuple("matchedPiTree", "matchedPiTree", "px:py:pz:p:pt:pterr:eta:q:dedx:dcaz:dcaxy:nhits");

        //go through each Lambda
        for(unsigned j = 0; j < v0candidates_la->size(); j++)
        {
            //pick out the Lambdas that match with the current true Lambda
            const reco::VertexCompositeCandidate & la = (*v0candidates_la)[j];

            //standard cuts
            double eta_la = la.eta();

            if(fabs(eta_la) > 2.4) continue;
            //end standard cuts

            //compare the current Lambda with the true one
            if(!compareTrk(la, Dvect1)) continue;

            //calculate dau1 quantities

            double px_la = la.px(), py_la = la.py(), pz_la = la.pz();
            double mass_la = la.mass();

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
            else
                mass_la = -999.9;  //safeguard: if proton charge is odd, then mass_dau1 will be set to -999.9


            //fill the dau1 info tree
             matchedLambdaTree->Fill(px_la, py_la, pz_la, la.pt(), \
                eta_la, mass_la, angle, dlos, is_anti_la, Dvect1[4]);
        }


        //go through each track
        int matchedntrk = 0;   //counter for matched ntrk
        for(unsigned j = 0; j < tracks->size(); j++)
        {
            //pick out the tracks that match with the current true pion
            const reco::Track & trk = (*tracks)[j];

            //standard cuts
            double eta_trk = trk.eta();

            //CMS standard quality bit (set to filter by highPurity)
            if(!trk.quality(reco::TrackBase::highPurity)) continue;

            if(fabs(eta_trk) > 2.4) continue;
            //end standard cuts

            //compare the current track with the true pion
            if(!compareTrk(trk, Dvect2)) continue;

            //calculate dau2 values//

            //DCA significance
            math::XYZPoint bestvtx(bestvx, bestvy, bestvz);

            double dzvtx_trk = trk.dz(bestvtx);
            double dxyvtx_trk = trk.dxy(bestvtx);
            double dzerror_trk = sqrt(trk.dzError()*trk.dzError() + bestvzError*bestvzError);
            double dxyerror_trk = sqrt(trk.d0Error()*trk.d0Error() + bestvxError*bestvyError);

            //to store
            double dcaz_trk = dzvtx_trk / dzerror_trk;
            double dcaxy_trk = dxyvtx_trk / dxyerror_trk;

            //dE/dx
            reco::TrackRef trkref = reco::TrackRef(tracks, j);    //get a reference to the current track

            double H2dedx = -999.9; //to store

            if(dEdxHandle.isValid())
            {
                const edm::ValueMap<reco::DeDxData> dEdxTrack = *dEdxHandle.product();
                H2dedx = dEdxTrack[trkref].dEdx();
            }

            //fill the dau2 info tree
            matchedPiTree->Fill(trk.px(), trk.py(), trk.pz(), trk.p(), trk.pt(), trk.ptError(), \
                eta_trk, trk.charge(), H2dedx, dcaz_trk, dcaxy_trk, trk.numberOfValidHits());

            matchedntrk++;
        }


        //store matched event info
        matchedEventInfo.push_back(matchedntrk);
        matchedEventInfo.push_back(bestvx);
        matchedEventInfo.push_back(bestvy);
        matchedEventInfo.push_back(bestvz);


        //analysis
        //reconstruct the LambdaC using possible matches for Lambda/Pi, and fill lcMatchTree
        recoData(matchedEventInfo, matchedLambdaTree, matchedPiTree, lcMatchTree);


        //clear the matched daughter trees for the next true daughter pair
        delete matchedLambdaTree;
        delete matchedPiTree;
    }



    // Now look at the background

    // First count the number of valid tracks
    int bkgntrk = 0;
    for(unsigned i = 0; i < tracks->size(); i++)
    {
        const reco::Track & trk = (*tracks)[i];

        //standard cuts

        //CMS standard quality bit (set to filter by highPurity)
        if(!trk.quality(reco::TrackBase::highPurity)) continue;

        if(fabs(trk.eta()) > 2.4) continue;

        bkgntrk++;
    }

    // Store background event info
    bkgEventInfo.push_back(bkgntrk);
    bkgEventInfo.push_back(bestvx);
    bkgEventInfo.push_back(bestvy);
    bkgEventInfo.push_back(bestvz);


    // Now perform reconstruction on each pair that doesn't match with a genParticle pair

    // Go through each Lambda
    for(unsigned i = 0; i < v0candidates_la->size(); i++)
    {
        const reco::VertexCompositeCandidate & la = (*v0candidates_la)[i];

        //standard cuts
        double eta_la = la.eta();

        if(fabs(eta_la) > 2.4) continue;
        //end standard cuts


        //calculate dau1 quantities

        double px_la = la.px(), py_la = la.py(), pz_la = la.pz();
        double mass_la = la.mass();

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
        double dlerror = sqrt(ROOT::Math::Similarity(totalCov, distanceVector)) / dl;
        
        double dlos = dl / dlerror;   //to store

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
        else
            mass_la = -999.9;  //safeguard: if proton charge is odd, then mass_dau1 will be set to -999.9


        //make and fill the dau1 info tree
        bkgLambdaTree = new TNtuple("bkgLambdaTree", "bkgLambdaTree", "px:py:pz:pt:eta:m:agl:dlsig:is_anti:stat");

        bkgLambdaTree->Fill(px_la, py_la, pz_la, la.pt(), \
            eta_la, mass_la, angle, dlos, is_anti_la, -999.9);  //status_matched_dau1 is given the garbage value -999.9


        //go through each track
        for(unsigned j = 0; j < tracks->size(); j++)
        {
            const reco::Track & trk = (*tracks)[j];

            //standard cuts
            double eta_trk = trk.eta();

            //CMS standard quality bit (set to filter by highPurity)
            if(!trk.quality(reco::TrackBase::highPurity)) continue;

            if(fabs(eta_trk) > 2.4) continue;
            //end standard cuts


            // Check to make sure the current daughter pair doesn't match any of the genParticle pairs
            bool pair_matches = false;
            for(int k = 0; k < nGenDau; k+=2)
            {
                vector<double> Dvect1 = (*pVect)[k];
                vector<double> Dvect2 = (*pVect)[k + 1];

                if( compareTrk(la, Dvect1) && compareTrk(trk, Dvect2) )
                    pair_matches = true;
            }

            if(pair_matches) continue;  // If this pair does match, then skip it


            //calculate dau2 values//

            //DCA significance
            math::XYZPoint bestvtx(bestvx, bestvy, bestvz);

            double dzvtx_trk = trk.dz(bestvtx);
            double dxyvtx_trk = trk.dxy(bestvtx);
            double dzerror_trk = sqrt(trk.dzError()*trk.dzError() + bestvzError*bestvzError);
            double dxyerror_trk = sqrt(trk.d0Error()*trk.d0Error() + bestvxError*bestvyError);

            //to store
            double dcaz_trk = dzvtx_trk / dzerror_trk;
            double dcaxy_trk = dxyvtx_trk / dxyerror_trk;

            //dE/dx
            reco::TrackRef trkref = reco::TrackRef(tracks, j);    //get a reference to the current track

            double H2dedx = -999.9; //to store

            if(dEdxHandle.isValid())
            {
                const edm::ValueMap<reco::DeDxData> dEdxTrack = *dEdxHandle.product();
                H2dedx = dEdxTrack[trkref].dEdx();
            }

            //make and fill the dau2 info tree
            bkgPiTree = new TNtuple("bkgPiTree", "bkgPiTree", "px:py:pz:p:pt:pterr:eta:q:dedx:dcaz:dcaxy:nhits");

            bkgPiTree->Fill(trk.px(), trk.py(), trk.pz(), trk.p(), trk.pt(), trk.ptError(), \
                eta_trk, trk.charge(), H2dedx, dcaz_trk, dcaxy_trk, trk.numberOfValidHits());

            //do reconstruction on the background and fill lcBkgTree with 1 leaf
            recoData(bkgEventInfo, bkgLambdaTree, bkgPiTree, lcBkgTree);

            // Clear bkgPiTree every loop so it never has more than 1 entry
            delete bkgPiTree;
        }

        // Clear bkgLambdaTree every loop so it never has more than 1 entry
        delete bkgLambdaTree;
    }
}


// ------------ method called once each job just before starting event loop  ------------
void 
LambdaCData::beginJob()
{
    TH1D::SetDefaultSumw2();


    //n_candidates is the number of total reconstructed LambdaC in the group associated with the relevant genParticle
    string generalEventInfo = "ntrkoffline:vtxX:vtxY:vtxZ:n_reconstructions";
    string LambdaCInfo = "mass:pt:eta:y_cm";
    string dau1Info = "mass_dau1:pt_dau1:eta_dau1:y_cm_dau1:3DPointingAngle_dau1:3DDecayLengthSig_dau1:is_anti_dau1:status_dau1_matched";
        // status_dau1_matched is only relevant for the matched tree. It will be garbage for the background tree
    string dau2Info = "pt_dau2:ptErr_dau2:p_dau2:eta_dau2:y_cm_dau2:charge_dau2:zDCASig_dau2:xyDCASig_dau2:dEdxHarmonic_dau2:nhits_dau2";

    string branchesStr = generalEventInfo + ":" + LambdaCInfo + ":" + dau1Info + ":" + dau2Info;
    
    //create the output trees
    lcMatchTree = fs->make<TNtuple>("lambdac_match", "lambdac_match", branchesStr.c_str());
    lcBkgTree = fs->make<TNtuple>("lambdac_bkg", "lambdac_bkg", branchesStr.c_str());

    string LambdaCInfo_gen = "pt:eta:y_cm";
    string dau1Info_gen = "id_dau1:status_dau1";
    string dau2Info_gen = "id_dau2";

    string branchesStr_gen = LambdaCInfo_gen + ":" + dau1Info_gen + ":" + dau2Info_gen;

    //create the output tree
    lcGenTree = fs->make<TNtuple>("lambdac_gen", "lambdac_gen", branchesStr_gen.c_str());

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
