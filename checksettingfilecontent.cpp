
// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cstdlib> // TODO Instead of atof, a string_to_int function should be used in common.cpp
#include <fstream>
#include <iomanip>
#include <iostream>

#include "assertions.hpp"
#include "checksettingfilecontent.h"
#include "exceptions.hpp"
#include "generate_default_settings.hpp"
#include "run_mode.h"
#include "settings.hpp"

vector < vector <string> > READ_SETTINGS_FILE (const string settingfilename) {

	vector < vector <string> > SET;

	ifstream inputfile (settingfilename.c_str());

	while (!inputfile.eof()) {

		string key, value;

		vector <string> buf;

		getline(inputfile, key, '\t');

		getline(inputfile, value);

		const size_t len = value.size();

		if (len > 0 && value.at(len-1)=='\r') { // on Linux/Mac, reading files from Windows

			value.erase(len-1);
		}

		if (!is_WHITE_CHARACTER (key) && !is_WHITE_CHARACTER (value)) {

			buf.push_back (to_uppercase (key));

			buf.push_back (to_uppercase (value));

			SET.push_back(buf);
		}
	}
	return SET;
}

bool is_WHITE_CHARACTER (const string in) {

	size_t length = in.size();

	if (length == 0) return true;
	else {};

	bool is_character_white = false;

	for (size_t i = 0; i < length; i++) if (in.at(i) == '\t' || in.at(i) == '\n' ) is_character_white = true;

	return is_character_white;
}

bool is_RECORD_CORRECT (const vector<string>& SETrecord, const vector<vector<vector<string> > >& DEF, size_t j, size_t k) {

	string actual_key = SETrecord.at(0);
	string list_key = DEF.at(j).at(0).at(0);

	if (actual_key != list_key) return false;

	string actual_value = capslock (SETrecord.at(1));
	string list_value = capslock (DEF.at(j).at(k).at(0));

	if (actual_key == "LINEWIDTH:" || actual_key == "STRESSANGLE:" || actual_key == "WELLINTERVAL_LENGTH:") {

		bool conv_has_failed = true;

		double min_value = string_to_double (DEF.at(j).at(k).at(1), conv_has_failed);
		double max_value = string_to_double (DEF.at(j).at(k).at(2), conv_has_failed);

		string_to_int (actual_value, conv_has_failed);

		if (actual_key == "LINEWIDTH:" && conv_has_failed) return false;

		if (actual_key == "STRESSANGLE:" && conv_has_failed) return false;

		if (actual_key == "WELLINTERVAL_LENGTH:" && conv_has_failed) return false;

		double value = string_to_double (actual_value, conv_has_failed);

		if (is_in_range(min_value, max_value, value) && !conv_has_failed) return true;
		else return false;
	}

	else if (actual_key == "CLUSTERNUMBER:") {

		if (actual_value == "A" || actual_value == "N") {

			if (actual_value == list_value) return true;
			else return false;
		}
		else {

			bool conv_has_failed = true;

			double min_value = string_to_double (DEF.at(j).at(k).at(1), conv_has_failed);
			double max_value = string_to_double (DEF.at(j).at(k).at(2), conv_has_failed);

			double value = string_to_double (actual_value, conv_has_failed);

			if (is_in_range(min_value, max_value, value) && !conv_has_failed) return true;
			else return false;
		}
	}
	else {

		if (actual_value == list_value) return true;
		else return false;
	}
}

bool is_SETTINGS_RECORD_CORRECT (const vector <string>& SETrecord) {

	const vector <vector <vector <string> > > DEF = RETURN_ALL_SETTINGS ();

	for (size_t j = 0; j < DEF.size(); j++) {
		for (size_t k = 1; k < DEF.at(j).size(); k++) {

			if (is_RECORD_CORRECT (SETrecord, DEF, j, k)) return true;
		}
	}
	return false;
}

bool is_SETTINGS_FILE_CORRECT (const string settingfilename) {

	ifstream inputfile (settingfilename.c_str());

	if (! inputfile.is_open()) return false;

	const vector <vector <string> > SET = READ_SETTINGS_FILE (settingfilename);

	if (SET.size() == 0) return false;

	for (size_t i = 0; i < SET.size(); i++) {

		if (!is_SETTINGS_RECORD_CORRECT (SET.at(i))){

			writeln ("   - Incorrect key/value pair for '" + SET.at(i).at(0)+ "' user input key.");
			return false;
		}
		else {

			writeln ("   - Correct key/value pair for '" + SET.at(i).at(0)+ "' user input key.");
		}
	}
	return true;
}

vector <vector <string> > RETURN_HARDCODED_SETTINGS () {

	vector < vector < vector <string> > > DEF = RETURN_ALL_SETTINGS ();

	vector < vector <string> > OUT;

	for (size_t i = 0; i < DEF.size(); i++) {

		vector <string> buf;

		buf.push_back(DEF.at(i).at(0).at(0));

		if      (DEF.at(i).at(0).at(0) == "LINEWIDTH:")   			buf.push_back("6");
		else if (DEF.at(i).at(0).at(0) == "WELLINTERVAL_LENGTH:")   buf.push_back("100");
		else if (DEF.at(i).at(0).at(0) == "STRESSANGLE:") 			buf.push_back("30");
		else buf.push_back(DEF.at(i).at(1).at(0));

		OUT.push_back(buf);
	}
	return OUT;
}

vector <vector <string> > COMPLETE_SET_WITH_DEFAULT (const vector <vector <string> >& SET) {

	vector <vector <vector <string> > > DEF = RETURN_ALL_SETTINGS ();

	vector <vector <string> > OUT;

	for (size_t i = 0; i < DEF.size(); i++) {

		string out_key   = DEF.at(i).at(0).at(0);
		string out_value = DEF.at(i).at(1).at(0);

		for (size_t j = 0; j < SET.size(); j++) {

			if (SET.at(j).at(0) == out_key) out_value = SET.at(j).at(1);
		}
		vector <string> buf;

		buf.push_back (out_key);
		buf.push_back (out_value);

		OUT.push_back (buf);
	}
	return OUT;
}

void LOAD_SETTINGS (const string projectname) {

	vector <vector <string> > SET = READ_SETTINGS_FILE (projectname + ".set");

	SET = COMPLETE_SET_WITH_DEFAULT (SET);

	dump_settings (SET);

	INIT_SETTINGS (SET);

	return;
}

void WRITE_SET_FILE (const vector <vector <string> >& SET, const string projectname) {

	ofstream o;

	o.open ((projectname + ".set").c_str());

	for (size_t i = 0; i < SET.size(); i++) o << SET.at(i).at(0) << '\t' << SET.at(i).at(1) << endl;
}

bool CHECK_SETTINGS (const string projectname) {

	if (!is_mode_GUI() && !is_mode_BATCH() && !is_mode_DEBUG()) ASSERT_DEAD_END();

	const string SFN = capslock (projectname + ".set");
	const string sfn = projectname + ".set";

	writeln ("");
	writeln ("============================");
	writeln ("1) CHECKING OF SETTINGS FILE");
	writeln ("============================");
	writeln ("");

	writeln (" - CHECKING " + SFN + " SETTINGS FILE");

	const bool CORR_SET = is_SETTINGS_FILE_CORRECT (sfn);

	if (CORR_SET) {

		writeln (" - " + SFN + " SETTINGS FILE IS CORRECT.");

		vector <vector <string> > SET = READ_SETTINGS_FILE (sfn);

		SET = COMPLETE_SET_WITH_DEFAULT (SET);

		INIT_SETTINGS (SET);

		return true;
	}
	else {

		writeln ("");
		writeln (" WARNING, SET_ERROR: the input " + SFN + "file structure is incorrect, the file will not be processed.");

		if (!CORR_SET && is_mode_GUI()) throw set_error();

		return false;
	}
}

void dump_settings (const vector <vector <string> >& SET) {

	writeln ("");
	writeln ("===============================");
	writeln ("3) SETTINGS USED FOR EVALUATION");
	writeln ("===============================");
	writeln ("");

	vector <vector <vector <string> > > DEF = RETURN_ALL_SETTINGS ();

	for (size_t i = 0; i < SET.size(); i++) {
		for (size_t j = 0; j < DEF.size(); j++) {
			for (size_t k = 1; k < DEF.at(j).size(); k++) {

				if (is_RECORD_CORRECT (SET.at(i), DEF, j, k)) {

					string actual_key = SET.at(i).at(0);
					string actual_value = SET.at(i).at(1);

					if (actual_key == "LINEWIDTH:") {

						cout << DEF.at(j).at(k).at(3) << flush;

						double LW = string_to_double(SET.at(i).at(1));
						cout << fixed << setprecision (1) << LW / 10.0 << flush;
						cout << DEF.at(j).at(k).at(4) << endl;
					}
					else if (actual_key == "STRESSANGLE:") {

						cout << DEF.at(j).at(k).at(3) << flush;
						cout << SET.at(i).at(1) << flush;
						cout << DEF.at(j).at(k).at(4) << endl;
					}
					else if (actual_key == "WELLINTERVAL_LENGTH:") {

						cout << DEF.at(j).at(k).at(3) << flush;
						cout << SET.at(i).at(1) << flush;
						cout << DEF.at(j).at(k).at(4) << endl;
					}
					else if (actual_key == "CLUSTERNUMBER:") {

						if (actual_value == "A" || actual_value == "N") cout << DEF.at(j).at(k).at(1) << endl;
						else {

							cout << DEF.at(j).at(k).at(3) << flush;
							cout << SET.at(i).at(1) << flush;
							cout << DEF.at(j).at(k).at(4) << endl;
						}
					}
					else cout << DEF.at(j).at(k).at(1) << endl;
				}
			}
		}
	}
	writeln ("");
	return;
}

void dbg_cout_settings_vector (const vector < vector <string> >& IN) {

	for (size_t i = 0; i < IN.size(); i++) {

		cout << IN.at(i).at(0) << '\t' << IN.at(i).at(1) << endl;
	}
}
