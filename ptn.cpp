// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

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

		const bool SC = is_allowed_SC_datatype(DT);
		const bool STRIAE = is_allowed_striae_datatype(DT);
		const bool HAS_OFFSET = !is_allowed_striae_none_sense(O);

		if ((SC || STRIAE) && HAS_OFFSET) {

			VCTR temp1 = declare_vector(
					q * inGDB.at(i).N.X + r * inGDB.at(i).SV.X,
					q * inGDB.at(i).N.Y + r * inGDB.at(i).SV.Y,
					q * inGDB.at(i).N.Z + r * inGDB.at(i).SV.Z);
			temp1 = unitvector (temp1);
			temp1 = flip_D_vector (temp1);
			DIPDIR_DIP dd = dipdir_dip_from_DXDYDZ (temp1);
			outGDB.at(i).ptnT = temp1;
			outGDB.at(i).ptnTd = dd;

			VCTR temp2 = declare_vector(
					q * inGDB.at(i).SV.X - r * inGDB.at(i).N.X,
					q * inGDB.at(i).SV.Y - r * inGDB.at(i).N.Y,
					q * inGDB.at(i).SV.Z - r * inGDB.at(i).N.Z);
			temp2 = unitvector (temp2);
			temp2 = flip_D_vector (temp2);
			dd = dipdir_dip_from_DXDYDZ (temp2);
			outGDB.at(i).ptnP = temp2;
			outGDB.at(i).ptnPd = dd;

			VCTR temp3 = crossproduct (temp2, temp1);
			temp3 = unitvector (temp3); // FIXME What if temp3 has approximately 0 length?
			temp3 = flip_D_vector (temp3);
			dd = dipdir_dip_from_DXDYDZ (temp3);
			outGDB.at(i).ptnN = temp3;
			outGDB.at(i).ptnNd = dd;
		}
	}
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
	return st;
}

STRESSFIELD sf_PTN (const vector <GDB>& inGDB) {

	STRESSFIELD sf, sf_ptn;

	vector <GDB> processGDB = inGDB;

	processGDB = ptn (processGDB);

	//if (inset.virt_striae == "Y" ) 	processGDB = generate_virtual_striae (processGDB);

	sf_ptn = computestressfield_DXDYDZ (eigenvalue_eigenvector (PTN_matrix (processGDB, "P")));
	sf.EIGENVALUE.X = sf_ptn.EIGENVALUE.X;
	sf.EIGENVECTOR1 = sf_ptn.EIGENVECTOR1;
	sf.S_1 = sf_ptn.S_1;

	sf_ptn = computestressfield_DXDYDZ (eigenvalue_eigenvector (PTN_matrix (processGDB, "T")));
	sf.EIGENVALUE.Z = sf_ptn.EIGENVALUE.Z;
	sf.EIGENVECTOR3 = sf_ptn.EIGENVECTOR1;
	sf.S_3 = sf_ptn.S_1;

	sf_ptn = computestressfield_DXDYDZ (eigenvalue_eigenvector (PTN_matrix (processGDB, "N")));
	sf.EIGENVALUE.Y = sf_ptn.EIGENVALUE.Y;
	sf.EIGENVECTOR2 = sf_ptn.EIGENVECTOR1;
	sf.S_2 = sf_ptn.S_1;

	return stress_regime (sf);
}

STRESSTENSOR st_PTN (const STRESSFIELD& sf) {

	return stresstensor_from_eigenvalue_eigenvector (sf);
}
