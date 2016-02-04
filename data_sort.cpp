// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <iostream>

#include "assertions.hpp"
#include "data_sort.hpp"
#include "structs.h"

bool by_ID(const GDB& x, const GDB& y) {

	return x.ID < y.ID;
}
bool by_rev_ID(const GDB& x, const GDB& y) {

	return x.ID > y.ID;
}

bool by_iID(const GDB& x, const GDB& y) {

	return x.iID < y.iID;
}
bool by_rev_iID(const GDB& x, const GDB& y) {

	return x.iID > y.iID;
}

bool by_DEPTH (const GDB& x, const GDB& y) {

	return x.DEPTH < y.DEPTH;
}
bool by_rev_DEPTH (const GDB& x, const GDB& y) {

	return x.DEPTH > y.DEPTH;
}

bool by_GC (const GDB& x, const GDB& y) {

	return x.GC.at(0) < y.GC.at(0);
}
bool by_rev_GC (const GDB& x, const GDB& y) {

	return x.GC.at(0) > y.GC.at(0);
}

bool by_CLUSTER (const GDB& x, const GDB& y) {

	return x.GC.at(1) < y.GC.at(1);
}
bool by_rev_CLUSTER (const GDB& x, const GDB& y) {

	return x.GC.at(1) > y.GC.at(1);
}

bool by_DT (const GDB& x, const GDB& y) {

	return x.DATATYPE < y.DATATYPE;
}
bool by_rev_DT (const GDB& x, const GDB& y) {

	return x.DATATYPE > y.DATATYPE;
}

bool by_FM (const GDB& x, const GDB& y) {

	return x.FORMATION < y.FORMATION;
}
bool by_rev_FM (const GDB& x, const GDB& y) {

	return x.FORMATION > y.FORMATION;
}

bool by_LOC (const GDB& x, const GDB& y) {

	return x.LOC < y.LOC;
}
bool by_rev_LOC (const GDB& x, const GDB& y) {

	return x.LOC > y.LOC;
}

bool by_RUP_ANG(const GDB& x, const GDB& y) {

	return x.GC.at(2) < y.GC.at(2);
}

bool by_RUP(const GDB& x, const GDB& y) {

	return x.RUP < y.RUP;
}

bool by_ANG(const GDB& x, const GDB& y) {

	return x.ANG < y.ANG;
}

bool by_LocGcType (const GDB& x, const GDB& y) {

	if (x.LOC != y.LOC) return x.LOC < y.LOC;
	if (x.GC != y.GC) return x.GC < y.GC;
	return x.DATATYPE < y.DATATYPE;
}

bool byLocTypeGc (const GDB& x, const GDB& y) {

	if (x.LOC != y.LOC) return x.LOC < y.LOC;
	if (x.DATATYPE != y.DATATYPE) return x.DATATYPE < y.DATATYPE;
	return x.GC < y.GC;
}

bool byLocFmType (const GDB& x, const GDB& y) {

	if (x.LOC != y.LOC) return x.LOC < y.LOC;
	if (x.FORMATION != y.FORMATION) return x.FORMATION < y.FORMATION;
	return x.DATATYPE < y.DATATYPE;
}

vector <GDB> SORT_GDB (const vector <GDB>& inGDB, const string SORT) {

	vector <GDB> P = inGDB;

	if (SORT == "ID") 				stable_sort (P.begin(), P.end(), by_ID);
	else if (SORT == "rID") 		stable_sort (P.begin(), P.end(), by_rev_ID);

	else if (SORT == "IID")			stable_sort (P.begin(), P.end(), by_iID);
	else if (SORT == "rIID")		stable_sort (P.begin(), P.end(), by_rev_iID);

	else if (SORT == "DEPTH") 		stable_sort (P.begin(), P.end(), by_DEPTH);
	else if (SORT == "rDEPTH")		stable_sort (P.begin(), P.end(), by_rev_DEPTH);

	else if (SORT == "LOC_GC_TYPE")	stable_sort (P.begin(), P.end(), by_LocGcType);

	else if (SORT == "GROUPCODE")	stable_sort (P.begin(), P.end(), by_GC);
	else if (SORT == "rGROUPCODE")	stable_sort (P.begin(), P.end(), by_rev_GC);

	else if (SORT == "CLUSTER")		stable_sort (P.begin(), P.end(), by_CLUSTER);
	else if (SORT == "rCLUSTER")	stable_sort (P.begin(), P.end(), by_rev_CLUSTER);

	else if (SORT == "DATATYPE")	stable_sort (P.begin(), P.end(), by_DT);
	else if (SORT == "rDATATYPE")	stable_sort (P.begin(), P.end(), by_rev_DT);

	else if (SORT == "FORMATION")	stable_sort (P.begin(), P.end(), by_FM);
	else if (SORT == "rFORMATION")	stable_sort (P.begin(), P.end(), by_rev_FM);

	else if (SORT == "LOCATION")	stable_sort (P.begin(), P.end(), by_LOC);
	else if (SORT == "rLOCATION")	stable_sort (P.begin(), P.end(), by_rev_LOC);

	else if (SORT == "RUP_ANG")		stable_sort (P.begin(), P.end(), by_RUP_ANG);

	else if (SORT == "RUP")			stable_sort (P.begin(), P.end(), by_RUP);
	else if (SORT == "ANG")			stable_sort (P.begin(), P.end(), by_ANG);

	else {

		cout << SORT << endl;
		ASSERT_DEAD_END();
	}
	return P;
}

bool byeigenvalue(const sort_jacobi& x, const sort_jacobi& y) {

	return x.eigenvalue < y.eigenvalue;
}

bool by_DEPTH_trj (const TRJ& x, const TRJ& y) {

	return x.DEPTH < y.DEPTH;
}
bool by_rev_DEPTH_trj (const TRJ& x, const TRJ& y) {

	return x.DEPTH > y.DEPTH;
}

vector <TRJ> SORT_TRJ (const vector <TRJ>& inTRJ, const string SORT) {

	ASSERT_ONE_OF(SORT, "DEPTH", "rDEPTH");

	vector <TRJ> P = inTRJ;

	if (SORT == "DEPTH") 	stable_sort (P.begin(), P.end(), by_DEPTH_trj);
	else 					stable_sort (P.begin(), P.end(), by_rev_DEPTH_trj);

	return P;
}

bool by_C(const RUP_table& x, const RUP_table& y) {

	return x.C < y.C;
}

vector <RUP_table> sort_by_C (const vector <RUP_table>& RT) {

	vector <RUP_table> OUT = RT;

	stable_sort (OUT.begin(), OUT.end(), by_C);

	return OUT;
}
