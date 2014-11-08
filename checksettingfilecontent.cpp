// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cstdlib> // TODO Instead of atof, a string_to_int function should be used in common.cpp
#include <fstream>
#include <iomanip>
#include <iostream>

#include "array_to_vector.hpp"
#include "checksettingfilecontent.h"
#include "exceptions.hpp"
#include "generate_default_settings.hpp"
#include "run_mode.h"
#include "settings.hpp"

bool has_settings_file_data (string settingfilename) {

	ifstream inputfile (settingfilename.c_str());

	if (inputfile.eof() || !inputfile.is_open()) return false;

	return true;
}

vector < vector <string> > read_settingsfile_to_vector (string settingfilename) {

	vector < vector <string> > SET;

	if (!has_settings_file_data (settingfilename)) return SET;
	else {};

	ifstream inputfile (settingfilename.c_str());

	while (!inputfile.eof()) {

		string key, value;

		vector <string> buf;

		getline(inputfile, key, '\t');

		getline(inputfile, value);

        size_t len = value.size();

        if (len > 0 && value.at(len-1)=='\r') { // on Linux/Mac, reading files from Windows

            value.erase(len-1);
        }

		if (!is_whitecharacter(key) && !is_whitecharacter(value)) {

			buf.push_back(to_uppercase(key));

			buf.push_back(to_uppercase(value));

			SET.push_back(buf);
		}
		else {}
	}

	return SET;
}

bool is_whitecharacter (string in) {

	size_t length = in.size();

	if (length == 0) return true;
	else {};

	bool is_character_white = false;

	for (size_t i = 0; i < length; i++) if (in.at(i) == '\t' || in.at(i) == '\n' ) is_character_white = true;

	return is_character_white;
}

bool fit_of_records (const vector<string>& SETrecord, const vector<vector<vector<string> > >& DEF, size_t j, size_t k) {

	string actual_key = SETrecord.at(0);
	string list_key = DEF.at(j).at(0).at(0);

	if (actual_key != list_key) return false;

	string actual_value = capslock (SETrecord.at(1));
	string list_value = capslock (DEF.at(j).at(k).at(0));

	if (actual_key == "LINEWIDTH:" || actual_key == "STRESSANGLE:") {

		bool conv_has_failed = true;

		double min_value = string_to_double (DEF.at(j).at(k).at(1), conv_has_failed);
		double max_value = string_to_double (DEF.at(j).at(k).at(2), conv_has_failed);

		string_to_int (actual_value, conv_has_failed);

		if (actual_key == "LINEWIDTH:" && conv_has_failed) return false;

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

bool is_setting_record_correct (vector <string> SETrecord) {

	vector <vector <vector <string> > > DEF = return_default_settings_database ();

	for (size_t j = 0; j < DEF.size(); j++) {
		for (size_t k = 1; k < DEF.at(j).size(); k++) {

			if (fit_of_records (SETrecord, DEF, j, k)) return true;
		}
	}
	return false;
}

bool is_settings_file_correct (string settingfilename) {

	vector <vector <string> > SET = read_settingsfile_to_vector (settingfilename);

	if (SET.size() == 0) return false;

	for (size_t i = 0; i < SET.size(); i++) {

		if (!is_setting_record_correct (SET.at(i))) return false;
	}

	return true;
}

vector <vector <string> > input_hardcoded () {

	vector < vector < vector <string> > > DEF = return_default_settings_database ();

	vector < vector <string> > OUT;

	for (size_t i = 0; i < DEF.size(); i++) {

		vector <string> buf;

		buf.push_back(DEF.at(i).at(0).at(0));

		if      (DEF.at(i).at(0).at(0) == "LINEWIDTH:")   buf.push_back("6");
		else if (DEF.at(i).at(0).at(0) == "STRESSANGLE:") buf.push_back("30");
		else buf.push_back(DEF.at(i).at(1).at(0));

		OUT.push_back(buf);
	}
	return OUT;
}

vector <vector <string> > apply_default (vector <vector <string> > SET) {

	vector <vector <vector <string> > > DEF = return_default_settings_database ();

	vector <vector <string> > OUT;

	for (size_t i = 0; i < DEF.size(); i++) {

		string out_key   = DEF.at(i).at(0).at(0);
		string out_value = DEF.at(i).at(1).at(0);

		for (size_t j = 0; j < SET.size(); j++) if (SET.at(j).at(0) == out_key) out_value = SET.at(j).at(1);

		vector <string> buf;

		buf.push_back(out_key);
		buf.push_back(out_value);

		OUT.push_back(buf);
	}

	return OUT;
}

string input_setting_decision () {

	string c;

	while (!((c == "S") || (c == "D") || (c == "N"))) {

		cout << endl;
		cout << "Do you want to use and save these settings.....[S]," 			<< endl;
		cout << "use default values.............................[D]," 			<< endl;
		cout << "input new ones.................................[N]," 			<< endl;
		cout << "or exit........................................[X]........?  " << flush;

		cin >> c;
		c = capslock (c);

		if (c == "X") throw exit_requested();
	}
	return c;
}

vector <vector <string> > decide_setting_status (string projectname) {

	if (!is_mode_DEBUG()) cout << endl << endl;
	if (!is_mode_DEBUG()) cout << "CHECK SETTINGS OF '" <<  capslock(projectname) << "' PROJECT"<< endl;

	if (is_settings_file_correct (projectname + ".set")) {

		if (!is_mode_DEBUG()) cout << "  - Using '" << capslock (projectname) << ".SET' setting file." << endl;

		return read_settingsfile_to_vector (projectname + ".set");
	}
	else {

		if (is_settings_file_correct ("sg2ps.set")) {

			if (!is_mode_DEBUG()) cout << "  - No valid setting found, using 'SG2PS.SET' setting file." << endl;

			return read_settingsfile_to_vector ("sg2ps.set");
		}
		else {

			cout << "  - No valid setting found at all, uisng hard coded ones." << endl;

			return input_hardcoded ();
		}
	}
}

void list_settings_options (vector <vector <vector <string> > > DEF, size_t k) {

	bool is_clustering  = (DEF.at(k).at(0).at(0) == "CLUSTERNUMBER:");
	bool is_linewidth   = (DEF.at(k).at(0).at(0) == "LINEWIDTH:");
	bool is_stressangle = (DEF.at(k).at(0).at(0) == "STRESSANGLE:");

	if (is_clustering) {

		for (size_t i = 1; i < DEF.at(k).size() - 1; i++) cout << DEF.at(k).at(i).at(2) << endl;

		cout << DEF.at(k).at(DEF.at(k).size() - 1).at(5) << endl;
	}
	else if (is_stressangle || is_linewidth) {

		for (size_t i = 1; i < DEF.at(k).size(); i++) cout << DEF.at(k).at(i).at(5) << endl;
	}
	else {

		for (size_t i = 1; i < DEF.at(k).size(); i++) cout << DEF.at(k).at(i).at(2) << endl;
	}
}

vector <vector <string> > inputsettings_manually (string projectname) {

	vector <vector <string> > OUT;

	vector <vector <vector <string> > > DEF = return_default_settings_database ();

	cout << endl << "INPUT USER SETTINGS - to exit: press [X]" << endl;

	for (size_t i = 0; i < DEF.size(); i++) {

		vector <string> buf;

		do {

			string c;

			buf.clear();

			list_settings_options (DEF, i);

			cin >> c;

			c = capslock(c);

			if (c == "X") throw exit_requested();

			buf.push_back(DEF.at(i).at(0).at(0));
			buf.push_back(c);

		} while (!is_setting_record_correct (buf));

		OUT.push_back(buf);
	}

	cout << endl << endl;
	cout << "NEW SETTINGS FOR '" << capslock (projectname) << "' PROJECT"<< endl;

	return OUT;

}

void manage_settings_batch (string projectname) {

	vector <vector <string> > SET = decide_setting_status (projectname);

	SET = apply_default(SET);

	if (!is_mode_DEBUG()) dump_actual_settings(SET);

	INIT_SETTINGS (SET);

	return;
}

void manage_settings_nobatch (string projectname) {

	vector <vector <string> > SET = decide_setting_status (projectname);

	SET = apply_default(SET);

	string c = "";

	do {

		SET = apply_default(SET);

		dump_actual_settings(SET);

		c = input_setting_decision ();

		if (c == "D") SET = input_hardcoded ();
		else if (c == "N") SET = inputsettings_manually (projectname);
		else {}

	} while (!(c == "S"));

	outputsettingfile (SET, projectname);

	INIT_SETTINGS (SET);

	return;
}

void outputsettingfile (vector <vector <string> > SET, string projectname) {

	ofstream o;

	o.open ((projectname + ".set").c_str());

	for (size_t i = 0; i < SET.size(); i++) {

		o << SET.at(i).at(0) << '\t' << SET.at(i).at(1) << endl;
	}
}

void dump_actual_settings (vector <vector <string> >SET) {

	vector <vector <vector <string> > > DEF = return_default_settings_database ();

	for (size_t i = 0; i < SET.size(); i++) {
		for (size_t j = 0; j < DEF.size(); j++) {
			for (size_t k = 1; k < DEF.at(j).size(); k++) {

				if (fit_of_records (SET.at(i), DEF, j, k)) {

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
}

void dbg_cout_settings_vector (vector < vector <string> > IN) {

	for (size_t i = 0; i < IN.size(); i++) {

		cout << IN.at(i).at(0) << '\t' << IN.at(i).at(1) << endl;
	}
}
