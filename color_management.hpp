// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef COLOR_MANAGEMENT_HPP_
#define COLOR_MANAGEMENT_HPP_

#include <vector>

#include "structs.h"

string pastel (const string COLOR);

vector <GDB> attach_k_means_group_codes (const vector <size_t>& which_group, const vector <GDB>& inGDB);

vector <vector <GDB> > associate_empty_clustercode (const vector <vector <GDB> >& inGDB_G, const size_t WHICH);

string generate_PSCOLOR_from_GC (const string GC);
string generate_PSCOLOR_from_COLOR (const string CLR);
string generate_DASH (const string CODE);

vector <GDB> GENERATE_PS_CODE (const vector <GDB>& inGDB);

#endif
