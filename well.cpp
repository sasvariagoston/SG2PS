// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "average.hpp"
#include "bingham.h"
#include "common.h"
#include "data_sort.hpp"
#include "filename.hpp"
#include "rgf.h"
#include "settings.hpp"
#include "standard_output.hpp"
#include "structs.h"
#include "well.hpp"

namespace {

vector <vector <WELL_INTERVAL> > W_INTERVAL;
vector <vector <WELL_FREQUENCY> > W_FREQUENCY;

const double MIN_DATAINTERVAL = 0.10;

const double MINIMUM_DEPTH_LIMIT = 1.0e-1;

bool WELL_PROCESSING_CALLS = false;

}

using namespace std;

bool is_WELL_PROCESSING_CALLS () {

	return WELL_PROCESSING_CALLS;
}

vector <vector <WELL_INTERVAL> > RETURN_INTERVAL () {

	return W_INTERVAL;
}

vector <vector <WELL_FREQUENCY> > RETURN_FREQUENCY () {

	return W_FREQUENCY;
}

vector <GDB> filter_too_small_distances (const vector <GDB>& IN) {

	vector <GDB> OUT;

	for (size_t i = 0; i < IN.size() - 1; i++) {

		const GDB ACT = IN.at(i);
		const GDB NXT = IN.at(i + 1);

		const bool LAST = i == IN.size() - 2;

		const bool CORRECT = (NXT.DEPTH - ACT.DEPTH) > MIN_DATAINTERVAL;

		if (CORRECT) {

			OUT.push_back (ACT);

			if (LAST) OUT.push_back (NXT);
		}
	}
	return OUT;
}

vector <WELL_FREQUENCY> FREQUENCY (const vector <GDB>& inGDB) {

	vector <WELL_FREQUENCY> OUT;

	vector <GDB> process_GDB = SORT_GDB (inGDB, "ID");
	process_GDB = SORT_GDB (process_GDB, "DEPTH");

	process_GDB = filter_too_small_distances (process_GDB);

	if (process_GDB.empty()) return OUT;

	process_GDB = SORT_GDB (process_GDB, "DEPTH");

	double MAX_FREQ = 0.0;

	vector <double> DIP_v, INT_v, FREQ_v, FREQ_v2, MAX_FREQ_v;
	vector <string> ID_v;

	for (size_t i = 0; i < process_GDB.size() - 1; i++) {

		WELL_FREQUENCY buf;

		const GDB ACT = process_GDB.at(i);
		const GDB NXT = process_GDB.at(i + 1);

		const double DIP = ACT.corr.DIP;

		const double INT = NXT.DEPTH - ACT.DEPTH;

		ASSERT_GE (INT, 0.0);

		buf.DEPTH = (NXT.DEPTH + ACT.DEPTH) / 2.0;

		if (DIP > 5.0) 	buf.FREQ = 1.0 / ((TAN (DIP)) * INT);
		else 			buf.FREQ = 1.0 / INT;

		if (buf.FREQ > MAX_FREQ) MAX_FREQ = buf.FREQ;

		OUT.push_back (buf);

		INT_v.push_back (INT);
		DIP_v.push_back (DIP);
		FREQ_v.push_back (buf.FREQ);
		MAX_FREQ_v.push_back(MAX_FREQ);
		ID_v.push_back (process_GDB.at(i).ID);
	}

	if (OUT.empty()) return OUT;

	for (size_t i = 0; i < OUT.size(); i++) {

		OUT.at(i).FREQ = OUT.at(i).FREQ / MAX_FREQ;
		FREQ_v2.push_back (OUT.at(i).FREQ);
	}

	if (OUT.empty()) return OUT;

	double MAX_DERIV = 0.0;

	for (size_t i = 0; i < OUT.size() - 1; i++) {
	    ASSERT_NE(OUT.size(), 0); // if out is empty, OUT.size()-1 == 18446744073709551615, not -1
		const WELL_FREQUENCY ACT = OUT.at(i);
		const WELL_FREQUENCY NXT = OUT.at(i + 1);

		double DIF = fabs (NXT.FREQ - ACT.FREQ);

		OUT.at(i).DERIV = fabs (DIF / (NXT.DEPTH - ACT.DEPTH));
		OUT.at(i).DERIV_DEPTH = (NXT.DEPTH + ACT.DEPTH) / 2.0;

		if (OUT.at(i).DERIV > MAX_DERIV) MAX_DERIV = OUT.at(i).DERIV;
	}
	for (size_t i = 0; i < OUT.size() - 1; i++) {

		OUT.at(i).DERIV = OUT.at(i).DERIV / MAX_DERIV;

		ASSERT_FINITE (OUT.at(i).DERIV);
	}
	OUT.erase (OUT.end() - 1, OUT.end());

	return OUT;
}

double record_derivate (const bool DIPDIR, const WELL_INTERVAL& ACT, const WELL_INTERVAL& NXT) {

	double OUT;

	if (DIPDIR) {

		double DIF = fabs (NXT.INT_AV_DD.DIPDIR - ACT.INT_AV_DD.DIPDIR);

		if (DIF > 180.0) DIF = 360.0 - DIF;

		OUT = fabs (DIF / (NXT.DEPTH - ACT.DEPTH));
	}
	else OUT = fabs ((NXT.INT_AV_DD.DIP - ACT.INT_AV_DD.DIP) / (NXT.DEPTH - ACT.DEPTH));

	ASSERT_FINITE (OUT);

	return OUT;
}

vector <WELL_INTERVAL> FIRST_DERIVATE (const vector <WELL_INTERVAL>& IN) {

	vector <WELL_INTERVAL> OUT;

	double MAX_DERIVATE = 0.0;

	for (size_t i = 0; i < IN.size() - 1; i++) {

		const WELL_INTERVAL ACT = IN.at(i);
		const WELL_INTERVAL NXT = IN.at(i + 1);

		const bool PROCESSABLE = fabs (ACT.DEPTH - NXT.DEPTH) > MINIMUM_DEPTH_LIMIT;

		if (PROCESSABLE) {

			WELL_INTERVAL buf = IN.at(i);

			const double DD_DRV = record_derivate (true, ACT, NXT);
			const double D_DRV = record_derivate (false, ACT, NXT);

			if (DD_DRV > MAX_DERIVATE) MAX_DERIVATE = DD_DRV;
			if (D_DRV > MAX_DERIVATE) MAX_DERIVATE = D_DRV;

			ASSERT_FINITE (MAX_DERIVATE);
			ASSERT (MAX_DERIVATE > 0);

			buf.DD_DERIV = DD_DRV;
			buf.D_DERIV = D_DRV;

			ASSERT_FINITE (buf.DD_DERIV, buf.D_DERIV);

			OUT.push_back (buf);
		}
	}

	for (size_t i = 0; i < OUT.size(); i++) {

		OUT.at(i).DD_DERIV = OUT.at(i).DD_DERIV / MAX_DERIVATE;
		OUT.at(i).D_DERIV  = OUT.at(i).D_DERIV  / MAX_DERIVATE;

		ASSERT_FINITE (OUT.at(i).DD_DERIV, OUT.at(i).D_DERIV);
	}
	ASSERT (OUT.size() <= IN.size());

	return OUT;
}

double calculate_interval_depth (const vector <GDB>& inGDB) {

	const bool A = is_WELL_INTERVAL_MIDDLE_AVERAGE ();
	const bool M = is_WELL_INTERVAL_MIDDLE_MEDIAN ();

	ASSERT (A || M);

	vector <GDB> process_GDB = SORT_GDB (inGDB, "DEPTH");

	vector <double> D;

	for (size_t i = 0; i < process_GDB.size(); i++) D.push_back (process_GDB.at(i).DEPTH);

	if (A) return average(D);
	else   return median(D);
}

vector <GDB> return_GDB_for_data_interval (const vector <GDB>& inGDB, const double MIN, const double MAX) {

	vector <GDB> OUT;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const double D = inGDB.at(i).DEPTH;

		if (is_in_range (MIN, MAX, D)) 	OUT.push_back(inGDB.at(i));
	}
	return OUT;
}

double stdev_for_interval (const vector <GDB>& inGDB, const bool DIPDIR, const size_t RUN) {

	ASSERT (inGDB.size() != 0);
	if (inGDB.size() == 1) return 0.0;

	const DIPDIR_DIP avDD = inGDB.at(0).avd;

	vector <double> MSFT;

	vector <VCTR> T1_v, T2_v;
	vector <DIPDIR_DIP> D1_v, D2_v;
	vector <double> ANG_v, MSFT_v;
	vector <string> ID_v;

	const vector <GDB> p_GDB = SORT_GDB (inGDB, "ID");

	for (size_t i = 0; i < p_GDB.size(); i++) {

		const DIPDIR_DIP DD = p_GDB.at(i).corr;

		DIPDIR_DIP D1;
		DIPDIR_DIP D2;

		if (DIPDIR) {

			D1.DIPDIR = avDD.DIPDIR;
			D1.DIP = 0.0;
			D2.DIPDIR = DD.DIPDIR;
			D2.DIP = 0.0;
		}
		else {

			D1.DIPDIR = avDD.DIPDIR;
			D1.DIP = avDD.DIP;
			D2.DIPDIR = avDD.DIPDIR;
			D2.DIP = DD.DIP;
		}
		const VCTR T1 = unitvector (DXDYDZ_from_dipdir_dip (D1), true);
		const VCTR T2 = unitvector (DXDYDZ_from_dipdir_dip (D2), true);

		double ANG = vector_angle (T1, T2);

		D1_v.push_back (D1);
		D2_v.push_back (D2);
		T1_v.push_back (T1);
		T2_v.push_back (T2);
		ID_v.push_back (p_GDB.at(i).ID);

		ANG_v.push_back (ANG);
		MSFT_v.push_back (ANG * ANG);

		MSFT.push_back (ANG * ANG);
	}
	return sqrt (average (MSFT));
}

WELL_INTERVAL interval_average (const vector <GDB>& inGDB, const size_t RUN) {

	WELL_INTERVAL OUT;

	ASSERT (!inGDB.empty());

	OUT.DEPTH = calculate_interval_depth (inGDB);//ok

	WELL_PROCESSING_CALLS = true;

	const vector <GDB> dummy = DATATYPE_AVERAGE (inGDB);

	WELL_PROCESSING_CALLS = false;

	const bool O = is_D_up (dummy.at(0).avD);

	const bool EQ1 = is_processable_for_average_EQ1 (inGDB);
	const bool EQ2 = is_processable_for_average_EQ2 (inGDB);

	VCTR AV_N;

	if (EQ1 || EQ2) AV_N = calculate_data_average_vector (inGDB);

	else {

		const vector <VCTR> BNG = generate_Bingham_dataset (inGDB);
		const STRESSTENSOR ST = st_BINGHAM (BNG);
		const STRESSFIELD SF = sf_BINGHAM (ST);

		AV_N = SF.EIGENVECTOR1;
	}
	if (!O) OUT.INT_AV_D = DXDYDZ_from_NXNYNZ (flip_vector (AV_N));
	else 	OUT.INT_AV_D = DXDYDZ_from_NXNYNZ (AV_N);

	OUT.INT_AV_DD = dipdir_dip_from_DXDYDZ (OUT.INT_AV_D);

	OUT.INT_AV_DD_STDEV = stdev_for_interval (dummy, true, RUN);//should be inGDB!
	OUT.INT_AV_D_STDEV = stdev_for_interval (dummy, false, RUN);//should be inGDB!

	return OUT;
}

vector <WELL_INTERVAL> WELL_AVERAGE_M (const vector <GDB>& p_GDB) {

	vector <WELL_INTERVAL> OUT;

	ASSERT (!p_GDB.empty());

	const double IVL = is_WELL_INTERVAL_LENGTH();

	const size_t S = p_GDB.size();

	const double MIN_DEPTH = p_GDB.at(0).DEPTH;
	const double MAX_DEPTH = p_GDB.at(S - 1).DEPTH;

	ASSERT_GT (MAX_DEPTH, MIN_DEPTH);
	ASSERT_GT ((MAX_DEPTH - MIN_DEPTH), IVL);

	for (double i = MIN_DEPTH; i < MAX_DEPTH - IVL + 1; i++) {

		const double MIN = i;
		const double MAX = i + IVL;

		vector <GDB> temp = return_GDB_for_data_interval (p_GDB, MIN, MAX);

		const bool PROCESSABLE = temp.size() != 0;

		WELL_INTERVAL buf;

		size_t RUN = string_to_size_t (double_to_string(i, 0));

		if (PROCESSABLE) buf = interval_average (temp, RUN);

		buf.SIZE = temp.size();
		buf.MIN = MIN;
		buf.MAX = MAX;

		if (buf.SIZE > 0) OUT.push_back (buf);
	}
	return OUT;
}

vector <WELL_INTERVAL> WELL_AVERAGE_D (const vector <GDB>& p_GDB) {

	vector <WELL_INTERVAL> OUT;

	ASSERT (!p_GDB.empty());

	const double IVL = is_WELL_INTERVAL_LENGTH();
	const size_t S = p_GDB.size();

	size_t I = string_to_size_t (double_to_string (IVL, 0));

	ASSERT_GE (S, I);

	for (size_t i = 0; i < S - I + 1; i++) {

		const size_t MIN = i;
		const size_t MAX = i + I - 1;

		vector <GDB> temp;

		temp.insert (temp.end(), p_GDB.begin() + MIN, p_GDB.begin() + MAX + 1);

		WELL_INTERVAL wbuf = interval_average (temp, i);

		wbuf.SIZE = temp.size();
		wbuf.MIN = temp.at(0).DEPTH;
		wbuf.MAX = temp.at(temp.size() - 1).DEPTH;

		ASSERT_NE (temp.size(), 0);

		OUT.push_back (wbuf);
	}
	return OUT;
}

void PROCESS_WELL_GROUPS (const vector <vector <GDB> >& inGDB_G) {

	if (is_WELLDATA_NO()) return;

	W_INTERVAL.clear();
	W_FREQUENCY.clear();

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		ASSERT (!inGDB_G.at(i).empty());

		const string DT = inGDB_G.at(i).at(0).DATATYPE;

		const bool AS_WELL = is_allowed_to_process_as_well (DT);

		setup_ACTUAL_LOCATION (inGDB_G.at(i).at(0).LOC);
		setup_ACTUAL_DATATYPE (DT);
		setup_ACTUAL_FORMATION(inGDB_G.at(i).at(0).FORMATION);
		setup_ACTUAL_GROUPCODE(inGDB_G.at(i).at(0).GC);

		vector <WELL_INTERVAL> INTERVAL_buf;
		vector <WELL_FREQUENCY> FREQUENCY_buf;

		vector <GDB> p_GDB = inGDB_G.at(i);
		p_GDB = SORT_GDB (p_GDB, "DEPTH");

		const bool is_M = is_WELL_INTERVAL_METER();
		const bool is_D = is_WELL_INTERVAL_DATANUMBER();

		ASSERT (is_M || is_D);

		const size_t S = p_GDB.size();

		const double MIN_DEPTH = p_GDB.at(0).DEPTH;
		const double MAX_DEPTH = p_GDB.at(S - 1).DEPTH;

		const double IVL = is_WELL_INTERVAL_LENGTH();

		const size_t I = string_to_size_t (double_to_string (IVL, 0));

		bool PROCESSABLE = false;

		if (is_M) PROCESSABLE = (MAX_DEPTH - MIN_DEPTH) > IVL;
		else 	  PROCESSABLE = (S >= I);

		if (p_GDB.size() <= 2) PROCESSABLE = false;

		if (PROCESSABLE && AS_WELL) {

			if (is_M)	INTERVAL_buf = WELL_AVERAGE_M (p_GDB);//ok
			else		INTERVAL_buf = WELL_AVERAGE_D (p_GDB);

			INTERVAL_buf = FIRST_DERIVATE (INTERVAL_buf);

			FREQUENCY_buf = FREQUENCY (p_GDB);
		}
		W_INTERVAL.push_back (INTERVAL_buf);
		W_FREQUENCY.push_back (FREQUENCY_buf);
	}
	if (is_CHK_WELL()) STANDARD_OUTPUT_WELL_GROUPS ();

	return;
}

void cout_dbg_well_INTERVAL (const vector <WELL_INTERVAL> INT) {

	cout << " -------- START WELL INTERVAL -------- " << endl;

	cout << "INTERVAL with " << INT.size() << " records." << endl;

	cout
	<< "DEPTH" << '\t'
	<< "INT_AV_DD.DIPDIR" << '\t'
	<< "INT_AV_DD.DIP" << '\t'
	<< "SIZE" << '\t'
	<< "D_DERIV" << '\t'
	<< "DD_DERIV" << '\t'
	<< "INT_AV_D_STDEV" << '\t'
	<< "INT_AV_DD_STDEV" << '\t'
	<<endl;

	for (size_t i = 0; i < INT.size(); i++) {

		cout
		<< INT.at(i).DEPTH << '\t'
		<< INT.at(i).INT_AV_DD.DIPDIR << '\t'
		<< INT.at(i).INT_AV_DD.DIP << '\t'
		<< INT.at(i).SIZE << '\t'
		<< INT.at(i).D_DERIV << '\t'
		<< INT.at(i).DD_DERIV << '\t'
		<< INT.at(i).INT_AV_D_STDEV << '\t'
		<< INT.at(i).INT_AV_DD_STDEV << '\t'
		<<endl;
	}
	cout << " --------- END WELL INTERVAL --------- " << endl;
}

void cout_dbg_well_INTERVAL (const vector <vector <WELL_INTERVAL> > INT) {

	cout << " -------- START WELL INTERVAL -------- " << endl;

	for (size_t i = 0; i < INT.size(); i++) {

		cout << "SUB INTERVAL " << i << " with " << INT.at(i).size() << " records." << endl;

		cout
		<< "DEPTH" << '\t'
		<< "INT_AV_DD.DIPDIR" << '\t'
		<< "INT_AV_DD.DIP" << '\t'
		<< "SIZE" << '\t'
		<< "D_DERIV" << '\t'
		<< "DD_DERIV" << '\t'
		<< "INT_AV_D_STDEV" << '\t'
		<< "INT_AV_DD_STDEV" << '\t'
		<<endl;

		for (size_t j = 0; j < INT.at(i).size(); j++) {

			cout
			<< INT.at(i).at(j).DEPTH << '\t'
			<< INT.at(i).at(j).INT_AV_DD.DIPDIR << '\t'
			<< INT.at(i).at(j).INT_AV_DD.DIP << '\t'
			<< INT.at(i).at(j).SIZE << '\t'
			<< INT.at(i).at(j).D_DERIV << '\t'
			<< INT.at(i).at(j).DD_DERIV << '\t'
			<< INT.at(i).at(j).INT_AV_D_STDEV << '\t'
			<< INT.at(i).at(j).INT_AV_DD_STDEV << '\t'
			<<endl;
		}
	}
	cout << " --------- END WELL INTERVAL --------- " << endl;
}
