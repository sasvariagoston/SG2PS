/*
 * well_ps.hpp
 *
 *  Created on: 25 Nov 2015
 *      Author: sasvariagoston_admin
 */

#ifndef WELL_PS_HPP_
#define WELL_PS_HPP_

void PS_well_header (const string DATATYPE, const string LOC, ofstream& o);
void PS_well_border (const vector <GDB>& inGDB, ofstream& o, const PAPER& P, const bool TILT);

double well_axes_step (const double MIN, const double MAX);

bool has_GDB_DEPTH_value_in_range (const vector <GDB>& inGDB, const double MIN, const double MAX);

double return_MIN_value (const vector <GDB>& inGDB, const size_t STEP);
double return_MAX_value (const vector <GDB>& inGDB, const size_t STEP);

void PS_well_coordinate_axes_INTERVAL (ofstream& o, const PAPER& P, const double X, const double LENGTH);
void PS_well_coordinate_grid_INTERVAL (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);

void PS_well_coordinate_axes_DIPDIR (ofstream& o, const PAPER& P, const double X, const double LENGTH);
void PS_well_coordinate_grid_DIPDIR (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);

void PS_well_coordinate_axes_DIP (ofstream& o, const PAPER& P, const double X, const double LENGTH);
void PS_well_coordinate_grid_DIP (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);

void PS_derivate_DIPDIR_DIP (ofstream& o, const PAPER& P, const double X);

void plot_well_frequency_derivate (const vector <WELL_FREQUENCY> IN, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);
void plot_well_frequency (const vector <WELL_FREQUENCY> IN, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);

vector <XY> cutting_points (const vector <XY>& IN);

vector <XY> generate_xy_vector (const vector <double>& VALUE, const vector <double>& DEPTH);
vector <vector <XY> >  generate_xy_vector_vector (const vector <XY>& IN);
vector <vector <XY> > tidy_xy_vector_vector (vector <vector <XY> >& IN);
vector <double> generate_VALUE_from_XY_vector (const vector <XY>& IN);
vector <double> generate_DEPTH_from_XY_vector (const vector <XY>& IN);

void plot_curve (const vector <double> DEPTH, const vector <double> VALUE, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE);

double return_plot_value (const WELL_INTERVAL ACT, const bool DIPDIR, const string TYPE);

void plot_well_curve (const vector <WELL_INTERVAL>& IN, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE);
void plot_well_measurements (const vector <GDB>& inGDB, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR);

void PS_well_intervals_error (const vector <WELL_INTERVAL>& INTERVAL, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);
void PS_well_intervals (const vector <WELL_INTERVAL>& INTERVAL, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);

void OUTPUT_TO_WELL_PS (const vector <vector <vector <vector <GDB> > > > GDB_G, const PFN P, const bool TILT);

#endif /* WELL_PS_HPP_ */
