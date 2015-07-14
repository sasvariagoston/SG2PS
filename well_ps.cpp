/*
 * well_ps.cpp
 *
 *  Created on: 25 Nov 2015
 *      Author: sasvariagoston_admin
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
//#include <sstream>
#include <stdlib.h>
#include <vector>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "common.h"
#include "data_io.h"
#include "data_sort.hpp"
#include "filename.hpp"
#include "run_mode.h"
#include "paper.hpp"
#include "platform_dep.hpp"
#include "ps.h"
#include "rgf.h"
#include "settings.hpp"
#include "standard_output.hpp"
#include "structs.h"
#include "well.hpp"
#include "well_ps.hpp"

struct WELL_TOPS {

	double DEPTH;
	string FORMATION;
};

namespace {

vector <WELL_TOPS> WTP;

vector <PEAK_TO_PLOT> PEAK;
vector <PEAK_TO_PLOT> FAULTS;

const double PL_WDT = 3.7;//was 4.0

const double PL_GP = 0.35;//was 0.9
const double PL_AX_GP = 0.1;//was 0.2

const double PL_LF = -2.7;//was -2.5

const string STD_CLR = "0.80 0.80 0.80";
const string AVR_CLR = "0.00 0.00 0.00";
const string DRV_CLR = "1.00 0.00 0.00";
const string FRQ_CLR = "0.00 0.00 1.00";

const VCTR WHT = declare_vector (1.00, 1.00, 1.00);
const VCTR GRY = declare_vector (0.83, 0.83, 0.83);
const VCTR BLK = declare_vector (0.00, 0.00, 0.00);

const VCTR PRP = declare_vector (1.00, 0.00, 1.00);

const VCTR RED = declare_vector (1.00, 0.00, 0.00);
const VCTR YLW = declare_vector (1.00, 1.00, 0.00);
const VCTR GRN = declare_vector (0.25, 0.75, 0.25);

const VCTR LRD = declare_vector (1.00, 0.50, 0.50);
const VCTR LYL = declare_vector (1.00, 1.00, 0.50);
const VCTR LGN = declare_vector (0.50, 1.00, 0.50);
}

using namespace std;

vector <PEAK_TO_PLOT> return_PEAK () {

	return PEAK;
}

vector <PEAK_TO_PLOT> return_FAULTS () {

	return FAULTS;
}
void PS_well_header (ofstream& o) {

	const string filename = return_ACTUAL_LOCATION() + "_" + return_ACTUAL_DATATYPE() + "_" + return_ACTUAL_GROUPCODE() + ".EPS";

	o << "%!PS-Adobe-3.0 EPSF-3.0" << '\n';
	o << "%%BoundingBox:  0 0 842 1191" << '\n';
	o << "%%Title: " << filename << '\n';
	o << "%%Creator: SG2PS" ;

	if (!(is_mode_DEBUG())) o << " (" << version_id() << ")" ;

	o << '\n';
	o << "%%EndComments" << '\n' << '\n';
	o << "<</PageSize [ 842 1191 ]>> setpagedevice " << '\n';

	text_PS (o, "/measurement {");
	newpath_PS (o);
	moveto_PS (o,  -1.0,  0.0, 3);
	rlineto_PS (o,  2.0,  0.0, 3);
	rlineto_PS (o,  0.0,  2.0, 3);
	rlineto_PS (o, -2.0,  0.0, 3);
	closepath_PS (o);
	color_PS (o, "0.00 0.00 0.00");
	fill_PS (o);
	stroke_PS(o);

	linewidth_PS(o, 0.5, 1);

	color_PS (o, "1.00 1.00 1.00");

	newpath_PS (o);
	moveto_PS (o,  -1.0,  0.0, 3);
	rlineto_PS (o,  2.0,  0.0, 3);
	rlineto_PS (o,  0.0,  2.0, 3);
	rlineto_PS (o, -2.0,  0.0, 3);
	closepath_PS (o);
	stroke_PS(o);

	text_PS (o, "} def");


	text_PS (o, "/overturned_measurement {");
		newpath_PS (o);
		moveto_PS (o,  -1.0,  0.0, 3);
		rlineto_PS (o,  2.0,  0.0, 3);
		rlineto_PS (o,  0.0,  2.0, 3);
		rlineto_PS (o, -2.0,  0.0, 3);
		closepath_PS (o);
		color_PS (o, "1.00 0.00 0.00");
		fill_PS (o);
		stroke_PS(o);

		linewidth_PS(o, 0.5, 1);

		color_PS (o, "1.00 1.00 1.00");

		newpath_PS (o);
		moveto_PS (o,  -1.0,  0.0, 3);
		rlineto_PS (o,  2.0,  0.0, 3);
		rlineto_PS (o,  0.0,  2.0, 3);
		rlineto_PS (o, -2.0,  0.0, 3);
		closepath_PS (o);
		stroke_PS(o);

		text_PS (o, "} def");

	return;
}

void PS_well_border (ofstream& o) {

	const PAPER P = RETURN_PAPER();

	const string DT = return_ACTUAL_DATATYPE();
	const string LOC = return_ACTUAL_LOCATION();
	const string GC = return_ACTUAL_GROUPCODE();
	const string FM = return_ACTUAL_FORMATION();

	const bool asked_KMEANS = !is_CLUSTERING_NONE();
	//const bool asked_RUPANG = !is_RUP_CLUSTERING_NONE();

	const bool by_GROUPCODE = is_GROUPSEPARATION_GROUPCODE ();
	//const bool by_KMEANS = is_GROUPSEPARATION_KMEANS ();
	const bool by_RUPANG = is_GROUPSEPARATION_RUPANG ();
	//const bool IGNORE = is_GROUPSEPARATION_IGNORE ();

	const bool color_by_COLORCODE = is_COLOURING_COLORCODE();
	const bool color_by_GROUPCODE = is_COLOURING_GROUPCODE();
	//const bool color_by_KMEANS = is_COLOURING_KMEANS ();
	const bool color_by_RUPANG = is_COLOURING_RUPANG ();
	const bool color_IGNORE = is_COLOURING_IGNORE ();

	const bool exists_GROUPCODE = 	GC.at(0) != 'X';
	//const bool exists_KMEANS = 	GC.at(1) != 'X';
	const bool exists_RUPANG = 		GC.at(2) != 'X';

	//if (!by_GROUPCODE && !by_KMEANS && !by_RUPANG && !IGNORE) ASSERT_DEAD_END();
	//if (!color_by_COLORCODE && !color_by_GROUPCODE && !color_by_KMEANS && !color_by_RUPANG && !color_IGNORE) ASSERT_DEAD_END();

	if (by_GROUPCODE && GC.size() < 1) ASSERT_DEAD_END();
	//if (by_KMEANS && GC.size() < 2) ASSERT_DEAD_END();
	if (by_RUPANG && GC.size() < 3) ASSERT_DEAD_END();

	newpath_PS(o);

	linewidth_PS ( o, 1.5, 1);

	moveto_PS (o, P.A, P.Y - P.A - 10.0, 3);
	lineto_PS (o, P.A, P.A, 3);
	lineto_PS (o, P.X - P.A, P.A, 3);
	lineto_PS (o, P.X - P.A, P.Y - P.A, 3);

	color_PS (o, "0.0 0.0 0.0");
	stroke_PS (o);

	font_PS(o, "ArialNarrow-Bold", 12);
	color_PS (o, "0.0 0.0 0.0");

	string T = DT + " FROM LOCATION " + LOC;

	if (asked_KMEANS) T = T + " - K-MEANS CLUSTERING USED";


	if (by_GROUPCODE) 	T =  T + ", GROUP '" + GC.at(0) + "' USING ORIGINAL GROUPCODE" ;
	else if (is_GROUPSEPARATION_IGNORE()) {}
	else {}

	if (color_by_COLORCODE) {

		T = T + ", COLOURED USING COLOUR CODE";
	}
	else if (color_by_GROUPCODE) {

		if (exists_GROUPCODE) T = T + ", COLOURED USING GROUP CODE";
	}
	else if (color_by_RUPANG) {

		if (is_RUP_CLUSTERING_ANG() && exists_RUPANG) {

			T = T + ", COLOURED USING 'ANG' CLUSTER CODE";
		}
		else if (is_RUP_CLUSTERING_RUP() && exists_RUPANG) {

			T = T + ", COLOURED USING 'RUP' CLUSTER CODE";
		}
	}
	else if (color_IGNORE) {}
	else {}

	const bool TLT = is_PROCESS_AS_TILTED ();
	const bool TRJ = is_PROCESS_AS_TRAJECTORY ();

	if 		(!TLT && !TRJ)	T = T + " - ORIGINAL DATA SET";
	else if ( TLT && !TRJ) 	T = T + " - BEDDING/PALEO NORTH CORRECTED DATA SET";
	else if ( TLT &&  TRJ) 	T = T + " - BEDDING/PALEO NORTH AND TRAJECTORY CORRECTED DATA SET";
	else if (!TLT &&  TRJ)	T = T + " - TRAJECTORY CORRECTED DATA SET";
	else ASSERT_DEAD_END();

	text_PS (o, P.A - P.B + P.D * 20.0, P.Y - P.A - P.D * 5.5, 3, T);

	text_PS(o, "%%-----end PSborder");
}

string generate_colorstep (const VCTR clr1, const VCTR clr2, const VCTR clr3, const size_t percent) {

	double P = percent / 100.0;

	if (P < 0.5) {

		return  double_to_string (clr2.X * 2 * P + clr1.X * (1 - 2 * P), 3) + " " +
				double_to_string (clr2.Y * 2 * P + clr1.Y * (1 - 2 * P) ,3) + " " +
				double_to_string (clr2.Z * 2 * P + clr1.Z * (1 - 2 * P) ,3);
	}

	else {

		P = P - 0.5;

		return  double_to_string (clr3.X * 2 * P + clr2.X * (1 - 2 * P), 3) + " " +
				double_to_string (clr3.Y * 2 * P + clr2.Y * (1 - 2 * P) ,3) + " " +
				double_to_string (clr3.Z * 2 * P + clr2.Z * (1 - 2 * P) ,3);
	}
}

string generate_peak_colors (const size_t percent) {

	return (generate_colorstep(WHT, GRY, PRP, percent));
}

string generate_error_colors (const size_t percent) {

	return (generate_colorstep(GRN, YLW, RED, percent));
}

string generate_binsize_colors (const size_t percent) {

	if (is_WELL_INTERVAL_DATANUMBER()) return STD_CLR;

	return (generate_colorstep (LRD, LYL, LGN, percent));
}

void PS_rectangle (ofstream& o, const PAPER& P, const double X1, const double Y1, const double X2, const double Y2) {

	newpath_PS(o);

	moveto_PS (o, X1 * P.D, Y1 * P.D, 3);
	lineto_PS (o, X2 * P.D, Y1 * P.D, 3);
	lineto_PS (o, X2 * P.D, Y2 * P.D, 3);
	lineto_PS (o, X1 * P.D, Y2 * P.D, 3);

	closepath_PS(o);
	stroke_PS(o);

	return;
}

void PS_rainbow (ofstream& o, const PAPER& P, const double X1, const double Y1, const double X2, const double Y2, const string MODE) {

	const bool BSZ = MODE == "BINSIZE";
	const bool ERR = MODE == "ERROR";

	if (!BSZ && !ERR) ASSERT_DEAD_END();

	linewidth_PS(o, 1, 1);

	newpath_PS(o);

	for (size_t i = 0; i < 100; i++) {

		string CLR;

		if (BSZ) CLR = generate_binsize_colors (i);
		else CLR = generate_error_colors(i);

		const double actX = X1 + i*((X2 - X1 - 0.5) / 100);

		color_PS(o, CLR);

		moveto_PS(o, actX * P.D, Y1 * P.D, 3);
		lineto_PS(o, actX * P.D, Y2 * P.D, 3);

		stroke_PS(o);
	}
	stroke_PS(o);
	color_PS(o, "0.0 0.0 0.0");

	PS_rectangle (o, P, X1, Y1, X2, Y2);

	return;
}

void PS_well_symbols (ofstream& o) {

	const PAPER P = RETURN_PAPER();

	linewidth_PS (o, 1, 1);

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 175*P.D, 1565*P.D, 3, "DATA NUMBER IN BINS");
	font_PS(o, "ArialNarrow", 7);
	text_PS(o, 110*P.D, 1583*P.D, 3, "MIN");
	text_PS(o, 145*P.D, 1583*P.D, 3, "MAX");
	PS_rainbow (o, P, 110, 1580, 160, 1560, "BINSIZE");

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 175*P.D, 1530*P.D, 3, "RELATIVE ERROR IN BINS");
	PS_rainbow (o, P, 110, 1545, 160, 1525, "ERROR");

	font_PS(o, "ArialNarrow", 7);
	text_PS(o, 110*P.D, 1548*P.D, 3, "MIN");
	text_PS(o, 145*P.D, 1548*P.D, 3, "MAX");

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 375*P.D, 1565*P.D, 3, "MEASURED DATA");
	translate_PS (o, 355*P.D, 1570*P.D, 3);
	text_PS (o, " newpath measurement");
	translate_PS (o, -355*P.D, - 1570*P.D, 3);

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 375*P.D, 1530*P.D, 3, "MEASURED DATA, OVERTURNED");
	translate_PS (o, 355*P.D, 1535*P.D, 3);
	text_PS (o, " newpath overturned_measurement");
	translate_PS (o, -355*P.D, - 1535*P.D, 3);

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 615*P.D, 1565*P.D, 3, "DATA AVERAGE");
	color_PS(o, AVR_CLR);
	linewidth_PS(o, 1.5, 1);
	moveto_PS(o, 575 * P.D, 1570 * P.D, 3);
	lineto_PS(o, 605 * P.D, 1570 * P.D, 3);
	stroke_PS(o);

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 615*P.D, 1530*P.D, 3, "DATA FREQUENCY");
	color_PS(o, FRQ_CLR);
	linewidth_PS(o, 1.5, 1);
	moveto_PS(o, 575 * P.D, 1535 * P.D, 3);
	lineto_PS(o, 605 * P.D, 1535 * P.D, 3);
	stroke_PS(o);

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 815*P.D, 1565*P.D, 3, "BIN HALF STANDARD DEVIATION ");
	color_PS(o, STD_CLR);
	linewidth_PS(o, 1.5, 1);
	moveto_PS(o, 775 * P.D, 1570 * P.D, 3);
	lineto_PS(o, 805 * P.D, 1570 * P.D, 3);
	stroke_PS(o);

	font_PS(o, "ArialNarrow", 10);
	color_PS(o, "0.0 0.0 0.0");
	text_PS(o, 815*P.D, 1530*P.D, 3, "DATA AVERAGE FIRST DERIVATE");
	color_PS(o, DRV_CLR);
	linewidth_PS(o, 1.5, 1);
	moveto_PS(o, 775 * P.D, 1535 * P.D, 3);
	lineto_PS(o, 805 * P.D, 1535 * P.D, 3);
	stroke_PS(o);
}

double well_axes_step (const double MIN, const double MAX) {

	const double DIF = MAX - MIN;

	if (DIF < 10.0) return 1.0;
	else {
		if (DIF < 20.0) return 2.0;
		else {
			if (DIF < 50.0) return 5.0;
			else {
				if (DIF < 100.0) return 10.0;
				else {
					if (DIF < 200.0) return 20.0;
					else {
						if (DIF < 500.0) return 50.0;
						else {
							if (DIF < 1000.0) return 100.0;
							else {
								if (DIF < 2000.0) return 200.0;
								else {
									if (DIF < 5000.0) return 500.0;
									else {
										return 1000.0;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

bool has_GDB_DEPTH_value_in_range (const vector <GDB>& inGDB, const double MIN, const double MAX) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		const double D = inGDB.at(i).DEPTH;

		if (is_in_range(MIN, MAX, D)) return true;
	}
	return false;
}

double return_MIN_value (const vector <GDB>& inGDB, const double STEP) {

	const vector <GDB> temp = SORT_GDB (inGDB, "DEPTH");

	const double LIMIT = inGDB.at(inGDB.size() - 1).DEPTH;

	for (double i = 0.0; i < LIMIT; i+=STEP) {

		if (has_GDB_DEPTH_value_in_range (temp, i, i + STEP)) return i;
	}
	return NaN();
}

double return_MAX_value (const vector <GDB>& inGDB, const double STEP) {

	const vector <GDB> temp = SORT_GDB (inGDB, "rDEPTH");

	for (double i = STEP * 2000.0; i > 0.0; i-=STEP) {

		if (has_GDB_DEPTH_value_in_range (temp, i, i + STEP)) return i + STEP;
	}
	return NaN();
}

void PS_well_coordinate_axes_INTERVAL (ofstream& o, const double X, const double LENGTH) {

	newpath_PS(o);
	color_PS (o, "0.0 0.0 0.0");
	linewidth_PS (o, 1, 1);

	const PAPER P = RETURN_PAPER();

	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X, P.O1Y - LENGTH, 3);
	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X + PL_WDT * P.A, P.O1Y, 3);
	stroke_PS (o);

	return;
}

void ps_well_formation_tops (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP) {

	font_PS(o, "ArialNarrow", 10);
	color_PS (o, "0.5 0.5 1.0");
	linewidth_PS (o, 2, 0);

	const PAPER P = RETURN_PAPER();

	for (size_t i = 0; i < WTP.size(); i++) {

		const double Y = LENGTH * ((WTP.at(i).DEPTH - MIN_VAL) / (MAX_VAL - MIN_VAL));

		moveto_PS (o, X + 900.0 * P.D, P.O1Y - Y, 3);
		lineto_PS (o, X + 150.0 * P.D, P.O1Y - Y, 3);
		stroke_PS(o);

		const string T = "TOP " + WTP.at(i).FORMATION + " (" + double_to_string (WTP.at(i).DEPTH, 0) + ")";

		text_PS (o, X + 5.0 * P.D, P.O1Y -Y - 5 * P.D, 3, T);
	}
	return;
}

void PS_well_coordinate_grid_INTERVAL (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP) {

	const PAPER P = RETURN_PAPER();

	font_PS(o, "ArialNarrow-Bold", 24);
	color_PS (o, "0.9 0.9 0.9");

	text_PS (o, X + (PL_WDT - 3.2) * P.A, P.O1Y + 45.0 * P.D, 3, "BIN SIZE");

	font_PS(o, "ArialNarrow", 12);
	color_PS (o, "0.0 0.0 0.0");

	for (double i = MIN_VAL; i < MAX_VAL + 0.5*STEP; i+=STEP) {

		const double INT = (i - MIN_VAL) / (MAX_VAL - MIN_VAL);

		if (i > MIN_VAL + 0.5*STEP) {

			color_PS (o, "0.9 0.9 0.9");

			moveto_PS (o, X + PL_WDT * P.A, P.O1Y - (INT * LENGTH), 3);
			lineto_PS (o, X + 5.0 * P.D, P.O1Y - (INT * LENGTH), 3);
			stroke_PS(o);

			color_PS (o, "0.0 0.0 0.0");
		}
		moveto_PS (o, X, P.O1Y - (INT * LENGTH), 3);
		lineto_PS (o, X - 5.0 * P.D, P.O1Y - (INT * LENGTH), 3);
		stroke_PS(o);

		text_PS (o, X - 40.0 * P.D, P.O1Y - (INT * LENGTH) - 6.0 * P.D, 3, double_to_string (i, 0));
	}
	return;
}

void PS_well_coordinate_axes_DIPDIR (ofstream& o, const double X, const double LENGTH) {

	const PAPER P = RETURN_PAPER();

	newpath_PS(o);
	color_PS (o, "0.0 0.0 0.0");
	linewidth_PS (o, 1, 1);

	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X, P.O1Y - LENGTH, 3);
	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X + PL_WDT * P.A, P.O1Y, 3);
	stroke_PS (o);

	font_PS(o, "ArialNarrow-Bold", 24);
	color_PS (o, "0.9 0.9 0.9");

	if (is_DATARULE_GERMAN()) 	text_PS (o, X + (PL_WDT - 3.5) * P.A, P.O1Y + 45.0 * P.D, 3, "DIP DIRECTION");
	else text_PS (o, X + (PL_WDT - 1.2) * P.A, P.O1Y + 45.0 * P.D, 3, "STRIKE DIRECTION");

	font_PS(o, "ArialNarrow", 12);

	color_PS (o, "0.0 0.0 0.0");
	text_PS(o, X + 1.5 * P.A, P.O1Y + 25.0 * P.D, 3, "VALUE");

	color_PS (o, "1.0 0.0 0.0");
	text_PS(o, X + 4.3 * P.A, P.O1Y + 25.0 * P.D, 3, "DERIVATE");
}

void PS_well_coordinate_grid_DIPDIR (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP) {

	const PAPER P = RETURN_PAPER();

	linewidth_PS (o, 1, 1);

	font_PS(o, "ArialNarrow", 12);
	color_PS (o, "0.0 0.0 0.0");

	for (size_t i = 0; i < 13; i++) {

		const double stp = (i * PL_WDT * P.A) / 12.0;

		moveto_PS (o, X + stp, P.O1Y, 3);
		lineto_PS (o, X + stp, P.O1Y + 5.0 * P.D, 3);
		stroke_PS(o);

		if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12) {

			if (i > 0 ) {

				color_PS (o, "0.9 0.9 0.9");

				moveto_PS (o, X + stp, P.O1Y - LENGTH, 3);
				lineto_PS (o, X + stp, P.O1Y - 5.0 * P.D, 3);
				stroke_PS(o);

				color_PS (o, "0.0 0.0 0.0");
			}
			if (i == 0) text_PS (o, X + stp - 3.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (0.0, 0));
			if (i == 3) text_PS (o, X + stp - 8.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (90.0, 0));
			if (i == 6) text_PS (o, X + stp - 12.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (180.0, 0));
			if (i == 9)	text_PS (o, X + stp - 12.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (270.0, 0));
			if (i == 12) text_PS (o, X + stp - 12.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (360.0, 0));
		}
	}

	for (double i = MIN_VAL; i < MAX_VAL + 0.5*STEP; i+=STEP) {

		const double INT = (i - MIN_VAL) / (MAX_VAL - MIN_VAL);

		if (i > MIN_VAL + 0.5*STEP) {

			color_PS (o, "0.9 0.9 0.9");

			moveto_PS (o, X + PL_WDT * P.A, P.O1Y - (INT * LENGTH), 3);
			lineto_PS (o, X + 5.0 * P.D, P.O1Y - (INT * LENGTH), 3);
			stroke_PS(o);

			color_PS (o, "0.0 0.0 0.0");
		}
		moveto_PS (o, X, P.O1Y - (INT * LENGTH), 3);
		lineto_PS (o, X - 5.0 * P.D, P.O1Y - (INT * LENGTH), 3);
		stroke_PS(o);
	}
	return;
}

void PS_well_coordinate_axes_DIP (ofstream& o, const double X, const double LENGTH) {

	const PAPER P = RETURN_PAPER();

	newpath_PS(o);
	color_PS (o, "0.0 0.0 0.0");
	linewidth_PS (o, 1, 1);

	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X, P.O1Y - LENGTH, 3);
	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X + PL_WDT * P.A, P.O1Y, 3);
	stroke_PS (o);

	font_PS(o, "ArialNarrow-Bold", 24);
	color_PS (o, "0.9 0.9 0.9");

	text_PS(o, X + (PL_WDT - 2.5) * P.A, P.O1Y + 45.0 * P.D, 3, "DIP ANGLE");

	font_PS(o, "ArialNarrow", 12);

	color_PS (o, "0.0 0.0 0.0");
	text_PS(o, X + 1.5 * P.A, P.O1Y + 25.0 * P.D, 3, "VALUE");

	color_PS (o, "1.0 0.0 0.0");
	text_PS(o, X + 4.3 * P.A, P.O1Y + 25.0 * P.D, 3, "DERIVATE");
}

void PS_well_coordinate_grid_DIP (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP) {

	const PAPER P = RETURN_PAPER();

	linewidth_PS (o, 1, 1);

	font_PS(o, "ArialNarrow", 12);
	color_PS (o, "0.0 0.0 0.0");

	for (size_t i = 0; i < 7; i++) { //7 was 4

		const double stp = (i * PL_WDT * P.A) / 6.0; //6.0 was 3.0

		moveto_PS (o, X + stp, P.O1Y, 3);
		lineto_PS (o, X + stp, P.O1Y + 5.0 * P.D, 3);
		stroke_PS(o);

		if (i > 0 ) {

			color_PS (o, "0.9 0.9 0.9");

			moveto_PS (o, X + stp, P.O1Y - LENGTH, 3);
			lineto_PS (o, X + stp, P.O1Y - 5.0 * P.D, 3);
			stroke_PS(o);

			color_PS (o, "0.0 0.0 0.0");
		}
		if (i == 0) text_PS (o, X + stp - 11.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (-90.0, 0));
		if (i == 3) text_PS (o, X + stp - 4.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (0.0, 0));
		if (i == 6) text_PS (o, X + stp - 7.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (90.0, 0));
	}

	for (double i = MIN_VAL; i < MAX_VAL + 0.5*STEP; i+=STEP) {

		const double INT = (i - MIN_VAL) / (MAX_VAL - MIN_VAL);

		if (i > MIN_VAL + 0.5*STEP) {

			color_PS (o, "0.9 0.9 0.9");

			moveto_PS (o, X + PL_WDT * P.A, 	P.O1Y - (INT * LENGTH), 3);
			lineto_PS (o, X + 5.0 * P.D, 	P.O1Y - (INT * LENGTH), 3);
			stroke_PS(o);

			color_PS (o, "0.0 0.0 0.0");
		}
		moveto_PS (o, X, 				P.O1Y - (INT * LENGTH), 3);
		lineto_PS (o, X - 5.0 * P.D, 	P.O1Y - (INT * LENGTH), 3);
		stroke_PS(o);
	}
	return;
}

void PS_well_coordinate_axes_FAULTS (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP) {

	const PAPER P = RETURN_PAPER();

	newpath_PS(o);
	color_PS (o, "0.9 1.0 1.0");
	linewidth_PS (o, 0, 1);
	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X, P.O1Y - LENGTH, 3);
	lineto_PS (o, X + PL_WDT * P.A, P.O1Y - LENGTH, 3);
	lineto_PS (o, X + PL_WDT * P.A, P.O1Y, 3);
	fill_PS(o);
	stroke_PS(o);

	newpath_PS(o);
	color_PS (o, "0.0 0.0 0.0");
	linewidth_PS (o, 1, 1);
	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X, P.O1Y - LENGTH, 3);
	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X + PL_WDT * P.A, P.O1Y, 3);
	stroke_PS(o);


	font_PS(o, "ArialNarrow-Bold", 24);
	color_PS (o, "0.9 0.9 0.9");

	text_PS(o, X + 0.5 * P.A, P.O1Y + 45.0 * P.D, 3, "FAULTS");

	font_PS(o, "ArialNarrow", 12);

	color_PS (o, "0.0 0.0 0.0");
	text_PS(o, X + 0.8 * P.A, P.O1Y + 25.0 * P.D, 3, "PROBABILITY");

	for (size_t i = 0; i < 4; i++) {

		const double stp = (i * PL_WDT * P.A) / 3.0;

		if (i == 3) {

			moveto_PS (o, X + stp, P.O1Y, 3);
			lineto_PS (o, X + stp, P.O1Y + 5.0 * P.D, 3);
			stroke_PS(o);

			color_PS (o, "0.9 0.9 0.9");

			moveto_PS (o, X + stp, P.O1Y - LENGTH, 3);
			lineto_PS (o, X + stp, P.O1Y - 5.0 * P.D, 3);
			stroke_PS(o);

			color_PS (o, "0.0 0.0 0.0");
		}
		if (i == 0) text_PS (o, X + stp - 3.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (0.0, 0));
		if (i == 3) text_PS (o, X + stp - 30.0 * P.D, P.O1Y + 8.0 * P.D, 3, "MAX");
	}
	return;
}

void PS_well_coordinate_axes_FREQUENCY (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const double STEP) {

	const PAPER P = RETURN_PAPER();

	newpath_PS(o);
	color_PS (o, "0.0 0.0 0.0");
	linewidth_PS (o, 1, 1);

	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X, P.O1Y - LENGTH, 3);
	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X + PL_WDT * P.A, P.O1Y, 3);

	font_PS(o, "ArialNarrow-Bold", 24);
	color_PS (o, "0.9 0.9 0.9");

	text_PS(o, X + 1.0 * P.A, P.O1Y + 45.0 * P.D, 3, "FREQUENCY");

	font_PS(o, "ArialNarrow", 12);

	color_PS (o, "0.0 0.0 0.0");
	text_PS(o, X + 1.5 * P.A, P.O1Y + 25.0 * P.D, 3, "VALUE");

	color_PS (o, "1.0 0.0 0.0");
	text_PS(o, X + 4.3 * P.A, P.O1Y + 25.0 * P.D, 3, "DERIVATE");

	color_PS (o, "0.0 0.0 0.0");

	for (size_t i = 0; i < 4; i++) {

		const double stp = (i * PL_WDT * P.A) / 3.0;

		if (i == 3) {

			moveto_PS (o, X + stp, P.O1Y, 3);
			lineto_PS (o, X + stp, P.O1Y + 5.0 * P.D, 3);
			stroke_PS(o);

			color_PS (o, "0.9 0.9 0.9");

			moveto_PS (o, X + stp, P.O1Y - LENGTH, 3);
			lineto_PS (o, X + stp, P.O1Y - 5.0 * P.D, 3);
			stroke_PS(o);

			color_PS (o, "0.0 0.0 0.0");
		}
		if (i == 0) text_PS (o, X + stp - 3.0 * P.D, P.O1Y + 8.0 * P.D, 3, double_to_string (0.0, 0));
		if (i == 3) text_PS (o, X + stp - 30.0 * P.D, P.O1Y + 8.0 * P.D, 3, "MAX");
	}

	for (double i = MIN_VAL; i < MAX_VAL + 0.5*STEP; i+=STEP) {

		const double INT = (i - MIN_VAL) / (MAX_VAL - MIN_VAL);

		if (i > MIN_VAL + 0.5*STEP) {

			color_PS (o, "0.9 0.9 0.9");

			moveto_PS (o, X + PL_WDT * P.A, P.O1Y - (INT * LENGTH), 3);
			lineto_PS (o, X + 5.0 * P.D, 	P.O1Y - (INT * LENGTH), 3);
			stroke_PS(o);

			color_PS (o, "0.0 0.0 0.0");
		}
		moveto_PS (o, X, 				P.O1Y - (INT * LENGTH), 3);
		lineto_PS (o, X - 5.0 * P.D, 	P.O1Y - (INT * LENGTH), 3);
		stroke_PS(o);
	}
	return;
}

void PS_derivate_DIPDIR_DIP (ofstream& o, const double X) {

	const PAPER P = RETURN_PAPER();

	newpath_PS(o);
	color_PS (o, "0.0 0.0 0.0");
	linewidth_PS (o, 1, 1);

	moveto_PS (o, X, P.O1Y, 3);
	lineto_PS (o, X + PL_WDT * P.A * 0.5, P.O1Y, 3);

	stroke_PS (o);

	return;
}

void SETUP_PEAK (const vector <double>& DEPTH, const vector <double>& VALUE) {

	if (DEPTH.size() != VALUE.size()) ASSERT_DEAD_END();

	if (DEPTH.size() < 1 || VALUE.size() < 1) ASSERT_DEAD_END();

	PEAK.clear();

	for (size_t i = 0; i < DEPTH.size(); i++) {

		if (i < DEPTH.size() - 1 && DEPTH.at(i) > DEPTH.at(i+1)) ASSERT_DEAD_END();

		PEAK_TO_PLOT buf;

		buf.DEPTH = DEPTH.at(i);
		buf.VALUE = VALUE.at(i);
		buf.COUNT = 0;

		PEAK.push_back (buf);
	}
	return;
}

double RETURN_VALUE_AVERAGE (vector <PEAK_TO_PLOT> IN) {

	vector <double> FOR_AVERAGE;

	for (size_t i = 0; i < IN.size(); i++) FOR_AVERAGE.push_back(IN.at(i).VALUE);

	return average (FOR_AVERAGE);

}
double RETURN_DEPTH_AVERAGE (vector <PEAK_TO_PLOT> IN) {

	vector <double> FOR_AVERAGE;

	for (size_t i = 0; i < IN.size(); i++) FOR_AVERAGE.push_back(IN.at(i).DEPTH);

	return average (FOR_AVERAGE);
}

vector <PEAK_TO_PLOT> generate_flattened_curve (const size_t bin) {

	vector <PEAK_TO_PLOT> OUT;

	const size_t PEAK_SIZE = PEAK.size();

	PEAK_TO_PLOT MIN = PEAK.at(0);
	PEAK_TO_PLOT MAX = PEAK.at(PEAK_SIZE - 1);

	MIN.DEPTH = MIN.DEPTH - 1;
	MIN.VALUE = RETURN_VALUE_AVERAGE (PEAK);

	MAX.DEPTH = MAX.DEPTH + 1;
	MAX.VALUE = RETURN_VALUE_AVERAGE (PEAK);

	OUT.push_back (MIN);

	for (size_t i = 0; i < PEAK_SIZE - bin + 1; i++) {

		vector <double> DEPTH_FOR_AVERAGE;
		vector <double> VALUE_FOR_AVERAGE;

		PEAK_TO_PLOT buf;

		for (size_t j = 0; j < bin; j++) {

			DEPTH_FOR_AVERAGE.push_back (PEAK.at (i+j).DEPTH);
			VALUE_FOR_AVERAGE.push_back (PEAK.at (i+j).VALUE);
		}
		buf.DEPTH = average(DEPTH_FOR_AVERAGE);
		buf.VALUE = average(VALUE_FOR_AVERAGE);
		buf.COUNT = NaN();

		OUT.push_back (buf);
	}
	OUT.push_back (MAX);

	return OUT;
}

void count_real_peaks (const vector <PEAK_TO_PLOT>& FL_PK, const string METHOD) {

	const bool MIN = (METHOD == "MIN" || METHOD == "MINMAX");
	const bool MAX = (METHOD == "MAX" || METHOD == "MINMAX");

	for (size_t j = 0; j < FL_PK.size() - 1; j++) {
		for (size_t i = 1; i < PEAK.size() - 1; i++) {

			const bool LAST = i == PEAK.size() - 2;

			const bool RANGE_OK_LAST = LAST && is_in_range_LW_EQ (FL_PK.at(j).DEPTH, FL_PK.at(j+1).DEPTH, PEAK.at(i).DEPTH);
			const bool RANGE_OK_OTHR = !LAST && is_in_range_UP_EQ (FL_PK.at(j).DEPTH, FL_PK.at(j+1).DEPTH, PEAK.at(i).DEPTH);

			const bool PRW_SLOPE_UP = PEAK.at(i-1).VALUE < PEAK.at(i).VALUE;
			const bool NXT_SLOPE_UP = PEAK.at(i).VALUE < PEAK.at(i+1).VALUE;

			const bool MAX_OK = PRW_SLOPE_UP && !NXT_SLOPE_UP;
			const bool MIN_OK = !PRW_SLOPE_UP && NXT_SLOPE_UP;

			const bool EVAL_MAX = MAX_OK && MAX;
			const bool EVAL_MIN = MIN_OK && MIN;

			const bool EVAL = (RANGE_OK_LAST || RANGE_OK_OTHR) && (EVAL_MAX || EVAL_MIN);

			if (EVAL) {

				const double ACT_D = FL_PK.at(j).DEPTH;
				const double NXT_D = FL_PK.at(j+1).DEPTH;
				const double PEAK_D = PEAK.at(i).DEPTH;

				const double ACT_V = FL_PK.at(j).VALUE;
				const double NXT_V = FL_PK.at(j+1).VALUE;
				const double PEAK_V = PEAK.at(i).VALUE;

				if (NXT_D <= ACT_D) ASSERT_DEAD_END();

				const double M = (NXT_D - ACT_D) / (PEAK_D - ACT_D);

				const double P = ((NXT_V - ACT_V) / M) + ACT_V;

				if ((MAX && PEAK_V > P) || (MIN && PEAK_V < P)) PEAK.at(i).COUNT = PEAK.at(i).COUNT + 1;
			}
		}
	}
	return;
}

void rescale_peaks () {

	double MAX = 0;

	for (size_t i = 0; i < PEAK.size(); i++) if (PEAK.at(i).COUNT > MAX) MAX = PEAK.at(i).COUNT;

	for (size_t i = 0; i < PEAK.size(); i++) PEAK.at(i).COUNT = PEAK.at(i).COUNT / MAX;

	return;
}

void associate_peaks_to_faults () {

	for (size_t i = 0; i < FAULTS.size() - 1; i++) {
		for (size_t j = 0; j < PEAK.size(); j++) {

			const double INT_MIN = FAULTS.at(i).DEPTH;
			const double INT_MAX = FAULTS.at(i+1).DEPTH;

			const bool LAST = FAULTS.size() - 2;

			bool FIT;

			if (LAST) 	FIT = is_in_range_UP_EQ (INT_MIN, INT_MAX, PEAK.at(j).DEPTH);
			else 		FIT = is_in_range_LW_EQ (INT_MIN, INT_MAX, PEAK.at(j).DEPTH);

			if (FIT) {

				FAULTS.at(i).COUNT = FAULTS.at(i).COUNT + PEAK.at(j).COUNT;
			}
		}
	}
	return;
}

void PEAK_IDENTIFICATION (const vector <double>& DEPTH, const vector <double>& VALUE, const string METHOD) {

	SETUP_PEAK (DEPTH, VALUE);

	if (METHOD == "NONE") return;

	vector <PEAK_TO_PLOT> PK = PEAK;

	const size_t STEP =  string_to_size_t (double_to_string ((PEAK.size() / 10.0), 0));

	for (size_t i = 2; i < PK.size(); i+=STEP) {

		const vector <PEAK_TO_PLOT> FL_PK = generate_flattened_curve (i);

		count_real_peaks (FL_PK, METHOD);
	}
	rescale_peaks ();

	if (is_CHK_WELL()) STANDARD_OUTPUT_PEAKS (METHOD);

	associate_peaks_to_faults ();

	if (is_CHK_WELL()) STANDARD_OUTPUT_FAULTS (METHOD);

	return;
}

void plot_well_faults (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL) {

	const PAPER P = RETURN_PAPER();

	if (FAULTS.size() < 1) return;

	if (is_CHK_WELL()) STANDARD_OUTPUT_FAULTS ("FINAL");

	double MAX = 0;

	color_PS (o, "0.00 0.00 0.00");
	linewidth_PS (o, 2, 1);

	for (size_t i = 0; i < FAULTS.size(); i++) if (FAULTS.at(i).COUNT > MAX) MAX = FAULTS.at(i).COUNT;

	for (size_t i = 0; i < FAULTS.size(); i++) {

		const double DEPTH = FAULTS.at(i).DEPTH;
		const double VALUE =(FAULTS.at(i).COUNT / MAX) * (FAULTS.at(i).COUNT / MAX);

		const double ACT_data_X = X + (PL_WDT * P.A * VALUE);
		const double ACT_data_Y = P.O1Y - (LENGTH * ((DEPTH - MIN_VAL) / (MAX_VAL - MIN_VAL)));

		newpath_PS (o);
		moveto_PS (o, X, ACT_data_Y, 3);
		lineto_PS (o, ACT_data_X, ACT_data_Y, 3);
		stroke_PS (o);
	}
	return;
}

void plot_well_frequency_derivate (const vector <WELL_FREQUENCY> IN, ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL) {

	if (IN.size() < 1) return;

	vector <double> DEPTH;
	vector <double> VALUE;

	for (size_t i = 0; i < IN.size() - 1; i++) {

		DEPTH.push_back (IN.at(i).DERIV_DEPTH);
		VALUE.push_back (IN.at(i).DERIV);
	}
	PEAK_IDENTIFICATION (DEPTH, VALUE, "MAX");

	plot_peaks (o, X, LENGTH, MIN_VAL, MAX_VAL, false, "DERIVATE");

	plot_curve (DEPTH, VALUE, o, X, LENGTH, MIN_VAL, MAX_VAL, false, "DERIVATE");

	return;
}

void plot_well_frequency (const vector <WELL_FREQUENCY> IN, ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL) {

	if (IN.size() < 1) return;

	vector <double> DEPTH;
	vector <double> VALUE;

	for (size_t i = 0; i < IN.size(); i++) {

		DEPTH.push_back (IN.at(i).DEPTH);
		VALUE.push_back (IN.at(i).FREQ);
	}
	PEAK_IDENTIFICATION (DEPTH, VALUE, "MAX");

	plot_peaks (o, X, LENGTH, MIN_VAL, MAX_VAL, false, "FREQUENCY");

	plot_curve (DEPTH, VALUE, o, X, LENGTH, MIN_VAL, MAX_VAL, false, "FREQUENCY");

	return;
}

vector <XY> cutting_points (const vector <XY>& IN) {

	vector <XY> OUT;

	for (size_t i = 0; i < IN.size() - 1; i++) {

		const bool LAST = i == IN.size() - 2;

		XY buf = IN.at(i);

		const double ACT = IN.at(i).X;
		const double NXT = IN.at(i+1).X;

		const double ACT_DPT = IN.at(i).Y;
		const double NXT_DPT = IN.at(i+1).Y;

		const bool PLUS = (NXT - ACT) > 180.0; 		// 012 > 355
		const bool MINUS = (NXT - ACT) < -180.0;	// 355 > 012

		OUT.push_back (buf);

		if (PLUS) {

			buf.X = -999.99;
			buf.Y = (2.0 * ACT_DPT + NXT_DPT) / 3.0;
			OUT.push_back (buf);

			buf.X = 999.99;
			buf.Y = (ACT_DPT + 2.0 * NXT_DPT) / 3.0;
			OUT.push_back (buf);
		}
		else if (MINUS) {

			buf.X = 999.99;
			buf.Y = (2.0 * ACT_DPT + NXT_DPT) / 3.0;
			OUT.push_back (buf);

			buf.X = -999.99;
			buf.Y = (ACT_DPT + 2.0 * NXT_DPT) / 3.0;
			OUT.push_back (buf);
		}
		else {}

		if (LAST) OUT.push_back (IN.at(i+1));
	}
	return OUT;
}

vector <XY> generate_xy_vector (const vector <double>& VALUE, const vector <double>& DEPTH, const bool DIPDIR) {

	if (DEPTH.size() != VALUE.size()) ASSERT_DEAD_END();

	vector <XY> OUT;

	for (size_t i = 0; i < VALUE.size(); i++) {

		XY buf;

		const double V = VALUE.at(i);

		buf.Y = DEPTH.at(i);

		if (DIPDIR) {

			if (V < 0.0) 		buf.X = 360.0 + V;
			else if (V > 360.0) buf.X = V - 360.0;
			else 				buf.X = V;
		}
		else {

			if (V < -90.0) 		buf.X = 0.0;
			else if (V > 90.0) 	buf.X = 90.0;
			else 				buf.X = V;
		}
		OUT.push_back (buf);
	}
	return OUT;
}

vector <vector <XY> >  generate_xy_vector_vector (const vector <XY>& IN) {

	vector <vector <XY> > OUT;

	vector <XY> BUF;

	for (size_t i = 0; i < IN.size() - 1; i++) {

		const bool LAST = i == IN.size() - 2;

		XY buf = IN.at(i);
		BUF.push_back (buf);

		const double ACT_VAL = IN.at(i).X;
		const double NXT_VAL = IN.at(i+1).X;

		const bool CUT = (ACT_VAL < -900.0 && NXT_VAL > 900.0) || (ACT_VAL > 900.0 && NXT_VAL < -900.0);

		if ((ACT_VAL > 900.0 && NXT_VAL > 900.0) || (ACT_VAL < -900.0 && NXT_VAL < -900.0)) ASSERT_DEAD_END();

		if (CUT) {

			OUT.push_back(BUF);
			BUF.clear();
		}

		if (LAST) {

			XY buf = IN.at(i+1);
			BUF.push_back (buf);
			OUT.push_back(BUF);
		}
	}
	return OUT;
}


vector <vector <XY> > tidy_xy_vector_vector (vector <vector <XY> >& IN) {

	vector <vector <XY> > OUT = IN;

	for (size_t i = 0; i < OUT.size(); i++) {

		const double FST = OUT.at(i).at (0).X;
		const double LST = OUT.at(i).at (OUT.at(i).size() - 1).X;

		const bool FST_OK = (FST < -900 || FST > 900);
		const bool LST_OK = (LST < -900 || LST > 900);

		if (i > 0 && i < OUT.size() - 1 && !(FST_OK && LST_OK)) ASSERT_DEAD_END();
		if (i == 0  && FST_OK) ASSERT_DEAD_END();
		if (i == OUT.size() && LST_OK) ASSERT_DEAD_END();

		for (size_t j = 0; j < OUT.at(i).size(); j++) {

			if (OUT.at(i).at(j).X < -900.0) OUT.at(i).at(j).X = 0.0;
			if (OUT.at(i).at(j).X > 900.0) OUT.at(i).at(j).X = 360.0;
		}
	}
	return OUT;
}


vector <double> generate_VALUE_from_XY_vector (const vector <XY>& IN) {

	vector <double> OUT;

	for (size_t i = 0; i < IN.size(); i++) OUT.push_back(IN.at(i).X);

	return OUT;
}


vector <double> generate_DEPTH_from_XY_vector (const vector <XY>& IN) {

	vector <double> OUT;

	for (size_t i = 0; i < IN.size(); i++) OUT.push_back(IN.at(i).Y);

	return OUT;
}

void plot_curve (const vector <double> DEPTH, const vector <double> VALUE, ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE) {

	const PAPER P = RETURN_PAPER();

	if (DEPTH.size() != VALUE.size()) ASSERT_DEAD_END();
	if (DEPTH.size() < 1) return;

	vector <double> ACT_X;
	vector <double> ACT_Y;
	vector <double> NXT_X;
	vector <double> NXT_Y;

	const bool A = TYPE == "AVERAGE";
	const bool E_MN = TYPE == "LOWER_STDEV";
	const bool E_MX = TYPE == "UPPER_STDEV";
	const bool D = TYPE == "DERIVATE";
	const bool F = TYPE == "FREQUENCY";

	double MX = 90.0;
	if (DIPDIR) MX = 360.0;
	if (D) MX = 2.0;
	if (F) MX = 1.0;

	double MN = 0.0;
	if (!DIPDIR) MN = -90.0;
	if (D) MN = 0.0;
	if (F) MN = 0.0;

	string CRV_CLR = STD_CLR;
	if (A) CRV_CLR = AVR_CLR;
	if (D) CRV_CLR = DRV_CLR;
	if (F) CRV_CLR = FRQ_CLR;

	double LW = 1.0;
	if (E_MN || E_MX) LW = 2.0;

	color_PS (o, CRV_CLR);
	linewidth_PS(o, LW, 1);

	for (size_t i = 0; i < DEPTH.size() - 1; i++) {

		const double ACT_D = DEPTH.at(i);
		const double ACT_V = VALUE.at(i);

		const double NXT_D = DEPTH.at(i + 1);
		const double NXT_V = VALUE.at(i + 1);

		const double ACT_data_X = X + (PL_WDT * P.A * ((ACT_V - MN) / (MX - MN)));
		const double NXT_data_X = X + (PL_WDT * P.A * ((NXT_V - MN) / (MX - MN)));

		const double ACT_data_Y = P.O1Y - (LENGTH * ((ACT_D - MIN_VAL) / (MAX_VAL - MIN_VAL)));
		const double NXT_data_Y = P.O1Y - (LENGTH * ((NXT_D - MIN_VAL) / (MAX_VAL - MIN_VAL)));

		ACT_X.push_back (ACT_data_X);
		ACT_Y.push_back (ACT_data_Y);
		NXT_X.push_back (NXT_data_X);
		NXT_Y.push_back (NXT_data_Y);
	}

	if (!(ACT_X.size() == ACT_Y.size() && ACT_Y.size() == NXT_X.size() &&  NXT_X.size() == NXT_Y.size())) ASSERT_DEAD_END();

	newpath_PS(o);
	color_PS (o, CRV_CLR);
	linewidth_PS(o, LW, 1);

	for (size_t i = 0; i < ACT_X.size(); i++) {

		moveto_PS(o, ACT_X.at(i), ACT_Y.at(i), 3);
		lineto_PS(o, NXT_X.at(i), NXT_Y.at(i), 3);
	}
	stroke_PS (o);
	return;
}

void plot_peaks (ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE) {

	const PAPER P = RETURN_PAPER();

	vector <double> ACT_X;
	vector <double> ACT_Y;

	const bool A = TYPE == "AVERAGE";
	const bool E_MN = TYPE == "LOWER_STDEV";
	const bool E_MX = TYPE == "UPPER_STDEV";
	const bool D = TYPE == "DERIVATE";
	const bool F = TYPE == "FREQUENCY";

	double MX = 90.0;
	if (DIPDIR) MX = 360.0;
	if (D) MX = 2.0;
	if (F) MX = 1.0;

	double MN = 0.0;
	if (!DIPDIR) MN = -90.0;
	if (D) MN = 0.0;
	if (F) MN = 0.0;

	string CRV_CLR = STD_CLR;
	if (A) CRV_CLR = AVR_CLR;
	if (D) CRV_CLR = DRV_CLR;
	if (F) CRV_CLR = FRQ_CLR;

	double LW = 1.0;
	if (E_MN || E_MX) LW = 2.0;

	color_PS (o, CRV_CLR);
	linewidth_PS(o, LW, 1);

	for (size_t i = 0; i < PEAK.size(); i++) {

		const double ACT_D = PEAK.at(i).DEPTH;
		const double ACT_V = PEAK.at(i).VALUE;

		const double ACT_data_X = X + (PL_WDT * P.A * ((ACT_V - MN) / (MX - MN)));
		const double ACT_data_Y = P.O1Y - (LENGTH * ((ACT_D - MIN_VAL) / (MAX_VAL - MIN_VAL)));

		ACT_X.push_back (ACT_data_X);
		ACT_Y.push_back (ACT_data_Y);
	}

	if (!(ACT_X.size() == ACT_Y.size() && ACT_Y.size() == PEAK.size())) ASSERT_DEAD_END();

	linewidth_PS(o, LW, 0);

	for (size_t i = 0; i < PEAK.size(); i++) {

		if (PEAK.at(i).COUNT > 0.0) {

			newpath_PS(o);
			string CRC_CLR = generate_peak_colors (PEAK.at(i).COUNT * 100);
			arc_PS (o, ACT_X.at(i), ACT_Y.at(i), 1.5, 0, 360, 1);
			color_PS (o, CRC_CLR);
			fill_PS (o);
			stroke_PS (o);
		}
	}
	return;
}

double return_plot_value (const WELL_INTERVAL ACT, const bool DIPDIR, const string TYPE) {

	const bool A = TYPE == "AVERAGE";
	const bool E_MN = TYPE == "LOWER_STDEV";
	const bool E_MX = TYPE == "UPPER_STDEV";
	const bool D = TYPE == "DERIVATE";

	const double DIPD = ACT.INT_AV_DD.DIPDIR;
	const double DD_STDEV = ACT.INT_AV_DD_STDEV;
	const double D_STDEV = ACT.INT_AV_D_STDEV;
	const double DD_DRV = ACT.DD_DERIV;
	const double D_DRV = ACT.D_DERIV;

	double DIP = ACT.INT_AV_DD.DIP;

	if (is_D_up(ACT.INT_AV_D)) DIP = - DIP;

	if (A && DIPDIR) return DIPD;

	else if (A && !DIPDIR)	return DIP;

	else if (E_MN && DIPDIR) return DIPD - 0.5 * DD_STDEV;

	else if (E_MX && DIPDIR) return DIPD + 0.5 * DD_STDEV;

	else if (E_MN && !DIPDIR) return DIP - 0.5 * D_STDEV;

	else if (E_MX && !DIPDIR) return DIP + 0.5 * D_STDEV;

	else if (D && DIPDIR) return DD_DRV;

	else if (D && !DIPDIR) return D_DRV;

	else {

		ASSERT_DEAD_END();
		return NaN();
	}
}

void return_records_with_formation_names (const vector <vector <GDB> >& inGDB_G){

	const vector <GDB> processGDB = SORT_GDB (MERGE_GROUPS_TO_GDB (inGDB_G), "DEPTH");

	WTP.clear();

	for (size_t i = 1; i < processGDB.size(); i++) {

		WELL_TOPS buf;

		const string ACT_FRM = processGDB.at(i - 1).FORMATION;
		const string NXT_FRM = processGDB.at(i).FORMATION;

		if (i == 1 && ACT_FRM.size() > 0) {

			buf.DEPTH = processGDB.at(i - 1).DEPTH;
			buf.FORMATION = processGDB.at(i - 1).FORMATION;
			WTP.push_back(buf);
		}

		if (ACT_FRM != NXT_FRM) {

			buf.DEPTH = processGDB.at(i).DEPTH;
			buf.FORMATION = processGDB.at(i).FORMATION;
			WTP.push_back(buf);
		}
	}
	return;
}

void plot_well_curve (const vector <WELL_INTERVAL>& IN, ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR, const string TYPE) {

	if (IN.size() < 2) return;

	vector <double> DEPTH;
	vector <double> VALUE;

	for (size_t i = 0; i < IN.size() - 1; i++) {

		const bool LAST = i == IN.size() - 1;

		const WELL_INTERVAL ACT = IN.at(i);
		const WELL_INTERVAL NXT = IN.at(i + 1);

		if (ACT.SIZE > 0 && NXT.SIZE > 0) {

			const double ACT_DATA = return_plot_value (ACT, DIPDIR, TYPE);
			const double NXT_DATA = return_plot_value (NXT, DIPDIR, TYPE);

			VALUE.push_back (ACT_DATA);
			DEPTH.push_back (ACT.DEPTH);

			if (LAST) {

				VALUE.push_back (NXT_DATA);
				DEPTH.push_back (NXT.DEPTH);
			}
		}
	}
	if 		(TYPE == "DERIVATE") 	PEAK_IDENTIFICATION (DEPTH, VALUE, "MAX");
	else if (TYPE == "AVERAGE") 	PEAK_IDENTIFICATION (DEPTH, VALUE, "MINMAX");
	else 							PEAK_IDENTIFICATION (DEPTH, VALUE, "NONE");

	if (is_CHK_WELL()) STANDARD_OUTPUT_WELL_PS (DEPTH, VALUE, DIPDIR, TYPE);

	vector <XY> PLOT = generate_xy_vector (VALUE, DEPTH, DIPDIR);

	PLOT = cutting_points (PLOT);

	vector <vector <XY> > PLOT_V = generate_xy_vector_vector (PLOT);

	PLOT_V = tidy_xy_vector_vector (PLOT_V);

	for (size_t i = 0; i < PLOT_V.size(); i++) {

		VALUE = generate_VALUE_from_XY_vector (PLOT_V.at(i));
		DEPTH = generate_DEPTH_from_XY_vector (PLOT_V.at(i));

		plot_peaks (o, X, LENGTH, MIN_VAL, MAX_VAL, DIPDIR, TYPE);

		plot_curve (DEPTH, VALUE, o, X, LENGTH, MIN_VAL, MAX_VAL, DIPDIR, TYPE);
	}
	return;
}

void plot_well_measurements (const vector <GDB>& inGDB, ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR) {

	const PAPER P = RETURN_PAPER();

	const vector <GDB> temp = SORT_GDB(inGDB, "DEPTH");

	double MX = 90.0;
	if (DIPDIR) MX = 360.0;

	double MN = -90.0;
	if (DIPDIR) MN = 0.0;

	const bool B = is_allowed_handle_as_bedding(inGDB.at(0).DATATYPE);

	for (size_t i = 0; i < inGDB.size(); i++) {

		const bool O = is_allowed_bedding_overturned_sense(inGDB.at(i).OFFSET);

		if (O && !B) ASSERT_DEAD_END();

		double DATA = NaN();

		if (DIPDIR) DATA = inGDB.at(i).corr.DIPDIR;
		else {

			if (O && B) DATA = - inGDB.at(i).corr.DIP;
			else		DATA =   inGDB.at(i).corr.DIP;
		}
		const double data_X = PL_WDT * P.A * ((DATA - MN) / (MX - MN));
		const double data_Y = LENGTH * ((inGDB.at(i).DEPTH - MIN_VAL) / (MAX_VAL - MIN_VAL));

		translate_PS (o, X + data_X, P.O1Y - data_Y, 3);

		if (O && B) text_PS (o, " newpath overturned_measurement");
		else text_PS (o, " newpath measurement");

		translate_PS (o, - X - data_X, - P.O1Y + data_Y, 3);
	}
	stroke_PS (o);

	return;
}

void PS_well_intervals_error (const vector <WELL_INTERVAL>& INTERVAL, ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL, const bool DIPDIR) {

	const PAPER P = RETURN_PAPER();

	//has bugs while generating colour code

	double  MIN_ERROR = 999.0;
	double  MAX_ERROR = 0.0;

	for (size_t i = 0; i < INTERVAL.size(); i++) {

		double 		SD = INTERVAL.at(i).INT_AV_D_STDEV;
		if (DIPDIR) SD = INTERVAL.at(i).INT_AV_DD_STDEV;

		if (SD > MAX_ERROR) MAX_ERROR = SD;
		if (SD < MIN_ERROR) MIN_ERROR = SD;
	}

	for (size_t i = 0; i < INTERVAL.size(); i++) {

		const WELL_INTERVAL I = INTERVAL.at(i);

		double 		SD = I.INT_AV_D_STDEV;
		if (DIPDIR) SD = I.INT_AV_DD_STDEV;

		const double MIN = I.MIN;
		const double MAX = I.MAX;

		const double COUNT = string_to_double (size_t_to_string (i));
		const double END = string_to_double (size_t_to_string (INTERVAL.size()));

		const double WIDTH = (PL_WDT * P.A) / END;

		const double X1 = X + PL_WDT * P.A * (COUNT / END);

		const double Y1 = P.O1Y - LENGTH * ((MAX - MIN_VAL) / (MAX_VAL - MIN_VAL));
		const double Y2 = P.O1Y - LENGTH * ((MIN - MIN_VAL) / (MAX_VAL - MIN_VAL));

		double CLR_RATIO = 100 * (1.0 * (SD - MIN_ERROR)) / (MAX_ERROR - MIN_ERROR);

		if (INTERVAL.at(i).SIZE == 0) CLR_RATIO = 0.0;

		string CLR = generate_error_colors (string_to_int (double_to_string (CLR_RATIO, 0)));

		newpath_PS(o);
		linewidth_PS (o, 0.5, 1);
		color_PS (o, CLR);

		moveto_PS (o, X1, Y1, 3);
		lineto_PS (o, X1, Y2, 3);
		lineto_PS (o, X1 + WIDTH, Y2, 3);
		lineto_PS (o, X1 + WIDTH, Y1, 3);
		lineto_PS (o, X1, Y1, 3);

		fill_PS (o);

		stroke_PS (o);
	}
}

void PS_well_intervals (const vector <WELL_INTERVAL>& INTERVAL, ofstream& o, const double X, const double LENGTH, const double MIN_VAL, const double MAX_VAL) {

	const PAPER P = RETURN_PAPER();

	const double LW = 0.5;

	linewidth_PS(o, LW, 1);

	size_t MAX_DATA = 0;
	size_t MIN_DATA = 999;

	for (size_t i = 0; i < INTERVAL.size(); i++) {

		if (INTERVAL.at(i).SIZE > MAX_DATA) MAX_DATA = INTERVAL.at(i).SIZE;
		if (INTERVAL.at(i).SIZE < MIN_DATA) MIN_DATA = INTERVAL.at(i).SIZE;
	}

	for (size_t i = 0; i < INTERVAL.size(); i++) {

		newpath_PS(o);

		const double MIN = INTERVAL.at(i).MIN;
		const double MAX = INTERVAL.at(i).MAX;

		const double COUNT = string_to_double (size_t_to_string (i));
		const double END = string_to_double (size_t_to_string (INTERVAL.size()));

		const double WIDTH = (PL_WDT * P.A) / END;

		const double X1 = X + PL_WDT * P.A * (COUNT / END);

		const double Y1 = P.O1Y - LENGTH * ((MAX - MIN_VAL) / (MAX_VAL - MIN_VAL));
		const double Y2 = P.O1Y - LENGTH * ((MIN - MIN_VAL) / (MAX_VAL - MIN_VAL));

		double CLR_RATIO = 100 * (1.0 * (INTERVAL.at(i).SIZE) - MIN_DATA) / (MAX_DATA - MIN_DATA);

		if (INTERVAL.at(i).SIZE == 0) CLR_RATIO = 0.0;

		string CLR = generate_binsize_colors (string_to_int (double_to_string (CLR_RATIO, 0)));

		color_PS (o, CLR);

		moveto_PS (o, X1, Y1, 3);
		lineto_PS (o, X1, Y2, 3);
		lineto_PS (o, X1 + WIDTH, Y2, 3);
		lineto_PS (o, X1 + WIDTH, Y1, 3);
		lineto_PS (o, X1, Y1, 3);

		fill_PS (o);

		stroke_PS (o);
	}
}

void INIT_FAULT_POSITIONS (const double MIN_VAL, const double MAX_VAL, const double STEP) {

	FAULTS.clear();

	for (double i = MIN_VAL; i <= MAX_VAL; i+=1) {

		PEAK_TO_PLOT buf;

		buf.DEPTH = i + (0.5);
		buf.VALUE = NaN();
		buf.COUNT = 0;

		FAULTS.push_back (buf);
	}
	return;
}

void SETUP_FAULT_POSITIONS (const double MIN_VAL, const double MAX_VAL, const double STEP) {

	INIT_FAULT_POSITIONS (MIN_VAL, MAX_VAL, STEP);

	return;
}

void WELL_PS (const vector <GDB>& inGDB, const vector <WELL_INTERVAL>& INT, const vector <WELL_FREQUENCY>& FREQ, ofstream& OPS, const double MIN_VAL, const double MAX_VAL, const double STEP) {

	const PAPER P = RETURN_PAPER();
	SETUP_FAULT_POSITIONS (MIN_VAL, MAX_VAL, STEP);

	const double LENGTH = P.Y * 0.8;

	//cout << "1A-----------------------------------------" << endl;
	double X = P.O1X + PL_LF * P.A;
	PS_well_coordinate_grid_INTERVAL (OPS, X, LENGTH, MIN_VAL, MAX_VAL, STEP);
	PS_well_intervals (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL);
	PS_well_coordinate_axes_INTERVAL (OPS, X, LENGTH);
	ps_well_formation_tops (OPS, X, LENGTH, MIN_VAL, MAX_VAL, STEP);

	//cout << "2A-----------------------------------------" << endl;
	X = X + (PL_WDT + PL_GP) * P.A;
	PS_well_coordinate_grid_DIPDIR (OPS, X, LENGTH, MIN_VAL, MAX_VAL, STEP);
	PS_well_intervals_error (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, true);
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, true, "LOWER_STDEV");
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, true, "UPPER_STDEV");
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, true, "AVERAGE");
	plot_well_measurements (inGDB, OPS, X, LENGTH, MIN_VAL, MAX_VAL, true);
	PS_well_coordinate_axes_DIPDIR (OPS, X, LENGTH);

	//cout << "2B-----------------------------------------" << endl;
	X = X + (PL_WDT + PL_AX_GP) * P.A;
	PS_derivate_DIPDIR_DIP (OPS, X);
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, true, "DERIVATE");

	//cout << "3A-----------------------------------------" << endl;
	X = X + (PL_WDT * 0.5 + PL_GP) * P.A;
	PS_well_coordinate_grid_DIP (OPS, X, LENGTH, MIN_VAL, MAX_VAL, STEP);
	PS_well_intervals_error (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, false);
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, false, "LOWER_STDEV");
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, false, "UPPER_STDEV");
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, false, "AVERAGE");
	plot_well_measurements (inGDB, OPS, X, LENGTH, MIN_VAL, MAX_VAL, false);
	PS_well_coordinate_axes_DIP (OPS, X, LENGTH);

	//cout << "3B-----------------------------------------" << endl;
	X = X + (PL_WDT + PL_AX_GP) * P.A;
	PS_derivate_DIPDIR_DIP (OPS, X);
	plot_well_curve (INT, OPS, X, LENGTH, MIN_VAL, MAX_VAL, false, "DERIVATE");

	//cout << "4A-----------------------------------------" << endl;
	X = X + (PL_WDT * 0.5 + PL_GP) * P.A;
	PS_well_coordinate_axes_FREQUENCY (OPS, X, LENGTH, MIN_VAL, MAX_VAL, STEP);
	plot_well_frequency (FREQ, OPS, X, LENGTH, MIN_VAL, MAX_VAL);

	//cout << "4B-----------------------------------------" << endl;
	X = X + (PL_WDT + PL_AX_GP) * P.A;
	PS_derivate_DIPDIR_DIP(OPS, X);
	plot_well_frequency_derivate (FREQ, OPS, X, LENGTH, MIN_VAL, MAX_VAL);

	//cout << "5A-----------------------------------------" << endl;
	X = X + (PL_WDT * 0.5 + PL_GP) * P.A;
	PS_well_coordinate_axes_FAULTS (OPS, X, LENGTH, MIN_VAL, MAX_VAL, STEP);
	plot_well_faults (OPS, X, LENGTH, MIN_VAL, MAX_VAL);

	return;
}

void OUTPUT_TO_WELL_PS (const vector <vector <GDB> >& GDB_G) {

	if (is_WELLDATA_NO()) return;

	const vector <vector <WELL_INTERVAL> > INTERVAL = RETURN_INTERVAL ();
	const vector <vector <WELL_FREQUENCY> > FREQUENCY = RETURN_FREQUENCY ();

	if (INTERVAL.size() != GDB_G.size()) ASSERT_DEAD_END();
	if (FREQUENCY.size() != GDB_G.size()) ASSERT_DEAD_END();

	return_records_with_formation_names (GDB_G);

	for (size_t i = 0; i < GDB_G.size(); i++) {

		const vector <GDB> temp = SORT_GDB (GDB_G.at(i), "DEPTH");

		const string DT = temp.at(0).DATATYPE;

		setup_ACTUAL_LOCATION (temp.at(0).LOC);
		setup_ACTUAL_DATATYPE (DT);
		setup_ACTUAL_FORMATION(temp.at(0).FORMATION);
		setup_ACTUAL_GROUPCODE(temp.at(0).GC);

		const bool PLN = is_allowed_plane_datatype (DT);
		const bool LIN = is_allowed_lineation_datatype (DT);

		const bool PROCESS_AS_WELL = PLN || LIN;

		if (PROCESS_AS_WELL) {

			const double MIN = temp.at(0).DEPTH;
			const double MAX = temp.at(temp.size() - 1).DEPTH;

			const double STEP = well_axes_step (MIN, MAX);

			const double MIN_VAL = return_MIN_value (temp, STEP);
			const double MAX_VAL = return_MAX_value (temp, STEP);

			INIT_PAPER (true);

			const bool GDB_OK = temp.size() > 1;

			const bool MIN_VAL_OK = MIN_VAL > 0.0;
			const bool MAX_VAL_OK = MAX_VAL > MIN_VAL;

			const bool INT_OK = INTERVAL.at(i).size() > 0;
			const bool FRQ_OK = FREQUENCY.at(i).size() > 0;

			if (GDB_OK && MIN_VAL_OK && MAX_VAL_OK && INT_OK && FRQ_OK) {

				string PS_NAME = generate_ACTUAL_WELL_PS_NAME ();

				ofstream OPS (PS_NAME.c_str());

				PS_well_header (OPS);

				PS_well_border (OPS);

				PS_well_symbols (OPS);

				WELL_PS (temp, INTERVAL.at(i), FREQUENCY.at(i), OPS, MIN_VAL, MAX_VAL, STEP);
			}
		}
	}
	return;
}

vector <size_t> return_INTERVAL_structure () {

	vector <vector <WELL_INTERVAL> > I = RETURN_INTERVAL();

	vector <size_t> OUT;

	for (size_t i = 0; i < I.size(); i++) {

		OUT.push_back (I.at(i).size());
	}
	return OUT;
}

vector <size_t> return_FREQUENCY_structure () {

	vector <vector <WELL_FREQUENCY> > F = RETURN_FREQUENCY();

	vector <size_t> OUT;

	for (size_t i = 0; i < F.size(); i++) {

		OUT.push_back (F.at(i).size());
	}
	return OUT;
}
