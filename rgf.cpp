
// Copyright (C) 2012 - 2014 Ágoston Sasvári
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
#include "ps.h"
#include "rakhmanov.hpp"
#include "random.hpp"
#include "retilt.hpp"
#include "rgf.h"
#include "run_mode.h"
#include "settings.hpp"
#include "standard_output.hpp"
//#include "well.hpp"
//#include "well_ps.hpp"

using namespace std;

namespace {

string INPUTFILENAME = "";

const double SN = 10e-8;
}

string return_inputfilename () {

	return INPUTFILENAME;
}

void set_inputfilename (const string filename) {

	INPUTFILENAME = filename;

	return;
}

vector <GDB> competeRGFcontect (const string projectname, const string inputxyfilename) {

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

		if (capslock(inputxyfilename) != "NONE")  outGDB.at(i) = insertxy (outGDB.at(i));
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

vector < vector < vector <vector <GDB> > > > clustering_GBD (const vector < vector < vector <vector <GDB> > > >& inGDB_G) {

	vector < vector < vector <vector <GDB> > > > outGDB_G = inGDB_G;

	cout << "CLUSTER" << endl;

	cout << "is_CLUSTERING_NONE():" << is_CLUSTERING_NONE() << endl;

	cout << "is_CLUSTERNUMBER()  :" << is_CLUSTERNUMBER() << endl;

	outGDB_G = associate_empty_clustercode (outGDB_G, 2);

	if (is_CLUSTERING_NONE() || is_CLUSTERNUMBER() == "1") return outGDB_G;

	if (!is_mode_DEBUG()) cout << "K-MEANS CLUSTERING OF INPUT DATABASE FILE" << endl;

	//outGDB_G = associate_empty_clustercode (outGDB_G, 2);

	for (size_t i = 0; i < outGDB_G.size(); i++) {
		for (size_t j = 0; j < outGDB_G.at(i).size(); j++) {
			for (size_t k = 0; k < outGDB_G.at(i).at(j).size(); k++) {

				vector <GDB> process_GDB = outGDB_G.at(i).at(j).at(k);

				const bool LITHOLOGY = is_allowed_lithology_datatype(process_GDB.at(0).DATATYPE);

				if (process_GDB.size() > 1 && !LITHOLOGY) outGDB_G.at(i).at(j).at(k) = K_MEANS (process_GDB);
			}
		}
	}
	return outGDB_G;
}

bool byLocTypeGc (const GDB& x, const GDB& y) {

	if (x.LOC != y.LOC) return x.LOC < y.LOC;
	if (x.DATATYPE != y.DATATYPE) return x.DATATYPE < y.DATATYPE;
	return x.GC < y.GC;
}

bool byLocGcType (const GDB& x, const GDB& y) {

	if (x.LOC != y.LOC) return x.LOC < y.LOC;
	if (x.GC != y.GC) return x.GC < y.GC;
	return x.DATATYPE < y.DATATYPE;
}

bool byiID(const GDB& x, const GDB& y) {

	return x.iID < y.iID;
}

bool byeigenvalue(const sort_jacobi& x, const sort_jacobi& y) {

	return x.eigenvalue < y.eigenvalue;
}

vector <GDB> sort_by_iID (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	sort(outGDB.begin(), outGDB.end(), byiID);

	return outGDB;
}

bool by_DEPTH (const GDB& x, const GDB& y) {

	return x.DEPTH < y.DEPTH;
}

vector <GDB> sort_by_DEPTH (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	sort (outGDB.begin(), outGDB.end(), by_DEPTH);

	return outGDB;
}

bool by_rev_DEPTH (const GDB& x, const GDB& y) {

	return x.DEPTH > y.DEPTH;
}

vector <GDB> sort_by_rev_DEPTH (const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;

	sort (outGDB.begin(), outGDB.end(), by_rev_DEPTH);

	return outGDB;
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

	//dbg_cout_GDB_vector(outGDB);

	outGDB = generate_OFFSET (outGDB);

	//dbg_cout_GDB_vector(outGDB);

	return outGDB;
}

vector < vector < vector < vector <GDB> > > > PREPARE_GDB_VECTOR_FOR_PROCESSING (const vector < vector < vector < vector <GDB> > > >& inGDB_G, const bool TILT) {

	if (!is_mode_DEBUG()) cout << "GEODATABASE PROCESSING OF INPUT DATABASE FILE" << endl;

	vector < vector < vector <vector <GDB> > > > outGDB_G;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		vector < vector <vector <GDB> > > buf3;

		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {

			vector <vector <GDB> > buf2;

			for (size_t k = 0; k < inGDB_G.at(i).at(j).size(); k++) {

				vector <GDB> buf = inGDB_G.at(i).at(j).at(k);

				buf = sort_by_iID (buf);

				//dbg_cout_GDB_vector(buf);

				if (!TILT) buf = PREPARE_GDB_FOR_PROCESSING (buf, false);
				else buf = PREPARE_GDB_FOR_PROCESSING (buf, true);

				//dbg_cout_GDB_vector(buf);

				buf2.push_back (buf);
			}
			buf3.push_back (buf2);
		}
		outGDB_G.push_back (buf3);
	}
	return outGDB_G;
}

void process_rgf (string inputfilename, string XY_filename) {

	set_inputfilename (inputfilename);

	INIT_DEBUG();

	const PFN projectfoldername = create_project_folder_names (inputfilename);

	vector <GDB> nGDB, tGDB, ST_nGDB, ST_tGDB;

	if (XY_filename == "NONE") nGDB = competeRGFcontect (inputfilename, "NONE");
	else nGDB = competeRGFcontect (inputfilename, XY_filename);

	//dbg_cout_GDB_vector(nGDB);
	sort(nGDB.begin(), nGDB.end(), byLocGcType);

	if (!is_mode_DEBUG()) CREATE_PROJECT_FOLDER (projectfoldername, nGDB);

	vector < vector < vector < vector <GDB> > > > nGDB_G = SEPARATE_DATASET_TO_GROUPS (nGDB, "GROUPS");

	nGDB_G = PREPARE_GDB_VECTOR_FOR_PROCESSING (nGDB_G, false);

	//dbg_cout_GDB_vector_vector(nGDB_G);

	nGDB_G = AVERAGE (nGDB_G);//eddig ok

	//nGDB_G = CALCULATE_FOLDSURFACE (nGDB_G);//new

	nGDB_G = clustering_GBD (nGDB_G);

	//dbg_cout_GDB_vector_vector(nGDB_G);

	nGDB = MERGE_GROUPS_TO_GDB (nGDB_G);

	nGDB_G = SEPARATE_DATASET_TO_GROUPS (nGDB, "CLUSTER");

	cout << "RETILT" << endl;

	vector < vector < vector < vector <GDB> > > > tGDB_G = RETILT (nGDB_G);

	cout << "OK1B" << endl;

	tGDB_G = PREPARE_GDB_VECTOR_FOR_PROCESSING (tGDB_G, true);

	cout << "OK2" << endl;

	tGDB_G = AVERAGE (tGDB_G);

	cout << "OK3" << endl;

	nGDB_G = PROCESS_GROUPS (nGDB_G, false);
	tGDB_G = PROCESS_GROUPS (tGDB_G, true);

	//PROCESS_WELL_GROUPS (nGDB_G, false);
	//PROCESS_WELL_GROUPS (tGDB_G, true);

	if (!is_mode_DEBUG()) cout << "DATA EVALUATION FROM '" << capslock(inputfilename) << ".RGF' DATABASE FILE" << endl;

	nGDB = MERGE_GROUPS_TO_GDB (nGDB_G);
	tGDB = MERGE_GROUPS_TO_GDB (tGDB_G);

	cout << "OK4" << endl;

	nGDB = GENERATE_PS_CODE (nGDB);
	tGDB = GENERATE_PS_CODE (tGDB);

	cout << "OK5" << endl;

	nGDB = sort_by_iID (nGDB);
	tGDB = sort_by_iID (tGDB);

	cout << "OK6" << endl;

	if (is_GROUPSEPARATION_IGNORE()) {

		nGDB_G = SEPARATE_DATASET_TO_GROUPS (nGDB, "NONE");
		tGDB_G = SEPARATE_DATASET_TO_GROUPS (tGDB, "NONE");
	}
	else if (is_GROUPSEPARATION_GROUPCODE()) {

		nGDB_G = SEPARATE_DATASET_TO_GROUPS (nGDB, "GROUPS");
		tGDB_G = SEPARATE_DATASET_TO_GROUPS (tGDB, "GROUPS");
	}
	else if (is_GROUPSEPARATION_KMEANS()) {

		nGDB_G = SEPARATE_DATASET_TO_GROUPS (nGDB, "CLUSTER");
		tGDB_G = SEPARATE_DATASET_TO_GROUPS (tGDB, "CLUSTER");
	}
	else if (is_GROUPSEPARATION_RUPANG()) {

		nGDB_G = SEPARATE_DATASET_TO_GROUPS (nGDB, "RUP");
		tGDB_G = SEPARATE_DATASET_TO_GROUPS (tGDB, "RUP");
	}
	else ASSERT_DEAD_END();

	cout << "OK7" << endl;

	//dbg_cout_GDB_vector (nGDB);
	//dbg_cout_GDB_vector (tGDB);
	//dbg_cout_GDB_vector_vector(nGDB_G);
	//dbg_cout_GDB_vector_vector(tGDB_G);

	STANDARD_OUTPUT (nGDB, tGDB);

	if (!is_mode_DEBUG()) OUTPUT_TO_RGF (nGDB_G, projectfoldername, false);
	if (!is_mode_DEBUG()) OUTPUT_TO_RGF (tGDB_G, projectfoldername, true);

	cout << "OK8" << endl;

	OUTPUT_TO_PS (nGDB_G, tGDB_G, projectfoldername);

	//OUTPUT_TO_WELL_PS (nGDB_G, projectfoldername, false);
	//OUTPUT_TO_WELL_PS (tGDB_G, projectfoldername, true);
	//careful! PS module has global variables!!!!

	cout << "OK9" << endl;

	if (!is_mode_DEBUG()) cout << "EXPORT FROM '" << capslock(inputfilename) << ".RGF' DATABASE FILE" << endl;

	copy_log(projectfoldername);
}

void dbg_cout_GDB_v_v_v_v_structure (const vector < vector < vector < vector <GDB> > > >& inGDB_G) {

	cout << "inGDB_G.size() : " << inGDB_G.size() << endl;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		cout << inGDB_G.at(i).at(0).at(0).at(0).DATATYPE << endl;
		cout << "inGDB_G.at(i).size() : " << inGDB_G.at(i).size() << endl;

		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {

			cout << "inGDB_G.at(i).at(j).size() : " << inGDB_G.at(i).at(j).size() << endl;

			for (size_t k = 0; k < inGDB_G.at(i).at(j).size(); k++) {

				cout << "inGDB_G.at(i).at(j).at(k).size() : " << inGDB_G.at(i).at(j).at(k).size() << endl;

			}
		}
	}
	return;
}

void dbg_cout_GDB_vector_vector (const vector < vector < vector < vector <GDB> > > >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) {
		for (size_t j = 0; j < inGDB_G.at(i).size(); j++) {
			for (size_t k = 0; k < inGDB_G.at(i).at(j).size(); k++) {

				const vector <GDB> ACT = inGDB_G.at(i).at(j).at(k);

				dbg_cout_GDB_vector (ACT);
			}
		}
	}
	return;
}

void dbg_cout_GDB_v_v (const vector < vector <GDB> >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		const vector <GDB> ACT = inGDB_G.at(i);

		dbg_cout_GDB_vector (ACT);
	}
	return;
}


void dbg_cout_GDB_vector (const vector <GDB>& inGDB) {

	cout << endl;
	cout << "-------- START DUMPING GBD VECTOR --------" << endl;

	cout
	<< "ID" << '\t' << "iID" << '\t'
	//<< "N.X" << '\t' << "N.Y" << '\t' << "N.Z" << '\t'
	//<< "D.X" << '\t' << "D.Y" << '\t'<< "D.Z" << '\t'
	//<< "S.X" << '\t' << "S.Y" << '\t'<< "S.Z" << '\t'
	//<< "NC.X" << '\t' << "NC.Y" << '\t'<< "NC.Z" << '\t'
	//<< "DC.X" << '\t' << "DC.Y" << '\t'<< "DC.Z" << '\t'
	//<< "SC.X" << '\t' << "SC.Y" << '\t'<< "SC.Z" << '\t'
	//<< "SV.X" << '\t' << "SV.Y" << '\t'<< "SV.Z" << '\t'

	//<< "LPITCH" << '\t'
	//<< "LPITCHSENSE" << '\t'
	//<< "PITCHANGLE" << '\t'

	//<< "MISFIT" << '\t'
	//<< "LINEATION" << '\t'
	//<< "UPWARD" << '\t'
	//<< "OFFSET" << '\t'
	//<< "UP" << '\t'
	//<< "DEPTH" << '\t'
	//<< "GC" << '\t'
	//<< "COLOR" << '\t'
	//<< "LOC" << '\t'
	//<< "LOCX" << '\t'
	//<< "LOCY" << '\t'
	//<< "FORMATION" << '\t'
	<< "DATATYPE" << '\t'
	//<< "DIPDIR" << '\t'
	//<< "DIP" << '\t'
	//<< "LDIR" << '\t'
	//<< "LDIP" << '\t'

	<< "corr.DIPDIR" << '\t'
	<< "corr.DIP" << '\t'
	<< "corrL.DIPDIR" << '\t'
	<< "corrL.DIP" << '\t'

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

		//<< fixed << setprecision(6)
		//<< T.N.X << '\t' << T.N.Y << '\t' << T.N.Z << '\t'
		//<< T.D.X << '\t' << T.D.Y << '\t'<< T.D.Z << '\t'
		//<< T.S.X << '\t' << T.S.Y << '\t'<< T.S.Z << '\t'
		//<< T.NC.X << '\t' << T.NC.Y << '\t'<< T.NC.Z << '\t'
		//<< T.DC.X << '\t' << T.DC.Y << '\t'<< T.DC.Z << '\t'
		//<< T.SC.X << '\t' << T.SC.Y << '\t'<< T.SC.Z << '\t'
		//remove//<< T._SV.X << '\t' << T._SV.Y << '\t'<< T._SV.Z << '\t'

		//<< T.LPITCH << '\t'
		//<< T.LPITCHSENSE << '\t'
		//<< T.PITCHANGLE << '\t'

		//<< fixed << setprecision(8)
		//<< T.MISFIT << '\t'
		//<< T.LINEATION << '\t'
		//remove//<< T.UPWARD << '\t'
		//<< T.OFFSET << '\t'
		//remove//<< T.UP<< '\t'

		//<< fixed << setprecision(0)
		//<< T.DEPTH << '\t'
		//<< T.GC << '\t'
		//<< T.COLOR << '\t'
		//<< T.LOC << '\t'
		//<< T.LOCX << '\t'
		//<< T.LOCY << '\t'
		//<< T.FORMATION << '\t'
		<< T.DATATYPE << '\t'

		//<< fixed << setprecision (0)
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

		//<< fixed << setprecision(3)
		//<< T.ptnPd.DIPDIR << '\t'
		//<< T.ptnPd.DIP << '\t'
		//<< T.ptnTd.DIPDIR << '\t'
		//<< T.ptnTd.DIP << '\t'
		//<< T.ptnNd.DIPDIR << '\t'
		//<< T.ptnNd.DIP << '\t'

		//<< fixed << setprecision(6)
		//<< T.avD.X << '\t' << T.avD.Y << '\t'<< T.avD.Z << '\t'
		//<< T.avS0D.X << '\t' << T.avS0D.Y << '\t'<< T.avS0D.Z << '\t'
		//<< T.avS0N.X << '\t' << T.avS0N.Y << '\t'<< T.avS0N.Z << '\t'

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
