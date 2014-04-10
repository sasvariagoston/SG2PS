// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef RETILT_HPP_
#define RETILT_HPP_

#include <vector>

#include "structs.h"

VCTR return_tilting_axis (GDB in, bool paleonorth);
double return_tilting_angle (GDB in, bool paleonorth);

VCTR ROT_generate_N_vector_plane (VCTR N, VCTR AXIS, double ANGLE);
VCTR ROT_generate_N_vector_lineation (VCTR D);
VCTR ROT_generate_D_vector_plane (VCTR N);
VCTR ROT_generate_D_vector_lineation (VCTR D, VCTR AXIS, double ANGLE);

bool SV_has_to_invert (VCTR N, VCTR AXIS, double ANGLE);

//VCTR invert_VCTR (VCTR in);

GDB TILT_DATA (GDB in, INPSET inset, bool by_paleonorth);

GDB S0_TILT (GDB inGDB, INPSET inSET);

vector <GDB> cGc_RETILT (vector <GDB> inGDB, INPSET inSET);

#endif
