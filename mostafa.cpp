// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "angelier.h"
#include "common.h"
#include "inversion.h"
#include "mostafa.h"
#include "ps.h"
#include "rgf.h"

using namespace std;

namespace {

STRESSTENSOR MOSTAFA_st;

}

STRESSTENSOR st_MOSTAFA () {

	return MOSTAFA_st;

}

STRESSFIELD sf_MOSTAFA (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center) {

	STRESSFIELD sf;

	int i = 0;
	int iteration_number = 100;

	do {

		MOSTAFA_st = st_ANGELIER (inGDB, inset);

		if (i == 0) inGDB = return_stressvector_estimators (MOSTAFA_st, inGDB, "ANGELIER", false);
		else 		inGDB = return_stressvector_estimators (MOSTAFA_st, inGDB, "MOSTAFA", false);

		sf = eigenvalue_eigenvector (MOSTAFA_st);
		sf = computestressfield_DXDYDZ (sf);
		sf = stress_regime (sf);

		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S1_ITER");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S2_ITER");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S3_ITER");

		PS_idealmovement (inGDB, o, inset, center);

		i++;

	} while (i < iteration_number);

	sf = eigenvalue_eigenvector (MOSTAFA_st);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
