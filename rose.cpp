// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iomanip>
#include <iostream>

#include "assertions.hpp"
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

	if (!DIPDIR && !DIP) ASSERT_DEAD_END();

	bool LIN = (inGDB.at(0).DATAGROUP == "LINEATION");
	bool SC_STRIAE = (inGDB.at(0).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE");
	bool PLANE = (inGDB.at(0).DATAGROUP == "PLANE");

	if (!LIN && !PLANE && !SC_STRIAE) ASSERT_DEAD_END();

	for (size_t i = 0; i < inGDB.size(); i++) {

		double TO_CHECK = 0.0;
		bool IN_RANGE = false;

		if (DIPDIR) {

			if (LIN || PLANE) {

				TO_CHECK = inGDB.at(i).corr.DIPDIR;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE && PLANE) counter.PLN_NUM++;
				if (IN_RANGE && LIN) counter.LIN_NUM++;
			}
			else if (SC_STRIAE) {

				TO_CHECK = inGDB.at(i).corr.DIPDIR;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE) counter.PLN_NUM++;
				TO_CHECK = inGDB.at(i).corrL.DIPDIR;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE) counter.LIN_NUM++;
			}
			else ASSERT_DEAD_END();
		}
		else if (DIP) {

			if (LIN || PLANE) {

				TO_CHECK = inGDB.at(i).corr.DIP;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE && PLANE) counter.PLN_NUM++;
				if (IN_RANGE && LIN) counter.LIN_NUM++;;
			}
			if (SC_STRIAE) {

				TO_CHECK = inGDB.at(i).corr.DIP;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE) counter.PLN_NUM++;
				TO_CHECK = inGDB.at(i).corrL.DIP;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE) counter.LIN_NUM++;
			}
		}
		else ASSERT_DEAD_END()
	}

	return counter;
}

void PS_draw_rose_DATATYPE (vector <GDB> inGBD, ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent, double begin_angle, bool vertical) {

	bool LINEATION = 	(is_allowed_lineation_datatype	(inGBD.at(0).DATAGROUP));
	bool PLANE = 		(is_allowed_plane_datatype		(inGBD.at(0).DATAGROUP));
	bool SC = 			(is_allowed_SC_datatype			(inGBD.at(0).DATAGROUP));
	bool STRIAE = 		(is_allowed_striae_datatype		(inGBD.at(0).DATAGROUP));

	if (!LINEATION && !PLANE && !SC && !STRIAE) ASSERT_DEAD_END();

	bool ASYMM = (inset.rosetype == "A");
	bool SYMM  = (inset.rosetype == "S");

	if (!SYMM && !ASYMM) ASSERT_DEAD_END();

	bool DRAW_STRIKE = (inset.rosedirection == "S" && !LINEATION);
	bool DRAW_DIP = (inset.rosedirection == "D" || LINEATION);


	if (!DRAW_STRIKE && !DRAW_DIP) ASSERT_DEAD_END();



	if (vertical) {

		if (PLANE) {

			PS_rosesegment (o, inset, center, percent.PLN_NUM, 90.0 + begin_angle, false);
		}

		else if (LINEATION) {

			PS_rosesegment (o, inset, center, percent.LIN_NUM, 90.0 + begin_angle, false);
		}
		else if (SC || STRIAE) {

			PS_rosesegment (o, inset, center, percent.PLN_NUM, 90.0 + begin_angle, false);
			PS_rosesegment (o, inset, center, percent.LIN_NUM, 90.0 + begin_angle, true);
		}
		else ASSERT_DEAD_END();

		return;
	}
	else {}

	if (ASYMM) {

		if (PLANE) {

			if (DRAW_DIP) 			PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle, false);
			else if (DRAW_STRIKE)	PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle - 90.0, false);
			else ASSERT_DEAD_END();
		}
		else if (LINEATION) {

			if (DRAW_DIP) 			PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle, false);
			else if (DRAW_STRIKE)	PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle - 90.0, false);
			else ASSERT_DEAD_END();
		}
		else if (SC || STRIAE) {

			if (DRAW_DIP) {

				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle, false);
				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle, true);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle - 90.0, false);
				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle - 00.0, true);
			}
			else ASSERT_DEAD_END();
		}
		else ASSERT_DEAD_END();
	}
	else if (SYMM) {

		if (PLANE) {

			if (DRAW_DIP) {

				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 000.0, false);
				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 180.0, false);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 000.0 - 90.0, false);
				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 180.0 - 90.0, false);
			}
			else ASSERT_DEAD_END();
		}
		else if (LINEATION) {

			if (DRAW_DIP) {

				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 000.0, false);
				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 180.0, false);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 000.0 - 90.0, false);
				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 180.0 - 90.0, false);
			}
			else ASSERT_DEAD_END();
		}
		else if (SC || STRIAE) {

			if (DRAW_DIP) {

				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 000.0, false);
				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 180.0, false);

				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 000.0, true);
				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 180.0, true);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 000.0 - 90.0, false);
				PS_rosesegment (o, inset, center, percent.PLN_NUM, begin_angle + 180.0 - 90.0, false);

				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 000.0 - 00.0, true);
				PS_rosesegment (o, inset, center, percent.LIN_NUM, begin_angle + 180.0 - 00.0, true);
			}
			else ASSERT_DEAD_END();
		}
		else ASSERT_DEAD_END();
	}
	else ASSERT_DEAD_END();
}

void PS_draw_rose_DIPDIR (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, const bool is_debug) {

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

		if (is_debug) {

			cout << fixed << setprecision(0) << flush;
			cout << "ANGLE: " << begin_angle << flush;
			cout << " LIN: " << datanumber.LIN_NUM << flush;
			cout << " PLN: " << datanumber.PLN_NUM << endl;
		}

		begin_angle = begin_angle + step_angle;
		if (inset.rosetype == "S") i++;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	do {

		if (inset.rosetype == "A") {

			datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIPDIR");
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

		PS_draw_rose_DATATYPE(inGDB, o, inset, center, percent, begin_angle, false);

		begin_angle = begin_angle + step_angle;
		if (inset.rosetype == "S") i++;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	percent.PLN_NUM = mx.PLN_NUM / inGDB.size();
	percent.LIN_NUM = mx.LIN_NUM / inGDB.size();

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

			if (is_debug) {

				cout << fixed << setprecision(0) << flush;
				cout << "ANGLE: " << begin_angle << flush;
				cout << " LIN: " << datanumber.LIN_NUM << flush;
				cout << " PLN: " << datanumber.PLN_NUM << endl;
			}

			PS_draw_rose_DATATYPE(inGDB, o, inset, center, percent, begin_angle, false);

			begin_angle = begin_angle + step_angle;
			if (inset.rosetype == "S") i++;
			i++;

		} while (i < segmentnumber);
	}
}

void PS_draw_rose_DIP (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, const bool is_debug) {

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

		begin_angle = begin_angle + step_angle;
		i++;

	} while (i < segmentnumber);

	i = 0;
	begin_angle = 0.0;

	percent.PLN_NUM = mx.PLN_NUM / inGDB.size();
	percent.LIN_NUM = mx.LIN_NUM / inGDB.size();

	PS_draw_rose_circle_vertical (o, inset, center, percent);

	if ((inGDB.at(j).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE")) {

		do {
			datanumber = compute_data_number_DIPDIR_DIP (inGDB, begin_angle, begin_angle + step_angle, "DIP");

			percent.LIN_NUM = datanumber.LIN_NUM / mx.LIN_NUM;
			percent.PLN_NUM = datanumber.PLN_NUM / mx.PLN_NUM;

			PS_draw_rose_DATATYPE(inGDB, o, inset, center, percent, begin_angle, true);

			begin_angle = begin_angle + step_angle;
			i++;

		} while (i < segmentnumber);
	}
}

void PS_draw_rose (vector <GDB> roseGDB, ofstream& o, INPSET inset, CENTER center, PAPER P, bool tilt, const bool is_debug) {

	if (!tilt) {

		center.X = P.O3X;
		center.Y = P.O3Y;
		PS_draw_rose_DIPDIR (roseGDB, o, inset, center, is_debug);

		center.X = P.O5X;
		center.Y = P.O5Y;
		PS_draw_rose_DIP (roseGDB, o, inset, center, is_debug);
	}
	else {

		center.X = P.O4X;
		center.Y = P.O4Y;
		PS_draw_rose_DIPDIR (roseGDB, o, inset, center, is_debug);

		center.X = P.O6X;
		center.Y = P.O6Y;
		PS_draw_rose_DIP (roseGDB, o, inset, center, is_debug);
	}
}
