// Copyright (C) 2012-2016,  Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
S1: 192/83 (-0.025145, -0.120809, -0.992357)
S2: 043/06 ( 0.682221,  0.723514, -0.105367)
S3: 313/04 (-0.730713,  0.679657, -0.064226)
EIGENVALUES: 0.823293, -0.337894, -0.485399

S1: 019/14 ( 0.310737,  0.917651, -0.247706)
S2: 179/75 ( 0.003282, -0.261642, -0.965159)
S3: 287/05 (-0.950490,  0.299097, -0.084314)
EIGENVALUES: 0.916728, -0.402233, -0.514495
*/

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "data_sort.hpp"
#include "inversion.h"
#include "fry.h"
#include "rgf.h"
#include "stresstensor.hpp"

using namespace std;

namespace {

size_t second_eigenvalue = 0;
size_t first_eigenvalue = 0;

vector <vector <double> > A = init_matrix (6);
vector <vector <double> > D = init_matrix (6);

vector <vector <double> > EVEV;
}

bool check_fry_matrix (const size_t frst_eigenvalue, const vector <vector <double> >& in_eigenvector) {

	vector <sort_jacobi> buffer;
	sort_jacobi buf;

	for (size_t i = 0; i < in_eigenvector.size(); i++) {

		buf.ID = i;
		buf.eigenvalue = in_eigenvector.at(frst_eigenvalue).at(i);
		buffer.push_back(buf);
	}

	stable_sort(buffer.begin(), buffer.end(), byeigenvalue);

	if (
		is_in_range(-0.005, 0.005, buffer.at(0).eigenvalue) &&
		is_in_range(-0.005, 0.005, buffer.at(1).eigenvalue) &&
		is_in_range(-0.005, 0.005, buffer.at(2).eigenvalue) &&
		is_in_range(-0.570, 0.580, buffer.at(3).eigenvalue) &&
		is_in_range(-0.570, 0.580, buffer.at(4).eigenvalue) &&
		is_in_range(-0.570, 0.580, buffer.at(5).eigenvalue)
	) return true;
	return false;
}

bool fry_correct (const vector <GDB>& inGDB) {

	EVEV = FRY_matrix (inGDB);

	A = generate_A (EVEV);
	D = generate_D (EVEV);

	first_eigenvalue = return_first_eigenvalue (A);
	second_eigenvalue = return_second_eigenvalue (A);

	if (check_fry_matrix (first_eigenvalue, D)) return true;
	return false;
}

vector <double> hyperplane_from_GDB (const GDB& inGDB)  {

	vector <double>  out = init_vector (6);
	CENTR_VECT o;

	const VCTR n = inGDB.N;
	const VCTR b = inGDB.DC;

	const VCTR t = unitvector (crossproduct (b, n), true);

	o.U = - n.X * t.X;
	o.V = - n.Y * t.Y;
	o.W = - n.Z * t.Z;

	o.X = - (t.X * n.Y + t.Y * n.X);
	o.Y = - (t.Y * n.Z + t.Z * n.Y);
	o.Z = - (t.Z * n.X + t.X * n.Z);

	o = unitvector (o);

	out.at(0) = o.U;
	out.at(1) = o.V;
	out.at(2) = o.W;
	out.at(3) = o.X;
	out.at(4) = o.Y;
	out.at(5) = o.Z;

	return out;
}

vector <vector <double> > FRY_matrix (const vector <GDB>& inGDB) {

	vector <GDB> processGDB = inGDB;

	vector <vector <double> > TNSR6 = init_matrix (6);

	for (size_t i = 0; i < processGDB.size(); i++) {

		const vector <double> hyperplane = hyperplane_from_GDB (processGDB.at(i));

		vector <vector <double> > temp  = init_matrix (6);
		temp = outer_product (hyperplane);

		TNSR6 = add_mtrx (TNSR6, temp);
	}
	return jacobi (TNSR6);
}

STRESSTENSOR st_FRY (const vector <GDB>& inGDB) {

	STRESSTENSOR st;

	st._11 = D.at(second_eigenvalue).at(0);
	st._22 = D.at(second_eigenvalue).at(1);
	st._33 = D.at(second_eigenvalue).at(2);
	st._12 = D.at(second_eigenvalue).at(3);
	st._23 = D.at(second_eigenvalue).at(4);
	st._13 = D.at(second_eigenvalue).at(5);

	return try_stresstensot_or_invert (st, inGDB);
}

STRESSFIELD sf_FRY (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
