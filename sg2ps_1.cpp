// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <string>
#include <ctime>

#include "rgf.h"
#include "checksettingfilecontent.h"
#include "checkrgffilecontent.h"
#include "checkxycontent.h"
#include "generate_default_settings.hpp"
#include "run_mode.h"

using namespace std;

//void print_banner();

void real_main(int argument_number, char *argv[]) {

	vector <string> ARG_V = vector<string>(argv+1, argv+argument_number);

	bool using_xy_files = false;

	setup_run_mode (ARG_V);

	check_arguments_number (ARG_V);

	dbg_generate_settings_file_list ();

	if (is_mode_GUI () || is_mode_DEBUG ()) ARG_V.erase(ARG_V.begin());

	print_banner();

	if (is_mode_COMMANDLINE()) ARG_V.push_back (inputfilename());

	ARG_V = check_rgf_inputs (ARG_V);

	if (is_mode_COMMANDLINE()) using_xy_files = needxyfile ();
	else using_xy_files = true;

	if (is_mode_COMMANDLINE()) manage_settings_nobatch (ARG_V.at(0));

	clock_t starttime = clock ();

	string xy_filename;

	for (size_t j = 0; j < ARG_V.size() ; j++) {

		manage_settings_batch (ARG_V.at(j));

		if (using_xy_files) xy_filename = check_xy_inputs (ARG_V.at(j));

		process_rgf (ARG_V.at(j), xy_filename, is_mode_DEBUG()); // TODO Reads in the rgf file again, was first checked in check_rgf_inputs

		if (!is_mode_DEBUG()) cout << "EVALUATION OF " << capslock(ARG_V.at(j)) << ".RGF FILE COMPLETED." << endl;
	}
	clock_t finishtime = clock();

	output_elapsed_time (starttime, finishtime);
}
