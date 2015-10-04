// Copyright (C) 2012- 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

#include "average.hpp"
#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "bingham.h"
#include "common.h"
#include "data_io.h"
#include "foldsurface.hpp"
#include "homogenity_check.hpp"
#include "rgf.h"
#include "run_mode.h"
#include "stresstensor.hpp"
#include "settings.hpp"
#include "well.hpp"

using namespace std;

bool is_datatype_processable_for_average (const string DT) {

	bool IS_STRIAE = 		is_allowed_striae_datatype(DT);
	bool IS_SC = 			is_allowed_SC_datatype(DT);
	bool IS_lithology = 	is_allowed_lithology_datatype(DT);

	return (!IS_STRIAE && !IS_SC && !IS_lithology);
}

bool is_processable_for_average_MT2 (const vector <GDB>& inGDB) {

	return ((inGDB.size() > 2) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_EQ2 (const vector <GDB>& inGDB) {

	return inGDB.size() == 2;
}

bool is_processable_for_average_EQ1 (const vector <GDB>& inGDB) {

	return inGDB.size() == 1;
}

bool is_processable_for_average_HMG (const vector <GDB>& inGDB) {

	return (!check_dataset_homogenity (inGDB) && inGDB.size() > 2);
}

VCTR process_for_average_MT2 (const vector <GDB>& inGDB) {

	vector <VCTR> N;

	for (size_t i = 0; i < inGDB.size(); i++) N.push_back(inGDB.at(i).N);

	return unitvector (VCTR_average(N));
}

VCTR process_for_average_EQ2 (const vector <GDB>& inGDB) {

	ASSERT2((inGDB.size() == 2), "More than 2 records for the average calculation despite 2 ones were expected");

	VCTR N1 = inGDB.at(0).N;
	VCTR N2 = inGDB.at(1).N;

	const bool is_SYMMETRICAL =
			is_in_range(N1.X, N1.X, -N2.X) &&
			is_in_range(N1.Y, N1.Y, -N2.Y) &&
			is_in_range(N1.Z, N1.Z, -N2.Z);

	if (is_SYMMETRICAL) return  flip_vector (inGDB.at(0).D);
	else return unitvector(declare_vector (N1.X + N2.X, N1.Y + N2.Y, N1.Z + N2.Z));
}

VCTR process_for_average_EQ1 (const vector <GDB>& inGDB) {

	return inGDB.at(0).N;
}

VCTR calculate_data_average_vector (const vector <GDB>& to_process) {

	if 		(is_processable_for_average_MT2 (to_process)) return process_for_average_MT2 (to_process);
	else if (is_processable_for_average_EQ2 (to_process)) return process_for_average_EQ2 (to_process);
	else if (is_processable_for_average_EQ1 (to_process)) return process_for_average_EQ1 (to_process);
	else if (is_processable_for_average_HMG (to_process)) return process_for_average_EQ1 (to_process);
	else ASSERT_DEAD_END();

	return declare_vector(NaN(), NaN(), NaN());
}

vector <GDB> apply_data_average_vector (const vector <GDB>& to_process, const VCTR& AV_D) {

	vector <GDB> outGDB = to_process;

	const bool O = is_D_up(AV_D);
	const bool B = is_allowed_handle_as_bedding (outGDB.at(0).DATATYPE);

	for (size_t i = 0; i < outGDB.size(); i++) {

		outGDB.at(i).avD = AV_D;

		if (O && B) outGDB.at(i).avd = dipdir_dip_from_DXDYDZ (flip_vector(AV_D));
		else outGDB.at(i).avd = dipdir_dip_from_DXDYDZ (AV_D);
	}
	return outGDB;
}

vector <GDB> DATATYPE_AVERAGE (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	const bool PROCESSABE = is_datatype_processable_for_average (outGDB.at(0).DATATYPE);//ok

	if (!PROCESSABE) return outGDB;

	const VCTR AV_N = unitvector (calculate_data_average_vector (outGDB));

	const bool O = is_N_down (AV_N);

	const bool B = is_allowed_handle_as_bedding (outGDB.at(0).DATATYPE);

	if (O && !B) ASSERT_DEAD_END();

	VCTR AV_D = unitvector(DXDYDZ_from_NXNYNZ (AV_N));
	if (O) AV_D = flip_vector(AV_D);

	outGDB = apply_data_average_vector (outGDB, AV_D);

	return outGDB;
}

VCTR return_group_bedding_vector (const vector <GDB>& inGDB) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		const string DT = inGDB.at(i).DATATYPE;

		if (is_allowed_handle_as_bedding (DT)) return inGDB.at(i).avD;
	}
	ASSERT_DEAD_END();

	return declare_vector(NaN(), NaN(), NaN());
}

vector <GDB> apply_group_bedding_vector (const vector <GDB>& inGDB_G, const VCTR AV_D) {

	vector <GDB> outGDB_G = inGDB_G;

	const bool O = is_D_up (AV_D);

	for (size_t i = 0; i < outGDB_G.size(); i++) {

		GDB ACT = outGDB_G.at(i);

		ACT.avS0D = AV_D;

		ACT.avS0offset = "NORMAL";
		ACT.avS0N = NXNYNZ_from_DXDYDZ(AV_D);
		ACT.avS0d = dipdir_dip_from_DXDYDZ(AV_D);

		if (O) {

			ACT.avS0offset = "OVERTURNED";
			ACT.avS0N = NXNYNZ_from_DXDYDZ(flip_vector(AV_D));
			ACT.avS0d = dipdir_dip_from_DXDYDZ(flip_vector(AV_D));
		}
		outGDB_G.at(i) = ACT;
	}
	return outGDB_G;
}

size_t has_relevant_bedding (const size_t i, const vector <vector <GDB> >& inGDB_G) {

	vector <size_t> EQ;

	const string eLOC = inGDB_G.at(i).at(0).LOC;
	const string eFM =  inGDB_G.at(i).at(0).FORMATION;
	const string eDT =  inGDB_G.at(i).at(0).DATATYPE;
	const char eGC =  	inGDB_G.at(i).at(0).GC.at(0);

	if (is_allowed_handle_as_bedding (eDT)) return i;

	for (size_t j = 0; j < inGDB_G.size(); j++) {

		const string LOC = 	inGDB_G.at(j).at(0).LOC;
		const string FM =  	inGDB_G.at(j).at(0).FORMATION;
		const string DT =  	inGDB_G.at(j).at(0).DATATYPE;
		const char GC =  	inGDB_G.at(j).at(0).GC.at(0);

		const bool EQ_LC = eLOC == LOC;
		const bool EQ_FM = eFM == FM;
		const bool EQ_DT = is_allowed_handle_as_bedding(DT);
		const bool EQ_GC = eGC == GC;

		bool FIT = EQ_LC && EQ_DT;

		if (is_FORMATION_USE() || is_WELLDATA_USE()) FIT = FIT && EQ_FM;

		if (is_GROUPS_USE()) FIT = FIT && EQ_GC;

		if (FIT) EQ.push_back(j);
	}
	if (EQ.size() == 0) return 99999;

	if (EQ.size() > 1) ASSERT_DEAD_END();

	return EQ.at(0);
}

vector <vector <GDB> > ASSOCIATE_AVERAGE_BEDDING_GROUPS (const vector <vector <GDB> >& inGDB_G) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	for (size_t i = 0; i < outGDB_G.size(); i++) {

		const size_t r = has_relevant_bedding (i, outGDB_G);

		if (r < 99990) {

			const bool B = is_allowed_handle_as_bedding (outGDB_G.at(r).at(0).DATATYPE);

			if (B) {

				const VCTR AV_D = return_group_bedding_vector (outGDB_G.at(r));

				outGDB_G.at(i) = apply_group_bedding_vector(outGDB_G.at(i), AV_D);
			}
		}
	}
	return outGDB_G;
}

vector <vector <GDB> > AVERAGE (const vector <vector <GDB> >& inGDB_G) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	for (size_t i = 0; i < outGDB_G.size(); i++) outGDB_G.at(i) = DATATYPE_AVERAGE (outGDB_G.at(i));

	return outGDB_G;
}

void dbg_averages (const vector <GDB>& inGDB) {

	cout
	<< "DATA_ID" << '\t'
	<< "DATATYPE" << '\t'
	<< "DIPDIR" << '\t'
	<< "DIP" << '\t'
	<< "L_DIPDIR" << '\t'
	<< "L_DIP" << '\t'
	<< "OFFSET" << '\t' << " ---- " << '\t'
	<< "AV_DIPDIR" << '\t'
	<< "AV_DIP" << '\t'<< " ---- " << '\t'
	<< "AV_BED_DIPDIR" << '\t'
	<< "AV_BED_DIP" << '\t' << " ---- " << '\t'
	<< "OFFSET" << endl;

	for (size_t i = 0; i < inGDB.size(); i++) {

		GDB R = inGDB.at(i);

		cout
		<< R.ID << '\t'
		<< R.DATATYPE << '\t'
		<< R.corr.DIPDIR << '\t'
		<< R.corr.DIP << '\t'
		<< R.corrL.DIPDIR << '\t'
		<< R.corrL.DIP << '\t'
		<< R.OFFSET	<< '\t' << " ---- " << '\t'
		<< R.avd.DIPDIR << '\t'
		<< R.avd.DIP << '\t' << " ---- " << '\t'
		<< R.avS0d.DIPDIR << '\t'
		<< R.avS0d.DIP << '\t' << " ---- " << '\t'
		<< R.avS0offset << endl;
	}
}
