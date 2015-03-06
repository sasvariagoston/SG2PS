
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>

#include "allowed_keys.hpp"
#include "array_to_vector.hpp"

template <typename T>
bool contains(const vector<T>& vec, const T& elem) {

	return find(vec.begin(), vec.end(), elem) != vec.end();
}

namespace {
const string groupcode_basic_allowed_str [] = {
		"X", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J"
};
const vector<string> allowed_basic_groupcodes_str(from_array(groupcode_basic_allowed_str));

const string groupcode_allowed_empty [] = {
		""
};
const vector<string> allowed_groupcodes_empty(from_array(groupcode_allowed_empty));

const string colorcode_allowed_str [] = {
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"
};
const vector<string> allowed_colorcodes_str(from_array(colorcode_allowed_str));

const string colorcode_allowed_num [] = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};
const vector<string> allowed_colorcodes_num(from_array(colorcode_allowed_num));

const string colorcode_allowed_empty [] = {
		""
};
const vector<string> allowed_colorcodes_empty(from_array(colorcode_allowed_empty));

const string datatype_lithology_allowed [] = {
		"LITHOLOGY"
};
const vector<string> allowed_lithology_datatypes(from_array(datatype_lithology_allowed));

const string datatype_lineation_allowed [] = {
		"BOUDAIN",
		"FOLDAXIS",
		"KINK",
		"LINEATION",
		"USERLINEATION1",
		"USERLINEATION2",
		"USERLINEATION3",
		"USERLINEATION4",
		"USERLINEATION5"
};
const vector<string> allowed_lineation_datatypes(from_array(datatype_lineation_allowed));

const string datatype_plane_allowed [] = {
		"BEDDING",
		"BOREHOLE BREAK-OUT"
		"CONTACT",
		"CROSSBEDDING",
		"FAULT",
		"FOLDPLANE",
		"FOLDSURFACE",
		"FRACTURE",
		"LITHOCLASE",
		"PLANE",
		"S1",
		"S2",
		"S3",
		"S4",
		"S5",
		"STRESS-INDUCED FRACTURE"
		"USERPLANE1",
		"USERPLANE2",
		"USERPLANE3",
		"USERPLANE4",
		"USERPLANE5",
		"VEIN"
};
const vector<string> allowed_plane_datatypes(from_array(datatype_plane_allowed));


const string datatype_striae_allowed [] = {
		"STRIAE"
};
const vector<string> allowed_striae_datatypes(from_array(datatype_striae_allowed));

const string datatype_SC_allowed [] = {
		"SC"
};
const vector<string> allowed_SC_datatypes(from_array(datatype_SC_allowed));

const string foldsurface_processing_allowed [] = {
		"FOLDSURFACE"
};
const vector<string> allowed_foldsurface_processing(from_array(foldsurface_processing_allowed));

const string striae_inverse_sense_allowed [] = {
		"+", "THRUST", "UP", "INVERSE", "U", "I"
};
const vector<string> allowed_striae_inverse_senses(from_array(striae_inverse_sense_allowed));

const string striae_normal_sense_allowed [] = {
		"-", "NORMAL", "FAULT", "DOWN", "DOWNWARD", "N"
};
const vector<string> allowed_striae_normal_senses(from_array(striae_normal_sense_allowed));

const string striae_dextral_sense_allowed [] = {
		"DEXTRAL", "DX", "D"
};
const vector<string> allowed_striae_dextral_senses(from_array(striae_dextral_sense_allowed));

const string striae_sinistral_sense_allowed [] = {
		"SINISTRAL", "SN", "S"
};
const vector<string> allowed_striae_sinistral_senses(from_array(striae_sinistral_sense_allowed));


const string striae_none_sense_allowed [] = {
		"X", "NONE"
};
const vector<string> allowed_striae_none_senses(from_array(striae_none_sense_allowed));

const string bedding_overturned_sense_allowed [] = {
		"O", "OVERTURNED"
};
const vector<string> allowed_bedding_overturned_senses(from_array(bedding_overturned_sense_allowed));

const string bedding_normal_sense_allowed [] = {
		"N", "NORMAL"
};
const vector<string> allowed_bedding_normal_senses(from_array(bedding_normal_sense_allowed));

const string bedding_none_sense_allowed [] = {
		""
};
const vector<string> allowed_bedding_none_senses(from_array(bedding_none_sense_allowed));

const string geodetic_allowed [] = {
		"N", "NNE", "NE", "ENE",
		"E", "ESE", "SE", "SSE",
		"S", "SSW", "SW", "WSW",
		"W", "WNW", "NW", "NNW"
};
const vector<string> allowed_geodetics(from_array(geodetic_allowed));

const string inversion_method_allowed [] = {
		"ANGELIER",
		"BINGHAM",
		"BRUTEFORCE",
		"FRY",
		"MICHAEL",
		"MOSTAFA",
		"NDA",
		"PTN",
		"SHAN",
		"NONE"
};
const vector<string> allowed_inversion_methods(from_array(inversion_method_allowed));

const string bingham_datatype_allowed [] = {
		"FRACTURE"
};
const vector<string> allowed_bingham_datatype(from_array(bingham_datatype_allowed));

const string handle_as_bedding_allowed [] = {
		"BEDDING"
};
const vector<string> allowed_handle_as_bedding (from_array(handle_as_bedding_allowed));





const string gui_mode_allowed [] = {
		"-G",
		"--G",
		"-GUI_CALLS",
		"--GUI_CALLS"
};
const vector<string> allowed_gui_mode(from_array(gui_mode_allowed));

const string debug_mode_allowed [] = {
		"-D",
		"--D",
		"-DEBUG",
		"--DEBUG"
};
const vector<string> allowed_debug_mode(from_array(debug_mode_allowed));

const string version_mode_allowed [] = {
		"-V",
		"--V",
		"-VERSION",
		"--VERSION"
};
const vector<string> allowed_version_mode(from_array(version_mode_allowed));

const string version_id_mode_allowed [] = {
		"-ID",
		"--ID",
		"-VID",
		"--VID",
		"-VERSION-ID",
		"--VERSION-ID",
		"-VERSION_ID",
		"--VERSION_ID"
};
const vector<string> allowed_version_id_mode(from_array(version_id_mode_allowed));

const string test_data_generation_mode_allowed [] = {
		"-T",
		"--T",
		"-TEST",
		"--TEST",
};
const vector<string> allowed_test_data_generation_mode(from_array(test_data_generation_mode_allowed));








}

const string pscolor_rgb_allowed [] = {

		"0.00 0.00 0.00",
		"0.00 0.00 0.00",
		"0.00 0.00 1.00",
		"1.00 0.00 0.67",
		"1.00 0.00 0.00",
		"1.00 0.50 0.00",
		"1.00 1.00 0.00",
		"0.00 1.00 0.00",
		"0.67 0.00 0.67",
		"0.50 1.00 1.00",
		"0.50 0.50 0.50"
};
const vector<string> allowed_pscolor_rgb(from_array(pscolor_rgb_allowed));

const string pscolor_gray_allowed [] = {

		"0.00 0.00 0.00",
		"0.00 0.00 0.00",
		"0.20 0.20 0.20",
		"0.20 0.20 0.20",
		"0.20 0.20 0.20",
		"0.40 0.40 0.40",
		"0.40 0.40 0.40",
		"0.40 0.40 0.40",
		"0.60 0.60 0.60",
		"0.60 0.60 0.60",
		"0.60 0.60 0.60"
};
const vector<string> allowed_pscolor_gray(from_array(pscolor_gray_allowed));

const string dash_allowed [] = {

		"   ",
		"   ",
		"   ",
		"3 3",
		"6 6",
		"   ",
		"3 3",
		"6 6",
		"   ",
		"3 3",
		"6 6"
};
const vector<string> allowed_dash(from_array(dash_allowed));

bool is_allowed_basic_groupcode_str(const string& groupcode) {

	return contains(allowed_basic_groupcodes_str, groupcode);
}

vector <string> allowed_basic_groupcode_str_vector () {

	return allowed_basic_groupcodes_str;
}

bool is_allowed_groupcode_empty(const string& groupcode) {

	return contains(allowed_groupcodes_empty, groupcode);
}

vector <string> allowed_groupcode_empty_vector () {

	return allowed_groupcodes_empty;
}

bool is_allowed_groupcode (const string& groupcode) {

	size_t L = groupcode.size();

	if (L == 1) {

		const string GC = char_to_string(groupcode.at(0));
		return is_allowed_basic_groupcode_str (GC);
	}
	else if (L == 3) {

		const string GC1 = char_to_string(groupcode.at(0));
		const string GC2 = char_to_string(groupcode.at(1));
		const string GC3 = char_to_string(groupcode.at(2));

		if (
				is_allowed_basic_groupcode_str (GC1) &&
				is_allowed_basic_groupcode_str (GC2) &&
				is_allowed_basic_groupcode_str (GC3)) return true;
		return false;
	}
	else return false;
}


bool is_allowed_colorcode_str(const string& colorcode) {

	return contains(allowed_colorcodes_str, colorcode);
}

vector <string> allowed_colorcode_str_vector () {

	return allowed_colorcodes_str;
}

bool is_allowed_colorcode_num(const string& colorcode) {

	return contains(allowed_colorcodes_num, colorcode);
}

vector <string> allowed_colorcode_num_vector () {

	return allowed_colorcodes_num;
}

bool is_allowed_colorcode_empty(const string& colorcode) {

	return contains(allowed_colorcodes_empty, colorcode);
}

vector <string> allowed_colorcode_empty_vector () {

	return allowed_colorcodes_empty;
}

bool is_allowed_coordinate(const string& coordinate){

	return (is_double (coordinate));
}

bool is_allowed_lithology_datatype(const string& datatype) {

	return (contains(allowed_lithology_datatypes, datatype));
}

bool is_allowed_lineation_datatype(const string& datatype) {

	return (contains(allowed_lineation_datatypes, datatype));
}

vector <string> allowed_lineation_datatype_vector () {

	return allowed_lineation_datatypes;
}

bool is_allowed_plane_datatype(const string& datatype) {

	return (contains(allowed_plane_datatypes, datatype));
}

vector <string> allowed_plane_datatype_vector () {

	return allowed_plane_datatypes;
}

bool is_allowed_striae_datatype(const string& datatype) {

	return (contains(allowed_striae_datatypes, datatype));
}

vector <string> allowed_striae_datatype_vector () {

	return allowed_striae_datatypes;
}

bool is_allowed_SC_datatype(const string& datatype) {

	return (contains(allowed_SC_datatypes, datatype));
}

vector <string> allowed_SC_datatype_vector () {

	return allowed_SC_datatypes;
}

bool is_allowed_foldsurface_processing(const string& datatype) {

	return (contains(allowed_foldsurface_processing, datatype));
}

bool is_allowed_dip(const string& s) {

	bool failed = true;

	double value = string_to_double (s, failed);

	return is_allowed_DIP (value) && !failed;
}

bool is_allowed_DIP(const double dip) {

	return is_in_range (0.0, 90.0, dip);
}

bool is_allowed_dir (const string& s){

	bool failed = true;

	double value = string_to_double (s, failed);

	return is_allowed_DIR (value) && !failed;
}

bool is_allowed_DIR (const double dir) {

	return is_in_range (0.0, 360.0, dir);
}

bool is_allowed_striae_inverse_sense(const string& sense) {

	return (contains(allowed_striae_inverse_senses, sense));
}

bool is_allowed_striae_normal_sense(const string& sense) {

	return (contains(allowed_striae_normal_senses, sense));
}

bool is_allowed_striae_dextral_sense(const string& sense) {

	return (contains(allowed_striae_dextral_senses, sense));
}

bool is_allowed_striae_sinistral_sense(const string& sense) {

	return (contains(allowed_striae_sinistral_senses, sense));
}

bool is_allowed_striae_none_sense(const string& sense) {

	return (contains(allowed_striae_none_senses, sense));
}

bool is_allowed_striae_sense(const string& sense) {

	return ((is_allowed_striae_inverse_sense (sense)) ||
			(is_allowed_striae_normal_sense (sense)) ||
			(is_allowed_striae_dextral_sense (sense)) ||
			(is_allowed_striae_sinistral_sense (sense)) ||
			(is_allowed_striae_none_sense (sense)));
}

bool is_allowed_bedding_overturned_sense(const string& sense) {

	return (contains(allowed_bedding_overturned_senses, sense));
}

bool is_allowed_bedding_normal_sense(const string& sense) {

	return (contains(allowed_bedding_normal_senses, sense));
}

bool is_allowed_bedding_none_sense(const string& sense) {

	return (contains(allowed_bedding_none_senses, sense));
}

bool is_allowed_bedding_sense(const string& sense) {

	return ((is_allowed_bedding_overturned_sense (sense)) ||
			(is_allowed_bedding_normal_sense (sense)) ||
			(is_allowed_bedding_none_sense (sense)));
}

bool is_allowed_geodetic (const string& geodetic) {

	return contains(allowed_geodetics, geodetic);
}

bool is_allowed_inversion_method (const string& method) {

	return contains(allowed_inversion_methods, method);
}

bool is_allowed_BINGHAM_datatype (const string& datatype) {

	return contains(allowed_bingham_datatype, datatype);
}

bool is_allowed_handle_as_bedding (const string& datatype) {

	return contains(allowed_handle_as_bedding, datatype);
}

bool is_allowed_pscolor_rgb (const string& datatype) {

	return contains(allowed_pscolor_rgb, datatype);
}

vector <string> allowed_pscolor_rgb_vector () {

	return allowed_pscolor_rgb;
}

bool is_allowed_pscolor_gray (const string& datatype) {

	return contains(allowed_pscolor_gray, datatype);
}

vector <string> allowed_pscolor_gray_vector () {

	return allowed_pscolor_gray;
}

bool is_allowed_dash (const string& datatype) {

	return contains(allowed_dash, datatype);
}

vector <string> allowed_dash_vector () {

	return allowed_dash;
}

bool is_allowed_gui_mode(const string& arg) {

	return contains(allowed_gui_mode, arg);
}
vector <string> allowed_gui_mode_vector () {

	return allowed_gui_mode;
}

bool is_allowed_debug_mode(const string& arg) {

	return contains(allowed_debug_mode, arg);
}
vector <string> allowed_debug_mode_vector () {

	return allowed_debug_mode;
}

bool is_allowed_version_mode(const string& arg) {

	return contains(allowed_version_mode, arg);
}
vector <string> allowed_version_mode_vector () {

	return allowed_version_mode;
}

bool is_allowed_version_id_mode(const string& arg) {

	return contains(allowed_version_id_mode, arg);
}
vector <string> allowed_version_id_mode_vector () {

	return allowed_version_id_mode;
}

bool is_allowed_test_data_generation_mode(const string& arg) {

	return contains(allowed_test_data_generation_mode, arg);
}
vector <string> is_allowed_test_data_generation_mode_vector () {

	return allowed_test_data_generation_mode;
}

bool is_double (const string& s) {

	bool failed = true;

	string_to_double (s, failed);

	return (!failed);
}

vector <string> merge_datatypes (vector <string> target, vector <string> source) {

	for (size_t i = 0; i < source.size(); i++) {

		target.push_back(source.at(i));
	}

	return target;
}
