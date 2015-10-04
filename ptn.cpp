// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
S1: 252/83 (-0.117481, -0.038418, -0.992332)
S2: 053/07 ( 0.796970,  0.591015, -0.124660)
S3: 143/01 ( 0.600084, -0.799859, -0.011146)
EIGENVALUES: 34.980318, 5.061414, 0.959156

S1: 018/17 ( 0.293015,  0.912436, -0.285662)
S2: 210/73 (-0.150925, -0.258344, -0.954191)
S3: 110/03 ( 0.936697, -0.346976, -0.046974)
EIGENVALUES: 34.980318, 5.061414, 0.959156
*/

#include <cmath>
#include <iomanip>
#include <iostream>

#include "allowed_keys.hpp"
#include "common.h"
#include "inversion.h"
#include "ptn.h"
#include "rgf.h"
#include "settings.hpp"
#include "stresstensor.hpp"
#include "structs.h"

using namespace std;

vector <GDB> ptn (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	const double r = is_STRESSANGLE() / 90.0;
	const double q = 1.0 - r;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const string DT = inGDB.at(i).DATATYPE;
		const string O = inGDB.at(i).OFFSET;

		const bool STRIAE = is_allowed_striae_datatype(DT);
		const bool HAS_OFFSET = !is_allowed_striae_none_sense(O);

		if (STRIAE && HAS_OFFSET) {

			outGDB.at(i).ptnT = unitvector(declare_vector (
					q * inGDB.at(i).N.X + r * inGDB.at(i).DC.X,
					q * inGDB.at(i).N.Y + r * inGDB.at(i).DC.Y,
					q * inGDB.at(i).N.Z + r * inGDB.at(i).DC.Z));

			outGDB.at(i).ptnP = unitvector (declare_vector (
					q * inGDB.at(i).DC.X - r * inGDB.at(i).N.X,
					q * inGDB.at(i).DC.Y - r * inGDB.at(i).N.Y,
					q * inGDB.at(i).DC.Z - r * inGDB.at(i).N.Z));

			outGDB.at(i).ptnN = unitvector (crossproduct (outGDB.at(i).ptnP, outGDB.at(i).ptnT));
		}
	}

	//dbg_cout_GDB_vector (outGDB);

	return outGDB;
}

STRESSTENSOR PTN_matrix (const vector <GDB>& inGDB, const string& axis) {

	STRESSTENSOR st;

	st._11 = 0.0;
	st._12 = 0.0;
	st._13 = 0.0;
	st._22 = 0.0;
	st._23 = 0.0;
	st._33 = 0.0;

	const VCTR N = declare_vector (0.0, 1.0, 0.0);
	const VCTR E = declare_vector (1.0, 0.0, 0.0);
	const VCTR U = declare_vector (0.0, 0.0, 1.0);

	VCTR AX;

	for (size_t i = 0; i < inGDB.size(); i++) {

		if 		(axis == "P") AX = inGDB.at(i).ptnP;
		else if (axis == "T") AX = inGDB.at(i).ptnT;
		else if (axis == "N") AX = inGDB.at(i).ptnN;
		else {};

		st._11 = st._11 + (dotproduct (AX, E, false) * dotproduct (AX, E, false));
		st._12 = st._12 + (dotproduct (AX, E, false) * dotproduct (AX, N, false));
		st._13 = st._13 + (dotproduct (AX, E, false) * dotproduct (AX, U, false));
		st._22 = st._22 + (dotproduct (AX, N, false) * dotproduct (AX, N, false));
		st._23 = st._23 + (dotproduct (AX, N, false) * dotproduct (AX, U, false));
		st._33 = st._33 + (dotproduct (AX, U, false) * dotproduct (AX, U, false));
	}
	//cout_dbg_stresstensor(st);

	return st;
}

STRESSFIELD sf_PTN (const vector <GDB>& inGDB) {

	STRESSFIELD sf, sf_ptn;

	vector <GDB> processGDB = inGDB;

	processGDB = ptn (processGDB);

	sf_ptn = eigenvalue_eigenvector (PTN_matrix (processGDB, "P"));
	sf.EIGENVALUE.X = sf_ptn.EIGENVALUE.X;
	sf.EIGENVECTOR1 = flip_vector (sf_ptn.EIGENVECTOR1);

	sf_ptn = eigenvalue_eigenvector (PTN_matrix (processGDB, "T"));
	sf.EIGENVALUE.Z = sf_ptn.EIGENVALUE.Z;
	sf.EIGENVECTOR3 = flip_vector (sf_ptn.EIGENVECTOR1);

	sf_ptn = eigenvalue_eigenvector (PTN_matrix (processGDB, "N"));
	sf.EIGENVALUE.Y = sf_ptn.EIGENVALUE.Y;
	sf.EIGENVECTOR2 = flip_vector (sf_ptn.EIGENVECTOR1);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ(sf);

	sf = stress_regime (sf);

	return sf;
}

STRESSTENSOR st_PTN (const STRESSFIELD& sf) {

	return stresstensor_from_eigenvalue_eigenvector (sf);
}
