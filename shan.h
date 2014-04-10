// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef SHAN_H
#define SHAN_H

#include "common.h"

using namespace std;

vector <vector <double> > SHAN_matrix (vector <GDB> inGDB, INPSET inset);

STRESSTENSOR st_SHAN (vector <GDB> inGDB, INPSET inset);

STRESSFIELD sf_SHAN (STRESSTENSOR st);

#endif
