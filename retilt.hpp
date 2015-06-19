
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef RETILT_HPP_
#define RETILT_HPP_

#include <vector>

#include "structs.h"

VCTR return_tilting_axis (const GDB& in, const string METHOD);
double return_tilting_angle (const GDB& in, const string METHOD);

GDB tilt_lineation (const GDB& in, const VCTR& AXIS, const double ANGLE);
GDB tilt_plane (const GDB& in, const VCTR& AXIS, const double ANGLE);
GDB tilt_striae (const GDB& in, const VCTR& AXIS, const double ANGLE);

GDB TILT_DATA (const GDB& in, const string METHOD);
//GDB S0_TILT (const GDB& in);

vector < vector <GDB> > RETILT (const vector < vector <GDB> >& inGDB, const string METHOD); //ok

#endif
