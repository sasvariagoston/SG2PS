// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef ALLOWED_KEYS_HPP_
#define ALLOWED_KEYS_HPP_

#include "common.h"

using namespace std;

bool is_allowed_groupcode_str (const string& groupcode);
vector <string> allowed_groupcode_str_vector ();
bool is_allowed_groupcode_empty (const string& groupcode);
vector <string> allowed_groupcode_empty_vector ();

bool is_allowed_colorcode_str(const string& colorcode);
vector <string> allowed_colorcode_str_vector  ();
bool is_allowed_colorcode_num(const string& colorcode);
vector <string> allowed_colorcode_num_vector ();
bool is_allowed_colorcode_empty(const string& colorcode);
vector <string> allowed_colorcode_empty_vector ();

bool is_allowed_coordinate(const string& coordinate);

bool is_allowed_lithology_datatype(const string& datatype);

bool is_allowed_lineation_datatype(const string& datatype);
vector <string> allowed_lineation_datatype_vector ();

bool is_allowed_plane_datatype(const string& datatype);
vector <string> allowed_plane_datatype_vector ();

bool is_allowed_striae_datatype(const string& datatype);
vector <string> allowed_striae_datatype_vector ();

bool is_allowed_SC_datatype(const string& datatype);
vector <string> allowed_SC_datatype_vector ();

bool is_allowed_foldsurface_processing(const string& datatype);

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

bool is_allowed_geodetic (const string& geodetic);

bool is_allowed_inversion_method (const string& method);

bool is_allowed_BINGHAM_datatype (const string& datatype);

bool is_allowed_handle_as_bedding (const string& datatype);

bool is_allowed_pscolor_rgb (const string& datatype);
vector <string> allowed_pscolor_rgb_vector ();
bool is_allowed_pscolor_gray (const string& datatype);
vector <string> allowed_pscolor_gray_vector ();
bool is_allowed_dash (const string& datatype);
vector <string> allowed_dash_vector ();

bool is_double (const string& coordinate);

vector <string> merge_datatypes (vector <string> target, vector <string> source);

#endif
