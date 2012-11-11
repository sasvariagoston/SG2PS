// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef ROSE_HPP_
#define ROSE_HPP_

#include <vector>

#include "structs.h"
#include "common.h"

ROSENUMBER compute_data_number_DIPDIR (vector <GDB> inGDB, double strike_begin, double strike_end);
ROSENUMBER compute_data_number_DIP (vector <GDB> inGDB, double strike_begin, double strike_end);

void PS_draw_rose_PLANE (GDB inGDB, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical);
void PS_draw_rose_LINEATION (GDB inGDB, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical);
void PS_draw_rose_SC_STRIAE (GDB inGDB, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical);

void PS_draw_rose_DIPDIR (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);
void PS_draw_rose_DIP (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);

void PS_draw_rose (vector <GDB> roseGDB, vector <GDB> tiltroseGDB, ofstream& o, INPSET inset, CENTER center, PAPER P);

#endif
