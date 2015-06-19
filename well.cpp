// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>

#include "assertions.hpp"
#include "average.hpp"
#include "bingham.h"
#include "common.h"
#include "data_sort.hpp"
#include "rgf.h"
//#include "run_mode.h"
#include "settings.hpp"
#include "structs.h"
#include "well.hpp"


namespace {

vector <vector <WELL_INTERVAL> > W_INTERVAL;

vector <vector <WELL_FREQUENCY> > W_FREQUENCY;

const double MIN_DATAINTERVAL = 0.10;

bool WELL_PROCESSING_CALLS = false;

}

using namespace std;

bool is_WELL_PROCESSING_CALLS () {

	return WELL_PROCESSING_CALLS;
}

vector <vector <WELL_INTERVAL> > RETURN_INTERVAL (){

	return W_INTERVAL;
}

vector <vector <WELL_FREQUENCY> > RETURN_FREQUENCY (){

	return W_FREQUENCY;
}
/*
vector <vector <WELL_INTERVAL> > MERGE_INTERVAL (const vector < vector <vector <vector <WELL_INTERVAL> > > >& IN) {

	vector <vector <WELL_INTERVAL> > OUT;

	for (size_t i = 0; i < IN.size(); i++) {
		for (size_t j = 0; j < IN.at(i).size(); j++) {
			for (size_t k = 0; k < IN.at(i).at(j).size(); k++) {

				//cout << i << j << k << IN.at(i).at(j).at(k).size() << endl;

				//cout << IN.at(0).at(0).at(0).at(0).DEPTH << endl;

				OUT.push_back (IN.at(i).at(j).at(k));
			}
		}
	}
	return OUT;
}
*/

/*
vector <vector <WELL_FREQUENCY> > MERGE_FREQUENCY (const vector < vector <vector <vector <WELL_FREQUENCY> > > >& IN) {

	vector <vector <WELL_FREQUENCY> > OUT;

	for (size_t i = 0; i < IN.size(); i++) {
		for (size_t j = 0; j < IN.at(i).size(); j++) {
			for (size_t k = 0; k < IN.at(i).at(j).size(); k++) {

				OUT.push_back (IN.at(i).at(j).at(k));

				//cout << IN.at(0).at(0).at(0).at(0).DEPTH << endl;

				//cout << i << j << k << IN.at(i).at(j).at(k).size() << endl;


			}
		}
	}
	return OUT;
}
*/

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

	vector <GDB> process_GDB = SORT_GDB (inGDB, "DEPTH");
	//vector <GDB> process_GDB = sort_by_DEPTH (inGDB);

	//cout << process_GDB.size() << endl;

	process_GDB = filter_too_small_distances (process_GDB);

	GDB_size_check (process_GDB);

	//cout << process_GDB.size() << endl;

	double MAX_FREQ = 0.0;

	//cout << fixed << setprecision (6) << endl;

	for (size_t i = 0; i < process_GDB.size() - 1; i++) {

		WELL_FREQUENCY buf;

		const GDB ACT = process_GDB.at(i);
		const GDB NXT = process_GDB.at(i + 1);

		const double DIP = ACT.corr.DIP;

		const double INT = NXT.DEPTH - ACT.DEPTH;

		if (INT < 0.0) ASSERT_DEAD_END();

		buf.DEPTH = (NXT.DEPTH + ACT.DEPTH) / 2.0;

		if (DIP > 5.0) 	buf.FREQ = 1.0 / ((TAN (DIP)) * INT);
		else 			buf.FREQ = 1.0 / INT;

		//buf.FREQ = 1.0 / ((COS (90.0 - DIP)) * INT);

		if (buf.FREQ > MAX_FREQ) MAX_FREQ = buf.FREQ;

		//cout << buf.DEPTH << "  -  " << buf.FREQ << "   -    " << DIP << endl;

		OUT.push_back (buf);
	}

	for (size_t i = 0; i < OUT.size(); i++) {

		OUT.at(i).FREQ = OUT.at(i).FREQ / MAX_FREQ;

		//cout << OUT.at(i).DEPTH << "  -  " << OUT.at(i).FREQ << endl;
	}

	double MAX_DERIV = 0.0;

	for (size_t i = 0; i < OUT.size() - 1; i++) {

		const WELL_FREQUENCY ACT = OUT.at(i);
		const WELL_FREQUENCY NXT = OUT.at(i + 1);

		double DIF = fabs (NXT.FREQ - ACT.FREQ);

		OUT.at(i).DERIV = fabs (DIF / (NXT.DEPTH - ACT.DEPTH));
		OUT.at(i).DERIV_DEPTH = (NXT.DEPTH + ACT.DEPTH) / 2.0;

		if (OUT.at(i).DERIV > MAX_DERIV) MAX_DERIV = OUT.at(i).DERIV;
	}

	for (size_t i = 0; i < OUT.size() - 1; i++) {

		OUT.at(i).DERIV = OUT.at(i).DERIV / MAX_DERIV;

		//cout << OUT.at(i).DERIV_DEPTH << "  -  " << OUT.at(i).DERIV << endl;
	}

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

	return OUT;
}

vector <WELL_INTERVAL> FIRST_DERIVATE (const vector <WELL_INTERVAL>& IN) {

	vector <WELL_INTERVAL> OUT = IN;

	double MAX_DERIVATE = 0.0;

	for (size_t i = 0; i < IN.size() - 1; i++) {

		const WELL_INTERVAL ACT = IN.at(i);
		const WELL_INTERVAL NXT = IN.at(i + 1);

		const bool PROCESSABLE = fabs (ACT.DEPTH - NXT.DEPTH) > 10e-2;

		OUT.at(i).DD_DERIV = 0.0;
		OUT.at(i).D_DERIV = 0.0;

		if (PROCESSABLE) {

			const double DD_DRV = record_derivate (true, ACT, NXT);
			const double D_DRV = record_derivate (false, ACT, NXT);

			if (DD_DRV > MAX_DERIVATE) MAX_DERIVATE = DD_DRV;
			if (D_DRV > MAX_DERIVATE) MAX_DERIVATE = D_DRV;

			//cout << OUT.at(i).DEPTH << "  -  " << DD_DRV << "  -  " << D_DRV << "  -  " <<  MAX_DERIVATE << endl;

			OUT.at(i).DD_DERIV = DD_DRV;
			OUT.at(i).D_DERIV = D_DRV;
		}
	}


	cout << fixed << setprecision(6) << flush;

	for (size_t i = 0; i < OUT.size(); i++) {

		if (!isnan(OUT.at(i).DD_DERIV)) OUT.at(i).DD_DERIV = OUT.at(i).DD_DERIV / MAX_DERIVATE;

		if (!isnan(OUT.at(i).D_DERIV)) OUT.at(i).D_DERIV = OUT.at(i).D_DERIV / MAX_DERIVATE;

		//cout << OUT.at(i).DEPTH << "  -  " << OUT.at(i).DD_DERIV << "  -  " << OUT.at(i).D_DERIV << endl;
	}

	for (size_t i = 1; i < OUT.size() ; i++) {

		if (isnan(OUT.at(i).DD_DERIV)) OUT.at(i).DD_DERIV = OUT.at(i - 1).DD_DERIV;

		if (isnan(OUT.at(i).D_DERIV)) OUT.at(i).D_DERIV = OUT.at(i - 1).D_DERIV;

		//cout << OUT.at(i).DEPTH << "  -  " << OUT.at(i).DD_DERIV << "  -  " << OUT.at(i).D_DERIV << endl;
	}


	return OUT;
}

double calculate_interval_depth (const vector <GDB>& inGDB) {

	vector <GDB> process_GDB = SORT_GDB (inGDB, "DEPTH");
	//vector <GDB> process_GDB = sort_by_DEPTH (inGDB);

	vector <double> D;

	for (size_t i = 0; i < process_GDB.size(); i++) D.push_back (process_GDB.at(i).DEPTH);

	if 		(is_WELL_INTERVAL_MIDDLE_AVERAGE()) return average(D);
	else if (is_WELL_INTERVAL_MIDDLE_MEDIAN()) 	return median(D);
	else ASSERT_DEAD_END();

	return NaN();
}

vector <GDB> return_GDB_for_data_interval (const vector <GDB>& inGDB, const double MIN, const double MAX) {

	vector <GDB> OUT;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const double D = inGDB.at(i).DEPTH;

		if (is_in_range (MIN, MAX, D)) 	OUT.push_back(inGDB.at(i));
	}
	return OUT;
}

double stdev_for_interval (const vector <GDB>& inGDB, const bool DIPDIR) {

	//if (is_WELL_INTERVAL_DATANUMBER()) {

	//	size_t L = double_to_size_t(is_WELL_INTERVAL_LENGTH());

	//	if (inGDB.size() < L) ASSERT_DEAD_END();
	//}

	const DIPDIR_DIP avDD = inGDB.at(0).avd;

	vector <double> MSFT;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const DIPDIR_DIP DD = inGDB.at(i).corr;

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
		const VCTR T1 = unitvector (DXDYDZ_from_dipdir_dip (D1));
		const VCTR T2 = unitvector (DXDYDZ_from_dipdir_dip (D2));

		double ANG = fabs (ACOS (dotproduct (T1, T2, false)));

		MSFT.push_back (ANG * ANG);
	}
	return sqrt (average (MSFT));
}

WELL_INTERVAL interval_average (const vector <GDB>& inGDB) {

	WELL_INTERVAL OUT;

	GDB_size_check(inGDB);

	OUT.DEPTH = calculate_interval_depth (inGDB);//ok

	WELL_PROCESSING_CALLS = true;

	const vector <GDB> dummy = DATATYPE_AVERAGE (inGDB);

	WELL_PROCESSING_CALLS = false;

	const bool O = is_D_up (dummy.at(0).avD);

	const vector <VCTR> BNG = generate_Bingham_dataset(inGDB);
	const STRESSTENSOR ST = st_BINGHAM (BNG);
	const STRESSFIELD SF = sf_BINGHAM (ST);

	if (!O) OUT.INT_AV_D = 	DXDYDZ_from_NXNYNZ (flip_vector (SF.EIGENVECTOR1));
	else OUT.INT_AV_D = 	DXDYDZ_from_NXNYNZ (SF.EIGENVECTOR1);

	OUT.INT_AV_DD = dipdir_dip_from_DXDYDZ (OUT.INT_AV_D);

	//OUT.INT_AV_D = dummy.at(0).avD;
	//OUT.INT_AV_DD = dummy.at(0).avd;

	//cout << " ------------------------------------------------ " << endl;
	//cout << OUT.DEPTH << "  -  "  << OUT.INT_AV_DD.DIPDIR << "/" << OUT.INT_AV_DD.DIP << endl;
	//dbg_cout_GDB_vector(inGDB);

	OUT.INT_AV_DD_STDEV = stdev_for_interval (dummy, true);//ok
	OUT.INT_AV_D_STDEV = stdev_for_interval (dummy, false);//ok

	return OUT;
}

vector <WELL_INTERVAL> WELL_AVERAGE_M (const vector <GDB>& p_GDB) {

	vector <WELL_INTERVAL> OUT;

	GDB_size_check (p_GDB);

	const double IVL = is_WELL_INTERVAL_LENGTH();//ok

	const size_t S = p_GDB.size();

	const double MIN_DEPTH = p_GDB.at(0).DEPTH;//ok
	const double MAX_DEPTH = p_GDB.at(S - 1).DEPTH;//ok

	if (MAX_DEPTH <= MIN_DEPTH) ASSERT_DEAD_END();
	if ((MAX_DEPTH - MIN_DEPTH) <= IVL) ASSERT_DEAD_END();

	for (double i = MIN_DEPTH; i < MAX_DEPTH - IVL + 1; i++) {

		const double MIN = i;//ok
		const double MAX = i + IVL;//ok

		vector <GDB> temp = return_GDB_for_data_interval (p_GDB, MIN, MAX);//ok

		const bool PROCESSABLE = temp.size() != 0;

		WELL_INTERVAL buf;

		if (PROCESSABLE) buf = interval_average (temp);//ok
		else {

			buf.INT_AV_DD.DIPDIR = NaN();
			buf.INT_AV_DD.DIP = NaN();
			buf.INT_AV_DD_STDEV = NaN();
			buf.INT_AV_D_STDEV = NaN();
		}
		buf.SIZE = temp.size();
		buf.MIN = MIN;
		buf.MAX = MAX;

		OUT.push_back (buf);
	}
	return OUT;
}

vector <WELL_INTERVAL> WELL_AVERAGE_D (const vector <GDB>& p_GDB) {

	vector <WELL_INTERVAL> OUT;

	GDB_size_check(p_GDB);

	const double IVL = is_WELL_INTERVAL_LENGTH();
	const size_t S = p_GDB.size();

	size_t I = string_to_size_t (double_to_string (IVL, 0));

	if (S < I) ASSERT_DEAD_END();

	//cout << "S   :" << S << endl;
	//cout << "I   :" << I << endl;
	//cout << "IVL :" << IVL << endl;

	for (size_t i = 0; i < S - I + 1; i++) {

		//cout << "WELL_AVERAGE_D : " << i << endl;

		const size_t MIN = i;
		const size_t MAX = i + I - 1;

		vector <GDB> temp;

		temp.insert (temp.end(), p_GDB.begin() + MIN, p_GDB.begin() + MAX + 1);

		WELL_INTERVAL wbuf = interval_average (temp);

		wbuf.SIZE = temp.size();
		wbuf.MIN = temp.at(0).DEPTH;
		wbuf.MAX = temp.at(temp.size() - 1).DEPTH;

		if (temp.size() == 0) ASSERT_DEAD_END();

		OUT.push_back (wbuf);
	}
	return OUT;
}

void PROCESS_WELL_GROUPS (const vector <vector <GDB> >& inGDB_G, const bool TILT) {

	if (is_WELLDATA_NO()) return;


	for (size_t i = 0; i < inGDB_G.size(); i++) {

		GDB_size_check (inGDB_G.at(i));

		vector <WELL_INTERVAL> INTERVAL_buf;
		vector <WELL_FREQUENCY> FREQUENCY_buf;

		vector <GDB> p_GDB = inGDB_G.at(i);
		p_GDB = SORT_GDB (p_GDB, "DEPTH");

		const bool is_M = is_WELL_INTERVAL_METER();
		const bool is_D = is_WELL_INTERVAL_DATANUMBER();
		const size_t S = p_GDB.size();

		const double MIN_DEPTH = p_GDB.at(0).DEPTH;
		const double MAX_DEPTH = p_GDB.at(S - 1).DEPTH;

		const double IVL = is_WELL_INTERVAL_LENGTH();
		//const size_t I = double_to_size_t (IVL);

		const size_t I = string_to_double (size_t_to_string (IVL));

		bool PROCESSABLE = false;

		if 		(is_M) 	PROCESSABLE = (MAX_DEPTH - MIN_DEPTH) > IVL;
		else if (is_D)	PROCESSABLE = (S >= I);
		else ASSERT_DEAD_END();

		if (p_GDB.size() <= 2) PROCESSABLE = false;

		//dbg_cout_GDB_vector (p_GDB);

		if (PROCESSABLE) {

			if (is_M)	INTERVAL_buf = WELL_AVERAGE_M (p_GDB);//ok
			else		INTERVAL_buf = WELL_AVERAGE_D (p_GDB);

			INTERVAL_buf = FIRST_DERIVATE (INTERVAL_buf);

			FREQUENCY_buf = FREQUENCY (p_GDB);
		}
		W_INTERVAL.push_back (INTERVAL_buf);
		W_FREQUENCY.push_back (FREQUENCY_buf);
	}
	return;
}

/*
 * void PROCESS_WELL_GROUPS (const vector <vector <vector <vector <GDB> > > >& inGDB_G, const bool TILT) {

	if (is_WELLDATA_NO()) return;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		vector <vector <vector <WELL_INTERVAL> > > INTERVAL_buf_3;
		vector <vector <vector <WELL_FREQUENCY> > > FREQUENCY_buf_3;

		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {

			vector <vector <WELL_INTERVAL> > INTERVAL_buf_2;
			vector <vector <WELL_FREQUENCY> > FREQUENCY_buf_2;

			for (size_t k = 0; k < inGDB_G.at(i).at(j).size(); k++) {

				vector <WELL_INTERVAL> INTERVAL_buf;
				vector <WELL_FREQUENCY> FREQUENCY_buf;

				vector <GDB> p_GDB = inGDB_G.at(i).at(j).at(k);
				p_GDB = SORT_GDB (p_GDB, "DEPTH");
				//p_GDB = sort_by_DEPTH (p_GDB);

				const bool is_M = is_WELL_INTERVAL_METER();
				const bool is_D = is_WELL_INTERVAL_DATANUMBER();
				const size_t S = p_GDB.size();

				const double MIN_DEPTH = p_GDB.at(0).DEPTH;
				const double MAX_DEPTH = p_GDB.at(S - 1).DEPTH;

				const double IVL = is_WELL_INTERVAL_LENGTH();
				//const size_t I = double_to_size_t (IVL);

				const size_t I = string_to_double (size_t_to_string (IVL));

				bool PROCESSABLE = false;

				if 		(is_M) 	PROCESSABLE = (MAX_DEPTH - MIN_DEPTH) > IVL;
				else if (is_D)	PROCESSABLE = (S >= I);
				else ASSERT_DEAD_END();

				if (p_GDB.size() <= 2) PROCESSABLE = false;

				//dbg_cout_GDB_vector (p_GDB);

				if (PROCESSABLE) {

					if (is_M)	INTERVAL_buf = WELL_AVERAGE_M (p_GDB);//ok
					else		INTERVAL_buf = WELL_AVERAGE_D (p_GDB);

					INTERVAL_buf = FIRST_DERIVATE (INTERVAL_buf);

					FREQUENCY_buf = FREQUENCY (p_GDB);
				}
				INTERVAL_buf_2.push_back (INTERVAL_buf);
				FREQUENCY_buf_2.push_back (FREQUENCY_buf);
			}
			INTERVAL_buf_3.push_back (INTERVAL_buf_2);
			FREQUENCY_buf_3.push_back (FREQUENCY_buf_2);
		}
		W_INTERVAL.push_back (INTERVAL_buf_3);
		W_FREQUENCY.push_back(FREQUENCY_buf_3);
	}
	//dbg_cout_GDB_vector_vector (inGDB_G);
	//dbg_cout_AVERAGE (W_AVERAGE);
}
 */






/*
void dbg_cout_AVERAGE (vector <vector <vector <vector <WELL_AVERAGE> > > > IN) {

	cout << "********  DEBUGGING WELL AVERAGES  ********" << endl;

	for (size_t i = 0; i < IN.size(); i++) {
		for (size_t j = 0; j < IN.at(i).size(); j++) {
			for (size_t k = 0; k < IN.at(i).at(j).size(); k++) {

				cout << "New average data set" << endl;

				for (size_t l = 0; l < IN.at(i).at(j).at(k).size(); l++) {

					cout
					<< fixed << setprecision (0)
					<< IN.at(i).at(j).at(k).at(l).DEPTH << '\t'
					<< IN.at(i).at(j).at(k).at(l).DD.DIPDIR << '\t'
					<< IN.at(i).at(j).at(k).at(l).DD.DIP << '\t'
					<< fixed << setprecision (6)
					<< IN.at(i).at(j).at(k).at(l).D.X << '\t'
					<< IN.at(i).at(j).at(k).at(l).D.Y << '\t'
					<< IN.at(i).at(j).at(k).at(l).D.Z << '\t' << endl;
				}
			}
		}
	}
	cout << "********  DEBUGGING WELL AVERAGES  ********" << endl;

	return;
}
*/
