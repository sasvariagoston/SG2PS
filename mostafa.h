// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef MOSTAFA_H
#define MOSTAFA_H

#include "common.h"

#include "inversion.h"

using namespace std;

STRESSTENSOR st_MOSTAFA ();

STRESSFIELD sf_MOSTAFA (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);

#endif
