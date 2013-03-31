// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef FRY_H
#define FRY_H

#include "common.h"

using namespace std;

vector <vector <double> > FRY_matrix (vector <GDB> inGDB, INPSET inset);

bool fry_correct (vector <GDB> inGDB, INPSET inset);

STRESSTENSOR st_FRY (vector <GDB> inGDB);

STRESSFIELD sf_FRY (STRESSTENSOR st);

#endif
