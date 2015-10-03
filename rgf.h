
// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef RGF_HPP_
#define RGF_HPP_

#include "inversion.h"
#include "cluster.h"
#include "common.h"

bool is_PROCESS_AS_TILTED ();
bool is_PROCESS_AS_TRAJECTORY ();

string return_inputfilename ();
void set_inputfilename (const string filename);

vector <GDB> competeRGFcontect (const string projectname);

double right_hand_rule_to_german (const double corrDIPDIR);
double german_to_right_hand_rule (const double corrDIPDIR);

vector <GDB> fix_360_0 (const vector <GDB>& inGDB);
vector <GDB> generate_NDS_vectors (const vector <GDB>& inGDB);
vector <GDB> generate_NCDCSC_vectors (const vector <GDB>& inGDB);
GDB generate_NCDCSC_LINEATION_SC (const GDB& inGDB);
GDB generate_NCDCSC_PITCH (const GDB& inGDB);
vector <GDB> manipulate_N (const vector <GDB>& inGDB);
vector <GDB> generate_MISFIT (const vector <GDB>& inGDB);

vector <GDB> striae_correction (const vector <GDB>& inGDB);
VCTR striae_DIP_correction (const GDB& in);
VCTR striae_DIPDIR_correction (const GDB& in);

bool is_D_up (const VCTR& D);
bool is_DC_up (const VCTR& DC);
bool is_N_down (const VCTR& N);
vector <GDB> generate_UP (const vector <GDB>& inGDB);
vector <GDB> generate_PITCHANGLE (const vector <GDB>& inGDB);
string return_new_offset (const double DIP, const double PTC, const bool UP);
void ASSERT_OFFSET (const string OR_OFS, const string NW_OFS);
vector <GDB> generate_OFFSET (const vector <GDB>& inGDB);
vector <GDB> generate_LAMBDA_STRESSVECTOR_ESTIMATORS (const vector <GDB>& inGDB);

vector <vector <GDB> > clustering_GBD (const vector <vector <GDB> >& inGDB_G);

void fold_from_planes (vector <GDB> inGDB, ofstream& o, CENTER center);

vector <GDB>  PREPARE_GDB_FOR_PROCESSING (const vector <GDB>& inGDB, const bool TILT);
vector < vector <GDB> >  PREPARE_GDB_VECTOR_FOR_PROCESSING (const vector < vector <GDB> >& inGDB_G, const bool TILT);

void PROCESS_RGF (const string inputfilename);
vector <vector <GDB> > EVALUATE (const vector <vector <GDB> >& inGDB_G);

void dbg_cout_GDB_vector_vector_structure (const vector < vector <GDB> >& inGDB_G);
vector <size_t> return_GDB_vector_vector_structure (const vector < vector <GDB> >& inGDB_G);

void dbg_cout_GDB_vector_vector (const vector < vector < GDB> >& inGDB_G);
void dbg_cout_GDB_vector (const vector <GDB>& inGDB);

void compare_structures (const vector <double>& IN1, const vector <double>& IN2);


#endif
