
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef STRESSTENSOR_HPP_
#define STRESSTENSOR_HPP_

STRESSTENSOR invert_stress_tensor (const STRESSTENSOR& st); //ok
STRESSTENSOR add_stress_tensor (const STRESSTENSOR& st1, const STRESSTENSOR& st2); //ok

STRESSFIELD computestressfield_NXNYNZ (const STRESSFIELD& in); //ok
STRESSFIELD computestressfield_DXDYDZ (const STRESSFIELD& in); //ok

VCTR return_stressvector (const STRESSTENSOR& st, const VCTR& N); //ok
VCTR return_normalstress (const STRESSTENSOR& st, const VCTR& N); //ok
VCTR return_shearstress (const STRESSTENSOR& st, const VCTR& N); //ok
VCTR return_upsilon (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV, const VCTR& UPSILON, const double& lambda, const string& method);

double return_ANG (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV);
double return_RUP (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV, const double& lambda);

double return_average_misfit (const STRESSTENSOR& st, const vector <GDB>& inGDB);

vector <GDB> return_stressvector_estimators (const STRESSTENSOR& st, const vector <GDB>& inGDB, const string& method); //ok

STRESSTENSOR return_stresstensor_from_n1_ang_phi (const VCTR& N1, const double& ANG, const double& PHI);

#endif
