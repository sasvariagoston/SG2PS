// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef SHAN_H
#define SHAN_H

#include "common.h"

using namespace std;

vector <vector <double> > shan_matrix_from_GDB (const GDB& inGDB);

vector <vector <double> > SHAN_matrix (const vector <GDB>& inGDB);

STRESSTENSOR st_SHAN (const vector <GDB>& inGDB);

STRESSFIELD sf_SHAN (const STRESSTENSOR& st);

#endif
