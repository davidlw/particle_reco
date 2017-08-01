#ifndef CALCKININFO_H_
#define CALCKININFO_H_

#include <cmath>

#include "TMath.h"

#include "ple.h"

using namespace std;
using namespace ple_definition;

namespace calcKinInfo {

double sqr(Float_t a)
{
    //utility function

    return a*a;
}



double calcMass(const ple& part)
{
    //requires part.p4 to be defined
    return pow( sqr(part.p4[3]) - (sqr(part.p4[0]) + sqr(part.p4[1]) + sqr(part.p4[2])), 0.5 );
}

double calcP(const ple& part)
{
    //requires part.p4[0, 1, 2]
    return pow( sqr(part.p4[0]) + sqr(part.p4[1]) + sqr(part.p4[2]) , 0.5 );
}

double calcPt(const ple& part)
{
    //requires part.p4[0, 1] to be defined
    return pow( sqr(part.p4[0]) + sqr(part.p4[1]), 0.5 );
}

double calcEta(const ple& part)
{
    //requires part.p4[0, 1, 2] to be defined
    return TMath::ATanH(part.p4[2] / calcP(part));
}

double calcEnergy(const ple& part)
{
    //requires part.mass and part.p4[0, 1, 2] to be defined
    return pow( sqr(part.mass) + sqr(calcP(part)) , 0.5 );
}

}	//namespace calcKinInfo

#endif