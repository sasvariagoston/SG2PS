// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>

#include "angelier.h"
#include "assertions.hpp"
#include "brute_force.hpp"
#include "bingham.h"
#include "common.h"
#include "data_io.h"
#include "fry.h"
#include "inversion.h"
#include "michael.h"
#include "mostafa.h"
#include "nda.h"
#include "ps.h"
#include "ptn.h"
#include "rgf.h"
#include "rup_clustering.hpp"
#include "shan.h"
#include "structs.h"
#include "valley_method.hpp"

using namespace std;

bool is_method_BINGHAM (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "FRACTURE" && inset.fracture == "B");
}

bool is_method_FRY (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "F");
}

bool is_method_MICHAEL (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "M");
}

bool is_method_SHAN (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "S");
}

bool is_method_ANGELIER (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "A");
}

bool is_method_MOSTAFA (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "O");
}

bool is_method_NDA (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "D");
}

bool is_method_BRUTEFORCE (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "B");
}

bool is_method_PTN (vector <GDB> inGDB, INPSET inset) {

	return (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "P");
}

size_t useful_striae_number (vector <GDB> inGDB) {

	size_t useful_striae_number = 0;

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (inGDB.at(i).OFFSET != "NONE") useful_striae_number++;
	}

	return useful_striae_number;
}

vector <GDB> return_striae_with_offset (vector <GDB> inGDB) {

	vector <GDB> outGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (inGDB.at(i).OFFSET != "NONE") outGDB.push_back(inGDB.at(i));
	}

	return outGDB;
}

vector <GDB> return_stressvector_estimators (const STRESSTENSOR& st, vector <GDB> inGDB, const string& method, const bool& compression_positive) {

	vector <GDB> outGDB = inGDB;

	for (size_t i  =0; i < inGDB.size(); i++) {

		//cout << "1" << endl;

		outGDB.at(i).SHEAR_S  = return_shearstress  (st, inGDB.at(i), compression_positive);

		//cout << fixed << setprecision(6) << endl;
		//cout << "SHEAR STRESS" << endl;
		//cout
		//<< outGDB.at(i).SHEAR_S.X << '\t'
		//<< outGDB.at(i).SHEAR_S.Y << '\t'
		//<< outGDB.at(i).SHEAR_S.Z << endl;

		//cout << "2" << endl;

		outGDB.at(i).NORMAL_S = return_normalstress (st, inGDB.at(i), compression_positive);

		//cout << "NORMAL STRESS" << endl;
		//cout
		//<< outGDB.at(i).NORMAL_S.X << '\t'
		//<< outGDB.at(i).NORMAL_S.Y << '\t'
		//<< outGDB.at(i).NORMAL_S.Z << endl;



		//cout << "3" << endl;

		outGDB.at(i).UPSILON  = return_upsilon (st, inGDB.at(i), method, compression_positive);

		//cout << "6" << endl;

		outGDB.at(i).ANG  = return_ANG (st, inGDB.at(i), compression_positive);

		//cout << "7" << endl;

		outGDB.at(i).RUP  = return_RUP (st, inGDB.at(i), compression_positive);

		//cout << "8" << endl;

		if (method == "MOSTAFA")

		outGDB.at(i).lambda =  sqrt(
				outGDB.at(i).SHEAR_S.X * outGDB.at(i).SHEAR_S.X +
				outGDB.at(i).SHEAR_S.Y * outGDB.at(i).SHEAR_S.Y +
				outGDB.at(i).SHEAR_S.Z * outGDB.at(i).SHEAR_S.Z);
	}

	return outGDB;
}

vector <GDB> generate_virtual_striae (vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;
	size_t original_set_size = inGDB.size();
	size_t i = 0;

	do {

		GDB buffer = inGDB.at(i);

		buffer.N = declare_vector (- buffer.N.X, - buffer.N.Y, buffer.N.Z);
		buffer.D = declare_vector (- buffer.D.X, - buffer.D.Y, buffer.D.Z);
		buffer.S = declare_vector (- buffer.S.X, - buffer.S.Y, buffer.S.Z);

		buffer.SV = declare_vector (- buffer.SV.X, - buffer.SV.Y, buffer.SV.Z);

		buffer.NC = declare_vector (- buffer.NC.X, - buffer.NC.Y, buffer.NC.Z);
		buffer.DC = declare_vector (- buffer.DC.X, - buffer.DC.Y, buffer.DC.Z);
		buffer.SC = declare_vector (- buffer.SC.X, - buffer.SC.Y, buffer.SC.Z);

		outGDB.push_back(buffer);

		i++;

	} while (i < original_set_size);

	i = 0;

	outGDB =  manipulate_N (outGDB);

	return outGDB;
}

string inversion_method (vector <GDB> inGDB, INPSET inset) {

	bool ANGELIER = 	is_method_ANGELIER(inGDB, inset);
	bool BINGHAM = 		is_method_BINGHAM(inGDB, inset);
	bool BRUTEFORCE = 	is_method_BRUTEFORCE(inGDB, inset);
	bool FRY = 			is_method_FRY(inGDB, inset);
	bool MICHAEL = 		is_method_MICHAEL(inGDB, inset);
	bool MOSTAFA = 		is_method_MOSTAFA(inGDB, inset);
	bool NDA = 			is_method_NDA(inGDB, inset);
	bool PTN = 			is_method_PTN(inGDB, inset);
	bool SHAN = 		is_method_SHAN(inGDB, inset);

	if (!ANGELIER && !BINGHAM && !BRUTEFORCE && !FRY &&!MICHAEL && !MOSTAFA && !NDA && !PTN && !SHAN) ASSERT_DEAD_END();

	if (ANGELIER) return "ANGELIER";
	else if (BINGHAM) return "BINGHAM";
	else if (BRUTEFORCE) return "BRUTEFORCE";
	else if (FRY) return "FRY";
	else if (MICHAEL) return "MICHAEL";
	else if (MOSTAFA) return "MOSTAFA";
	else if (NDA) return "NDA";
	else if (PTN) return "PTN";
	else return "SHAN";
}

void ps_inversion (STRESSTENSOR st, STRESSFIELD sf, vector <GDB> inGDB, vector <VALLEY> V, INPSET inset, ofstream& o, CENTER center, CENTER mohr_center, PAPER P) {

	bool ANGELIER = 	is_method_ANGELIER(inGDB, inset);
	bool BINGHAM = 		is_method_BINGHAM(inGDB, inset);
	bool BRUTEFORCE = 	is_method_BRUTEFORCE(inGDB, inset);
	bool FRY = 			is_method_FRY(inGDB, inset);
	bool MICHAEL = 		is_method_MICHAEL(inGDB, inset);
	bool MOSTAFA = 		is_method_MOSTAFA(inGDB, inset);
	bool NDA = 			is_method_NDA(inGDB, inset);
	bool PTN = 			is_method_PTN(inGDB, inset);
	bool SHAN = 		is_method_SHAN(inGDB, inset);

	if (!ANGELIER && !BINGHAM && !BRUTEFORCE && !FRY &&!MICHAEL && !MOSTAFA && !NDA && !PTN && !SHAN) ASSERT_DEAD_END();

	string method = inversion_method (inGDB, inset);
	PS_stressdata (o, center, P, sf, method);

	if (BINGHAM) return;
	else {}

	PS_stressarrows (o, center, P,  sf);

	if (PTN)	PS_mohr_circle (inGDB, o, inset, mohr_center, P, sf, st, true);
	else 		PS_mohr_circle (inGDB, o, inset, mohr_center, P, sf, st, false);

	if (ANGELIER || MOSTAFA || SHAN || FRY) PS_RUP_ANG_distribution (inGDB, inset, V, o, center, P, "RUP");
	else {}

	PS_RUP_ANG_distribution (inGDB, inset, V, o, center, P, "ANG");

	PS_stress_state (o, P, center, sf);

	sf = stressvector_to_DXDYDZ (sf);
}

void bingham_result_output (STRESSFIELD sf) {

	cout
	<< "e1: "   	<< setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_1.DIPDIR
	<<  "/"     	<< setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_1.DIP
	<< " (" 		<< setfill ('0') << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.X * 100.0
	<< "%), e2: " 	<< setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_2.DIPDIR
	<<  "/"     	<< setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_2.DIP
	<< " ("			<< setfill ('0') << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Y * 100.0
	<< "%), e3: " 	<< setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_3.DIPDIR
	<<  "/"     	<< setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_3.DIP
	<< " ("			<< setfill ('0') << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Z * 100.0
	<< "%)"
	<< endl;
}

void inversion_result_output (STRESSFIELD sf, double average_misfit) {

	cout
	<< "s1: "   		<< setfill ('0') << setw (3)  << fixed << setprecision (0) << sf.S_1.DIPDIR
	<<  "/"     		<< setfill ('0') << setw (2)  << fixed << setprecision (0) << sf.S_1.DIP
	<< ", s2: " 		<< setfill ('0') << setw (3)  << fixed << setprecision (0) << sf.S_2.DIPDIR
	<<  "/"     		<< setfill ('0') << setw (2)  << fixed << setprecision (0) << sf.S_2.DIP
	<< ", s3: " 		<< setfill ('0') << setw (3)  << fixed << setprecision (0) << sf.S_3.DIPDIR
	<<  "/"     		<< setfill ('0') << setw (2)  << fixed << setprecision (0) << sf.S_3.DIP
	<< ", " 			<< setfill (' ') << setw (18) << sf.delvaux_rgm
	<< ", R: "  		<< setfill ('0') << setw (4)  << fixed << setprecision (3) << sf.stressratio
	<< ", R': " 		<< setfill ('0') << setw (4)  << fixed << setprecision (3) << sf.delvaux_str
	<< ", av. misfit: " << setfill (' ') << setw (4)  << fixed << setprecision (1) << average_misfit
	<< " deg." << endl;
}

vector <GDB> inversion (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, CENTER mohr_center, PAPER P, bool tilt) {

	bool is_ANG = (inset.clustering_RUP_ANG == "A");
	bool is_RUP = (inset.clustering_RUP_ANG == "R");

	STRESSFIELD sf;
	STRESSTENSOR st;

	bool ANGELIER = 	is_method_ANGELIER(inGDB, inset);
	bool BINGHAM = 		is_method_BINGHAM(inGDB, inset);
	bool BRUTEFORCE = 	is_method_BRUTEFORCE(inGDB, inset);
	bool FRY = 			is_method_FRY(inGDB, inset);
	bool MICHAEL = 		is_method_MICHAEL(inGDB, inset);
	bool MOSTAFA = 		is_method_MOSTAFA(inGDB, inset);
	bool NDA = 			is_method_NDA(inGDB, inset);
	bool PTN = 			is_method_PTN(inGDB, inset);
	bool SHAN = 		is_method_SHAN(inGDB, inset);

	if (!ANGELIER && !BINGHAM && !BRUTEFORCE && !FRY && !MICHAEL && !MOSTAFA && !NDA && !PTN && !SHAN) ASSERT_DEAD_END();

	bool successfull = false;
	double average_misfit;
	vector <VALLEY> V;

	if (ANGELIER) {

		st = st_ANGELIER (inGDB, inset);

		sf = sf_ANGELIER (st);

		sf = computestressfield_DXDYDZ (sf);

		sf =  stress_regime (sf);
	}
	else if (BINGHAM) {

		st = st_BINGHAM (inGDB);

		sf = eigenvalue_eigenvector (st);

		sf = sf_BINGHAM (sf);

		sf = computestressfield_DXDYDZ (sf);
	}
	else if (FRY) {

		if (fry_correct (inGDB, inset)) {

			st = st_FRY (inGDB);

			sf = sf_FRY (st);

			sf = computestressfield_DXDYDZ (sf);

			sf = stress_regime (sf);
		}
		else successfull = false;
	}
	else if (MICHAEL) {

		st = st_MICHAEL (inGDB, inset);

		sf = sf_MICHAEL (st);

		sf = computestressfield_DXDYDZ (sf);

		sf = stress_regime (sf);
	}
	else if (MOSTAFA) {

		sf = sf_MOSTAFA (inGDB, o, inset, center);

		st = st_MOSTAFA ();
	}
	else if (NDA) {

		st = st_NDA (inGDB, inset);

		sf = sf_NDA (st);

		sf = computestressfield_DXDYDZ (sf);

		sf =  stress_regime (sf);
	}
	else if (BRUTEFORCE) {

		VCTR ORIGO = declare_vector(0.0, 0.0, 1.0);
		size_t POINTS_DISTANCE = 9;
		vector <VCTR> CNTRVCTR = generate_centroids_net (ORIGO, POINTS_DISTANCE, inset);
		//NUMERICALLY OK

		double ANG_MIN = 0.0;
		double ANG_MAX = 180.0;
		vector <double> ANGVCTR = generate_angle_vector_180 (ANG_MIN, ANG_MAX, 18);
		//NUMERICALLY OK

		double PHI_MIN = 0.0;
		double PHI_MAX = 1.0;
		vector <double> PHIVCTR = generate_phi_vector (PHI_MIN, PHI_MAX, 10);
		//NUMERICALLY OK

		vector <BRUTEFORCE_RESULT> BR_RAW = BRUTEFORCE_ENGINE (inGDB, CNTRVCTR, ANGVCTR, PHIVCTR, inset);

		BR_RAW = return_minimum_misfits (BR_RAW, 10);


		for (size_t i = 0; i < BR_RAW.size(); i++) {

			o
			<< BR_RAW.at(i).NRM.X << '\t'
			<< BR_RAW.at(i).NRM.Y << '\t'
			<< BR_RAW.at(i).NRM.Z << '\t'
			<< BR_RAW.at(i).ANG << '\t'
			<< BR_RAW.at(i).PHI << '\t'
			<< BR_RAW.at(i).MISFIT << endl;
		}



		exit (1);

		vector <BRUTEFORCE_RESULT> BR_FINAL;

		for (size_t i = 0; i < BR_RAW.size(); i++) {

			string COUNT = int_to_string(i);

			ostringstream filename;

			filename << "BRUTEFORCE_" << i << ".TXT";

			ofstream o(filename.str().c_str());

			o
			<< "NRM.X" << '\t'
			<< "NRM.Y" << '\t'
			<< "NRM.Z" << '\t'
			<< "ANG" << '\t'
			<< "PHI" << '\t'
			<< "MISFIT" << endl;

			VCTR ORIGO = BR_RAW.at(i).NRM;
			POINTS_DISTANCE = 1;
			vector <VCTR> CNTRVCTR = generate_centroids_net(ORIGO, POINTS_DISTANCE, inset);

			ANG_MIN = BR_RAW.at(i).ANG - 7.0;
			ANG_MAX = BR_RAW.at(i).ANG + 7.0;
			if (ANG_MIN < 0.0) ANG_MIN = 0.0;
			if (ANG_MAX > 180.0) ANG_MAX = 180.0;
			vector <double> ANGVCTR = generate_angle_vector_180 (ANG_MIN, ANG_MAX, 14);

			PHI_MIN = BR_RAW.at(i).PHI - 0.07;
			PHI_MAX = BR_RAW.at(i).PHI + 0.07;
			if (PHI_MIN < 0.0) PHI_MIN = 0.0;
			if (PHI_MAX > 1.0) PHI_MAX = 1.0;
			vector <double> PHIVCTR = generate_phi_vector (PHI_MIN, PHI_MAX, 10);

			vector <BRUTEFORCE_RESULT> BR_FINE = BRUTEFORCE_ENGINE (inGDB, CNTRVCTR, ANGVCTR, PHIVCTR, inset);


			for (size_t k = 0; k < BR_FINE.size(); k++) {

				o
				<< BR_FINE.at(k).NRM.X << '\t'
				<< BR_FINE.at(k).NRM.Y << '\t'
				<< BR_FINE.at(k).NRM.Z << '\t'
				<< BR_FINE.at(k).ANG << '\t'
				<< BR_FINE.at(k).PHI << '\t'
				<< BR_FINE.at(k).MISFIT << '\n';
			}

			BR_FINE = return_minimum_misfits (BR_FINE, 1);

			BRUTEFORCE_RESULT buf = BR_FINE.at(0);

			BR_FINAL.push_back(buf);
		}

		//exit (1);


	//	st._11 =  0.222668;
	//	st._12 =  0.128558;
	//	st._13 = -0.906418;
	//	st._22 =  0.127125;
	//	st._23 = -0.222668;
	//	st._33 = -0.128558;

		sf = eigenvalue_eigenvector (st);

		sf = computestressfield_DXDYDZ (sf);

		sf =  stress_regime (sf);
	}

	else if (PTN) {

		sf = sf_PTN (inGDB, inset);

		st = st_PTN (sf);
	}
	else if (SHAN) {

		st = st_SHAN (inGDB, inset);

		sf = sf_SHAN (st);

		sf = computestressfield_DXDYDZ (sf);

		sf = stress_regime (sf);
	}
	else ASSERT_DEAD_END()



	cout << "1" << endl;
	successfull = check_correct_stressfield (sf);
	cout << "2" << endl;

	if 		(MOSTAFA) 				inGDB = return_stressvector_estimators (st, inGDB, "MOSTAFA", false);
	else if (!MOSTAFA && !BINGHAM) 	inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
	else {};

	cout << "3" << endl;



	if (successfull) {

		//cout << "3" << endl;

		average_misfit = return_average_misfit (st, inGDB, false);

		//cout << "31" << endl;

		if (BINGHAM) bingham_result_output (sf);

		else {

			inversion_result_output (sf, average_misfit);

			if 		(is_ANG) V = return_valleygraph_for_dataset (inGDB, "ANG");
			else if (is_RUP) V = return_valleygraph_for_dataset (inGDB, "RUP");
			else {}

			if (V.size() == 1 && V.at(0).DIR == "X") V.clear();

			if (is_RUP || is_ANG) {

				if 		(V.size() == 0) cout << "    - Cannot cluster input data set using RUP / ANG values." << endl;
				else if (V.size() > 9) 	cout << "    - Clustering result not reliable: more than 9 clusters." << endl;
				else					cout << "    - Input data set separated into " << V.size() + 1 << " clusters." << endl;

				if 		(is_RUP) inGDB = associate_GDB_DATA_clusters (inGDB, V, inset, "RUP");
				else if (is_ANG) inGDB = associate_GDB_DATA_clusters (inGDB, V, inset, "ANG");
				else    		 inGDB = associate_GDB_DATA_clusters (inGDB, V, inset, "");
			}

			PS_idealmovement (inGDB, o, inset, center);
		}

		process_one_by_one (inGDB, o, inset, center, P, tilt);

		ps_inversion (st, sf, inGDB, V, inset, o, center, mohr_center, P);

		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S1");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S2");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S3");
	}
	else cout << "unable to compute stress field for the data set." << endl;

	//cout << "3" << endl;

	return inGDB;
}
