// Copyright (C) 2012 - 2014 Ágoston Sasvári
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
bool DEPTHcheck ();
bool GCcheck ();
bool COLORcheck ();
bool LOCcheck ();
bool XY_inrgf_check ();
bool DATATYPEcheck ();
bool DIPDIRcheck ( );
bool DIPcheck ();
bool STRIAE_SC_check ();
bool PALEONcheck ();

vector <string> RETURN_CORRECT_RGF_CMD (vector <string> inputfilename_vector);

vector <string> check_rgf_set_inputs (const vector <string>& inputfilename_vector);


bool is_OTHERcorrect (const vector <string>& in);
bool is_BEDDINGcorrect (const vector <string>& in);
bool is_SCcorrect (const vector <string>& in);
bool is_LINEATIONcorrect (const vector <string>& in);
bool is_PITCHcorrect (const vector <string>& in);

vector <GDB> create_GDB_from_rgf (const string& file_name);

bool error_cout (const vector <string>& bad_records, const string recordtype);

//----------------------------------------------------

bool is_RGF_CORRECT (const string projectname);//ok


#endif
