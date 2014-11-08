// Copyright (C) 2012 - 2014 Ágoston Sasvári
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

void check_standard_output_SF (const vector <GDB>& inGDB, const STRESSFIELD& standard);
STRESSFIELD init_standard_GDB_PTN ();
STRESSFIELD init_standard_GDB_ANGELIER ();
STRESSFIELD init_standard_GDB_BINGHAM ();
STRESSFIELD init_standard_GDB_BRUTEFORCE ();
STRESSFIELD init_standard_GDB_FRY ();
STRESSFIELD init_standard_GDB_MICHAEL ();
STRESSFIELD init_standard_GDB_MOSTAFA ();
STRESSFIELD init_standard_GDB_SHAN ();

vector <VALLEY> return_standard_valleygraph ();
void check_standard_output_valley (const vector <VALLEY> V_IN, const vector <VALLEY> V_ST);

vector <HISTOGRAM> return_standard_histogram ();
void check_standard_output_histogram (const vector <HISTOGRAM> H_IN, const vector <HISTOGRAM> H_ST);

vector <vector <double> > return_standard_distance_matrix ();
void check_standard_distance_matrix (vector <vector <double> > IN, vector <vector <double> > ST);

vector <size_t> return_standard_whichgroup ();
void check_standard_whichgroup (vector <size_t> IN, vector <size_t> ST);

void debug_TRI_GRID (const vector <GDB>& inGDB, const vector <TRIANGLE>& IN);
void check_standard_TRI_GRID (const vector <TRIANGLE>& IN, const vector <TRIANGLE>& ST);
vector <TRIANGLE> return_standard_TRI_GRID ();

void debug_RECT_GRID (const vector <GDB>& inGDB, const vector < vector <GRID_CENTER> >& IN);
void check_standard_RECT_GRID (const vector < vector <GRID_CENTER> >& IN, const vector < vector <GRID_CENTER> >& ST);
vector < vector <GRID_CENTER> > return_standard_RECT_GRID ();

void check_standard_output_striae (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB);
vector <GDB> init_standard_GDB_striae ();

void check_standard_output_fracture (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB);
vector <GDB> init_standard_GDB_fracture ();

void check_standard_output_lithology (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB);
vector <GDB> init_standard_GDB_lithology ();

void check_standard_output_SC (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB);
vector <GDB> init_standard_GDB_SC ();

void check_standard_output_lineation (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB);
vector <GDB> init_standard_GDB_lineation ();

void check_standard_RETILT (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB);
vector <GDB> init_standard_RETILT ();

vector <GDB> init_standard_AVERAGE ();

void check_standard_FOLDSURFACE (const vector <GDB>& nGDB);

void check_standard_ROSE (const vector <ROSENUMBER>& in, const vector <ROSENUMBER>& ST);
vector <ROSENUMBER> generate_standard_ROSE ();

void check_standard_COLORS (const vector <GDB>& inGDB);

void compare_GDB (const GDB& nGDB, const GDB& ST_nGDB);

void INIT_DEBUG ();
void STANDARD_OUTPUT (const vector <GDB>& nGDB, const vector <GDB>& tGDB);


#endif



