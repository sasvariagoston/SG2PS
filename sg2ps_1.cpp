
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

void real_main(int argument_number, char *argv[]) {

	vector <string> ARG_V = vector<string>(argv+1, argv+argument_number);

	vector <vector <string> > SET;

	vector <string> ARG_V_CORRECT;

	bool using_xy_files = true;


	MANAGE_RUN_MODE (ARG_V);

	ARG_V = REMOVE_FIRST_ARGUMENT (ARG_V);

	CHECK_ARGUMENTS_NUMBER (ARG_V);

	print_banner ();


	if (is_mode_COMMANDLINE()) {

		ARG_V.push_back (inputfilename());

		using_xy_files = needxyfile ();

		load_settings_cmd (ARG_V.at(0));

		ARG_V_CORRECT = RETURN_CORRECT_RGF_CMD (ARG_V);
	}
	else ARG_V_CORRECT = check_rgf_set_inputs (ARG_V);


	clock_t starttime = clock ();

	string xy_filename;


	for (size_t i = 0; i < ARG_V_CORRECT.size() ; i++) {

		const string IF = ARG_V_CORRECT.at(i);

		load_settings_batch (IF);

		if (using_xy_files) xy_filename = check_xy_inputs (IF);

		process_rgf (IF, xy_filename); // TODO Reads in the rgf file again, was first checked in check_rgf_inputs

		if (!is_mode_DEBUG()) cout << "EVALUATION OF " << capslock(IF) << ".RGF FILE COMPLETED." << endl;
	}
	clock_t finishtime = clock();

	output_elapsed_time (starttime, finishtime);
}
