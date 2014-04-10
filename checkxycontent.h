// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef CHECKXYCONTENT_HPP_
#define CHECKXYCONTENT_HPP_

#include "structs.h"

using namespace std;

bool needxyfile ();

string inputxyfilename();

void read_in_xy(const string& file_name);
bool input_xy (const string& projectname);

bool LOCATIONcheck ();
bool LOCATIONcheck_duplicate ();
bool XYcheck ();

bool xyfile_correct (string projectname);

void uppercase_xy_to_check ();
string check_xy_inputs (string inputfilename);

GDB insertxy (GDB inGDB);

#endif
