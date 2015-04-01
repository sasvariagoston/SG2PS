
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
#include "stresstensor.hpp"

using namespace std;

vector <VCTR> generate_Bingham_dataset (const vector <GDB>& inGDB) {

	vector <VCTR> T;

	const string DT = inGDB.at(0).DATATYPE;

	const bool FOR_FOLDSURFACE = is_allowed_foldsurface_processing (DT);

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (FOR_FOLDSURFACE) 	T.push_back(inGDB.at(i).N);
		else 					T.push_back(inGDB.at(i).N);//sure!
	}
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

	const VCTR N = declare_vector (0.0, 1.0, 0.0);
	const VCTR E = declare_vector (1.0, 0.0, 0.0);
	const VCTR U = declare_vector (0.0, 0.0, 1.0);

	for (size_t i = 0; i < N_VCTR.size(); i++) {

		STRESSTENSOR T;

		VCTR PN = N_VCTR.at(i);

		//remove!
		PN = flip_vector(PN);

		//cout << fixed << setprecision(6) << endl;
		//cout << PN.Z << endl;

		//const bool OTB = (is_overturned (inGDB.at(i)) && is_allowed_handle_as_bedding (inGDB.at(i).DATATYPE));

		const double norm_sqr = dotproduct(PN, PN);

		if (fabs(norm_sqr-1.0) > 1.0e-4) {
			ASSERT2(false, "Should be unitvector [X, Y, Z] = [ " << PN.X << ", " <<PN.Y << ", " << PN.Z << "]");
		}

		T._11 = (dotproduct (PN, E, false) * dotproduct (PN, E, false));
		T._12 = (dotproduct (PN, E, false) * dotproduct (PN, N, false));
		T._13 = (dotproduct (PN, E, false) * dotproduct (PN, U, false));
		T._22 = (dotproduct (PN, N, false) * dotproduct (PN, N, false));
		T._23 = (dotproduct (PN, N, false) * dotproduct (PN, U, false));
		T._33 = (dotproduct (PN, U, false) * dotproduct (PN, U, false));

		//if (OTB) T = invert_stress_tensor(T);

		st = add_stress_tensor (st, T);
	}

	//cout << fixed << setprecision(6) << endl;

	//cout << st._11 << endl;
	//cout << st._12 << endl;
	//cout << st._13 << endl;
	//cout << st._22 << endl;
	//cout << st._23 << endl;
	//cout << st._33 << endl;

	return st;
}

STRESSFIELD sf_BINGHAM (STRESSTENSOR& st) {

	//cout << "sf_BINGHAM" << endl;

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

	sf = computestressfield_NXNYNZ(sf);

	return sf;
}
