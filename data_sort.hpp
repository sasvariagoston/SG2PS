// Copyright (C) 2012-2016,  Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef DATA_SORT_HPP_
#define DATA_SORT_HPP_

#include "structs.h"

bool by_ID(const GDB& x, const GDB& y);
bool by_rev_ID(const GDB& x, const GDB& y);

bool by_iID(const GDB& x, const GDB& y);
bool by_rev_iID(const GDB& x, const GDB& y);

bool by_DEPTH (const GDB& x, const GDB& y);
bool by_rev_DEPTH (const GDB& x, const GDB& y);

bool by_GC (const GDB& x, const GDB& y);
bool by_rev_GC (const GDB& x, const GDB& y);

bool by_DT (const GDB& x, const GDB& y);
bool by_rev_DT (const GDB& x, const GDB& y);

//bool by_FM (const GDB& x, const GDB& y);
//bool by_rev_FM (const GDB& x, const GDB& y);

bool by_LOC (const GDB& x, const GDB& y);
bool by_rev_LOC (const GDB& x, const GDB& y);

bool by_RUP_ANG(const GDB& x, const GDB& y);
bool by_RUP(const GDB& x, const GDB& y);
bool by_ANG(const GDB& x, const GDB& y);

bool by_LocGcType (const GDB& x, const GDB& y);
bool byLocTypeGc(const GDB& x, const GDB& y);
bool byLocFmType (const GDB& x, const GDB& y);

bool byeigenvalue(const sort_jacobi& x, const sort_jacobi& y);

vector <GDB> SORT_GDB (const vector <GDB>& inGDB, const string SORT);

bool by_DEPTH_trj (const TRJ& x, const TRJ& y);
bool by_rev_DEPTH_trj (const TRJ& x, const TRJ& y);

vector <TRJ> SORT_TRJ (const vector <TRJ>& inTRJ, const string SORT);

bool by_C(const RUP_table& x, const RUP_table& y);
vector <RUP_table> sort_by_C (const vector <RUP_table>& RT);

#endif /* DATA_SORT_HPP_ */
