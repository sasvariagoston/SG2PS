// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "angelier.h"
#include "assertions.hpp"
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

vector <GDB> return_stressvector_estimators (STRESSTENSOR st, vector <GDB> inGDB, string method, bool compression_positive) {

	vector <GDB> outGDB = inGDB;

	size_t i = 0.0;

	do {

		outGDB.at(i).SHEAR_S  = return_shearstress  (st, inGDB.at(i), compression_positive);
		outGDB.at(i).NORMAL_S = return_normalstress (st, inGDB.at(i), compression_positive);
		outGDB.at(i).UPSILON  = return_upsilon (st, inGDB.at(i), method, compression_positive);

		outGDB.at(i).ANG  = return_ANG (st, inGDB.at(i), compression_positive);
		outGDB.at(i).RUP  = return_RUP (st, inGDB.at(i), compression_positive);

		if (method == "MOSTAFA")

		outGDB.at(i).lambda =  sqrt(
				outGDB.at(i).SHEAR_S.X * outGDB.at(i).SHEAR_S.X +
				outGDB.at(i).SHEAR_S.Y * outGDB.at(i).SHEAR_S.Y +
				outGDB.at(i).SHEAR_S.Z * outGDB.at(i).SHEAR_S.Z);

		i++;

	} while (i < inGDB.size());

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

void ps_inversion (string method, STRESSTENSOR st, STRESSFIELD sf, vector <GDB> inGDB, vector <VALLEY> V, INPSET inset, ofstream& o, CENTER center, CENTER mohr_center, PAPER P) {

	PS_stressdata (o, center, P, sf, method);
	if (method == "BINGHAM") return;

	PS_stressarrows (o, center, P,  sf);

	if (method == "PTN") 	PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, true);
	else 					PS_mohr_circle (inGDB, o, mohr_center, P, sf, st, false);

	if (method == "ANGELIER" || method == "MOSTAFA" || method == "SHAN" || method == "FRY") {

		PS_RUP_ANG_distribution (inGDB, inset, V, o, center, P, "RUP");
	}

	//PS_RUP_ANG_distribution (inGDB, inset, V, o, center, P, "ANG");

	//PS_ANG_distribution (inGDB, o, center, P);

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
	<< " deg."
	<< endl;
}

vector <GDB> inversion (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, CENTER mohr_center, PAPER P) {

	bool is_ANG = (inset.clustering_RUP_ANG == "A");
	bool is_RUP = (inset.clustering_RUP_ANG == "R");

	STRESSFIELD sf;
	STRESSTENSOR st;

	string method = "";

	if 		(inGDB.at(0).DATATYPE == "FRACTURE" && inset.fracture == "B")	method = "BINGHAM";
	else if (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "F")	method = "FRY";
	else if (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "M")	method = "MICHAEL";
	else if (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "S")	method = "SHAN";
	else if (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "A")	method = "ANGELIER";
	else if (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "O") 	method = "MOSTAFA";
	else if (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "D") 	method = "NDA";
	else if (inGDB.at(0).DATATYPE == "STRIAE" && inset.inversion == "P") 	method = "PTN";
	else {};

	bool successfull = false;
	double average_misfit;
	vector <VALLEY> V;

	if (method == "ANGELIER") {

		st = st_ANGELIER (inGDB, inset);

		sf = sf_ANGELIER (st);
	}

	else if (method == "BINGHAM") {

		st = st_BINGHAM (inGDB);

		sf = sf_BINGHAM (st);
	}

	else if (method == "FRY") {

		if (fry_correct (inGDB, inset)) {

			st = st_FRY (inGDB);

			sf = sf_FRY (st);
		}

		else successfull = false;
	}

	else if (method == "MICHAEL") {

		st = st_MICHAEL (inGDB, inset);

		sf = sf_MICHAEL (st);
	}

	else if (method == "MOSTAFA") {

		sf = sf_MOSTAFA (inGDB, o, inset, center);

		st = st_MOSTAFA ();
	}

	else if (method == "NDA") {

		st = st_NDA (inGDB, inset);

		sf = sf_NDA (st);
	}

	else if (method == "PTN") {

		sf = sf_PTN (inGDB, inset);

		st = st_PTN (sf);
	}

	else if (method == "SHAN") {

		st = st_SHAN (inGDB, inset);

		sf = sf_SHAN (st);
	}

	else {}


	successfull = check_correct_stressfield (sf);


	if 		(method == "MOSTAFA") 							inGDB = return_stressvector_estimators (st, inGDB, "MOSTAFA", false);
	else if (method != "MOSTAFA" && method != "BINGHAM") 	inGDB = return_stressvector_estimators (st, inGDB, "ANGELIER", false);
	else {};



	if (successfull) {

		average_misfit = return_average_misfit (st, inGDB, false);

		if (method == "BINGHAM") bingham_result_output (sf);

		else {

			inversion_result_output (sf, average_misfit);

			if 		(is_ANG) V = return_valleygraph_for_dataset (inGDB, "ANG");
			else if (is_RUP) V = return_valleygraph_for_dataset (inGDB, "RUP");
			else {}

			dbg_cout_V(V);

			if (is_RUP || is_ANG) {

				if (V.size() == 0) 		cout << "    - Cannot cluster input data set using RUP / ANG values." << endl;
				else if (V.size() > 9) 	cout << "    - Clustering result not reliable: more than 9 clusters." << endl;
				else					cout << "    - Input data set separated into " << V.size() + 1 << " clusters." << endl;

				if 		(is_RUP) inGDB = associate_GDB_DATA_clusters (inGDB, V, inset, "RUP");
				else if (is_ANG) inGDB = associate_GDB_DATA_clusters (inGDB, V, inset, "ANG");
				else if (is_RUP) inGDB = associate_GDB_DATA_clusters (inGDB, V, inset, "");


				inGDB = colorcode_grom_groupcode (inGDB);
			}

			PS_idealmovement (inGDB, o, inset, center);
		}

		//dbg_cout_RGF_colors(inGDB);

		ps_inversion (method, st, sf, inGDB, V, inset, o, center, mohr_center, P);

		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S1");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S2");
		PS_lineation (inGDB.at(0), o, inset, center, sf, false, "S3");
	}
	else cout << "unable to compute stress field for the data set." << endl;

	return inGDB;
}
