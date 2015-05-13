
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*

USING ANG.RGF:
==============

s1: 036/73
s2: 245/15
s3: 153/08
RADIAL EXTENSIVE
R: 0.195
R': 0.195
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

	const VCTR t = unitvector (crossproduct (s, n));

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

	const double misfit1 = return_average_misfit (st, inGDB);
	st = invert_stress_tensor (st);
	const double misfit2 = return_average_misfit (st, inGDB);

	if (misfit1 < misfit2) return invert_stress_tensor (st);
	return st;
}

STRESSFIELD sf_SHAN (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
