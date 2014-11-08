// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "assertions.hpp"
#include "allowed_keys.hpp"
#include "color_management.hpp"
#include "common.h"
#include "ps.h"
#include "rup_clustering.hpp"
#include "settings.hpp"
#include "structs.h"
#include "valley_method.hpp"

using namespace std;

bool by_border (const BRDR& x, const BRDR& y) {

	return x.border < y.border;
}

vector <BRDR> sort_by_border (const vector <BRDR>& in) {

	vector <BRDR> out = in;

	sort(out.begin(), out.end(), by_border);

	return out;
}

double return_datamin (const vector <GDB>& inGDB, const string field) {

	if (field == "RUP") {

		const vector <GDB> outGDB = sort_by_RUP (inGDB);
		return outGDB.at(0).RUP;
	}
	else if (field == "ANG") {

		const vector <GDB> outGDB = sort_by_ANG (inGDB);
		return outGDB.at(0).ANG;
	}
	else ASSERT_DEAD_END();
	return NaN();
}

double return_datamax (const vector <GDB>& inGDB, const string field) {

	if (field == "RUP") {

		const vector <GDB> outGDB = sort_by_RUP (inGDB);
		return outGDB.at(inGDB.size() - 1).RUP;
	}
	else if (field == "ANG") {

		const vector <GDB> outGDB = sort_by_ANG (inGDB);
		return outGDB.at(inGDB.size() - 1).ANG;
	}
	else ASSERT_DEAD_END();
	return NaN();
}

size_t return_count_max (const vector <GDB>& inGDB, const string method, const size_t bin_number) {

	vector <HISTOGRAM> H = generate_DATA_histogram (GDB_to_table (inGDB, method), bin_number);

	H = sort_by_COUNT (H);

	return H.at(H.size() - 1).COUNT;
}

size_t return_step (const double DATA_max) {

	if (DATA_max < 5.0) return 1;
	else {

		if (DATA_max < 10) return 2;
		else {

			if (DATA_max < 25) return 5;
			else {

				if (DATA_max < 50) return 10;
				else {

					if (DATA_max < 100) return 20;
					else {

						if (DATA_max < 250) return 50;
						else {

							if (DATA_max < 500) return 100;
							else {

								if (DATA_max < 1000) return 200;
								else return DATA_max / 5.0;
							}
						}
					}
				}
			}
		}
	}
}

void ps_draw_histogram_bars (const vector <GDB>& inGDB, const vector <line_RUP_ANG>& L_R_A, ofstream& o, const CENTER& center, const PAPER& P, const double DATA_min, const double DATA_max, const size_t bin_number, const double binsize, const string method) {

	double counter = DATA_min;
	double X, Y1, Y2;

	const bool RUP_display = (method == "RUP");
	const bool ANG_display = (method == "ANG");

	if (!RUP_display && !ANG_display) ASSERT_DEAD_END();

	const bool BW1 = ((!is_RUP_CLUSTERING_RUP() && !is_RUP_CLUSTERING_ANG()) || (is_RUP_CLUSTERING_RUP() && ANG_display) || (is_RUP_CLUSTERING_ANG() && RUP_display));
	const bool BW2 = !is_COLOURING_RUPANG();

	for (size_t j = 0; j < L_R_A.size(); j++) {

		size_t data_counter = 0;

		for (size_t i = 0; i < inGDB.size(); i++) {

			if (RUP_display && is_in_range (counter, counter + binsize, inGDB.at(i).RUP)) data_counter++;
			if (ANG_display && is_in_range (counter, counter + binsize, inGDB.at(i).ANG)) data_counter++;
		}
		size_t count_max = return_count_max (inGDB, method, bin_number);

		double linewidth = L_R_A.at(j).COUNT * 30.0;
		linewidth = linewidth /count_max;

		if (RUP_display)		X = center.X + P.R + 2.0 * P.B + linewidth / 2.0;
		else if (ANG_display)	X = center.X + P.R + 4.0 * P.B + linewidth / 2.0;

		Y1 = center.Y - P.R + 2.0 * P.R * (L_R_A.at(j).L_STR / DATA_max);
		Y2 = center.Y - P.R + 2.0 * P.R * (L_R_A.at(j).L_END / DATA_max);

		linewidth_PS (o, linewidth, 1);

		if (!BW1 && !BW2) color_PS (o, L_R_A.at(j).GC);
		else color_PS (o, "0.7 0.7 0.7");

		newpath_PS(o);
		moveto_PS(o, X, Y1, 3);
		lineto_PS(o, X, Y2, 3);
		stroke_PS(o);

		counter = counter + binsize;
	}
}

void ps_RUP_ANG_header (ofstream& o, const CENTER& center, const PAPER& P){

	const bool RUP = is_RUP_CLUSTERING_RUP();

	double X, Y;

	font_PS(o, "ArialNarrow-Bold", 8);

	color_PS(o, "0.0 0.0 0.0");

	X = center.X + P.R + 4.0 * P.B + 5.0 * P.D,
	Y = center.Y + P.R + 7.0 * P.D;
	text_PS (o, X, Y, 3, "ANG");

	if (RUP && !is_INVERSION_ANGELIER() && !is_INVERSION_MOSTAFA() && !is_INVERSION_SHAN() && !is_INVERSION_FRY()) return;

	X = center.X + P.R + 2.0 * P.B + 5.0 * P.D,
	Y = center.Y + P.R + 7.0 * P.D;
	text_PS (o, X, Y, 3, "RUP");
}

void ps_percentage (ofstream& o, const CENTER& center, const PAPER& P, const string METHOD, const double DATA_max) {

	const bool RUP = METHOD == "RUP";
	const bool ANG = METHOD == "ANG";

	if (!RUP && !ANG) ASSERT_DEAD_END();

	const size_t step = return_step (DATA_max);

	font_PS (o, "ArialNarrow-Bold", 6);

	for (size_t counter = 0; counter < DATA_max; counter+=step) {

		double X1, X2;

		if (RUP)	X1 = center.X + P.R + 2.0 * P.B;
		else 		X1 = center.X + P.R + 4.0 * P.B;

		if (RUP)	X2 = center.X + P.R + 2.2 * P.B;
		else		X2 = center.X + P.R + 4.2 * P.B;

		double Y = center.Y - P.R + 2.0 * P.R * (counter / DATA_max);

		color_PS(o, "0.0 0.0 0.0");
		linewidth_PS (o, 0.5, 1);

		newpath_PS (o);
		moveto_PS (o, X1, Y, 3);
		lineto_PS (o, X2, Y, 3);
		stroke_PS (o);

		if (RUP) 	X1 = center.X + P.R + 2.4 * P.B;
		else 		X1 = center.X + P.R + 4.4 * P.B;

		Y = center.Y - P.R + 2.0 * P.R * (counter / DATA_max) - 1.0 * P.D;

		string T = int_to_string (counter);

		if (RUP)	T = T +  "%";
		else 		T = T +  "deg";

		text_PS (o, X1, Y, 3, T);
	}
}

void ps_percentage_max (ofstream& o, const CENTER& center, const PAPER& P, const string METHOD, const double DATA_max) {

	double X1, X2;

	const bool RUP = METHOD == "RUP";
	const bool ANG = METHOD == "ANG";

	if (!RUP && !ANG) ASSERT_DEAD_END();

	if (RUP) {

		X1 = center.X + P.R + 2.0 * P.B;
		X2 = center.X + P.R + 2.2 * P.B;
	}
	else {

		X1 = center.X + P.R + 4.0 * P.B;
		X2 = center.X + P.R + 4.2 * P.B;
	}

	double Y = center.Y - P.R + 2.0 * P.R;

	color_PS(o, "0.0 0.0 0.0");
	linewidth_PS (o, 0.5, 1);

	newpath_PS(o);
	moveto_PS (o, X1, Y, 3);
	lineto_PS (o, X2, Y, 3);
	stroke_PS (o);

	if (RUP)	X1 = center.X + P.R + 2.4 * P.B;
	else 		X1 = center.X + P.R + 4.4 * P.B;

	Y = center.Y - P.R + 2.0 * P.R - 1.0;

	string T = double_to_string (DATA_max, 0);

	if (RUP)	T = T +  "%";
	else 		T = T +  "deg";

	text_PS (o, X1, Y, 3, T);
}

vector <line_RUP_ANG> generate_graph_histogram (const vector <HISTOGRAM>& H, const vector <VALLEY>& V, const double DATA_MAX) {

	const double SN = 10e-8;

	vector <line_RUP_ANG> out;

	BRDR brdr_buf;
	vector <BRDR> BORDERS;

	brdr_buf.border = DATA_MAX + SN;
	brdr_buf.ID = "MAX";
	BORDERS.push_back(brdr_buf);

	for (size_t i = 0; i < V.size(); i++) {

		brdr_buf.border = V.at(i).BIN_CENTER;
		brdr_buf.ID = "VAL";
		brdr_buf.COUNT = NaN ();
		BORDERS.push_back (brdr_buf);
	}

	for (size_t i = 0; i < H.size(); i++) {

		brdr_buf.border = H.at(i).BIN_MIN;
		brdr_buf.ID = "";
		brdr_buf.COUNT = H.at(i).COUNT;
		BORDERS.push_back(brdr_buf);
	}

	BORDERS = sort_by_border (BORDERS);

	size_t valley_counter = 0;

	const vector <string> GC = allowed_basic_groupcode_str_vector ();

	for (size_t i = 0; i < BORDERS.size() - 1; i++) {

		line_RUP_ANG buffer;

		buffer.L_STR = BORDERS.at(i).border;
		buffer.L_END = BORDERS.at(i+1).border;
		buffer.COUNT = BORDERS.at(i).COUNT;

		if (valley_counter == 0) buffer.GC = "0.7 0.7 0.7";
		else buffer.GC = generate_PSCOLOR_from_GC (GC.at(valley_counter + 1));

		if (BORDERS.at(i).ID == "VAL" && i > 0) {

			valley_counter++;

			buffer.COUNT = BORDERS.at(i-1).COUNT;
		}
		else buffer.COUNT = BORDERS.at(i).COUNT;

		out.push_back(buffer);
	}
	return out;
 }
