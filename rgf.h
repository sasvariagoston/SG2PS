// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef RGF_HPP_
#define RGF_HPP_

#include "inversion.h"
#include "cluster.h"
#include "common.h"

//vector <GDB> produce_OFFSET (vector <GDB> inGDB);

string return_inputfilename ();
void set_inputfilename (const string filename);

vector <GDB> competeRGFcontect (const string projectname, const string inputxyfilename); //ok

double right_hand_rule_to_german (const double corrDIPDIR); //ok
double german_to_right_hand_rule (const double corrDIPDIR); //ok

//string cGc_datagroup (string DATATYPE);

vector <GDB> generate_NDS_vectors (const vector <GDB>& inGDB); //ok
vector <GDB> generate_NCDCSC_vectors (const vector <GDB>& inGDB); //ok
GDB generate_NCDCSC_LINEATION_SC (const GDB& inGDB); //ok
GDB generate_NCDCSC_PITCH (const GDB& inGDB); //ok
vector <GDB> manipulate_N (const vector <GDB>& inGDB);//ok
vector <GDB> generate_MISFIT (const vector <GDB>& inGDB); //ok

vector <GDB> striae_correction (const vector <GDB>& inGDB); //ok
VCTR striae_DIP_correction (const GDB& in); //ok
VCTR striae_DIPDIR_correction (const GDB& in); //ok

vector <GDB> generate_UP (const vector <GDB>& inGDB); //ok
vector <GDB> generate_PITCHANGLE (const vector <GDB>& inGDB); //ok
vector <GDB> generate_OFFSET (const vector <GDB>& inGDB); //ok
vector <GDB> generate_LAMBDA_STRESSVECTOR_ESTIMATORS (const vector <GDB>& inGDB); //ok
vector <GDB> generate_tilted_UP (const vector <GDB>& inGDB); //ok

vector < vector < vector <vector <GDB> > > > clustering_GBD (const vector < vector < vector <vector <GDB> > > >& inGDB_G); //ok






//vector <GDB> cGc_NDS_DCNCSC (vector <GDB> inGDB);














bool byLocType(const GDB& x, const GDB& y);//ok
bool byLocTypeGc(const GDB& x, const GDB& y);//ok
bool byLocGcType (const GDB& x, const GDB& y); //ok
bool byiID(const GDB& x, const GDB& y);//ok

bool bycorrDIPDIRcorrDIP(const GDB& x, const GDB& y);
bool bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP(const GDB& x, const GDB& y);

bool byeigenvalue(const sort_jacobi& x, const sort_jacobi& y);

vector < vector <GDB> > sort_by_iID (const vector < vector <GDB> >& inGDB_G);
vector <GDB> sort_by_iID (const vector <GDB>& inGDB); //ok

bool stopcriteria (const string pDT, const string DT, const string pL, const string L, const string pGC, const string GC); //ok
bool stopcriteria (const string pDT, const string DT, const string pL, const string L);




void fold_from_planes (vector <GDB> inGDB, ofstream& o, CENTER center);






vector <GDB>  PREPARE_GDB_FOR_PROCESSING (const vector <GDB>& inGDB, const bool TILT); //ok
vector < vector < vector < vector <GDB> > > >  PREPARE_GDB_VECTOR_FOR_PROCESSING (const vector < vector < vector < vector <GDB> > > >& inGDB_G, const bool TILT); //ok

void process_rgf (string inputfilename, string XY_filename, bool is_debug);

void dbg_cout_GDB_v_v_v_v_structure (const vector < vector < vector < vector <GDB> > > >& inGDB_G);
void dbg_cout_GDB_vector_vector (const vector < vector < vector < vector <GDB> > > >& inGDB_G); //ok
void dbg_cout_GDB_vector (const vector <GDB>& inGDB); //ok

#endif
