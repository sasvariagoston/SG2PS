
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "average.hpp"
#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "bingham.h"
#include "brute_force.hpp"
#include "checkrgffilecontent.h"
#include "checkxycontent.h"
#include "color_management.hpp"
#include "data_io.h"
#include "data_sort.hpp"
#include "filename.hpp"
#include "ps.h"
#include "rakhmanov.hpp"
#include "random.hpp"
#include "retilt.hpp"
#include "rgf.h"
#include "run_mode.h"
#include "settings.hpp"
#include "standard_output.hpp"
#include "well.hpp"
#include "well_ps.hpp"

using namespace std;

namespace {

string INPUTFILENAME = "";

const double SN = 10e-8;

bool PROCESS_AS_TILTED = false;
bool PROCESS_AS_TRAJECTORY = false;

vector <vector <vector <GDB> > > MASTER_GDB;

}

bool is_PROCESS_AS_TILTED () {

	return PROCESS_AS_TILTED;
}

bool is_PROCESS_AS_TRAJECTORY () {

	return PROCESS_AS_TRAJECTORY;
}

string return_inputfilename () {

	return INPUTFILENAME;
}

void set_inputfilename (const string filename) {

	INPUTFILENAME = filename;

	return;
}

vector <GDB> competeRGFcontect (const string projectname) {

	vector <GDB> outGDB = create_GDB_from_rgf (projectname + ".rgf");

	for (size_t i = 0; i < outGDB.size(); i++) {

		const string GC = outGDB.at(i).GC;

		if (GC.size() == 0) outGDB.at(i).GC = "X";
		else if (GC.size() == 1) {}
		else if (GC.size() == 3) {

			if (is_INPUTGROUP_FIRST()) 			outGDB.at(i).GC = char_to_string(GC.at(0));
			else if (is_INPUTGROUP_SECOND()) 	outGDB.at(i).GC = char_to_string(GC.at(1));
			else if (is_INPUTGROUP_THIRD()) 	outGDB.at(i).GC = char_to_string(GC.at(2));
			else outGDB.at(i).GC = "X";
		}
		else ASSERT_DEAD_END();

		if (is_DATARULE_RIGHT_HAND_RULE()) outGDB.at(i).corr.DIPDIR = right_hand_rule_to_german (outGDB.at(i).corr.DIPDIR);

		//if (capslock(inputxyfilename) != "NONE")  outGDB.at(i) = insertxy (outGDB.at(i));
	}
	return outGDB;
}

double right_hand_rule_to_german (const double corrDIPDIR) {

	if ((corrDIPDIR >= 0.0) && (corrDIPDIR < 270.0)) 	return corrDIPDIR + 90.0;
	else 												return corrDIPDIR - 270.0;
}

double german_to_right_hand_rule (const double corrDIPDIR) {

	if ((corrDIPDIR > 90.0) && (corrDIPDIR <= 360.0)) 	return corrDIPDIR - 90.0;
	else 												return corrDIPDIR + 270.0;
}

vector <GDB> fix_360_0 (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const double DD = inGDB.at(i).corr.DIPDIR;
		const double D  = inGDB.at(i).corr.DIP;

		if (DD == 360.0) outGDB.at(i).corr.DIPDIR = 360.0 - SN;
		if (DD ==   0.0) outGDB.at(i).corr.DIPDIR = SN;

		if (D == 90.0) outGDB.at(i).corr.DIP = 90.0 - SN;
		if (D ==  0.0) outGDB.at(i).corr.DIP = SN;
	}
	return outGDB;
}

vector <GDB> generate_NDS_vectors (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	const string DT = outGDB.at(0).DATATYPE;
	const bool L = is_allowed_lithology_datatype(DT);
	const bool B = is_allowed_handle_as_bedding(DT);

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (!L) {

			const string OF = outGDB.at(i).OFFSET;
			const bool O = is_allowed_bedding_overturned_sense (OF);

			if (O && !B) ASSERT_DEAD_END();

			const DIPDIR_DIP DDD = outGDB.at(i).corr;

			VCTR N = NXNYNZ_from_dipdir_dip (DDD);

			if (O && B) N = flip_vector(N);

			outGDB.at(i).N = N;

			VCTR D = DXDYDZ_from_dipdir_dip (DDD);
			if (O && B) D = flip_vector(D);
			outGDB.at(i).D = D;

			outGDB.at(i).S = crossproduct(N, D);
		}
	}
	return outGDB;
}

vector <GDB> generate_NCDCSC_vectors (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < inGDB.size(); i++) {

		const bool STRIAE = is_allowed_striae_datatype(outGDB.at(i).DATATYPE);

		const bool SC = 		outGDB.at(i).LINEATION == "SC";
		const bool PITCH = 		outGDB.at(i).LINEATION == "PITCH";
		const bool LINEATION = 	outGDB.at(i).LINEATION == "LINEATION";

		if (STRIAE || SC) {

			if (SC || LINEATION) outGDB.at(i) = generate_NCDCSC_LINEATION_SC (outGDB.at(i));
			else if (PITCH) 	 outGDB.at(i) = generate_NCDCSC_PITCH (outGDB.at(i));
			else ASSERT_DEAD_END();
		}
	}
	return outGDB;
}

GDB generate_NCDCSC_LINEATION_SC (const GDB& inGDB) {

	GDB outGDB = inGDB;

	outGDB.NC = NXNYNZ_from_dipdir_dip (outGDB.corrL);

	const bool NCX_EQ_NX (is_in_range (outGDB.NC.X, outGDB.NC.X, outGDB.N.X));
	const bool NCY_EQ_NY (is_in_range (outGDB.NC.Y, outGDB.NC.Y, outGDB.N.Y));
	const bool NCZ_EQ_NZ (is_in_range (outGDB.NC.Z, outGDB.NC.Z, outGDB.N.Z));

	if (NCX_EQ_NX && NCY_EQ_NY && NCZ_EQ_NZ) outGDB.N.X = outGDB.N.X + SN;

	outGDB.DC = DXDYDZ_from_dipdir_dip (outGDB.corrL);

	const bool DCX_EQ_DX (is_in_range (outGDB.DC.X, outGDB.DC.X, outGDB.D.X));
	const bool DCY_EQ_DY (is_in_range (outGDB.DC.Y, outGDB.DC.Y, outGDB.D.Y));
	const bool DCZ_EQ_DZ (is_in_range (outGDB.DC.Z, outGDB.DC.Z, outGDB.D.Z));

	if (DCX_EQ_DX && DCY_EQ_DY && DCZ_EQ_DZ) outGDB.D.X = outGDB.D.X + SN;

	outGDB.SC = crossproduct (outGDB.NC, outGDB.DC);

	return outGDB;
}

GDB generate_NCDCSC_PITCH (const GDB& inGDB) {

	GDB outGDB = inGDB;

	const string PITCH = outGDB.LPITCHSENSE;
	double GEOD_DIR = NaN();

	const vector <string> GDV = allowed_geodetic_vector();
	const vector <double> GDA = geodetic_angle_vector();

	if (GDV.size() != GDA.size()) ASSERT_DEAD_END();

	for (size_t i = 0; i < GDV.size(); i++) if (PITCH == GDV.at(i)) GEOD_DIR = GDA.at(i);

	const VCTR GEODETIC = unitvector (declare_vector(SIN (GEOD_DIR), COS (GEOD_DIR), 0.0));

	const double ANGLE = outGDB.LPITCH;

	const bool ROT_RIGHT = vector_angle(GEODETIC, outGDB.S) > 90.0;

	if (ROT_RIGHT) 	outGDB.DC = ROTATE (outGDB.N, outGDB.S, - 180.0 + ANGLE);
	else 			outGDB.DC = ROTATE (outGDB.N, outGDB.S, - ANGLE);

	outGDB.corrL = dipdir_dip_from_DXDYDZ (outGDB.DC);

	outGDB.NC = NXNYNZ_from_dipdir_dip (outGDB.corrL);

	outGDB.SC = crossproduct (outGDB.NC, outGDB.DC);

	outGDB.PITCHANGLE = outGDB.LPITCH;

	return outGDB;
}

vector <GDB> manipulate_N (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	uniform_generator_reset();

	for (size_t i = 0; i < outGDB.size(); i++) {

		const string DT = outGDB.at(i).DATATYPE;

		const bool LITHOLOGY = 	is_allowed_lithology_datatype(DT);
		const bool PLANE = 		is_allowed_plane_datatype(DT);
		const bool LINEATION = 	is_allowed_lineation_datatype(DT);
		const bool STRIAE = 	is_allowed_striae_datatype(DT);
		const bool SC = 		is_allowed_SC_datatype(DT);

		if (!LITHOLOGY && !PLANE && !LINEATION && !STRIAE && !SC) ASSERT_DEAD_END();

		const double a = uniform_0_1();
		const double S = a * 0.01;

		if (a <= 0.33333) {

			if (outGDB.at(i).N.X > 0.5) 	outGDB.at(i).N.X = outGDB.at(i).N.X - S;
			else 							outGDB.at(i).N.X = outGDB.at(i).N.X + S;

			if (outGDB.at(i).NC.X > 0.5) 	outGDB.at(i).NC.X = outGDB.at(i).NC.X - S;
			else 							outGDB.at(i).NC.X = outGDB.at(i).NC.X + S;
		}
		else if  (a >= 0.6666) {

			if (outGDB.at(i).N.Z > 0.5) 	outGDB.at(i).N.Z = outGDB.at(i).N.Z - S;
			else 							outGDB.at(i).N.Z = outGDB.at(i).N.Z + S;

			if (outGDB.at(i).NC.Z > 0.5) 	outGDB.at(i).NC.Z = outGDB.at(i).NC.Z - S;
			else 							outGDB.at(i).NC.Z = outGDB.at(i).NC.Z + S;
		}
		else {

			if (outGDB.at(i).N.Y > 0.5) 	outGDB.at(i).N.Y = outGDB.at(i).N.Y - S;
			else 							outGDB.at(i).N.Y = outGDB.at(i).N.Y + S;

			if (outGDB.at(i).NC.Y > 0.5) 	outGDB.at(i).NC.Y = outGDB.at(i).NC.Y - S;
			else 							outGDB.at(i).NC.Y = outGDB.at(i).NC.Y + S;
		}

		if (!LITHOLOGY) {

			outGDB.at(i).N = unitvector (outGDB.at(i).N);

			if (SC || STRIAE) outGDB.at(i).NC = unitvector (outGDB.at(i).NC);
		}
	}
	return outGDB;
}

vector <GDB> generate_MISFIT (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		const bool LINEATION = outGDB.at(i).LINEATION == "LINEATION";

		if (LINEATION) {

			const VCTR N  = outGDB.at(i).N;
			const VCTR DC = outGDB.at(i).DC;

			const double MSF = vector_angle(N, DC);

			outGDB.at(i).MISFIT = 90.0 - fabs(MSF);
		}
	}
	return outGDB;
}

vector <GDB> striae_correction (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	const string DT = outGDB.at(0).DATATYPE;
	const bool STRIAE = is_allowed_striae_datatype(DT);

	if (! STRIAE) return outGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		const bool LINEATION = outGDB.at(i).LINEATION == "LINEATION";

		const VCTR N  = outGDB.at(i).N;
		const VCTR NC = outGDB.at(i).NC;
		const VCTR SC = outGDB.at(i).SC;
		const VCTR DC = outGDB.at(i).DC;

		const bool SC_N_has_same_strike = is_in_range(0.9999, 1.0001, fabs (dotproduct (SC, N)));
		const bool NC_N_has_same_strike = is_in_range(0.9999, 1.0001, fabs (dotproduct (NC, N)));

		if (LINEATION && !SC_N_has_same_strike && !NC_N_has_same_strike) {

			const VCTR TEST_DIPDIR = 	striae_DIPDIR_correction (outGDB.at(i));
			const VCTR TEST_DIP = 		striae_DIP_correction (outGDB.at(i));

			double misfit_DIPDIR_corr = fabs (vector_angle (TEST_DIPDIR, DC));
			double misfit_DIP_corr = 	fabs (vector_angle (TEST_DIP, DC));

			if (misfit_DIPDIR_corr < misfit_DIP_corr) outGDB.at(i).DC = TEST_DIPDIR;
			else outGDB.at(i).DC = TEST_DIP;

			outGDB.at(i).corrL = dipdir_dip_from_DXDYDZ(outGDB.at(i).DC);

			outGDB.at(i).NC = NXNYNZ_from_DXDYDZ(outGDB.at(i).DC);

			outGDB.at(i).SC = crossproduct (outGDB.at(i).NC, outGDB.at(i).DC);
		}
	}
	return outGDB;
}

VCTR striae_DIP_correction (const GDB& in) {

	GDB PROC = in;

	bool NX_EQ_SCX = is_in_range (PROC.N.X, PROC.N.X, PROC.SC.X);
	bool NY_EQ_SCY = is_in_range (PROC.N.Y, PROC.N.Y, PROC.SC.Y);
	bool NZ_EQ_SCZ = is_in_range (PROC.N.Z, PROC.N.Z, PROC.SC.Z);

	if (NX_EQ_SCX && NY_EQ_SCY && NZ_EQ_SCZ) PROC.N.X = PROC.N.X + 10e-8;

	VCTR STR = crossproduct (PROC.SC, PROC.N);

	STR = unitvector(STR);

	if (STR.Z < 0.0) return STR;
	return flip_vector(STR);
}

VCTR striae_DIPDIR_correction (const GDB& in) {

	GDB PROC = in;

	bool NX_EQ_NCX = is_in_range (PROC.N.X, PROC.N.X, PROC.NC.X);
	bool NY_EQ_NCY = is_in_range (PROC.N.Y, PROC.N.Y, PROC.NC.Y);
	bool NZ_EQ_NCZ = is_in_range (PROC.N.Z, PROC.N.Z, PROC.NC.Z);

	if (NX_EQ_NCX && NY_EQ_NCY && NZ_EQ_NCZ) PROC.N.X = PROC.N.X + 10e-8;

	VCTR STR = crossproduct (in.N, in.NC);

	STR = unitvector(STR);

	if (STR.Z < 0.0) return STR;
	return flip_vector(STR);
}

bool is_D_up (const VCTR& D) {

	return (D.Z > 0.0);
}

bool is_DC_up (const VCTR& DC) {

	return (DC.Z > 0.0);
}

bool is_N_down (const VCTR& N) {

	return (N.Z < 0.0);
}

vector <GDB> generate_UP (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		GDB ACT = outGDB.at(i);

		const bool STRIAE = is_allowed_striae_datatype (ACT.DATATYPE);

		const bool NON = is_allowed_striae_none_sense (ACT.OFFSET);
		const bool SIN = is_allowed_striae_sinistral_sense (ACT.OFFSET);
		const bool DXT = is_allowed_striae_dextral_sense (ACT.OFFSET);
		const bool NRM = is_allowed_striae_normal_sense (ACT.OFFSET);

		if (STRIAE && !NON) {

			const VCTR TEST  = crossproduct(ACT.D, ACT.DC);

			const bool p = (TEST.Z > 0.0);

			bool UP = true;

			if (NRM || (p && SIN) || (!p && DXT)) UP = false;

			if (UP) {

				ACT.DC = flip_vector (ACT.DC);
				ACT.NC = flip_vector (ACT.NC);
				ACT.SC = crossproduct(ACT.NC, ACT.DC);
			}
		}
		outGDB.at(i) = ACT;
	}
	return outGDB;
}

vector <GDB> generate_PITCHANGLE (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	const bool STRIAE = is_allowed_striae_datatype (outGDB.at(0).DATATYPE);

	for (size_t i = 0; i < outGDB.size(); i++) {

		const bool NON = is_allowed_striae_none_sense(outGDB.at(i).OFFSET);

		if (STRIAE && !NON) {

			const VCTR S = outGDB.at(i).S;
			const VCTR DC = outGDB.at(i).DC;
			const bool UP = is_DC_up(DC);

			if (UP) outGDB.at(i).PITCHANGLE = vector_angle(S, flip_vector(DC));
			else outGDB.at(i).PITCHANGLE = vector_angle(S, DC);
		}
	}
	return outGDB;
}

string return_new_offset (const double DIP, const double PTC, const bool UP) {

	const bool STEEP_DIP = DIP >= 15.0;

	string OFFSET = "NORMAL";
	if (UP) OFFSET = "REVERSE";

	if (STEEP_DIP && is_in_range (0.0, 45.0, PTC)) {

		if (UP) OFFSET = "DEXTRAL";
		else OFFSET = "SINISTRAL";
	}

	if (STEEP_DIP && is_in_range (135.0, 180.0, PTC)) {

		if (UP) OFFSET = "SINISTRAL";
		else OFFSET = "DEXTRAL";
	}
	return OFFSET;
}

void ASSERT_OFFSET (const string OR_OFS, const string NW_OFS) {

	const bool OS = is_allowed_striae_sinistral_sense(OR_OFS);
	const bool NS = is_allowed_striae_sinistral_sense(NW_OFS);

	const bool OD = is_allowed_striae_dextral_sense(OR_OFS);
	const bool ND = is_allowed_striae_dextral_sense(NW_OFS);

	if ((OS && ND) || (OD && NS)) ASSERT_DEAD_END();

	return;
}

vector <GDB> generate_OFFSET (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		const GDB ACT = outGDB.at(i);

		bool STRIAE = is_allowed_striae_datatype (ACT.DATATYPE);

		const bool NON = is_allowed_striae_none_sense(ACT.OFFSET);

		if (STRIAE && !NON) {

			const string ORIG_OFFSET = ACT.OFFSET;

			const double DIP = ACT.corr.DIP;
			const double PTC = ACT.PITCHANGLE;
			const bool UP = is_DC_up(ACT.DC);

			const string NEW_OFFSET = return_new_offset (DIP, PTC, UP);

			if (NEW_OFFSET != ORIG_OFFSET) outGDB.at(i).OFFSET = NEW_OFFSET;

			ASSERT_OFFSET (ORIG_OFFSET, NEW_OFFSET);
		}
	}
	return outGDB;
}

vector <GDB> generate_LAMBDA_STRESSVECTOR_ESTIMATORS (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		const bool STRIAE = is_allowed_striae_datatype (outGDB.at(i).DATATYPE);

		if (STRIAE) outGDB.at(i).lambda = sqrt(3.0) / 2.0;
	}
	return outGDB;
}

vector <vector <GDB> > clustering_GBD (const vector <vector <GDB> >& inGDB_G) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	outGDB_G = associate_empty_clustercode (outGDB_G, 2);

	if (is_CLUSTERING_NONE() || is_CLUSTERNUMBER() == "1") return outGDB_G;

	if (!is_mode_DEBUG()) cout << "K-MEANS CLUSTERING OF INPUT DATABASE FILE" << endl;

	for (size_t i = 0; i < outGDB_G.size(); i++) {

		vector <GDB> process_GDB = outGDB_G.at(i);

		const bool LITHOLOGY = is_allowed_lithology_datatype(process_GDB.at(0).DATATYPE);

		if (process_GDB.size() > 1 && !LITHOLOGY) outGDB_G.at(i) = K_MEANS (process_GDB);
	}
	return outGDB_G;
}

vector <GDB>  PREPARE_GDB_FOR_PROCESSING (const vector <GDB>& inGDB, const bool TILT) {

	vector <GDB> outGDB = inGDB;

	if (!TILT) {

		outGDB = fix_360_0 (outGDB);

		outGDB = generate_NDS_vectors (outGDB);

		outGDB = generate_NCDCSC_vectors (outGDB);

		outGDB = generate_LAMBDA_STRESSVECTOR_ESTIMATORS (outGDB);

		outGDB = generate_MISFIT (outGDB);

		outGDB = striae_correction (outGDB);

		outGDB = generate_UP (outGDB);
	}
	outGDB = generate_PITCHANGLE (outGDB);

	outGDB = generate_OFFSET (outGDB);

	return outGDB;
}

vector < vector <GDB> > PREPARE_GDB_VECTOR_FOR_PROCESSING (const vector < vector <GDB> >& inGDB_G, const bool TILT) {

	if (!is_mode_DEBUG()) cout << "  - Geodatabase processing of the input file" << endl;

	vector <vector <GDB> > outGDB_G = inGDB_G;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		vector <GDB> process_GDB = SORT_GDB (inGDB_G.at(i), "IID");

		process_GDB = PREPARE_GDB_FOR_PROCESSING (process_GDB, TILT);

		outGDB_G.at(i) = process_GDB;
	}
	return outGDB_G;
}

vector <vector <GDB> > EVALUATE (const vector <vector <GDB> >& inGDB_G) {

	vector <vector <GDB> > P = inGDB_G;

	const bool TLT = PROCESS_AS_TILTED;
	const bool TRJ = PROCESS_AS_TRAJECTORY;

	writeln ("");
	writeln ("-------------------------------------------------------");

	string msg = "- Data evaluation for the";

	if (TLT) msg = msg + " tilted data set ";
	else msg = msg + " original data set ";

	if (TRJ) msg = msg + "with trajectory correction.";
	else msg = msg + "without trajectory correction.";

	if (!is_mode_DEBUG()) writeln (msg);

	if (TRJ) P = RETILT (P, "TRAJECTORY");

	if (TLT) {

		P = RETILT (P, "BEDDING");
		P = RETILT (P, "PALEONORTH");
	}

	if (TRJ || TLT) {

		P = PREPARE_GDB_VECTOR_FOR_PROCESSING (P, true);
		P = AVERAGE (P);
		P = ASSOCIATE_AVERAGE_BEDDING_GROUPS (P);
	}

	P = PROCESS_GROUPS (P, TLT);

	vector <GDB> p = MERGE_GROUPS_TO_GDB (P);

	p = GENERATE_PS_CODE (p);

	p = SORT_GDB (p, "IID");

	P = SEPARATE_DATASET_GROUPS (p);

	if (is_GROUPSEPARATION_IGNORE()) {}
	else if (is_GROUPSEPARATION_GROUPCODE()) 	P = SEPARATE_DATASET (P, "GROUPS", "GROUPCODE");//ez nem kell az alapfugvenybe!
	else if (is_GROUPSEPARATION_KMEANS()) 		P = SEPARATE_DATASET (P, "CLUSTER", "CLUSTER");
	else if (is_GROUPSEPARATION_RUPANG()) 		P = SEPARATE_DATASET (P, "RUP_ANG", "RUP_ANG");
	else ASSERT_DEAD_END();

	STANDARD_OUTPUT (p);

	PROCESS_WELL_GROUPS (P);

	return P;
}

void PROCESS_RGF (const string inputfilename) {

	writeln ("");
	writeln ("===========================");
	writeln ("6) PROCESSING DATA FILE(S)");
	writeln ("===========================");
	writeln ("");

	set_inputfilename (inputfilename);

	INIT_DEBUG();

	GENERATE_FOLDER_NAMES (inputfilename);

	vector <GDB> nGDB = competeRGFcontect (inputfilename);
	if (is_XY_FILE_CORRECT()) nGDB = insert_xy_values (nGDB);
	if (is_TRAJECTORY_FILE_CORRECT()) nGDB = APPLY_TRAJECTORY (nGDB);
	if (!is_mode_DEBUG()) CREATE_PROJECT_FOLDERS (nGDB);

	nGDB = SORT_GDB (nGDB, "LOC_GC_TYPE");
	vector < vector <GDB> > nGDB_G = SEPARATE_DATASET_GROUPS (nGDB);
	nGDB_G = PREPARE_GDB_VECTOR_FOR_PROCESSING (nGDB_G, false);
	nGDB_G = AVERAGE (nGDB_G);

	nGDB_G = ASSOCIATE_AVERAGE_BEDDING_GROUPS (nGDB_G);
	nGDB_G = clustering_GBD (nGDB_G);
	nGDB = MERGE_GROUPS_TO_GDB (nGDB_G);
	nGDB_G = SEPARATE_DATASET_GROUPS (nGDB);

	if (!is_mode_DEBUG()) cout << "DATA EVALUATION FROM '" << capslock(inputfilename) << ".RGF' DATABASE FILE" << endl;

	size_t LOOPS_NUMBER = 2;
	if (is_TRAJECTORY_FILE_CORRECT()) LOOPS_NUMBER = 4;

	for (size_t i = 0; i < LOOPS_NUMBER; i++) {

		PROCESS_AS_TILTED = false;
		if (i == 1 || i == 3) PROCESS_AS_TILTED = true;

		PROCESS_AS_TRAJECTORY = false;
		if (i == 2 || i == 3) PROCESS_AS_TRAJECTORY = true;

		const vector <vector <GDB> > process_GDB_G = EVALUATE (nGDB_G);

		MASTER_GDB.push_back (process_GDB_G);

		if (!is_mode_DEBUG()) OUTPUT_TO_RGF (MASTER_GDB.at(i));

		if (PROCESS_AS_TILTED) OUTPUT_TO_PS (MASTER_GDB.at(i-1), MASTER_GDB.at(i));

		OUTPUT_TO_WELL_PS (MASTER_GDB.at(i));
	}
	if (!is_mode_DEBUG()) cout << "EXPORT FROM '" << capslock(inputfilename) << ".RGF' DATABASE FILE" << endl;

	copy_log ();
}

void dbg_cout_GDB_vector_vector_structure (const vector < vector <GDB> >& inGDB_G) {

	cout << endl;
	cout << "-------- GBD VECTOR STRUCTURE --------" << endl;

	cout << " * <vector> GDB size: " << inGDB_G.size() << endl;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		cout << " * GDB.at(" << i << ") size: " << inGDB_G.at(i).size() << flush;
		cout << " " << inGDB_G.at(i).at(0).LOC << flush;
		cout << " " << inGDB_G.at(i).at(0).DATATYPE << flush;
		cout << " " << inGDB_G.at(i).at(0).FORMATION << flush;
		cout << " " << inGDB_G.at(i).at(0).GC << endl;
	}
	cout << "------ END GBD VECTOR STRUCTURE ------" << endl;
	cout << endl;
	return;
}

vector <double> return_GDB_vector_vector_structure (const vector < vector <GDB> >& inGDB_G) {

	vector <double> OUT;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		OUT.push_back (inGDB_G.at(i).size());
	}
	return OUT;
}

void dbg_cout_GDB_vector_vector (const vector < vector <GDB> >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) dbg_cout_GDB_vector (inGDB_G.at(i));

	return;
}

void dbg_cout_GDB_vector (const vector <GDB>& inGDB) {

	cout << endl;
	cout << "-------- START DUMPING GBD VECTOR --------" << endl;

	cout
	<< "ID" << '\t' << "iID" << '\t'
	//<< "N.X" << '\t' << "N.Y" << '\t' << "N.Z" << '\t'
	<< "D.X" << '\t' << "D.Y" << '\t'<< "D.Z" << '\t'
	//<< "S.X" << '\t' << "S.Y" << '\t'<< "S.Z" << '\t'
	//<< "NC.X" << '\t' << "NC.Y" << '\t'<< "NC.Z" << '\t'
	//<< "DC.X" << '\t' << "DC.Y" << '\t'<< "DC.Z" << '\t'
	//<< "SC.X" << '\t' << "SC.Y" << '\t'<< "SC.Z" << '\t'

	//<< "LPITCH" << '\t'
	//<< "LPITCHSENSE" << '\t'
	//<< "PITCHANGLE" << '\t'

	//<< "MISFIT" << '\t'
	//<< "LINEATION" << '\t'
	<< "OFFSET" << '\t'
	//<< "DEPTH" << '\t'
	<< "GC" << '\t'
	<< "COLOR" << '\t'
	//<< "LOC" << '\t'
	//<< "LOCX" << '\t'
	//<< "LOCY" << '\t'
	//<< "FORMATION" << '\t'
	//<< "DATATYPE" << '\t'
	//<< "DIPDIR" << '\t'
	//<< "DIP" << '\t'
	//<< "LDIR" << '\t'
	//<< "LDIP" << '\t'

	<< "corr.DIPDIR" << '\t'
	<< "corr.DIP" << '\t'
	//<< "corrL.DIPDIR" << '\t'
	//<< "corrL.DIP" << '\t'

	//<< "PALEON" << '\t'
	//<< "COMMENT" << '\t'
	//<< "DATAGROUP" << '\t'
	//<< "PSCOLOR" << '\t'
	//<< "DASHED" << '\t'

	//<< "ptnP.X" << '\t' << "ptnP.Y" << '\t'<< "ptnP.Z" << '\t'
	//<< "ptnT.X" << '\t' << "ptnT.Y" << '\t'<< "ptnT.Z" << '\t'
	//<< "ptnN.X" << '\t' << "ptnN.Y" << '\t'<< "ptnN.Z" << '\t'

	//<< "ptnPd.DIPDIR" << '\t'
	//<< "ptnPd.DIP" << '\t'
	//<< "ptnTd.DIPDIR" << '\t'
	//<< "ptnTd.DIP" << '\t'
	//<< "ptnNd.DIPDIR" << '\t'
	//<< "ptnNd.DIP" << '\t'

	//<< "avD.X" << '\t' << "avD.Y" << '\t'<< "avD.Z" << '\t'
	//<< "avS0D.X" << '\t' << "avS0D.Y" << '\t'<< "avS0D.Z" << '\t'
	//<< "avS0N.X" << '\t' << "avS0N.Y" << '\t'<< "avS0N.Z" << '\t'
	//<< "T.X" << '\t' << "T.Y" << '\t'<< "T.Z" << '\t'

	//<< "avS0d.DIPDIR" << '\t'
	//<< "avS0d.DIP" << '\t'
	//<< "avd.DIPDIR" << '\t'
	//<< "avd.DIP" << '\t'
	//<< "avS0offset" << '\t'


	//<< "fold_great_circle_N.X" << '\t'
	//<< "fold_great_circle_N.Y" << '\t'
	//<< "fold_great_circle_N.Z" << '\t'

	//<< "SHEAR_S.X" << '\t' << "SHEAR_S.Y" << '\t'<< "SHEAR_S.Z" << '\t'
	//<< "NORMAL_S.X" << '\t' << "NORMAL_S.Y" << '\t'<< "NORMAL_S.Z" << '\t'
	//<< "UPSILON.X" << '\t' << "UPSILON.Y" << '\t'<< "UPSILON.Z" << '\t'

	//<< "lambda" << '\t'
	//<< "ANG" << '\t'
	//<< "RUP" << '\t'
	<< endl;

	for (size_t i = 0; i < inGDB.size(); i++) {

		GDB T = inGDB.at(i);

		cout

		<< fixed << setprecision(6)
		<< T.ID << '\t' << T.iID << '\t'

		<< fixed << setprecision(6)
		//<< T.N.X << '\t' << T.N.Y << '\t' << T.N.Z << '\t'
		<< T.D.X << '\t' << T.D.Y << '\t'<< T.D.Z << '\t'
		//<< T.S.X << '\t' << T.S.Y << '\t'<< T.S.Z << '\t'
		//<< T.NC.X << '\t' << T.NC.Y << '\t'<< T.NC.Z << '\t'
		//<< T.DC.X << '\t' << T.DC.Y << '\t'<< T.DC.Z << '\t'
		//<< T.SC.X << '\t' << T.SC.Y << '\t'<< T.SC.Z << '\t'

		//<< T.LPITCH << '\t'
		//<< T.LPITCHSENSE << '\t'
		//<< T.PITCHANGLE << '\t'

		//<< fixed << setprecision(8)
		//<< T.MISFIT << '\t'
		//<< T.LINEATION << '\t'
		<< T.OFFSET << '\t'
		//<< T.DEPTH << '\t'

		<< fixed << setprecision(0)
		<< T.GC << '\t'
		<< T.COLOR << '\t'
		//<< T.LOC << '\t'
		//<< T.LOCX << '\t'
		//<< T.LOCY << '\t'
		//<< T.FORMATION << '\t'
		//<< T.DATATYPE << '\t'

		<< fixed << setprecision (0)
		//<< T.DIPDIR << '\t'
		//<< T.DIP << '\t'
		//<< T.LDIR << '\t'
		//<< T.LDIP << '\t'

		<< T.corr.DIPDIR << '\t'
		<< T.corr.DIP << '\t'
		<< T.corrL.DIPDIR << '\t'
		<< T.corrL.DIP << '\t'

		//<< fixed << setprecision(0)
		//<< T.PALEON << '\t'
		//<< T.COMMENT << '\t'
		//<< T.DATAGROUP << '\t'
		//<< T.PSCOLOR << '\t'
		//<< T.DASHED << '\t'

		//<< fixed << setprecision(6)
		//<< T.ptnP.X << '\t' << T.ptnP.Y << '\t'<< T.ptnP.Z << '\t'
		//<< T.ptnT.X << '\t' << T.ptnT.Y << '\t'<< T.ptnT.Z << '\t'
		//<< T.ptnN.X << '\t' << T.ptnN.Y << '\t'<< T.ptnN.Z << '\t'

		////<< fixed << setprecision(3)
		////<< T.ptnPd.DIPDIR << '\t'
		////<< T.ptnPd.DIP << '\t'
		////<< T.ptnTd.DIPDIR << '\t'
		////<< T.ptnTd.DIP << '\t'
		////<< T.ptnNd.DIPDIR << '\t'
		////<< T.ptnNd.DIP << '\t'

		<< fixed << setprecision(6)
		//<< T.avD.X << '\t' << T.avD.Y << '\t'<< T.avD.Z << '\t'
		//<< T.avS0D.X << '\t' << T.avS0D.Y << '\t'<< T.avS0D.Z << '\t'
		//<< T.avS0N.X << '\t' << T.avS0N.Y << '\t'<< T.avS0N.Z << '\t'
		//<< T.T.X << '\t' << T.T.Y << '\t'<< T.T.Z << '\t'

		//<< fixed << setprecision(1)
		//<< T.avS0d.DIPDIR << '\t'
		//<< T.avS0d.DIP << '\t'
		//<< T.avd.DIPDIR << '\t'
		//<< T.avd.DIP << '\t'
		//<< T.avS0offset << '\t'

		//<< fixed << setprecision(6)
		//<< T.fold_great_circle_N.X << '\t'
		//<< T.fold_great_circle_N.Y << '\t'
		//<< T.fold_great_circle_N.Z << '\t'

		//<< fixed << setprecision(6)
		//<< T.SHEAR_S.X << '\t' << T.SHEAR_S.Y << '\t'<< T.SHEAR_S.Z << '\t'
		//<< T.NORMAL_S.X << '\t' << T.NORMAL_S.Y << '\t'<< T.NORMAL_S.Z << '\t'
		//<< T.UPSILON.X << '\t' << T.UPSILON.Y << '\t'<< T.UPSILON.Z << '\t'

		//<< T.lambda << '\t'
		//<< T.ANG << '\t'
		//<< T.RUP << '\t'

		<< endl;
	}
	cout << "-------- END DUMPING GBD VECTOR --------" << endl << endl;

	return;
}

void compare_structures (const vector <double>& IN1, const vector <double>& IN2) {

	if (IN1.size() != IN2.size()) ASSERT_DEAD_END();

	for (size_t i = 0; i < IN1.size(); i++) {

		if (IN1.at(i) != IN2.at(i)) ASSERT_DEAD_END();
	}
	return;
}
