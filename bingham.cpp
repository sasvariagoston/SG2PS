// Copyright (C) 2012- 2015 Ágoston Sasvári
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
#include "stresstensor.hpp"

using namespace std;

vector <VCTR> generate_Bingham_dataset (const vector <GDB>& inGDB) {

	vector <VCTR> T;

	for (size_t i = 0; i < inGDB.size(); i++) T.push_back(inGDB.at(i).N);

	return T;
}

STRESSTENSOR st_BINGHAM (const vector <VCTR>& N_VCTR) {

	STRESSTENSOR st;

	st._11 = 0.0;
	st._12 = 0.0;
	st._13 = 0.0;
	st._22 = 0.0;
	st._23 = 0.0;
	st._33 = 0.0;

	for (size_t i = 0; i < N_VCTR.size(); i++) {

		STRESSTENSOR T;

		VCTR PN = N_VCTR.at(i);

		const double norm_sqr = dotproduct(PN, PN);

		if (fabs(norm_sqr-1.0) > 1.0e-4) {
			ASSERT2(false, "Should be unitvector [X, Y, Z] = [ " << PN.X << ", " <<PN.Y << ", " << PN.Z << "]");
		}

		T._11 = PN.X * PN.X;
		T._12 = PN.X * PN.Y;
		T._13 = PN.X * PN.Z;
		T._22 = PN.Y * PN.Y;
		T._23 = PN.Y * PN.Z;
		T._33 = PN.Z * PN.Z;

		st = add_stress_tensor (st, T);
	}
	return st;
}

STRESSFIELD sf_BINGHAM (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	const double total_eigenvalue = fabs(sf.EIGENVALUE.X) + fabs(sf.EIGENVALUE.Y) + fabs(sf.EIGENVALUE.Z);

	const VCTR eigenvalue = sf.EIGENVALUE;

	sf.EIGENVALUE.X = fabs(eigenvalue.X / total_eigenvalue);
	sf.EIGENVALUE.Y = fabs(eigenvalue.Y / total_eigenvalue);
	sf.EIGENVALUE.Z = fabs(eigenvalue.Z / total_eigenvalue);

	const double EV1 = sf.EIGENVALUE.X;
	const double EV2 = sf.EIGENVALUE.Y;
	const double EV3 = sf.EIGENVALUE.Z;

	const VCTR EVC1 = sf.EIGENVECTOR1;
	const VCTR EVC2 = sf.EIGENVECTOR2;
	const VCTR EVC3 = sf.EIGENVECTOR3;

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

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ(sf);

	/*
	==========================
	         For 230
    ==========================
	Eigenval.   	Eigenvect.
	==========================
	Max:  0.74    	239 / 18
	Int:  0.26    	331 / 06
	Min:  0.01    	079 / 71
	*/

	return sf;
}
