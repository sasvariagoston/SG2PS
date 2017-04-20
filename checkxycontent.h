// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef CHECKXYCONTENT_HPP_
#define CHECKXYCONTENT_HPP_

#include "structs.h"

using namespace std;

void read_in_xy (const string& file_name);
bool input_xy (const string& projectname);

bool LOCATIONcheck ();
bool LOCATIONcheck_duplicate ();
bool XYcheck ();

vector <GDB> insert_xy_values (const vector <GDB>& inGDB);
bool CHECK_XY_FILE (const string projectname);

#endif
