
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
//#include <iomanip>
//#include <fstream>
//#include <iostream>

#include "angelier.h"
//#include "assertions.hpp"
#include "allowed_keys.hpp"
#include "brute_force.hpp"
#include "bingham.h"
//#include "common.h"
//#include "data_io.h"
#include "fry.h"
//#include "inversion.h"
#include "michael.h"
#include "mostafa.h"
#include "nda.h"
//#include "ps.h"
#include "ptn.h"
//#include "rgf.h"
//#include "rup_clustering.hpp"
#include "settings.hpp"
#include "shan.h"
#include "structs.h"
//#include "valley_method.hpp"
//#include "yamaji.hpp"

using namespace std;

size_t minimum_independent_dataset () {

	if 		(is_INVERSION_SPRANG()) 	return 1;
	else if (is_INVERSION_TURNER()) 	return 1;
	else if (is_INVERSION_MICHAEL()) 	return 4;
	else if (is_INVERSION_SHAN()) 		return 5;
	else if (is_INVERSION_FRY()) 		return 6;
	else if (is_INVERSION_ANGELIER()) 	return 4;
	else if (is_INVERSION_MOSTAFA()) 	return 4;
	else if (is_INVERSION_BRUTEFORCE()) return 1;
	else if (is_BINGHAM_USE()) 			return 4;
	else return 1;
}

bool check_dataset_offset_homogenity (const vector <GDB>& inGDB) {

	const string ofs1 = inGDB.at(0).OFFSET;
	const string ofs2 = inGDB.at(inGDB.size() - 1).OFFSET;
	return (ofs1 != ofs2);
}

bool check_dataset_geometry_homogenity (const vector <GDB>& inGDB) {

	const bool STRIAE = 	is_allowed_striae_datatype(inGDB.at(0).DATATYPE);
	const bool SC = 		is_allowed_SC_datatype(inGDB.at(0).DATATYPE);
	//const bool BEDDING = 	((inGDB.at(0).DATATYPE) == "BEDDING");
	const bool BEDDING = 	is_allowed_handle_as_bedding (inGDB.at(0).DATATYPE);


	const double minDD = inGDB.at(0).corr.DIPDIR;
	const double maxDD = inGDB.at(inGDB.size() - 1).corr.DIPDIR;
	const double var1 = fabs(maxDD - minDD);

	const double minD = inGDB.at(0).corr.DIP;
	const double maxD = inGDB.at(inGDB.size() - 1).corr.DIP;
	const double var2 = fabs(maxD - minD);

	const double minLDD = inGDB.at(0).corrL.DIPDIR;
	const double maxLDD = inGDB.at(inGDB.size() - 1).corrL.DIPDIR;
	const double var3 = fabs(maxLDD - minLDD);

	const double minLD = inGDB.at(0).corrL.DIP;
	const double maxLD = inGDB.at(inGDB.size() - 1).corrL.DIP;
	const double var4 = fabs(maxLD - minLD);

	if (SC || STRIAE) return (var1 > 0.1 || var2 > 0.1 || var3 > 0.1 || var4 > 0.1);
	else if (BEDDING) return (var1 > 0.1 || var2 > 0.1);
	else return (var1 > 0.1 || var2 > 0.1);
}

bool check_dataset_homogenity (const vector <GDB>& inGDB) {

	const bool STRIAE = 	(is_allowed_striae_datatype(inGDB.at(0).DATATYPE));
	const bool SC = 		(is_allowed_SC_datatype(inGDB.at(0).DATATYPE));
	//const bool BEDDING = 	((inGDB.at(0).DATATYPE) == "BEDDING");
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

	if (SC || STRIAE)	sort(processGDB.begin(), processGDB.end(), bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP);
	else 				sort(processGDB.begin(), processGDB.end(), bycorrDIPDIRcorrDIP);

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

	const bool STRIAE = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);

	if (is_INVERSION_NONE() && STRIAE) return true;

	if (!(check_dataset_homogenity (inGDB))) return false;

	const vector <GDB> test = return_GDB_with_no_homogeneous_data (inGDB);

	if (test.size() >= minimum_independent_dataset ()) return true;
	return false;
}
