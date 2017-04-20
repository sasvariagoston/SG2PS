// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef PTN_H
#define PTN_H

#include "common.h"
#include "structs.h"

using namespace std;

vector <GDB> ptn (const vector <GDB>& inGDB);

STRESSTENSOR PTN_matrix (const vector <GDB>& inGDB, const string& axis);

STRESSFIELD sf_PTN (const vector <GDB>& inGDB);

STRESSTENSOR st_PTN (const STRESSFIELD& sf, const vector <GDB> inGD);

#endif
