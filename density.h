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

vector <GRID_CENTER> generate_rectangular_grid_from_triange_center (size_t cell_number);

vector <GRID_CENTER> calculate_grid_cell_values_from_triangle (vector <GRID_CENTER> rect_grid, vector <GRID_CENTER> tri_center);

bool by_GRID_COUNT (const GRID_CENTER& x, const GRID_CENTER& y);

vector <GRID_CENTER> sort_by_GRID_COUNT (vector <GRID_CENTER> GC);

vector <GRID_CENTER> normalize_grid_cell_values (vector <GRID_CENTER> rect_grid, vector <GRID_CENTER> tri_center);

void dbg_cout_rect_grid (vector <GRID_CENTER> rect_grid);

#endif
