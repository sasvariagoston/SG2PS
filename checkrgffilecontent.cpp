// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "checkrgffilecontent.h"
#include "checksettingfilecontent.h"
#include "exceptions.hpp"
#include "read_csv.hpp"
#include "ReservedColumnNames.hpp"
#include "run_mode.h"
#include "settings.hpp"

template <typename T>
bool contains(const vector<T>& vec, const T& elem) {

	return find(vec.begin(), vec.end(), elem) != vec.end();
}

namespace {

vector <vector <string> > orig_table;

vector <vector <string> > rgf_to_check;

enum record_name {
	DATA_ID,
	DEPTH,
	GROUP,
	COLOR,
	LOCATION,
	LOCX,
	LOCY,
	FORMATION,
	DATATYPE,
	DIR,
	DIP,
	LDIR,
	LDIP,
	SENSE,
	PALEONORTH,
	COMMENT,
	SIZE
};

}

void read_in_rgf(const string& file_name); // throws rgf_error if duplicate col names are found

bool input_rgf (const string& projectname) {

	try {

		read_in_rgf(projectname + ".rgf");
	}
	catch (rgf_error& ) {

		return false;
	}

	size_t n_records = rgf_to_check.size();

	if (n_records == 0) {
		// nothing has been read but we don't know why (empty or read failed)
		cout << "    - Cannot process " << capslock(projectname + ".rgf") << " file." << endl;
		return false;
	}
	else {

		if (!is_mode_DEBUG()) cout << "    - Input " << capslock(projectname + ".rgf") << " file read, ";

		if (!is_mode_DEBUG()) cout << n_records << " record(s) imported." << endl;
	}

	return true;
}

static bool by_DEPTH_string(const vector<string>& x, const vector<string>& y) {

    return x.at(DEPTH) < y.at(DEPTH);
}

void complete_rgf_to_check () {

	const bool W = is_WELLDATA_USE();

	for (size_t i = 1; i < rgf_to_check.size(); i++) {

		rgf_to_check.at(i).at(SENSE) = capslock(rgf_to_check.at(i).at(SENSE));
		rgf_to_check.at(i).at(DATATYPE) = capslock(rgf_to_check.at(i).at(DATATYPE));
		rgf_to_check.at(i).at(GROUP) = capslock(rgf_to_check.at(i).at(GROUP));
		rgf_to_check.at(i).at(COLOR) = capslock(rgf_to_check.at(i).at(COLOR));
		rgf_to_check.at(i).at(LOCATION) = capslock(rgf_to_check.at(i).at(LOCATION));
		rgf_to_check.at(i).at(FORMATION) = capslock(rgf_to_check.at(i).at(FORMATION));

		if (rgf_to_check.at(i).at(LOCATION) == "") {

			rgf_to_check.at(i).at(LOCATION) = rgf_to_check.at(i-1).at(LOCATION);
		}

		if (rgf_to_check.at(i).at(LOCX) == "") {

			rgf_to_check.at(i).at(LOCX) = rgf_to_check.at(i-1).at(LOCX);
		}

		if (rgf_to_check.at(i).at(LOCY) == "") {

			rgf_to_check.at(i).at(LOCY) = rgf_to_check.at(i-1).at(LOCY);
		}

		if (rgf_to_check.at(i).at(FORMATION) == "") {

			if (!W) rgf_to_check.at(i).at(FORMATION) = rgf_to_check.at(i-1).at(FORMATION);
		}

		if (rgf_to_check.at(i).at(DATATYPE) == "") {

			rgf_to_check.at(i).at(DATATYPE) = rgf_to_check.at(i-1).at(DATATYPE);
		}
	}
	if (!W) return;

	stable_sort(rgf_to_check.begin(), rgf_to_check.end(), by_DEPTH_string);

	for (size_t i = 1; i < rgf_to_check.size(); i++) {

		if (rgf_to_check.at(i).at(FORMATION) == "") {

			rgf_to_check.at(i).at(FORMATION) = rgf_to_check.at(i-1).at(FORMATION);
		}
	}
	return;
}

bool IDcheck_duplicate () {

	map <string, int> record_to_check;

	bool error = false;

	size_t i = 0;

	for (i = 0; i < rgf_to_check.size(); i++) {

		string ID = rgf_to_check.at(i).at(DATA_ID);

		pair<string, int> ID_and_counter(ID, i);

		pair <map <string, int>::iterator, bool> p = record_to_check.insert(ID_and_counter);

		if (!(p.second)) {

			if (!is_mode_DEBUG()) cout << "    - ERROR: DATA_ID " << ID << " used in line " << i + 1 << " is already used at line " << (*(p.first)).second + 1 << "." <<endl;

			error = true;
		}
	}

	if (error) return false;

	if (!is_mode_DEBUG()) cout << "    - Correct DATA_ID's in all records." << endl;

	return true;
}

bool IDcheck () {

	vector <size_t> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		string ID = rgf_to_check.at(i).at(DATA_ID);

		if (ID == "") bad_records.push_back (i + 1);
	}

	if (bad_records.size() == 0) {

		cout << "    - Existing DATA_ID's in all records." << endl;

		return true;
	}
	else {

		cout <<"    - ERROR: empty DATA_ID(s) in the following record(s):  " << flush;

		for (size_t j = 0; j < bad_records.size() - 1; j++) {

			cout << bad_records.at(j) << ", " << flush;
		}
		cout << bad_records.at(bad_records.size()-1) << "." << endl;

		return false;
	}
}

bool DEPTHcheck () {

	if (is_WELLDATA_NO()) return true;

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		const string DPT = rgf_to_check.at(i).at(DEPTH);

		bool failed = false;

		const double DEPTH = string_to_double (DPT, failed);

		const bool POS = DEPTH >= 0.0;

		if (failed || !POS) bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
	}
	return error_cout (bad_records, "depth code");
}

bool GCcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		string GC = rgf_to_check.at(i).at(GROUP);

		bool GC_STR = is_allowed_groupcode (GC);
		bool GC_EMP = is_allowed_groupcode_empty (GC);

		if (!GC_EMP && !GC_STR) bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
	}
	return error_cout (bad_records, "group code");
}

bool COLORcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		string CLR = rgf_to_check.at(i).at(COLOR);

		bool CLR_NUM = is_allowed_colorcode_num		(CLR);
		bool CLR_STR = is_allowed_colorcode_str		(CLR);
		bool CLR_EMP = is_allowed_colorcode_empty 	(CLR);

		bool CORR_CLR = CLR_NUM || CLR_STR || CLR_EMP;

		if (!CORR_CLR) bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
	}
	return error_cout (bad_records, "color code");
}

bool LOCcheck () {

	bool correct = (rgf_to_check.at(0).at(LOCATION) != "");

	if (!correct) cout <<"    - ERROR: LOCALITY must be indicated at least in the 1st record." << endl;

	return correct;
}

bool XY_inrgf_check () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		string LX = rgf_to_check.at(i).at(LOCX);
		string LY = rgf_to_check.at(i).at(LOCY);

		if ((!is_allowed_coordinate (LX) && (LX != ""))   ||   (!is_allowed_coordinate (LY) && (LY != "")))

			bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
	}

	return error_cout (bad_records, "coordinate");
}

bool DATATYPEcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		if
			(	(!is_allowed_lithology_datatype (rgf_to_check.at(i).at(DATATYPE))) &&
				(!is_allowed_lineation_datatype (rgf_to_check.at(i).at(DATATYPE))) &&
				(!is_allowed_plane_datatype (rgf_to_check.at(i).at(DATATYPE))) &&
				(!is_allowed_striae_datatype (rgf_to_check.at(i).at(DATATYPE))) &&
				(!is_allowed_SC_datatype (rgf_to_check.at(i).at(DATATYPE)))) {

			bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
		}
	}

	return error_cout (bad_records, "datatype");
}

bool DIPDIRcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		if (
				((rgf_to_check.at(i).at(DIR) != "") && (rgf_to_check.at(i).at(DATATYPE) == "LITHOLOGY")) ||
				((rgf_to_check.at(i).at(DIR) == "") && (rgf_to_check.at(i).at(DATATYPE) != "LITHOLOGY")) ||
				((rgf_to_check.at(i).at(DIR) != "") && (!is_allowed_dir (rgf_to_check.at(i).at(DIR))))) {

			bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
		}
	}

	return error_cout (bad_records, "strike/dip direction");
}

bool DIPcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		if (
				((rgf_to_check.at(i).at(DIP) != "") && (rgf_to_check.at(i).at(DATATYPE) == "LITHOLOGY")) ||
				((rgf_to_check.at(i).at(DIP) == "") && (rgf_to_check.at(i).at(DATATYPE) != "LITHOLOGY")) ||
				((rgf_to_check.at(i).at(DIP) != "") && (!is_allowed_dip (rgf_to_check.at(i).at(DIP))))) {

			bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
		}
	}

	return error_cout (bad_records, "dip angle");
}

bool STRIAE_SC_check () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		if (
				(!is_OTHERcorrect (rgf_to_check.at(i))) &&
				(!is_BEDDINGcorrect (rgf_to_check.at(i))) &&
				(!is_SCcorrect (rgf_to_check.at(i))) &&
				(!is_LINEATIONcorrect (rgf_to_check.at(i))) &&
				(!is_PITCHcorrect(rgf_to_check.at(i)))

		) {

			bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
		}
	}

	return error_cout (bad_records, "CS/striae");
}

bool PALEONcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		if (!is_allowed_dir (rgf_to_check.at(i).at(PALEONORTH)) && rgf_to_check.at(i).at(PALEONORTH) != "") bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
	}

	return error_cout (bad_records, "paleo north direction");

}

bool is_RGF_CORRECT (const string projectname) {

	if (!(input_rgf (projectname))) return false;

	complete_rgf_to_check ();

	bool SUCCESS = true;

	if (!IDcheck ()) 			SUCCESS = false;
	if (!IDcheck_duplicate ()) 	SUCCESS = false;
	if (!DEPTHcheck ()) 		SUCCESS = false;
	if (!GCcheck ()) 			SUCCESS = false;
	if (!COLORcheck ()) 		SUCCESS = false;
	if (!LOCcheck ()) 			SUCCESS = false;
	if (!XY_inrgf_check ()) 	SUCCESS = false;
	if (!DATATYPEcheck ()) 		SUCCESS = false;
	if (!DIPDIRcheck ()) 		SUCCESS = false;
	if (!DIPcheck ()) 			SUCCESS = false;
	if (!STRIAE_SC_check ()) 	SUCCESS = false;
	if (!PALEONcheck ()) 		SUCCESS = false;

	return SUCCESS;
}

bool is_OTHERcorrect (const vector <string>& in) {

	return (
			!is_allowed_SC_datatype(in.at(DATATYPE)) &&
			!is_allowed_striae_datatype(in.at(DATATYPE)) &&
			//!(in.at(DATATYPE) == "BEDDING") &&
			!is_allowed_handle_as_bedding (in.at(DATATYPE)) &&
			in.at(LDIR) == "" &&
			in.at(LDIP) == "" &&
			in.at(SENSE) == ""
	);
}

bool is_BEDDINGcorrect (const vector <string>& in) {

	return (
			//(in.at(DATATYPE) == "BEDDING") &&
			is_allowed_handle_as_bedding (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			in.at(LDIR) == "" &&
			in.at(LDIP) == "" &&
			is_allowed_bedding_sense (in.at(SENSE))
	);
}

bool is_SCcorrect (const vector <string>& in) {

	return (
			is_allowed_SC_datatype (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			is_allowed_dir (in.at(LDIR)) &&
			is_allowed_dip (in.at(LDIP)) &&
			in.at(SENSE) == ""
	);
}

bool is_LINEATIONcorrect (const vector <string>& in) {

	return (
			is_allowed_striae_datatype (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			is_allowed_dir (in.at(LDIR)) &&
			is_allowed_dip (in.at(LDIP)) &&
			is_allowed_striae_sense (in.at(SENSE)));
}

bool is_PITCHcorrect (const vector <string>& in) {

	return (
			is_allowed_striae_datatype (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			is_allowed_dip (in.at(LDIR)) &&
			is_allowed_geodetic (in.at(LDIP)) &&
			is_allowed_striae_sense (in.at(SENSE)));
}

bool error_cout (const vector <string>& bad_records, const string recordtype) {

	if (bad_records.size() == 0) {

		if (!is_mode_DEBUG()) cout << "    - Correct " << recordtype << "(s) in all records." << endl;

		return true;
	}

	else {

		if (!is_mode_DEBUG()) cout << "    - ERROR: incorrect " << recordtype << "(s) in the following record(s):  " << flush;

		for (size_t j = 0; j < bad_records.size() - 1; j++) {

			cout << bad_records.at(j) << ", " << flush;
		}

		cout << bad_records.at(bad_records.size()-1) << "." << endl;

		return false;
	}
}

double to_double(const vector<string>& row, record_name column) {

	return string_to_double(row.at(column));
}

vector <GDB> create_GDB_from_rgf () {

	vector <GDB> outGDB;

	//read_in_rgf (file_name); // TODO Can throw rgf_error in console, batch and debug mode

	complete_rgf_to_check ();

	for (size_t i = 0; i < rgf_to_check.size(); ++i) {

		GDB buffer;
		// TODO Should be moved to ctor   -------------------------------------
		buffer.DIPDIR = NaN();
		buffer.DIP = NaN();
		buffer.LDIR = NaN();
		buffer.LDIP = NaN();

		buffer.corr.DIPDIR = NaN();
		buffer.corr.DIP = NaN();
		buffer.corrL.DIPDIR = NaN();
		buffer.corrL.DIP = NaN();
		buffer.DEPTH = NaN();

		buffer.LPITCH = NaN();

		buffer.OFFSET = 		"NONE";
		buffer.LINEATION =		"NONE";
		buffer.LPITCHSENSE = 	"NONE";

		buffer.iID = 		i;
		//---------------------------------------------------------------------
		const vector<string>& row = rgf_to_check.at(i);

		buffer.ID = 		row.at(DATA_ID);
		buffer.GC = 		row.at(GROUP);
		buffer.COLOR = 		row.at(COLOR);
		buffer.LOC = 		row.at(LOCATION);

		if (row.at(LOCX) == "")	buffer.LOCX = 0.0;
		else buffer.LOCX = 	to_double(row, LOCX);

		if (row.at(LOCY) == "")	buffer.LOCY = 0.0;
		else buffer.LOCY = 	to_double(row, LOCY);

		buffer.FORMATION = 	row.at(FORMATION);
		buffer.DATATYPE = 	row.at(DATATYPE);

		if (	(is_allowed_lineation_datatype (row.at(DATATYPE))) ||
				(is_allowed_plane_datatype (row.at(DATATYPE)))) {

			buffer.DIPDIR = 		to_double(row, DIR);
			buffer.DIP = 			to_double(row, DIP);
			buffer.corr.DIPDIR = 	to_double(row, DIR);
			buffer.corr.DIP = 		to_double(row, DIP);
		}

		if (is_PITCHcorrect(row)) buffer.LINEATION = "PITCH";
		if (is_LINEATIONcorrect(row)) buffer.LINEATION = "LINEATION";
		if (is_SCcorrect(row)) buffer.LINEATION = "SC";


		if (	(is_allowed_striae_datatype (row.at(DATATYPE))) ||
				(is_allowed_SC_datatype (row.at(DATATYPE)))) {

			buffer.DIPDIR = 		to_double(row, DIR);
			buffer.DIP = 			to_double(row, DIP);
			buffer.corr.DIPDIR = 	to_double(row, DIR);
			buffer.corr.DIP = 		to_double(row, DIP);


			if (buffer.LINEATION != "PITCH" ) {

				buffer.LDIR = 			to_double(row, LDIR);
				buffer.LDIP = 			to_double(row, LDIP);
				buffer.corrL.DIPDIR =	to_double(row, LDIR);
				buffer.corrL.DIP = 		to_double(row, LDIP);
			}

			else {

				buffer.LPITCH = 		to_double(row, LDIR);
				buffer.LPITCHSENSE = 	row.at(LDIP);
			}
		}

		if (is_allowed_lithology_datatype (row.at(DATATYPE)))	buffer.DATAGROUP = "LITHOLOGY";
		if (is_allowed_plane_datatype (row.at(DATATYPE))) 		buffer.DATAGROUP = "PLANE";
		if (is_allowed_lineation_datatype (row.at(DATATYPE))) 	buffer.DATAGROUP = "LINEATION";
		if (is_allowed_striae_datatype (row.at(DATATYPE))) 		buffer.DATAGROUP = "STRIAE";
		if (is_allowed_SC_datatype (row.at(DATATYPE))) 			buffer.DATAGROUP = "SC";

		if (is_allowed_handle_as_bedding (row.at(DATATYPE))) {

			if (is_allowed_bedding_overturned_sense(row.at(SENSE))) 	buffer.OFFSET = "OVERTURNED";
			if (is_allowed_bedding_normal_sense(row.at(SENSE))) 		buffer.OFFSET = "NORMAL";
		}

		if (is_allowed_striae_datatype(row.at(DATATYPE))) {

			if (is_allowed_striae_inverse_sense(row.at(SENSE)))		buffer.OFFSET = "INVERSE";
			if (is_allowed_striae_normal_sense(row.at(SENSE))) 		buffer.OFFSET = "NORMAL";
			if (is_allowed_striae_dextral_sense(row.at(SENSE))) 	buffer.OFFSET = "DEXTRAL";
			if (is_allowed_striae_sinistral_sense(row.at(SENSE)))	buffer.OFFSET = "SINISTRAL";
		}

		if (is_LINEATIONcorrect (row))	buffer.LINEATION = "LINEATION";
		if (is_PITCHcorrect (row)) {

			buffer.LINEATION = "PITCH";
		}
		if (is_SCcorrect (row)) 		buffer.LINEATION = "SC";

		if (row.at(PALEONORTH) == "") buffer.PALEON = 0.0;
		else buffer.PALEON = to_double(row, PALEONORTH);

		buffer.COMMENT = row.at(COMMENT);

		if (is_WELLDATA_USE()) buffer.DEPTH = string_to_double (row.at(DEPTH));

		outGDB.push_back(buffer);
	}
	ASSERT(outGDB.size()==rgf_to_check.size());

	if (!is_mode_DEBUG()) cout << "  - Geodatabase completed for " << outGDB.size() << " records." << endl; // TODO The original message was lying

	return outGDB;
}

void CHECK_RGF (const string projectname) {

	if (!is_mode_STD() && !is_mode_DEBUG()) ASSERT_DEAD_END();

	const string RFN = capslock (projectname);
	const string rfn = projectname ;

	writeln ("");
	writeln ("==============================");
	writeln ("2) CHECKING OF INPUT DATA FILE");
	writeln ("==============================");
	writeln ("");

	writeln (" - CHECKING " + RFN + ".RGF INPUT DATA FILE");

	if (is_RGF_CORRECT (rfn)) writeln (" - " + RFN + ".RGF INPUT DATA FILE IS CORRECT.");



	else {

		writeln ("");
		writeln (" WARNING, RGF_ERROR: the input " + RFN + ".RGF file structure is incorrect, the file will not be processed.");

		throw rgf_error();
	}
}

//=============================================================================
// Comma-separated values (CSV) file I/O contributed by Ali Baharev

namespace {

vector<size_t> index_map;

const size_t NOT_FOUND = numeric_limits<size_t>::max();

}

template <typename Container, typename T>
size_t find_index(const Container& c, const T& value) { // assumes vector

	typename Container::const_iterator pos = std::find(c.begin(), c.end(), value);

	return (pos == c.end()) ? NOT_FOUND : static_cast<size_t>( distance(c.begin(), pos) );
}

size_t cell_index(size_t i) {

	return (i < index_map.size()) ? index_map.at(i) : NOT_FOUND;
}

template <typename T>
const vector<T> set_difference(vector<T> A, vector<T> B) {

	stable_sort (A.begin(), A.end());

	stable_sort (B.begin(), B.end());

	vector<T> C;

	set_difference(A.begin(), A.end(), B.begin(), B.end(), back_inserter(C));

	return C;
}

void show_col_names(const string& msg, const vector<string>& v) {

	if (is_mode_DEBUG()) return;

	if (v.empty()) {

		return;
	}

	cout << "    - The following columns are " << msg << ":\n        ";

	for (size_t i=0; i<v.size(); ++i) {

		cout << "\"" << v.at(i) << "\"" << ((i==v.size()-1) ? '\n' : '\t');
	}

}

void dump_col_status(const vector<string>& found, const vector<string>& ignored) {

	vector<string> unused = set_difference(reserved_column_names(), found);

	if (!is_mode_DEBUG()) cout << "    - The header (column names) of the data file has been processed.\n";

	show_col_names("reserved but unused", unused);

	show_col_names("ignored", ignored);
}

void show_unused_ignored_columns() {	// Column map: original -> rgf

	const vector<string>& header = orig_table.at(0);

	vector<string> found, ignored;

	for (size_t i=0; i<header.size(); ++i) {

		const string& orig_col_name = header.at(i);

		size_t index = cell_index(i);

		if (index==NOT_FOUND) {

			ignored.push_back(orig_col_name);
		}
		else {

			found.push_back(orig_col_name);
			// user's name == reserved name, case insensitive
			ASSERT_EQ(to_uppercase(orig_col_name), reserved_column_names().at(index));
		}
	}

	dump_col_status(found, ignored);
}

void check_duplicates(const vector<size_t>& dup) {

	if (dup.empty()) {

		return;
	}

	cout << "Error: the following reserved column names were found multiple times:\n";

	for (size_t i=0; i<dup.size(); ++i) {

		cout << reserved_column_names().at(dup.at(i)) << '\n';
	}

	cout << "Exiting..." << endl;

	throw rgf_error();
}

void build_column_map() {

	vector<size_t> duplicates;

	const vector<string>& header = orig_table.at(0);

	index_map.assign(header.size(), NOT_FOUND);

	for (size_t i=0; i<header.size(); ++i) {

		const string& col_name = header.at(i);

		size_t index = find_index(reserved_column_names(), to_uppercase(col_name));

		if (index==NOT_FOUND) {

			// That's OK
		}
		else if (contains(index_map, index)) {

			duplicates.push_back(index);
		}
		else {

			index_map.at(i) = index;
		}
	}

	check_duplicates(duplicates);

	show_unused_ignored_columns();
}

void convert_row(const vector<string>& orig_row) {

	const size_t n_col = reserved_column_names().size();

	vector<string> row(n_col);

	for (size_t i=0; i<orig_row.size(); ++i) {

		size_t index = cell_index(i);

		if (index==NOT_FOUND) {
			// That is OK, ignored column
		}
		else {
			row.at(index) = to_uppercase( orig_row.at(i) );
			//row.at(index) = orig_row.at(i); // useful for testing
		}
	}

	rgf_to_check.push_back(row);
}

void convert_csv_rgf() {

	build_column_map();

	for (size_t i=1; i<orig_table.size(); ++i) {

		convert_row(orig_table.at(i));
	}
}

void csv_to_rgf() {

	rgf_to_check.clear();

	convert_csv_rgf();
}

// Entry point, clear all global mutable state first!
// throws rgf_error if duplicate col names are found
void read_in_rgf(const string& file_name) {

	orig_table.clear();
	index_map.clear();
	rgf_to_check.clear();

	size_t lines_read = read_csv(file_name, orig_table);

	if (lines_read >= 1) { // At least we got a header

		csv_to_rgf();
	}
}

//-----------------------------------------------------------------------------
// The functions below are mainly for debugging

void dump_table(const string& file_name, const vector<vector<string> >& table) {

	ofstream out(file_name.c_str());

	for (size_t i=0; i<table.size(); ++i) {

		const vector<string>& row = table.at(i);

		for (size_t j=0; j<row.size(); ++j) {

			const string& cell = row.at(j);

			out << cell << ((j < row.size()-1)? '\t' : '\n');
		}
	}
}

void dbg_dump_tables() {

	dump_table("orig.txt", orig_table);

	dump_table("conv.txt", rgf_to_check);
}


