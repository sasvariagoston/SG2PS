// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <string>
#include <vector>
#include <iomanip>

#include "rose.h"
#include "ps.h"

using namespace std;

ROSENUMBER compute_data_number_DIPDIR (vector <GDB> inGDB, double strike_begin, double strike_end) {

	ROSENUMBER counter;

	counter.LIN_NUM = 0.0;
	counter.PLN_NUM = 0.0;

	size_t i = 0;

	if (inGDB.at(0).DATAGROUP == "LINEATION") {

		do {

			if ((inGDB.at(i).corr.DIPDIR >= strike_begin) && (inGDB.at(i).corr.DIPDIR < strike_end)) counter.LIN_NUM++;

			i++;

		} while (i < inGDB.size());
	}

	if (inGDB.at(0).DATAGROUP == "PLANE") {

		do {

			if ((inGDB.at(i).corr.DIPDIR >= strike_begin) && (inGDB.at(i).corr.DIPDIR < strike_end)) counter.PLN_NUM++;

			i++;

		} while (i < inGDB.size());
	}

	if ((inGDB.at(0).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) {

		do {

			if ((inGDB.at(i).corr.DIPDIR >= strike_begin) && (inGDB.at(i).corr.DIPDIR < strike_end)) counter.PLN_NUM++;
			if ((inGDB.at(i).corrL.DIPDIR >= strike_begin) && (inGDB.at(i).corrL.DIPDIR   < strike_end)) counter.LIN_NUM++;

			i++;

		} while (i < inGDB.size());
	}

	return counter;
}

ROSENUMBER compute_data_number_DIP (vector <GDB> inGDB, double strike_begin, double strike_end) {

	ROSENUMBER counter;

	counter.LIN_NUM = 0.0;
	counter.PLN_NUM = 0.0;

	size_t i = 0;

	if (inGDB.at(0).DATAGROUP == "LINEATION") {

		do {

			if ((inGDB.at(i).corr.DIP >= strike_begin) && (inGDB.at(i).corr.DIP < strike_end)) counter.LIN_NUM++;

			i++;

		} while (i < inGDB.size());
	}

	if (inGDB.at(0).DATAGROUP == "PLANE") {

		do {

			if ((inGDB.at(i).corr.DIP >= strike_begin) && (inGDB.at(i).corr.DIP < strike_end)) counter.PLN_NUM++;

			i++;

		} while (i < inGDB.size());
	}

	if ((inGDB.at(0).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) {

		do {

			if ((inGDB.at(i).corr.DIP >= strike_begin) && (inGDB.at(i).corr.DIP < strike_end)) counter.PLN_NUM++;
			if ((inGDB.at(i).corrL.DIP >= strike_begin) && (inGDB.at(i).corrL.DIP < strike_end)) counter.LIN_NUM++;

			i++;

		} while (i < inGDB.size());
	}

	return counter;
}

void PS_draw_rose_LINEATION (GDB inGDB, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical) {

	if (vertical) {

		PS_rosesegment (o, inset, center, percent.LIN_NUM, 90 + begin_angle, false);

		return;
	}

	if (inset.rosetype == "A") PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle, false);

	else {

		PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle,       false);
		PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 180, false);
	}
}

void PS_draw_rose_PLANE (GDB inGDB, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical) {

	if (vertical) {

		PS_rosesegment (o, inset, center, percent.PLN_NUM, 90 + begin_angle, false);
		return;
	}

	if (inset.rosetype == "A") PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle, false);

	else {

		PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle,       false);
		PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 180, false);
	}
}

void PS_draw_rose_SC_STRIAE (GDB inGDB, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical) {

	if (vertical) {

		PS_rosesegment (o, inset, center, percent.LIN_NUM, 90 + begin_angle, true);
		return;
	}

	if (inset.rosetype == "A") {

		PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle, true);
	}

	else {
		PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle,       true);
		PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 180, true);
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

	double begin_angle = 0.0;
	double step_angle = 2.5;

	if (inset.rosebinning == "B") step_angle =  5.0;
	if (inset.rosebinning == "C") step_angle = 10.0;
	if (inset.rosebinning == "D") step_angle = 22.5;

	int segmentnumber = 360.0 / step_angle;

	int i = 0;;
	size_t j = 0;

	do {

		if (inset.rosetype == "A") datanumber = compute_data_number_DIPDIR (inGDB, begin_angle, begin_angle + step_angle);
		else {

			datanumber.LIN_NUM =
					compute_data_number_DIPDIR (inGDB, begin_angle,         begin_angle + step_angle        ).LIN_NUM +
					compute_data_number_DIPDIR (inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0).LIN_NUM;

			datanumber.PLN_NUM =
					compute_data_number_DIPDIR(inGDB, begin_angle,         begin_angle + step_angle        ).PLN_NUM +
					compute_data_number_DIPDIR (inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0).PLN_NUM;
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

		if (inset.rosetype == "A") datanumber = compute_data_number_DIPDIR (inGDB, begin_angle, begin_angle + step_angle);
		else {

			datanumber.LIN_NUM =
					compute_data_number_DIPDIR(inGDB, begin_angle, begin_angle + step_angle).LIN_NUM +
					compute_data_number_DIPDIR(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0).LIN_NUM;

			datanumber.PLN_NUM =
					compute_data_number_DIPDIR(inGDB, begin_angle, begin_angle + step_angle).PLN_NUM +
					compute_data_number_DIPDIR(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0).PLN_NUM;
		}

		percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
		percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

		if ((inGDB.at(j).DATAGROUP == "LINEATION") && (datanumber.LIN_NUM >= 0.9))

			PS_draw_rose_LINEATION (inGDB.at(j), o, inset, center, percent, begin_angle, false);


		if ((inGDB.at(j).DATAGROUP == "PLANE") && (datanumber.PLN_NUM >= 0.9))

			PS_draw_rose_PLANE (inGDB.at(j), o, inset, center, percent, begin_angle, false);


		if (((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) && ((datanumber.LIN_NUM >= 0.9) || (datanumber.PLN_NUM >= 0.9)))

			PS_draw_rose_PLANE (inGDB.at(j), o, inset, center, percent, begin_angle, false);


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

	PS_draw_rose_circle_horizontal (o, center, percent);


	if ((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) {

		do {

			if (inset.rosetype == "A") datanumber = compute_data_number_DIPDIR (inGDB, begin_angle, begin_angle + step_angle);
			else {

				datanumber.LIN_NUM =
						compute_data_number_DIPDIR(inGDB, begin_angle, begin_angle + step_angle).LIN_NUM +
						compute_data_number_DIPDIR(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0).LIN_NUM;

				datanumber.PLN_NUM =
						compute_data_number_DIPDIR(inGDB, begin_angle, begin_angle + step_angle).PLN_NUM +
						compute_data_number_DIPDIR(inGDB, begin_angle + 180.0, begin_angle + step_angle + 180.0).PLN_NUM;
			}

			percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
			percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

			PS_draw_rose_SC_STRIAE (inGDB.at(j), o, inset, center, percent, begin_angle, false);

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

	int segmentnumber = 90.0 / step_angle;

	int i = 0;
	size_t j = 0;

	do {

		datanumber = compute_data_number_DIP (inGDB, begin_angle, begin_angle + step_angle);

		if (datanumber.LIN_NUM > mx.LIN_NUM) mx.LIN_NUM = datanumber.LIN_NUM;
		if (datanumber.PLN_NUM > mx.PLN_NUM) mx.PLN_NUM = datanumber.PLN_NUM;

		begin_angle = begin_angle + step_angle;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	do {

		datanumber = compute_data_number_DIP (inGDB, begin_angle, begin_angle + step_angle);

		percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
		percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;


		if ((inGDB.at(j).DATAGROUP == "LINEATION") && (datanumber.LIN_NUM >= 0.9))

			PS_draw_rose_LINEATION (inGDB.at(j), o, inset, center, percent, begin_angle, true);

		if ((inGDB.at(j).DATAGROUP == "PLANE") && (datanumber.PLN_NUM >= 0.9))

			PS_draw_rose_PLANE (inGDB.at(j), o, inset, center, percent, begin_angle, true);

		if (((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) && ((datanumber.LIN_NUM >= 0.9) || (datanumber.PLN_NUM >= 0.9)))

			PS_draw_rose_PLANE (inGDB.at(j), o, inset, center, percent, begin_angle, true);

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

	PS_draw_rose_circle_vertical (o, center, percent);

	if ((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) {

		do {
			datanumber = compute_data_number_DIP (inGDB, begin_angle, begin_angle + step_angle);

			percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
			percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

			PS_draw_rose_SC_STRIAE (inGDB.at(j), o, inset, center, percent, begin_angle, true);

			begin_angle = begin_angle + step_angle;
			i++;

		} while (i < segmentnumber);
	}
}

void PS_draw_rose (vector <GDB> roseGDB, vector <GDB> tiltroseGDB, ofstream& o, INPSET inset, CENTER center, PAPER P) {

	center.X = P.O3X;
	center.Y = P.O3Y;
	PS_draw_rose_DIPDIR (roseGDB, o, inset, center);

	center.X = P.O4X;
	center.Y = P.O4Y;
	PS_draw_rose_DIPDIR (tiltroseGDB, o, inset, center);

	center.X = P.O5X;
	center.Y = P.O5Y;
	PS_draw_rose_DIP (roseGDB, o, inset, center);

	center.X = P.O6X;
	center.Y = P.O6Y;
	PS_draw_rose_DIP (tiltroseGDB, o, inset, center);
}
