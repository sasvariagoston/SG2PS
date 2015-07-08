
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef ROSE_HPP_
#define ROSE_HPP_

#include "common.h"

ROSENUMBER compute_data_number_DIPDIR_DIP (const vector <GDB>& inGDB, const double strike_begin, const double strike_end, const string DIPDIR_DIP); //ok

void PS_draw_rose_DATATYPE (const vector <GDB>& inGBD, ofstream& o, const CENTER& center, const ROSENUMBER& percent, const double begin_angle, const bool vertical); //ok

void PS_draw_rose_DIPDIR_DIP (vector <GDB> inGDB, ofstream& o, CENTER center, const string MODE, const bool TILT);

//void PS_draw_rose (const vector <GDB>& roseGDB, ofstream& o, const CENTER center, const bool tilt);//ok

#endif
