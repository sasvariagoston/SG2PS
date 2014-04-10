// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef RGF_HPP_
#define RGF_HPP_

#include "inversion.h"
#include "cluster.h"
#include "common.h"

vector <GDB> produce_OFFSET (vector <GDB> inGDB);

vector <GDB> competeRGFcontect (string projectname, string inputxyfilename, INPSET inSET);

double right_hand_rule_to_german (double corrDIPDIR);
double german_to_right_hand_rule (double corrDIPDIR);

string cGc_datagroup (string DATATYPE);

vector <GDB> cGc_NDS (vector <GDB> inGDB);

GDB cGc_NCDCSC_LINEATION_SC (GDB inGDB);
GDB cGc_NCDCSC_PITCH (GDB inGDB);
vector <GDB> manipulate_N (vector <GDB> inGDB);

vector <GDB> cGc_NDS_DCNCSC (vector <GDB> inGDB);
vector <GDB> cGc_PITCHANGLE (vector <GDB> inGDB);

vector <GDB> cGc_MISFIT (vector <GDB> inGDB);

vector <GDB> cGc_UP (vector <GDB> inGDB);
vector <GDB> cGc_tilted_UP (vector <GDB> inGDB);

vector <GDB> cGc_OFFSET (vector <GDB> inGDB);
vector <GDB> cGc_LAMBDA_STRESSVECTOR_ESTIMATORS (vector <GDB> inGDB);

CORRECTSTRIAE cGc_correct_striae_DIPcor (GDB inGDB);
CORRECTSTRIAE cGc_correct_striae_DIPDIRcor (GDB inGDB);
vector <GDB> cGc_striae_correction (vector <GDB> inGDB);

bool byLocType(const GDB& x, const GDB& y);
bool byLocTypeGc(const GDB& x, const GDB& y);
bool byiID(const GDB& x, const GDB& y);

bool bycorrDIPDIRcorrDIP(const GDB& x, const GDB& y);
bool bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP(const GDB& x, const GDB& y);

bool byeigenvalue(const sort_jacobi& x, const sort_jacobi& y);
vector <GDB> sort_by_iID (vector <GDB> inGDB);

bool stopcriteria (string prevDATATYPE, string DATATYPE, string prevLOC, string LOC, string prevGC, string GC, INPSET inset);
bool stopcriteria (string prevDATATYPE, string DATATYPE, string prevLOC, string LOC);

size_t minimum_independent_dataset (const vector <GDB>& inGDB, const INPSET& inset);

vector <GDB> return_GDB_with_no_homogeneous_data (vector <GDB> inGDB);

bool correct_inhomogeneous_number (vector <GDB> inGDB, INPSET inset);
bool check_dataset_geometry_homogenity (vector <GDB> inGDB);
bool check_dataset_offset_homogenity (vector <GDB> inGDB);
bool check_dataset_homogenity (vector <GDB> inGDB);

void fold_from_planes (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);

vector <GDB> ptn (vector <GDB> inGDB, INPSET inset);

vector <GDB> clustering_GBD (INPSET inset, vector <GDB> inGDB);

void process_rgf (string inputfilename, string XY_filename, INPSET inset, bool is_debug);

void dbg_vctr (vector <GDB> inGDB, string to_dump);

void dbg_cout_GDB_vector (const vector <GDB>& inGDB);

#endif
