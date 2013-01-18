// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <map>
#include <iostream>

#include "checkxycontent.h"
#include "checkrgffilecontent.h" // TODO XY related functions in checkrgffilecontent.h
#include "run_mode.h"
#include "exceptions.hpp"
#include "read_csv.hpp"

namespace {

vector <vector <string> > xy_to_check;
vector <vector <string> > orig_table;

enum record_name {
	LOCATION,
	LOC_X,
	LOC_Y,
	FORMATION,
	SIZE
};

}

using namespace std;

bool needxyfile () {

	string need_xy_file;

	cout << endl;

	cout << "COORDINATE FILE" << endl;

	do {

		cout << "Do You want to use own coordinates in XY data format....[Y]," 				<< endl;
		cout << "or use coordinate data in RGF file......................[N]........?  "	<< flush;

		cin >> need_xy_file;

		need_xy_file = capslock(need_xy_file);

		if (need_xy_file == "X") {

			if (is_GUI()) throw exit_requested();
			else return false;
		}
	}

	while (!((need_xy_file == "Y") || (need_xy_file == "N")));

	if (need_xy_file == "Y") return true;

	return false;
}

string inputxyfilename () {

	string xyfilename;

	cout << endl << "  - Enter coordinate file (*.xy) name without extension, or press 'X' to skip: " << endl;

	cin  >> xyfilename;

	return xyfilename;
}

void read_in_xy(const string& file_name) {

	orig_table.clear();
	xy_to_check.clear();

	read_csv(file_name, orig_table);

	for (size_t i = 0; i < orig_table.size(); i++) {

		vector<string> row(SIZE);

		row = orig_table.at(i); // FIXME Bug

		xy_to_check.push_back(row);
	}
}

bool input_xy (const string& projectname) {

	try {

		read_in_xy(projectname+".xy");
	}
	catch (xy_error& ) { // FIXME Nobody throws this...

		return false;
	}

	size_t n_records = xy_to_check.size();

	if (n_records <= 1) {

		cout << "  - Cannot process " << capslock(projectname + ".xy") << " file." << endl;

		return false; // TODO What should happen with empty / header only files? Is it an error?
	}

	else {

		cout << "  - Input " << capslock(projectname + ".xy") << " file read, " << n_records - 1 << " record(s) imported." << endl;

		return true;
	}
}

bool LOCATIONcheck () {

	vector <size_t> bad_records;

	for (size_t i = 1; i < xy_to_check.size(); i++) {

		string LOC = xy_to_check.at(i).at(LOCATION);

		if (LOC == "") bad_records.push_back (i + 1);
	}

	if (bad_records.size() == 0) {

		cout << "    - Existing LOCATION's in all records of XY file." << endl;

		return true;
	}

	else {

		cout <<"    - XY ERROR: empty LOCATION(s) in the following record(s):  " << flush;

		for (size_t j = 0; j < bad_records.size() - 1; j++) {

			cout << bad_records.at(j) << ", " << flush;
		}

		cout << bad_records.at(bad_records.size()-1) << "." << endl;

		return false;
	}
}

bool LOCATIONcheck_duplicate () {

	map <string, int> record_to_check;

	bool error = false;

	size_t i = 0;

	for (i = 1; i < xy_to_check.size(); i++) {

		string LOC = xy_to_check.at(i).at(LOCATION);

		pair<string, int> ID_and_counter(LOC, i);

		pair <map <string, int>::iterator, bool> p = record_to_check.insert(ID_and_counter);

		if (!(p.second)) {

			cout << "    - XY ERROR: LOCATION " << LOC << " used in line " << i + 1 << " is already used at line " << (*(p.first)).second + 1 << "." <<endl;

			error = true;
		}
	}

	if (error) return false;

	cout << "    - Correct LOCATION's in all records of XY file." << endl;

	return true;
}

bool XYcheck () {

	vector <string> bad_records;

	for (size_t i = 1; i < xy_to_check.size(); i++) {

		if (!is_allowed_coordinate (xy_to_check.at(i).at(LOC_X)) ||	!is_allowed_coordinate (xy_to_check.at(i).at(LOC_Y))) bad_records.push_back(xy_to_check.at(i).at(LOCATION));
	}

	return error_cout (bad_records, "coordinate");
}

bool xyfile_correct (string projectname) {

	if (!(input_xy (projectname))) return false;

	if  (!(LOCATIONcheck () && LOCATIONcheck_duplicate () && XYcheck ())) {

		if (is_GUI()) throw xy_error ();

		else return false;
	}

	return true;
}

// FIXME Dead function. Who does the upper case conversion?
void uppercase_xy_to_check () {

	for (size_t i = 1; i < xy_to_check.size(); i++) {

		for (size_t j = 0; j < SIZE; j++) {

			cout << i << j << endl;

			if (xy_to_check.at(i).at(j) != "")  xy_to_check.at(i).at(j) = capslock (xy_to_check.at(i).at(j));
		}
	}
}

string check_xy_inputs (string inputfilename) {

	cout << "CHECKING OF '" << capslock (inputfilename) << ".XY' FILE INTEGRITY" << endl;

	if (is_COMMANDLINE()) {

		if (capslock (inputfilename) == "X") return "NONE";

		while (!(xyfile_correct(inputfilename))) {

			cout << "  - Input " << capslock(inputfilename) << ".XY file structure is incorrect." << endl;

			inputfilename = inputxyfilename();

			if (capslock (inputfilename) == "X") return "NONE";
		}

		cout << "  - Input " << capslock(inputfilename) << ".XY file structure is correct." << endl;

		return inputfilename;
	}

	else { // GUI, BATCH, DEBUG

		if (xyfile_correct(inputfilename)) {

			cout << "  - Input " << capslock(inputfilename) << ".XY file structure is correct." << endl;

			return inputfilename;
		}

		else {

			cout << "  - Input " << capslock(inputfilename) << ".XY file does not exist or file structure is incorrect, file will not be used." << endl;

			//if (is_GUI()) throw xy_error () // FIXME Hmm, why shouldn't we throw it?

			//else;

			 return "NONE";
		}
	}
}

GDB insertxy (GDB inGDB) {

	GDB outGDB = inGDB;

	bool failed;

	for (size_t i = 0; i < xy_to_check.size(); i++) {

	if (outGDB.LOC == xy_to_check.at(i).at(LOCATION)) {

			outGDB.LOC = 		xy_to_check.at(i).at(LOCATION);
			outGDB.LOCX = 		string_to_double(xy_to_check.at(i).at(LOC_X), failed);
			outGDB.LOCY = 		string_to_double(xy_to_check.at(i).at(LOC_Y), failed);
			outGDB.FORMATION = 	xy_to_check.at(i).at(FORMATION);
		}
	}

	return outGDB;
}
