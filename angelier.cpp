// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "angelier.h"
#include "assertions.hpp"
#include "common.h"
#include "inversion.h"
#include "rgf.h"

using namespace std;

ANG_PRM angelier_parameters (vector <GDB> inGDB) {

	size_t i = 0;
	ANG_PRM rs;
	VCTR N, S;
	double lambda = 0.0;

	rs.a = 0.0;		rs.b = 0.0;		rs.c = 0.0;		rs.d = 0.0;
	rs.e = 0.0;		rs.f = 0.0;		rs.g = 0.0;		rs.h = 0.0;
	rs.i = 0.0;		rs.j = 0.0;		rs.k = 0.0;		rs.l = 0.0;
	rs.m = 0.0;		rs.n = 0.0;		rs.p = 0.0;		rs.q = 0.0;
	rs.u = 0.0;		rs.v = 0.0;		rs.w = 0.0;

	do {

		N = inGDB.at(i).N;
		S = inGDB.at(i).SV;
		lambda = inGDB.at(i).lambda;

		S = declare_vector (- inGDB.at(i).SV.X, - inGDB.at(i).SV.Y, - inGDB.at(i).SV.Z);

		rs.a = rs.a + (N.X * N.X) + (N.Y * N.Y) - 4.0 * (N.X * N.X) * (N.Y * N.Y);
		rs.b = rs.b + (N.X * N.X) + (N.Z * N.Z) - 4.0 * (N.X * N.X) * (N.Z * N.Z);
		rs.c = rs.c + (N.Y * N.Y) + (N.Z * N.Z) - 4.0 * (N.Y * N.Y) * (N.Z * N.Z);

		rs.d = rs.d + (1.0 - (4.0 * N.X * N.X)) * N.Y * N.Z;
		rs.e = rs.e + (1.0 - (4.0 * N.Y * N.Y)) * N.Z * N.X;
		rs.f = rs.f + (1.0 - (4.0 * N.Z * N.Z)) * N.X * N.Y;

		rs.g = rs.g - (1.5 * N.X * N.Y * (1.0 - 2.0 * N.X * N.X));
		rs.h = rs.h - (1.5 * N.X * N.Z * (1.0 - 2.0 * N.X * N.X));

		rs.i = rs.i + 3.0 * N.X * N.X * N.Y * N.Z;

		rs.j = rs.j + (sqrt(3.0) / 2.0) * (  1.0 - (2.0 * N.Y * N.Y) + (2.0 * N.Z * N.Z)) * N.X * N.Y;
		rs.k = rs.k + (sqrt(3.0) / 2.0) * (- 1.0 - (2.0 * N.Y * N.Y) + (2.0 * N.Z * N.Z)) * N.X * N.Z;

		rs.l = rs.l + sqrt(3.0) * ((N.Z * N.Z) - (N.Y * N.Y)) * N.Y * N.Z;

		rs.m = rs.m + ((3.0 * sqrt(3.0)) / 4.0) * N.X * N.X * ((N.Z * N.Z) - (N.Y * N.Y));

		rs.n = rs.n + 1.5 * (N.X * N.X) * ((N.Y * N.Y) + (N.Z * N.Z)) - (3.0 * (N.Y * N.Y) * (N.Z * N.Z));

		rs.p = rs.p + lambda * (sqrt(3.0) / 2.0) * (N.Z * S.Z - N.Y * S.Y);

		rs.q = rs.q + lambda * 1.5 * N.X * S.X;

		rs.u = rs.u + lambda * (N.X * S.Y + N.Y * S.X);
		rs.v = rs.v + lambda * (N.X * S.Z + N.Z * S.X);
		rs.w = rs.w + lambda * (N.Y * S.Z + N.Z * S.Y);

		i++;

	} while (i < inGDB.size());

	return rs;
}

STRESSTENSOR compute_angelier_stresstensor (ANG_PRM p, vector <GDB> inGDB) {

	vector < vector < double > > temp;
	vector < double > quartic_result;
	vector < double > quartic_roots_for_psi;

	double psi, alpha, beta, gamma;
	double misfit = 0.0;
	double prev_misfit = 1E20;
	STRESSTENSOR st, min_st;
	size_t i = 0;
	double A, B, C, D, E;
	double Y1, Y2, Y3, Y4;
	const double pi = 3.1415926535;

	temp = declare_3x3_matrix (p.a, p.d, p.e, p.d, p.b, p.f, p.e, p.f, p.c);
	double d_0 = det_3 (temp);


	temp = declare_3x3_matrix (p.g, p.d, p.e, p.h, p.b, p.f, p.i, p.f, p.c);
	double d_1 = det_3 (temp);

	temp = declare_3x3_matrix (p.a, p.g, p.e, p.d, p.h, p.f, p.e, p.i, p.c);
	double d_2 = det_3 (temp);

	temp = declare_3x3_matrix (p.a, p.d, p.g, p.d, p.b, p.h, p.e, p.f, p.i);
	double d_3 = det_3 (temp);


	temp = declare_3x3_matrix (p.j, p.d, p.e, p.k, p.b, p.f, p.l, p.f, p.c);
	double d_i_1 = det_3 (temp);

	temp = declare_3x3_matrix (p.a, p.j, p.e, p.d, p.k, p.f, p.e, p.l, p.c);
	double d_i_2 = det_3 (temp);

	temp = declare_3x3_matrix (p.a, p.d, p.j, p.d, p.b, p.k, p.e, p.f, p.l);
	double d_i_3 = det_3 (temp);


	temp = declare_3x3_matrix (p.u, p.d, p.e, p.v, p.b, p.f, p.w, p.f, p.c);
	double d_ii_1 = det_3 (temp);

	temp = declare_3x3_matrix (p.a, p.u, p.e, p.d, p.v, p.f, p.e, p.w, p.c);
	double d_ii_2 = det_3 (temp);

	temp = declare_3x3_matrix (p.a, p.d, p.u, p.d, p.b, p.v, p.e, p.f, p.w);
	double d_ii_3 = det_3 (temp);



	Y1 = p.j * d_1 +    p.k * d_2 +    p.l * d_3;

	Y2 = p.j * d_i_1 +  p.k * d_i_2 +  p.l * d_i_3 -
		 p.g * d_1   -  p.h * d_2   -  p.i * d_3;

	Y3 = p.j * d_ii_1 + p.k * d_ii_2 + p.l * d_ii_3 + p.p * d_0;

	Y4 = p.g * d_ii_1 + p.h * d_ii_2 + p.i * d_ii_3 + p.q * d_0;



	A =          p.m * d_0 + Y1 - Y3;
	B = - 2.0 * (p.n * d_0 + Y2 + Y4);
	C = - 6.0 * (p.m * d_0 + Y1);
	D =   2.0 * (p.n * d_0 + Y2 - Y4);
	E =          p.m * d_0 + Y1 + Y3;

	quartic_result = quartic_solution (A, B, C, D, E);

	if (quartic_result.at(0) > 999.0) {

		min_st._11 = 999.99;
		min_st._12 = 999.99;
		min_st._13 = 999.99;
		min_st._22 = 999.99;
		min_st._23 = 999.99;
		min_st._33 = 999.99;

		return min_st;
	}

	else if ((quartic_result.at(4) != 0.0) && (quartic_result.at(5) != 0.0)) {

		quartic_roots_for_psi.push_back (quartic_result.at(0));
		quartic_roots_for_psi.push_back (quartic_result.at(3));
	}

	else {

		quartic_roots_for_psi.push_back (quartic_result.at(0));
		quartic_roots_for_psi.push_back (quartic_result.at(1));
		quartic_roots_for_psi.push_back (quartic_result.at(2));
		quartic_roots_for_psi.push_back (quartic_result.at(3));
	}

	do {

		psi = 2.0 * atan (quartic_roots_for_psi.at(i));

		ASSERT(!isnan(psi));

		alpha = (d_1 * cos (psi) + d_i_1 * sin (psi) + d_ii_1) / d_0;
		gamma = (d_2 * cos (psi) + d_i_2 * sin (psi) + d_ii_2) / d_0;
		beta  = (d_3 * cos (psi) + d_i_3 * sin (psi) + d_ii_3) / d_0;

		st._11 = cos (psi);
		st._12 = alpha;
		st._13 = gamma;
		st._22 = cos (psi + (2.0 * pi) / 3.0);
		st._23 = beta;
		st._33 = cos (psi + (4.0 * pi) / 3.0);

		misfit = return_average_misfit (st, inGDB, false);

		if (misfit < prev_misfit) {

			min_st = st;
			prev_misfit = misfit;
		}

		i++;

	} while (i < quartic_roots_for_psi.size());

	return min_st;
}

STRESSTENSOR st_ANGELIER (vector <GDB> inGDB, INPSET inset) {

	if (inset.virt_striae == "Y" ) inGDB = generate_virtual_striae (inGDB);

	ANG_PRM parameters = angelier_parameters (inGDB);

	return compute_angelier_stresstensor (parameters, inGDB);
}

STRESSFIELD sf_ANGELIER (STRESSTENSOR st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
