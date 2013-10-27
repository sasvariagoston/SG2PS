// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "inversion.h"
#include "nda.h"
#include "rgf.h"

using namespace std;

STRESSTENSOR st_NDA (vector <GDB> inGDB, INPSET inset) {

	double r = inset.angle / 90.0;
	double q = 1.0 - r;

	size_t i = 0;

	STRESSTENSOR st;

	if (inset.virt_striae == "Y" ) inGDB = generate_virtual_striae (inGDB);

	st._11 = 0.0;
	st._12 = 0.0;
	st._13 = 0.0;
	st._22 = 0.0;
	st._23 = 0.0;
	st._33 = 0.0;

	VCTR N = declare_vector (0.0, 1.0, 0.0);
	VCTR E = declare_vector (1.0, 0.0, 0.0);
	VCTR U = declare_vector (0.0, 0.0, 1.0);

	VCTR p, t;

	do {

		if (inGDB.at(i).OFFSET != "NONE") {

			t = declare_vector(
					q * inGDB.at(i).N.X + r * inGDB.at(i).SV.X,
					q * inGDB.at(i).N.Y + r * inGDB.at(i).SV.Y,
					q * inGDB.at(i).N.Z + r * inGDB.at(i).SV.Z
			);

			p = declare_vector(
					q * inGDB.at(i).SV.X - r * inGDB.at(i).N.X,
					q * inGDB.at(i).SV.Y - r * inGDB.at(i).N.Y,
					q * inGDB.at(i).SV.Z - r * inGDB.at(i).N.Z
			);
		}

		else {

			p = declare_vector(0.0, 0.0, 0.0);
			t = declare_vector(0.0, 0.0, 0.0);
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

		i++;

	} while (i < inGDB.size());

	return st;
}

STRESSFIELD sf_NDA (STRESSTENSOR st) {

	return eigenvalue_eigenvector (st);
}
