// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "brute_force.hpp"
#include "common.h"
#include "data_sort.hpp"
#include "rgf.h"
#include "stresstensor.hpp"

using namespace std;

namespace {

const double MINIMUM_DET = 1.0e-25;
}

double stresstensor_determinant (const STRESSTENSOR& st) {

	return	(st._11 * st._22 * st._33) +
			(st._12 * st._23 * st._13) +
			(st._13 * st._12 * st._23) -
			(st._13 * st._22 * st._13) -
			(st._12 * st._12 * st._33) -
			(st._11 * st._23 * st._23);
}

bool is_stress_tensor_singular (const STRESSTENSOR& st) {

	double det = stresstensor_determinant (st);

	return (fabs (det) < 1.0e-12);
}

/*
STRESSTENSOR fix_stress_tensor_singularity(STRESSTENSOR& st) {

	const double one_plus_tiny = 1 + 1.0e-4;

	double det = stresstensor_determinant (st);

	if (det < MINIMUM_DET) {

		st._11 *= one_plus_tiny;
		st._22 *= one_plus_tiny;
		st._33 *= one_plus_tiny;
	}
	return st;
}
*/

bool is_stresstensor_diagonal (const STRESSTENSOR st) {

	return st._13 < 1.0e-20 && st._23 < 1.0e-20 && st._12 < 1.0e-20;
}

STRESSFIELD eigenvalue_eigenvector_diagonal (STRESSTENSOR st) {

	STRESSFIELD sf;

	if (st._11 > st._22 && st._11 > st._33) {

		if (st._22 > st._33) {

			sf.EIGENVALUE = declare_vector(st._11, st._22, st._33);
			sf.EIGENVECTOR1 = declare_vector(1.0, 0.0, 0.0);
			sf.EIGENVECTOR2 = declare_vector(0.0, 1.0, 0.0);
			sf.EIGENVECTOR3 = declare_vector(0.0, 0.0, 1.0);
		}
		else{
			sf.EIGENVALUE = declare_vector(st._11, st._33, st._22);
			sf.EIGENVECTOR1 = declare_vector(1.0, 0.0, 0.0);
			sf.EIGENVECTOR3 = declare_vector(0.0, 1.0, 0.0);
			sf.EIGENVECTOR2 = declare_vector(0.0, 0.0, 1.0);
		}
	}
	else if (st._22 > st._11 && st._22 > st._33) {

		if (st._11 > st._33) {

			sf.EIGENVALUE = declare_vector(st._22, st._11, st._33);
			sf.EIGENVECTOR2 = declare_vector(1.0, 0.0, 0.0);
			sf.EIGENVECTOR1 = declare_vector(0.0, 1.0, 0.0);
			sf.EIGENVECTOR3 = declare_vector(0.0, 0.0, 1.0);
		}
		else{
			sf.EIGENVALUE = declare_vector(st._22, st._33, st._11);
			sf.EIGENVECTOR2 = declare_vector(1.0, 0.0, 0.0);
			sf.EIGENVECTOR3 = declare_vector(0.0, 1.0, 0.0);
			sf.EIGENVECTOR1 = declare_vector(0.0, 0.0, 1.0);
		}
	}
	else if (st._33 > st._11 && st._33 > st._22) {

		if (st._11 > st._22) {

			sf.EIGENVALUE = declare_vector(st._33, st._11, st._22);
			sf.EIGENVECTOR3 = declare_vector(1.0, 0.0, 0.0);
			sf.EIGENVECTOR1 = declare_vector(0.0, 1.0, 0.0);
			sf.EIGENVECTOR2 = declare_vector(0.0, 0.0, 1.0);
		}
		else{
			sf.EIGENVALUE = declare_vector(st._33, st._22, st._11);
			sf.EIGENVECTOR3 = declare_vector(1.0, 0.0, 0.0);
			sf.EIGENVECTOR2 = declare_vector(0.0, 1.0, 0.0);
			sf.EIGENVECTOR1 = declare_vector(0.0, 0.0, 1.0);
		}
	}
	else ASSERT_DEAD_END();

	return sf;
}

STRESSFIELD eigenvalue_eigenvector (STRESSTENSOR st) {

	//if (is_stresstensor_diagonal(st)) return eigenvalue_eigenvector_diagonal(st);

	STRESSFIELD sf;

	const double A =   1.0;

	const double B = - (st._11 + st._22 + st._33);

	const double C =   (st._11 * st._22) + (st._22 * st._33) + (st._11 * st._33) - (st._12 * st._12) - (st._23 * st._23) - (st._13 * st._13);

	const double D = - ((st._11 * st._22 * st._33) + (2.0 * st._12 * st._23 * st._13) - (st._12 * st._12 * st._33) - (st._23 * st._23 * st._11) - (st._13 * st._13 * st._22));

	vector <double> X = cubic_solution (A, B, C, D);

	stable_sort (X.begin(), X.begin()+3);

	sf.EIGENVALUE.X = X.at(2);
	sf.EIGENVALUE.Y = X.at(1);
	sf.EIGENVALUE.Z = X.at(0);

	double a1 = st._11 - sf.EIGENVALUE.X;
	double b1 = st._12;
	double c1 = st._13;
	double a2 = st._12;
	double b2 = st._22 - sf.EIGENVALUE.X;
	double c2 = st._23;

	sf.EIGENVECTOR1.Z = 1.0;

    ASSERT_GT(fabs(b1), 1.0e-20);

    const double denom1 = (b2 * a1) - (a2 * b1);

    ASSERT_NE(denom1, 0.0);               // Please keep it like this: We would like to
    ASSERT_GE(fabs(denom1), MINIMUM_DET); // distinguish between small and exact zero.

	sf.EIGENVECTOR1.X = ((b1 * c2) - (b2 * c1)) / denom1;

	sf.EIGENVECTOR1.Y = - ((a1 * sf.EIGENVECTOR1.X) + c1) / b1;
	sf.EIGENVECTOR1 = unitvector (sf.EIGENVECTOR1, true);

	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR1.X)) sf.EIGENVECTOR1.X = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR1.Y)) sf.EIGENVECTOR1.Y = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR1.Z)) sf.EIGENVECTOR1.Z = 1.0 - 1E-8;
	sf.EIGENVECTOR1 = unitvector (sf.EIGENVECTOR1, true);

	a1 = st._11 - sf.EIGENVALUE.Y;
	b2 = st._22 - sf.EIGENVALUE.Y;

	sf.EIGENVECTOR2.Z = 1.0;

	const double denom2 = (b2 * a1) - (a2 * b1);

	ASSERT_NE(denom2, 0.0);               // Please keep it like this: We would like to
	ASSERT_GE(fabs(denom2), MINIMUM_DET); // distinguish between small and exact zero.

	sf.EIGENVECTOR2.X = ((b1 * c2) - (b2 * c1)) / denom2;
	sf.EIGENVECTOR2.Y = - ((a1 * sf.EIGENVECTOR2.X) + c1) / b1;

	sf.EIGENVECTOR2 = unitvector (sf.EIGENVECTOR2, true);

	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR2.X)) sf.EIGENVECTOR2.X = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR2.Y)) sf.EIGENVECTOR2.Y = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR2.Z)) sf.EIGENVECTOR2.Z = 1.0 - 1E-8;
	sf.EIGENVECTOR2 = unitvector (sf.EIGENVECTOR2, true);

	a1 = st._11 - sf.EIGENVALUE.Z;
	b2 = st._22 - sf.EIGENVALUE.Z;

	sf.EIGENVECTOR3.Z = 1.0;

	const double denom3 = (b2 * a1) - (a2 * b1);

    ASSERT_NE(denom3, 0.0);               // Please keep it like this: We would like to
    ASSERT_GE(fabs(denom3), MINIMUM_DET); // distinguish between small and exact zero.

	sf.EIGENVECTOR3.X = ((b1 * c2) - (b2 * c1)) / denom3;
	sf.EIGENVECTOR3.Y = - ((a1 * sf.EIGENVECTOR3.X) + c1) / b1;
	sf.EIGENVECTOR3 = unitvector (sf.EIGENVECTOR3, true);

	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR3.X)) sf.EIGENVECTOR3.X = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR3.X)) sf.EIGENVECTOR3.Y = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR3.X)) sf.EIGENVECTOR3.Z = 1.0 - 1E-8;
	sf.EIGENVECTOR3 = unitvector (sf.EIGENVECTOR3, true);

	sf = sort_stress_axes (sf);

	return sf;
}

vector <STRESSAXIS> stressfield_to_stress_axes (const STRESSFIELD& sf) {

	STRESSAXIS E1;
	E1.EIGENVALUE = sf.EIGENVALUE.X;
	E1.EIGENVECTOR = sf.EIGENVECTOR1;

	STRESSAXIS E2;
	E2.EIGENVALUE = sf.EIGENVALUE.Y;
	E2.EIGENVECTOR = sf.EIGENVECTOR2;

	STRESSAXIS E3;
	E3.EIGENVALUE = sf.EIGENVALUE.Z;
	E3.EIGENVECTOR = sf.EIGENVECTOR3;

	return {E1, E2, E3};
}

STRESSFIELD stress_axes_to_stressfield (const vector <STRESSAXIS>& sa) {

	ASSERT (sa.size() == 3);

	STRESSFIELD OUT;

	OUT.EIGENVALUE.X = sa.at(0).EIGENVALUE;
	OUT.EIGENVECTOR1 = sa.at(0).EIGENVECTOR;

	OUT.EIGENVALUE.Y = sa.at(1).EIGENVALUE;
	OUT.EIGENVECTOR2 = sa.at(1).EIGENVECTOR;

	OUT.EIGENVALUE.Z = sa.at(2).EIGENVALUE;
	OUT.EIGENVECTOR3 = sa.at(2).EIGENVECTOR;

	return OUT;
}

STRESSFIELD sort_stress_axes (const STRESSFIELD& sf) {

	vector <STRESSAXIS> SA = stressfield_to_stress_axes (sf);

	SA = sort_by_stressaxes (SA);

	return stress_axes_to_stressfield (SA);
}

STRESSTENSOR stresstensor_from_eigenvalue_eigenvector (STRESSFIELD sf) {

	vector < vector <double> >  D = init_matrix (3);
	vector < vector <double> >  T = init_matrix (3);

	STRESSTENSOR out;

	const VCTR E1 = unitvector (sf.EIGENVECTOR1, true);
	const VCTR E2 = unitvector (sf.EIGENVECTOR2, true);
	const VCTR E3 = unitvector (sf.EIGENVECTOR3, true);

	D.at(0).at(0) = E1.X;
	D.at(0).at(1) = E1.Y;
	D.at(0).at(2) = E1.Z;

	D.at(1).at(0) = E2.X;
	D.at(1).at(1) = E2.Y;
	D.at(1).at(2) = E2.Z;

	D.at(2).at(0) = E3.X;
	D.at(2).at(1) = E3.Y;
	D.at(2).at(2) = E3.Z;

	T.at(0).at(0) = sf.EIGENVALUE.X;
	T.at(0).at(1) = 0.0;
	T.at(0).at(2) = 0.0;

	T.at(1).at(0) = 0.0;
	T.at(1).at(1) = sf.EIGENVALUE.Y;
	T.at(1).at(2) = 0.0;

	T.at(2).at(0) = 0.0;
	T.at(2).at(1) = 0.0;
	T.at(2).at(2) = sf.EIGENVALUE.Z;

	T = mult_mtrx (transpose(D), T);

	T = mult_mtrx (T, D);

	out._11 = T.at(0).at(0);
	out._12 = T.at(0).at(1);
	out._13 = T.at(0).at(2);
	out._22 = T.at(1).at(1);
	out._23 = T.at(1).at(2);
	out._33 = T.at(2).at(2);

	return out;
}

STRESSTENSOR convert_matrix_to_stresstensor (const vector <vector <double> >& IN) {

	ASSERT2(IN.size() == 3, "3x3 matrix expected for stress tensor conversion");
	ASSERT2(IN.at(0).size() == 3, "3x3 matrix expected for stress tensor conversion");

	STRESSTENSOR OUT;

	OUT._11 = IN.at(0).at(0);
	OUT._12 = IN.at(0).at(1);
	OUT._13 = IN.at(0).at(2);
	OUT._22 = IN.at(1).at(1);
	OUT._23 = IN.at(1).at(2);
	OUT._33 = IN.at(2).at(2);

	return OUT;
}

STRESSFIELD stress_regime (const STRESSFIELD& in) {

	STRESSFIELD out = in;

	out.stressratio = (out.EIGENVALUE.Y - out.EIGENVALUE.Z) / (out.EIGENVALUE.X - out.EIGENVALUE.Z);

	out.shmax = 999.99;
	out.shmin = 999.99;

	if ((out.S_1.DIP <= out.S_3.DIP) && (out.S_2.DIP <= out.S_3.DIP)) {

		out.regime = "COMPRESSIONAL";
		out.shmax = out.S_1.DIPDIR;
		out.delvaux_str = 2.0 + out.stressratio;
	}

	else if ((out.S_1.DIP <= out.S_2.DIP) && (out.S_3.DIP <= out.S_2.DIP)) {

		out.regime = "STRIKE-SLIP";
		out.shmax = out.S_1.DIPDIR;
		out.shmin = out.shmax + 90.0;
		out.delvaux_str = 2.0 - out.stressratio;
	}

	else {

		out.regime = "EXTENSIONAL";
		out.shmin = out.S_3.DIPDIR;
		out.delvaux_str = out.stressratio;
	}

	if 		(is_in_range(0.00, 0.25, out.delvaux_str)) out.delvaux_rgm = "RADIAL EXTENSIVE";
	else if (is_in_range(0.25, 0.75, out.delvaux_str)) out.delvaux_rgm = "PURE EXTENSIVE";
	else if (is_in_range(0.75, 1.25, out.delvaux_str)) out.delvaux_rgm = "TRANSTENSIVE";
	else if (is_in_range(1.25, 1.75, out.delvaux_str)) out.delvaux_rgm = "PURE STRIKE SLIP";
	else if (is_in_range(1.75, 2.25, out.delvaux_str)) out.delvaux_rgm = "TRANSPRESSIVE";
	else if (is_in_range(2.25, 2.75, out.delvaux_str)) out.delvaux_rgm = "PURE COMPRESSIVE";
	else out.delvaux_rgm = "RADIAL COMPRESSIVE";

	return out;
}

STRESSTENSOR try_stresstensot_or_invert (const STRESSTENSOR& st, const vector <GDB>& inGDB) {

	const double ANG1 = return_average_misfit (st, inGDB);

	STRESSTENSOR st2 = invert_stress_tensor(st);

	const double ANG2 = return_average_misfit (st2, inGDB);

	if (ANG1 < ANG2) return st;

	return st2;
}

STRESSTENSOR invert_stress_tensor (const STRESSTENSOR& st) {

	STRESSTENSOR out;

	out._11 = - st._11;
	out._12 = - st._12;
	out._13 = - st._13;
	out._22 = - st._22;
	out._23 = - st._23;
	out._33 = - st._33;

	return out;
}

STRESSTENSOR add_stress_tensor (const STRESSTENSOR& st1, const STRESSTENSOR& st2) {

	STRESSTENSOR out;

	out._11 = st1._11 + st2._11;
	out._12 = st1._12 + st2._12;
	out._13 = st1._13 + st2._13;
	out._22 = st1._22 + st2._22;
	out._23 = st1._23 + st2._23;
	out._33 = st1._33 + st2._33;

	return out;
}

STRESSFIELD computestressfield_NXNYNZ (const STRESSFIELD& in) {

	STRESSFIELD sf = in;

	sf.S_1 = dipdir_dip_from_NXNYNZ (in.EIGENVECTOR1);
	sf.S_2 = dipdir_dip_from_NXNYNZ (in.EIGENVECTOR2);
	sf.S_3 = dipdir_dip_from_NXNYNZ (in.EIGENVECTOR3);

	return sf;
}

STRESSFIELD computestressfield_DXDYDZ (const STRESSFIELD& in) {

	STRESSFIELD sf = in;

	sf.S_1 = dipdir_dip_from_DXDYDZ (in.EIGENVECTOR1);
	sf.S_2 = dipdir_dip_from_DXDYDZ (in.EIGENVECTOR2);
	sf.S_3 = dipdir_dip_from_DXDYDZ (in.EIGENVECTOR3);

	return sf;
}

STRESSFIELD correct_SF_to_fit_D (const STRESSFIELD& in) {

	STRESSFIELD out = in;

	if (is_D_up (in.EIGENVECTOR1)) out.EIGENVECTOR1 = flip_vector (in.EIGENVECTOR1);
	if (is_D_up (in.EIGENVECTOR2)) out.EIGENVECTOR2 = flip_vector (in.EIGENVECTOR2);
	if (is_D_up (in.EIGENVECTOR3)) out.EIGENVECTOR3 = flip_vector (in.EIGENVECTOR3);

	return out;
}

STRESSFIELD correct_SF_to_fit_N (const STRESSFIELD& in) {

	STRESSFIELD out = in;

	if (is_N_down (in.EIGENVECTOR1)) out.EIGENVECTOR1 = flip_vector (in.EIGENVECTOR1);
	if (is_N_down (in.EIGENVECTOR2)) out.EIGENVECTOR2 = flip_vector (in.EIGENVECTOR2);
	if (is_N_down (in.EIGENVECTOR3)) out.EIGENVECTOR3 = flip_vector (in.EIGENVECTOR3);

	return out;
}

VCTR return_stressvector (const STRESSTENSOR& st, const VCTR& N) {

	VCTR OUT = declare_vector (
			(st._11 * N.X + st._12 * N.Y + st._13 * N.Z),
			(st._12 * N.X + st._22 * N.Y + st._23 * N.Z),
			(st._13 * N.X + st._23 * N.Y + st._33 * N.Z));

	OUT = flip_vector (OUT);

	ASSERT_FINITE (OUT.X, OUT.Y, OUT.Z);

	return OUT;
}

VCTR return_normalstress (const STRESSTENSOR& st, const VCTR& N) {

	const VCTR stressvector = return_stressvector (st, N);

	const double stress = (N.X * stressvector.X) + (N.Y * stressvector.Y) + (N.Z * stressvector.Z);

	VCTR OUT = declare_vector (N.X * stress, N.Y * stress, N.Z * stress);

	ASSERT_FINITE (OUT.X, OUT.Y, OUT.Z);

	return OUT;
}

VCTR return_shearstress (const STRESSTENSOR& st, const VCTR& N) {

	const VCTR stressvector = return_stressvector (st, N);

	const VCTR normalstress = return_normalstress (st, N);

	return (declare_vector(
			stressvector.X - normalstress.X,
			stressvector.Y - normalstress.Y,
			stressvector.Z - normalstress.Z));
}

double return_ANG (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV) {

	const VCTR shearstress = return_shearstress (st, N);

	return ACOS (dotproduct (SV, shearstress, true));
}

double return_RUP (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV, const double& lambda) {

	const VCTR shearstress  = return_shearstress (st, N);
	const VCTR stressvector = return_stressvector (st, N);

	double out = lambda * lambda;

	out = out +
			(shearstress.X * shearstress.X) +
			(shearstress.Y * shearstress.Y) +
			(shearstress.Z * shearstress.Z);

	out = out - 2.0 * lambda * dotproduct (SV, stressvector, false);

	return ((sqrt(out * out)) / lambda) * 100.0;
}

double return_average_misfit (const STRESSTENSOR& st, const vector <GDB>& inGDB) {

	double misfit = 0.0;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const VCTR N = inGDB.at(i).N;
		const VCTR SV = inGDB.at(i).DC;

		const double ang = return_ANG (st, N, SV);

		misfit = misfit + ang;
	}
	return misfit / inGDB.size();
}

vector <GDB> return_stressvector_estimators (const STRESSTENSOR& st, const vector <GDB>& inGDB, const string& method) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		VCTR N = inGDB.at(i).N;
		VCTR SV = inGDB.at(i).DC;

		double lambda = inGDB.at(i).lambda;

		outGDB.at(i).SHEAR_S  = return_shearstress  (st, N);

		outGDB.at(i).NORMAL_S = return_normalstress (st, N);

		outGDB.at(i).ANG  = return_ANG (st, N, SV);

		outGDB.at(i).RUP  = return_RUP (st, N, SV, lambda);

		if (method == "MOSTAFA")

		outGDB.at(i).lambda = sqrt(
				outGDB.at(i).SHEAR_S.X * outGDB.at(i).SHEAR_S.X +
				outGDB.at(i).SHEAR_S.Y * outGDB.at(i).SHEAR_S.Y +
				outGDB.at(i).SHEAR_S.Z * outGDB.at(i).SHEAR_S.Z);
	}
	return outGDB;
}

STRESSTENSOR return_stresstensor_from_n1_ang_phi (const VCTR& N1, const double& ANG, const double& PHI) {

	const vector <vector <double> > M1 = DIR_MX1_from_n1 (N1, ANG);

	const vector <vector <double> > T = st_from_reduced_stresstensor (M1, PHI);

	return convert_matrix_to_stresstensor (T);
}
