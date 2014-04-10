// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef FOLDSURFACE_H
#define FOLDSURFACE_H

#include <vector>

#include "common.h"
#include "structs.h"

using namespace std;

void plot_fold_great_circle (VCTR GR_CRC, ofstream& o, INPSET inset, CENTER center);

void calculate_foldsurface (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);

#endif
