// Copyright (C) 2012-2016,  Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

using namespace std;

#include "allowed_keys.hpp"
#include "exceptions.hpp"
#include "generate_default_settings.hpp"
#include "run_mode.h"

namespace {

bool M_STD = false;
bool M_DBG = false;
bool M_VER = false;
bool M_VID = false;
bool M_TST = false;
}

bool is_mode_STD () {

	return M_STD;
}

bool is_mode_DEBUG () {

	return M_DBG;
}

bool is_mode_VERSION () {

	return M_VER;
}

bool is_mode_VERSION_ID () {

	return M_VID;
}

bool is_mode_GENERATE_TEST_FILES () {

	return M_TST;
}

void init_run_mode () {

	M_STD = false;
	M_DBG = false;
	M_VER = false;
	M_VID = false;
	M_TST = false;
}

void setup_run_mode (const vector <string>&  ARG_V) {

	init_run_mode ();

	string MODE = "";

	const string A = capslock (ARG_V.at(0));

	if (is_allowed_debug_mode (A) && ARG_V.size() == 2) 	{

		M_DBG = true;
		MODE = "DEBUG";
	}
	else if (is_allowed_version_id_mode (A)) {

		M_VID = true;
		MODE = "VERSION ID";
	}
	else if (is_allowed_version_mode (A)) {
		M_VER = true;
		MODE = "VERSION";
	}
	else if (is_allowed_test_data_generation_mode(A) && ARG_V.size() == 2) {
		M_TST = true;
		MODE = "TEST FILE GENERATION";
	}
	else if (ARG_V.size() == 1) {

		M_STD = true;
		MODE = "STANDARD";
	}
	else throw arg_error ();
	if (!M_VER && !M_VID) cout << "Running 'SG2PS' in '" << MODE << "' mode." << endl;

	return;
}

void MANAGE_RUN_MODE (const vector <string>& ARG_V) {

	setup_run_mode (ARG_V);

	if (is_mode_GENERATE_TEST_FILES()) {

		if (ARG_V.size() == 1) throw arg_error ();

		dbg_generate_settings_file_list (ARG_V.at(1));
		throw exit_requested ();
	}

	if (is_mode_VERSION()) {

		cout << version() << endl;
		throw exit_requested ();
	}

	if (is_mode_VERSION_ID()) {

		cout << version_id() << endl;
		throw exit_requested ();
	}
	return;
}
