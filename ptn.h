// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef PTN_H
#define PTN_H

#include "common.h"
#include "structs.h"

using namespace std;

STRESSTENSOR PTN_matrix (vector <GDB> inGDB, string axis);

STRESSTENSOR st_PTN (STRESSFIELD sf);

STRESSFIELD sf_PTN (vector <GDB> inGDB, INPSET inset);

#endif
