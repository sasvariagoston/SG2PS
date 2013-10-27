// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef CHECKRGFFILECONTENT_HPP_
#define CHECKRGFFILECONTENT_HPP_

#include "common.h"

using namespace std;

string inputfilename ();

void push_to_table(const string& line);
bool input_rgf (const string& projectname);

void complete_rgf_to_check();

bool IDcheck_duplicate ();
bool IDcheck ();
bool GCcheck ();
bool COLORcheck ();
bool LOCcheck ();
bool XY_inrgf_check ();
bool DATATYPEcheck ();
bool DIPDIRcheck ( );
bool DIPcheck ();
bool STRIAE_SC_check ();
bool PALEONcheck ();

vector <string> check_rgf_inputs (vector <string> inputfilename_vector);

bool rgffile_correct (string projectname);

bool is_OTHERcorrect (vector <string> in);
bool is_BEDDINGcorrect (vector <string> in);
bool is_SCcorrect (vector <string> in);
bool is_LINEATIONcorrect (vector <string> in);
bool is_PITCHcorrect (vector <string> in);

vector <GDB> create_GDB_from_rgf (const string& file_name);

bool error_cout (vector <string> bad_records, string recordtype);

#endif
