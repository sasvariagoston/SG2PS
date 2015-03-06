
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef RETILT_HPP_
#define RETILT_HPP_

#include <vector>

#include "structs.h"

VCTR return_tilting_axis (const GDB& in, const bool paleonorth); //ok
double return_tilting_angle (const GDB& in, const bool paleonorth); //ok

bool SV_has_to_invert (const VCTR& N, const VCTR& AXIS, const double ANGLE);

GDB TILT_DATA (const GDB& in, const bool TILT_BY_PALEONORTH);
GDB S0_TILT (const GDB& in);//ok
vector < vector < vector < vector <GDB> > > > RETILT (const vector < vector < vector < vector <GDB> > > >& inGDB); //ok

#endif
