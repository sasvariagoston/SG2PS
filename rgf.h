// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef RGF_HPP_
#define RGF_HPP_

#include <string>
#include <vector>

#include "structs.h"
#include "data_io.h"
#include "common.h"
#include "cluster.h"
#include "angelier.h"

string GC_from_tempGC (string GCtemp);
double DIPDIR_from_DIPDIRtemp (string DIPDIRtemp, INPSET inset, string DATAGROUP);
double corrDIPDIR_from_DIPDIR (double DIPDIR, INPSET inset, string DATAGROUP);

double DIP_from_DIPtemp (string DIPtemp, string DATAGROUP);
double corrDIP_from_DIP (double DIP, string DATAGROUP);

double LDIR_from_LDIRtemp (string LDIRtemp, string DATAGROUP);
double corrLDIR_from_LDIR (double LDIR, string DATAGROUP);

string produce_LINEATION (string LDIRtemp, string LDIPtemp);
string produce_OFFSET (string SENSEtemp);
vector <GDB> compete_colorcode (vector <GDB> inGDB);
vector <GDB> black_colorcode (vector <GDB> inGDB);
vector <GDB> colorcode_grom_groupcode (vector <GDB> inGDB);

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
bool byeigenvalue(const sort_jacobi& x, const sort_jacobi& y);
vector <GDB> sort_by_iID (vector <GDB> inGDB);

bool stopcriteria (string prevDATATYPE, string DATATYPE, string prevLOC, string LOC, string prevGC, string GC, INPSET inset);
bool stopcriteria (string prevDATATYPE, string DATATYPE, string prevLOC, string LOC);

void fold_from_planes (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, PAPER P);

vector <GDB> cGc_average (vector <GDB> inGDB);
vector <GDB> cGc_s0_average (vector <GDB> inGDB);

GDB plane_tilt (GDB inGDB, bool paleonorth);
GDB lineation_tilt (GDB inGDB, bool paleonorth);
GDB SC_tilt (GDB inGDB, bool paleonorth);
GDB striae_tilt (GDB inGDB, bool paleonorth);
GDB S0_TILT (GDB inGDB, bool paleonorth);
vector <GDB> cGc_RETILT (vector <GDB> inGDB, INPSET inSET);

vector <GDB> ptn (vector <GDB> inGDB, INPSET inset);

vector <GDB> clustering_GBD (INPSET inset, vector <GDB> inGDB);

void process_rgf (string inputfilename, string XY_filename, INPSET inset);

#endif
