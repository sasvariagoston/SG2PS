
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

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
					q * processGDB.at(i).N.X + r * processGDB.at(i).SV.X,
					q * processGDB.at(i).N.Y + r * processGDB.at(i).SV.Y,
					q * processGDB.at(i).N.Z + r * processGDB.at(i).SV.Z);

			p = declare_vector(
					q * processGDB.at(i).SV.X - r * processGDB.at(i).N.X,
					q * processGDB.at(i).SV.Y - r * processGDB.at(i).N.Y,
					q * processGDB.at(i).SV.Z - r * processGDB.at(i).N.Z);
		}

		st._11 = st._11 +
				(dotproduct (p, E, false) * dotproduct (p, E, false)) -
				(dotproduct (t, E, false) * dotproduct (t, E, false));

		st._12 = st._12 +
				(dotproduct (p, E, false) * dotproduct (p, N, false)) -
				(dotproduct (t, E, false) * dotproduct (t, N, false));

		st._13 = st._13 +
				(dotproduct (p, E, false) * dotproduct (p, U, false)) -
				(dotproduct (t, E, false) * dotproduct (t, U, false));

		st._22 = st._22 +
				(dotproduct (p, N, false) * dotproduct (p, N, false)) -
				(dotproduct (t, N, false) * dotproduct (t, N, false));

		st._23 = st._23 +
				(dotproduct (p, N, false) * dotproduct (p, U, false)) -
				(dotproduct (t, N, false) * dotproduct (t, U, false));

		st._33 = st._33 +
				(dotproduct (p, U, false) * dotproduct (p, U, false)) -
				(dotproduct (t, U, false) * dotproduct (t, U, false));
	}
	return st;
}

STRESSFIELD sf_NDA (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
