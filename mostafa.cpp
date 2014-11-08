// Copyright (C) 2012 - 2014 Ágoston Sasvári
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
#include "stresstensor.hpp"

using namespace std;

namespace {

//STRESSTENSOR MOSTAFA_st;

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
		sf = computestressfield_DXDYDZ (sf);
		sf = stress_regime (sf);

		SFV.push_back(sf);
		MOSTAFA_stv.push_back(MOSTAFA_st);
	}
	return SFV;
}
