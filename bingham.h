// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef BINGHAM_H
#define BINGHAM_H

#include "common.h"

using namespace std;

STRESSTENSOR st_BINGHAM (vector <GDB> inGDB);

STRESSFIELD sf_BINGHAM (STRESSTENSOR st);

#endif
