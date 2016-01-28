// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
S1: 183/82 (-0.008416, -0.145508, -0.989321)
S2: 045/06 ( 0.706469,  0.699321, -0.108865)
S3: 315/06 (-0.707694,  0.699841, -0.096912)
EIGENVALUES: 1.375502, -0.532479, -0.843023

S1: 020/15 ( 0.325814,  0.908593, -0.261350)
S2: 176/73 ( 0.021955, -0.283630, -0.958682)
S3: 288/06 (-0.945179,  0.306614, -0.112359)
EIGENVALUES: 0.967683, -0.397449, -0.570234

*/

#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "rgf.h"
#include "shan.h"
#include "stresstensor.hpp"

using namespace std;

vector < vector <double> > shan_matrix_from_GDB (const GDB& inGDB)  {

	vector <double>  o = init_vector (5);

	const VCTR n = inGDB.N;
	const VCTR s = inGDB.DC;

	const VCTR t = unitvector (crossproduct (s, n), true);

	o.at(0) = n.X * t.X - n.Z * t.Z;
	o.at(1) = n.Y * t.Y - n.Z * t.Z;
	o.at(2) = n.X * t.Y + n.Y * t.X;
	o.at(3) = n.X * t.Z + n.Z * t.X;
	o.at(4) = n.Y * t.Z + n.Z * t.Y;

	return outer_product (o);
}

vector <vector <double> > SHAN_matrix (const vector <GDB>& inGDB) {

	vector <GDB> processGDB = inGDB;

	vector <vector <double> > shan_matrix = init_matrix (5);

	for (size_t i = 0; i < processGDB.size(); i++) {

		vector <vector <double> > temp = init_matrix (5);

		temp = shan_matrix_from_GDB (processGDB.at(i));

		shan_matrix = add_mtrx (shan_matrix, temp);
	}
	return jacobi (shan_matrix);
}

STRESSTENSOR st_SHAN (const vector <GDB>& inGDB) {

	STRESSTENSOR st;

	vector <vector <double> > A = init_matrix (5);
	vector <vector <double> > D = init_matrix (5);
	vector <vector <double> > EVEV = SHAN_matrix (inGDB);

	A = generate_A (EVEV);
	D = generate_D (EVEV);

	const int first_eigenvalue = return_first_eigenvalue (A);

	st._11 = D.at(first_eigenvalue).at(0);
	st._22 = D.at(first_eigenvalue).at(1);
	st._12 = D.at(first_eigenvalue).at(2);
	st._13 = D.at(first_eigenvalue).at(3);
	st._23 = D.at(first_eigenvalue).at(4);
	st._33 = 0.0 - st._11 - st._22;

	return try_stresstensot_or_invert (st, inGDB);
}

STRESSFIELD sf_SHAN (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
