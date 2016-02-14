// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef STRESSTENSOR_HPP_
#define STRESSTENSOR_HPP_

#include "rgf.h"

double stresstensor_determinant (const STRESSTENSOR& st);
void check_stress_tensor_singularity(const STRESSTENSOR& st);
STRESSTENSOR fix_stress_tensor_singularity(STRESSTENSOR& st);

STRESSFIELD eigenvalue_eigenvector (STRESSTENSOR st);
STRESSTENSOR stresstensor_from_eigenvalue_eigenvector (STRESSFIELD sf);

STRESSFIELD stress_regime (const STRESSFIELD& in);
STRESSTENSOR convert_matrix_to_stresstensor (const vector <vector <double> >& IN);

STRESSTENSOR try_stresstensot_or_invert (const STRESSTENSOR& st, const vector <GDB>& inGDB);
STRESSTENSOR invert_stress_tensor (const STRESSTENSOR& st);
STRESSTENSOR add_stress_tensor (const STRESSTENSOR& st1, const STRESSTENSOR& st2);

STRESSFIELD computestressfield_NXNYNZ (const STRESSFIELD& in);
STRESSFIELD computestressfield_DXDYDZ (const STRESSFIELD& in);

STRESSFIELD correct_SF_to_fit_D (const STRESSFIELD& in);
STRESSFIELD correct_SF_to_fit_N (const STRESSFIELD& in);

VCTR return_stressvector (const STRESSTENSOR& st, const VCTR& N);
VCTR return_normalstress (const STRESSTENSOR& st, const VCTR& N);
VCTR return_shearstress (const STRESSTENSOR& st, const VCTR& N);

double return_ANG (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV);
double return_RUP (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV, const double& lambda);

double return_average_misfit (const STRESSTENSOR& st, const vector <GDB>& inGDB);

vector <GDB> return_stressvector_estimators (const STRESSTENSOR& st, const vector <GDB>& inGDB, const string& method); //ok

STRESSTENSOR return_stresstensor_from_n1_ang_phi (const VCTR& N1, const double& ANG, const double& PHI);

#endif
