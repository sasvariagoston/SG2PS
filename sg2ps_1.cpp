
// Copyright (C) 2012-2015, Ágoston Sasvári
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
#include "trajectory.hpp"

using namespace std;

void real_main(int argument_number, char *argv[]) {

	vector <string> ARG_V = vector<string>(argv+1, argv+argument_number);

    MANAGE_RUN_MODE (ARG_V);

	print_banner ();

	ARG_V = REMOVE_FIRST_ARGUMENT (ARG_V);

	CHECK_ARGUMENTS_NUMBER (ARG_V);

	clock_t starttime = clock ();

	for (size_t i = 0; i < ARG_V.size() ; i++) {

		const string IF = ARG_V.at(i);

		const bool SET_OK = CHECK_SETTINGS (ARG_V.at(i));

		if (SET_OK) {

			const bool RGF_OK = CHECK_RGF (ARG_V.at(i));

			if (RGF_OK) {

				LOAD_SETTINGS (IF);

				CHECK_XY_FILE (IF);

				CHECK_TRAJECTORY_FILE (IF);

				// FIXME Errors are ignored!

				PROCESS_RGF (IF);

				if (!is_mode_DEBUG()) writeln ("EVALUATION OF " + capslock(IF) + ".RGF FILE COMPLETED.");
			}
		}
	}
	clock_t finishtime = clock();

	output_elapsed_time (starttime, finishtime);
}
