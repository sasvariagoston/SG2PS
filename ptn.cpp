// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "inversion.h"
#include "ptn.h"
#include "rgf.h"
#include "structs.h"

using namespace std;

STRESSTENSOR PTN_matrix (vector <GDB> inGDB, string axis) {

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
	VCTR AX;

	do {

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

		i++;

	} while (i < inGDB.size());

	return st;
}


STRESSTENSOR st_PTN (STRESSFIELD sf) {

	return stresstensor_from_eigenvalue_eigenvector (sf);
}

STRESSFIELD sf_PTN (vector <GDB> inGDB, INPSET inset) {

	vector <GDB> processGDB;
	STRESSFIELD sf, sf_ptn;

	if (inset.virt_striae == "Y" ) 	processGDB = generate_virtual_striae (inGDB);
	else 							processGDB = inGDB;

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
