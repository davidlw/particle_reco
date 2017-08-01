#ifndef COMPARETRK_H_
#define COMPARETRK_H_

#include <cmath>
#include <vector>

#include <TMath.h>

#include "DataFormats/TrackReco/interface/Track.h"

namespace compareTrk_definition {

template <class T> bool compareTrk(T& trk, const vector<double>& vect)
{
    //vect should be of the form <pt, eta, phi, charge>
    if(trk.charge() != vect[3]) return false; //check matching charge

    double deltaR = sqrt( pow(trk.eta() - vect[1], 2) + pow(trk.phi() - vect[2], 2) );
    if(deltaR > 0.5) return false;  //check for low deltaR

    //check pt percent difference
    if( fabs((trk.pt() - vect[0]) / trk.pt()) > 0.5 ) return false;

    return true;
}

}	//namespace compareTrk_definition

#endif