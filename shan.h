// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef SHAN_H
#define SHAN_H

#include "common.h"

using namespace std;

vector <vector <double> > shan_matrix_from_GDB (const GDB& inGDB); //ok

vector <vector <double> > SHAN_matrix (const vector <GDB>& inGDB); //ok

STRESSTENSOR st_SHAN (const vector <GDB>& inGDB); //ok

STRESSFIELD sf_SHAN (const STRESSTENSOR& st); //ok

#endif
