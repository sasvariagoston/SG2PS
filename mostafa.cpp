// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
S1: 225/86 (-0.054567, -0.053753, -0.997062)
S2: 044/04 ( 0.690774,  0.719005, -0.076567)
S3: 134/00 ( 0.721009, -0.692923, -0.002103)
EIGENVALUES: 1.008409, -0.425752, -0.582657

S1: 016/14 ( 0.261988,  0.933894, -0.243319)
S2: 175/75 ( 0.020422, -0.257434, -0.966080)
S3: 284/05 (-0.964855,  0.248133, -0.086516)
EIGENVALUES: 1.232731, -0.531661, -0.701071
*/

#include <cmath>
#include <iomanip>
#include <iostream>

#include "angelier.h"
#include "common.h"
#include "inversion.h"
#include "mostafa.h"
#include "ps.h"
#include "rgf.h"
#include "stresstensor.hpp"

using namespace std;

namespace {

vector <STRESSTENSOR> MOSTAFA_stv;

}

vector <STRESSTENSOR> stv_MOSTAFA () {

	return MOSTAFA_stv;
}

vector <STRESSFIELD> sfv_MOSTAFA (const vector <GDB>& inGDB) {

	vector <STRESSFIELD> SFV;

	vector <GDB> process_GDB = inGDB;

	const size_t iteration_number = 100;

	MOSTAFA_stv.clear();

	for (size_t i = 0; i < iteration_number; i++) {

		STRESSTENSOR MOSTAFA_st = st_ANGELIER (process_GDB);

		string METHOD;

		if (i == 0)	METHOD = "ANGELIER";
		else 		METHOD = "MOSTAFA";

		process_GDB = return_stressvector_estimators (MOSTAFA_st, process_GDB, METHOD);

		STRESSFIELD sf = eigenvalue_eigenvector (MOSTAFA_st);

		sf = correct_SF_to_fit_D (sf);

		sf = computestressfield_DXDYDZ (sf);

		sf = stress_regime (sf);

		SFV.push_back(sf);
		MOSTAFA_stv.push_back(MOSTAFA_st);
	}
	return SFV;
}
