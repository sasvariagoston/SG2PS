#ifndef ALLOWED_KEYS_HPP_
#define ALLOWED_KEYS_HPP_

#include "common.h"

using namespace std;

bool is_allowed_groupcode(const string& groupcode);
bool is_allowed_colorcode(const string& colorcode);
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

bool is_double (const string& coordinate);

vector <string> merge_datatypes (vector <string> target, vector <string> source);

#endif
