// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef MICHAEL_H
#define MICHAEL_H

#include "common.h"

using namespace std;

vector <vector <double> > michael_parameters (const vector <GDB>& inGDB);

vector <vector <double> > stressvector_parameters (const vector <GDB>& inGDB);

STRESSTENSOR st_MICHAEL (const vector <GDB>& inGDB);

STRESSFIELD sf_MICHAEL (const STRESSTENSOR& st);

#endif
