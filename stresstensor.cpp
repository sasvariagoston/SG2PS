
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "allowed_keys.hpp"
#include "array_to_vector.hpp"
#include "assertions.hpp"
#include "brute_force.hpp"
#include "common.h"
#include "rgf.h"
#include "stresstensor.hpp"

using namespace std;

double stresstensor_determinant (const STRESSTENSOR& st) {

	return	(st._11 * st._22 * st._33) +
			(st._12 * st._23 * st._13) +
			(st._13 * st._12 * st._23) -
			(st._13 * st._22 * st._13) -
			(st._12 * st._12 * st._33) -
			(st._11 * st._23 * st._23);
}

void check_stress_tensor_singularity(const STRESSTENSOR& st) {

	double det = stresstensor_determinant (st);

	ASSERT2(fabs(det) > 1.0e-25, "Stress tensor nearly singluar, determinant = "<< det);
}

STRESSTENSOR fix_stress_tensor_singularity(STRESSTENSOR& st) {

	const double one_plus_tiny = 1 + 1.0e-4;

	double det = stresstensor_determinant (st);

	if (det < 10e-25) {

		st._11 *= one_plus_tiny;
		st._22 *= one_plus_tiny;
		st._33 *= one_plus_tiny;
	}
	return st;
}

STRESSFIELD eigenvalue_eigenvector (STRESSTENSOR st) {

	st =  fix_stress_tensor_singularity (st);

	check_stress_tensor_singularity (st);

	STRESSFIELD sf;

	vector < double > X;

	const double A =   1.0;

	const double B = - (st._11 + st._22 + st._33);

	const double C =   (st._11 * st._22) + (st._22 * st._33) + (st._11 * st._33) - (st._12 * st._12) - (st._23 * st._23) - (st._13 * st._13);

	const double D = - ((st._11 * st._22 * st._33) + (2.0 * st._12 * st._23 * st._13) - (st._12 * st._12 * st._33) - (st._23 * st._23 * st._11) - (st._13 * st._13 * st._22));

	X = cubic_solution (A, B, C, D);

	sort(X.begin(), X.begin()+3);

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
	sf.EIGENVECTOR1.X = ((b1 * c2) - (b2 * c1)) / ((b2 * a1) - (a2 * b1));
	sf.EIGENVECTOR1.Y = - ((a1 * sf.EIGENVECTOR1.X) + c1) / b1;
	sf.EIGENVECTOR1 = unitvector (sf.EIGENVECTOR1);

	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR1.X)) sf.EIGENVECTOR1.X = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR1.Y)) sf.EIGENVECTOR1.Y = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR1.Z)) sf.EIGENVECTOR1.Z = 1.0 - 1E-8;
	sf.EIGENVECTOR1 = unitvector (sf.EIGENVECTOR1);



	a1 = st._11 - sf.EIGENVALUE.Y;
	b2 = st._22 - sf.EIGENVALUE.Y;

	sf.EIGENVECTOR2.Z = 1.0;

	double denom = (b2 * a1) - (a2 * b1);

	if (fabs(denom) < 1.0e-20) {
		ASSERT2(false, "Computing eigenvector, small denom = "<< denom);
	}

	sf.EIGENVECTOR2.X = ((b1 * c2) - (b2 * c1)) / denom;
	sf.EIGENVECTOR2.Y = - ((a1 * sf.EIGENVECTOR2.X) + c1) / b1;

	ASSERT2(fabs(b1)>1.0e-20, "Computing eigenvector, b1 = "<< b1);

	sf.EIGENVECTOR2 = unitvector (sf.EIGENVECTOR2);

	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR2.X)) sf.EIGENVECTOR2.X = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR2.Y)) sf.EIGENVECTOR2.Y = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR2.Z)) sf.EIGENVECTOR2.Z = 1.0 - 1E-8;
	sf.EIGENVECTOR2 = unitvector (sf.EIGENVECTOR2);



	a1 = st._11 - sf.EIGENVALUE.Z;
	b2 = st._22 - sf.EIGENVALUE.Z;

	sf.EIGENVECTOR3.Z = 1.0;
	sf.EIGENVECTOR3.X = ((b1 * c2) - (b2 * c1)) / ((b2 * a1) - (a2 * b1));
	sf.EIGENVECTOR3.Y = - ((a1 * sf.EIGENVECTOR3.X) + c1) / b1;
	sf.EIGENVECTOR3 = unitvector (sf.EIGENVECTOR3);

	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR3.X)) sf.EIGENVECTOR3.X = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR3.X)) sf.EIGENVECTOR3.Y = 1.0 - 1E-8;
	if (is_in_range(0.9999, 1.0001, sf.EIGENVECTOR3.X)) sf.EIGENVECTOR3.Z = 1.0 - 1E-8;
	sf.EIGENVECTOR3 = unitvector (sf.EIGENVECTOR3);

	return sf;
}

STRESSTENSOR stresstensor_from_eigenvalue_eigenvector (STRESSFIELD sf) {

	vector < vector <double> >  D = init_matrix (3);
	vector < vector <double> >  T = init_matrix (3);

	STRESSTENSOR out;

	D.at(0).at(0) = sf.EIGENVECTOR1.X;
	D.at(0).at(1) = sf.EIGENVECTOR1.Y;
	D.at(0).at(2) = sf.EIGENVECTOR1.Z;

	D.at(1).at(0) = sf.EIGENVECTOR2.X;
	D.at(1).at(1) = sf.EIGENVECTOR2.Y;
	D.at(1).at(2) = sf.EIGENVECTOR2.Z;

	D.at(2).at(0) = sf.EIGENVECTOR3.X;
	D.at(2).at(1) = sf.EIGENVECTOR3.Y;
	D.at(2).at(2) = sf.EIGENVECTOR3.Z;


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

VCTR return_stressvector (const STRESSTENSOR& st, const VCTR& N) {

	const VCTR out = declare_vector (
			(st._11 * N.X + st._12 * N.Y + st._13 * N.Z),
			(st._12 * N.X + st._22 * N.Y + st._23 * N.Z),
			(st._13 * N.X + st._23 * N.Y + st._33 * N.Z));

	//was: if (! compression_positive)	out = invert_VCTR(out);
	//and called as compression_positive = false;
	return flip_vector (out);
}

VCTR return_normalstress (const STRESSTENSOR& st, const VCTR& N) {

	const VCTR stressvector = return_stressvector (st, N);

	const double stress = (N.X * stressvector.X) + (N.Y * stressvector.Y) + (N.Z * stressvector.Z);

	return (declare_vector (N.X * stress, N.Y * stress, N.Z * stress));
}

VCTR return_shearstress (const STRESSTENSOR& st, const VCTR& N) {

	const VCTR stressvector = return_stressvector (st, N);

	const VCTR normalstress = return_normalstress (st, N);

	return (declare_vector(
			stressvector.X - normalstress.X,
			stressvector.Y - normalstress.Y,
			stressvector.Z - normalstress.Z));
}

VCTR return_upsilon (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV, const VCTR& UPSILON, const double& lambda, const string& method) {

	const VCTR shearstress = return_shearstress (st, N);

	if (method == "ANGELIER")

		return declare_vector(
				(SV.X * lambda) - shearstress.X,
				(SV.Y * lambda) - shearstress.Y,
				(SV.Z * lambda) - shearstress.Z);
	else

		return declare_vector(
				(UPSILON.X * lambda) - shearstress.X,
				(UPSILON.Y * lambda) - shearstress.Y,
				(UPSILON.Z * lambda) - shearstress.Z);
}

double return_ANG (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV) {

	const VCTR shearstress = return_shearstress (st, N);

	//return ACOS_NUM (dotproduct (SV, shearstress, true)); //was ACOS!!!
	//check WHY ACOS_NUM was used here!

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

		//was: double ang = return_ANG (st, inGDB.at(i).N, inGDB.at(i)._SV);
		//was: misfit = misfit + ang;
	}
	return misfit / inGDB.size();
}

vector <GDB> return_stressvector_estimators (const STRESSTENSOR& st, const vector <GDB>& inGDB, const string& method) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		VCTR N = inGDB.at(i).N;

		VCTR SV = inGDB.at(i).DC;
		//was: VCTR SV = inGDB.at(i)._SV;

		VCTR UPSILON = 	inGDB.at(i).UPSILON;
		double lambda = inGDB.at(i).lambda;

		outGDB.at(i).SHEAR_S  = return_shearstress  (st, N);

		outGDB.at(i).NORMAL_S = return_normalstress (st, N);

		outGDB.at(i).UPSILON  = return_upsilon (st, N, SV, UPSILON, lambda , method);

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
