// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef COLOR_MANAGEMENT_HPP_
#define COLOR_MANAGEMENT_HPP_

#include <vector>

#include "structs.h"

bool existence_of_group_GDB (string expression, vector <GDB> inGDB);
bool existence_of_groupcodes (vector <GDB> inGDB);

//vector <GDB> attach_color_codes (vector <int> which_group, vector <GDB> inGDB);
vector <GDB> attach_group_codes (vector <int> which_group, vector <GDB> inGDB);

string generate_PSCOLOR_from_GC (string GC, INPSET inset);
string generate_PSCOLOR_from_COLOR (string CLR, INPSET inset);
string generate_DASHED_from_GC (string GC, INPSET inset);

vector <GDB> fill_with_black_colorcode (vector <GDB> inGDB);
vector <GDB> colorcode_grom_groupcode (vector <GDB> inGDB, INPSET inset);

#endif
