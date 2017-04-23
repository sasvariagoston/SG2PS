
// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iomanip>
#include <iostream>

#include "assertions.hpp"
#include "allowed_keys.hpp"
#include "data_sort.hpp"
#include "filename.hpp"
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

	ASSERT_EXACTLY_ONE_TRUE (DIPDIR, DIP);

	bool LIN = (inGDB.at(0).DATAGROUP == "LINEATION");
	bool SC_STRIAE = (inGDB.at(0).DATAGROUP == "SC") || (inGDB.at(0).DATAGROUP == "STRIAE");
	bool PLANE = (inGDB.at(0).DATAGROUP == "PLANE");

	ASSERT_EXACTLY_ONE_TRUE (LIN, PLANE, SC_STRIAE);

	for (size_t i = 0; i < inGDB.size(); i++) {

		double TO_CHECK = 0.0;
		bool IN_RANGE = false;

		if (DIPDIR) {

			ASSERT_EXACTLY_ONE_TRUE (LIN, PLANE, SC_STRIAE);

			if (LIN || PLANE) {

				TO_CHECK = inGDB.at(i).corr.DIPDIR;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE && PLANE) counter.PLN_NUM++;
				if (IN_RANGE && LIN) counter.LIN_NUM++;
			}
			else {

				TO_CHECK = inGDB.at(i).corr.DIPDIR;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE) counter.PLN_NUM++;
				TO_CHECK = inGDB.at(i).corrL.DIPDIR;
				IN_RANGE = (is_in_range(strike_begin, strike_end, TO_CHECK));
				if (IN_RANGE) counter.LIN_NUM++;
			}
		}
		else {

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
	}
	return counter;
}

void PS_draw_rose_DATATYPE (const vector <GDB>& inGBD, ofstream& o, const CENTER& center, const ROSENUMBER& P, const double begin_angle, const bool VERTICAL) {

	ASSERT_EXACTLY_ONE_TRUE (is_ROSETYPE_ASYMMETRICAL(), is_ROSETYPE_SYMMETRICAL());

	bool LINEATION = 	(is_allowed_lineation_datatype	(inGBD.at(0).DATAGROUP));
	bool PLANE = 		(is_allowed_plane_datatype		(inGBD.at(0).DATAGROUP));
	bool SC = 			(is_allowed_SC_datatype			(inGBD.at(0).DATAGROUP));
	bool STRIAE = 		(is_allowed_striae_datatype		(inGBD.at(0).DATAGROUP));
	ASSERT_EXACTLY_ONE_TRUE (LINEATION, PLANE, SC, STRIAE);

	bool DRAW_STRIKE = 	is_ROSEDIRECTION_STRIKE() && !LINEATION;
	bool DRAW_DIP = 	is_ROSEDIRECTION_DIP() || LINEATION;
	ASSERT_EXACTLY_ONE_TRUE (DRAW_STRIKE, DRAW_DIP);

	const string CLR = inGBD.at(0).PSCOLOR;

	if (VERTICAL) {

		if (PLANE) PS_rosesegment (o, center, P.PLN_NUM, 90.0 + begin_angle, false, CLR);

		else if (LINEATION) PS_rosesegment (o, center, P.LIN_NUM, 90.0 + begin_angle, false, CLR);

		else {

			PS_rosesegment (o, center, P.PLN_NUM, 90.0 + begin_angle, false, CLR);
			PS_rosesegment (o, center, P.LIN_NUM, 90.0 + begin_angle, true, CLR);
		}
		return;
	}

	if (is_ROSETYPE_ASYMMETRICAL()) {

		if (PLANE) {

			if (DRAW_DIP) 	PS_rosesegment (o, center, P.PLN_NUM, begin_angle, false, CLR);
			else 			PS_rosesegment (o, center, P.PLN_NUM, begin_angle - 90.0, false, CLR);
		}
		else if (LINEATION) {

			if (DRAW_DIP)	PS_rosesegment (o, center, P.LIN_NUM, begin_angle, false, CLR);
			else 			PS_rosesegment (o, center, P.LIN_NUM, begin_angle - 90.0, false, CLR);
		}
		else  {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle, false, CLR);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle, true, CLR);
			}
			else {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle - 90.0, false, CLR);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle - 00.0, true, CLR);
			}
		}
	}
	else {

		if (PLANE) {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0, false, CLR);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0, false, CLR);
			}
			else {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0 - 90.0, false, CLR);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0 - 90.0, false, CLR);
			}
		}
		else if (LINEATION) {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0, false, CLR);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0, false, CLR);
			}
			else {

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0 - 90.0, false, CLR);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0 - 90.0, false, CLR);
			}
		}
		else {

			if (DRAW_DIP) {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0, false, CLR);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0, false, CLR);

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0, true, CLR);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0, true, CLR);
			}
			else {

				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 000.0 - 90.0, false, CLR);
				PS_rosesegment (o, center, P.PLN_NUM, begin_angle + 180.0 - 90.0, false, CLR);

				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 000.0 - 00.0, true, CLR);
				PS_rosesegment (o, center, P.LIN_NUM, begin_angle + 180.0 - 00.0, true, CLR);
			}
		}
	}
}

bool process_as_lineation (const vector <GDB>& inGDB) {

	const bool LN = is_allowed_lineation_datatype(inGDB.at(0).DATATYPE);
	const bool SC = is_allowed_SC_datatype(inGDB.at(0).DATATYPE);
	const bool ST = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);

	return (LN || SC || ST);
}

bool process_as_plane (const vector <GDB>& inGDB) {

	const bool PL = is_allowed_plane_datatype(inGDB.at(0).DATATYPE);
	const bool SC = is_allowed_SC_datatype(inGDB.at(0).DATATYPE);
	const bool ST = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);

	return (PL || SC || ST);
}

size_t return_s () {

	size_t 						S =  25;
	if (is_ROSEBINSIZE_5_00()) 	S =  50;
	if (is_ROSEBINSIZE_10_00()) S = 100;
	if (is_ROSEBINSIZE_22_50()) S = 225;

	return S;
}

ROSE_DATA return_rose_data (const vector <GDB>& inGDB, const string MODE) {

	ROSE_DATA OUT;

	ASSERT2 (inGDB.size() > 0, "Empty GDB in 'PS_draw_rose_DIPDIR_DIP' function.");

	const bool PR_L = process_as_lineation (inGDB);
	const bool PR_P = process_as_plane (inGDB);

	ASSERT2 (PR_L || PR_P, "Neither plane nor lineation, nor SC nor striae data processed in 'PS_draw_rose_DIPDIR_DIP' function.");

	const bool LT = is_allowed_lithology_datatype(inGDB.at(0).DATATYPE);
	ASSERT2 (!LT, "Lithology data processed in 'PS_draw_rose_DIPDIR_DIP' function.");

	vector <ROSENUMBER> N;

	const size_t S = return_s ();

	const bool DD = MODE == "DIPDIR";
	const bool D = MODE == "DIP";

	ASSERT_EXACTLY_ONE_TRUE (D, DD);
	ASSERT_EXACTLY_ONE_TRUE (is_ROSETYPE_ASYMMETRICAL(), is_ROSETYPE_SYMMETRICAL());

	size_t MAX_ANG = 0;

	if (is_ROSETYPE_ASYMMETRICAL()) {

		if (D) 	MAX_ANG = 900;
		else 	MAX_ANG = 3600;
	}
	else {
		if (D) 	MAX_ANG = 900;
		else 	MAX_ANG = 1800;
	}

	ROSENUMBER MX;
	MX.LIN_NUM = 0.0;
	MX.PLN_NUM = 0.0;

	for (size_t i = 0; (i * S) < MAX_ANG; i++) {

		double MIN = (i*S) / 10.0;
		double MAX = ((i+1) * S) / 10.0;

		if (is_ROSETYPE_ASYMMETRICAL()) {

			N.push_back (compute_data_number_DIPDIR_DIP (inGDB, MIN, MAX, MODE));
		}
		else {

			const ROSENUMBER T1 = compute_data_number_DIPDIR_DIP (inGDB, MIN, MAX, MODE);

			MIN = ((i*S) + 1800) / 10.0;
			MAX = (((i+1)*S) + 1800) / 10.0;

			const ROSENUMBER T2 = compute_data_number_DIPDIR_DIP (inGDB, MIN, MAX, MODE);

			ROSENUMBER TEMP;

			if (PR_L) 	TEMP.LIN_NUM = T1.LIN_NUM + T2.LIN_NUM;
			if (PR_P)	TEMP.PLN_NUM = T1.PLN_NUM + T2.PLN_NUM;

			N.push_back(TEMP);
		}

		if (PR_L) ASSERT_FINITE (N.at(i).LIN_NUM, MX.LIN_NUM);
		if (PR_P) ASSERT_FINITE (N.at(i).PLN_NUM, MX.PLN_NUM);

		if (PR_L && N.at(i).LIN_NUM > MX.LIN_NUM) MX.LIN_NUM = N.at(i).LIN_NUM;
		if (PR_P && N.at(i).PLN_NUM > MX.PLN_NUM) MX.PLN_NUM = N.at(i).PLN_NUM;
	}
	ASSERT_FINITE (MX.LIN_NUM, MX.PLN_NUM);

	OUT.DATA = N;
	OUT.MAX = MX;
	OUT.inGDB = inGDB;

	return OUT;
}

void DRAW_ROSE (vector <GDB> inGDB, ofstream& o, CENTER center, const bool DIP, const vector <ROSENUMBER>& N) {

	vector <ROSENUMBER> to_plot = N;

	const size_t S = return_s ();

	ASSERT_GE (to_plot.size(), 1);

	for (size_t i = 0; i < to_plot.size(); i++) PS_draw_rose_DATATYPE (inGDB, o, center, to_plot.at(i), (i*S) / 10.0, DIP);

	return;
}

void PS_draw_rose_DIPDIR_DIP (const vector <vector <GDB> >& inGDB_G, ofstream& o, CENTER center, const string MODE, const bool TILT) {

	const bool PR_L = process_as_lineation (inGDB_G.at(0));
	const bool PR_P = process_as_plane (inGDB_G.at(0));

	const bool DD = MODE == "DIPDIR";
	const bool D = MODE == "DIP";

	ASSERT_EXACTLY_ONE_TRUE (D, DD);
	ASSERT_EXACTLY_ONE_TRUE (is_ROSETYPE_ASYMMETRICAL(), is_ROSETYPE_SYMMETRICAL());

	vector <ROSE_DATA> R;

	size_t data_number = 0;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		const ROSE_DATA r = return_rose_data (inGDB_G.at(i), MODE);

		if (is_mode_DEBUG() && is_CHK_ROSE()) standard_output_rose (r.DATA, TILT);

		R.push_back (r);

		data_number = data_number + inGDB_G.at(i).size();
	}
	ASSERT_GE (R.size(), 1);
	ASSERT_EQ (inGDB_G.size(), R.size());

	ROSENUMBER MX;

	MX.LIN_NUM = 0.0;
	MX.PLN_NUM = 0.0;

	for (size_t i = 0; i < R.size(); i++) {

		if (R.at(i).MAX.LIN_NUM > MX.LIN_NUM)  MX.LIN_NUM  = R.at(i).MAX.LIN_NUM;
		if (R.at(i).MAX.PLN_NUM > MX.PLN_NUM)  MX.PLN_NUM  = R.at(i).MAX.PLN_NUM;
	}

	if (MX.LIN_NUM < MX.PLN_NUM) stable_sort(R.begin(), R.end(), by_PNL_NUM);
	else stable_sort(R.begin(), R.end(), by_LIN_NUM);

	double MAX = MX.LIN_NUM;
	if (MX.LIN_NUM < MX.PLN_NUM) MAX = MX.PLN_NUM;

	ASSERT_FINITE (MAX);
	ASSERT_GE (MAX, 0);

	for (size_t i = 0; i < R.size(); i++) {
		for (size_t j = 0; j < R.at(i).DATA.size(); j++) {

			if (PR_L) {

				R.at(i).DATA.at(j).LIN_NUM = R.at(i).DATA.at(j).LIN_NUM / MAX;
				ASSERT_FINITE (R.at(i).DATA.at(j).LIN_NUM);
			}

			if (PR_P) {

				R.at(i).DATA.at(j).PLN_NUM = R.at(i).DATA.at(j).PLN_NUM / MAX;
				ASSERT_FINITE (R.at(i).DATA.at(j).PLN_NUM);
			}
		}
	}
	for (size_t i = 0; i < R.size(); i++) {

		DRAW_ROSE (R.at(i).inGDB, o, center, D, R.at(i).DATA);
	}
	PS_draw_rose_circle (o, center, MAX / data_number, D);
	return;
}
