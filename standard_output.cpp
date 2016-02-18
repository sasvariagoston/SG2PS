// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "assertions.hpp"
#include "allowed_keys.hpp"
#include "common.h"
#include "filename.hpp"
#include "inversion.h"
#include "rgf.h"
#include "run_mode.h"
#include "rup_clustering.hpp"
#include "settings.hpp"
#include "standard_output.hpp"
#include "structs.h"
#include "valley_method.hpp"
#include "well.hpp"
#include "well_ps.hpp"

using namespace std;

namespace {

bool CHK_LITHOLOGY_DT = false;
bool CHK_LINEATION_DT = false;
bool CHK_PLANE_DT = false;
bool CHK_STRIAE_DT = false;
bool CHK_SC_DT = false;

bool CHK_ANGELIER = false;
bool CHK_BINGHAM = false;
bool CHK_BRUTEFORCE = false;
bool CHK_FRY = false;
bool CHK_MICHAEL = false;
bool CHK_MOSTAFA = false;
bool CHK_SPRANG = false;
bool CHK_SHAN = false;
bool CHK_TURNER = false;

bool CHK_K_MEANS = false;
bool CHK_RUP_ANG = false;

bool CHK_CONTOURING = false;
bool CHK_RETILT = false;
bool CHK_FOLDSURFACE = false;
bool CHK_ROSE = false;
bool CHK_AVERAGE = false;
bool CHK_COLORS = false;

bool CHK_PLOT_LINEATION = false;
bool CHK_PLOT_PLANE = false;
bool CHK_PLOT_STRIAE = false;

bool CHK_WELL = false;

}

bool is_CHK_LITHOLOGY_DT () {

	return CHK_LITHOLOGY_DT;
}

bool is_CHK_LINEATION_DT () {

	return CHK_LINEATION_DT;
}

bool is_CHK_PLANE_DT () {

	return CHK_PLANE_DT;
}

bool is_CHK_STRIAE_DT () {

	return CHK_STRIAE_DT;
}

bool is_CHK_SC_DT () {

	return CHK_SC_DT;
}

bool is_CHK_ANGELIER () {

	return CHK_ANGELIER;
}

bool is_CHK_BINGHAM () {

	return CHK_BINGHAM;
}

bool is_CHK_BRUTEFORCE () {

	return CHK_BRUTEFORCE;
}

bool is_CHK_FRY () {

	return CHK_FRY;
}

bool is_CHK_MICHAEL () {

	return CHK_MICHAEL;
}

bool is_CHK_MOSTAFA () {

	return CHK_MOSTAFA;
}

bool is_CHK_SPRANG () {

	return CHK_SPRANG;
}

bool is_CHK_SHAN () {

	return CHK_SHAN;
}

bool is_CHK_TURNER () {

	return CHK_TURNER;
}

bool is_CHK_K_MEANS () {

	return CHK_K_MEANS;
}

bool is_CHK_RUP_ANG () {

	return CHK_RUP_ANG;
}

bool is_CHK_CONTOURING () {

	return CHK_CONTOURING;
}

bool is_CHK_RETILT () {

	return CHK_RETILT;
}

bool is_CHK_FOLDSURFACE () {

	return CHK_FOLDSURFACE;
}

bool is_CHK_ROSE () {

	return CHK_ROSE;
}

bool is_CHK_AVERAGE () {

	return CHK_AVERAGE;
}

bool is_CHK_COLORS () {

	return CHK_COLORS;
}

bool is_CHK_PLOT_LINEATION () {

	return CHK_PLOT_LINEATION;
}

bool is_CHK_PLOT_PLANE () {

	return CHK_PLOT_PLANE;
}

bool is_CHK_PLOT_STRIAE () {

	return CHK_PLOT_STRIAE;
}

bool is_CHK_WELL () {

	return CHK_WELL;
}

void INIT_DEBUG () {

	const string inputfilename = return_inputfilename();

	if (is_mode_DEBUG()) {

		if (inputfilename == "ST_DT_LITHOLOGY") CHK_LITHOLOGY_DT = true;
		else if (inputfilename == "ST_DT_LINEATION") CHK_LINEATION_DT = true;
		else if (inputfilename == "ST_DT_PLANE") CHK_PLANE_DT = true;
		else if (inputfilename == "ST_DT_SC") CHK_SC_DT = true;
		else if (inputfilename == "ST_DT_STRIAE") CHK_STRIAE_DT = true;

		else if (inputfilename == "ST_INV_ANGELIER") CHK_ANGELIER = true;
		else if (inputfilename == "ST_INV_BINGHAM") CHK_BINGHAM = true;
		else if (inputfilename == "ST_INV_BRUTEFORCE") CHK_BRUTEFORCE = true;
		else if (inputfilename == "ST_INV_FRY") CHK_FRY = true;
		else if (inputfilename == "ST_INV_MICHAEL") CHK_MICHAEL = true;
		else if (inputfilename == "ST_INV_MOSTAFA") CHK_MOSTAFA = true;
		else if (inputfilename == "ST_INV_SPRANG") CHK_SPRANG = true;
		else if (inputfilename == "ST_INV_SHAN") CHK_SHAN = true;
		else if (inputfilename == "ST_INV_TURNER") CHK_TURNER = true;

		else if (inputfilename == "ST_RUP_ANG") CHK_RUP_ANG = true;
		else if (inputfilename == "ST_K-MEANS") CHK_K_MEANS = true;

		else if (inputfilename == "ST_CONTOURING") CHK_CONTOURING = true;
		else if (inputfilename == "ST_RETILT") CHK_RETILT = true;
		else if (inputfilename == "ST_FOLDSURFACE") CHK_FOLDSURFACE = true;
		else if (inputfilename == "ST_ROSE") CHK_ROSE = true;
		else if (inputfilename == "ST_AVERAGE") CHK_AVERAGE = true;
		else if (inputfilename == "ST_COLORS") CHK_COLORS = true;

		else if (inputfilename == "ST_PLOT_PLANE") CHK_PLOT_PLANE = true;
		else if (inputfilename == "ST_PLOT_STRIAE") CHK_PLOT_STRIAE = true;
		else if (inputfilename == "ST_PLOT_LINEATION") CHK_PLOT_LINEATION = true;

		else if (inputfilename == "ST_WELL") CHK_WELL = true;
		else ASSERT_DEAD_END();
	}
}

void STANDARD_OUTPUT (const vector <GDB>& inGDB) {

	if (!is_mode_DEBUG()) return;

	const bool TILT = is_PROCESS_AS_TILTED();

	string T = "NRM";
	if (TILT) T = "TLT";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	if (is_CHK_LITHOLOGY_DT () && !TILT) 		dump_RGF_to_file (inGDB, "ST_DT_LITHOLOGY");
	else if (is_CHK_LINEATION_DT() && !TILT) 	dump_RGF_to_file (inGDB, "ST_DT_LINEATION");
	else if (is_CHK_PLANE_DT () && !TILT) 		dump_RGF_to_file (inGDB, "ST_DT_PLANE");
	else if (is_CHK_STRIAE_DT () && !TILT)		dump_RGF_to_file (inGDB, "ST_DT_STRIAE");
	else if (is_CHK_SC_DT () && !TILT) 			dump_RGF_to_file (inGDB, "ST_DT_SC");
	else if (is_CHK_RETILT() && TILT)			dump_RGF_to_file (inGDB, "ST_RETILT");
	else if (is_CHK_AVERAGE())					dump_RGF_to_file (inGDB, "ST_AVERAGE_"+T);
	else if (is_CHK_COLORS()) 					dump_RGF_to_file (inGDB, "ST_COLORS_"+T);
	else if (is_CHK_FOLDSURFACE()) 				dump_RGF_to_file (inGDB, "ST_FOLDSURFACE");
	else if (is_CHK_ANGELIER() && is_INVERSION_ANGELIER()) 		dump_RGF_SF_to_file (inGDB, "ST_INV_ANGELIER_"+T);
	else if (is_CHK_BINGHAM() && is_BINGHAM_USE()) 				dump_RGF_SF_to_file (inGDB, "ST_INV_BINGHAM_"+T);
	else if (is_CHK_BRUTEFORCE() && is_INVERSION_BRUTEFORCE())	dump_RGF_SF_to_file (inGDB, "ST_INV_BRUTEFORCE_"+T);
	else if (is_CHK_FRY() && is_INVERSION_FRY()) 				dump_RGF_SF_to_file (inGDB, "ST_INV_FRY_"+T);
	else if (is_CHK_MICHAEL() && is_INVERSION_MICHAEL()) 		dump_RGF_SF_to_file (inGDB, "ST_INV_MICHAEL_"+T);
	else if (is_CHK_MOSTAFA() && is_INVERSION_MOSTAFA()) 		dump_RGF_SF_to_file (inGDB, "ST_INV_MOSTAFA_"+T);
	else if (is_CHK_SPRANG() && is_INVERSION_SPRANG()) 			dump_RGF_SF_to_file (inGDB, "ST_INV_SPRANG_"+T);
	else if (is_CHK_SHAN() && is_INVERSION_SHAN())				dump_RGF_SF_to_file (inGDB, "ST_INV_SHAN_"+T);
	else if (is_CHK_TURNER() && is_INVERSION_TURNER())			dump_RGF_SF_to_file (inGDB, "ST_INV_TURNER_"+T);
	else {};
}

void dump_RGF_SF_to_file (const vector <GDB>& inGDB, const string FN) {

	dump_RGF_to_file (inGDB, FN);
	dump_SF_to_file (inGDB.at(0).SFV, FN);
}

void dump_RGF_to_file (const vector <GDB>& inGDB, const string FN) {

	ofstream o((FN + "_GDB.csv").c_str());

	o
	<< "NAME_ID s" << '\t'
	<< "iID i" << '\t'
	<< "N.X d" << '\t'  << "N.Y d" << '\t'  << "N.Z d" << '\t'
	<< "D.X d" << '\t'  << "D.Y d" << '\t'  << "D.Z d" << '\t'
	<< "S.X d" << '\t'  << "S.Y d" << '\t'  << "S.Z d" << '\t'
	<< "NC.X d" << '\t' << "NC.Y d" << '\t' << "NC.Z d" << '\t'
	<< "DC.X d" << '\t' << "DC.Y d" << '\t' << "DC.Z d" << '\t'
	<< "SC.X d" << '\t' << "SC.Y d" << '\t' << "SC.Z d" << '\t'

	<< "LPITCH  d" << '\t'
	<< "LPITCHSENSE s" << '\t'
	<< "PITCHANGLE  d" << '\t'

	<< "MISFIT d" << '\t'
	<< "LINEATION s" << '\t'
	<< "OFFSET s" << '\t'

	<< "DEPTH d" << '\t'
	<< "GC s" << '\t'
	<< "COLOR s" << '\t'
	<< "LOC s" << '\t'
	<< "LOCX d" << '\t'
	<< "LOCY d" << '\t'
	<< "FORMATION s" << '\t'
	<< "DATATYPE s" << '\t'

	<< "DIPDIR d" << '\t'
	<< "DIP d" << '\t'
	<< "LDIR d" << '\t'
	<< "LDIP d" << '\t'
	<< "corr.DIPDIR d" << '\t'
	<< "corr.DIP d" << '\t'
	<< "corrL.DIPDIR d" << '\t'
	<< "corrL.DIP d" << '\t'

	<< "PALEON d" << '\t'
	<< "COMMENT s" << '\t'
	<< "DATAGROUP s" << '\t'
	<< "PSCOLOR s" << '\t'
	<< "DASHED s" << '\t'

	<< "ptnP.X d" << '\t' << "ptnP.Y d" << '\t'<< "ptnP.Z d" << '\t'
	<< "ptnT.X d" << '\t' << "ptnT.Y d" << '\t'<< "ptnT.Z d" << '\t'
	<< "ptnN.X d" << '\t' << "ptnN.Y d" << '\t'<< "ptnN.Z d" << '\t'

	<< "avD.X d"   << '\t' << "avD.Y d"   << '\t'<< "avD.Z d" << '\t'
	<< "avS0D.X d" << '\t' << "avS0D.Y d" << '\t'<< "avS0D.Z d" << '\t'
	<< "avS0N.X d" << '\t' << "avS0N.Y d" << '\t'<< "avS0N.Z d" << '\t'

	<< "avS0d.DIPDIR d" << '\t'
	<< "avS0d.DIP d" << '\t'
	<< "avd.DIPDIR d" << '\t'
	<< "avd.DIP d" << '\t'
	<< "avS0offset s" << '\t'

	<< "TRAJECTORY.X d" << '\t' << "TRAJECTORY.Y d" << '\t'<< "TRAJECTORY.Z d" << '\t'

	<< "fold_great_circle_N.X d" << '\t'
	<< "fold_great_circle_N.Y d" << '\t'
	<< "fold_great_circle_N.Z d" << '\t'

	<< "SHEAR_S.X d" << '\t'  << "SHEAR_S.Y d"  << '\t'<< "SHEAR_S.Z d" << '\t'
	<< "NORMAL_S.X d" << '\t' << "NORMAL_S.Y d" << '\t'<< "NORMAL_S.Z d" << '\t'

	<< "lambda d" << '\t'
	<< "ANG d" << '\t'
	<< "RUP d" << endl;

	for (size_t i = 0; i < inGDB.size(); i++) {

		GDB T = inGDB.at(i);

		o
		<< T.ID << '\t'
		<< T.iID << '\t'
		<< dmp_dbl(T.N.X, 8) << '\t'  << dmp_dbl(T.N.Y, 8) << '\t'   << dmp_dbl(T.N.Z, 8) << '\t'
		<< dmp_dbl(T.D.X, 8) << '\t'  << dmp_dbl(T.D.Y, 8) << '\t'  << dmp_dbl(T.D.Z, 8) << '\t'
		<< dmp_dbl(T.S.X, 8) << '\t'  << dmp_dbl(T.S.Y, 8) << '\t'  << dmp_dbl(T.S.Z, 8) << '\t'
		<< dmp_dbl(T.NC.X, 8) << '\t' << dmp_dbl(T.NC.Y, 8) << '\t' << dmp_dbl(T.NC.Z, 8) << '\t'
		<< dmp_dbl(T.DC.X, 8) << '\t' << dmp_dbl(T.DC.Y, 8) << '\t' << dmp_dbl(T.DC.Z, 8) << '\t'
		<< dmp_dbl(T.SC.X, 8) << '\t' << dmp_dbl(T.SC.Y, 8) << '\t' << dmp_dbl(T.SC.Z, 8) << '\t'

		<< dmp_dbl(T.LPITCH, 8) << '\t'
		<< T.LPITCHSENSE << '\t'
		<< dmp_dbl(T.PITCHANGLE, 8) << '\t'

		<< dmp_dbl(T.MISFIT, 8) << '\t'
		<< T.LINEATION << '\t'
		<< T.OFFSET << '\t'

		<< dmp_dbl(T.DEPTH, 8) << '\t'
		<< T.GC << '\t'
		<< T.COLOR << '\t'
		<< T.LOC << '\t'
		<< dmp_dbl(T.LOCX, 8) << '\t'
		<< dmp_dbl(T.LOCY, 8) << '\t'
		<< T.FORMATION << '\t'
		<< T.DATATYPE << '\t'

		<< dmp_dbl(T.DIPDIR, 8) << '\t'
		<< dmp_dbl(T.DIP, 8) << '\t'
		<< dmp_dbl(T.LDIR, 8) << '\t'
		<< dmp_dbl(T.LDIP, 8) << '\t'
		<< dmp_dbl(T.corr.DIPDIR, 8) << '\t'
		<< dmp_dbl(T.corr.DIP, 8) << '\t'
		<< dmp_dbl(T.corrL.DIPDIR, 8) << '\t'
		<< dmp_dbl(T.corrL.DIP, 8) << '\t'

		<< dmp_dbl(T.PALEON, 8) << '\t'
		<< T.COMMENT << '\t'
		<< T.DATAGROUP << '\t'
		<< T.PSCOLOR << '\t'
		<< T.DASHED << '\t'

		<< dmp_dbl(T.ptnP.X, 8) << '\t' << dmp_dbl(T.ptnP.Y, 8) << '\t'<< dmp_dbl(T.ptnP.Z, 8) << '\t'
		<< dmp_dbl(T.ptnT.X, 8) << '\t' << dmp_dbl(T.ptnT.Y, 8) << '\t'<< dmp_dbl(T.ptnT.Z, 8) << '\t'
		<< dmp_dbl(T.ptnN.X, 8) << '\t' << dmp_dbl(T.ptnN.Y, 8) << '\t'<< dmp_dbl(T.ptnN.Z, 8) << '\t'

		<< dmp_dbl(T.avD.X, 8)   << '\t' << dmp_dbl(T.avD.Y, 8)   << '\t'<< dmp_dbl(T.avD.Z, 8) << '\t'
		<< dmp_dbl(T.avS0D.X, 8) << '\t' << dmp_dbl(T.avS0D.Y, 8) << '\t'<< dmp_dbl(T.avS0D.Z, 8) << '\t'
		<< dmp_dbl(T.avS0N.X, 8) << '\t' << dmp_dbl(T.avS0N.Y, 8) << '\t'<< dmp_dbl(T.avS0N.Z, 8) << '\t'

		<< dmp_dbl(T.avS0d.DIPDIR, 8) << '\t'
		<< dmp_dbl(T.avS0d.DIP, 8) << '\t'
		<< dmp_dbl(T.avd.DIPDIR, 8) << '\t'
		<< dmp_dbl(T.avd.DIP, 8) << '\t'
		<< T.avS0offset << '\t'

		<< dmp_dbl(T.T.X, 8) << '\t'  << dmp_dbl(T.T.Y, 8) << '\t'   << dmp_dbl(T.T.Z, 8) << '\t'

		<< dmp_dbl(T.fold_great_circle_N.X, 8) << '\t'
		<< dmp_dbl(T.fold_great_circle_N.Y, 8) << '\t'
		<< dmp_dbl(T.fold_great_circle_N.Z, 8) << '\t'

		<< dmp_dbl(T.SHEAR_S.X, 8) << '\t'  << dmp_dbl(T.SHEAR_S.Y, 8)  << '\t'<< dmp_dbl(T.SHEAR_S.Z, 8) << '\t'
		<< dmp_dbl(T.NORMAL_S.X, 8) << '\t' << dmp_dbl(T.NORMAL_S.Y, 8) << '\t'<< dmp_dbl(T.NORMAL_S.Z, 8) << '\t'

		<< dmp_dbl(T.lambda, 8) << '\t'
		<< dmp_dbl(T.ANG, 8) << '\t'
		<< dmp_dbl(T.RUP, 8) << endl;
	}
	return;
}

void dump_SF_to_file (const vector <STRESSFIELD>& SF, const string FN) {

	ofstream o((FN + "_SF.csv").c_str());

	o
	<< "EIGENVECTOR1.X d" << '\t' << "EIGENVECTOR1.Y d" << '\t' << "EIGENVECTOR1.Z d" << '\t'
	<< "EIGENVECTOR2.X d" << '\t' << "EIGENVECTOR2.Y d" << '\t' << "EIGENVECTOR2.Z d" << '\t'
	<< "EIGENVECTOR3.X d" << '\t' << "EIGENVECTOR3.Y d" << '\t' << "EIGENVECTOR3.Z d" << '\t'

	<< "EIGENVALUE.X d" << '\t' << "EIGENVALUE.Y d" << '\t' << "EIGENVALUE.Z d" << '\t'

	<< "S_1.DIPDIR d" << '\t' << "S_1.DIP d" << '\t'
	<< "S_2.DIPDIR d" << '\t' << "S_2.DIP d" << '\t'
	<< "S_3.DIPDIR d" << '\t' << "S_3.DIP d" << '\t'

	<< "stressratio d" << '\t'
	<< "delvaux_str d" << '\t'
	<< "regime s" << '\t'
	<< "delvaux_rgm s" << '\t'
	<< "shmax d" << '\t'
	<< "shmin d" << endl;

	for (size_t i = 0; i < SF.size(); i++) {

		const STRESSFIELD S = SF.at(i);

		o
		<< dmp_dbl(S.EIGENVECTOR1.X, 8) << '\t' << dmp_dbl(S.EIGENVECTOR1.Y, 8) << '\t' << dmp_dbl(S.EIGENVECTOR1.Z, 8) << '\t'
		<< dmp_dbl(S.EIGENVECTOR2.X, 8) << '\t' << dmp_dbl(S.EIGENVECTOR2.Y, 8) << '\t' << dmp_dbl(S.EIGENVECTOR2.Z, 8) << '\t'
		<< dmp_dbl(S.EIGENVECTOR3.X, 8) << '\t' << dmp_dbl(S.EIGENVECTOR3.Y, 8) << '\t' << dmp_dbl(S.EIGENVECTOR3.Z, 8) << '\t'

		<< dmp_dbl(S.EIGENVALUE.X, 8) << '\t' << dmp_dbl(S.EIGENVALUE.Y, 8) << '\t' << dmp_dbl(S.EIGENVALUE.Z, 8) << '\t'

		<< dmp_dbl(S.S_1.DIPDIR, 8) << '\t' << dmp_dbl(S.S_1.DIP, 8) << '\t'
		<< dmp_dbl(S.S_2.DIPDIR, 8) << '\t' << dmp_dbl(S.S_2.DIP, 8) << '\t'
		<< dmp_dbl(S.S_3.DIPDIR, 8) << '\t' << dmp_dbl(S.S_3.DIP, 8) << '\t'

		<< dmp_dbl(S.stressratio, 8) << '\t'
		<< dmp_dbl(S.delvaux_str, 8) << '\t'
		<< S.regime << '\t'
		<< S.delvaux_rgm << '\t'
		<< dmp_dbl(S.shmax, 8) << '\t'
		<< dmp_dbl(S.shmin, 8) << endl;
	}
}

void dump_RECT_GRID_to_file (const vector <vector <GRID_CENTER> >& IN, const string FN) {

	ofstream o((FN + "_RECT_GRID.csv").c_str());

	o
	<< "CENTER.X d" << '\t' << "CENTER.Y d" << '\t'
	<< "COUNT i" << endl;

	for (size_t i = 0; i < IN.size(); i++) {
		for (size_t j = 0; j < IN.at(i).size(); j++) {

			const GRID_CENTER I = IN.at(i).at(j);

			o
			//<< dmp_dbl(I.CENTER.X, 8) << '\t' << dmp_dbl(I.CENTER.Y, 8) << '\t' << dmp_dbl(I.CENTER.Z, 8) << '\t'

			<< dmp_dbl(I.CENTER.X, 8) << '\t'
			<< dmp_dbl(I.CENTER.Y, 8) << '\t'
			<< I.COUNT << endl;
		}
	}
}

void dump_TRI_GRID_to_file (const vector <TRIANGLE>& T, const string FN) {

	ofstream o((FN + "_TRI_GRID.csv").c_str());

	o
	<< "GROUP i" << '\t'
	<< "CENTER.A.X d" << '\t' << "CENTER.A.Y d" << '\t'
	<< "CENTER.B.X d" << '\t' << "CENTER.B.Y d" << '\t'
	<< "CENTER.C.X d" << '\t' << "CENTER.C.Y d" << '\t'
	<< "COUNT i" << endl;

	for (size_t i = 0; i < T.size(); i++) {

		const TRIANGLE I = T.at(i);

		o
		<< I.GROUP << '\t'
		<< dmp_dbl(I.A.X, 8) << '\t' << dmp_dbl(I.A.Y, 8) << '\t'
		<< dmp_dbl(I.B.X, 8) << '\t' << dmp_dbl(I.B.Y, 8) << '\t'
		<< dmp_dbl(I.C.X, 8) << '\t' << dmp_dbl(I.C.Y, 8) << '\t'
		<< I.COUNT << endl;
	}
}

void dump_DISTANCE_MATRIX_to_file (const vector <vector <double> >& DM, const string FN) {

	ofstream o((FN + "_DIST_MX.csv").c_str());

	for (size_t i = 0; i < DM.at(0).size(); i++) {
		if (i!=0)
			o << '\t';
		o << "COL_" << i << " d";
	}
	o << endl;

	for (size_t i = 0; i < DM.size(); i++) {
		for (size_t j = 0; j < DM.at(i).size(); j++) {
			if (j!=0)
				o << '\t';
			o << DM.at(i).at(j);
		}
		o << endl;
	}
	return;
}

void dump_WHICH_GROUP_to_file (const vector <size_t>& WG, const string FN) {

	ofstream o((FN + "_WCH_GRP.csv").c_str());

	o << "GROUP i" << endl;

	for (size_t i = 0; i < WG.size(); i++) 	o << WG.at(i) << endl;

	return;
}

void dump_HISTOGRAM_to_file (const vector <HISTOGRAM>& H, const string FN) {

	ofstream o((FN + "_HIST.csv").c_str());

	o << "BIN_MIN d" << '\t' << "COUNT i" << '\t' << "GC s" << endl;

	for (size_t i = 0; i < H.size(); i++) 	{

		const HISTOGRAM h = H.at(i);

		o
		<< dmp_dbl(h.BIN_MIN, 8) << '\t'
		<< h.COUNT << '\t'
		<< h.GC << endl;
	}
	return;
}

void dump_VALLEY_to_file (const vector <VALLEY>& V, const string FN) {

	ofstream o;

	o.open ((FN + "_VALLEY.csv").c_str());

	o
	<< "BIN_CENTER d" << '\t'
	<< "BIN_ID i" << '\t'
	<< "DIR s" << endl;

	for (size_t i = 0; i < V.size(); i++) 	{

		VALLEY v = V.at(i);

		o
		<< dmp_dbl(v.BIN_CENTER, 8) << '\t'
		<< v.BIN_ID << '\t'
		<< v.DIR << endl;
	}
	return;
}

void dump_ROSENUMBER_to_file (const vector <ROSENUMBER>& R, const string FN) {

	ofstream o;

	o.open ((FN + ".csv").c_str());

	o << fixed << setprecision (8) << flush;

	o << "PLN_NUM d" << '\t' << "LIN_NUM d" << endl;

	for (size_t i = 0; i < R.size(); i++) 	{

		ROSENUMBER r = R.at(i);

		o
		<< dmp_dbl (r.PLN_NUM, 8) << '\t'
		<< dmp_dbl (r.LIN_NUM, 8) << endl;
	}
	return;
}

void dump_INTERVAL_to_file (ofstream& o) {

	vector <vector <WELL_INTERVAL> > I = RETURN_INTERVAL ();

	o
	<< "DEPTH d" << '\t'
	<< "SIZE i" << '\t'
	<< "MIN d" << '\t'
	<< "MAX d" << '\t'
	<< "INT_AV_D.X d" << '\t' << "INT_AV_D.Y d" << '\t' << "INT_AV_D.Z d" << '\t'
	<< "INT_AV_DD.DIPDIR d" << '\t' << "INT_AV_DD.DIP d" << '\t'
	<< "INT_AV_DD_STDEV d" << '\t' << "INT_AV_D_STDEV d" << '\t'
	<< "DD_DERIV d" << '\t' << "D_DERIV d" << endl;

	for (size_t i = 0; i < I.size(); i++) {
		for (size_t j = 0; j < I.at(i).size(); j++) {

			WELL_INTERVAL w = I.at(i).at(j);

			o
			<< dmp_dbl (w.DEPTH, 8) << '\t'
			<< w.SIZE << '\t'
			<< dmp_dbl (w.MIN, 8) << '\t'
			<< dmp_dbl (w.MAX, 8) << '\t'
			<< dmp_dbl (w.INT_AV_D.X, 8) << '\t'
			<< dmp_dbl (w.INT_AV_D.Y, 8) << '\t'
			<< dmp_dbl (w.INT_AV_D.Z, 8) << '\t'
			<< dmp_dbl (w.INT_AV_DD.DIPDIR, 8) << '\t'
			<< dmp_dbl (w.INT_AV_DD.DIP, 8) << '\t'
			<< dmp_dbl (w.INT_AV_DD_STDEV, 8) << '\t'
			<< dmp_dbl (w.INT_AV_D_STDEV, 8) << '\t'
			<< dmp_dbl (w.DD_DERIV, 8) << '\t'
			<< dmp_dbl (w.D_DERIV, 8) << endl;
		}
		//o << endl;
	}
	return;
}

void standard_output_INTERVAL () {

	string T = "ST_WELL_INTERVAL";

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	ofstream o;
	o.open ((T + ".csv").c_str());

	dump_INTERVAL_to_file (o);

	return;
}

void dump_FREQUENCY_to_file (ofstream& o) {

	vector <vector <WELL_FREQUENCY> > F = RETURN_FREQUENCY();

	o
	<< "DEPTH d" << '\t'
	<< "FREQ d"	<< '\t'
	<< "DERIV_DEPTH d" << '\t'
	<< "DERIV d" << endl;

	for (size_t i = 0; i < F.size(); i++) {
		for (size_t j = 0; j < F.at(i).size(); j++) {

			WELL_FREQUENCY f = F.at(i).at(j);

			o
			<< dmp_dbl (f.DEPTH, 8) << '\t'
			<< dmp_dbl (f.FREQ, 8) << '\t'
			<< dmp_dbl (f.DERIV_DEPTH, 8) << '\t'
			<< dmp_dbl (f.DERIV, 8) << endl;
		}
		//o << endl;
	}
	return;
}

void standard_output_FREQUENCY () {

	string T = "ST_WELL_FREQUENCY";

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	ofstream o;
	o.open ((T + ".csv").c_str());

	dump_FREQUENCY_to_file (o);

	return;
}

void STANDARD_OUTPUT_WELL_GROUPS () {

	standard_output_INTERVAL ();
	standard_output_FREQUENCY ();

	return;
}

void dump_CURVE_to_file (const vector <double>& DEPTH, const vector <double>& VALUE, ofstream& o) {

	o << "DEPTH d" << '\t' << "VALUE d" << endl;

	for (size_t i = 0; i < VALUE.size(); i++) {

		o << dmp_dbl (DEPTH.at(i), 8) << '\t'<< dmp_dbl (VALUE.at(i), 8) << endl;
	}
	return;
}


void STANDARD_OUTPUT_WELL_PS (const vector <double>& DEPTH, const vector <double>& VALUE, const bool DIPDIR, const string TYPE) {

	string T = return_ACTUAL_LOCATION();
	T = T + "_" + return_ACTUAL_FORMATION();
	T = T + "_" + return_ACTUAL_DATATYPE();
	T = T + "_" + return_ACTUAL_GROUPCODE();

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	if (DIPDIR) T = T + "_DIPDIR";
	else T = T + "_DIP";

	T = T + "_" + TYPE;

	ofstream o;
	o.open (("ST_WELL_CURVE" + T + ".csv").c_str());

	dump_CURVE_to_file (DEPTH, VALUE, o);
}

void dump_PEAK_to_file (ofstream& o, const bool PEAK) {

	vector <PEAK_TO_PLOT> P;

	if (PEAK) P = return_PEAK ();
	else P = return_FAULTS();

	o << "DEPTH d" << '\t' << "VALUE d" << '\t' << "COUNT d"<< endl;

	for (size_t i = 0; i < P.size(); i++) {

		ASSERT (PEAK || is_nan (P.at(i).VALUE));

		o
		<< dmp_dbl (P.at(i).DEPTH, 8) << '\t'
		<< dmp_dbl (P.at(i).VALUE, 8) << '\t'
		<< dmp_dbl (P.at(i).COUNT, 8) << endl;
	}
	return;
}

void STANDARD_OUTPUT_PEAKS (const string METHOD) {

	string T = return_ACTUAL_LOCATION();
	T = T + "_" + return_ACTUAL_FORMATION();
	T = T + "_" + return_ACTUAL_DATATYPE();
	T = T + "_" + return_ACTUAL_GROUPCODE();

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	T = T + "_" + METHOD;

	ofstream o;
	o.open (("ST_WELL_PEAKS" + T + ".csv").c_str());

	dump_PEAK_to_file (o, true);
}

void STANDARD_OUTPUT_FAULTS (const string METHOD) {

	string T = return_ACTUAL_LOCATION();
	T = T + "_" + return_ACTUAL_FORMATION();
	T = T + "_" + return_ACTUAL_DATATYPE();
	T = T + "_" + return_ACTUAL_GROUPCODE();

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	T = T + "_" + METHOD;

	ofstream o;
	o.open (("ST_WELL_FAULTS" + T + ".csv").c_str());

	dump_PEAK_to_file (o, false);
}

void dump_INTMINMAX_to_file (const size_t RUN, const size_t SIZE, const double MIN_DEPTH, const double MAX_DEPTH) {

	stringstream s;
	s << fixed << setfill ('0') << setw (4) << RUN << flush;
	string T = s.str();

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	T = T + "_" + return_ACTUAL_DATATYPE();
	T = T + "_" + return_ACTUAL_GROUPCODE();

	ofstream o;
	o.open (("ST_WELL_INTMINMAX_" + T + ".csv").c_str());

	o
	<< "RUN i" << '\t'
	<< "SIZE i" << '\t'
	<< "MIN_DEPTH d" << '\t'
	<< "MAX_DEPTH d" << endl;

	o
	<< RUN << '\t'
	<< SIZE << '\t'
	<< dmp_dbl (MIN_DEPTH, 8) << '\t'
	<< dmp_dbl (MAX_DEPTH, 8) << endl;
}

void dump_STDEV_to_file (const size_t& RUN, const bool DIPDIR, const vector <string> ID_v, const vector <DIPDIR_DIP>& D1_v, const vector <DIPDIR_DIP>& D2_v, const vector <VCTR>& T1_v, const vector <VCTR>& T2_v, const vector <double>& ANG_v, const vector <double>& MSFT_v) {

	stringstream s;
	s << fixed << setfill ('0') << setw (4) << RUN << flush;
	string T = s.str();

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	if (DIPDIR) T = T + "_DIPDIR";
	else T = T + "_DIP";

	T = T + "_" + return_ACTUAL_DATATYPE();
	T = T + "_" + return_ACTUAL_GROUPCODE();

	ofstream o;
	o.open (("ST_WELL_STDEV_" + T + ".csv").c_str());

	o
	<< "ID s" << '\t'
	<< "D1.DIPDIR d" << '\t' << "D1.DIP d" << '\t'
	<< "D2.DIPDIR d" << '\t' << "D2.DIP d" << '\t'
	<< "T1.X d" << '\t' << "T1.Y d" << '\t' << "T1.Z d" << '\t'
	<< "T2.X d" << '\t' << "T2.Y d" << '\t' << "T2.Z d" << '\t'
	<< "ANG d" << '\t'
	<< "MSFT d" << endl;

	for (size_t i = 0; i < ANG_v.size(); i++) {

		o
		<< ID_v.at(i) << '\t'
		<< dmp_dbl (D1_v.at(i).DIPDIR , 8) << '\t' << dmp_dbl (D1_v.at(i).DIP , 8) << '\t'
		<< dmp_dbl (D2_v.at(i).DIPDIR , 8) << '\t' << dmp_dbl (D2_v.at(i).DIP , 8) << '\t'
		<< dmp_dbl (T1_v.at(i).X , 8) << '\t' << dmp_dbl (T1_v.at(i).Y , 8) << '\t' << dmp_dbl (T1_v.at(i).Z , 8) << '\t'
		<< dmp_dbl (T2_v.at(i).X , 8) << '\t' << dmp_dbl (T2_v.at(i).Y , 8) << '\t' << dmp_dbl (T2_v.at(i).Z , 8) << '\t'
		<< dmp_dbl (ANG_v.at(i), 8) << '\t'
		<< dmp_dbl (MSFT_v.at(i), 8) << endl;
	}
	return;
}

void dump_FREQ_PRM_to_file (const vector <string>& ID_v, const vector <double>& INT_v, const vector <double>& DIP_v, const vector <double>& FREQ_v, const vector <double>& FREQ_v2, const vector <double>& MAX_FREQ_v) {

	string T = return_ACTUAL_DATATYPE();

	if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
	else T = T + "_NRM";

	if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

	T = T + "_" + return_ACTUAL_DATATYPE();
	T = T + "_" + return_ACTUAL_GROUPCODE();

	ofstream o;
	o.open (("ST_WELL_FREQ_PRM_" + T + ".csv").c_str());

	o
	<< "ID s" << '\t'
	<< "INT d" << '\t'
	<< "DIP d" << '\t'
	<< "FREQ d" << '\t'
	<< "FREQ_NRM d" << '\t'
	<< "FREQ_MAX d" << endl;

	for (size_t i = 0; i < INT_v.size(); i++) {

		o
		<< ID_v.at(i) << '\t'
		<< dmp_dbl (INT_v.at(i), 8) << '\t'
		<< dmp_dbl (DIP_v.at(i), 8) << '\t'
		<< dmp_dbl (FREQ_v.at(i), 8) << '\t'
		<< dmp_dbl (FREQ_v2.at(i), 8) << '\t'
		<< dmp_dbl (MAX_FREQ_v.at(i), 8) << endl;
	}
	return;
}
