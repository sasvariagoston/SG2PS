// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <string>
#include <ctime>

#include "exceptions.hpp"
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

	if (ARG_V.size() < 1 || ARG_V.size() > 2) throw arg_error();

	MANAGE_RUN_MODE (ARG_V);

	print_banner ();

	string FILE_TO_EVAL = ARG_V.at(0);
	if (! is_mode_STD()) FILE_TO_EVAL = ARG_V.at(1);

	clock_t starttime = clock ();

	CHECK_SETTINGS (FILE_TO_EVAL);

	CHECK_RGF (FILE_TO_EVAL);

	LOAD_SETTINGS (FILE_TO_EVAL);

	const bool XY_OK = CHECK_XY_FILE (FILE_TO_EVAL);

	const bool TRJ_OK = CHECK_TRAJECTORY_FILE (FILE_TO_EVAL);

	PROCESS_RGF (FILE_TO_EVAL, XY_OK, TRJ_OK);

	if (!is_mode_DEBUG()) writeln ("EVALUATION OF " + capslock(FILE_TO_EVAL) + ".RGF FILE COMPLETED.");
	clock_t finishtime = clock();

	output_elapsed_time (starttime, finishtime);
}
