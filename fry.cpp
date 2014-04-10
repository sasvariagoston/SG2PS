// Copyright (C) 2012 - 20143 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "inversion.h"
#include "fry.h"
#include "rgf.h"

using namespace std;

namespace {

size_t second_eigenvalue = 0;
size_t first_eigenvalue = 0;

vector <vector <double> > A = init_matrix (6);
vector <vector <double> > D = init_matrix (6);

vector <vector <double> > EVEV;

}

vector <vector <double> > FRY_matrix (vector <GDB> inGDB, INPSET inset) {

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

bool fry_correct (vector <GDB> inGDB, INPSET inset) {

	EVEV = FRY_matrix (inGDB, inset);

	A = generate_A (EVEV);
	D = generate_D (EVEV);

	first_eigenvalue = return_first_eigenvalue (A);
	second_eigenvalue = return_second_eigenvalue (A);

	if (check_fry_matrix (first_eigenvalue, D)) return true;
	return false;
}

STRESSTENSOR st_FRY (vector <GDB> inGDB) {

	STRESSTENSOR st;

	double misfit1 = 0;
	double misfit2 = 0;

	st._11 = D.at(second_eigenvalue).at(0);
	st._22 = D.at(second_eigenvalue).at(1);
	st._33 = D.at(second_eigenvalue).at(2);
	st._12 = D.at(second_eigenvalue).at(3);
	st._23 = D.at(second_eigenvalue).at(4);
	st._13 = D.at(second_eigenvalue).at(5);

	misfit1 = return_average_misfit (st, inGDB, false);

	st = invert_stress_tensor (st);

	misfit2 = return_average_misfit (st, inGDB, false);

	if (misfit1 < misfit2) return invert_stress_tensor (st);
	return st;
}

STRESSFIELD sf_FRY (STRESSTENSOR st) {

	return eigenvalue_eigenvector (st);
}
