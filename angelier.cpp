// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iomanip>
#include <cmath>

#include "angelier.h"
#include "ps.h"
#include "common.h"
#include "rgf.h"

using namespace std;

vector <vector < double> > michael_parameters (vector <GDB> inGDB) {

	vector <vector < double> > out = init_matrix (3 * inGDB.size(), 5);

	size_t i = 0;

	do {

		VCTR N = inGDB.at(i).N;

		out.at( (i * 3) + 0 ).at(0) =     N.X - (N.X * N.X * N.X) + (N.X * N.Z * N.Z);
		out.at( (i * 3) + 0 ).at(1) =     N.Y -               (2.0 * N.Y * N.X * N.X);
		out.at( (i * 3) + 0 ).at(2) =     N.Z -               (2.0 * N.Z * N.X * N.X);
		out.at( (i * 3) + 0 ).at(3) =         - (N.X * N.Y * N.Y) + (N.X * N.Z * N.Z);
		out.at( (i * 3) + 0 ).at(4) =         -               (2.0 * N.X * N.Y * N.Z);

		out.at( (i * 3) + 1 ).at(0) =         - (N.Y * N.X * N.X) + (N.Y * N.Z * N.Z);
		out.at( (i * 3) + 1 ).at(1) =     N.X -               (2.0 * N.X * N.Y * N.Y);
		out.at( (i * 3) + 1 ).at(2) =         -               (2.0 * N.X * N.Y * N.Z);
		out.at( (i * 3) + 1 ).at(3) =     N.Y - (N.Y * N.Y * N.Y) + (N.Y * N.Z * N.Z);
		out.at( (i * 3) + 1 ).at(4) =     N.Z -               (2.0 * N.Z * N.Y * N.Y);

		out.at( (i * 3) + 2 ).at(0) =   - N.Z - (N.Z * N.X * N.X) + (N.Z * N.Z * N.Z);
		out.at( (i * 3) + 2 ).at(1) =         -	            (2.0 * N.X * N.Y * N.Z);
		out.at( (i * 3) + 2 ).at(2) =     N.X -               (2.0 * N.X * N.Z * N.Z);
		out.at( (i * 3) + 2 ).at(3) =   - N.Z - (N.Y * N.Y * N.Z) + (N.Z * N.Z * N.Z);
		out.at( (i * 3) + 2 ).at(4) =     N.Y -               (2.0 * N.Y * N.Z * N.Z);

		i++;
	}

	while (i < inGDB.size());

	return out;
}

vector <vector < double> > stressvector_parameters (vector <GDB> inGDB) {

	vector <vector < double> > o = init_matrix (3 * inGDB.size(), 1);
	size_t i = 0;

	do {

		cout << inGDB.at(i).SV.X << endl;

		o.at( (i * 3) + 0 ).at(0) =  inGDB.at(i).SV.X;
		o.at( (i * 3) + 1 ).at(0) =  inGDB.at(i).SV.Y;
		o.at( (i * 3) + 2 ).at(0) =  inGDB.at(i).SV.Z;

		i++;

	} while (i < inGDB.size());

	return o;
}


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

STRESSTENSOR ptn_P (vector <GDB> inGDB) {

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
	VCTR P;

	do {

		P = inGDB.at(i).ptnP;

		st._11 = st._11 + (dotproduct (P, E, false) * dotproduct (P, E, false));
		st._12 = st._12 + (dotproduct (P, E, false) * dotproduct (P, N, false));
		st._13 = st._13 + (dotproduct (P, E, false) * dotproduct (P, U, false));
		st._22 = st._22 + (dotproduct (P, N, false) * dotproduct (P, N, false));
		st._23 = st._23 + (dotproduct (P, N, false) * dotproduct (P, U, false));
		st._33 = st._33 + (dotproduct (P, U, false) * dotproduct (P, U, false));

		i++;

	} while (i < inGDB.size());

	return st;
}

STRESSTENSOR ptn_T (vector <GDB> inGDB) {

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
	VCTR T;

	do {

		T = inGDB.at(i).ptnT;

		st._11 = st._11 + (dotproduct (T, E, false) * dotproduct (T, E, false));
		st._12 = st._12 + (dotproduct (T, E, false) * dotproduct (T, N, false));
		st._13 = st._13 + (dotproduct (T, E, false) * dotproduct (T, U, false));
		st._22 = st._22 + (dotproduct (T, N, false) * dotproduct (T, N, false));
		st._23 = st._23 + (dotproduct (T, N, false) * dotproduct (T, U, false));
		st._33 = st._33 + (dotproduct (T, U, false) * dotproduct (T, U, false));

		i++;

	} while (i < inGDB.size());

	return st;
}

STRESSTENSOR ptn_N (vector <GDB> inGDB) {

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
	VCTR n;

	do {

		n =  inGDB.at(i).ptnN;

		st._11 = st._11 + (dotproduct (n, E, false) * dotproduct (n, E, false));
		st._12 = st._12 + (dotproduct (n, E, false) * dotproduct (n, N, false));
		st._13 = st._13 + (dotproduct (n, E, false) * dotproduct (n, U, false));
		st._22 = st._22 + (dotproduct (n, N, false) * dotproduct (n, N, false));
		st._23 = st._23 + (dotproduct (n, N, false) * dotproduct (n, U, false));
		st._33 = st._33 + (dotproduct (n, U, false) * dotproduct (n, U, false));

		i++;

	} while (i < inGDB.size());

	return st;
}

vector <vector <double> > FRY (vector <GDB> inGDB, INPSET inset) {

	size_t i = 0;

	vector <double>  hyperplane;
	vector <vector <double> > TNSR6 = init_matrix (6);
	vector <vector <double> > temp  = init_matrix (6);

	if (inset.virt_striae == "Y" ) inGDB = generate_virtual_striae (inGDB);

	do {

		hyperplane = hyperplane_from_GDB (inGDB.at(i));

		temp = outer_product (hyperplane);

		TNSR6 = add_mtrx (TNSR6, temp);

		i++;

	} while (i < inGDB.size());

	return jacobi (TNSR6);
}

vector <vector <double> > SHAN (vector <GDB> inGDB, INPSET inset) {

	size_t i = 0;

	vector <vector <double> > shan_matrix = init_matrix (5);
	vector <vector <double> > temp = init_matrix (5);

	if (inset.virt_striae == "Y" ) inGDB = generate_virtual_striae (inGDB);

	do {

		temp = shan_matrix_from_GDB (inGDB .at(i));
		shan_matrix = add_mtrx (shan_matrix, temp);

		i++;

	} while (i < inGDB.size());

	return jacobi (shan_matrix);
}

STRESSTENSOR ANGELIER (vector <GDB> inGDB, INPSET inset) {

	if (inset.virt_striae == "Y" ) inGDB = generate_virtual_striae (inGDB);

	ANG_PRM parameters = angelier_parameters (inGDB);

	STRESSTENSOR st = compute_angelier_stresstensor (parameters, inGDB);

	return st;
}

STRESSTENSOR MICHAEL (vector <GDB> inGDB, INPSET inset) {


	STRESSTENSOR st;

	if (inset.virt_striae == "Y" ) inGDB = generate_virtual_striae (inGDB);

	vector < vector <double> > M = michael_parameters (inGDB);
	vector < vector <double> > M_t = transpose (M);

	vector < vector <double> > B = stressvector_parameters (inGDB);

	vector < vector <double> > LU = init_matrix (10, 5);
	vector < vector <double> > U = init_matrix (5);
	vector < vector <double> > L = init_matrix (5);

	vector < vector <double> > Z = init_matrix (5, 1);

	vector < vector <double> > X = init_matrix (5, 1);

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

	st =  invert_stress_tensor (st);

	return st;
}

STRESSFIELD  MICHAEL_PROCESS (vector <GDB> inGDB, INPSET inset) {

	STRESSTENSOR st = MICHAEL (inGDB, inset);

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}

STRESSTENSOR NDA (vector <GDB> inGDB, INPSET inset) {

	double r = inset.angle / 90.0;
	double q = 1.0 - r;

	size_t i = 0;

	STRESSTENSOR st;

	if (inset.virt_striae == "Y" ) inGDB = generate_virtual_striae (inGDB);

	st._11 = 0.0;
	st._12 = 0.0;
	st._13 = 0.0;
	st._22 = 0.0;
	st._23 = 0.0;
	st._33 = 0.0;

	VCTR N = declare_vector (0.0, 1.0, 0.0);
	VCTR E = declare_vector (1.0, 0.0, 0.0);
	VCTR U = declare_vector (0.0, 0.0, 1.0);

	VCTR p, t;

	do {

		if (inGDB.at(i).OFFSET != "NONE") {

			t = declare_vector(
					q * inGDB.at(i).N.X + r * inGDB.at(i).SV.X,
					q * inGDB.at(i).N.Y + r * inGDB.at(i).SV.Y,
					q * inGDB.at(i).N.Z + r * inGDB.at(i).SV.Z
			);

			p = declare_vector(
					q * inGDB.at(i).SV.X - r * inGDB.at(i).N.X,
					q * inGDB.at(i).SV.Y - r * inGDB.at(i).N.Y,
					q * inGDB.at(i).SV.Z - r * inGDB.at(i).N.Z
			);
		}

		else {

			p = declare_vector(0.0, 0.0, 0.0);
			t = declare_vector(0.0, 0.0, 0.0);
		}

		st._11 = st._11 +
				(dotproduct (p, E, false) * dotproduct (p, E, false)) -
				(dotproduct (t, E, false) * dotproduct (t, E, false));

		st._12 = st._12 +
				(dotproduct (p, E, false) * dotproduct (p, N, false)) -
				(dotproduct (t, E, false) * dotproduct (t, N, false));

		st._13 = st._13 +
				(dotproduct (p, E, false) * dotproduct (p, U, false)) -
				(dotproduct (t, E, false) * dotproduct (t, U, false));

		st._22 = st._22 +
				(dotproduct (p, N, false) * dotproduct (p, N, false)) -
				(dotproduct (t, N, false) * dotproduct (t, N, false));

		st._23 = st._23 +
				(dotproduct (p, N, false) * dotproduct (p, U, false)) -
				(dotproduct (t, N, false) * dotproduct (t, U, false));

		st._33 = st._33 +
				(dotproduct (p, U, false) * dotproduct (p, U, false)) -
				(dotproduct (t, U, false) * dotproduct (t, U, false));

		i++;

	} while (i < inGDB.size());

	return st;
}

STRESSFIELD NDA_PROCESS (vector <GDB> inGDB, INPSET inset) {

	STRESSTENSOR st = NDA (inGDB, inset);

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}

STRESSTENSOR BINGHAM (vector <GDB> inGDB) {

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

vector <GDB> return_stressvector_estimators (STRESSTENSOR st, vector <GDB> inGDB, string method, bool compression_positive) {

	vector <GDB> outGDB = inGDB;

	size_t i = 0.0;

	do {

		outGDB.at(i).SHEAR_S  = return_shearstress  (st, inGDB.at(i), compression_positive);
		outGDB.at(i).NORMAL_S = return_normalstress (st, inGDB.at(i), compression_positive);
		outGDB.at(i).UPSILON  = return_upsilon (st, inGDB.at(i), method, compression_positive);

		outGDB.at(i).ANG  = return_ANG (st, inGDB.at(i), compression_positive);
		outGDB.at(i).RUP  = return_RUP (st, inGDB.at(i), compression_positive);

		if (method == "MOSTAFA")

		outGDB.at(i).lambda =  sqrt(
				outGDB.at(i).SHEAR_S.X * outGDB.at(i).SHEAR_S.X +
				outGDB.at(i).SHEAR_S.Y * outGDB.at(i).SHEAR_S.Y +
				outGDB.at(i).SHEAR_S.Z * outGDB.at(i).SHEAR_S.Z);

		i++;

	} while (i < inGDB.size());

	return outGDB;
}

vector <GDB> generate_virtual_striae (vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;
	GDB buffer;

	size_t original_set_size = inGDB.size();
	size_t i = 0;

	do {

		buffer = inGDB.at(i);

		buffer.N.X = - buffer.N.X;
		buffer.N.Y = - buffer.N.Y;

		buffer.D.X = - buffer.D.X;
		buffer.D.Y = - buffer.D.Y;

		buffer.S.X = - buffer.S.X;
		buffer.S.Y = - buffer.S.Y;

		buffer.SV.X = - buffer.SV.X;
		buffer.SV.Y = - buffer.SV.Y;

		buffer.NC.X = - buffer.NC.X;
		buffer.NC.Y = - buffer.NC.Y;

		buffer.DC.X = - buffer.DC.X;
		buffer.DC.Y = - buffer.DC.Y;

		buffer.SC.X = - buffer.SC.X;
		buffer.SC.Y = - buffer.SC.Y;

		outGDB.push_back(buffer);

		i++;

	} while (i < original_set_size);

	i = 0;

	outGDB =  manipulate_N (outGDB);

	return outGDB;
}


STRESSFIELD BINGHAM_PROCESS (vector <GDB> inGDB) {

	STRESSTENSOR st = BINGHAM (inGDB);

	st = invert_stress_tensor (st);

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	double total_eigenvalue = sf.EIGENVALUE.X + sf.EIGENVALUE.Y + sf.EIGENVALUE.Z;

	VCTR eigenvalue = sf.EIGENVALUE;

	sf.EIGENVALUE.X = eigenvalue.Z / total_eigenvalue;
	sf.EIGENVALUE.Y = eigenvalue.Y / total_eigenvalue;
	sf.EIGENVALUE.Z = eigenvalue.X / total_eigenvalue;

	return computestressfield_DXDYDZ (sf);
}


vector <GDB> inversion (string method, vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, CENTER mohr_center, PAPER P) {

	STRESSFIELD sf, sf_ptn;
	STRESSTENSOR st;

	bool successfull = false;

	vector <GDB> tempGDB;

	size_t second_eigenvalue = 0;
	size_t first_eigenvalue = 0;

	int i = 0;
	int iteration_number = 100;

	vector <vector <double> > EVEV;
	vector <vector <double> > A;
	vector <vector <double> > D;

	double average_misfit = 0;
	double misfit1 = 0;
	double misfit2 = 0;

	if (method == "BINGHAM") {

		sf = BINGHAM_PROCESS (inGDB);

		successfull = check_correct_stressfield (sf);
	}

	else if (method == "NDA") {

		st = NDA (inGDB, inset);

		sf = eigenvalue_eigenvector (st);

		sf = computestressfield_DXDYDZ (sf);

		sf = stress_regime (sf);

		successfull = check_correct_stressfield (sf);

		inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
	}

	else if (method == "ANGELIER") {

		st = ANGELIER (inGDB, inset);

		sf = eigenvalue_eigenvector (st);

		sf = computestressfield_DXDYDZ (sf);

		sf = stress_regime (sf);

		successfull = check_correct_stressfield (sf);

		inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
	}

	else if (method == "MOSTAFA") {

		do {
			st = ANGELIER (inGDB, inset);

			if (i == 0) inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
			else 		inGDB = return_stressvector_estimators (st, inGDB, "MOSTAFA", false);

			sf = eigenvalue_eigenvector (st);
			sf = computestressfield_DXDYDZ (sf);
			sf = stress_regime (sf);
			PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S1_ITER");
			PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S2_ITER");
			PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S3_ITER");
			PS_idealmovement (inGDB, o, inset, center);

			i++;

		} while (i < iteration_number);

		sf = eigenvalue_eigenvector (st);

		sf = computestressfield_DXDYDZ (sf);

		sf = stress_regime (sf);

		successfull = check_correct_stressfield (sf);

		inGDB = return_stressvector_estimators (st, inGDB, "MOSTAFA", false);
	}


	else if (method == "MICHAEL") {

		sf = MICHAEL_PROCESS (inGDB, inset);

		successfull = check_correct_stressfield (sf);

		inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
	}

	else if (method == "FRY") {

		A = init_matrix (6);
		D = init_matrix (6);

		EVEV = FRY (inGDB, inset);

		A = generate_A (EVEV);
		D = generate_D (EVEV);

		first_eigenvalue = return_first_eigenvalue (A);
		second_eigenvalue = return_second_eigenvalue (A);


		if (check_fry_matrix (first_eigenvalue, D)) {

			st._11 = D.at(second_eigenvalue).at(0);
			st._22 = D.at(second_eigenvalue).at(1);
			st._33 = D.at(second_eigenvalue).at(2);
			st._12 = D.at(second_eigenvalue).at(3);
			st._23 = D.at(second_eigenvalue).at(4);
			st._13 = D.at(second_eigenvalue).at(5);

			misfit1 = return_average_misfit (st, inGDB, false);
			st = invert_stress_tensor (st);
			misfit2 = return_average_misfit (st, inGDB, false);

			if (misfit1 < misfit2) st = invert_stress_tensor (st);

			sf = eigenvalue_eigenvector (st);
			sf = computestressfield_DXDYDZ (sf);
			sf = stress_regime (sf);
			successfull = check_correct_stressfield (sf);
			inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
		}

		else successfull = false;
	}

	else if (method == "SHAN") {

		A = init_matrix (5);
		D = init_matrix (5);

		EVEV = SHAN (inGDB, inset);

		A = generate_A (EVEV);
		D = generate_D (EVEV);

		first_eigenvalue = return_first_eigenvalue (A);

		st._11 = D.at(first_eigenvalue).at(0);
		st._22 = D.at(first_eigenvalue).at(1);
		st._12 = D.at(first_eigenvalue).at(2);
		st._13 = D.at(first_eigenvalue).at(3);
		st._23 = D.at(first_eigenvalue).at(4);
		st._33 = 0.0 - st._11 - st._22;

		misfit1 = return_average_misfit (st, inGDB, false);
		st = invert_stress_tensor (st);

		misfit2 = return_average_misfit (st, inGDB, false);

		if (misfit1 < misfit2) st = invert_stress_tensor (st);

		sf = eigenvalue_eigenvector (st);

		sf = computestressfield_DXDYDZ (sf);

		sf = stress_regime (sf);

		successfull = check_correct_stressfield (sf);

		inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
	}

	else if (method == "PTN") {

		if (inset.virt_striae == "Y" ) tempGDB = generate_virtual_striae (inGDB);

		st = ptn_P (tempGDB);
		sf_ptn = eigenvalue_eigenvector (st);
		sf_ptn = computestressfield_DXDYDZ (sf_ptn);
		sf.EIGENVALUE.X = sf_ptn.EIGENVALUE.X;
		sf.EIGENVECTOR1 = sf_ptn.EIGENVECTOR1;
		sf.S_1 = sf_ptn.S_1;

		st = ptn_T (tempGDB);
		sf_ptn = eigenvalue_eigenvector (st);
		sf_ptn = computestressfield_DXDYDZ (sf_ptn);
		sf.EIGENVALUE.Z = sf_ptn.EIGENVALUE.Z;
		sf.EIGENVECTOR3 = sf_ptn.EIGENVECTOR1;
		sf.S_3 = sf_ptn.S_1;

		st = ptn_N (tempGDB);
		sf_ptn = eigenvalue_eigenvector (st);
		sf_ptn = computestressfield_DXDYDZ (sf_ptn);
		sf.EIGENVALUE.Y = sf_ptn.EIGENVALUE.Y;
		sf.EIGENVECTOR2 = sf_ptn.EIGENVECTOR1;
		sf.S_2 = sf_ptn.S_1;

		st = stresstensor_from_eigenvalue_eigenvector (sf);

		sf = stress_regime (sf);

		successfull = check_correct_stressfield (sf);

		inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
	}

	else {

	}

	if (successfull) {

		if (method == "ANGELIER") {

			PS_stressdata (o, center, P, sf, method);
			PS_stressarrows (o, center, P,  sf);
			PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, false);
			PS_RUP_distribution (inGDB, o, center, P);
			PS_ANG_distribution (inGDB, o, center, P);
			PS_stress_state (o, P, center, sf);
			sf = stressvector_to_DXDYDZ (sf);
			average_misfit = return_average_misfit (st, inGDB, false);
		}

		else if (method == "MOSTAFA") {

			PS_stressdata (o, center, P, sf, method);
			PS_stressarrows (o, center, P,  sf);
			PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, false);
			PS_RUP_distribution (inGDB, o, center, P);
			PS_ANG_distribution (inGDB, o, center, P);
			PS_stress_state (o, P, center, sf);
			sf = stressvector_to_DXDYDZ (sf);
			average_misfit = return_average_misfit (st, inGDB, false);
		}

		else if (method == "BINGHAM") {

			PS_stressdata (o, center, P, sf, method);
		}


		else if (method == "NDA") {

			PS_stressdata (o, center, P, sf, method);
			PS_stressarrows (o, center, P,  sf);
			PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, false);
			PS_ANG_distribution (inGDB, o, center, P);
			PS_stress_state (o, P, center, sf);
			sf = stressvector_to_DXDYDZ (sf);
			average_misfit = return_average_misfit (st, inGDB, false);
		}

		else if (method == "SHAN") {

			PS_stressdata (o, center, P, sf, method);
			PS_stressarrows (o, center, P,  sf);
			PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, false);
			PS_RUP_distribution (inGDB, o, center, P);
			PS_ANG_distribution (inGDB, o, center, P);
			PS_stress_state (o, P, center, sf);
			sf = stressvector_to_DXDYDZ (sf);
			average_misfit = return_average_misfit (st, inGDB, false);
		}

		else if (method == "MICHAEL") {

			PS_stressdata (o, center, P, sf, method);
			PS_stressarrows (o, center, P,  sf);
			PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, false);
			PS_ANG_distribution (inGDB, o, center, P);
			PS_stress_state (o, P, center, sf);
			sf = stressvector_to_DXDYDZ (sf);
			average_misfit = return_average_misfit (st, inGDB, false);
		}

		else if (method == "FRY") {

			PS_stressdata (o, center, P, sf, method);
			PS_stressarrows (o, center, P,  sf);
			PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, false);
			PS_RUP_distribution (inGDB, o, center, P);
			PS_ANG_distribution (inGDB, o, center, P);
			PS_stress_state (o, P, center, sf);
			sf = stressvector_to_DXDYDZ (sf);
			average_misfit = return_average_misfit (st, inGDB, false);
		}

		else if (method == "PTN") {

			PS_stressdata (o, center, P, sf, method);
			PS_stressarrows (o, center, P,  sf);
			PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, true);
			PS_ANG_distribution (inGDB, o, center, P);
			PS_stress_state (o, P, center, sf);
			sf = stressvector_to_DXDYDZ (sf);
			average_misfit = return_average_misfit (st, inGDB, false);
		}

		else {}

		if (method == "BINGHAM")

			cout
			<< "e1: "   	<< setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_1.DIPDIR
			<<  "/"     	<< setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_1.DIP
			<< " (" 		<< setfill ('0') << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.X * 100.0
			<< "%), e2: " 	<< setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_2.DIPDIR
			<<  "/"     	<< setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_2.DIP
			<< " ("			<< setfill ('0') << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Y * 100.0
			<< "%), e3: " 	<< setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_3.DIPDIR
			<<  "/"     	<< setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_3.DIP
			<< " ("			<< setfill ('0') << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Z * 100.0
			<< "%)"
			<< endl;

		else {

			cout
			<< "s1: "   		<< setfill ('0') << setw (3)  << fixed << setprecision (0) << sf.S_1.DIPDIR
			<<  "/"     		<< setfill ('0') << setw (2)  << fixed << setprecision (0) << sf.S_1.DIP
			<< ", s2: " 		<< setfill ('0') << setw (3)  << fixed << setprecision (0) << sf.S_2.DIPDIR
			<<  "/"     		<< setfill ('0') << setw (2)  << fixed << setprecision (0) << sf.S_2.DIP
			<< ", s3: " 		<< setfill ('0') << setw (3)  << fixed << setprecision (0) << sf.S_3.DIPDIR
			<<  "/"     		<< setfill ('0') << setw (2)  << fixed << setprecision (0) << sf.S_3.DIP
			<< ", " 			<< setfill (' ') << setw (18) << sf.delvaux_rgm
			<< ", R: "  		<< setfill ('0') << setw (4)  << fixed << setprecision (3) << sf.stressratio
			<< ", R': " 		<< setfill ('0') << setw (4)  << fixed << setprecision (3) << sf.delvaux_str
			<< ", av. misfit: " << setfill (' ') << setw (4)  << fixed << setprecision (1) << average_misfit
			<< " deg." << endl;

			PS_idealmovement (inGDB, o, inset, center);
		}

		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S1");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S2");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S3");
	}

	else cout << "unable to compute stress field for the data set." << endl;

	return inGDB;
}
