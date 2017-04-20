// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef STANDARD_OUTPUT_HPP_
#define STANDARD_OUTPUT_HPP_

bool is_CHK_LITHOLOGY_DT ();
bool is_CHK_LINEATION_DT ();
bool is_CHK_PLANE_DT ();
bool is_CHK_STRIAE_DT ();
bool is_CHK_SC_DT ();

bool is_CHK_ANGELIER ();
bool is_CHK_BINGHAM ();
bool is_CHK_BRUTEFORCE ();
bool is_CHK_FRY ();
bool is_CHK_MICHAEL ();
bool is_CHK_MOSTAFA ();
bool is_CHK_SPRANG ();
bool is_CHK_SHAN ();
bool is_CHK_TURNER ();

bool is_CHK_K_MEANS ();
bool is_CHK_RUP_ANG ();

bool is_CHK_CONTOURING ();
bool is_CHK_RETILT ();
bool is_CHK_FOLDSURFACE ();
bool is_CHK_ROSE ();
bool is_CHK_AVERAGE ();
bool is_CHK_COLORS ();

bool is_CHK_PLOT_LINEATION ();
bool is_CHK_PLOT_PLANE ();
bool is_CHK_PLOT_STRIAE ();

bool is_CHK_WELL ();

void check_standard_output_SF (const vector <GDB>& inGDB, const STRESSFIELD& standard);

void INIT_DEBUG ();
void STANDARD_OUTPUT (const vector <GDB>& nGDB);

void dump_RGF_SF_to_file (const vector <GDB>& inGDB, const string FN);
void dump_RGF_to_file (const vector <GDB>& inGDB, const string FN);
void dump_SF_to_file (const vector <STRESSFIELD>& SF, const string FN);
void dump_RECT_GRID_to_file (const vector <vector <GRID_CENTER> >& IN, const string FN);
void dump_TRI_GRID_to_file (const vector <TRIANGLE>& T, const string FN);

void dump_DISTANCE_MATRIX_to_file (const vector <vector <double> >& DM, const string FN);
void dump_WHICH_GROUP_to_file (const vector <size_t>& WG, const string FN);
void dump_HISTOGRAM_to_file (const vector <HISTOGRAM>& H, const string FN);
void dump_VALLEY_to_file (const vector <VALLEY>& V, const string FN);

void standard_output_rose (const vector <ROSENUMBER>& N, const bool TILT);
void dump_ROSENUMBER_to_file (const vector <ROSENUMBER>& R, const string FN);

void dump_INTERVAL_to_file (ofstream& o);
void standard_output_INTERVAL ();
void dump_FREQUENCY_to_file (ofstream& o);
void standard_output_FREQUENCY ();
void STANDARD_OUTPUT_WELL_GROUPS ();

void dump_CURVE_to_file (const vector <double>& DEPTH, const vector <double>& VALUE, ofstream& o);
void STANDARD_OUTPUT_WELL_PS (const vector <double>& DEPTH, const vector <double>& VALUE, const bool DIPDIR, const string TYPE);

void dump_PEAK_to_file (ofstream& o, const bool PEAK);
void STANDARD_OUTPUT_PEAKS (const string METHOD);
void STANDARD_OUTPUT_FAULTS (const string METHOD);

void dump_INTMINMAX_to_file (const size_t RUN, const size_t SIZE, const double MIN_DEPTH, const double MAX_DEPTH);
void dump_STDEV_to_file (const size_t& RUN, const bool DIPDIR, const vector <string> ID_v, const vector <DIPDIR_DIP>& D1_v, const vector <DIPDIR_DIP>& D2_v, const vector <VCTR>& T1_v, const vector <VCTR>& T2_v, const vector <double>& ANG_v, const vector <double>& MSFT_v);
void dump_FREQ_PRM_to_file (const vector <string>& ID_v, const vector <double>& INT_v, const vector <double>& DIP_v, const vector <double>& FREQ_v, const vector <double>& FREQ_v2, const vector <double>& MAX_FREQ_v);
#endif
