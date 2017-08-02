#ifndef PASSESCUTS_H_
#define PASSESCUTS_H_

#include <cmath>

bool passesCuts(double pt, double md1, double ptd1, double etad1, double ptd2, double etad2)
{
    const double md1_mu = 1.116;
    const double md1_sigma = 0.0028;

    if(pt < 4)
    {
        if( !(ptd1 > 1) ) return false;
        if( !(ptd2 > 1.2) ) return false;
        if( !(fabs(etad1) < 1.1) ) return false;
        if( !(fabs(etad2) < 1.1) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 4 && pt < 6)
    {
        if( !(ptd1 > 1) ) return false;
        if( !(ptd2 > 1.1) ) return false;
        if( !(fabs(etad1) < 1.3) ) return false;
        if( !(fabs(etad2) < 1.3) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 6 && pt < 8)
    {
        if( !(ptd1 > 1.5) ) return false;
        if( !(ptd2 > 1.5) ) return false;
        if( !(fabs(etad1) < 1.5) ) return false;
        if( !(fabs(etad2) < 1.5) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 8 && pt < 10)
    {
        if( !(ptd1 > 2.7) ) return false;
        if( !(ptd2 > 1.4) ) return false;
        if( !(fabs(etad1) < 1.4) ) return false;
        if( !(fabs(etad2) < 1.4) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 10)
    {
        if( !(ptd1 > 3.5) ) return false;
        if( !(ptd2 > 2.5) ) return false;
        if( !(fabs(etad1) < 1.1) ) return false;
        if( !(fabs(etad2) < 1.2) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }

    return true; // default, the function should never reach this point
}


#endif