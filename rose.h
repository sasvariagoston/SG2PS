// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef ROSE_HPP_
#define ROSE_HPP_

#include "common.h"

ROSENUMBER compute_data_number_DIPDIR_DIP (vector <GDB> inGDB, double strike_begin, double strike_end, string DIPDIR_DIP);

void PS_draw_rose_DATATYPE (vector <GDB> inGBD, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical);

void PS_draw_rose_DIPDIR (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);
void PS_draw_rose_DIP (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);

void PS_draw_rose (vector <GDB> roseGDB, ofstream& o, INPSET inset, CENTER center, PAPER P, bool tilt);

#endif
