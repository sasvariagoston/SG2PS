// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef FRY_H
#define FRY_H

using namespace std;

bool check_fry_matrix (const size_t frst_eigenvalue, const vector <vector <double> >& in_eigenvector);

bool fry_correct (const vector <GDB>& inGDB);

vector <double> hyperplane_from_GDB (const GDB& inGDB);

vector <vector <double> > FRY_matrix (const vector <GDB>& inGDB);

STRESSTENSOR st_FRY (const vector <GDB>& inGDB);

STRESSFIELD sf_FRY (const STRESSTENSOR& st);

STRESSFIELD INVERSION_FRY (const vector <GDB>& inGDB);

#endif
