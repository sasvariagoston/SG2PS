// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef ANGELIER_HPP_
#define ANGELIER_HPP_

#include <string>
#include <vector>
#include <cmath>

#include "structs.h"
#include "common.h"


using namespace std;

vector <vector < double> > michael_parameters (vector <GDB> inGDB);
vector <vector < double> > stressvector_parameters (vector <GDB> inGDB);

ANG_PRM angelier_parameters (vector <GDB> inGDB);
STRESSTENSOR compute_angelier_stresstensor (ANG_PRM parameters, vector <GDB> inGDB);

STRESSTENSOR ptn_P (vector <GDB> inGDB);
STRESSTENSOR ptn_T (vector <GDB> inGDB);
STRESSTENSOR ptn_N (vector <GDB> inGDB);

vector <vector <double> > FRY (vector <GDB> inGDB, INPSET inset);

vector <vector <double> > SHAN (vector <GDB> inGDB, INPSET inset);

STRESSTENSOR ANGELIER (vector <GDB> inGDB, INPSET inset);

STRESSTENSOR MICHAEL (vector <GDB> inGDB, INPSET inset);
STRESSFIELD MICHAEL_PROCESS (vector <GDB> inGDB, INPSET inset);

STRESSTENSOR NDA (vector <GDB> inGDB, INPSET inset);
STRESSFIELD NDA_PROCESS (vector <GDB> inGDB, INPSET inset);

STRESSTENSOR BINGHAM (vector <GDB> inGDB);
STRESSFIELD BINGHAM_PROCESS (vector <GDB> inGDB);

vector <GDB> return_stressvector_estimators (STRESSTENSOR st, vector <GDB> inGDB, string method, bool compression_positive);

vector <GDB> generate_virtual_striae (vector <GDB> inGDB);

vector <GDB> inversion (string method, vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, CENTER mohr_center, PAPER P);

#endif
