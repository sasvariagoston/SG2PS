// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef NDA_H
#define NDA_H

#include "common.h"

using namespace std;

STRESSTENSOR st_NDA (const vector <GDB>& inGDB);

STRESSFIELD sf_NDA (const STRESSTENSOR& st);

#endif
