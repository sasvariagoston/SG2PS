// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>

#include "angelier.h"
#include "assertions.hpp"
#include "allowed_keys.hpp"
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
#include "run_mode.h"
#include "rup_clustering.hpp"
#include "settings.hpp"
#include "shan.h"
#include "stresstensor.hpp"
#include "structs.h"
#include "valley_method.hpp"
#include "yamaji.hpp"

using namespace std;

namespace {

vector <STRESSTENSOR> STV;
vector <STRESSFIELD> SFV;

}

vector <GDB> return_striae_with_offset (const vector <GDB>& inGDB) {

	vector <GDB> outGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const string O = inGDB.at(i).OFFSET;

		if (! is_allowed_striae_none_sense (O)) outGDB.push_back (inGDB.at(i));
	}
	return outGDB;
}

vector <GDB> generate_virtual_striae (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		GDB buf = inGDB.at(i);

		buf.N =  declare_vector (- buf.N.X, - buf.N.Y, buf.N.Z);
		buf.D =  declare_vector (- buf.D.X, - buf.D.Y, buf.D.Z);
		buf.S =  declare_vector (- buf.S.X, - buf.S.Y, buf.S.Z);

		buf.NC = declare_vector (- buf.NC.X, - buf.NC.Y, buf.NC.Z);
		buf.DC = declare_vector (- buf.DC.X, - buf.DC.Y, buf.DC.Z);
		buf.SC = declare_vector (- buf.SC.X, - buf.SC.Y, buf.SC.Z);

		outGDB.push_back(buf);
	}
	outGDB = manipulate_N (outGDB);

	return outGDB;
}

void cout_inversion_results (const vector <GDB>& inGDB, const vector <STRESSFIELD>& SF_V) {

	if (is_mode_DEBUG()) return;

	const bool IS_STRIAE = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);

	if (SF_V.size() < 1) {

		cout << "no stress field was computed for the input data set." << endl;
		return;
	}

	const STRESSFIELD MSF = SF_V.at (SF_V.size() - 1);

	if (is_BINGHAM_USE() && !IS_STRIAE) {

		cout
		<<  fixed << setprecision (0)
		<< "e1: " << setfill ('0') << setw (3) << MSF.S_1.DIPDIR
		<<  "/"   << setfill ('0') << setw (2) << MSF.S_1.DIP << flush;

		cout
		<<  fixed << setprecision (1)
		<< " (" << setfill ('0') << setw (4) << MSF.EIGENVALUE.X * 100.0 << flush;

		cout
		<<  fixed << setprecision (0)
		<< "%), e2: " << setfill ('0') << setw (3) << MSF.S_2.DIPDIR
		<<  "/"       << setfill ('0') << setw (2) << MSF.S_2.DIP << flush;

		cout
		<<  fixed << setprecision (1)
		<< " ("	<< setfill ('0') << setw (4) << MSF.EIGENVALUE.Y * 100.0 << flush;

		cout
		<<  fixed << setprecision (0)
		<< "%), e3: " << setfill ('0') << setw (3) << MSF.S_3.DIPDIR
		<<  "/"       << setfill ('0') << setw (2) << MSF.S_3.DIP << flush;

		cout
		<<  fixed << setprecision (1)
		<< " ("	 << setfill ('0') << setw (4)  << MSF.EIGENVALUE.Z * 100.0 << "%)" << endl;
	}
	else {

		cout << fixed << setprecision (0) << flush;
		cout
		<< "s1: "  	<< setfill ('0') << setw (3)  << MSF.S_1.DIPDIR
		<<  "/"     << setfill ('0') << setw (2)  << MSF.S_1.DIP
		<< ", s2: " << setfill ('0') << setw (3)  << MSF.S_2.DIPDIR
		<<  "/"     << setfill ('0') << setw (2)  << MSF.S_2.DIP
		<< ", s3: " << setfill ('0') << setw (3)  << MSF.S_3.DIPDIR
		<<  "/"     << setfill ('0') << setw (2)  << MSF.S_3.DIP
		<< ", " 	<< flush;

		cout << setfill (' ') << setw (18) << MSF.delvaux_rgm << flush;

		cout << fixed << setprecision (3) << flush;
		cout
		<< ", R: "  << setfill ('0') << setw (4)  << MSF.stressratio
		<< ", R': " << setfill ('0') << setw (4)  << MSF.delvaux_str << flush;

		cout << fixed << setprecision (1) << flush;
		cout
		<< ", av. misfit: " << setfill (' ') << setw (4)  << inGDB.at(0).AV_MISF
		<< " deg." << endl;
	}
}

vector <STRESSTENSOR> return_STV () {

	return STV;
}

vector <STRESSFIELD> return_SFV () {

	return SFV;
}

vector <GDB> ASSOCIATE_STV_SFV (const vector <GDB>& inGDB, const vector <STRESSTENSOR>& ST_V, const vector <STRESSFIELD>& SF_V) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		outGDB.at(i).STV = ST_V;
		outGDB.at(i).SFV = SF_V;
	}
	return outGDB;
}

vector <GDB> apply_inversion_result (const vector <GDB>& inGDB, const STRESSTENSOR st) {

	vector <GDB> outGDB = inGDB;

	const double AV_MISF = return_average_misfit (st, outGDB);

	string METHOD = "ANGELIER";
	if (is_INVERSION_MOSTAFA()) METHOD = "MOSTAFA";

	outGDB = return_stressvector_estimators (st, outGDB, METHOD);

	for (size_t i = 0; i < outGDB.size(); i++) outGDB.at(i).AV_MISF = AV_MISF;

	return outGDB;
}

void INVERSION (const vector <GDB>& inGDB) {

	STV.clear();
	SFV.clear();

	STRESSTENSOR TEST;

	const bool IS_STRIAE = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);

	if (is_INVERSION_ANGELIER() && IS_STRIAE) {

		TEST = st_ANGELIER (inGDB);

		if (! is_stress_tensor_singular (TEST)) {

			STV.push_back (TEST);
			SFV.push_back (sf_ANGELIER (STV.at(0)));
		}
	}
	else if (is_BINGHAM_USE() && !IS_STRIAE) {
		const vector <VCTR> BNG = generate_Bingham_dataset(inGDB);

		TEST = st_BINGHAM (BNG);

		if (! is_stress_tensor_singular (TEST)) {

			STV.push_back (TEST);
			SFV.push_back (sf_BINGHAM (STV.at(0)));
		}
	}
	else if (is_INVERSION_BRUTEFORCE() && IS_STRIAE) {

		TEST = st_BRUTEFORCE (inGDB);

		if (! is_stress_tensor_singular (TEST)) {

			STV.push_back (TEST);
			SFV.push_back (sf_BRUTEFORCE (STV.at(0)));
		}
	}
	else if (is_INVERSION_FRY() && fry_correct (inGDB) && IS_STRIAE) {

		TEST = st_FRY (inGDB);

		if (! is_stress_tensor_singular (TEST)) {

			STV.push_back (TEST);
			SFV.push_back (sf_FRY (STV.at(0)));
		}
	}
	else if (is_INVERSION_MICHAEL() && IS_STRIAE) {

		TEST = st_MICHAEL (inGDB);

		if (! is_stress_tensor_singular (TEST)) {

			STV.push_back (TEST);
			SFV.push_back (sf_MICHAEL(STV.at(0)));
		}
	}
	else if (is_INVERSION_MOSTAFA() && IS_STRIAE) {
		SFV = sfv_MOSTAFA (inGDB);
		STV = stv_MOSTAFA ();
	}
	else if (is_INVERSION_SPRANG() && IS_STRIAE) {

		TEST = st_NDA (inGDB);

		if (! is_stress_tensor_singular (TEST)) {

			STV.push_back (TEST);
			SFV.push_back (sf_NDA (STV.at(0)));
		}
	}
	else if (is_INVERSION_YAMAJI() && IS_STRIAE) {
		//STV.push_back (st_YAMAJI (inGDB));
		//SFV has to be coded
		ASSERT_DEAD_END ();
	}
	else if (is_INVERSION_TURNER() && IS_STRIAE) {

		const STRESSFIELD TEST_SF = sf_PTN (inGDB);

		TEST = st_PTN (TEST_SF, inGDB);

		if (! is_stress_tensor_singular (TEST)) {

			SFV.push_back (TEST_SF);
			STV.push_back (TEST);
		}
	}
	else if (is_INVERSION_SHAN() && IS_STRIAE) {

		TEST = st_SHAN (inGDB);

		if (! is_stress_tensor_singular (TEST)) {

			STV.push_back (st_SHAN(inGDB));
			SFV.push_back (sf_SHAN(STV.at(0)));
		}
	}
	else ASSERT_DEAD_END();

	for (size_t i = 0; i < SFV.size(); i++) {

		const bool successfull = check_correct_stressfield (SFV.at(i));

		if (!successfull) {

			STV.clear();
			SFV.clear();
		}
	}
}

void cout_dbg_sf (const vector <GDB>& inGDB) {

	const STRESSFIELD SF = inGDB.at(0).SFV.at (inGDB.at(0).SFV.size() - 1);

	cout
	<< "EIGENVECTOR1.X" << '\t'	<< "EIGENVECTOR1.Y" << '\t'	<< "EIGENVECTOR1.Z" << '\t'
	<< "EIGENVECTOR2.X" << '\t'	<< "EIGENVECTOR2.Y" << '\t'	<< "EIGENVECTOR2.Z" << '\t'
	<< "EIGENVECTOR3.X" << '\t'	<< "EIGENVECTOR3.Y" << '\t'	<< "EIGENVECTOR3.Z" << '\t'
	<< "EIGENVALUE.X" << '\t'	<< "EIGENVALUE.Y" << '\t'	<< "EIGENVALUE.Z" << '\t'

	<< "S_1.DIPDIR" << '\t'	<< "S_1.DIP" << '\t'
	<< "S_2.DIPDIR" << '\t'	<< "S_2.DIP" << '\t'
	<< "S_3.DIPDIR" << '\t'	<< "S_3.DIP" << '\t'

	<< "stressratio" << '\t' 	<< "delvaux_str" << '\t'
	<< "regime" << '\t'			<< "delvaux_rgm" << '\t'
	<< "shmax" << '\t'			<< "shmin" << '\t'
	<< endl;

	cout << fixed << setprecision(8) << flush;

	cout
	<< SF.EIGENVECTOR1.X << '\t'	<< SF.EIGENVECTOR1.Y << '\t'	<< SF.EIGENVECTOR1.Z << '\t'
	<< SF.EIGENVECTOR2.X << '\t'	<< SF.EIGENVECTOR2.Y << '\t'	<< SF.EIGENVECTOR2.Z << '\t'
	<< SF.EIGENVECTOR3.X << '\t'	<< SF.EIGENVECTOR3.Y << '\t'	<< SF.EIGENVECTOR3.Z << '\t'
	<< SF.EIGENVALUE.X << '\t'		<< SF.EIGENVALUE.Y << '\t'		<< SF.EIGENVALUE.Z << '\t'

	<< SF.S_1.DIPDIR << '\t'	<< SF.S_1.DIP << '\t'
	<< SF.S_2.DIPDIR << '\t'	<< SF.S_2.DIP << '\t'
	<< SF.S_3.DIPDIR << '\t'	<< SF.S_3.DIP << '\t'

	<< SF.stressratio << '\t'	<< SF.delvaux_str << '\t'
	<< SF.regime << '\t'		<< SF.delvaux_rgm << '\t'
	<< SF.shmax << '\t'			<< SF.shmin << '\t'
	<< endl;
}

void cout_dbg_stressfield (const STRESSFIELD& sf) {

	cout << endl;
	cout << "*****************************************" << endl;
	cout << "****    START DUMPING STRESSFIELD    ****" << endl << endl;

	cout << fixed << setprecision(0) << flush;
	cout << "S1: " << sf.S_1.DIPDIR << "/" << sf.S_1.DIP << flush;
	cout << fixed << setprecision(6) << flush;
	cout << " ("   << sf.EIGENVECTOR1.X << ", " << sf.EIGENVECTOR1.Y << ", " << sf.EIGENVECTOR1.Z << ")" << endl;
	cout << fixed << setprecision(0) << flush;
	cout << "S2: " << sf.S_2.DIPDIR << "/" << sf.S_2.DIP << flush;
	cout << fixed << setprecision(6) << flush;
	cout << " ("   << sf.EIGENVECTOR2.X << ", " << sf.EIGENVECTOR2.Y << ", " << sf.EIGENVECTOR2.Z << ")" << endl;
	cout << fixed << setprecision(0) << flush;
	cout << "S3: " << sf.S_3.DIPDIR << "/" << sf.S_3.DIP << flush;
	cout << fixed << setprecision(6) << flush;
	cout << " ("   << sf.EIGENVECTOR3.X << ", " << sf.EIGENVECTOR3.Y << ", " << sf.EIGENVECTOR3.Z << ")" << endl;

	cout << "EIGENVALUES: " << sf.EIGENVALUE.X << ", " << sf.EIGENVALUE.Y << ", " << sf.EIGENVALUE.Z << endl << endl;

	cout << "****    END DUMPING STRESSFIELD    ****" << endl;
	cout << "***************************************" << endl;
}

void cout_dbg_stressfield (const vector <STRESSFIELD>& sf) {

	for (size_t i = 0; i < sf.size(); i++) {

		cout << fixed << setprecision(0) << flush;
		cout << "S1: " << sf.at(i).S_1.DIPDIR << "/" << sf.at(i).S_1.DIP << flush;
		cout << fixed << setprecision(6) << flush;
		cout << " ("   << sf.at(i).EIGENVECTOR1.X << ", " << sf.at(i).EIGENVECTOR1.Y << ", " << sf.at(i).EIGENVECTOR1.Z << ")" << endl;
		cout << fixed << setprecision(0) << flush;
		cout << "S2: " << sf.at(i).S_2.DIPDIR << "/" << sf.at(i).S_2.DIP << flush;
		cout << fixed << setprecision(6) << flush;
		cout << " ("   << sf.at(i).EIGENVECTOR2.X << ", " << sf.at(i).EIGENVECTOR2.Y << ", " << sf.at(i).EIGENVECTOR2.Z << ")" << endl;
		cout << fixed << setprecision(0) << flush;
		cout << "S3: " << sf.at(i).S_3.DIPDIR << "/" << sf.at(i).S_3.DIP << flush;
		cout << fixed << setprecision(6) << flush;
		cout << " ("   << sf.at(i).EIGENVECTOR3.X << ", " << sf.at(i).EIGENVECTOR3.Y << ", " << sf.at(i).EIGENVECTOR3.Z << ")" << endl;

		cout << "EIGENVALUES: " << sf.at(i).EIGENVALUE.X << ", " << sf.at(i).EIGENVALUE.Y << ", " << sf.at(i).EIGENVALUE.Z << ")" << endl;
	}
}

void cout_dbg_stresstensor (const STRESSTENSOR& in) {

	cout << scientific  << setprecision (16) << endl;

	cout << in._11 << "    " << in._12  << "    " << in._13 << endl;
	cout << in._12 << "    " << in._22  << "    " << in._23 << endl;
	cout << in._13 << "    " << in._23  << "    " << in._33 << endl;

	return;
}

void cout_dbg_STV (const vector <STRESSTENSOR>& ST_V) {

	for (size_t i = 0; i < ST_V.size(); i++) {

		STRESSTENSOR A = ST_V.at(i);

		cout << fixed << setprecision (6) << endl;

		cout << A._11 << "    " << A._12  << "    " << A._13 << endl;
		cout << A._12 << "    " << A._22  << "    " << A._23 << endl;
		cout << A._13 << "    " << A._23  << "    " << A._33 << endl;
	}
}
