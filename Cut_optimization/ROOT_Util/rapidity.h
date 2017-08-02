//
// rapidity.h
// ROOT_Util/
//
// Created by Johann Gan on 6/13/17
//
// Calculates a particle's rapidity from its eta, mass, and transverse momentum.
//      <cm_correction> is an optional correction for calculating center of mass rapidity via the relation:
//          y_cm = y + cm_correction

/////////////////////////////////////////////////

#ifndef RAPIDITY_H_
#define RAPIDITY_H_

/////////////////////////////////////////////////

#include <cmath>

/////////////////////////////////////////////////


double rapidity(double eta, double m, double pt, double cm_correction = 0)
{
    return log( (hypot(m, pt * cosh(eta)) + pt * sinh(eta)) / hypot(m, pt) ) + cm_correction;
}


#endif /* RAPIDITY_H_ */
