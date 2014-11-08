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

using namespace std;

VCTR N_to_S (const VCTR& in) {

	if (in.Z < 0.0) return unitvector(declare_vector( in.Y, -in.X, 0.0));
	else 			return unitvector(declare_vector(-in.Y,  in.X, 0.0));
}

VCTR N_to_D (const VCTR& in) {

	const VCTR S = N_to_S (in);

	return crossproduct(S, in);
}

bool is_overturned (const GDB& in){

	return is_allowed_bedding_overturned_sense (in.OFFSET);
}

bool is_bedding_present (const vector <GDB>& to_process) {

	for (size_t i = 0; i < to_process.size(); i++) {

		if (is_allowed_handle_as_bedding(to_process.at(i).DATATYPE) && to_process.at(i).avd.DIP < 91.0) return true;
	}
	return false;
}

bool is_plane_dataset_singular (const vector <GDB>& inGDB) {

	const vector <GDB> T = generate_Bingham_dataset (inGDB);

	const STRESSTENSOR st = st_BINGHAM (T);

	const double det = stresstensor_determinant (st);

	return (fabs(det) < 10e-15);
}

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

	return ((inGDB.size() == 2) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_EQ1 (const vector <GDB>& inGDB) {

	return ((inGDB.size() == 1) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_HMG (const vector <GDB>& inGDB) {

	return (!check_dataset_homogenity (inGDB));
}

vector <GDB> generate_Bingham_dataset (const vector <GDB>& inGDB) {

	vector <GDB> T = inGDB;

	const bool FOR_FOLDSURFACE = is_allowed_foldsurface_processing(T.at(0).DATATYPE);

	for (size_t i = 0; i < T.size(); i++) {

		bool OTB = (is_overturned (inGDB.at(i)) && is_allowed_foldsurface_processing(inGDB.at(i).DATATYPE));

		if (FOR_FOLDSURFACE) {

			if (OTB) 	T.at(i).N = flip_vector (inGDB.at(i).N);
			else		T.at(i).N =              inGDB.at(i).N;
		}
		else {

			if (OTB) 	T.at(i).N = flip_vector (inGDB.at(i).D);
			else 		T.at(i).N =              inGDB.at(i).D;
		}
	}
	return T;
}

VCTR process_for_average_MT2 (const vector <GDB>& inGDB) {

	vector <GDB> temp_for_Bingham = generate_Bingham_dataset (inGDB);

	const bool FOR_FOLDSURFACE = is_allowed_foldsurface_processing(inGDB.at(0).DATATYPE);

	STRESSTENSOR st = st_BINGHAM (temp_for_Bingham);

	STRESSFIELD sf = sf_BINGHAM (st);

	bool OVERTURNED = (fabs(sf.EIGENVALUE.Z) > fabs(sf.EIGENVALUE.X));

	if (FOR_FOLDSURFACE) {

		if (!OVERTURNED) return (sf.EIGENVECTOR1);
		else return flip_vector(sf.EIGENVECTOR1);
	}
	else {

		if (OVERTURNED) return (sf.EIGENVECTOR3);
		else return flip_vector(sf.EIGENVECTOR3);
	}
}

bool data_EQ2_homogeneous_and_one_overturned (const vector <GDB>& inGDB) {

	ASSERT2((inGDB.size() == 2), "More than 2 records for the average calculation despite 2 ones were expected");

	return check_dataset_geometry_homogenity (inGDB);
}

VCTR process_for_average_EQ2 (const vector <GDB>& inGDB) {

	ASSERT2((inGDB.size() == 2), "More than 2 records for the average calculation despite 2 ones were expected");

	VCTR N1 = inGDB.at(0).N;
	VCTR N2 = inGDB.at(1).N;

	//const bool OTB_1 = is_overturned (inGDB.at(0)) && (inGDB.at(0).DATATYPE) == "BEDDING";
	//const bool OTB_2 = is_overturned (inGDB.at(1)) && (inGDB.at(1).DATATYPE) == "BEDDING";

	const bool OTB_1 = is_overturned (inGDB.at(0)) && is_allowed_handle_as_bedding (inGDB.at(0).DATATYPE);
	const bool OTB_2 = is_overturned (inGDB.at(1)) && is_allowed_handle_as_bedding (inGDB.at(1).DATATYPE);


	if (OTB_1) N1 = flip_vector(N1);
	if (OTB_2) N2 = flip_vector(N2);

	const bool is_SYMMETRICAL =
			is_in_range(N1.X, N1.X, -N2.X) &&
			is_in_range(N1.Y, N1.Y, -N2.Y) &&
			is_in_range(N1.Z, N1.Z,  N2.Z);

	if (!data_EQ2_homogeneous_and_one_overturned (inGDB)) {

		return declare_vector (0.0, 1.0, 0.0);
	}

	if (is_SYMMETRICAL) return inGDB.at(0).S;

	VCTR OUT = N_to_D(unitvector (declare_vector (
			N1.X + N2.X,
			N1.Y + N2.Y,
			N1.Z + N2.Z)));

	return OUT;
}

VCTR process_for_average_EQ1 (const vector <GDB>& inGDB) {

	GDB I = inGDB.at(0);

	//if (is_overturned (I) && (I.DATATYPE) == "BEDDING") return (flip_vector(I.D));

	if (is_overturned (I) && is_allowed_handle_as_bedding(I.DATATYPE)) return (flip_vector(I.D));

	else return I.D;
}

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

VCTR calculate_data_average_vector (const vector <GDB>& to_process) {

	VCTR OUT;

	const bool MT2 = is_processable_for_average_MT2 (to_process);
	const bool EQ2 = is_processable_for_average_EQ2 (to_process);
	const bool EQ1 = is_processable_for_average_EQ1 (to_process);
	const bool HMG = is_processable_for_average_HMG (to_process);

	if (!MT2 && !EQ1 && !EQ2 && !HMG) ASSERT_DEAD_END();

	if 		(MT2) OUT = process_for_average_MT2 (to_process);
	else if (EQ2) OUT = process_for_average_EQ2 (to_process);
	else if (EQ1) OUT = process_for_average_EQ1 (to_process);
	else if (HMG) OUT = process_for_average_EQ1 (to_process);
	else ASSERT_DEAD_END();

	return OUT;
}

vector <GDB> apply_data_average_vector (const vector <GDB>& to_process, const VCTR& AV_D, const string MODE) {

	const bool FOLD = MODE == "FOLD";

	vector <GDB> outGDB = to_process;

	const bool IS_OVERTURNED = AV_D.Z > 0.0;
	const bool IS_BEDDING = is_allowed_handle_as_bedding (to_process.at(0).DATATYPE);

	const bool OTB = IS_OVERTURNED && IS_BEDDING;

	for (size_t i = 0; i < outGDB.size(); i++) {

		if (FOLD) {

			outGDB.at(i).fold_great_circle_N = AV_D;
		}
		else {
			outGDB.at(i).avD = AV_D;

			if (OTB) 	outGDB.at(i).avd = dipdir_dip_from_DXDYDZ (flip_vector(AV_D));
			else 		outGDB.at(i).avd = dipdir_dip_from_DXDYDZ (AV_D);
		}
	}
	return outGDB;
}

vector <GDB> DATATYPE_AVERAGE (const vector <GDB>& inGDB, const string MODE) {

	const bool FOLD = MODE == "FOLD";

	vector <GDB> outGDB = inGDB;

	const bool PROCESSABE = is_datatype_processable_for_average (outGDB.at(0).DATATYPE);

	if (!PROCESSABE) return outGDB;

	const bool SNG = outGDB.size() > 2 && is_plane_dataset_singular (outGDB);

	for (size_t i = 0; i < outGDB.size(); i++) {

		VCTR AV_D;

		if (!SNG) {

			AV_D = calculate_data_average_vector (outGDB);
		}
		else  {

			AV_D = outGDB.at(0).D;

			if (!is_mode_DEBUG()) cout << "  - Singular " << outGDB.at(0).DATATYPE << " data set at location " << outGDB.at(0).LOC << "." << endl;
		}

		outGDB = apply_data_average_vector (outGDB, AV_D, MODE);
	}
	return outGDB;
}

vector < vector <GDB> > calculate_average_for_groups (const vector <vector <GDB> >& inGDB_G) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	for (size_t i = 0; i < outGDB_G.size(); i++) {

			vector <GDB> ACT = outGDB_G.at(i);

			ACT = init_average (ACT);

			ACT = DATATYPE_AVERAGE (ACT, "");

			outGDB_G.at(i) = ACT;
	}
	return outGDB_G;
}

bool has_group_bedding (const vector <vector <GDB> >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) {
		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {

			//if (inGDB_G.at(i).at(j).DATATYPE == "BEDDING") return true;
			if (is_allowed_handle_as_bedding(inGDB_G.at(i).at(j).DATATYPE)) return true;
		}
	}
	return false;
}

VCTR return_group_bedding_vector (const vector <vector <GDB> >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) {
		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {

			//if (inGDB_G.at(i).at(j).DATATYPE == "BEDDING") return inGDB_G.at(i).at(j).avD;
			if (is_allowed_handle_as_bedding (inGDB_G.at(i).at(j).DATATYPE)) return inGDB_G.at(i).at(j).avD;
		}
	}
	ASSERT_DEAD_END();

	return declare_vector(NaN(), NaN(), NaN());
}

vector <vector <GDB> > apply_group_bedding_vector (const vector <vector <GDB> >& inGDB_G, const VCTR AV_D) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	const bool OVERTURNED = (AV_D.Z > 0.0);

	for (size_t i = 0; i < outGDB_G.size(); i++) {
		for (size_t j = 0; j < outGDB_G.at(i).size(); j++) {

			GDB ACT = outGDB_G.at(i).at(j);

			ACT.avS0D = AV_D;
			ACT.avS0N = NXNYNZ_from_DXDYDZ(AV_D);

			if (OVERTURNED) {

				ACT.avS0d = dipdir_dip_from_DXDYDZ (flip_vector(AV_D));
				ACT.avS0offset = "OVERTURNED";
			}
			else ACT.avS0d = dipdir_dip_from_DXDYDZ (AV_D);

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

			ACT = calculate_average_for_groups (ACT);

			if (has_group_bedding (ACT)) {

				const VCTR AV_D = return_group_bedding_vector (ACT);

				ACT = apply_group_bedding_vector (ACT, AV_D);
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
