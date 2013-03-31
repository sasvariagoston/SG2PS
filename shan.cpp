// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "rgf.h"
#include "shan.h"

using namespace std;

vector <vector <double> > SHAN_matrix (vector <GDB> inGDB, INPSET inset) {

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


STRESSTENSOR st_SHAN (vector <GDB> inGDB, INPSET inset) {

	vector <vector <double> > A = init_matrix (5);
	vector <vector <double> > D = init_matrix (5);
	vector <vector <double> > EVEV = SHAN_matrix (inGDB, inset);

	int first_eigenvalue = 0;
	double misfit1 = 0.0;
	double misfit2 = 0.0;

	STRESSTENSOR st;

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


	if (misfit1 < misfit2) return invert_stress_tensor (st);
	else return st;
}

STRESSFIELD sf_SHAN (STRESSTENSOR st) {

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
}
