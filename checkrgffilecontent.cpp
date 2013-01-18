// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iomanip>
#include <limits>
#include <map>

#include "checkrgffilecontent.h"
#include "assertions.hpp"
#include "exceptions.hpp"
#include "read_csv.hpp"
#include "ReservedColumnNames.hpp"
#include "run_mode.h"

using namespace std;

namespace {

vector <vector <string> > orig_table;

vector <vector <string> > rgf_to_check;

const string groupcode_allowed [] = {
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "X",
		""
};

const vector<string> allowed_groupcodes(from_array(groupcode_allowed));

const string colorcode_allowed [] = {
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		""
};

const vector<string> allowed_colorcodes(from_array(colorcode_allowed));

const string datatype_lithology_allowed [] = {
		"LITHOLOGY"
};

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

const string datatype_plane_allowed [] = {
		"BEDDING",
		"CONTACT",
		"CROSSBEDDING",
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
		"USERPLANE1",
		"USERPLANE2",
		"USERPLANE3",
		"USERPLANE4",
		"USERPLANE5",
		"VEIN"
};

const string datatype_striae_allowed [] = {
		"STRIAE"
};

const string datatype_SC_allowed [] = {
		"SC"
};

const vector<string> allowed_lithology_datatypes(from_array(datatype_lithology_allowed));
const vector<string> allowed_lineation_datatypes(from_array(datatype_lineation_allowed));
const vector<string> allowed_plane_datatypes(from_array(datatype_plane_allowed));
const vector<string> allowed_striae_datatypes(from_array(datatype_striae_allowed));
const vector<string> allowed_SC_datatypes(from_array(datatype_SC_allowed));

const string striae_inverse_sense_allowed [] = {
		"+", "THRUST", "UP", "INVERSE", "U", "I"
};

const string striae_normal_sense_allowed [] = {
		"-", "NORMAL", "FAULT", "DOWN", "DOWNWARD", "N"
};

const string striae_dextral_sense_allowed [] = {
		"DEXTRAL", "DX", "D"
};

const string striae_sinistral_sense_allowed [] = {
		"SINISTRAL", "SN", "S"
};

const string striae_none_sense_allowed [] = {
		"X", "NONE"
};

const vector<string> allowed_striae_inverse_senses(from_array(striae_inverse_sense_allowed));
const vector<string> allowed_striae_normal_senses(from_array(striae_normal_sense_allowed));
const vector<string> allowed_striae_dextral_senses(from_array(striae_dextral_sense_allowed));
const vector<string> allowed_striae_sinistral_senses(from_array(striae_sinistral_sense_allowed));
const vector<string> allowed_striae_none_senses(from_array(striae_none_sense_allowed));

const string bedding_overturned_sense_allowed [] = {
		"O", "OVERTURNED"
};

const string bedding_normal_sense_allowed [] = {
		"N", "NORMAL"
};

const string bedding_none_sense_allowed [] = {
		""
};

const vector<string> allowed_bedding_overturned_senses(from_array(bedding_overturned_sense_allowed));
const vector<string> allowed_bedding_normal_senses(from_array(bedding_normal_sense_allowed));
const vector<string> allowed_bedding_none_senses(from_array(bedding_none_sense_allowed));

const string geodetic_allowed [] = {
		"N", "NNE", "NE", "ENE",
		"E", "ESE", "SE", "SSE",
		"S", "SSW", "SW", "WSW",
		"W", "WNW", "NW", "NNW"
};

const vector<string> allowed_geodetics(from_array(geodetic_allowed));

enum record_name {
	DATA_ID,
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

struct record {

	string data_id;
	string group;
	string color;
	string location;
	string locx;
	string locy;
	string formation;
	string datatype;
	double dir;
	double dip;
	double ldir;
	double ldip;
	string sense;
	double paleonorth;
	string comment;
};

}

string inputfilename () {

	string filename;
	string projectname;
	ifstream rgffile;

	do {

		cout << endl << "Enter RGF file (*.rgf) name without extension........:  " << flush;

		cin  >> projectname;

		if ((projectname == "X") || (projectname == "x")) throw exit_requested();

		filename = projectname + ".rgf";

		rgffile.open (filename.c_str());
		
		if (rgffile.is_open())cout << "  - Input " << capslock(projectname)<< ".RGF file opened." << endl << endl;
		else cout << "    - ERROR, cannot open " << capslock(projectname)<< ".RGF geodatabase file. Please try again." << endl << endl;
	}
	
	while (!(rgffile.is_open()));

	rgffile.close();

	return projectname;
}

void read_in_rgf(const string& file_name); // throws rgf_error if duplicate col names are found

bool input_rgf (const string& projectname) {

	try {

		read_in_rgf(projectname+".rgf");
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

		cout << "    - Input " << capslock(projectname + ".rgf") << " file read, ";

		cout << n_records << " record(s) imported." << endl;
	}

	return true;
}

void complete_rgf_to_check () {

	for (size_t i = 1; i < rgf_to_check.size(); i++) {

		if (rgf_to_check.at(i).at(LOCATION) == "") 	rgf_to_check.at(i).at(LOCATION) = 	rgf_to_check.at(i-1).at(LOCATION);
		if (rgf_to_check.at(i).at(LOCX) == "") 		rgf_to_check.at(i).at(LOCX) = 		rgf_to_check.at(i-1).at(LOCX);
		if (rgf_to_check.at(i).at(LOCY) == "") 		rgf_to_check.at(i).at(LOCY) = 		rgf_to_check.at(i-1).at(LOCY);
		if (rgf_to_check.at(i).at(FORMATION) == "") rgf_to_check.at(i).at(FORMATION) = 	rgf_to_check.at(i-1).at(FORMATION);
		if (rgf_to_check.at(i).at(DATATYPE) == "") 	rgf_to_check.at(i).at(DATATYPE) = 	rgf_to_check.at(i-1).at(DATATYPE);
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

			cout << "    - ERROR: DATA_ID " << ID << " used in line " << i + 1 << " is already used at line " << (*(p.first)).second + 1 << "." <<endl;

			error = true;
		}
	}

	if (error) return false;

	cout << "    - Correct DATA_ID's in all records." << endl;

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

bool GCcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		if (!is_allowed_groupcode (rgf_to_check.at(i).at(GROUP))) bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
	}

	return error_cout (bad_records, "group code");
}

bool COLORcheck () {

	vector <string> bad_records;

	for (size_t i = 0; i < rgf_to_check.size(); i++) {

		if (!is_allowed_colorcode (rgf_to_check.at(i).at(COLOR))) bad_records.push_back(rgf_to_check.at(i).at(DATA_ID));
	}

	return error_cout (bad_records, "color code");
}

bool LOCcheck () {

	bool correct = (rgf_to_check.at(0).at(LOCATION) != "");

	if (!correct) cout <<"    - ERROR: LOCALITY must be indicated at least in the 1st record." << endl;

	return correct;
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

vector <string> check_rgf_inputs (vector <string> inputfilename_vector) {

	if (is_COMMANDLINE()) {

		while (!(rgffile_correct (inputfilename_vector.at(0)))) {

			cout << "    - Input " << capslock(inputfilename_vector.at(0)) << ".RGF file structure is incorrect; please enter file name again, or press 'X' to exit." << endl;

			inputfilename_vector.at(0) = inputfilename();
		}

		return inputfilename_vector;
	}

	else if (is_GUI() || is_DEBUG()) {

		if (rgffile_correct(inputfilename_vector.at(0))) {

			cout << "    - Input " << capslock(inputfilename_vector.at(0)) << ".RGF file structure is correct." << endl;

			return inputfilename_vector;
		}

		else throw runtime_error ("No file to process.");
	}

	else {  //is_BATCH

		for (size_t j = inputfilename_vector.size() - 1; j >= 0; j--) {

			if (rgffile_correct(inputfilename_vector.at(j))) {

				cout << "    - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is correct." << endl;

				if (j == 0) return inputfilename_vector;
			}

			else {

				cout << "    - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is incorrect, file will be not evaluated." << endl;

				inputfilename_vector.erase(inputfilename_vector.begin() + j);

				if (inputfilename_vector.size() == 0) {

					cout << "    - No input file to process, exiting." << endl;

					throw exit_requested();
				}
			}
		}

		return inputfilename_vector;
	}
}

bool rgffile_correct (string projectname) {

	if (!(input_rgf (projectname))) return false;

	complete_rgf_to_check ();

	if  (!(
			IDcheck () &&
			IDcheck_duplicate () &&
			GCcheck () &&
			COLORcheck () &&
			LOCcheck () &&
			XYcheck () &&
			DATATYPEcheck () &&
			DIPDIRcheck () &&
			DIPcheck () &&
			STRIAE_SC_check	() &&
			PALEONcheck ()		)) {

		if (is_GUI()) {

			throw rgf_error();
		}
		else return false;
	}

	return true;
}

bool is_STRIAE (const string DATATYPE) {

	return DATATYPE == "STRIAE";
}

bool is_SC (const string DATATYPE) {

	return DATATYPE == "SC";
}

bool is_BEDDING (const string DATATYPE) {

	return DATATYPE == "BEDDING";
}

bool is_OTHERcorrect (vector <string> in) {

	return (
			!is_SC (in.at(DATATYPE)) &&
			!is_STRIAE (in.at(DATATYPE)) &&
			!is_BEDDING (in.at(DATATYPE)) &&
			in.at(LDIR) == "" &&
			in.at(LDIP) == "" &&
			in.at(SENSE) == ""
	);
}

bool is_BEDDINGcorrect (vector <string> in) {

	return (
			is_BEDDING (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			in.at(LDIR) == "" &&
			in.at(LDIP) == "" &&
			is_allowed_bedding_sense (in.at(SENSE))
	);
}

bool is_SCcorrect (vector <string> in) {

	return (
			is_SC (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			is_allowed_dir (in.at(LDIR)) &&
			is_allowed_dip (in.at(LDIP)) &&
			in.at(SENSE) == ""
	);
}

bool is_LINEATIONcorrect (vector <string> in) {

	return (
			is_STRIAE (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			is_allowed_dir (in.at(LDIR)) &&
			is_allowed_dip (in.at(LDIP)) &&
			is_allowed_striae_sense (in.at(SENSE)));
}

bool is_PITCHcorrect (vector <string> in) {

	return (
			is_STRIAE (in.at(DATATYPE)) &&
			is_allowed_dir (in.at(DIR)) &&
			is_allowed_dip (in.at(DIP)) &&
			is_allowed_dip (in.at(LDIR)) &&
			is_allowed_geodetic (in.at(LDIP)) &&
			is_allowed_striae_sense (in.at(SENSE))
	);
}

bool is_allowed_groupcode(const string& groupcode) {

	return contains(allowed_groupcodes, groupcode);
}

bool is_allowed_colorcode(const string& colorcode) {

	return contains(allowed_colorcodes, colorcode);
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

bool is_allowed_plane_datatype(const string& datatype) {

	return (contains(allowed_plane_datatypes, datatype));
}

bool is_allowed_striae_datatype(const string& datatype) {

	return (contains(allowed_striae_datatypes, datatype));
}

bool is_allowed_SC_datatype(const string& datatype) {

	return (contains(allowed_SC_datatypes, datatype));
}

bool is_allowed_dir(const string& s){

	bool failed = true;

	double value = string_to_double (s, failed);

	return ((value >= 0.0) && (value <= 360.0) && !failed);
}

bool is_allowed_dip(const string& s) {

	bool failed = true;

	double value = string_to_double (s, failed);

	return ((value >= 0.0) && (value <= 90.0) && !failed);
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

bool is_double (const string& s) {

	bool failed = true;

	string_to_double (s, failed);

	return (!failed);
}

bool error_cout (vector <string> bad_records, string recordtype) {

	if (bad_records.size() == 0) {

		cout << "    - Correct " << recordtype << "(s) in all records." << endl;

		return true;
	}

	else {

		cout << "    - ERROR: incorrect " << recordtype << "(s) in the following record(s):  " << flush;

		for (size_t j = 0; j < bad_records.size() - 1; j++) {

			cout << bad_records.at(j) << ", " << flush;
		}

		cout << bad_records.at(bad_records.size()-1) << "." << endl;

		return false;
	}
}

vector <GDB> create_GDB_from_rgf () {

	vector <GDB> outGDB;

	size_t i = 0;

	for (i = 0; i < rgf_to_check.size(); i++) {

		bool failed;

		GDB buffer;

		buffer.DIPDIR = 999.99;
		buffer.DIP = 	999.99;
		buffer.LDIR = 	999.99;
		buffer.LDIP = 	999.99;

		buffer.corr.DIPDIR = 	999.99;
		buffer.corr.DIP = 		999.99;
		buffer.corrL.DIPDIR =	999.99;
		buffer.corrL.DIP = 		999.99;

		buffer.LPITCH = 999.99;

		buffer.OFFSET = 		"NONE";
		buffer.LINEATION =		"NONE";
		buffer.LPITCHSENSE = 	"NONE";

		buffer.iID = 		i;

		buffer.ID = 		rgf_to_check.at(i).at(DATA_ID);
		buffer.GC = 		rgf_to_check.at(i).at(GROUP);
		buffer.COLOR = 		rgf_to_check.at(i).at(COLOR);
		buffer.LOC = 		rgf_to_check.at(i).at(LOCATION);
		buffer.LOCX = 		string_to_double(rgf_to_check.at(i).at(LOCX), failed);
		buffer.LOCY = 		string_to_double(rgf_to_check.at(i).at(LOCY), failed);
		buffer.FORMATION = 	rgf_to_check.at(i).at(FORMATION);
		buffer.DATATYPE = 	rgf_to_check.at(i).at(DATATYPE);

		if (	(is_allowed_lineation_datatype (rgf_to_check.at(i).at(DATATYPE))) ||
				(is_allowed_plane_datatype (rgf_to_check.at(i).at(DATATYPE)))) {

			buffer.DIPDIR = 		string_to_double(rgf_to_check.at(i).at(DIR), failed);
			buffer.DIP = 			string_to_double(rgf_to_check.at(i).at(DIP), failed);
			buffer.corr.DIPDIR = 	string_to_double(rgf_to_check.at(i).at(DIR), failed);
			buffer.corr.DIP = 		string_to_double(rgf_to_check.at(i).at(DIP), failed);
		}

		if (is_PITCHcorrect(rgf_to_check.at(i))) buffer.LINEATION = "PITCH";
		if (is_LINEATIONcorrect(rgf_to_check.at(i))) buffer.LINEATION = "LINEATION";
		if (is_SCcorrect(rgf_to_check.at(i))) buffer.LINEATION = "SC";


		if (	(is_allowed_striae_datatype (rgf_to_check.at(i).at(DATATYPE))) ||
				(is_allowed_SC_datatype (rgf_to_check.at(i).at(DATATYPE)))) {

			buffer.DIPDIR = 		string_to_double(rgf_to_check.at(i).at(DIR), failed);
			buffer.DIP = 			string_to_double(rgf_to_check.at(i).at(DIP), failed);
			buffer.corr.DIPDIR = 	string_to_double(rgf_to_check.at(i).at(DIR), failed);
			buffer.corr.DIP = 		string_to_double(rgf_to_check.at(i).at(DIP), failed);


			if (buffer.LINEATION != "PITCH" ) {

				buffer.LDIR = 			string_to_double(rgf_to_check.at(i).at(LDIR), failed);
				buffer.LDIP = 			string_to_double(rgf_to_check.at(i).at(LDIP), failed);
				buffer.corrL.DIPDIR =	string_to_double(rgf_to_check.at(i).at(LDIR), failed);
				buffer.corrL.DIP = 		string_to_double(rgf_to_check.at(i).at(LDIP), failed);
			}

			else {

				buffer.LPITCH = 		string_to_double(rgf_to_check.at(i).at(LDIR), failed);
				buffer.LPITCHSENSE = 	rgf_to_check.at(i).at(LDIP);
			}
		}

		if (is_allowed_lithology_datatype (rgf_to_check.at(i).at(DATATYPE)))	buffer.DATAGROUP = "LITHOLOGY";
		if (is_allowed_plane_datatype (rgf_to_check.at(i).at(DATATYPE))) 		buffer.DATAGROUP = "PLANE";
		if (is_allowed_lineation_datatype (rgf_to_check.at(i).at(DATATYPE))) 	buffer.DATAGROUP = "LINEATION";
		if (is_allowed_striae_datatype (rgf_to_check.at(i).at(DATATYPE))) 		buffer.DATAGROUP = "STRIAE";
		if (is_allowed_SC_datatype (rgf_to_check.at(i).at(DATATYPE))) 			buffer.DATAGROUP = "SC";

		if (rgf_to_check.at(i).at(DATATYPE) == "BEDDING") {

			if (is_allowed_bedding_overturned_sense(rgf_to_check.at(i).at(SENSE))) 	buffer.OFFSET = "OVERTURNED";
			if (is_allowed_bedding_normal_sense(rgf_to_check.at(i).at(SENSE))) 		buffer.OFFSET = "NORMAL";
		}

		if (rgf_to_check.at(i).at(DATATYPE) == "STRIAE") {

			if (is_allowed_striae_inverse_sense(rgf_to_check.at(i).at(SENSE)))		buffer.OFFSET = "INVERSE";
			if (is_allowed_striae_normal_sense(rgf_to_check.at(i).at(SENSE))) 		buffer.OFFSET = "NORMAL";
			if (is_allowed_striae_dextral_sense(rgf_to_check.at(i).at(SENSE))) 		buffer.OFFSET = "DEXTRAL";
			if (is_allowed_striae_sinistral_sense(rgf_to_check.at(i).at(SENSE)))	buffer.OFFSET = "SINISTRAL";
		}

		if (is_LINEATIONcorrect (rgf_to_check.at(i)))	buffer.LINEATION = "LINEATION";
		if (is_PITCHcorrect (rgf_to_check.at(i))) 		buffer.LINEATION = "PITCH";
		if (is_SCcorrect (rgf_to_check.at(i))) 			buffer.LINEATION = "SC";

		buffer.PALEON = string_to_double(rgf_to_check.at(i).at(PALEONORTH), failed);

		buffer.COMMENT = rgf_to_check.at(i).at(COMMENT);

		outGDB.push_back(buffer);
	}

	cout << "  - Geodatabase completed for " << i << " records." << endl;

	return outGDB;
}

//=============================================================================
// Comma-separated values (CSV) file I/O contributed by Ali Baharev

namespace {

vector<size_t> index_map;

const size_t NOT_FOUND = numeric_limits<size_t>::max();

}

template <typename Container, typename T>
size_t find_index(const Container& c, const T& value) {

	typename Container::const_iterator pos = std::find(c.begin(), c.end(), value);

	return (pos == c.end()) ? NOT_FOUND : std::distance(c.begin(), pos);
}

size_t cell_index(size_t i) {

	return (i < index_map.size()) ? index_map.at(i) : NOT_FOUND;
}

template <typename T>
const vector<T> set_difference(vector<T> A, vector<T> B) {

	sort(A.begin(), A.end());

	sort(B.begin(), B.end());

	vector<T> C;

	set_difference(A.begin(), A.end(), B.begin(), B.end(), back_inserter(C));

	return C;
}

void show_col_names(const string& msg, const vector<string>& v) {

	if (v.empty()) {

		return;
	}

	cout << "      - The following columns are " << msg << ":\n        ";

	for (size_t i=0; i<v.size(); ++i) {

		cout << "\"" << v.at(i) << "\"" << ((i==v.size()-1) ? '\n' : '\t');
	}

}

void dump_col_status(const vector<string>& found, const vector<string>& ignored) {

	vector<string> unused = set_difference(reserved_column_names(), found);

	cout << "    - The header (column names) of the data file has been processed.\n";

	show_col_names("unused", unused);

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

void convert_row(const vector<string>& orig_row, size_t index) {

	const size_t n_col = reserved_column_names().size();

	vector<string> row(n_col);

	for (size_t i=0; i<orig_row.size(); ++i) {

		size_t index = cell_index(i);

		if (index!=NOT_FOUND) {

			row.at(index) = to_uppercase( orig_row.at(i) ); // TODO Convert to uppercase?
			//row.at(index) = orig_row.at(i);
		}
	}

	rgf_to_check.push_back(row);
}

void convert_csv_rgf() {

	build_column_map();

	for (size_t i=1; i<orig_table.size(); ++i) {

		convert_row(orig_table.at(i), i-1);
	}
}

void csv_to_rgf() {

	rgf_to_check.clear();

	convert_csv_rgf();
}

// Entry point, clear all global mutable state first!
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


