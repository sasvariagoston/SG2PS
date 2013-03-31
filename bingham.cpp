// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "assertions.hpp"
#include "bingham.h"
#include "common.h"
#include "rgf.h"

using namespace std;

STRESSTENSOR st_BINGHAM (vector <GDB> inGDB) {

	size_t i = 0;
	STRESSTENSOR st;

	st._11 = 0.0;
	st._12 = 0.0;
	st._13 = 0.0;
	st._22 = 0.0;
	st._23 = 0.0;
	st._33 = 0.0;

	VCTR N = declare_vector (0.0, 1.0, 0.0);
	VCTR E = declare_vector (1.0, 0.0, 0.0);
	VCTR U = declare_vector (0.0, 0.0, 1.0);
	VCTR planenormal;

	do {

		planenormal = inGDB.at(i).N;

		double norm_sqr = dotproduct(planenormal, planenormal);

		if (fabs(norm_sqr-1.0) > 1.0e-4) {
			ASSERT2(false, "Should be unitvector [X, Y, Z] = [ "<<planenormal.X<<", "<<planenormal.Y<<", "<<planenormal.Z<<"]");
		}

		st._11 = st._11 + (dotproduct (planenormal, E, false) * dotproduct (planenormal, E, false));
		st._12 = st._12 + (dotproduct (planenormal, E, false) * dotproduct (planenormal, N, false));
		st._13 = st._13 + (dotproduct (planenormal, E, false) * dotproduct (planenormal, U, false));
		st._22 = st._22 + (dotproduct (planenormal, N, false) * dotproduct (planenormal, N, false));
		st._23 = st._23 + (dotproduct (planenormal, N, false) * dotproduct (planenormal, U, false));
		st._33 = st._33 + (dotproduct (planenormal, U, false) * dotproduct (planenormal, U, false));

		i++;

	} while (i < inGDB.size());

	return st;
}

STRESSFIELD sf_BINGHAM (STRESSTENSOR st) {

	st = invert_stress_tensor (st);

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	double total_eigenvalue = sf.EIGENVALUE.X + sf.EIGENVALUE.Y + sf.EIGENVALUE.Z;

	VCTR eigenvalue = sf.EIGENVALUE;

	sf.EIGENVALUE.X = eigenvalue.Z / total_eigenvalue;
	sf.EIGENVALUE.Y = eigenvalue.Y / total_eigenvalue;
	sf.EIGENVALUE.Z = eigenvalue.X / total_eigenvalue;

	return computestressfield_DXDYDZ (sf);
}
