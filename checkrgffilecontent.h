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
bool XYcheck ();
bool DATATYPEcheck ();
bool DIPDIRcheck ( );
bool DIPcheck ();
bool STRIAE_SC_check ();
bool PALEONcheck ();

vector <string> check_rgf_inputs (vector <string> inputfilename_vector);

bool rgffile_correct (string projectname);

bool is_STRIAE (const string DATATYPE);
bool is_SC (const string DATATYPE);
bool is_BEDDING (const string DATATYPE);

bool is_OTHERcorrect (vector <string> in);
bool is_BEDDINGcorrect (vector <string> in);
bool is_SCcorrect (vector <string> in);
bool is_LINEATIONcorrect (vector <string> in);
bool is_PITCHcorrect (vector <string> in);

bool is_allowed_groupcode(const string& groupcode);
bool is_allowed_colorcode(const string& colorcode);
bool is_allowed_coordinate(const string& coordinate);

bool is_allowed_lithology_datatype(const string& datatype);
bool is_allowed_lineation_datatype(const string& datatype);
bool is_allowed_plane_datatype(const string& datatype);
bool is_allowed_striae_datatype(const string& datatype);
bool is_allowed_SC_datatype(const string& datatype);

bool is_allowed_dip(const string& dip);
bool is_allowed_dir(const string& dir);

bool is_allowed_striae_inverse_sense(const string& sense);
bool is_allowed_striae_normal_sense(const string& sense);
bool is_allowed_striae_dextral_sense(const string& sense);
bool is_allowed_striae_sinistral_sense(const string& sense);
bool is_allowed_striae_none_sense(const string& sense);
bool is_allowed_striae_sense(const string& sense);

bool is_allowed_bedding_overturned_sense(const string& sense);
bool is_allowed_bedding_normal_sense(const string& sense);
bool is_allowed_bedding_none_sense(const string& sense);
bool is_allowed_bedding_sense(const string& sense);

vector <GDB> create_GDB_from_rgf (const string& file_name);

bool is_allowed_geodetic (const string& geodetic) ;

bool is_double (const string& coordinate);

bool error_cout (vector <string> bad_records, string recordtype);

#endif
