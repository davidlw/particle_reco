#ifndef PASSESCUTS_H_
#define PASSESCUTS_H_

#include <cmath>

bool passesCuts(double pt, double md1, double ptd1, double etad1, double ptd2, double etad2)
{
    const double md1_mu = 0.498;
    const double md1_sigma = 0.0067;

    if(pt < 2)
    {
        if( !(ptd1 > 1) ) return false;
        if( !(ptd2 > 0.4) ) return false;
        if( !(fabs(etad1) < 1.4) ) return false;
        if( !(fabs(etad2) < 1) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 2 && pt < 4)
    {
        if( !(ptd1 > 1.4) ) return false;
        if( !(ptd2 > 0.3) ) return false;
        if( !(fabs(etad1) < 1.4) ) return false;
        if( !(fabs(etad2) < 1) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 4 && pt < 6)
    {
        if( !(ptd1 > 0.5) ) return false;
        if( !(ptd2 > 0.5) ) return false;
        if( !(fabs(etad1) < 1.4) ) return false;
        if( !(fabs(etad2) < 1.9) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 6 && pt < 8)
    {
        if( !(ptd1 > 0.5) ) return false;
        if( !(ptd2 > 1.5) ) return false;
        if( !(fabs(etad1) < 1.4) ) return false;
        if( !(fabs(etad2) < 1.4) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 8 && pt < 10)
    {
        if( !(ptd1 > 1.6) ) return false;
        if( !(ptd2 > 2.3) ) return false;
        if( !(fabs(etad1) < 1.1) ) return false;
        if( !(fabs(etad2) < 1.1) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }
    else if(pt >= 10)
    {
        if( !(ptd1 > 2.5) ) return false;
        if( !(ptd2 > 3.5) ) return false;
        if( !(fabs(etad1) < 1.1) ) return false;
        if( !(fabs(etad2) < 1.1) ) return false;
        if( !( fabs((md1 - md1_mu) / md1_sigma) <= 2 ) ) return false;

        return true;
    }

    return true; // default, the function should never reach this point
}


#endif