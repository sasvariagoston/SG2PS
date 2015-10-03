
// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef ANGELIER_HPP_
#define ANGELIER_HPP_

#include "common.h"

#include "inversion.h"

using namespace std;

ANG_PRM angelier_parameters (const vector <GDB>& inGDB);

STRESSTENSOR compute_angelier_stresstensor (const ANG_PRM& p, const vector <GDB>& inGDB);

STRESSTENSOR st_ANGELIER (const vector <GDB>& inGDB);

STRESSFIELD sf_ANGELIER (const STRESSTENSOR& st);

#endif
