
// Copyright (C) 2012- 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <iomanip>
#include <math.h>

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

using namespace std;

/*
VCTR N_to_S (const VCTR& in) {

	if (in.Z < 0.0) return unitvector(declare_vector( in.Y, -in.X, 0.0));
	else 			return unitvector(declare_vector(-in.Y,  in.X, 0.0));
}

VCTR N_to_D (const VCTR& in) {

	const VCTR S = N_to_S (in);

	return crossproduct(S, in);
}
*/

/*
bool is_overturned (const GDB& in){

	return is_allowed_bedding_overturned_sense (in.OFFSET);
}
*/

/*
bool is_bedding_present (const vector <GDB>& to_process) {

	for (size_t i = 0; i < to_process.size(); i++) {

		if (is_allowed_handle_as_bedding(to_process.at(i).DATATYPE) && to_process.at(i).avd.DIP < 91.0) return true;
	}
	return false;
}
*/

/*
bool is_plane_dataset_singular (const vector <GDB>& inGDB) {

	const vector <VCTR> BNG = generate_Bingham_dataset (inGDB);//ok

	//cout << fixed << setprecision(6) << endl;

	//for (size_t i = 0; i < BNG.size(); i++)

		//cout << BNG.at(i).X << " - " << BNG.at(i).Y << " - " << BNG.at(i).Z << endl;

	const STRESSTENSOR st = st_BINGHAM (BNG);

	const double det = stresstensor_determinant (st);

	return (fabs(det) < 10e-15);
}
*/

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




/*vector <GDB> generate_Bingham_dataset (const vector <GDB>& inGDB) {

	vector <GDB> T = inGDB;

	const string DT = T.at(0).DATATYPE;

	const bool FOR_FOLDSURFACE = is_allowed_foldsurface_processing (DT);
	//const bool BEDDING = is_allowed_handle_as_bedding(DT);

	for (size_t i = 0; i < T.size(); i++) {

		//bool OTB = (is_overturned (inGDB.at(i)) && is_allowed_foldsurface_processing(inGDB.at(i).DATATYPE));

		if (FOR_FOLDSURFACE) T.at(i).N = inGDB.at(i).N;

			////!if (OTB) 	T.at(i).N = flip_vector (inGDB.at(i).N);
			////else		T.at(i).N =              inGDB.at(i).N;

		else T.at(i).N = inGDB.at(i).D;

			////!if (OTB) 	T.at(i).N = flip_vector (inGDB.at(i).D);
			////else 		T.at(i).N =              inGDB.at(i).D;

	}
	return T;
}
 *
 */


VCTR process_for_average_MT2 (const vector <GDB>& inGDB) {

	vector <VCTR> N;

	for (size_t i = 0; i < inGDB.size(); i++) N.push_back(inGDB.at(i).N);

	return unitvector (VCTR_average(N));
}

/*
bool data_EQ2_homogeneous_and_one_overturned (const vector <GDB>& inGDB) {

	ASSERT2((inGDB.size() == 2), "More than 2 records for the average calculation despite 2 ones were expected");

	return check_dataset_geometry_homogenity (inGDB);
}
*/

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

/*
vector <GDB> init_average (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		outGDB.at(i).avD = 		declare_vector (NaN(), NaN(), NaN());
		outGDB.at(i).avS0D = 	declare_vector (NaN(), NaN(), NaN());

		outGDB.at(i).avd.DIPDIR = 	NaN();
		outGDB.at(i).avd.DIP = 		NaN();
		outGDB.at(i).avS0d.DIPDIR =	NaN();
		outGDB.at(i).avS0d.DIP = 	NaN();
	}
	return outGDB;
}
*/
VCTR calculate_data_average_vector (const vector <GDB>& to_process) {

	const string DT = to_process.at(0).DATATYPE;
	const string LC = to_process.at(0).LOC;

	const bool MT2 = is_processable_for_average_MT2 (to_process);
	const bool EQ2 = is_processable_for_average_EQ2 (to_process);
	const bool EQ1 = is_processable_for_average_EQ1 (to_process);
	const bool HMG = is_processable_for_average_HMG (to_process);

	if (!MT2 && !EQ1 && !EQ2 && !HMG) ASSERT_DEAD_END();

	if (MT2) 		return process_for_average_MT2 (to_process);
	else if (EQ2) 	return process_for_average_EQ2 (to_process);
	else if (EQ1) 	return process_for_average_EQ1 (to_process);
	else if (HMG) 	return process_for_average_EQ1 (to_process);
	else ASSERT_DEAD_END();

	return declare_vector(NaN(), NaN(), NaN());
}

vector <GDB> apply_data_average_vector (const vector <GDB>& to_process, const VCTR& AV_D, const string MODE) {

	vector <GDB> outGDB = to_process;

	const bool O = is_D_up(AV_D);
	const bool B = is_allowed_handle_as_bedding (outGDB.at(0).DATATYPE);

	for (size_t i = 0; i < outGDB.size(); i++) {

		outGDB.at(i).avD = AV_D;

		if (O && B) outGDB.at(i).avd = dipdir_dip_from_DXDYDZ (flip_vector(AV_D));
		else 		outGDB.at(i).avd = dipdir_dip_from_DXDYDZ (AV_D);
	}
	return outGDB;
}

vector <GDB> DATATYPE_AVERAGE (const vector <GDB>& inGDB, const string MODE) {

	vector <GDB> outGDB = inGDB;

	const bool PROCESSABE = is_datatype_processable_for_average (outGDB.at(0).DATATYPE);//ok

	if (!PROCESSABE) return outGDB;

	const VCTR AV_N = calculate_data_average_vector (outGDB);

	const bool O = is_N_down(AV_N);

	//cout << fixed << setprecision(6) << endl;
	//cout << "AV_N.X: " << AV_N.X << endl;

	VCTR AV_D = DXDYDZ_from_NXNYNZ(AV_N);
	if (O) AV_D = flip_vector(AV_D);

	//const DIPDIR_DIP DDD = dipdir_dip_from_DXDYDZ(AV_D);
	//cout << fixed << setprecision(0) << endl;
	//cout << "************************************************" << endl;
	//cout << "AVERAGE: " << DDD.DIPDIR << "/" << DDD.DIP << endl;

	//const bool O = is_N_down(AV_N);

	//cout << "AVERAGE O:" << O << endl;

	const bool B = is_allowed_handle_as_bedding (outGDB.at(0).DATATYPE);

	if (O && !B) ASSERT_DEAD_END();

	outGDB = apply_data_average_vector (outGDB, AV_D, MODE);

	return outGDB;
}

vector < vector <GDB> > calculate_average_for_groups (const vector <vector <GDB> >& inGDB_G) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	for (size_t i = 0; i < outGDB_G.size(); i++) outGDB_G.at(i) = DATATYPE_AVERAGE (outGDB_G.at(i), "");

	return outGDB_G;
}

bool has_group_bedding (const vector <vector <GDB> >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) {
		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {

			const string DT = inGDB_G.at(i).at(j).DATATYPE;

			if (is_allowed_handle_as_bedding(DT)) return true;
		}
	}
	return false;
}

VCTR return_group_bedding_vector (const vector <vector <GDB> >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) {
		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {

			const string DT = inGDB_G.at(i).at(j).DATATYPE;

			if (is_allowed_handle_as_bedding (DT)) return inGDB_G.at(i).at(j).avD;
		}
	}
	ASSERT_DEAD_END();

	return declare_vector(NaN(), NaN(), NaN());
}

vector <vector <GDB> > apply_group_bedding_vector (const vector <vector <GDB> >& inGDB_G, const VCTR AV_D) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	const bool O = is_D_up (AV_D);

	for (size_t i = 0; i < outGDB_G.size(); i++) {
		for (size_t j = 0; j < outGDB_G.at(i).size(); j++) {

			GDB ACT = outGDB_G.at(i).at(j);

			ACT.avS0D = AV_D;

			if (!O) {

				ACT.avS0offset = "NORMAL";
				ACT.avS0N = NXNYNZ_from_DXDYDZ(AV_D);
				ACT.avS0d = dipdir_dip_from_DXDYDZ(AV_D);
			}
			else {

				ACT.avS0offset = "OVERTURNED";
				ACT.avS0N = NXNYNZ_from_DXDYDZ(flip_vector(AV_D));
				ACT.avS0d = dipdir_dip_from_DXDYDZ(flip_vector(AV_D));
			}
			outGDB_G.at(i).at(j) = ACT;
		}
	}
	return outGDB_G;
}

vector < vector < vector <vector <GDB> > > > AVERAGE (const vector < vector < vector <vector <GDB> > > >& inGDB_G) {

	vector < vector < vector <vector <GDB> > > > outGDB_G = inGDB_G;

	for (size_t i = 0; i < outGDB_G.size(); i++) {

		for (size_t j = 0; j < outGDB_G.at(i).size(); j++) {

			vector <vector <GDB> > ACT = outGDB_G.at(i).at(j);

			//dbg_cout_GDB_v_v (ACT);
			ACT = calculate_average_for_groups (ACT);//seems to be ok

			//dbg_cout_GDB_v_v(ACT);
			if (has_group_bedding (ACT)) {

				const VCTR AV_D = return_group_bedding_vector (ACT);

				ACT = apply_group_bedding_vector (ACT, AV_D);
				//dbg_cout_GDB_v_v(ACT);
			}
			outGDB_G.at(i).at(j) = ACT;
		}
	}
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
