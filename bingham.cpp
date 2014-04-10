// Copyright (C) 2012- 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "allowed_keys.hpp"
#include "average.hpp"
#include "assertions.hpp"
#include "bingham.h"
#include "common.h"
#include "exceptions.hpp"
#include "rgf.h"

using namespace std;

STRESSTENSOR st_BINGHAM (vector <GDB> inGDB) {

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

	for (size_t i = 0; i < inGDB.size(); i++) {

		STRESSTENSOR T;

		VCTR PN = inGDB.at(i).N;

		bool OTB = (is_overturned (inGDB.at(i)) && (inGDB.at(i).DATATYPE) == "BEDDING");

		double norm_sqr = dotproduct(PN, PN);

		if (fabs(norm_sqr-1.0) > 1.0e-4) {
			ASSERT2(false, "Should be unitvector [X, Y, Z] = [ " << PN.X << ", " <<PN.Y << ", " << PN.Z << "]");
		}

		T._11 = (dotproduct (PN, E, false) * dotproduct (PN, E, false));
		T._12 = (dotproduct (PN, E, false) * dotproduct (PN, N, false));
		T._13 = (dotproduct (PN, E, false) * dotproduct (PN, U, false));
		T._22 = (dotproduct (PN, N, false) * dotproduct (PN, N, false));
		T._23 = (dotproduct (PN, N, false) * dotproduct (PN, U, false));
		T._33 = (dotproduct (PN, U, false) * dotproduct (PN, U, false));

		if (OTB) T = invert_stress_tensor(T);

		st = add_stress_tensor (st, T);
	}
	return st;
}

STRESSFIELD sf_BINGHAM (STRESSFIELD sf) {

	double total_eigenvalue = sf.EIGENVALUE.X + sf.EIGENVALUE.Y + sf.EIGENVALUE.Z;

	VCTR eigenvalue = sf.EIGENVALUE;

	sf.EIGENVALUE.X = eigenvalue.Z / total_eigenvalue;
	sf.EIGENVALUE.Y = eigenvalue.Y / total_eigenvalue;
	sf.EIGENVALUE.Z = eigenvalue.X / total_eigenvalue;


	double EV1 = sf.EIGENVALUE.X;
	double EV2 = sf.EIGENVALUE.Y;
	double EV3 = sf.EIGENVALUE.Z;

	VCTR EVC1 = sf.EIGENVECTOR1;
	VCTR EVC2 = sf.EIGENVECTOR2;
	VCTR EVC3 = sf.EIGENVECTOR3;

	if (EV1 >= EV2 && EV2 >= EV3) {
		sf.EIGENVALUE = declare_vector (EV1, EV2, EV3);
		sf.EIGENVECTOR1 = EVC1;
		sf.EIGENVECTOR2 = EVC2;
		sf.EIGENVECTOR3 = EVC3;
	}
	else if (EV1 >= EV3 && EV3 >= EV2) {
		sf.EIGENVALUE = declare_vector (EV1, EV3, EV2);
		sf.EIGENVECTOR1 = EVC1;
		sf.EIGENVECTOR2 = EVC3;
		sf.EIGENVECTOR3 = EVC2;
	}
	else if (EV2 >= EV1 && EV1 >= EV3) {
		sf.EIGENVALUE = declare_vector (EV2, EV1, EV3);
		sf.EIGENVECTOR1 = EVC2;
		sf.EIGENVECTOR2 = EVC1;
		sf.EIGENVECTOR3 = EVC3;
	}
	else if (EV2 >= EV3 && EV3 >= EV1) {
		sf.EIGENVALUE = declare_vector (EV2, EV3, EV1);
		sf.EIGENVECTOR1 = EVC2;
		sf.EIGENVECTOR2 = EVC3;
		sf.EIGENVECTOR3 = EVC1;
	}
	else if (EV3 >= EV1 && EV1 >= EV2) {
		sf.EIGENVALUE = declare_vector (EV3, EV1, EV2);
		sf.EIGENVECTOR1 = EVC3;
		sf.EIGENVECTOR2 = EVC1;
		sf.EIGENVECTOR3 = EVC2;
	}
	else if (EV3 >= EV2 && EV2 >= EV1) {
		sf.EIGENVALUE = declare_vector (EV3, EV2, EV1);
		sf.EIGENVECTOR1 = EVC3;
		sf.EIGENVECTOR2 = EVC2;
		sf.EIGENVECTOR3 = EVC1;
	}
	else ASSERT_DEAD_END();

	return sf;
}
