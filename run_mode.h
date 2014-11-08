// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef RUN_MODE_HPP_
#define RUN_MODE_HPP_

using namespace std;

bool is_mode_GUI ();
bool is_mode_BATCH ();
bool is_mode_COMMANDLINE ();
bool is_mode_DEBUG ();
bool is_mode_VERSION ();
bool is_mode_VERSION_ID ();
bool is_mode_GENERATE_TEST_FILES ();

void setup_run_mode (const vector <string>& ARG_V);

size_t return_min_argument_number ();
size_t return_max_argument_number ();

void check_arguments_number (const vector <string>& ARG_V);

#endif
