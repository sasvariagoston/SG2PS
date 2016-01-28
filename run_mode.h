// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef RUN_MODE_HPP_
#define RUN_MODE_HPP_

using namespace std;

bool is_mode_STD ();
bool is_mode_DEBUG ();
bool is_mode_VERSION ();
bool is_mode_VERSION_ID ();
bool is_mode_GENERATE_TEST_FILES ();

void init_run_mode ();
void setup_run_mode (const vector <string>&  ARG);

void MANAGE_RUN_MODE (const vector <string>&  ARG) ;

#endif
