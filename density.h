// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef DENSITY_HPP_
#define DENSITY_HPP_


#include <algorithm>
#include <vector>

#include "structs.h"

vector < vector <GRID_CENTER> > generate_rectangular_grid_from_triange_center (const size_t cell_number);

double return_grid_size (const vector < vector <GRID_CENTER> >& rect_grid);

vector < vector <GRID_CENTER> > calculate_grid_cell_values_from_triangle (vector < vector <GRID_CENTER> >& rect_grid, const vector <GRID_CENTER>& tri_center);

size_t return_RECT_GRID_max_count (const vector < vector <GRID_CENTER> >& RECT_GRID);

vector <double> return_isoline_percentages (const double C_MN, const double C_MX);

vector < vector <GRID_CENTER> > generate_empty_binary_rect_grid (const size_t cell_number);

vector < vector <GRID_CENTER> > generate_binary_rect_grid (const vector < vector <GRID_CENTER> >& rect_grid, const size_t COUNT);

vector < vector <GRID_CENTER> > generate_marching_squares (const vector < vector <GRID_CENTER> >& bin_grid);

vector < vector <GRID_CENTER> > check_saddle (vector < vector <GRID_CENTER> >& m_sq, const vector < vector <GRID_CENTER> >& bin_grid, const double isoline);

vector <LINE> manage_points_outside (const vector <LINE>& L);

vector <LINE> generate_raw_lines (const vector <vector <GRID_CENTER> >& m_sq, const vector <vector <GRID_CENTER> >& rect_grid, const double isoline);

vector <LINE> return_line_from_m_sq_number (const vector <vector <GRID_CENTER> >& m_sq, const vector <vector <GRID_CENTER> >& rect_grid, const size_t j, const size_t i, const size_t isoline);

vector <vector <LINE> > generate_line_vector (const vector <LINE>& in);

vector <LINE> turn_off_record (vector <LINE>& in);

vector <LINE> flip_line (const vector <LINE>& L);

vector <vector <LINE> > connect_vectors(vector <vector <LINE> >& LV);

vector <vector <LINE> > eliminate_empty_short_records (vector <vector <LINE> >& LV);

vector < vector <XY> > eliminate_short_distances (const vector < vector <XY> >& BZ);

vector < vector <XY> > generate_bezier_points (const vector <vector <LINE> >& LV);

bool is_point_in_circle (const XY& in);

XY interpolate_between_points (const XY& A, const XY& B);

XY generate_new_start (const XY& A);

bool is_closed_line (const XY& min_A, const XY& max_B);

vector <XY> flip_BZ_line (const vector <XY>& BZ);

vector < vector <XY> > extrapolate_to_circle (const vector < vector <XY> >& inBZ);

bool is_line_CCW (const vector <XY>& I);

vector <XY> flip_line (const vector <XY>& I);

vector <XY> flip_curve_to_CCW (const vector <XY>& inBZ);

vector <BEZIER> generate_final_bezier (const vector < vector <XY> >& inBZ);

double return_t (const XY& C, const XY& B, const XY& P);

double return_distance_from_side (const XY& C, const XY& B, const XY& P);

double return_distance_from_point (const XY& A, const XY& P);

double return_triangle_area (const XY& A, const XY& B, const XY& C);

bool is_point_inside_curve (const vector <XY>& I, const GRID_CENTER& GP);

bool curve_contains_peak (const vector <XY>& I, const vector < vector <GRID_CENTER> >& RECT_GRID, const double& CONTOUR);

double polygon_area (const vector <XY>& I); //OK

bool is_line_close_unitcircle (const vector <XY>& I, const double CELL);

vector <XY> close_contourline (const vector <XY>& I, const double START_ANGLE, const double END_ANGLE, const double CELL, const bool CHECK_DISTANCE);

void contourline_to_ps (ofstream& o, const CENTER& center, const vector <XY>& BZ, const double& FRST_ANGLE, const double& LAST_ANGLE, const double& CONTOUR, const double& C_MN, const double& C_MX);

void output_contourline (ofstream& o, const CENTER& center, vector <vector <XY> >& BZ,  vector < vector <GRID_CENTER> >& RECT_GRID, const double& CONTOUR, const double C_MN, const double C_MX, const double& MAX, const bool is_debug);

bool is_processable_for_contouring (const vector <GDB>& inGDB);

void CONTOURING (const vector <GDB>& inGDB, ofstream& o, const CENTER center, const bool TILT);

void dbg_cout_NET (const vector <vector <vector <XY> > >& NET);
void dbg_cout_TRI_GRID (const vector <TRIANGLE>& TRI_GRID);
void dbg_cout_CONTOURS (const vector <size_t>& CNTR_AT);
void dbg_cout_triangle_center (const vector <GRID_CENTER>& TRI_CENTER);
void dbg_cout_rect_grid (const vector < vector <GRID_CENTER> >& rect_grid, const bool display_coordinates);
void cout_bezier_to_ps (const vector < vector <BEZIER> >& B, ofstream& o, const size_t& isoline, const double& max_COUNT);
void dbg_cout_line (const vector <LINE>& L);
void dbg_cout_line_vctr (const vector <vector <LINE> >& LV);
void dbg_bezier_points (const vector <XY>& BZ);
void dbg_bezier_curve (const vector <BEZIER>& B);
void cout_rect_grid_to_ps (const vector <vector < GRID_CENTER> >& rect_grid, ofstream& o);

#endif
