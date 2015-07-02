// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef WELL_PS_HPP_
#define WELL_PS_HPP_

#include "structs.h"

vector <PEAK_TO_PLOT> return_PEAK ();
vector <PEAK_TO_PLOT> return_FAULTS ();

void PS_well_header (const string DATATYPE, const string LOC, ofstream& o);
void PS_well_border (const vector <GDB>& inGDB, ofstream& o, const PAPER& P, const bool TILT, const bool TRJ);

string generate_colorstep (const VCTR clr1, const VCTR clr2, const VCTR clr3, const size_t percent);
string generate_peak_colors (const size_t percent);
string generate_error_colors (const size_t percent);
string generate_binsize_colors (const size_t percent);

void PS_rectangle (ofstream& o, const PAPER& P, const double X1, const double Y1, const double X2, const double Y2);
void PS_rainbow (ofstream& o, const PAPER& P, const double X1, const double Y1, const double X2, const double Y2, const string MODE);
void PS_well_symbols (ofstream& o, const PAPER& P);

double well_axes_step (const double MIN, const double MAX);

bool has_GDB_DEPTH_value_in_range (const vector <GDB>& inGDB, const double MIN, const double MAX);

double return_MIN_value (const vector <GDB>& inGDB, const size_t STEP);
double return_MAX_value (const vector <GDB>& inGDB, const size_t STEP);

void ps_well_formation_tops (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);

void PS_well_coordinate_axes_INTERVAL (ofstream& o, const PAPER& P, const double X, const double LENGTH);
void PS_well_coordinate_grid_INTERVAL (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);
void PS_well_coordinate_axes_DIPDIR (ofstream& o, const PAPER& P, const double X, const double LENGTH);
void PS_well_coordinate_grid_DIPDIR (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);
void PS_well_coordinate_axes_DIP (ofstream& o, const PAPER& P, const double X, const double LENGTH);
void PS_well_coordinate_grid_DIP (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);

void PS_well_coordinate_axes_FAULTS (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);
void PS_well_coordinate_axes_FREQUENCY (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);
void PS_derivate_DIPDIR_DIP (ofstream& o, const PAPER& P, const double X);

void SETUP_PEAK (const vector <double>& DEPTH, const vector <double>& VALUE);

double RETURN_VALUE_AVERAGE (vector <PEAK_TO_PLOT> IN);
double RETURN_DEPTH_AVERAGE (vector <PEAK_TO_PLOT> IN);

vector <PEAK_TO_PLOT> generate_flattened_curve (const size_t bin);

void count_real_peaks (const vector <PEAK_TO_PLOT>& FL_PK, const string METHOD);
void rescale_peaks ();
void associate_peaks_to_faults ();
void PEAK_IDENTIFICATION (const vector <double>& DEPTH, const vector <double>& VALUE, const string METHOD);

void plot_well_faults (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);
void plot_well_frequency_derivate (const vector <WELL_FREQUENCY> IN, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);
void plot_well_frequency (const vector <WELL_FREQUENCY> IN, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);

vector <XY> cutting_points (const vector <XY>& IN);
vector <XY> generate_xy_vector (const vector <double>& VALUE, const vector <double>& DEPTH, const bool DIPDIR);
vector <vector <XY> >  generate_xy_vector_vector (const vector <XY>& IN);
vector <vector <XY> > tidy_xy_vector_vector (vector <vector <XY> >& IN);
vector <double> generate_VALUE_from_XY_vector (const vector <XY>& IN);
vector <double> generate_DEPTH_from_XY_vector (const vector <XY>& IN);

void plot_curve (const vector <double> DEPTH, const vector <double> VALUE, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE);
void plot_peaks (ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE);

double return_plot_value (const WELL_INTERVAL ACT, const bool DIPDIR, const string TYPE);
void return_records_with_formation_names (const vector <vector <GDB> >& inGDB_G);

void plot_well_curve (const vector <WELL_INTERVAL>& IN, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE);
void plot_well_measurements (const vector <GDB>& inGDB, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR);

void PS_well_intervals_error (const vector <WELL_INTERVAL>& INTERVAL, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR);
void PS_well_intervals (const vector <WELL_INTERVAL>& INTERVAL, ofstream& o, const PAPER& P, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL);

void INIT_FAULT_POSITIONS (const double MIN_VAL, const double MAX_VAL, const double STEP);
void SETUP_FAULT_POSITIONS (const double MIN_VAL, const double MAX_VAL, const double STEP);
void WELL_PS (const vector <GDB>& inGDB, const vector <WELL_INTERVAL>& INT, const vector <WELL_FREQUENCY>& FREQ, ofstream& OPS, const PAPER& P, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP);
void OUTPUT_TO_WELL_PS (const vector <vector <GDB> >& GDB_G, const PFN& PF, const bool TILT, const bool TRJ);

#endif /* WELL_PS_HPP_ */
