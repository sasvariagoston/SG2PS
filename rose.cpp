// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iomanip>
#include <iostream>

#include "assertions.hpp"
#include "allowed_keys.hpp"
#include "rgf.h"
#include "rose.h"
#include "run_mode.h"
#include "ps.h"
#include "settings.hpp"
#include "standard_output.hpp"

using namespace std;

ROSENUMBER compute_data_number_DIPDIR_DIP (const vector <GDB>& inGDB, const double strike_begin, const double strike_end, const string DIPDIR_DIP) {

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

void PS_draw_rose_DATATYPE (const vector <GDB>& inGBD, ofstream& o, const CENTER& center, const ROSENUMBER& P, const double begin_angle, const bool VERTICAL) {

	bool LINEATION = 	(is_allowed_lineation_datatype	(inGBD.at(0).DATAGROUP));
	bool PLANE = 		(is_allowed_plane_datatype		(inGBD.at(0).DATAGROUP));
	bool SC = 			(is_allowed_SC_datatype			(inGBD.at(0).DATAGROUP));
	bool STRIAE = 		(is_allowed_striae_datatype		(inGBD.at(0).DATAGROUP));

	if (!LINEATION && !PLANE && !SC && !STRIAE) ASSERT_DEAD_END();

	bool DRAW_STRIKE = is_ROSEDIRECTION_STRIKE() && !LINEATION;
	bool DRAW_DIP = 	is_ROSEDIRECTION_DIP() || LINEATION;

	if (!DRAW_STRIKE && !DRAW_DIP) ASSERT_DEAD_END();

	if (VERTICAL) {

		if (PLANE) {

			PS_rosesegment (o, center, P.PLN_NUM, 90.0 + begin_angle, false);
		}
		else if (LINEATION) {

			PS_rosesegment (o, center, P.LIN_NUM, 90.0 + begin_angle, false);
		}
		else if (SC || STRIAE) {

			PS_rosesegment (o, center, P.PLN_NUM, 90.0 + begin_angle, false);
			PS_rosesegment (o, center, P.LIN_NUM, 90.0 + begin_angle, true);
		}
		else ASSERT_DEAD_END();

		return;
	}
	else {}

	if (is_ROSETYPE_ASYMMETRICAL()) {

		if (PLANE) {

			if (DRAW_DIP) 			PS_rosesegment (o, center, P.PLN_NUM, begin_angle, false);
			else if (DRAW_STRIKE)	PS_rosesegment (o, center, P.PLN_NUM, begin_angle - 90.0, false);
			else ASSERT_DEAD_END();
		}
		else if (LINEATION) {

			if (DRAW_DIP) 			PS_rosesegment (o, center, P.LIN_NUM, begin_angle, false);
			else if (DRAW_STRIKE)	PS_rosesegment (o, center, P.LIN_NUM, begin_angle - 90.0, false);
			else ASSERT_DEAD_END();
		}
		else if (SC || STRIAE) {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle, false);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle, true);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle - 90.0, false);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle - 00.0, true);
			}
			else ASSERT_DEAD_END();
		}
		else ASSERT_DEAD_END();
	}
	else if (is_ROSETYPE_SYMMETRICAL()) {

		if (PLANE) {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0, false);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0, false);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0 - 90.0, false);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0 - 90.0, false);
			}
			else ASSERT_DEAD_END();
		}
		else if (LINEATION) {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0, false);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0, false);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0 - 90.0, false);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0 - 90.0, false);
			}
			else ASSERT_DEAD_END();
		}
		else if (SC || STRIAE) {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0, false);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0, false);

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0, true);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0, true);
			}
			else if (DRAW_STRIKE) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0 - 90.0, false);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0 - 90.0, false);

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0 - 00.0, true);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0 - 00.0, true);
			}
			else ASSERT_DEAD_END();
		}
		else ASSERT_DEAD_END();
	}
	else ASSERT_DEAD_END();
}

void PS_draw_rose_DIPDIR_DIP (vector <GDB> inGDB, ofstream& o, CENTER center, const string MODE) {

	vector <ROSENUMBER> N;
	ROSENUMBER MX;

	MX.LIN_NUM = 0.0;
	MX.PLN_NUM = 0.0;

	size_t 						S =  25;
	if (is_ROSEBINSIZE_5_00()) 	S =  50;
	if (is_ROSEBINSIZE_10_00()) S = 100;
	if (is_ROSEBINSIZE_22_50()) S = 225;

	const bool DD = MODE == "DIPDIR";
	const bool D = MODE == "DIP";

	size_t MAX_ANG = NaN();
	if (is_ROSETYPE_ASYMMETRICAL()) {

		if (D) 			MAX_ANG = 900;
		else if (DD)	MAX_ANG = 3600;
		else ASSERT_DEAD_END();
	}
	else {
		if (D) 			MAX_ANG = 900;
		else if (DD)	MAX_ANG = 1800;
		else ASSERT_DEAD_END();
	}

	for (size_t i = 0; (i * S) < MAX_ANG; i++) {

		double MIN = (i*S) / 10.0;
		double MAX = ((i+1) * S) / 10.0;

		if (is_ROSETYPE_ASYMMETRICAL()) {

			N.push_back (compute_data_number_DIPDIR_DIP (inGDB, MIN, MAX, MODE));
		}
		else if (is_ROSETYPE_SYMMETRICAL()){

			const ROSENUMBER T1 = compute_data_number_DIPDIR_DIP (inGDB, MIN, MAX, MODE);

			MIN = ((i*S) + 1800) / 10.0;
			MAX = (((i+1)*S) + 1800) / 10.0;

			const ROSENUMBER T2 = compute_data_number_DIPDIR_DIP (inGDB, MIN, MAX, MODE);

			ROSENUMBER TEMP;

			TEMP.LIN_NUM = T1.LIN_NUM + T2.LIN_NUM;
			TEMP.PLN_NUM = T1.PLN_NUM + T2.PLN_NUM;

			N.push_back(TEMP);
		}
		else ASSERT_DEAD_END();

		if (N.at(i).LIN_NUM > MX.LIN_NUM) MX.LIN_NUM = N.at(i).LIN_NUM;
		if (N.at(i).PLN_NUM > MX.PLN_NUM) MX.PLN_NUM = N.at(i).PLN_NUM;
	}

	if (is_mode_DEBUG() && is_CHK_ROSE()) {

		const vector <GDB> sGDB = sort_by_iID (inGDB);

		if (sGDB.at(0).ID == "ANG___001")	check_standard_ROSE (N, generate_standard_ROSE ());
		else {

			cout << "Test record 'ANG___001' not found - incorrect test file." << endl;
			ASSERT_DEAD_END();
		}
	}

	for (size_t i = 0; i < N.size(); i++) {

		N.at(i).LIN_NUM = N.at(i).LIN_NUM / MX.LIN_NUM;
		N.at(i).PLN_NUM = N.at(i).PLN_NUM / MX.PLN_NUM;

		if (DD) 	PS_draw_rose_DATATYPE (inGDB, o, center, N.at(i), (i*S) / 10.0, false);
		else if (D)	PS_draw_rose_DATATYPE (inGDB, o, center, N.at(i), (i*S) / 10.0, true);
		else ASSERT_DEAD_END();
	}

	if (DD) PS_draw_rose_circle (o, center, MX.PLN_NUM / inGDB.size(), false);
	else 	PS_draw_rose_circle (o, center, MX.PLN_NUM / inGDB.size(), true);

	return;
}
