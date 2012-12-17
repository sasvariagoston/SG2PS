// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <map>
#include <stdexcept>
#include <sstream>
#include <iomanip>

#include "checkrgffilecontent.h"
#include "exceptions.hpp"

using namespace std;

namespace {

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

const string datatype_allowed [] = {
		"BEDDING",
		"BOUDAIN",
		"CONTACT",
		"CROSSBEDDING",
		"FOLDAXIS",
		"FOLDPLANE",
		"FOLDSURFACE",
		"KINK",
		"LINEATION",
		"LITHOLOGY",
		"LITHOCLASE",
		"SC",
		"S1", "S2", "S3", "S4", "S5",
		"FRACTURE",
		"STRIAE",
		"USERPLANE4", "USERPLANE5",
		"USERLINEATION1", "USERLINEATION2", "USERLINEATION3", "USERLINEATION4", "USERLINEATION5",
		"VEIN"
};

const vector<string> allowed_datatypes(from_array(datatype_allowed));

const string striae_sense_allowed [] = {
		"+", "THRUST", "UP", "INVERSE", "U", "I",
		"-", "NORMAL", "FAULT", "DOWN", "DOWNWARD", "N",
		"DEXTRAL", "DX", "D",
		"SINISTRAL", "SN", "S",
		"X", "NONE"
};

const vector<string> allowed_striae_senses(from_array(striae_sense_allowed));

const string bedding_sense_allowed [] = {
		"O", "OVERTURNED",
		"N", "NORMAL",
		""
};

const vector<string> allowed_bedding_senses(from_array(bedding_sense_allowed));

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


/*int ID_index() {

	return ID;
}

int NAME_index() {

	return NAME;
}

int VALUE_index() {

	return VALUE;
}
 */

struct record {

	//record() : id(-1), name("default name"), value(-1) { }

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

vector<record> converted_table;

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

void push_to_table(const string& line) {

	vector<string> row;

	string cell;

	istringstream iss(line);

	while (getline(iss, cell, '\t')) {

		cell = capslock (cell);

		row.push_back(cell);
	}

	if (row.size() < SIZE) {

		row.resize(SIZE);
	}

	rgf_to_check.push_back(row);
}

void input_rgf (const string& projectname) {

	rgf_to_check.clear();

	int lines_read = 0;

	ifstream rgf_file((projectname + ".rgf").c_str()) ;

	string line;

	while (getline(rgf_file, line)) {

		++lines_read;

		push_to_table(line);
	}

	cout << "    - Input file opened, " << lines_read << " record(s) found." << endl;

	return;
}

void complete_rgf_to_check () {

	for ( size_t i = 2; i < rgf_to_check.size(); i++) {

		if (rgf_to_check.at(i).at(GROUP) == "") 	rgf_to_check.at(i).at(GROUP) = 		rgf_to_check.at(i-1).at(GROUP);
		if (rgf_to_check.at(i).at(COLOR) == "") 	rgf_to_check.at(i).at(COLOR) = 		rgf_to_check.at(i-1).at(COLOR);
		if (rgf_to_check.at(i).at(LOCATION) == "") 	rgf_to_check.at(i).at(LOCATION) = 	rgf_to_check.at(i-1).at(LOCATION);
		if (rgf_to_check.at(i).at(LOCX) == "") 		rgf_to_check.at(i).at(LOCX) = 		rgf_to_check.at(i-1).at(LOCX);
		if (rgf_to_check.at(i).at(LOCY) == "") 		rgf_to_check.at(i).at(LOCY) = 		rgf_to_check.at(i-1).at(LOCY);
		if (rgf_to_check.at(i).at(FORMATION) == "") rgf_to_check.at(i).at(FORMATION) = 	rgf_to_check.at(i-1).at(FORMATION);
		if (rgf_to_check.at(i).at(DATATYPE) == "") 	rgf_to_check.at(i).at(DATATYPE) = 	rgf_to_check.at(i-1).at(DATATYPE);
		if (rgf_to_check.at(i).at(PALEONORTH) == "")rgf_to_check.at(i).at(PALEONORTH) = rgf_to_check.at(i-1).at(PALEONORTH);
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

			cout << "    - ERROR: DATA_ID '" << ID << "' used in line " << i << " is already used at line '" << (*(p.first)).second << "' ." <<endl;

			error = true;
		}
	}

	if (error) return false;

	cout << "    - Correct DATA_ID's in all of " << i << " records." << endl;

	return true;
}

bool IDcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string ID = rgf_to_check.at(i).at(DATA_ID);

		if (ID == "") errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: empty DATA_ID(s) in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Existing DATA_ID's in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

bool GCcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string groupcode = rgf_to_check.at(i).at(GROUP);

		string ID = rgf_to_check.at(i).at(DATA_ID);

		if (!is_allowed_groupcode (groupcode)) errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect group codes(s) in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct group codes in all of " << i << " records." << endl;
		
		return true;
	}
	
	cout << "." <<endl;

	return false;
}

bool COLORcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string colorcode = rgf_to_check.at(i).at(COLOR);

		string ID = rgf_to_check.at(i).at(DATA_ID);

		if (!is_allowed_colorcode (colorcode)) errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect color codes(s) in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct color codes in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

bool LOCcheck () {

	bool correct = (rgf_to_check.at(0).at(LOCATION) != "");

	if (!correct) cout <<"    - ERROR: LOCALITY must be indicated at least in the 1st record." << endl;

	return correct;
}

bool XYcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string COORD_X = rgf_to_check.at(i).at(LOCX);

		string COORD_Y = rgf_to_check.at(i).at(LOCY);

		string ID = rgf_to_check.at(i).at(DATA_ID);

		if (!is_allowed_coordinate (COORD_X)) errorcounter++;

		if (!is_allowed_coordinate (COORD_Y)) errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect coordinate(s) in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct coordinate(s) in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

bool DATATYPEcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string TYPE_OF_DATA = rgf_to_check.at(i).at(DATATYPE);

		string ID = rgf_to_check.at(i).at(DATA_ID);

		if (!is_allowed_datatype (TYPE_OF_DATA)) errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect data type(s) in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct data type(s) in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

bool DIPDIRcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string PLANE_DIR = rgf_to_check.at(i).at(DIR);

		string ID = rgf_to_check.at(i).at(DATA_ID);

		string TYPE_OF_DATA = rgf_to_check.at(i).at(DATATYPE);

		if ((PLANE_DIR != "") && (TYPE_OF_DATA == "LITHOLOGY")) errorcounter++;

		if ((PLANE_DIR != "") && (!is_allowed_dir (PLANE_DIR))) errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect strike/dip direction(s) in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct strike/dip direction(s) in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

bool DIPcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string PLANE_DIP = rgf_to_check.at(i).at(DIP);

		string ID = rgf_to_check.at(i).at(DATA_ID);

		string TYPE_OF_DATA = rgf_to_check.at(i).at(DATATYPE);

		if ((PLANE_DIP != "") && (TYPE_OF_DATA == "LITHOLOGY")) errorcounter++;

		if ((PLANE_DIP != "") &&  (!is_allowed_dip (PLANE_DIP))) errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect dip angle(s) in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct dip angle(s) in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

bool STRIAE_SC_check () {

	size_t i = 0;

	size_t errorcounter = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		bool STRIAE = 	rgf_to_check.at(i).at(DATATYPE) == "STRIAE";
		bool SC = 		rgf_to_check.at(i).at(DATATYPE) == "SC";
		bool BEDDING = 	rgf_to_check.at(i).at(DATATYPE) == "BEDDING";

		string ID = 				rgf_to_check.at(i).at(DATA_ID);
		string PLANE_DIR = 			rgf_to_check.at(i).at(DIR);
		string PLANE_DIP = 			rgf_to_check.at(i).at(DIP);
		string LINEATION_DIR = 		rgf_to_check.at(i).at(LDIR);
		string LINEATION_DIP = 		rgf_to_check.at(i).at(LDIP);
		string LINEATION_SENSE = 	rgf_to_check.at(i).at(SENSE);

		bool OTHERcorrect = (
				((!SC) && (!STRIAE ) && (!BEDDING)) &&
				(LINEATION_DIR == "") &&
				(LINEATION_DIP == "") &&
				(LINEATION_SENSE == ""));

		bool BEDDINGcorrect = (
				(BEDDING) &&
				(is_allowed_dir (PLANE_DIR)) &&
				(is_allowed_dip (PLANE_DIP)) &&
				(LINEATION_DIR == "") &&
				(LINEATION_DIP == "") &&
				is_allowed_bedding_sense (LINEATION_SENSE));

		bool SCcorrect = (
				(SC) &&
				(is_allowed_dir (PLANE_DIR)) &&
				(is_allowed_dip (PLANE_DIP)) &&
				(is_allowed_dir (LINEATION_DIR)) &&
				(is_allowed_dip (LINEATION_DIP)) &&
				(LINEATION_SENSE == ""));

		bool LINEATIONcorrect = (
				(STRIAE) &&
				(is_allowed_dir (PLANE_DIR)) &&
				(is_allowed_dip (PLANE_DIP)) &&
				(is_allowed_dir (LINEATION_DIR)) &&
				(is_allowed_dip (LINEATION_DIP)) &&
				is_allowed_striae_sense (LINEATION_SENSE));

		bool PITCHcorrect = (
				STRIAE &&
				is_allowed_dir (PLANE_DIR) &&
				is_allowed_dip (PLANE_DIP) &&
				is_allowed_dip (LINEATION_DIR) &&
				is_allowed_geodetic (LINEATION_DIP) &&
				is_allowed_striae_sense (LINEATION_SENSE));

		if (!(OTHERcorrect || BEDDINGcorrect || SCcorrect ||  LINEATIONcorrect || PITCHcorrect)) errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect striae/SC data in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct striae/SC in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

bool PALEONcheck () {

	size_t errorcounter = 0;

	size_t i = 0;

	for (i = 1; i < rgf_to_check.size(); i++) {

		string P_NORTH = rgf_to_check.at(i).at(PALEONORTH);

		string ID = rgf_to_check.at(i).at(DATA_ID);

		if (!is_allowed_dir (P_NORTH) && P_NORTH != "") errorcounter++;

		if (errorcounter == 1) cout <<"    - ERROR: incorrect paleo north data in following record(s):  " << ID << flush;

		if (errorcounter > 1) cout << ", " << ID << flush;
	}

	if (errorcounter == 0) {

		cout << "    - Correct paleo north data in all of " << i << " records." << endl;

		return true;
	}

	cout << "." <<endl;

	return false;
}

vector <string> check_rgf_inputs (vector <string> inputfilename_vector, bool batch) {

	size_t j = 1;

	cout << "CHECKING INPUT FILE(S) INTEGRITY" << endl;

	do {

		if (batch) {

			if (rgffile_correct(inputfilename_vector.at(j))) cout << "  - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is correct." << endl;

			else {

				inputfilename_vector.erase(inputfilename_vector.begin() + j);
			}

			if (inputfilename_vector.size() < 2) throw runtime_error ("No file to process");
		}

		else {

			while (!(rgffile_correct (inputfilename_vector.at(j)))) {

				cout << "  - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is incorrect; please enter file name again, or press 'X' to exit." << endl;
				inputfilename_vector.at(j) = inputfilename();
			}

			cout << "  - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is correct." << endl;
		}

		j++;

	} while (j < inputfilename_vector.size());

	return inputfilename_vector;
}

vector <string> create_inputfilename_vector (int argc, char *argv[]) {

	int j = 0;
	vector <string> out;

	do  {

		out.push_back(argv[j]);
		j++;

	} while (j < argc);

	return out;
}

bool rgffile_correct (string projectname) {

	input_rgf (projectname);

	complete_rgf_to_check ();

	if  (!(
			IDcheck () &&
			IDcheck_duplicate () &&
			GCcheck () &&
			COLORcheck () &&
			LOCcheck () &&
			DATATYPEcheck () &&
			DIPDIRcheck () &&
			DIPcheck () &&
			STRIAE_SC_check	() &&
			PALEONcheck ()		))


	throw rgf_error();

	return true;
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

bool is_allowed_datatype(const string& datatype) {

	return contains(allowed_datatypes, datatype);
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

bool is_allowed_striae_sense(const string& sense) {

	return contains(allowed_striae_senses, sense);
}

bool is_allowed_bedding_sense(const string& sense) {

	return contains(allowed_bedding_senses, sense);
}

bool is_allowed_geodetic (const string& geodetic) {

	return contains(allowed_geodetics, geodetic);
}

bool is_double (const string& s) {

	bool failed = true;

	double value = string_to_double (s, failed);

	return (!failed);
}
