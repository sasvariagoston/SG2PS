
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef BINGHAM_H
#define BINGHAM_H

#include "common.h"

using namespace std;

vector <VCTR> generate_Bingham_dataset (const vector <GDB>& inGDB);

STRESSTENSOR st_BINGHAM (const vector <VCTR>& N);

STRESSFIELD sf_BINGHAM (const STRESSTENSOR& st);

#endif
