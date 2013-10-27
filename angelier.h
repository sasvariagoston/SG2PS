// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef ANGELIER_HPP_
#define ANGELIER_HPP_

#include "common.h"

#include "inversion.h"

using namespace std;

ANG_PRM angelier_parameters (vector <GDB> inGDB);

STRESSTENSOR compute_angelier_stresstensor (ANG_PRM p, vector <GDB> inGDB);

STRESSTENSOR st_ANGELIER (vector <GDB> inGDB, INPSET inset);

STRESSFIELD sf_ANGELIER (STRESSTENSOR st);

#endif
