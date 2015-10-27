// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.


/*
S1: 226/85 (-0.065704, -0.063404, -0.995823)
S2: 047/05 ( 0.724023,  0.683706, -0.091302)
S3: 317/00 (-0.686639,  0.726998, -0.000984)
EIGENVALUES: 1.010557, -0.270462, -0.740095

S1: 053/10 ( 0.792472,  0.586710, -0.166612)
S2: 144/03 ( 0.588486, -0.807318, -0.043832)
S3: 248/80 (-0.160226, -0.063313, -0.985048)
EIGENVALUES: 1.598986, -0.590352, -1.008634
*/

#include <cmath>
#include <iomanip>
#include <iostream>

#include "angelier.h"
#include "assertions.hpp"
#include "common.h"
#include "inversion.h"
#include "rgf.h"
#include "stresstensor.hpp"

using namespace std;

ANG_PRM angelier_parameters (const vector <GDB>& inGDB) {

	ANG_PRM rs;

	rs.a = 0.0;		rs.b = 0.0;		rs.c = 0.0;		rs.d = 0.0;
	rs.e = 0.0;		rs.f = 0.0;		rs.g = 0.0;		rs.h = 0.0;
	rs.i = 0.0;		rs.j = 0.0;		rs.k = 0.0;		rs.l = 0.0;
	rs.m = 0.0;		rs.n = 0.0;		rs.p = 0.0;		rs.q = 0.0;
	rs.u = 0.0;		rs.v = 0.0;		rs.w = 0.0;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const VCTR N = inGDB.at(i).N;

		const VCTR S = flip_vector (inGDB.at(i).DC);

		const double lambda = inGDB.at(i).lambda;

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
	}
	return rs;
}

STRESSTENSOR compute_angelier_stresstensor (const ANG_PRM& p, const vector <GDB>& inGDB) {

	vector <double> quartic_roots_for_psi;

	double prev_misfit = 1E20;
	const double pi = 3.1415926535;

	STRESSTENSOR st, min_st;

	const double d_0 = det_3 (declare_3x3_matrix (p.a, p.d, p.e, p.d, p.b, p.f, p.e, p.f, p.c));

	const double d_1 = det_3 (declare_3x3_matrix (p.g, p.d, p.e, p.h, p.b, p.f, p.i, p.f, p.c));
	const double d_2 = det_3 (declare_3x3_matrix (p.a, p.g, p.e, p.d, p.h, p.f, p.e, p.i, p.c));
	const double d_3 = det_3 (declare_3x3_matrix (p.a, p.d, p.g, p.d, p.b, p.h, p.e, p.f, p.i));

	const double d_i_1 = det_3 (declare_3x3_matrix (p.j, p.d, p.e, p.k, p.b, p.f, p.l, p.f, p.c));
	const double d_i_2 = det_3 (declare_3x3_matrix (p.a, p.j, p.e, p.d, p.k, p.f, p.e, p.l, p.c));
	const double d_i_3 = det_3 (declare_3x3_matrix (p.a, p.d, p.j, p.d, p.b, p.k, p.e, p.f, p.l));

	const double d_ii_1 = det_3 (declare_3x3_matrix (p.u, p.d, p.e, p.v, p.b, p.f, p.w, p.f, p.c));
	const double d_ii_2 = det_3 (declare_3x3_matrix (p.a, p.u, p.e, p.d, p.v, p.f, p.e, p.w, p.c));
	const double d_ii_3 = det_3 (declare_3x3_matrix (p.a, p.d, p.u, p.d, p.b, p.v, p.e, p.f, p.w));


	const double Y1 = p.j * d_1 +    p.k * d_2 +    p.l * d_3;

	const double Y2 = p.j * d_i_1 +  p.k * d_i_2 +  p.l * d_i_3 -
					  p.g * d_1   -  p.h * d_2   -  p.i * d_3;

	const double Y3 = p.j * d_ii_1 + p.k * d_ii_2 + p.l * d_ii_3 + p.p * d_0;

	const double Y4 = p.g * d_ii_1 + p.h * d_ii_2 + p.i * d_ii_3 + p.q * d_0;


	const double A =          p.m * d_0 + Y1 - Y3;
	const double B = - 2.0 * (p.n * d_0 + Y2 + Y4);
	const double C = - 6.0 * (p.m * d_0 + Y1);
	const double D =   2.0 * (p.n * d_0 + Y2 - Y4);
	const double E =          p.m * d_0 + Y1 + Y3;

	const vector <double> quartic_result = quartic_solution (A, B, C, D, E);

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

	for (size_t i = 0; i < quartic_roots_for_psi.size(); i++) {

		const double psi = 2.0 * atan (quartic_roots_for_psi.at(i));

		ASSERT(!isnan(psi));

		const double alpha = (d_1 * cos (psi) + d_i_1 * sin (psi) + d_ii_1) / d_0;
		const double gamma = (d_2 * cos (psi) + d_i_2 * sin (psi) + d_ii_2) / d_0;
		const double beta  = (d_3 * cos (psi) + d_i_3 * sin (psi) + d_ii_3) / d_0;

		st._11 = cos (psi);
		st._12 = alpha;
		st._13 = gamma;
		st._22 = cos (psi + (2.0 * pi) / 3.0);
		st._23 = beta;
		st._33 = cos (psi + (4.0 * pi) / 3.0);

		const double misfit = return_average_misfit (st, inGDB);

		if (misfit < prev_misfit) {

			min_st = st;
			prev_misfit = misfit;
		}
	}
	return min_st;
}

STRESSTENSOR st_ANGELIER (const vector <GDB>& inGDB) {

	vector <GDB> processGDB = inGDB;

	ANG_PRM parameters = angelier_parameters (processGDB);

	STRESSTENSOR st = compute_angelier_stresstensor (parameters, processGDB);

	return try_stresstensot_or_invert (st, processGDB);
}

STRESSFIELD sf_ANGELIER (const STRESSTENSOR& st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
