// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef CHECKRGFFILECONTENT_HPP_
#define CHECKRGFFILECONTENT_HPP_

#include <string>
#include <algorithm>

#include "structs.h"
#include "common.h"

using namespace std;

template <typename T, size_t n>
std::vector<T> from_array(const T (&array)[n]) {

	return std::vector<T>(array, array+n);
}

template <typename T>
bool contains(const std::vector<T>& vec, const T& elem) {

	return std::find(vec.begin(), vec.end(), elem) != vec.end();
}

string inputfilename ();

void push_to_table(const string& line);
void input_rgf (const string& projectname);

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

vector <string> check_rgf_inputs (vector <string> inputfilename_vector, bool batch);

vector <string> create_inputfilename_vector (int argc, char *argv[]);

bool rgffile_correct (string projectname);

bool is_allowed_groupcode(const string& groupcode);
bool is_allowed_colorcode(const string& colorcode);
bool is_allowed_coordinate(const string& coordinate);
bool is_allowed_datatype(const string& datatype);
bool is_allowed_dip(const string& dip);
bool is_allowed_dir(const string& dir);
bool is_allowed_striae_sense(const string& sense);
bool is_allowed_bedding_sense(const string& sense);
bool is_allowed_geodetic (const string& geodetic) ;

bool is_double (const string& coordinate);

bool error_cout (vector <string> bad_records, string recordtype);

#endif
