// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
//#include <cmath>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "average.hpp"
#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "bingham.h"
//#include "checkrgffilecontent.h"
//#include "checkxycontent.h"
#include "common.h"
//#include "data_io.h"
#include "foldsurface.hpp"
//#include "ps.h"
//#include "random.hpp"
//#include "retilt.hpp"
#include "rgf.h"

using namespace std;


VCTR N_to_S (VCTR in) {

	if (in.Z < 0.0) return (unitvector(declare_vector( in.Y, -in.X, 0.0)));
	else 			return (unitvector(declare_vector(-in.Y,  in.X, 0.0)));
}

/*

VCTR D_to_S (VCTR in) {

	if (in.Z > 0.0) return (unitvector(declare_vector( in.Y, -in.X, 0.0)));
	else 			return (unitvector(declare_vector(-in.Y, in.X, 0.0)));
}

VCTR D_to_N (VCTR in) {

	VCTR S = D_to_S (in);

	return crossproduct(in, S);
}
*/

VCTR N_to_D (VCTR in) {

	VCTR S = N_to_S (in);

	//cout << "S: " << S.X << "  " << S.Y << endl;

	return crossproduct(S, in);
}


bool is_overturned (GDB in){

	return is_allowed_bedding_overturned_sense(in.OFFSET);
}

/*
bool is_BEDDING (GDB in){

	return (in.DATATYPE == "BEDDING" || in.DATATYPE == "FOLDSURFACE");
}
*/

bool is_bedding_present (vector <GDB> to_process) {

	for (size_t i = 0; i < to_process.size(); i++) {

		if (to_process.at(i).DATATYPE == "BEDDING" && to_process.at(i).avd.DIP < 91.0) return true;
	}
	return false;
}

bool is_plane_dataset_singular (vector <GDB> inGDB, string METHOD) {

	vector <GDB> temp_for_Bingham = generate_Bingham_dataset (inGDB, METHOD);

	//dbg_averages(temp_for_Bingham);

	STRESSTENSOR st = st_BINGHAM (temp_for_Bingham);

	double det = stresstensor_determinant (st);

	cout << temp_for_Bingham.at(0).DATATYPE << endl;
	cout << fixed << setprecision (15) << det << endl;


	if (fabs(det) < 10e-15) {

		//cout << temp_for_Bingham.at(0).ID << endl;

		//cout << temp_for_Bingham.at(0).DATATYPE << endl;

		//cout << fixed << setprecision (25) << det << endl;

		//cout << "SINGULAR" << endl;
	}

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

	//cout << "BINGHAM TENSOR GENERATION" << endl;
	//cout << inGDB.size() << endl;
	//cout << inGDB.at(0).DATATYPE << endl;

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

	//vector <GDB> temp_for_Bingham;

	/*
	for (size_t i = 0; i < inGDB.size(); i++) {

		bool OTB = (is_overturned (inGDB.at(i)) && is_allowed_foldsurface_processing(inGDB.at(i).DATATYPE));

		GDB buf;

		temp_for_Bingham.push_back(buf);
		temp_for_Bingham.at(i) = inGDB.at(i);


		if (FOR_AVERAGE) {

			if (OTB) 	temp_for_Bingham.at(i).N = flip_vector(inGDB.at(i).D);
			else 		temp_for_Bingham.at(i).N =             inGDB.at(i).D;
		}
		else if (FOR_FOLDSURFACE) {

			if (OTB) 	temp_for_Bingham.at(i).N = flip_vector(inGDB.at(i).N);
			else 		temp_for_Bingham.at(i).N =             inGDB.at(i).N;
		}
		else ASSERT_DEAD_END()
	}
*/
		//cout << fixed << setprecision(3) << endl;
		//cout << " -------------BINGHAM----OF" << flush;
		//cout << inGDB.at(0).ID << flush;
		//cout << "---------- " << endl;

		//STRESSFIELD sf = sf_BINGHAM (st_BINGHAM (temp_for_Bingham)); // volt

		STRESSTENSOR st = st_BINGHAM (temp_for_Bingham); // ez lett

		STRESSFIELD sf = eigenvalue_eigenvector (st);

		bool OVERTURNED = (fabs(sf.EIGENVALUE.Z) > fabs(sf.EIGENVALUE.X));

		//cout << "OVERTURNED?  " << OVERTURNED << endl;

		sf = sf_BINGHAM (sf);


		//cout << " -------------BEFORE N_TO_D " << endl;
		//cout << "E1: " << sf.EIGENVALUE.X << " " << sf.EIGENVECTOR1.X << " " << sf.EIGENVECTOR1.Y << " " << sf.EIGENVECTOR1.Z << endl;
		//cout << "E2: " << sf.EIGENVALUE.Y << " " << sf.EIGENVECTOR2.X << " " << sf.EIGENVECTOR2.Y << " " << sf.EIGENVECTOR2.Z << endl;
		//cout << "E3: " << sf.EIGENVALUE.Z << " " << sf.EIGENVECTOR3.X << " " << sf.EIGENVECTOR3.Y << " " << sf.EIGENVECTOR3.Z << endl;
		VCTR D1 =  (sf.EIGENVECTOR1);
		VCTR D2 =  (sf.EIGENVECTOR2);
		VCTR D3 =  (sf.EIGENVECTOR3);
		//cout << " ------------- EIGENVALUE AND EIGENVECTOR ------------- " << endl;
		//cout << "E1: " << '\t' << sf.EIGENVALUE.X << '\t' << D1.X << '\t' << D1.Y << '\t' << D1.Z << endl;
		//cout << "E2: " << '\t' << sf.EIGENVALUE.Y << '\t' << D2.X << '\t' << D2.Y << '\t' << D2.Z << endl;
		//cout << "E3: " << '\t' << sf.EIGENVALUE.Z << '\t' << D3.X << '\t' << D3.Y << '\t' << D3.Z << endl;

		//DIPDIR_DIP DD1 = dipdir_dip_from_DXDYDZ(D1);
		//cout << DD1.DIPDIR << "/" << DD1.DIP << endl;
		//DIPDIR_DIP DD2 = dipdir_dip_from_DXDYDZ(D2);
		//cout << DD2.DIPDIR << "/" << DD2.DIP << endl;
		//DIPDIR_DIP DD3 = dipdir_dip_from_DXDYDZ(D3);
		//cout << DD3.DIPDIR << "/" << DD3.DIP << endl;

		if (FOR_AVERAGE) {


			if (OVERTURNED) return (sf.EIGENVECTOR3); //alapjaraton ez fut
			else return flip_vector(sf.EIGENVECTOR3);
		}
		else {


			/*
			if (OVERTURNED) {

				sf.EIGENVECTOR1 = flip_vector(sf.EIGENVECTOR1);
				sf.EIGENVECTOR2 = flip_vector(sf.EIGENVECTOR2);
				sf.EIGENVECTOR3 = flip_vector(sf.EIGENVECTOR3);
			}

			sf.EIGENVECTOR1 = flip_N_vector(sf.EIGENVECTOR1);
			sf.EIGENVECTOR2 = flip_N_vector(sf.EIGENVECTOR2);
			sf.EIGENVECTOR3 = flip_N_vector(sf.EIGENVECTOR3);
			 */


			//cout << "FOLD SURFACE EIGENVECTORS" << endl;

			//cout << fixed << setprecision(3) << endl;



			//DIPDIR_DIP DD1 = dipdir_dip_from_NXNYNZ(D1);
			//cout << "E1: " << '\t' << sf.EIGENVALUE.X << '\t' << D1.X << '\t' << D1.Y << '\t' << D1.Z << endl;
			//cout << "E1: " << DD1.DIPDIR << "/" << DD1.DIP << endl;

			//plane normal returned!!!!

			//DIPDIR_DIP DD2 = dipdir_dip_from_NXNYNZ(D2);
			//cout << "E2: " << '\t' << sf.EIGENVALUE.Y << '\t' << D2.X << '\t' << D2.Y << '\t' << D2.Z << endl;
			//cout << "E2: " << DD2.DIPDIR << "/" << DD2.DIP << endl;
			//DIPDIR_DIP DD3 = dipdir_dip_from_NXNYNZ(D3);
			//cout << "E3: " << '\t' << sf.EIGENVALUE.Z << '\t' << D3.X << '\t' << D3.Y << '\t' << D3.Z << endl;
			//cout << "E3: " << DD3.DIPDIR << "/" << DD3.DIP << endl;

			if (!OVERTURNED) return (sf.EIGENVECTOR1); // ez meg a foldsurfce eseten
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

	//cout << "OTB: " << inGDB.at(0).ID << " " << OTB_1 << OTB_2 << endl;
	//cout << "N1: "  << N1.X << " " << N1.Y << " " << N1.Z << endl;
	//cout << "N2: "  << N2.X << " " << N2.Y << " " << N2.Z << endl;

	if (OTB_1) N1 = flip_vector(N1);
	if (OTB_2) N2 = flip_vector(N2);

	//cout << "N1: "  << N1.X << " " << N1.Y << " " << N1.Z << endl;
	//cout << "N2: "  << N2.X << " " << N2.Y << " " << N2.Z << endl;


	if (!data_EQ2_homogeneous_and_one_overturned (inGDB)) {

		//cout << "OPPOSITE DIPS!!!" << endl;
		//cout << inGDB.at(0).ID << "  -  " << inGDB.at(1).ID << endl;

		return (declare_vector (0.0, 1.0, 0.0));
	}

	//rossz eredmenyt ad vissza, mert a lentebbi konverzio soran elvesz, hogy at volt bilentve!

	//kulon fuggvenyt kell irni erre, ami kozvetlenu szamon a D-bol N-et!!!

	//megnezni, hol lett meg meghivva a DXDYDZ_from NXNYNZ

	VCTR OUT = N_to_D(unitvector (declare_vector (
			N1.X + N2.X,
			N1.Y + N2.Y,
			N1.Z + N2.Z)));

	//cout << "OUT: "  << OUT.X << " " << OUT.Y << " " << OUT.Z << endl;

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

	//cout << "OTB: " << to_process.at(0).ID << "  " << IS_OVERTURNED << IS_BEDDING << endl;

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

		bool is_DATA_FOLDSURFACE = is_allowed_foldsurface_processing(to_process.at(0).DATATYPE);

		RANGE_START = RANGE_START + length_container.at(i);

		//cout << "---------------SNG STRART  " << endl;

		bool PROCESSABE = (is_datatype_processable_for_average(to_process));

		bool MT2 = (to_process.size() > 2);

		bool SNG = MT2 && is_plane_dataset_singular (to_process, "AVERAGE");


		//cout << "LOC  " << to_process.at(0).LOC << endl;
		//cout << "---------------proc END  " << PROCESSABE << endl;

		if (FOR_DATA_AVERAGE && PROCESSABE && !SNG) {

			VCTR AV_D = calculate_data_average_vector (to_process, "AVERAGE");

			//cout << to_process.at(0).ID;
			//dbg_averages(to_process);

			to_process = apply_data_average_vector (to_process, AV_D, "DATA_AVERAGE");
		}
		else if (FOR_BEDDING_AVERAGE && PROCESSABE) {

			VCTR AV_D = calculate_bedding_average_vector (to_process);

			//cout << "USE BEDDING DATA" << endl;
			//cout << to_process.at(0).avd.DIPDIR << endl;

			to_process = apply_data_average_vector (to_process, AV_D, "BEDDING_AVERAGE");
		}
		else if (is_DATA_FOLDSURFACE && FOR_FOLD_SURFACE && PROCESSABE && !SNG) {

			VCTR GR_CRC = calculate_data_average_vector (to_process, "FOLDSURFACE");

			//cout << "FOLD CIRCLE: " << flush;
			//cout << fixed << setprecision (3) << flush;
			//cout << GR_CRC.X << '\t' << GR_CRC.Y << '\t' << GR_CRC.Z << endl;

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

	//dbg_averages(outGDB);

	length_container = generate_block_length (outGDB, "BEDDING_AVERAGE");
	outGDB = DATATYPE_AVERAGE (outGDB, length_container, "BEDDING_AVERAGE");

	//dbg_averages(outGDB);

	cout << "  - Data set averages were computed for " << outGDB.size() << " records." << endl;

	return outGDB;
}
