// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
S1: 246/83 (-0.104884, -0.046121, -0.993414)
S2: 053/06 ( 0.790434,  0.602329, -0.111418)
S3: 143/02 ( 0.603501, -0.796914, -0.026719)
EIGENVALUES: 18.888006, -1.842917, -17.045090

S1: 019/16 ( 0.306754,  0.909331, -0.281103)
S2: 208/73 (-0.134418, -0.250993, -0.958611)
S3: 109/03 ( 0.942249, -0.331843, -0.045237)
EIGENVALUES: 18.888006, -1.842917, -17.045090
*/

#include <cmath>
#include <iomanip>
#include <iostream>

#include "allowed_keys.hpp"
#include "common.h"
#include "inversion.h"
#include "nda.h"
#include "rgf.h"
#include "settings.hpp"
#include "stresstensor.hpp"

using namespace std;

STRESSTENSOR st_NDA (const vector <GDB>& inGDB) {

	STRESSTENSOR st;

	vector <GDB> processGDB = inGDB;

	double r = is_STRESSANGLE() / 90.0;
	double q = 1.0 - r;

	st._11 = 0.0;
	st._12 = 0.0;
	st._13 = 0.0;
	st._22 = 0.0;
	st._23 = 0.0;
	st._33 = 0.0;

	VCTR N = declare_vector (0.0, 1.0, 0.0);
	VCTR E = declare_vector (1.0, 0.0, 0.0);
	VCTR U = declare_vector (0.0, 0.0, 1.0);

	for (size_t i = 0; i < processGDB.size(); i++) {

		const bool HAS_SENSE = !is_allowed_striae_none_sense (processGDB.at(i).OFFSET);

		VCTR p = declare_vector(0.0, 0.0, 0.0);
		VCTR t = declare_vector(0.0, 0.0, 0.0);

		if (HAS_SENSE)  {

			t = declare_vector(
					q * processGDB.at(i).N.X + r * processGDB.at(i).DC.X,
					q * processGDB.at(i).N.Y + r * processGDB.at(i).DC.Y,
					q * processGDB.at(i).N.Z + r * processGDB.at(i).DC.Z);

			p = declare_vector(
					q * processGDB.at(i).DC.X - r * processGDB.at(i).N.X,
					q * processGDB.at(i).DC.Y - r * processGDB.at(i).N.Y,
					q * processGDB.at(i).DC.Z - r * processGDB.at(i).N.Z);
		}

		const double pE = dotproduct (p, E, false);
		const double tE = dotproduct (t, E, false);
		const double pN = dotproduct (p, N, false);
		const double tN = dotproduct (t, N, false);
		const double pU = dotproduct (p, U, false);
		const double tU = dotproduct (t, U, false);

		st._11 = st._11 + pE * pE - tE * tE;
		st._12 = st._12 + pE * pN - tE * tN;
		st._13 = st._13 + pE * pU - tE * tU;
		st._22 = st._22 + pN * pN - tN * tN;
		st._23 = st._23 + pN * pU - tN * tU;
		st._33 = st._33 + pU * pU - tU * tU;
	}
	return try_stresstensot_or_invert (st, inGDB);
}

STRESSFIELD sf_NDA (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
