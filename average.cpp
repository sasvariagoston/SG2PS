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
#include "foldsurface.hpp"
#include "rgf.h"

using namespace std;

VCTR N_to_S (VCTR in) {

	if (in.Z < 0.0) return (unitvector(declare_vector( in.Y, -in.X, 0.0)));
	else 			return (unitvector(declare_vector(-in.Y,  in.X, 0.0)));
}

VCTR N_to_D (VCTR in) {

	VCTR S = N_to_S (in);

	return crossproduct(S, in);
}

bool is_overturned (GDB in){

	return is_allowed_bedding_overturned_sense(in.OFFSET);
}

bool is_bedding_present (vector <GDB> to_process) {

	for (size_t i = 0; i < to_process.size(); i++) {

		if (to_process.at(i).DATATYPE == "BEDDING" && to_process.at(i).avd.DIP < 91.0) return true;
	}
	return false;
}

bool is_plane_dataset_singular (vector <GDB> inGDB, string METHOD) {

	vector <GDB> temp_for_Bingham = generate_Bingham_dataset (inGDB, METHOD);

	STRESSTENSOR st = st_BINGHAM (temp_for_Bingham);

	double det = stresstensor_determinant (st);

	return (fabs(det) < 10e-15);
}

bool is_datatype_processable_for_average (vector <GDB> inGDB) {

	string DT = inGDB.at(0).DATATYPE;
	string LOC = inGDB.at(0).LOC;

	bool IS_STRIAE = is_allowed_striae_datatype(DT);
	bool IS_SC = is_allowed_SC_datatype(DT);
	bool IS_lithology = is_allowed_lithology_datatype(DT);

	return (!IS_STRIAE && !IS_SC && !IS_lithology);
}

bool is_processable_for_average_MT2 (vector <GDB> inGDB) {

	return ((inGDB.size() > 2) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_EQ2 (vector <GDB> inGDB) {

	return ((inGDB.size() == 2) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_EQ1 (vector <GDB> inGDB) {

	return ((inGDB.size() == 1) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_HMG (vector <GDB> inGDB) {

	return (!check_dataset_homogenity (inGDB));
}

vector <GDB> generate_Bingham_dataset (vector <GDB> inGDB, string method) {

	vector <GDB> temp_for_Bingham;

	bool FOR_AVERAGE = 			(method == "AVERAGE");
	bool FOR_BEDDING_AVERAGE = 	(method == "BEDDING_AVERAGE");
	bool FOR_FOLDSURFACE = 		(method == "FOLDSURFACE");

	if (!FOR_AVERAGE && !FOR_FOLDSURFACE && !FOR_BEDDING_AVERAGE) ASSERT_DEAD_END();

	for (size_t i = 0; i < inGDB.size(); i++) {

			bool OTB = (is_overturned (inGDB.at(i)) && is_allowed_foldsurface_processing(inGDB.at(i).DATATYPE));

			GDB buf;

			temp_for_Bingham.push_back(buf);
			temp_for_Bingham.at(i) = inGDB.at(i);


			if (FOR_AVERAGE || FOR_BEDDING_AVERAGE) {

				if (OTB) 	temp_for_Bingham.at(i).N = flip_vector(inGDB.at(i).D);
				else 		temp_for_Bingham.at(i).N =             inGDB.at(i).D;
			}
			else if (FOR_FOLDSURFACE) {

				if (OTB) 	temp_for_Bingham.at(i).N = flip_vector(inGDB.at(i).N);
				else 		temp_for_Bingham.at(i).N =             inGDB.at(i).N;
			}
			else ASSERT_DEAD_END()
		}
	return temp_for_Bingham;
}

VCTR process_for_average_MT2 (vector <GDB> inGDB, string method) {

	vector <GDB> temp_for_Bingham = generate_Bingham_dataset (inGDB, method);

	bool FOR_AVERAGE = 		(method == "AVERAGE");
	bool FOR_FOLDSURFACE = 	(method == "FOLDSURFACE");

	if (!FOR_AVERAGE && !FOR_FOLDSURFACE) ASSERT_DEAD_END();

	STRESSTENSOR st = st_BINGHAM (temp_for_Bingham);

	STRESSFIELD sf = eigenvalue_eigenvector (st);

	bool OVERTURNED = (fabs(sf.EIGENVALUE.Z) > fabs(sf.EIGENVALUE.X));

	sf = sf_BINGHAM (sf);

	if (FOR_AVERAGE) {


		if (OVERTURNED) return (sf.EIGENVECTOR3);
		else return flip_vector(sf.EIGENVECTOR3);
	}
	else {

		if (!OVERTURNED) return (sf.EIGENVECTOR1);
		else return flip_vector(sf.EIGENVECTOR1);
	}
}

bool data_EQ2_homogeneous_and_one_overturned (vector <GDB> inGDB) {

	ASSERT2((inGDB.size() == 2), "More than 2 records for the average calculation despite 2 ones were expected");

	return check_dataset_geometry_homogenity (inGDB);
}

VCTR process_for_average_EQ2 (vector <GDB> inGDB) {

	ASSERT2((inGDB.size() == 2), "More than 2 records for the average calculation despite 2 ones were expected");

	VCTR N1 = inGDB.at(0).N;
	VCTR N2 = inGDB.at(1).N;

	bool OTB_1 = (is_overturned (inGDB.at(0)) && (inGDB.at(0).DATATYPE) == "BEDDING");
	bool OTB_2 = (is_overturned (inGDB.at(1)) && (inGDB.at(1).DATATYPE) == "BEDDING");

	if (OTB_1) N1 = flip_vector(N1);
	if (OTB_2) N2 = flip_vector(N2);

	bool is_SYMMETRICAL =
			is_in_range(N1.X, N1.X, -N2.X) &&
			is_in_range(N1.Y, N1.Y, -N2.Y) &&
			is_in_range(N1.Z, N1.Z,  N2.Z);

	if (!data_EQ2_homogeneous_and_one_overturned (inGDB)) return (declare_vector (0.0, 1.0, 0.0));

	if (is_SYMMETRICAL) return inGDB.at(0).S;

	VCTR OUT = N_to_D(unitvector (declare_vector (
			N1.X + N2.X,
			N1.Y + N2.Y,
			N1.Z + N2.Z)));

	return OUT;
}

VCTR process_for_average_EQ1 (vector <GDB> inGDB) {

	GDB I = inGDB.at(0);

	if (is_overturned (I) && (I.DATATYPE) == "BEDDING") return (flip_vector(I.D));
	else return I.D;
}

vector <GDB> init_average (vector <GDB> inGDB) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		inGDB.at(i).avD = 	declare_vector (0.0, 1.0, 0.0);
		inGDB.at(i).avS0D = 	declare_vector (0.0, 1.0, 0.0);

		inGDB.at(i).avd.DIPDIR = 	999.99;
		inGDB.at(i).avd.DIP = 		999.99;
		inGDB.at(i).avS0d.DIPDIR =	999.99;
		inGDB.at(i).avS0d.DIP = 	999.99;
	}
	return inGDB;
}

vector <size_t> generate_block_length (vector <GDB> inGDB, string METHOD) {

	vector <size_t> length_container;

	size_t records_counter = 0;

	if (inGDB.size() == 1) {

		length_container.push_back(1);
		return length_container;
	}

	bool FOR_DATA_AVERAGE = 	(METHOD == "DATA_AVERAGE");
	bool FOR_BEDDING_AVERAGE = 	(METHOD == "BEDDING_AVERAGE");
	bool FOR_FOLD_SURFACE = 	(METHOD == "FOLDSURFACE");

	if (!FOR_DATA_AVERAGE && !FOR_BEDDING_AVERAGE && !FOR_FOLD_SURFACE) ASSERT_DEAD_END();

	for (size_t SIZE = 1; SIZE < inGDB.size(); SIZE++) {

		bool END_CRITERIA = false;

		bool END_DATABASE = false;
		if (SIZE == inGDB.size() - 1) END_DATABASE = true;

		string PRW_LOC = inGDB.at(SIZE - 1).LOC;
		string REC_LOC = inGDB.at(SIZE).LOC;

		string PRW_DATATYPE = inGDB.at(SIZE - 1).DATATYPE;
		string REC_DATATYPE = inGDB.at(SIZE).DATATYPE;

		if (FOR_DATA_AVERAGE || FOR_FOLD_SURFACE) 	END_CRITERIA = (PRW_LOC != REC_LOC || PRW_DATATYPE != REC_DATATYPE);
		else 										END_CRITERIA = (PRW_LOC != REC_LOC);

		records_counter++;

		if (END_CRITERIA || END_DATABASE) {

			if (END_CRITERIA && !END_DATABASE) length_container.push_back(records_counter);
			else if (!END_CRITERIA && END_DATABASE) length_container.push_back(records_counter + 1);
			else if (END_CRITERIA && END_DATABASE) {

				length_container.push_back(records_counter);
				length_container.push_back(1);
			}
			else ASSERT_DEAD_END();

			records_counter = 0;
		}
		else{}// OK
	}
	return length_container;
}


VCTR calculate_bedding_average_vector (vector <GDB> to_process) {

	VCTR out;

	for (size_t i = 0; i < to_process.size(); i++) {

		if (to_process.at(i).DATATYPE == "BEDDING") out = to_process.at(i).avD;
	}
	return out;
}

VCTR calculate_data_average_vector (vector <GDB> to_process, string method) {

	VCTR OUT;

	bool MT2 = is_processable_for_average_MT2 (to_process);
	bool EQ2 = is_processable_for_average_EQ2 (to_process);
	bool EQ1 = is_processable_for_average_EQ1 (to_process);
	bool HMG = is_processable_for_average_HMG (to_process);

	if (!MT2 && !EQ1 && !EQ2 && !HMG) ASSERT_DEAD_END();

	if 		(MT2) OUT = process_for_average_MT2 (to_process, method);
	else if (EQ2) OUT = process_for_average_EQ2 (to_process);
	else if (EQ1) OUT = process_for_average_EQ1 (to_process);
	else if (HMG) OUT = process_for_average_EQ1 (to_process);
	else ASSERT_DEAD_END();

	return OUT;
}

vector <GDB> apply_data_average_vector (vector <GDB> to_process, VCTR AV_D, string METHOD) {

	bool FOR_DATA_AVERAGE = 	(METHOD == "DATA_AVERAGE");
	bool FOR_BEDDING_AVERAGE =	(METHOD == "BEDDING_AVERAGE");

	if (!FOR_DATA_AVERAGE && !FOR_BEDDING_AVERAGE) ASSERT_DEAD_END();

	bool IS_OVERTURNED = (AV_D.Z > 0.0);
	bool IS_BEDDING = (to_process.at(0).DATATYPE == "BEDDING");

	bool OTB = (IS_OVERTURNED && IS_BEDDING);

	for (size_t i = 0; i < to_process.size(); i++) {

		if (FOR_DATA_AVERAGE) {

			to_process.at(i).avD = AV_D;

			if (OTB) 	to_process.at(i).avd = dipdir_dip_from_DXDYDZ(flip_vector(AV_D));
			else 		to_process.at(i).avd = dipdir_dip_from_DXDYDZ(AV_D);
		}
		else if (FOR_BEDDING_AVERAGE) {

			bool BED_OK = is_bedding_present (to_process);

			if (BED_OK) {

				to_process.at(i).avS0D = AV_D;

				to_process.at(i).avS0N = NXNYNZ_from_DXDYDZ(AV_D);

				if (OTB) 	to_process.at(i).avS0d = dipdir_dip_from_DXDYDZ(flip_vector(AV_D));
				else 		to_process.at(i).avS0d = dipdir_dip_from_DXDYDZ(AV_D);

				if (OTB) to_process.at(i).avS0offset = "OVERTURNED";
			}
			else {

				to_process.at(i).avS0d.DIPDIR = 999.99;
				to_process.at(i).avS0d.DIP = 	999.99;
			}
		}
		else ASSERT_DEAD_END();
	}

	return to_process;
}

vector <GDB> DATATYPE_AVERAGE (vector <GDB> inGDB, vector <size_t> length_container, string METHOD) {

	vector <GDB> outGDB;

	bool FOR_DATA_AVERAGE = 	(METHOD == "DATA_AVERAGE");
	bool FOR_BEDDING_AVERAGE = 	(METHOD == "BEDDING_AVERAGE");
	bool FOR_FOLD_SURFACE = 	(METHOD == "FOLDSURFACE");

	if (!FOR_DATA_AVERAGE && !FOR_BEDDING_AVERAGE && !FOR_FOLD_SURFACE) ASSERT_DEAD_END();

	size_t RANGE_START = 0;
	size_t RANGE_END = 0;

	for (size_t i = 0; i < length_container.size(); i++) {

		vector <GDB> to_process;

		RANGE_END = RANGE_END + length_container.at(i);

		for (size_t k = RANGE_START; k < RANGE_END; k++) to_process.push_back(inGDB.at(k));

		RANGE_START = RANGE_START + length_container.at(i);


		bool is_DATA_FOLDSURFACE = is_allowed_foldsurface_processing(to_process.at(0).DATATYPE);
		bool PROCESSABE = (is_datatype_processable_for_average(to_process));
		bool MT2 = (to_process.size() > 2);
		bool SNG = MT2 && is_plane_dataset_singular (to_process, "AVERAGE");

		if (FOR_DATA_AVERAGE && PROCESSABE && !SNG) {

			VCTR AV_D = calculate_data_average_vector (to_process, "AVERAGE");

			to_process = apply_data_average_vector (to_process, AV_D, "DATA_AVERAGE");
		}
		else if (FOR_BEDDING_AVERAGE && PROCESSABE) {

			VCTR AV_D = calculate_bedding_average_vector (to_process);

			to_process = apply_data_average_vector (to_process, AV_D, "BEDDING_AVERAGE");
		}
		else if (is_DATA_FOLDSURFACE && FOR_FOLD_SURFACE && PROCESSABE && !SNG) {

			VCTR GR_CRC = calculate_data_average_vector (to_process, "FOLDSURFACE");

			to_process = apply_data_average_vector (to_process, GR_CRC, "DATA_AVERAGE");
		}
		else if (SNG) {

			cout << "  - Singular " << to_process.at(0).DATATYPE << " data set at location " << to_process.at(0).LOC << "." << endl;
		}
		else {} //OK

		outGDB = merge_GDB(to_process, outGDB);
	}
	return outGDB;
}

void dbg_averages (vector <GDB> inGDB) {

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

vector <GDB> cGc_average (vector <GDB> inGDB) {

	vector <GDB> outGDB = init_average (inGDB);

	vector <size_t> length_container;

	length_container = generate_block_length (outGDB, "DATA_AVERAGE");
	outGDB = DATATYPE_AVERAGE (outGDB, length_container, "DATA_AVERAGE");

	length_container = generate_block_length (outGDB, "BEDDING_AVERAGE");
	outGDB = DATATYPE_AVERAGE (outGDB, length_container, "BEDDING_AVERAGE");

	cout << "  - Data set averages were computed for " << outGDB.size() << " records." << endl;

	return outGDB;
}
