// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "common.h"
#include "rup_clustering.hpp"
#include "structs.h"
#include "valley_method.hpp"

using namespace std;

bool by_border (const BRDR& x, const BRDR& y) {

	return x.border < y.border;
}

vector <BRDR> sort_by_border (vector <BRDR> in) {

	sort(in.begin(), in.end(), by_border);

	return in;
}

double return_datamin (vector <GDB> inGDB, string field) {

	if (field == "RUP") {

		inGDB = sort_by_RUP(inGDB);
		return inGDB.at(0).RUP;
	}

	else {
		inGDB = sort_by_ANG(inGDB);
		return inGDB.at(0).ANG;
	}
}

double return_datamax (vector <GDB> inGDB, string field) {

	if (field == "RUP") {

		inGDB = sort_by_RUP(inGDB);
		return inGDB.at(inGDB.size() - 1).RUP;
	}

	else {
		inGDB = sort_by_ANG(inGDB);
		return inGDB.at(inGDB.size() - 1).ANG;
	}
}

int return_count_max (vector <GDB> inGDB, string method, size_t bin_number) {

	vector <HISTOGRAM> H = generate_DATA_histogram (GDB_to_table (inGDB, method), bin_number);

	H = sort_by_COUNT (H);

	return H.at(H.size() - 1).COUNT;
}

double return_step (double DATA_max) {

	if (DATA_max < 1.0) return 0.2;
	else {

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
}

void ps_draw_histogram_bars (vector <GDB> inGDB, vector <line_RUP_ANG> L_R_A, INPSET inset, ofstream& o, CENTER center, PAPER P, double DATA_min, double DATA_max, size_t bin_number, double binsize, string method) {

	double counter = DATA_min;
	double X, Y1, Y2;

	bool RUP_clustering = (inset.clustering_RUP_ANG == "R");
	bool ANG_clustering = (inset.clustering_RUP_ANG == "A");

	bool RUP_display = (method == "RUP");
	bool ANG_display = (method == "ANG");

	bool BW = ((!RUP_clustering && !ANG_clustering) || (RUP_clustering && ANG_display) || (ANG_clustering && RUP_display));

	for (size_t j = 0; j < L_R_A.size(); j++) {

		size_t data_counter = 0;

		for (size_t i = 0; i < inGDB.size(); i++) {

			if (method == "RUP" && is_in_range (counter, counter + binsize, inGDB.at(i).RUP)) data_counter++;
			if (method == "ANG" && is_in_range (counter, counter + binsize, inGDB.at(i).ANG)) data_counter++;
		}

		size_t count_max = return_count_max (inGDB, method, bin_number);

		double linewidth = L_R_A.at(j).COUNT * 30.0;
		linewidth = linewidth /count_max;

		if (method == "RUP")	X = center.X + P.R + 2.0 * P.B + linewidth / 2.0;
		else 					X = center.X + P.R + 4.0 * P.B + linewidth / 2.0;

		Y1 = center.Y - P.R + 2.0 * P.R * (L_R_A.at(j).L_STR / DATA_max);
		Y2 = center.Y - P.R + 2.0 * P.R * (L_R_A.at(j).L_END / DATA_max);

		if (inset.grayscale == "Y") o <<  "  0.8 0.8 0.8 setrgbcolor " << linewidth << " setlinewidth" << endl;
		else {
			if (BW)	o <<  "  0.8 0.8 0.8 setrgbcolor " << linewidth << " setlinewidth" << endl;
			else 	o <<  L_R_A.at(j).GC << " setrgbcolor " << linewidth << " setlinewidth" << endl;
		}
		o
		<< "  newpath "
		<< fixed << setprecision (3) << X << " " << Y1 << " moveto "
		<< fixed << setprecision (3) << X << " " << Y2 << " lineto stroke" << endl;

		counter = counter + binsize;
	}
}

void ps_RUP_ANG_header (ofstream& o, CENTER center, PAPER P, string method){

	double X = 0;
	double Y = 0;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;


	if (method == "RUP") 	X = center.X + P.R + 2.0 * P.B + 5.0;
	else 					X = center.X + P.R + 4.0 * P.B + 5.0;

	Y = center.Y + P.R + 7.0;

	o
	<< "  " << fixed << setprecision (3) << X
	<< " "  << fixed << setprecision (3) << Y << flush;

	if (method == "RUP")	o << "  moveto (RUP) 0 0 0 setrgbcolor show " << endl;
	else 					o << "  moveto (ANG) 0 0 0 setrgbcolor show " << endl;
}

void ps_percentage (ofstream& o, CENTER center, PAPER P, string method, double DATA_max) {

	double X1, X2, Y;

	double step =  return_step (DATA_max);

	double counter = 0;

	o << fixed << setprecision (0) << endl;

	o << "/ArialNarrow-Bold findfont 6 scalefont setfont" << endl;
	o << "  0.0 0.0 0.0 setrgbcolor 0.5 setlinewidth" << endl;

	do {

		if (method == "RUP") {

			X1 = center.X + P.R + 2.0 * P.B;
			X2 = center.X + P.R + 2.2 * P.B;
		}
		else {

			X1 = center.X + P.R + 4.0 * P.B;
			X2 = center.X + P.R + 4.2 * P.B;
		}

		Y = center.Y - P.R + 2.0 * P.R * (counter / DATA_max);

		o << "  0.0 0.0 0.0 setrgbcolor 0.5 setlinewidth" << endl;
		o
		<< "  newpath "
		<< fixed << setprecision (3) << X1 << " " << Y << " moveto "
		<< fixed << setprecision (3) << X2 << " " << Y << " lineto stroke" << endl;


		if (method == "RUP") 	X1 = center.X + P.R + 2.4 * P.B;
		else 					X1 = center.X + P.R + 4.4 * P.B;

		Y = center.Y - P.R + 2.0 * P.R * (counter / DATA_max) - 1.0;

		o
		<< "  " << fixed << setprecision (3) << X1
		<< " "  << fixed << setprecision (3) << Y
		<< fixed << setprecision (0) << "  moveto (" << counter << flush;

		if (method == "RUP") o << "%) 0 0 0 setrgbcolor show " << endl;
		else 				 o << " deg) 0 0 0 setrgbcolor show " << endl;

		counter = counter + step;

	} while (counter < DATA_max);
}

void ps_percentage_max (ofstream& o, CENTER center, PAPER P, string method, double DATA_max) {

	double X1, X2, Y;

	if (method == "RUP") {

		X1 = center.X + P.R + 2.0 * P.B;
		X2 = center.X + P.R + 2.2 * P.B;
	}
	else {

		X1 = center.X + P.R + 4.0 * P.B;
		X2 = center.X + P.R + 4.2 * P.B;
	}

	Y = center.Y - P.R + 2.0 * P.R;

	o
	<< "  newpath "
	<< fixed << setprecision (3) << X1 << " " << Y << " moveto "
	<< fixed << setprecision (3) << X2 << " " << Y << " lineto stroke" << endl;




	if (method == "RUP")	X1 = center.X + P.R + 2.4 * P.B;
	else 					X1 = center.X + P.R + 4.4 * P.B;

	Y = center.Y - P.R + 2.0 * P.R - 1.0;

	o
	<< "  " << fixed << setprecision (3) << X1
	<< " "  << fixed << setprecision (3) << Y
	<< fixed << setprecision (0) << flush;

	if (method == "RUP")	o << "  moveto (" << DATA_max << "%) 0 0 0 setrgbcolor show " << endl;
	else 					o << "  moveto (" << DATA_max << " deg) 0 0 0 setrgbcolor show " << endl;
}

vector <line_RUP_ANG> generate_graph_histogram (vector <HISTOGRAM> H, vector <VALLEY> V, double DATA_MAX) {

	double SN = 10e-8;

	line_RUP_ANG buffer;
	vector <line_RUP_ANG> out;

	BRDR brdr_buf;
	vector <BRDR> BORDERS;

	vector <string> H_CLR;

	H_CLR.push_back("0.50 0.50 0.50");
	H_CLR.push_back("0.50 0.50 1.00");
	H_CLR.push_back("1.00 0.50 0.83");
	H_CLR.push_back("1.00 0.50 0.50");
	H_CLR.push_back("1.00 0.75 0.50");
	H_CLR.push_back("1.00 1.00 0.50");
	H_CLR.push_back("0.50 1.00 0.50");
	H_CLR.push_back("0.83 0.50 0.83");
	H_CLR.push_back("0.75 1.00 1.00");
	H_CLR.push_back("0.75 0.75 0.75");

	string act_clr = "0.80 0.80 0.80";

	brdr_buf.border = DATA_MAX + SN;
	brdr_buf.ID = "MAX";
	BORDERS.push_back(brdr_buf);

	for (size_t i = 0; i < V.size(); i++) {

		brdr_buf.border = V.at(i).BIN_CENTER;
		brdr_buf.ID = "VAL";
		BORDERS.push_back(brdr_buf);
	}

	for (size_t i = 0; i < H.size(); i++) {

		brdr_buf.border = H.at(i).BIN_MIN;
		brdr_buf.ID = "";
		brdr_buf.COUNT = H.at(i).COUNT;
		BORDERS.push_back(brdr_buf);
	}

	BORDERS = sort_by_border(BORDERS);

	size_t col_cntr = 0;

	for (size_t i = 0; i < BORDERS.size() - 1; i++) {

		buffer.L_STR = BORDERS.at(i).border;
		buffer.L_END = BORDERS.at(i+1).border;

		buffer.COUNT = BORDERS.at(i).COUNT;

		if (BORDERS.at(i).ID == "VAL" && i > 0) {

			col_cntr++;
			buffer.COUNT = BORDERS.at(i-1).COUNT;
		}
		else buffer.COUNT = BORDERS.at(i).COUNT;

		buffer.GC = H_CLR.at(col_cntr);

		out.push_back(buffer);
	}

	return out;
 }
