// Copyright (C) 2012 - 20143 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

using namespace std;

#include <vector>
#include <iostream>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "exceptions.hpp"

namespace {

bool M_GUI = false;
bool M_BTC = false;
bool M_CMD = false;
bool M_DBG = false;
bool M_VER = false;
bool M_VID = false;
bool M_TST = false;
};

bool is_mode_GUI () {

	return M_GUI;
}

bool is_mode_BATCH () {

	return M_BTC;
}

bool is_mode_COMMANDLINE () {

	return M_CMD;
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

void setup_run_mode (const vector <string>& ARG_V) {

	M_GUI = false;
	M_BTC = false;
	M_CMD = false;
	M_DBG = false;
	M_VER = false;
	M_VID = false;
	M_TST = false;

	string MODE = "";

	if (ARG_V.size() < 1) {

		M_CMD = true;
		MODE = "COMMAND LINE";
	}
	else {

		const string ARG = capslock (ARG_V.at(0));

		if (is_allowed_debug_mode (ARG)) 	{

			M_DBG = true;
			MODE = "DEBUG";
		}
		else if (is_allowed_gui_mode(ARG)) 	{

			M_GUI = true;
			MODE = "GUI";
		}
		else if (is_allowed_version_id_mode(ARG)) {

			M_VID = true;
			MODE = "VERSION ID";
			cout << version_id() << endl;
		}
		else if (is_allowed_version_mode(ARG)) {
			M_VER = true;
			MODE = "VERSION";
			cout << version() << endl;
		}
		else if (is_allowed_test_data_generation_mode(ARG)) {
			M_TST = true;
			MODE = "TEST FILE GENERATION";
		}
		else {
			M_BTC = true;
			MODE = "BATCH";
		}
	}
	if (!M_GUI && !M_BTC && !M_CMD && !M_DBG && !M_VER && !M_VID && !M_TST) ASSERT_DEAD_END();

	return;
}

size_t return_min_argument_number () {

	if 		(M_GUI) return 1;
	else if (M_BTC) return 1;
	else if (M_CMD) return 0;
	else if (M_DBG) return 1;
	else if (M_VER) return 0;
	else if (M_VID) return 0;
	else if (M_TST) return 0;
	else {
		ASSERT_DEAD_END();
		return 999;
	}
}

size_t return_max_argument_number () {

	if 		(M_GUI) return 1;
	else if (M_BTC) return 999;
	else if (M_CMD) return 0;
	else if (M_DBG) return 999;
	else if (M_VER) return 999;
	else if (M_VID) return 999;
	else if (M_TST) return 999;
	else {
		ASSERT_DEAD_END();
		return 999;
	}
}

void check_arguments_number (const vector <string>& ARG_V) {

	const size_t ARG_NUM = ARG_V.size();

	const size_t ARG_MIN = return_min_argument_number();
	const size_t ARG_MAX = return_max_argument_number();

	if (!(ARG_MIN <= ARG_NUM && ARG_MAX >= ARG_NUM)) throw arg_error ();
	return;
}
