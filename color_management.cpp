// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "color_management.hpp"
#include "ps.h"
#include "settings.hpp"

using namespace std;

vector <vector <GDB> > associate_empty_clustercode (const vector <vector <GDB> >& inGDB_G, const size_t WHICH) {

	ASSERT_ONE_OF(WHICH, 2, 3);

	vector <vector <GDB> > outGDB_G = inGDB_G;

	for (size_t i = 0; i < outGDB_G.size(); i++) {
		for (size_t j = 0; j < outGDB_G.at(i).size(); j++) {

			const string GC = outGDB_G.at(i).at(j).GC;

			ASSERT_EXACTLY_ONE_TRUE (GC.size() == 1, GC.size() == 2);

			outGDB_G.at(i).at(j).GC = GC + 'X';
		}
	}
	return outGDB_G;
}

string pastel (const string COLOR) {

	ASSERT (is_valid_color(COLOR));

	stringstream ss (COLOR);

	double r=-1, g=-1, b=-1;

	string rest;

	ss >> r >> g >> b >> rest;

	string R = double_to_string ((r+1)/2, 3);
	string G = double_to_string ((g+1)/2, 3);
	string B = double_to_string ((b+1)/2, 3);

	const string OUT = R + " " + G + " " + B;

	ASSERT (is_valid_color (OUT));

	return OUT;
}

vector <GDB> attach_k_means_group_codes (const vector <size_t>& which_group, const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	const bool NO_CLUSTERING = which_group.size() == 0;

	const vector <string> GC_STR_VCT = allowed_basic_groupcode_str_vector();

	for (size_t i = 0; i < outGDB.size(); i++) {

		const string GC = outGDB.at(i).GC;

		if (NO_CLUSTERING) 	outGDB.at(i).GC.at(1) = 'X';
		else 				outGDB.at(i).GC.at(1) = GC_STR_VCT.at (which_group.at(i)).at(0);
	}
	return outGDB;
}

string generate_PSCOLOR_from_GC (const string GC) {

	ASSERT (!is_allowed_groupcode_empty(GC));
	ASSERT (is_allowed_basic_groupcode_str(GC));

	const vector <string> GC_STR_V = allowed_basic_groupcode_str_vector();

	vector <string> PS_CLR_V;

	ASSERT_EXACTLY_ONE_TRUE (is_GRAYSCALE_USE(), is_GRAYSCALE_NONE());

	if (is_GRAYSCALE_USE())	PS_CLR_V = allowed_pscolor_gray_vector();
	else 					PS_CLR_V = allowed_pscolor_rgb_vector();

	for (size_t i = 0; i < GC_STR_V.size(); i++) {

		if (GC_STR_V.at(i) == GC) {

			return PS_CLR_V.at(i);
		}
	}
	ASSERT_DEAD_END();


	return PS_CLR_V.at(0);
}

string generate_PSCOLOR_from_COLOR (const string CLR) {

	bool IS_ALW_CC_STR = is_allowed_colorcode_str(CLR);
	bool IS_ALW_CC_NUM = is_allowed_colorcode_num(CLR);
	bool IS_ALW_CC_EPY = is_allowed_colorcode_empty(CLR);

	ASSERT_EXACTLY_ONE_TRUE (IS_ALW_CC_STR, IS_ALW_CC_NUM, IS_ALW_CC_EPY);

	vector <string> CLR_STR_V = allowed_colorcode_str_vector();
	vector <string> CLR_NUM_V = allowed_colorcode_num_vector();

	vector <string> PS_CLR_V;

	ASSERT_EXACTLY_ONE_TRUE (is_GRAYSCALE_USE(), is_GRAYSCALE_NONE());

	if (is_GRAYSCALE_USE()) PS_CLR_V = allowed_pscolor_gray_vector();
	else 					PS_CLR_V = allowed_pscolor_rgb_vector();

	if (IS_ALW_CC_EPY) return PS_CLR_V.at(0);

	for (size_t i = 0; i < CLR_STR_V.size(); i++) {

		if (CLR_STR_V.at(i) == CLR) return PS_CLR_V.at(i);
		if (CLR_NUM_V.at(i) == CLR) return PS_CLR_V.at(i);
	}
	ASSERT_DEAD_END();
	return PS_CLR_V.at(0);
}

string generate_DASH (const string CODE) {

	const bool COLOR = is_COLOURING_COLORCODE();
	const bool GROUPS = is_COLOURING_GROUPCODE();
	const bool KMEANS = is_COLOURING_KMEANS();
	const bool RUP = is_COLOURING_RUPANG ();
	const bool IGNORE = is_COLOURING_IGNORE ();
	ASSERT_EXACTLY_ONE_TRUE (COLOR, GROUPS, KMEANS, RUP, IGNORE);

	const vector <string> GROUPCODE_STR_V = allowed_basic_groupcode_str_vector();
	const vector <string> COLORNUM_STR_V = allowed_colorcode_num_vector();
	const vector <string> COLORSTR_STR_V = allowed_colorcode_str_vector();
	const vector <string> DSH_V = allowed_dash_vector();

	if (is_GRAYSCALE_NONE () || is_PLOT_HOEPPENER () ) return DSH_V.at(0);

	if (COLOR) {

		for (size_t i = 0; i < COLORNUM_STR_V.size(); i++) {

			if (COLORNUM_STR_V.at(i) == CODE) return DSH_V.at(i+1);
			if (COLORSTR_STR_V.at(i) == CODE) return DSH_V.at(i+1);
		}
		return DSH_V.at(0);

		ASSERT_DEAD_END();
	}
	else if (GROUPS || KMEANS || RUP) {

		for (size_t i = 0; i < GROUPCODE_STR_V.size(); i++) {

			if (GROUPCODE_STR_V.at(i) == CODE) return DSH_V.at(i);
		}
		ASSERT_DEAD_END();
	}
	else {}

	return DSH_V.at(0);
}

vector <GDB> GENERATE_PS_CODE (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		string USE_THIS = "";

		bool COLOURING_IGNORE = is_COLOURING_IGNORE ();
		bool COLOURING_COLORCODE = is_COLOURING_COLORCODE ();
		bool COLOURING_GROUPCODE = is_COLOURING_GROUPCODE ();
		bool COLOURING_KMEANS = is_COLOURING_KMEANS();
		bool COLOURING_RUPANG = is_COLOURING_RUPANG();

		ASSERT_EXACTLY_ONE_TRUE (COLOURING_IGNORE, COLOURING_COLORCODE, COLOURING_GROUPCODE, COLOURING_KMEANS, COLOURING_RUPANG);

		const string DT = outGDB.at(i).DATATYPE;

		const bool PLANE = is_allowed_plane_datatype (DT);
		const bool BEDDING = is_allowed_handle_as_bedding (DT);
		const bool OT = is_allowed_bedding_overturned_sense (outGDB.at(i).OFFSET);

		const bool OVERTURNED_NOT_BEDDING = PLANE && !BEDDING && OT;

		if (COLOURING_COLORCODE) {

			USE_THIS  = outGDB.at(i).COLOR;
			outGDB.at(i).PSCOLOR = generate_PSCOLOR_from_COLOR (USE_THIS);
		}
		else if (COLOURING_GROUPCODE) {

			USE_THIS = outGDB.at(i).GC.at(0);
			outGDB.at(i).PSCOLOR = generate_PSCOLOR_from_GC (USE_THIS);
		}
		else if (COLOURING_KMEANS) {

			USE_THIS = outGDB.at(i).GC.at(1);
			outGDB.at(i).PSCOLOR = generate_PSCOLOR_from_GC (USE_THIS);
		}
		else if (COLOURING_RUPANG) {

			USE_THIS = outGDB.at(i).GC.at(2);
			outGDB.at(i).PSCOLOR = generate_PSCOLOR_from_GC (USE_THIS);//ok
		}
		else {

			vector <string> PS_COL_V = allowed_pscolor_rgb_vector();
			outGDB.at(i).PSCOLOR = PS_COL_V.at(0);
		}

		if (OVERTURNED_NOT_BEDDING) outGDB.at(i).DASHED = "6 6";
		else outGDB.at(i).DASHED = generate_DASH (USE_THIS);
	}
	return outGDB;
}
