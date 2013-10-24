// Copyright (C) 2012, 2013 Ágoston Sasvári
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

		if (!is_whitecharacter(key) && !is_whitecharacter(value)) {

			buf.push_back(key);

			buf.push_back(value);

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

bool fit_of_records (vector  <string> SETrecord, size_t j, size_t k) {

	vector < vector < vector <string> > > DEF = return_default_settings_database ();

	string actual_key = SETrecord.at(0);
	string list_key = DEF.at(j).at(0).at(0);

	if (actual_key != list_key) return false;

	string actual_value = capslock (SETrecord.at(1));
	string list_value = capslock (DEF.at(j).at(k).at(0));

	if (actual_key == "LINEWIDTH:" || actual_key == "STRESSANGLE:") {

		bool conv_has_failed = true;

		double min_value = string_to_double (DEF.at(j).at(k).at(1), conv_has_failed);
		double max_value = string_to_double (DEF.at(j).at(k).at(2), conv_has_failed);

		int int_value = string_to_int (actual_value, conv_has_failed);

		if (actual_key == "LINEWIDTH:" && conv_has_failed) return false;

		double value = string_to_double (actual_value, conv_has_failed);

		//if (actual_key == "LINEWIDTH:") value = value / 10.0;

		//cout << fixed << setprecision(3) << endl;

		//cout << "FIT_OF_RECORDS" << endl;
		//cout << actual_value << " converted to " << value << " and the conversion " << flush;
		//if (!conv_has_failed) cout << " is successfull" << endl;
		//else cout << " has failed" << endl;

		//cout << min_value << '\t' << max_value << endl;

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

			//cout << "FIT_OF_RECORDS" << endl;
			//cout << actual_value << " converted to " << value << " and the conversion is " << flush;
			//if (!conv_has_failed) cout << " is successfull" << endl;
			//else cout << " has failed" << endl;

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

			if (fit_of_records (SETrecord, j, k)) return true;
		}
	}

	return false;
}

bool is_settings_file_correct (string settingfilename) {

	vector <vector <string> > SET = read_settingsfile_to_vector (settingfilename);

	if (SET.size() == 0) return false;

	for (size_t i = 0; i < SET.size(); i++) {

		if (!is_setting_record_correct (SET.at(i))) {

			//cout << i << " " << flush;

			//cout << SET.at(i).at(0) << " " << flush;
			//cout << SET.at(i).at(1) << " " << flush;

			//cout << " IS INCORRECT."<< endl;
		}

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

vector <vector <string> > settings_to_vector (INPSET inset) {

	vector <vector <string> > out;

	for (size_t i = 0; i < 19; i++) {

		vector <string> rec;

		string buf = "";

		rec.push_back(buf);
		rec.push_back(buf);

		out.push_back(rec);
	}

	out.at( 0).at(0) = "DATARULE:";			out.at( 0).at(1) = inset.datarule;
	out.at( 1).at(0) = "PLOT:";				out.at( 1).at(1) = inset.plot;
	out.at( 2).at(0) = "PLOTTYPE:";			out.at( 2).at(1) = inset.plottype;
	out.at( 3).at(0) = "HEMISPHERE:";		out.at( 3).at(1) = inset.hemisphere;
	out.at( 4).at(0) = "TILTING:";			out.at( 4).at(1) = inset.tilting;
	out.at( 5).at(0) = "GROUP:";			out.at( 5).at(1) = inset.group;
	out.at( 6).at(0) = "CLUSTERNUMBER:";	out.at( 6).at(1) = inset.clusternumber;
	out.at( 7).at(0) = "LABELING:";			out.at( 7).at(1) = inset.labeling;
	out.at( 8).at(0) = "INVERSION:";		out.at( 8).at(1) = inset.inversion;
	out.at( 9).at(0) = "RUP_CLUSTERING:";	out.at( 9).at(1) = inset.clustering_RUP_ANG;
	out.at(10).at(0) = "VIRTUAL:";			out.at(10).at(1) = inset.virt_striae;
	out.at(11).at(0) = "IDEALMOVEMENT:";	out.at(11).at(1) = inset.idealmovement;
	out.at(12).at(0) = "STRESSANGLE:";		out.at(12).at(1) = double_to_string(inset.angle, 1); // double here
	out.at(13).at(0) = "BINGHAM:";			out.at(13).at(1) = inset.fracture;
	out.at(14).at(0) = "LINEWIDTH:";		out.at(14).at(1) = double_to_string(inset.linewidth, 1);// double here
	out.at(15).at(0) = "ROSETYPE:";			out.at(15).at(1) = inset.rosetype;
	out.at(16).at(0) = "ROSEDIRECTION:";	out.at(16).at(1) = inset.rosedirection;
	out.at(17).at(0) = "ROSEBINNING:";		out.at(17).at(1) = inset.rosebinning;
	out.at(18).at(0) = "CONTOURING:";		out.at(18).at(1) = inset.contouring;
	out.at(19).at(0) = "GRAYSCALE:";		out.at(19).at(1) = inset.grayscale;


	return out;
}

INPSET vector_to_settings (vector <vector <string> > SET) {

	INPSET inset;

	bool failed;

	for (size_t i = 0; i < SET.size(); i++) {

		if 		(SET.at(i).at(0) == "DATARULE:") 		inset.datarule 				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "PLOT:")			inset.plot 					= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "PLOTTYPE:")		inset.plottype 				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "HEMISPHERE:")		inset.hemisphere 			= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "TILTING:")			inset.tilting 				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "GROUP:")			inset.group 				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "CLUSTERNUMBER:")	inset.clusternumber 		= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "LABELING:")		inset.labeling 				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "INVERSION:")		inset.inversion				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "RUP_CLUSTERING:")	inset.clustering_RUP_ANG	= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "VIRTUAL:")			inset.virt_striae			= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "IDEALMOVEMENT:")	inset.idealmovement			= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "STRESSANGLE:")		inset.angle					= string_to_double(SET.at(i).at(1), failed);
		else if (SET.at(i).at(0) == "BINGHAM:")			inset.fracture				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "LINEWIDTH:")		inset.linewidth				= string_to_double(SET.at(i).at(1), failed) / 10.0;
		else if (SET.at(i).at(0) == "ROSETYPE:")		inset.rosetype				= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "ROSEDIRECTION:")	inset.rosedirection			= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "ROSEBINNING:")		inset.rosebinning			= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "CONTOURING:")		inset.contouring			= SET.at(i).at(1);
		else if (SET.at(i).at(0) == "GRAYSCALE:")		inset.grayscale				= SET.at(i).at(1);
		else {}
	}

	return inset;
}

string input_setting_decision () {

	string c;

	while (!((c == "S") || (c == "D") || (c == "N"))) {

		cout << endl;
		cout << "Do You want to use and save these settings.....[S]," 			<< endl;
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

	cout << endl << endl;
	cout << "CHECK SETTINGS OF '" <<  capslock(projectname) << "' PROJECT"<< endl;

	if (is_settings_file_correct (projectname + ".set")) {

		cout << "  - Using '" << capslock (projectname) << ".SET' setting file." << endl;

		return read_settingsfile_to_vector (projectname + ".set");
	}
	else {

		//cout << " +++ +++ INCORRECT SETTINGDS FILE +++ +++" << endl;

		if (is_settings_file_correct ("sg2ps.set")) {

			cout << "  - No valid setting found, using 'SG2PS.SET' setting file." << endl;

			return read_settingsfile_to_vector ("sg2ps.set");
		}
		else {

			cout << "  - No valid setting found at all, uisng hard coded ones." << endl;

			return input_hardcoded ();
		}
	}
}

void list_settings_options (vector <vector <vector <string> > > DEF, size_t k) {

	//cout << DEF.at(k).at(0).at(1) << endl;

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

INPSET manage_settings_batch (string projectname) {

	vector <vector <string> > SET = decide_setting_status (projectname);

	SET = apply_default(SET);

	dump_actual_settings(SET);

	return vector_to_settings(SET);
}

INPSET manage_settings_nobatch (string projectname) {

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

	return vector_to_settings(SET);
}

void outputsettingfile (vector <vector <string> > SET, string projectname) {

	ofstream o;

	o.open ((projectname + ".set").c_str());

	for (size_t i = 0; i < SET.size(); i++) {

		o << SET.at(i).at(0) << '\t' << SET.at(i).at(1) << endl;
	}
}

void dump_actual_settings (vector <vector <string> >SET) {

	//dbg_generate_settings_file_list ();

	vector <vector <vector <string> > > DEF = return_default_settings_database ();

	for (size_t i = 0; i < SET.size(); i++) {
		for (size_t j = 0; j < DEF.size(); j++) {
			for (size_t k = 1; k < DEF.at(j).size(); k++) {

				//	cout << "i: " << i << "j: " << j << "k: " << k << endl;

				if (fit_of_records (SET.at(i), j, k)) {

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

void dbg_cout_inpset (INPSET inset) {

	cout << "inset.datarule" << '\t' << inset.datarule << endl;
	cout << "inset.plot" << '\t' << inset.plot << endl;
	cout << "inset.plottype" << '\t' << inset.plottype << endl;
	cout << "inset.hemisphere" << '\t' << inset.hemisphere << endl;
	cout << "inset.tilting" << '\t' << inset.tilting << endl;
	cout << "inset.group" << '\t' << inset.group << endl;
	cout << "inset.clusternumber" << '\t' << inset.clusternumber << endl;
	cout << "inset.labeling" << '\t' << inset.labeling << endl;
	cout << "inset.inversion" << '\t' << inset.inversion << endl;
	cout << "inset.clustering_RUP_ANG" << '\t' << inset.clustering_RUP_ANG << endl;
	cout << "inset.virt_striae" << '\t' << inset.virt_striae << endl;
	cout << "inset.idealmovement" << '\t' << inset.idealmovement << endl;
	cout << "inset.angle" << '\t' << inset.angle << endl;
	cout << "inset.fracture" << '\t' << inset.fracture << endl;
	cout << "inset.linewidth" << '\t' << inset.linewidth << endl;
	cout << "inset.rosetype" << '\t' << inset.rosetype << endl;
	cout << "inset.rosebinning" << '\t' << inset.rosebinning << endl;
	cout << "inset.contouring" << '\t' << inset.contouring << endl;
	cout << "inset.grayscale" << '\t' << inset.grayscale << endl;
}
