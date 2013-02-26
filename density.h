// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef DENSITY_HPP_
#define DENSITY_HPP_


#include <algorithm>
#include <vector>

#include "structs.h"


XY stereonet_coordinate_from_DIPDIR_DIP (DIPDIR_DIP in, CENTER center, INPSET inset);

VCTR density_color_from_percentage (double percentage);

//DENSITY density_in_cell (vector <GDB> in, size_t search_dipdir, size_t search_dip, size_t radius);

//vector <DENSITY> generate_density_vector (vector <GDB> in, size_t radius);

void plot_densities (vector <GDB> inGDB, vector <GDB> tiltinGDB, ofstream& o, INPSET inset, CENTER center, PAPER P);

#endif
