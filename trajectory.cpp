// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iomanip>
#include <iostream>
#include <map>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "checkrgffilecontent.h"
#include "data_sort.hpp"
#include "exceptions.hpp"
#include "read_csv.hpp"
#include "rgf.h"
#include "run_mode.h"
#include "settings.hpp"
#include "structs.h"

struct TRJ_N {

	double DEPTH;
	VCTR N;
};

namespace {

bool TRAJECTORY_FILE_CORRECT = false;
bool TRAJECTORY_ZXY = false;
bool TRAJECTORY_DAP = false;

bool TRAJECTORY_HAS_ZERO = false;
bool TRAJECTORY_HAS_MORE_THAN_ONE_RECORD = false;


vector <TRJ> TRAJECTORY;
vector <TRJ_N> TRAJECTORY_N;

vector <vector <string> > trj_to_check;
vector <vector <string> > orig_table;

enum record_name {
	DEPTH,
	X_AZIMUTH,
	Y_PLUNGE,
	SIZE
};

}

using namespace std;

bool is_TRAJECTORY_FILE_CORRECT () {

	return TRAJECTORY_FILE_CORRECT;
}

vector <TRJ> return_TRAJECTORY () {

	return TRAJECTORY;
}

void read_in_trj (const string& file_name) {

	orig_table.clear();
	trj_to_check.clear();

	read_csv (file_name, orig_table);

	for (size_t i = 0; i < orig_table.size(); i++) {

		vector<string> row = orig_table.at (i);

		row.resize(SIZE);

		row = vec_to_uppercase (row);

		trj_to_check.push_back (row);
	}
}

bool input_trajectory (const string& projectname) {

	read_in_trj (projectname + ".trj");

	size_t n_records = trj_to_check.size();

	if (n_records <= 1) {

		if (!is_mode_DEBUG()) {

			writeln ("  - Cannot process " + (capslock(projectname)) + ".TRJ file or the file is empty.");
		}
		return false;
	}
	else {

		if (!is_mode_DEBUG()) {

			write   ("  - Input " + capslock (projectname + ".trj") + " file read, ");
			write   (size_t_to_string (n_records - 1));
			writeln (" record(s) imported.");
		}
		return true;
	}
}

bool trjDEPTHcheck () {

	if (is_WELLDATA_NO()) return true;

	vector <string> bad_records;

	for (size_t i = 1; i < trj_to_check.size(); i++) {

		const string DPT = trj_to_check.at(i).at(DEPTH);

		bool failed = false;

		const double DPH = string_to_double (DPT, failed);

		const bool POS = DPH >= 0.0;

		if (is_in_range(0.0, 0.0, DPH)) TRAJECTORY_HAS_ZERO = true;

		if (failed || !POS) bad_records.push_back (trj_to_check.at(i).at(DEPTH));
	}

	if (! TRAJECTORY_HAS_ZERO) writeln ("    - Trajectory file has no record for 0 m MD.");
	return error_cout (bad_records, "depth code");
}

bool trjDEPTHcheck_duplicate () {

	map <string, int> record_to_check;

	bool error = false;

	for (size_t i = 1; i < trj_to_check.size(); i++) {

		string DPT = trj_to_check.at(i).at(DEPTH);

		pair<string, int> ID_and_counter(DPT, i);

		pair <map <string, int>::iterator, bool> p = record_to_check.insert (ID_and_counter);

		if (!(p.second)) {

			if (!is_mode_DEBUG())  cout << "    - TRAJECTORY ERROR: DEPTH " << DPT << " used in line " << i + 1 << " is already used at line " << (*(p.first)).second + 1 << "." <<endl;

			error = true;
		}
	}

	if (error) return false;

	if (!is_mode_DEBUG())  cout << "    - Correct DEPTH's in all records of the trajectory file." << endl;

	return true;
}

bool trjXYcheck () {

	vector <string> bad_records;

	for (size_t i = 1; i < trj_to_check.size(); i++) {

		if (!is_allowed_coordinate (trj_to_check.at(i).at (X_AZIMUTH)) ||	!is_allowed_coordinate (trj_to_check.at(i).at(Y_PLUNGE))) bad_records.push_back (trj_to_check.at(i).at(DEPTH));
	}

	return error_cout (bad_records, "coordinate");
}


bool trj_azimuth (){

	for (size_t i = 1; i < trj_to_check.size(); i++) {

		const double DD = string_to_double(trj_to_check.at(i).at(X_AZIMUTH));

		if (! is_allowed_DIR (DD)) return false;
	}
	return true;
}

bool trj_plunge () {

	for (size_t i = 1; i < trj_to_check.size(); i++) {

		const double D = string_to_double (trj_to_check.at(i).at(Y_PLUNGE));

		if (! is_allowed_DIP (D)) return false;
	}
	return true;
}

void CHECK_TRAJECTORY_FILE (const string projectname) {

	writeln ("");
	writeln ("===========================");
	writeln ("5) CHECKING TRAJECTORY FILE");
	writeln ("===========================");
	writeln ("");

	if (!is_WELLDATA_USE()) {

		cout << "  - No well data processed, no need for trajectory file." << endl;

		return;
	}

	if (!(input_trajectory (projectname))) {

		TRAJECTORY_FILE_CORRECT = false;
		return;
	}

	if  (!(trjDEPTHcheck_duplicate() && trjDEPTHcheck () && trjXYcheck ())) {

		if (is_mode_GUI()) throw trj_error();
		else {

			TRAJECTORY_FILE_CORRECT = false;
			return;
		}
	}
	TRAJECTORY_FILE_CORRECT = true;

	string MSG;

	if (trj_azimuth () && trj_plunge ()) {

		TRAJECTORY_DAP = true;
		MSG = " 'depth - azimuth - plunge' ";

	}
	else {

		TRAJECTORY_ZXY = true;
		MSG = " 'depth - X - Y' ";
	}
	writeln ("    - Trajectory file has"+MSG+"records.");
	return;
}

void generate_trajectory_vector () {

	for (size_t i = 1; i < trj_to_check.size(); i++) {

		TRJ buf;

		buf.DEPTH = 	string_to_double (trj_to_check.at(i).at(DEPTH));
		buf.X_AZIMUTH = string_to_double (trj_to_check.at(i).at(X_AZIMUTH));
		buf.Y_PLUNGE =  string_to_double (trj_to_check.at(i).at(Y_PLUNGE));

		TRAJECTORY.push_back(buf);
	}
	return;
}

void add_record_zero_max () {

	if (TRAJECTORY_N.size() < 1) return;

	TRJ_N MAX = TRAJECTORY_N.at (TRAJECTORY_N.size() - 1);

	MAX.DEPTH = 99999;

	TRAJECTORY_N.push_back (MAX);

	if (TRAJECTORY_HAS_ZERO) return;

	vector <TRJ_N> OUT;

	TRJ_N dummy = TRAJECTORY_N.at(0);

	dummy.DEPTH = 0;

	OUT.push_back (dummy);

	OUT.insert (OUT.end(), TRAJECTORY_N.begin(), TRAJECTORY_N.end());

	TRAJECTORY_N = OUT;
}

void generate_trajectory_normal_XYZ () {

	for (size_t i = 1; i < TRAJECTORY.size(); i++) {

		const double ACT_DEPTH = TRAJECTORY.at(i-1).DEPTH;
		const double NXT_DEPTH = TRAJECTORY.at(i).DEPTH;

		const double ACT_X = TRAJECTORY.at(i-1).X_AZIMUTH;
		const double NXT_X = TRAJECTORY.at(i).X_AZIMUTH;

		const double ACT_Y = TRAJECTORY.at(i-1).Y_PLUNGE;
		const double NXT_Y = TRAJECTORY.at(i).Y_PLUNGE;

		TRJ_N buf;

		buf.DEPTH = ACT_DEPTH;

		buf.N = unitvector (declare_vector (NXT_X-ACT_X, NXT_Y-ACT_Y, NXT_DEPTH-ACT_DEPTH));

		if (is_N_down (buf.N)) ASSERT_DEAD_END();

		TRAJECTORY_N.push_back (buf);
	}
	return;
}

void generate_trajectory_normal_DAP () {

	for (size_t i = 0; i < TRAJECTORY.size() - 1; i++) {

		const double ACT_DEPTH = TRAJECTORY.at(i).DEPTH;
		const double NXT_DEPTH = TRAJECTORY.at(i+1).DEPTH;
		const double ACT_DD = TRAJECTORY.at(i).X_AZIMUTH;
		const double ACT_D = TRAJECTORY.at(i).Y_PLUNGE;

		TRJ_N buf;

		DIPDIR_DIP DD;

		DD.DIPDIR = ACT_DD;
		DD.DIP = ACT_D;

		buf.DEPTH = (NXT_DEPTH + ACT_DEPTH) / 2.0;
		buf.N = NXNYNZ_from_dipdir_dip (DD);

		if (is_N_down (buf.N)) ASSERT_DEAD_END();

		TRAJECTORY_N.push_back (buf);
	}
}

void generate_trajectory_normal () {

	if (TRAJECTORY_ZXY) generate_trajectory_normal_XYZ ();
	else if (TRAJECTORY_DAP)generate_trajectory_normal_DAP ();
	else ASSERT_DEAD_END();

	add_record_zero_max ();

	return;
}

void PROCESSING_TRAJECTORY () {

	generate_trajectory_vector ();

	if (TRAJECTORY.size() < 2) return;
	else TRAJECTORY_HAS_MORE_THAN_ONE_RECORD = true;

	TRAJECTORY = SORT_TRJ (TRAJECTORY, "DEPTH");

	generate_trajectory_normal ();

	return;
}


vector <GDB> APPLY_TRAJECTORY (const vector <GDB>& inGDB) {

	PROCESSING_TRAJECTORY();

	if (! TRAJECTORY_HAS_MORE_THAN_ONE_RECORD) return inGDB;

	vector <GDB> outGDB = inGDB;

	size_t counter = 0;

	for (size_t i = 1; i < TRAJECTORY_N.size(); i++) {
		for (size_t j = 0; j < outGDB.size(); j++) {

			const double ACT_TRJ_DPT = TRAJECTORY_N.at(i-1).DEPTH;
			const double NXT_TRJ_DPT = TRAJECTORY_N.at(i).DEPTH;

			const double DATA_DPT = outGDB.at(j).DEPTH;

			const bool LAST = i == TRAJECTORY_N.size() - 1;

			const bool FIT1 = is_in_range_UP_EQ (ACT_TRJ_DPT, NXT_TRJ_DPT, DATA_DPT);
			const bool FIT2 = is_in_range_LW_EQ (ACT_TRJ_DPT, NXT_TRJ_DPT, DATA_DPT) && LAST;

			if (FIT1 || FIT2) {

				counter++;

				outGDB.at(j).T = TRAJECTORY_N.at(i).N;
			}
		}
	}
	cout << "  - Trajectory correction was applied for "<<counter<<" records of "<<outGDB.size()<<"."<<endl;

	if (counter > outGDB.size()) ASSERT_DEAD_END();

	return outGDB;
}

void dbg_TRAJECTORY () {

	cout << endl;
	cout << "******** DUMPING TRAJECTORY ********" << endl << endl;

	cout << "DEPTH" << '\t' << "X_AZIMUTH" << '\t'<< "Y_PLUNGE" << endl;

	cout << fixed << setprecision(2) << flush;

	for (size_t i = 0; i < TRAJECTORY.size(); i++) {

		cout
		<< TRAJECTORY.at(i).DEPTH << '\t'
		<< TRAJECTORY.at(i).X_AZIMUTH << '\t'
		<< TRAJECTORY.at(i).Y_PLUNGE << '\t' << endl;
	}
	cout << "******** END DUMPING TRAJECTORY ********" << endl;
	cout << endl;
}

void dbg_TRAJECTORY_N () {

	cout << endl;
	cout << "******** DUMPING TRAJECTORY AS NORMAL VECTOR ********" << endl << endl;

	cout << "DEPTH" << '\t' << "X" << '\t' << "Y" << '\t' << "Z" << endl;

	cout << fixed << setprecision(2) << flush;

	for (size_t i = 0; i < TRAJECTORY_N.size(); i++) {

		cout
		<< fixed << setprecision(2)
		<< TRAJECTORY_N.at(i).DEPTH << '\t'
		<< fixed << setprecision(6)
		<< TRAJECTORY_N.at(i).N.X << '\t'
		<< TRAJECTORY_N.at(i).N.Y << '\t'
		<< TRAJECTORY_N.at(i).N.Z << '\t' << endl;
	}
	cout << "******** END DUMPING TRAJECTORY AS NORMAL VECTOR ********" << endl;
	cout << endl;
}
