// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef ROSE_HPP_
#define ROSE_HPP_

#include "common.h"

ROSENUMBER compute_data_number_DIPDIR_DIP (const vector <GDB>& inGDB, const double strike_begin, const double strike_end, const string DIPDIR_DIP); //ok

void PS_draw_rose_DATATYPE (const vector <GDB>& inGBD, ofstream& o, const CENTER& center, const ROSENUMBER& percent, const double begin_angle, const bool vertical); //ok

bool process_as_lineation (const vector <GDB>& inGDB);
bool process_as_plane (const vector <GDB>& inGDB);

void PS_draw_rose_DIPDIR_DIP (const vector <vector <GDB> >& inGDB_G, ofstream& o, CENTER center, const string MODE, const bool TILT);

#endif
