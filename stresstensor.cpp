
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

	sf.S_1 = dipdir_dip_from_NXNYNZ (sf.EIGENVECTOR1);
	sf.S_2 = dipdir_dip_from_NXNYNZ (sf.EIGENVECTOR2);
	sf.S_3 = dipdir_dip_from_NXNYNZ (sf.EIGENVECTOR3);

	return sf;
}

STRESSFIELD computestressfield_DXDYDZ (const STRESSFIELD& in) {

	STRESSFIELD sf = in;

	sf.S_1 = dipdir_dip_from_DXDYDZ (sf.EIGENVECTOR1);
	sf.S_2 = dipdir_dip_from_DXDYDZ (sf.EIGENVECTOR2);
	sf.S_3 = dipdir_dip_from_DXDYDZ (sf.EIGENVECTOR3);

	return sf;
}

VCTR return_stressvector (const STRESSTENSOR& st, const VCTR& N) {

	const VCTR out = declare_vector (
			(st._11 * N.X + st._12 * N.Y + st._13 * N.Z),
			(st._12 * N.X + st._22 * N.Y + st._23 * N.Z),
			(st._13 * N.X + st._23 * N.Y + st._33 * N.Z));

	//was: if (! compression_positive)	out = invert_VCTR(out);
	//and called as compression_positive = false;
	return invert_VCTR (out);
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

	return ACOS (dotproduct (SV, shearstress, true)); //was ACOS!!!
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

		////double ang = return_ANG (st, inGDB.at(i).N, inGDB.at(i).SV);
		////misfit = misfit + ang;
	}
	return misfit / inGDB.size();
}

vector <GDB> return_stressvector_estimators (const STRESSTENSOR& st, const vector <GDB>& inGDB, const string& method) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		VCTR N = 	inGDB.at(i).N;
		VCTR SV;//// = 	inGDB.at(i).SV;
		VCTR UPSILON = 	inGDB.at(i).UPSILON;
		double lambda = inGDB.at(i).lambda;

		outGDB.at(i).SHEAR_S  = return_shearstress  (st, N);

		outGDB.at(i).NORMAL_S = return_normalstress (st, N);

		outGDB.at(i).UPSILON  = return_upsilon (st, N, SV, UPSILON, lambda , method);

		outGDB.at(i).ANG  = return_ANG (st, N, SV);

		outGDB.at(i).RUP  = return_RUP (st, N, SV, lambda);

		if (method == "MOSTAFA")

			outGDB.at(i).lambda =  sqrt(
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
