// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.


#include <iomanip>
#include <iostream>

#include "allowed_keys.hpp"
#include "rose.h"
#include "ps.h"

using namespace std;

ROSENUMBER compute_data_number_DIPDIR_DIP (vector <GDB> inGDB, double strike_begin, double strike_end, string DIPDIR_DIP) {

	ROSENUMBER counter;
	counter.LIN_NUM = 0.0;
	counter.PLN_NUM = 0.0;

	bool DIPDIR = 	(DIPDIR_DIP == "DIPDIR");
	bool DIP = 		(DIPDIR_DIP == "DIP");

	//if (!DIPDIR && !DIP) cout << "FALSE" << endl;
	//ASSERT(!DIPDIR && !DIP);

	bool LIN = (inGDB.at(0).DATAGROUP == "LINEATION");
	bool SC_STRIAE = (inGDB.at(0).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE");
	bool PLANE = (inGDB.at(0).DATAGROUP == "PLANE");

	for (size_t i = 0; i < inGDB.size(); i++) {

		double VALUE = 0.0;

		if (DIPDIR) VALUE = inGDB.at(i).corr.DIPDIR;
		if (DIP) 	VALUE = inGDB.at(i).corr.DIP;

		bool IN_RANGE = (is_in_range(strike_begin, strike_end, VALUE));

		if (IN_RANGE) {

			if (LIN || SC_STRIAE) counter.LIN_NUM++;
			if (PLANE || SC_STRIAE) counter.PLN_NUM++;

			//cout
			//<< strike_begin << '\t' << strike_end << '\t'
			//<< counter.LIN_NUM << '\t' << counter.PLN_NUM << endl;

		}
		else {}
	}
	return counter;
}

void PS_draw_rose_DATATYPE (vector <GDB> inGBD, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical) {

	bool LINEATION = 	(is_allowed_lineation_datatype	(inGBD.at(0).DATAGROUP));
	bool PLANE = 		(is_allowed_plane_datatype		(inGBD.at(0).DATAGROUP));
	bool SC = 			(is_allowed_SC_datatype			(inGBD.at(0).DATAGROUP));
	bool STRIAE = 		(is_allowed_striae_datatype		(inGBD.at(0).DATAGROUP));

	bool DRAW_PLN = (PLANE || SC || STRIAE);
	bool DRAW_LIN = (LINEATION || SC || STRIAE);

	bool ASYMM = (inset.rosetype == "A");
	bool SYMM  = (inset.rosetype == "S");

	//if (!ASYMM && !SYMM) cout << "ERROR" << endl;
	//if (!DRAW_PLN && !DRAW_LIN) cout << "ERROR" << endl;

	double VALUE = 0;

	if (DRAW_PLN) VALUE = percent.PLN_NUM;
	if (DRAW_LIN) VALUE = percent.LIN_NUM;

	if (VALUE < 0.9) return;

	if (vertical) {

		PS_rosesegment (o, inset, center, VALUE, 90 + begin_angle, SC, vertical);
		return;
	}

	if (ASYMM) PS_rosesegment (o, inset, center, VALUE, begin_angle, SC, vertical);

	if (SYMM) {
		PS_rosesegment (o, inset, center, VALUE, begin_angle,       SC, vertical);
		PS_rosesegment (o, inset, center, VALUE, begin_angle + 180, SC, vertical);
	}
}

void PS_draw_rose_DIPDIR (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center) {

	ROSENUMBER datanumber;
	ROSENUMBER mx;
	ROSENUMBER percent;

	mx.LIN_NUM = 0.0;
	mx.PLN_NUM = 0.0;

	percent.LIN_NUM = 0.0;
	percent.PLN_NUM = 0.0;

	//bool draw_strikes = false;
	//if (inset.rosedirection == "S") draw_strikes = true;

	double begin_angle = 0.0;

	double step_angle = 2.5;

	if (inset.rosebinning == "B") step_angle =  5.0;
	if (inset.rosebinning == "C") step_angle = 10.0;
	if (inset.rosebinning == "D") step_angle = 22.5;

	int segmentnumber = static_cast<int>(360.0 / step_angle);

	int i = 0;
	size_t j = 0;

	do {

		if (inset.rosetype == "A") datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIPDIR");
		else {

			datanumber.LIN_NUM =
					compute_data_number_DIPDIR_DIP 	(inGDB, begin_angle,         begin_angle + step_angle        , "DIPDIR").LIN_NUM +
					compute_data_number_DIPDIR_DIP 	(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0, "DIPDIR").LIN_NUM;

			datanumber.PLN_NUM =
					compute_data_number_DIPDIR_DIP	(inGDB, begin_angle,          begin_angle + step_angle        , "DIPDIR").PLN_NUM +
					compute_data_number_DIPDIR_DIP 	(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0, "DIPDIR").PLN_NUM;
		}

		if (datanumber.LIN_NUM > mx.LIN_NUM) mx.LIN_NUM = datanumber.LIN_NUM;
		if (datanumber.PLN_NUM > mx.PLN_NUM) mx.PLN_NUM = datanumber.PLN_NUM;

		begin_angle = begin_angle + step_angle;
		if (inset.rosetype == "S") i++;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	do {

		if (inset.rosetype == "A") {

			datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIPDIR");
			//cout << "ASYMM" << endl;

		}
		else {

			datanumber.LIN_NUM =
					compute_data_number_DIPDIR_DIP(inGDB, begin_angle, begin_angle + step_angle, "DIPDIR").LIN_NUM +
					compute_data_number_DIPDIR_DIP(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0, "DIPDIR").LIN_NUM;

			datanumber.PLN_NUM =
					compute_data_number_DIPDIR_DIP(inGDB, begin_angle, begin_angle + step_angle, "DIPDIR").PLN_NUM +
					compute_data_number_DIPDIR_DIP(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0, "DIPDIR").PLN_NUM;
		}

		percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
		percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

		//double prev_begin_angle;

		/*
		if (draw_strikes) {

			prev_begin_angle = begin_angle;

			if (begin_angle > 90.0) begin_angle = begin_angle - 90.0;
			else begin_angle = begin_angle + 270.0;
		}
		*/

		PS_draw_rose_DATATYPE(inGDB, o, inset, center, percent, begin_angle, false);

		//if (draw_strikes) prev_begin_angle = begin_angle;
		//else {}

		begin_angle = begin_angle + step_angle;
		if (inset.rosetype == "S") i++;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	percent.PLN_NUM = mx.PLN_NUM / inGDB.size();
	percent.LIN_NUM = mx.LIN_NUM / inGDB.size();

	if (inGDB.at(j).DATAGROUP == "LINEATION") {

		percent.PLN_NUM = percent.LIN_NUM;
	}

	PS_draw_rose_circle_horizontal (o, inset, center, percent);


	if ((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) {

		do {

			if (inset.rosetype == "A") datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIPDIR");
			else {

				datanumber.LIN_NUM =
						compute_data_number_DIPDIR_DIP(inGDB, begin_angle, begin_angle + step_angle, "DIPDIR").LIN_NUM +
						compute_data_number_DIPDIR_DIP(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0, "DIPDIR").LIN_NUM;

				datanumber.PLN_NUM =
						compute_data_number_DIPDIR_DIP(inGDB, begin_angle, begin_angle + step_angle, "DIPDIR").PLN_NUM +
						compute_data_number_DIPDIR_DIP(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0, "DIPDIR").PLN_NUM;
			}

			percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
			percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

			PS_draw_rose_DATATYPE(inGDB, o, inset, center, percent, begin_angle, false);
			//PS_draw_rose_SC_STRIAE (o, inset, center, percent, begin_angle, false);

			begin_angle = begin_angle + step_angle;
			if (inset.rosetype == "S") i++;
			i++;

		} while (i < segmentnumber);
	}
}

void PS_draw_rose_DIP (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center) {

	ROSENUMBER datanumber;
	ROSENUMBER mx;
	ROSENUMBER percent;

	mx.LIN_NUM = 0.0;
	mx.PLN_NUM = 0.0;

	percent.LIN_NUM = 0.0;
	percent.PLN_NUM = 0.0;

	double begin_angle = 0.0;
	double step_angle = 2.5;

	if (inset.rosebinning == "B") step_angle =  5.0;
	if (inset.rosebinning == "C") step_angle = 10.0;
	if (inset.rosebinning == "D") step_angle = 22.5;

	int segmentnumber = static_cast<int>(90.0 / step_angle);

	int i = 0;
	size_t j = 0;

	do {

		datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIP");

		if (datanumber.LIN_NUM > mx.LIN_NUM) mx.LIN_NUM = datanumber.LIN_NUM;
		if (datanumber.PLN_NUM > mx.PLN_NUM) mx.PLN_NUM = datanumber.PLN_NUM;

		begin_angle = begin_angle + step_angle;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	do {

		datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIP");

		percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
		percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

		PS_draw_rose_DATATYPE(inGDB, o, inset, center, percent, begin_angle, true);

		/*
		if ((inGDB.at(j).DATAGROUP == "LINEATION"))

			//&& (datanumber.LIN_NUM >= 0.9))

			PS_draw_rose_LINEATION (o, inset, center, percent, begin_angle, true);

		if ((inGDB.at(j).DATAGROUP == "PLANE") && (datanumber.PLN_NUM >= 0.9))

			PS_draw_rose_PLANE (o, inset, center, percent, begin_angle, true);

		if (((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) && ((datanumber.LIN_NUM >= 0.9) || (datanumber.PLN_NUM >= 0.9)))

			PS_draw_rose_PLANE (o, inset, center, percent, begin_angle, true);

		 */
		begin_angle = begin_angle + step_angle;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	percent.PLN_NUM = mx.PLN_NUM / inGDB.size();
	percent.LIN_NUM = mx.LIN_NUM / inGDB.size();

	if (inGDB.at(j).DATAGROUP == "LINEATION") {

		percent.PLN_NUM = percent.LIN_NUM;
	}

	PS_draw_rose_circle_vertical (o, inset, center, percent);

	if ((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) {

		do {
			datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIP");

			percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
			percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

			PS_draw_rose_DATATYPE(inGDB, o, inset, center, percent, begin_angle, true);
			//PS_draw_rose_SC_STRIAE (o, inset, center, percent, begin_angle, true);

			begin_angle = begin_angle + step_angle;
			i++;

		} while (i < segmentnumber);
	}
}

void PS_draw_rose (vector <GDB> roseGDB, ofstream& o, INPSET inset, CENTER center, PAPER P, bool tilt) {

	if (!tilt) {

		center.X = P.O3X;
		center.Y = P.O3Y;
		PS_draw_rose_DIPDIR (roseGDB, o, inset, center);

		center.X = P.O5X;
		center.Y = P.O5Y;
		PS_draw_rose_DIP (roseGDB, o, inset, center);
	}
	else {

		center.X = P.O4X;
		center.Y = P.O4Y;
		PS_draw_rose_DIPDIR (roseGDB, o, inset, center);

		center.X = P.O6X;
		center.Y = P.O6Y;
		PS_draw_rose_DIP (roseGDB, o, inset, center);
	}
}
