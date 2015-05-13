
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
USING ANG.RGF:
==============

s1: 070/74
s2: 238/16
s3: 329/03
PURE EXTENSIVE
R: 0.516
R': 0.516
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
	return st;
}

STRESSFIELD sf_NDA (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
