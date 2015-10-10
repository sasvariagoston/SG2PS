// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef TRAJECTORY_HPP_
#define TRAJECTORY_HPP_

#include "structs.h"

vector <TRJ> return_TRAJECTORY ();

bool CHECK_TRAJECTORY_FILE (const string projectname);

vector <GDB> APPLY_TRAJECTORY (const vector <GDB>& inGDB);

void dbg_TRAJECTORY ();
void dbg_TRAJECTORY_N ();

#endif /* TRAJECTORY_HPP_ */
