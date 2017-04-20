// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>

#include "allowed_keys.hpp"
#include "checkxycontent.h"
#include "checkrgffilecontent.h"
#include "run_mode.h"
#include "exceptions.hpp"
#include "read_csv.hpp"
#include "settings.hpp"

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

void read_in_xy(const string& file_name) {

	orig_table.clear();

	xy_to_check.clear();

	read_csv (file_name, orig_table);

	for (size_t i=0; i<orig_table.size(); ++i) {

		vector<string> row = orig_table.at(i);

		row.resize(SIZE);

		row = vec_to_uppercase(row);

		xy_to_check.push_back(row);
	}
}

bool input_xy (const string& projectname) {

	read_in_xy (projectname + ".xy");

	size_t n_records = xy_to_check.size();

	if (n_records <= 1) {

		if (!is_mode_DEBUG()) {

			writeln ("  - Cannot process " + (capslock(projectname)) + ".xy file or the file is empty.");
		}
		return false;
	}
	else {

		if (!is_mode_DEBUG()) {

			write   ("  - Input " + capslock (projectname + ".xy") + " file read, ");
			write   (size_t_to_string (n_records - 1));
			writeln (" record(s) imported.");
		}

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

		if (!is_mode_DEBUG()) cout << "    - Existing LOCATION's in all records of XY file." << endl;

		return true;
	}

	else {

		if (!is_mode_DEBUG()) cout <<"    - XY ERROR: empty LOCATION(s) in the following record(s):  " << flush;

		for (size_t j = 0; j < bad_records.size() - 1; j++) {

			if (!is_mode_DEBUG()) cout << bad_records.at(j) << ", " << flush;
		}

		if (!is_mode_DEBUG()) cout << bad_records.at(bad_records.size()-1) << "." << endl;

		return false;
	}
}

bool LOCATIONcheck_duplicate () {

	map <string, int> record_to_check;

	bool error = false;

	for (size_t i = 1; i < xy_to_check.size(); i++) {

		string LOC = xy_to_check.at(i).at(LOCATION);

		pair<string, int> ID_and_counter(LOC, i);

		pair <map <string, int>::iterator, bool> p = record_to_check.insert(ID_and_counter);

		if (!(p.second)) {

			if (!is_mode_DEBUG())  cout << "    - XY ERROR: LOCATION " << LOC << " used in line " << i + 1 << " is already used at line " << (*(p.first)).second + 1 << "." <<endl;

			error = true;
		}
	}

	if (error) return false;

	if (!is_mode_DEBUG())  cout << "    - Correct LOCATION's in all records of XY file." << endl;

	return true;
}

bool XYcheck () {

	vector <string> bad_records;

	for (size_t i = 1; i < xy_to_check.size(); i++) {

		if (!is_allowed_coordinate (xy_to_check.at(i).at(LOC_X)) ||	!is_allowed_coordinate (xy_to_check.at(i).at(LOC_Y))) bad_records.push_back(xy_to_check.at(i).at(LOCATION));
	}

	return error_cout (bad_records, "coordinate");
}

vector <GDB> insert_xy_values (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {
		for (size_t j = 0; j < xy_to_check.size(); j++) {

			if (outGDB.at(i).LOC == xy_to_check.at(j).at(LOCATION)) {

				outGDB.at(i).LOC = 			xy_to_check.at(j).at(LOCATION);
				outGDB.at(i).LOCX = 		string_to_double (xy_to_check.at(j).at(LOC_X));
				outGDB.at(i).LOCY = 		string_to_double (xy_to_check.at(j).at(LOC_Y));
				outGDB.at(i).FORMATION = 	xy_to_check.at(j).at(FORMATION);
			}
		}
	}
	return outGDB;
}

bool CHECK_XY_FILE (const string projectname) {

	writeln ("");
	writeln ("============================");
	writeln ("4A) CHECKING COORDINATE FILE");
	writeln ("============================");
	writeln ("");

	if (is_WELLDATA_USE()) {

		cout << "  - No well data processed, no need for trajectory file." << endl;

		return false;
	}

	if (!(input_xy (projectname))) return false;

	if  (!(LOCATIONcheck () && LOCATIONcheck_duplicate () && XYcheck ())) throw xy_error (); //{

	return true;
}
