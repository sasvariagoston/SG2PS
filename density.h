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

//void plot_densities (vector <GDB> inGDB, vector <GDB> tiltinGDB, ofstream& o, INPSET inset, CENTER center, PAPER P);



vector < vector <GRID_CENTER> > generate_rectangular_grid_from_triange_center (size_t cell_number);

vector < vector <GRID_CENTER> > calculate_grid_cell_values_from_triangle (vector < vector <GRID_CENTER> > rect_grid, vector <GRID_CENTER> tri_center);

double return_rect_grid_max_count (vector < vector <GRID_CENTER > > rect_grid);

vector < vector <GRID_CENTER> > normalize_grid_cell_values (vector < vector <GRID_CENTER> > rect_grid, vector <GRID_CENTER> tri_center);




size_t return_contour_step (double max_COUNT);

size_t return_isoline (double max_COUNT, size_t k);

vector < vector <GRID_CENTER_S> > generate_empty_binary_rect_grid (size_t cell_number);

vector < vector <GRID_CENTER_S> > generate_binary_rect_grid (vector < vector <GRID_CENTER> > rect_grid, size_t COUNT);

vector < vector <GRID_CENTER_S> > generate_marching_squares (vector < vector <GRID_CENTER_S> > bin_grid);

vector < vector <GRID_CENTER_S> > check_saddle (vector < vector <GRID_CENTER_S> > m_sq, vector < vector <GRID_CENTER> > bin_grid, size_t isoline);




vector <LINE> generate_raw_lines (vector <vector <GRID_CENTER_S> > m_sq, vector <vector <GRID_CENTER> > rect_grid, size_t isoline);

vector <LINE> return_line_from_m_sq_number (vector <vector <GRID_CENTER_S> > m_sq, vector <vector <GRID_CENTER> > rect_grid, size_t j, size_t i, size_t isoline);

vector <vector <LINE> > generate_line_vector (vector <LINE> in);

vector <LINE> turn_off_record (vector <LINE> in);

vector <LINE> flip_line (vector <LINE> L);

vector <vector <LINE> > connect_vectors(vector <vector <LINE> > LV);

vector <vector <LINE> > tidy_vectors(vector <vector <LINE> > LV);




vector < vector <VCTR> > generate_bezier_points (vector <vector <LINE> > LV);

bool is_point_in_circle (VCTR in);

bool is_circle_border (VCTR A, VCTR B);

vector < vector <VCTR> > fit_to_circle_I (vector < vector <VCTR> > inBZ);

VCTR interpolate_between_points (VCTR A, VCTR B, bool linestart);

VCTR generate_new_start (VCTR A);

bool is_closed_line (VCTR min_A, VCTR max_B);

vector <VCTR> flip_BZ_line (vector <VCTR> BZ);

vector <VCTR> turn_off_BZ_record (vector <VCTR> in);

vector < vector <VCTR> > fit_to_circle_II (vector < vector <VCTR> > inBZ);

vector < vector <VCTR> > shorten_bezier (vector < vector <VCTR> > inBZ);

vector < vector <VCTR> > generate_final_bezier (vector < vector <VCTR> > inBZ);

vector < vector <VCTR> > eliminate_empty_records (vector < vector <VCTR> > inBZ);

vector < vector <VCTR> > eliminate_out_circle_curves (vector < vector <VCTR> > inBZ);










void contouring (vector <GDB> inGDB, INPSET inset);





void dbg_cout_triangle_center (vector <GRID_CENTER> TRI_CENTER);

void dbg_cout_rect_grid (vector < vector <GRID_CENTER> > rect_grid);

void dbg_cout_rect_grid (vector < vector <GRID_CENTER_S> > rect_grid);

void cout_line_to_ps_header (ofstream& o);

void cout_line_to_ps (vector <LINE> l, ofstream& o, size_t isoline, double max_COUNT);

void cout_bezier_to_ps (vector < vector <VCTR> > BZ, ofstream& o, size_t isoline, double max_COUNT);

void dbg_cout_line_vctr (vector <vector <LINE> > LV);

void dbg_bezier_points (vector < vector <VCTR> > BZ);

void cout_rect_grid_to_ps (vector <vector < GRID_CENTER> > rect_grid, ofstream& o, INPSET inset, double max_COUNT);



#endif
