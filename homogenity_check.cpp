// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <iostream>

#include "angelier.h"
#include "assertions.hpp"
#include "allowed_keys.hpp"
#include "brute_force.hpp"
#include "bingham.h"
#include "common.h"
#include "data_sort.hpp"
#include "fry.h"
#include "michael.h"
#include "mostafa.h"
#include "nda.h"
#include "ptn.h"
#include "rgf.h"
#include "settings.hpp"
#include "shan.h"
#include "structs.h"

using namespace std;

size_t minimum_independent_dataset () {

	if 		(is_INVERSION_SPRANG()) 	return 6;//ok
	else if (is_INVERSION_TURNER()) 	return 6;//ok
	else if (is_INVERSION_MICHAEL()) 	return 5;//ok
	else if (is_INVERSION_SHAN()) 		return 5;//ok
	else if (is_INVERSION_FRY()) 		return 6;//ok
	else if (is_INVERSION_ANGELIER()) 	return 4;//ok
	else if (is_INVERSION_MOSTAFA()) 	return 4;//ok
	else if (is_INVERSION_BRUTEFORCE()) return 6;//ok
	else if (is_BINGHAM_USE()) 			return 6;//ok
	else return 1;
}

bool check_dataset_offset_homogenity (const vector <GDB>& inGDB) {

	vector <GDB> TEST = inGDB;
	const size_t SIZE = TEST.size() - 1;

	stable_sort (TEST.begin(), TEST.end(), byOFFSET);

	const string ofs1 = TEST.at(0).OFFSET;
	const string ofs2 = inGDB.at(SIZE).OFFSET;

	return (ofs1 != ofs2);
}

bool check_dataset_geometry_homogenity (const vector <GDB>& inGDB) {

	if (is_allowed_lithology_datatype (inGDB.at(0).DATATYPE)) return true;

	vector <GDB> TEST = inGDB;
	const size_t SIZE = TEST.size() - 1;

	const bool STRIAE = 	is_allowed_striae_datatype(TEST.at(0).DATATYPE);
	const bool SC = 		is_allowed_SC_datatype(TEST.at(0).DATATYPE);

	if (STRIAE || SC) stable_sort (TEST.begin(), TEST.end(), bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP);
	else stable_sort (TEST.begin(), TEST.end(), bycorrDIPDIRcorrDIP);

	check_RGF_for_NAN_INF (TEST);

	const double minDD = TEST.at(0).corr.DIPDIR;
	const double maxDD = TEST.at(SIZE).corr.DIPDIR;
	const double var1 = fabs(maxDD - minDD);

	const double minD = TEST.at(0).corr.DIP;
	const double maxD = TEST.at(SIZE).corr.DIP;
	const double var2 = fabs(maxD - minD);

	if (STRIAE || SC) {

		const double minLDD = TEST.at(0).corrL.DIPDIR;
		const double maxLDD = TEST.at(SIZE).corrL.DIPDIR;
		const double var3 = fabs(maxLDD - minLDD);

		const double minLD = TEST.at(0).corrL.DIP;
		const double maxLD = TEST.at(SIZE).corrL.DIP;
		const double var4 = fabs(maxLD - minLD);

		ASSERT_FINITE (var1, var2, var3, var4);

		return (var1 > 0.1 || var2 > 0.1 || var3 > 0.1 || var4 > 0.1);
	}
	ASSERT_FINITE (var1, var2);

	return (var1 > 0.1 || var2 > 0.1);
}

bool check_dataset_DEPTH_homogenity (const vector <GDB>& inGDB) {

	ASSERT_GT (inGDB.size(), 0);

	vector <GDB> TEST = inGDB;

	TEST = SORT_GDB (TEST, "DEPTH");

	const size_t SIZE = TEST.size() - 1;

	for (size_t i = 0; i < TEST.size(); i++) {

		ASSERT_FINITE (TEST.at(i).DEPTH);
	}

	const double minDPT = TEST.at(0).DEPTH;
	const double maxDPT = TEST.at(SIZE).DEPTH;
	const double var1 = fabs(maxDPT - minDPT);

	ASSERT_FINITE (var1);

	return (var1 < 0.1);
}

bool check_dataset_homogenity (const vector <GDB>& inGDB) {

	const bool STRIAE = 	(is_allowed_striae_datatype(inGDB.at(0).DATATYPE));
	const bool SC = 		(is_allowed_SC_datatype(inGDB.at(0).DATATYPE));
	const bool BEDDING = 	is_allowed_handle_as_bedding (inGDB.at(0).DATATYPE);

	const bool OFFSET = 	check_dataset_offset_homogenity (inGDB);
	const bool GEOMETRY = 	check_dataset_geometry_homogenity (inGDB);

	if (SC || STRIAE ) return (GEOMETRY || OFFSET);
	else if (BEDDING) return (GEOMETRY || !OFFSET);
	else return (GEOMETRY);
}

vector <GDB> return_GDB_with_no_homogeneous_data (const vector <GDB>& inGDB) {

	vector <GDB> resultGDB;
	vector <GDB> processGDB = inGDB;

	const bool STRIAE = 	(is_allowed_striae_datatype(processGDB.at(0).DATATYPE));
	const bool SC = 		(is_allowed_SC_datatype(processGDB.at(0).DATATYPE));

	check_RGF_for_NAN_INF (processGDB);

	if (SC || STRIAE) stable_sort (processGDB.begin(), processGDB.end(), bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP);

	else stable_sort (processGDB.begin(), processGDB.end(), bycorrDIPDIRcorrDIP);

	for (size_t i = 0; i < processGDB.size() - 1; i++) {

		GDB comp1 = processGDB.at(i);
		GDB comp2 = processGDB.at(i + 1);

		vector <GDB> testGDB;

		testGDB.push_back (comp1);
		testGDB.push_back (comp2);

		if (check_dataset_homogenity (testGDB)) resultGDB.push_back(comp1);
		if (i == inGDB.size() - 2) 				resultGDB.push_back(comp2);
	}
	return resultGDB;
}

bool has_inhomogeneous_enough (const vector <GDB>& inGDB) {

	const bool is_STRIAE = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);
	const bool is_SC = is_allowed_SC_datatype(inGDB.at(0).DATATYPE);

	if (is_INVERSION_NONE() && (is_STRIAE || is_SC)) return true;

	if (!(check_dataset_homogenity (inGDB))) return false;

	const vector <GDB> test = return_GDB_with_no_homogeneous_data (inGDB);

	if (test.size() >= minimum_independent_dataset ()) return true;
	return false;
}
