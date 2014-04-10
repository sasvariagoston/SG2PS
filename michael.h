// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef MICHAEL_H
#define MICHAEL_H

#include "common.h"

using namespace std;

vector <vector < double> > michael_parameters (vector <GDB> inGDB);

vector <vector < double> > stressvector_parameters (vector <GDB> inGDB);

STRESSTENSOR st_MICHAEL (vector <GDB> inGDB, INPSET inset);

STRESSFIELD sf_MICHAEL (STRESSTENSOR st);

#endif
