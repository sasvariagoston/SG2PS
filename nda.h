// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef NDA_H
#define NDA_H

#include "common.h"

using namespace std;

STRESSTENSOR st_NDA (vector <GDB> inGDB, INPSET inset);

STRESSFIELD sf_NDA (STRESSTENSOR st);

#endif
