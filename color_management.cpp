// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <cmath>
//#include <iomanip>
#include <iostream>

#include "allowed_keys.hpp"
//#include "angelier.h"
#include "assertions.hpp"
//#include "brute_force.hpp"
//#include "bingham.h"
#include "color_management.hpp"
//#include "common.h"
//#include "data_io.h"
//#include "fry.h"
//#include "inversion.h"
//#include "michael.h"
//#include "mostafa.h"
//#include "nda.h"
//#include "ps.h"
//#include "ptn.h"
//#include "rgf.h"
//#include "rup_clustering.hpp"
//#include "shan.h"
//#include "structs.h"
//#include "valley_method.hpp"

using namespace std;

bool existence_of_group_GDB (string expression, vector <GDB> inGDB) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (inGDB.at(i).GC == expression) return true;

	}
	return false;
}

bool existence_of_groupcodes (vector <GDB> inGDB) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		string GC = inGDB.at(i).GC;

		bool GC_STR = is_allowed_groupcode_str(GC);

		//if (!(inGDB.at(i).GC == "X")) return true;

		if (GC_STR) return true;
	}
	return false;
}


vector <GDB> attach_group_codes (vector <int> which_group, vector <GDB> inGDB) {

	vector <string> GC_STR_VCT = allowed_groupcode_str_vector();

	for (size_t i = 0; i < inGDB.size(); i++) {

		inGDB.at(i).GC = GC_STR_VCT.at(which_group.at(i));
	}

	return inGDB;
}


//string complete_colorcode (string in) {

	//if 		(in == "B" || in == "1") 	return "0.00 0.00 1.00";
	//else if (in == "C" || in == "2")	return "1.00 0.00 0.67";
	//else if (in == "D" || in == "3")	return "1.00 0.00 0.00";
	//else if (in == "E" || in == "4")	return "1.00 0.50 0.00";
	//else if (in == "F" || in == "5")	return "1.00 1.00 0.00";
	//else if (in == "G" || in == "6")	return "0.00 1.00 0.00";
	//else if (in == "H" || in == "7")	return "0.67 0.00 0.67";
	//else if (in == "I" || in == "8")	return "0.50 1.00 1.00";
	//else if (in == "J" || in == "9")	return "0.50 0.50 0.50";
	////else 								return "0.00 0.00 0.00";
//}

string generate_PSCOLOR_from_GC (string GC, INPSET inset) {

	bool IS_ALW_GC_STR = is_allowed_groupcode_str(GC);
	bool IS_ALW_GC_EPY = is_allowed_groupcode_empty(GC);

	if (!IS_ALW_GC_EPY && ! IS_ALW_GC_STR) ASSERT_DEAD_END();

	bool GRAYSCALE = (inset.grayscale == "Y");

	vector <string> GC_STR_V = allowed_groupcode_str_vector();

	vector <string> PS_CLR_V;

	if (GRAYSCALE) 	PS_CLR_V = allowed_pscolor_gray_vector();
	else			PS_CLR_V = allowed_pscolor_rgb_vector();

	//cout << GC << endl;

	//cout << GC_STR_V.size() << endl;
	//cout << PS_CLR_V.size() << endl;

	for (size_t i = 0; i < GC_STR_V.size(); i++) {

		if (GC_STR_V.at(i) == GC) return PS_CLR_V.at(i);
	}
	return PS_CLR_V.at(0);
}

string generate_PSCOLOR_from_COLOR (string CLR, INPSET inset) {

	bool IS_ALW_CC_STR = is_allowed_colorcode_str(CLR);
	bool IS_ALW_CC_NUM = is_allowed_colorcode_num(CLR);
	bool IS_ALW_CC_EPY = is_allowed_colorcode_empty(CLR);

	if (!IS_ALW_CC_STR && ! IS_ALW_CC_NUM && !IS_ALW_CC_EPY) ASSERT_DEAD_END();

	bool GRAYSCALE = (inset.grayscale == "Y");

	vector <string> CLR_STR_V = allowed_colorcode_str_vector();
	vector <string> CLR_NUM_V = allowed_colorcode_num_vector();

	vector <string> PS_CLR_V;

	if (GRAYSCALE) 	PS_CLR_V = allowed_pscolor_gray_vector();
	else			PS_CLR_V = allowed_pscolor_rgb_vector();

	//ASSERT(CLR_STR_V.size() == CLR_NUM_V.size());
	//ASSERT(CLR_STR_V.size() == PS_CLR_V.size());

	for (size_t i = 0; i < CLR_STR_V.size(); i++) {

		if (CLR_STR_V.at(i) == CLR) return PS_CLR_V.at(i);
		if (CLR_NUM_V.at(i) == CLR) return PS_CLR_V.at(i);

	}
	return PS_CLR_V.at(0);
}

string generate_DASHED_from_GC (string GC, INPSET inset) {

	if (inset.grayscale == "N") return "";

	bool IS_ALW_GC_STR = is_allowed_groupcode_str(GC);
	bool IS_ALW_GC_EPY = is_allowed_groupcode_empty(GC);

	if (!IS_ALW_GC_EPY && ! IS_ALW_GC_STR) ASSERT_DEAD_END();

	vector <string> GC_STR_V = allowed_groupcode_str_vector();

	vector <string> DSH_V = allowed_dash_vector();

	for (size_t i = 0; i < GC_STR_V.size(); i++) {

		if (GC_STR_V.at(i) == GC) return DSH_V.at(i);
	}
	return DSH_V.at(0);
}

bool contains_colorcode (vector <GDB> inGDB) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		string CC = inGDB.at(i).COLOR;

		bool ALW_CLR_NUM = is_allowed_colorcode_num(CC);
		bool ALW_CLR_STR = is_allowed_colorcode_str(CC);

		if (ALW_CLR_NUM || ALW_CLR_STR) return true;
	}
	return false;
}

bool contains_groupcode (vector <GDB> inGDB) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		string GC = inGDB.at(i).GC;

		bool ALW_GC_STR = is_allowed_groupcode_str(GC);

		if (ALW_GC_STR) return true;
	}
	return false;
}

vector <GDB> colorcode_grom_groupcode (vector <GDB> inGDB, INPSET inset) {

	bool CONTAINS_COLORCODE = contains_colorcode(inGDB);
	bool CONTAINS_GROUPCODE = contains_groupcode(inGDB);

	bool USE_GROUPS = (inset.group == "Y");
	bool RUP_OR_ANG = (inset.clustering_RUP_ANG != "N");

	bool by_COLOR = (CONTAINS_COLORCODE && !CONTAINS_GROUPCODE);
	bool by_GROUP = (CONTAINS_GROUPCODE && !USE_GROUPS) ||
			(CONTAINS_GROUPCODE && USE_GROUPS && RUP_OR_ANG);

	for (size_t i = 0; i < inGDB.size(); i++) {

		string GC = inGDB.at(i).GC;
		string COL = inGDB.at(i).COLOR;

		if (by_COLOR) {

			//cout << "COLOR USED" << endl;

			inGDB.at(i).PSCOLOR = generate_PSCOLOR_from_COLOR (COL, inset);
		}
		else if (by_GROUP) {

			//cout << "GROUP USED" << endl;

			inGDB.at(i).PSCOLOR = generate_PSCOLOR_from_GC (GC, inset);
		}
		else {

			vector <string> PS_COL_V = allowed_pscolor_rgb_vector();

			//cout << "BLACK USED" << endl;

			inGDB.at(i).PSCOLOR = PS_COL_V.at(0);
		}

		inGDB.at(i).DASHED = generate_DASHED_from_GC(GC, inset);
	}
	return inGDB;
}

vector <GDB> fill_with_black_colorcode (vector <GDB> inGDB) {

	vector <string> PS_COL_V = allowed_pscolor_rgb_vector();

	for (size_t i = 0; i < inGDB.size(); i++) {

		inGDB.at(i).PSCOLOR = PS_COL_V.at(0);
	}
	return inGDB;
}
