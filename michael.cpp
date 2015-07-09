
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
S1: 228/87 (-0.042638, -0.038321, -0.998355)
S2: 042/03 ( 0.670402,  0.739803, -0.057028)
S3: 132/00 ( 0.740772, -0.671731, -0.005854)
EIGENVALUES: 1.515783, -0.641149, -0.874634

S1: 021/13 ( 0.343129,  0.910750, -0.229775)
S2: 168/74 ( 0.055530, -0.263868, -0.962959)
S3: 289/08 (-0.937645,  0.317660, -0.141115)
EIGENVALUES: 1.515783, -0.641149, -0.874634
*/

#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "inversion.h"
#include "michael.h"
#include "rgf.h"
#include "stresstensor.hpp"

using namespace std;

vector <vector < double> > michael_parameters (const vector <GDB>& inGDB) {

	vector <vector < double> > out = init_matrix (3 * inGDB.size(), 5);

	for (size_t i = 0; i < inGDB.size(); i++) {

		VCTR N = inGDB.at(i).N;

		out.at((i * 3) + 0).at(0) =   N.X - (N.X * N.X * N.X) + (N.X * N.Z * N.Z);
		out.at((i * 3) + 0).at(1) =   N.Y -               (2.0 * N.Y * N.X * N.X);
		out.at((i * 3) + 0).at(2) =   N.Z -               (2.0 * N.Z * N.X * N.X);
		out.at((i * 3) + 0).at(3) =       - (N.X * N.Y * N.Y) + (N.X * N.Z * N.Z);
		out.at((i * 3) + 0).at(4) =       -               (2.0 * N.X * N.Y * N.Z);

		out.at((i * 3) + 1).at(0) =       - (N.Y * N.X * N.X) + (N.Y * N.Z * N.Z);
		out.at((i * 3) + 1).at(1) =   N.X -               (2.0 * N.X * N.Y * N.Y);
		out.at((i * 3) + 1).at(2) =       -               (2.0 * N.X * N.Y * N.Z);
		out.at((i * 3) + 1).at(3) =   N.Y - (N.Y * N.Y * N.Y) + (N.Y * N.Z * N.Z);
		out.at((i * 3) + 1).at(4) =   N.Z -               (2.0 * N.Z * N.Y * N.Y);

		out.at((i * 3) + 2).at(0) = - N.Z - (N.Z * N.X * N.X) + (N.Z * N.Z * N.Z);
		out.at((i * 3) + 2).at(1) =       -	              (2.0 * N.X * N.Y * N.Z);
		out.at((i * 3) + 2).at(2) =   N.X -               (2.0 * N.X * N.Z * N.Z);
		out.at((i * 3) + 2).at(3) = - N.Z - (N.Y * N.Y * N.Z) + (N.Z * N.Z * N.Z);
		out.at((i * 3) + 2).at(4) =   N.Y -               (2.0 * N.Y * N.Z * N.Z);
	}
	return out;
}

vector <vector < double> > stressvector_parameters (const vector <GDB>& inGDB) {

	vector <vector < double> > o = init_matrix (3 * inGDB.size(), 1);

	for (size_t i = 0; i < inGDB.size(); i++) {

		o.at((i * 3) + 0).at(0) = inGDB.at(i).DC.X;
		o.at((i * 3) + 1).at(0) = inGDB.at(i).DC.Y;
		o.at((i * 3) + 2).at(0) = inGDB.at(i).DC.Z;
	}
	return o;
}

STRESSTENSOR st_MICHAEL (const vector <GDB>& inGDB) {

	STRESSTENSOR st;

	vector <GDB> processGDB = inGDB;

	vector <vector<double> > M = michael_parameters (processGDB);
	const vector <vector<double> > M_t = transpose (M);

	vector <vector<double> > B = stressvector_parameters (processGDB);

	vector <vector<double> > LU = init_matrix (10, 5);
	vector <vector<double> > U = init_matrix (5);
	vector <vector<double> > L = init_matrix (5);

	vector <vector<double> > Z = init_matrix (5, 1);

	vector <vector<double> > X = init_matrix (5, 1);

	M = mult_mtrx (M_t, M);

	B = mult_mtrx (M_t, B);

	LU = LU_decomposition (M);

	U = generate_A (LU);
	L = generate_D (LU);

	Z = compute_Z (L, B);

	X = compute_X (U, Z);

	st._11 =  X.at(0).at(0);
	st._12 =  X.at(1).at(0);
	st._13 =  X.at(2).at(0);
	st._22 =  X.at(3).at(0);
	st._23 =  X.at(4).at(0);
	st._33 = - (st._11 + st._22);

	return invert_stress_tensor (st);
}

STRESSFIELD sf_MICHAEL (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	sf = stress_regime (sf);

	return sf;
}
