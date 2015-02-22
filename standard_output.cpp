// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>


#include "assertions.hpp"
#include "allowed_keys.hpp"
#include "common.h"
#include "inversion.h"
#include "rgf.h"
//#include "rose.h"
#include "run_mode.h"
//#include "ps.h"
#include "rup_clustering.hpp"
#include "settings.hpp"
#include "standard_output.hpp"
#include "structs.h"
#include "valley_method.hpp"

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



void check_standard_output_SF (const vector <GDB>& inGDB, const STRESSFIELD& standard) {

	const STRESSFIELD t = inGDB.at(0).SFV.at(inGDB.at(0).SFV.size() - 1);
	const STRESSFIELD S = standard;

	const bool ST = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);

	//if (inGDB.at(0).ID == "ANG_001") {

		if (! is_in_range (t.EIGENVECTOR1.X, t.EIGENVECTOR1.X, S.EIGENVECTOR1.X)) cout << "EIGENVECTOR1.X test failed: " << t.EIGENVECTOR1.X << " <> " << S.EIGENVECTOR1.X << endl;
		if (! is_in_range (t.EIGENVECTOR1.Y, t.EIGENVECTOR1.Y, S.EIGENVECTOR1.Y)) cout << "EIGENVECTOR1.Y test failed: " << t.EIGENVECTOR1.Y << " <> " << S.EIGENVECTOR1.Y << endl;
		if (! is_in_range (t.EIGENVECTOR1.Z, t.EIGENVECTOR1.Z, S.EIGENVECTOR1.Z)) cout << "EIGENVECTOR1.Z test failed: " << t.EIGENVECTOR1.Z << " <> " << S.EIGENVECTOR1.Z << endl;
		if (! is_in_range (t.EIGENVECTOR2.X, t.EIGENVECTOR2.X, S.EIGENVECTOR2.X)) cout << "EIGENVECTOR2.X test failed: " << t.EIGENVECTOR2.X << " <> " << S.EIGENVECTOR2.X << endl;
		if (! is_in_range (t.EIGENVECTOR2.Y, t.EIGENVECTOR2.Y, S.EIGENVECTOR2.Y)) cout << "EIGENVECTOR2.Y test failed: " << t.EIGENVECTOR2.Y << " <> " << S.EIGENVECTOR2.Y << endl;
		if (! is_in_range (t.EIGENVECTOR2.Z, t.EIGENVECTOR2.Z, S.EIGENVECTOR2.Z)) cout << "EIGENVECTOR2.Z test failed: " << t.EIGENVECTOR2.Z << " <> " << S.EIGENVECTOR2.Z << endl;
		if (! is_in_range (t.EIGENVECTOR3.X, t.EIGENVECTOR3.X, S.EIGENVECTOR3.X)) cout << "EIGENVECTOR3.X test failed: " << t.EIGENVECTOR3.X << " <> " << S.EIGENVECTOR3.X << endl;
		if (! is_in_range (t.EIGENVECTOR3.Y, t.EIGENVECTOR3.Y, S.EIGENVECTOR3.Y)) cout << "EIGENVECTOR3.Y test failed: " << t.EIGENVECTOR3.Y << " <> " << S.EIGENVECTOR3.Y << endl;
		if (! is_in_range (t.EIGENVECTOR3.Z, t.EIGENVECTOR3.Z, S.EIGENVECTOR3.Z)) cout << "EIGENVECTOR3.Z test failed: " << t.EIGENVECTOR3.Z << " <> " << S.EIGENVECTOR3.Z << endl;
		if (! is_in_range (t.EIGENVALUE.X, t.EIGENVALUE.X, S.EIGENVALUE.X)) cout << "EIGENVALUE.X test failed: " << t.EIGENVALUE.X << " <> " << S.EIGENVALUE.X << endl;
		if (! is_in_range (t.EIGENVALUE.Y, t.EIGENVALUE.Y, S.EIGENVALUE.Y)) cout << "EIGENVALUE.Y test failed: " << t.EIGENVALUE.Y << " <> " << S.EIGENVALUE.Y << endl;
		if (! is_in_range (t.EIGENVALUE.Z, t.EIGENVALUE.Z, S.EIGENVALUE.Z)) cout << "EIGENVALUE.Z test failed: " << t.EIGENVALUE.Z << " <> " << S.EIGENVALUE.Z << endl;
		if (! is_in_range (t.S_1.DIPDIR, t.S_1.DIPDIR, S.S_1.DIPDIR)) cout << "S_1.DIPDIR test failed: " << t.S_1.DIPDIR << " <> " << S.S_1.DIPDIR << endl;
		if (! is_in_range (t.S_1.DIP, t.S_1.DIP, S.S_1.DIP)) cout << "S_1.DIP test failed: " << t.S_1.DIP << " <> " << S.S_1.DIP << endl;
		if (! is_in_range (t.S_2.DIPDIR, t.S_2.DIPDIR, S.S_2.DIPDIR)) cout << "S_2.DIPDIR test failed: " << t.S_2.DIPDIR << " <> " << S.S_2.DIPDIR << endl;
		if (! is_in_range (t.S_2.DIP, t.S_2.DIP, S.S_2.DIP)) cout << "S_2.DIP test failed: " << t.S_2.DIP << " <> " << S.S_2.DIP << endl;
		if (! is_in_range (t.S_3.DIPDIR, t.S_3.DIPDIR, S.S_3.DIPDIR)) cout << "S_3.DIPDIR test failed: " << t.S_3.DIPDIR << " <> " << S.S_3.DIPDIR << endl;
		if (! is_in_range (t.S_3.DIP, t.S_3.DIP, S.S_3.DIP)) cout << "S_3.DIP test failed: " << t.S_3.DIP << " <> " << S.S_3.DIP << endl;
		if (! is_in_range (t.stressratio, t.stressratio, S.stressratio) && ST) cout << "stressratio test failed: " << t.stressratio << " <> " << S.stressratio << endl;
		if (! is_in_range (t.delvaux_str, t.delvaux_str, S.delvaux_str) && ST) cout << "delvaux_str test failed: " << t.delvaux_str << " <> " << S.delvaux_str << endl;
		if (t.regime != S.regime) cout << "regime test failed: " << t.regime << " <> " << S.regime << endl;
		if (t.delvaux_rgm != S.delvaux_rgm) cout << "delvaux_rgm test failed: " << t.delvaux_rgm << " <> " << S.delvaux_rgm << endl;
		if (! is_in_range (t.shmax, t.shmax, S.shmax) && ST) cout << "shmax test failed: " << t.shmax << " <> " << S.shmax << endl;
		if (! is_in_range (t.shmin, t.shmin, S.shmin) && ST) cout << "shmin test failed: " << t.shmin << " <> " << S.shmin << endl;
		cout << "**** END OF TEST" << endl;
	//}
	//else ASSERT_DEAD_END();
}

vector <VALLEY> return_standard_valleygraph () {

	vector <VALLEY> OUT;

	VALLEY buf;

	buf.BIN_CENTER = 12.61672352;
	buf.BIN_ID = 999;
	buf.DIR = "V";
	OUT.push_back(buf);

	buf.BIN_CENTER = 37.53249382;
	buf.BIN_ID = 999;
	buf.DIR = "V";
	OUT.push_back(buf);

	return OUT;
}

void check_standard_output_valley (const vector <VALLEY> V_IN, const vector <VALLEY> V_ST) {

	cout << "**** VALLEY GRAPH FOR RUP CLUSTERING: IF FAILS LISTED BELOW" << endl;

	for (size_t i = 0; i < V_IN.size(); i++) {

		const double IN_BC = V_IN.at(i).BIN_CENTER;
		const double ST_BC = V_ST.at(i).BIN_CENTER;

		const size_t IN_BI = V_IN.at(i).BIN_ID;
		const size_t ST_BI = V_ST.at(i).BIN_ID;

		const string IN_D = V_IN.at(i).DIR;
		const string ST_D = V_ST.at(i).DIR;

		if (! is_in_range (ST_BC, ST_BC, IN_BC)) cout << "Bin center test failed: " << ST_BC << " <> " << IN_BC << endl;
		if (! is_in_range (ST_BI, ST_BI, IN_BI)) cout << "Bin ID test failed: " << ST_BI << " <> " << IN_BI << endl;
		if (ST_D != IN_D) cout << "Bin DIR test failed: " << ST_D << " <> " << IN_D << endl;
	}
	cout << "**** END OF VALLEY TEST" << endl;
}

vector <HISTOGRAM> return_standard_histogram () {

	vector <HISTOGRAM> OUT;

	HISTOGRAM buf;

	buf.BIN_MIN = 0.27330965;
	buf.COUNT = 16;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 6.50225223;
	buf.COUNT = 5;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 12.73119480;
	buf.COUNT = 1;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 18.96013738;
	buf.COUNT = 3;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 25.18907996;
	buf.COUNT = 10;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 31.41802253;
	buf.COUNT = 2;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 37.64696511;
	buf.COUNT = 0;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 43.87590769;
	buf.COUNT = 2;
	buf.GC = "";
	OUT.push_back(buf);

	buf.BIN_MIN = 50.10485026;
	buf.COUNT = 3;
	buf.GC = "";
	OUT.push_back(buf);

	return OUT;
}

void check_standard_output_histogram (const vector <HISTOGRAM> H_IN, const vector <HISTOGRAM> H_ST) {

	cout << "**** HISTOGRAM FOR RUP CLUSTERING: IF FAILS LISTED BELOW" << endl;

	for (size_t i = 0; i < H_IN.size(); i++) {

		const double IN_BM = H_IN.at(i).BIN_MIN;
		const double ST_BM = H_ST.at(i).BIN_MIN;

		const size_t IN_C = H_IN.at(i).COUNT;
		const size_t ST_C = H_ST.at(i).COUNT;

		const string IN_G = H_IN.at(i).GC;
		const string ST_G = H_ST.at(i).GC;

		if (! is_in_range (ST_BM, ST_BM, IN_BM)) cout << "Histogram MIN test failed: " << ST_BM << " <> " << IN_BM << endl;
		if (! is_in_range (ST_C, ST_C, IN_C)) cout << "Histogram COUNT test failed: " << ST_C << " <> " << IN_C << endl;
		if (ST_G != IN_G) cout << "Histogram GROUP CODE test failed: " << ST_G << " <> " << IN_G << endl;
	}
	cout << "**** END OF HISTOGRAM TEST" << endl;
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
		else ASSERT_DEAD_END();
	}
}

void STANDARD_OUTPUT (const vector <GDB>& nGDB, const vector <GDB>& tGDB) {

	if (!is_mode_DEBUG()) return;

	if (is_CHK_LITHOLOGY_DT ()) 	dump_RGF_to_file (nGDB, "ST_DT_LITHOLOGY");
	else if (is_CHK_LINEATION_DT()) dump_RGF_to_file (nGDB, "ST_DT_LINEATION");
	else if (is_CHK_PLANE_DT ()) 	dump_RGF_to_file (nGDB, "ST_DT_PLANE");
	else if (is_CHK_STRIAE_DT ())	dump_RGF_to_file (nGDB, "ST_DT_STRIAE");
	else if (is_CHK_SC_DT ()) 		dump_RGF_to_file (nGDB, "ST_DT_SC");

	else if (is_CHK_ANGELIER() && is_INVERSION_ANGELIER()) 		dump_RGF_to_file (nGDB, "ST_INV_ANGELIER");
	else if (is_CHK_BINGHAM() && is_BINGHAM_USE()) 				dump_RGF_to_file (nGDB, "ST_INV_BINGHAM");
	else if (is_CHK_BRUTEFORCE() && is_INVERSION_BRUTEFORCE()) 	dump_RGF_to_file (nGDB, "ST_INV_BRUTEFORCE");
	else if (is_CHK_FRY() && is_INVERSION_FRY()) 				dump_RGF_to_file (nGDB, "ST_INV_FRY");
	else if (is_CHK_MICHAEL() && is_INVERSION_MICHAEL()) 		dump_RGF_to_file (nGDB, "ST_INV_MICHAEL");
	else if (is_CHK_MOSTAFA() && is_INVERSION_MOSTAFA()) 		dump_RGF_to_file (nGDB, "ST_INV_MOSTAFA");
	else if (is_CHK_SPRANG() && is_INVERSION_SPRANG()) 			dump_RGF_to_file (nGDB, "ST_INV_SPRANG");
	else if (is_CHK_SHAN() && is_INVERSION_SHAN()) 				dump_RGF_to_file (nGDB, "ST_INV_SHAN");
	else if (is_CHK_TURNER() && is_INVERSION_TURNER()) 			dump_RGF_to_file (nGDB, "ST_INV_TURNER");

	//else if (is_CHK_RUP_ANG() && is_RUP_CLUSTERING_RUP()) {

		//const vector <VALLEY> V_IN = return_valleygraph_for_dataset (nGDB, "RUP");

		//void dump_HISTOGRAM_to_file (const vector <HISTOGRAM>& H, const string FN);
		//void dump_VALLEY_to_file (const vector <VALLEY>& V, const string FN);




		//const vector <VALLEY> V_IN = return_valleygraph_for_dataset (nGDB, "RUP");
		//const vector <VALLEY> V_ST = return_standard_valleygraph ();
		//check_standard_output_valley(V_IN, V_ST);

		//const size_t bin_number = return_DATA_ideal_bin_number (GDB_to_table (nGDB, "RUP"));

		//const vector <HISTOGRAM> H_IN = generate_DATA_histogram (GDB_to_table (nGDB, "RUP"), bin_number);
		//const vector <HISTOGRAM> H_ST = return_standard_histogram();
		//check_standard_output_histogram (H_IN, H_ST);
	//}
	else if (is_CHK_RETILT()) dump_RGF_to_file (tGDB, "ST_RETILT");
	else if (is_CHK_AVERAGE())dump_RGF_to_file (nGDB, "ST_AVERAGE");
	else if (is_CHK_COLORS()) dump_RGF_to_file (nGDB, "ST_COLORS");
	else if (is_CHK_FOLDSURFACE()) dump_RGF_to_file (nGDB, "ST_FOLDSURFACE");
	else {};


	/*

	else if (is_CHK_RUP_ANG() && is_RUP_CLUSTERING_RUP()) {

		const vector <VALLEY> V_IN = return_valleygraph_for_dataset (nGDB, "RUP");
		const vector <VALLEY> V_ST = return_standard_valleygraph ();
		check_standard_output_valley(V_IN, V_ST);

		const size_t bin_number = return_DATA_ideal_bin_number (GDB_to_table (nGDB, "RUP"));

		const vector <HISTOGRAM> H_IN = generate_DATA_histogram (GDB_to_table (nGDB, "RUP"), bin_number);
		const vector <HISTOGRAM> H_ST = return_standard_histogram();
		check_standard_output_histogram (H_IN, H_ST);
	}

	 */
}

void dump_RGF_to_file (const vector <GDB>& inGDB, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	o
	<< "ID" << '\t' << "iID" << '\t'
	<< "N.X" << '\t' << "N.Y" << '\t' << "N.Z" << '\t'
	<< "D.X" << '\t' << "D.Y" << '\t'<< "D.Z" << '\t'
	<< "S.X" << '\t' << "S.Y" << '\t'<< "S.Z" << '\t'
	<< "NC.X" << '\t' << "NC.Y" << '\t'<< "NC.Z" << '\t'
	<< "DC.X" << '\t' << "DC.Y" << '\t'<< "DC.Z" << '\t'
	<< "SC.X" << '\t' << "SC.Y" << '\t'<< "SC.Z" << '\t'
	<< "SV.X" << '\t' << "SV.Y" << '\t'<< "SV.Z" << '\t'

	<< "LPITCH" << '\t'
	<< "LPITCHSENSE" << '\t'
	<< "PITCHANGLE" << '\t'

	<< "MISFIT" << '\t'
	<< "LINEATION" << '\t'
	<< "UPWARD" << '\t'
	<< "OFFSET" << '\t'
	<< "UP" << '\t'

	<< "DEPTH" << '\t'
	<< "GC" << '\t'
	<< "COLOR" << '\t'
	<< "LOC" << '\t'
	<< "LOCX" << '\t'
	<< "LOCY" << '\t'
	<< "FORMATION" << '\t'
	<< "DATATYPE" << '\t'

	<< "DIPDIR" << '\t'
	<< "DIP" << '\t'
	<< "LDIR" << '\t'
	<< "LDIP" << '\t'
	<< "corr.DIPDIR" << '\t'
	<< "corr.DIP" << '\t'
	<< "corrL.DIPDIR" << '\t'
	<< "corrL.DIP" << '\t'

	<< "PALEON" << '\t'
	<< "COMMENT" << '\t'
	<< "DATAGROUP" << '\t'
	<< "PSCOLOR" << '\t'
	<< "DASHED" << '\t'

	<< "ptnP.X" << '\t' << "ptnP.Y" << '\t'<< "ptnP.Z" << '\t'
	<< "ptnT.X" << '\t' << "ptnT.Y" << '\t'<< "ptnT.Z" << '\t'
	<< "ptnN.X" << '\t' << "ptnN.Y" << '\t'<< "ptnN.Z" << '\t'

	<< "ptnPd.DIPDIR" << '\t'
	<< "ptnPd.DIP" << '\t'
	<< "ptnTd.DIPDIR" << '\t'
	<< "ptnTd.DIP" << '\t'
	<< "ptnNd.DIPDIR" << '\t'
	<< "ptnNd.DIP" << '\t'

	<< "avD.X" << '\t' << "avD.Y" << '\t'<< "avD.Z" << '\t'
	<< "avS0D.X" << '\t' << "avS0D.Y" << '\t'<< "avS0D.Z" << '\t'
	<< "avS0N.X" << '\t' << "avS0N.Y" << '\t'<< "avS0N.Z" << '\t'

	<< "avS0d.DIPDIR" << '\t'
	<< "avS0d.DIP" << '\t'
	<< "avd.DIPDIR" << '\t'
	<< "avd.DIP" << '\t'
	<< "avS0offset" << '\t'

	<< "fold_great_circle_N.X" << '\t'
	<< "fold_great_circle_N.Y" << '\t'
	<< "fold_great_circle_N.Z" << '\t'

	<< "SHEAR_S.X" << '\t' << "SHEAR_S.Y" << '\t'<< "SHEAR_S.Z" << '\t'
	<< "NORMAL_S.X" << '\t' << "NORMAL_S.Y" << '\t'<< "NORMAL_S.Z" << '\t'
	<< "UPSILON.X" << '\t' << "UPSILON.Y" << '\t'<< "UPSILON.Z" << '\t'

	<< "lambda" << '\t'
	<< "ANG" << '\t'
	<< "RUP" << '\t' << endl;

	for (size_t i = 0; i < inGDB.size(); i++) {

		GDB T = inGDB.at(i);

		o
		<< fixed << setprecision (8)
		<< T.ID << '\t' << T.iID << '\t'
		<< T.N.X << '\t' << T.N.Y << '\t' << T.N.Z << '\t'
		<< T.D.X << '\t' << T.D.Y << '\t'<< T.D.Z << '\t'
		<< T.S.X << '\t' << T.S.Y << '\t'<< T.S.Z << '\t'
		<< T.NC.X << '\t' << T.NC.Y << '\t'<< T.NC.Z << '\t'
		<< T.DC.X << '\t' << T.DC.Y << '\t'<< T.DC.Z << '\t'
		<< T.SC.X << '\t' << T.SC.Y << '\t'<< T.SC.Z << '\t'
		<< T.SV.X << '\t' << T.SV.Y << '\t'<< T.SV.Z << '\t'

		<< T.LPITCH << '\t'
		<< T.LPITCHSENSE << '\t'
		<< T.PITCHANGLE << '\t'

		<< T.MISFIT << '\t'
		<< T.LINEATION << '\t'
		<< T.UPWARD << '\t'
		<< T.OFFSET << '\t'
		<< T.UP<< '\t'

		<< T.DEPTH << '\t'
		<< T.GC << '\t'
		<< T.COLOR << '\t'
		<< T.LOC << '\t'
		<< T.LOCX << '\t'
		<< T.LOCY << '\t'
		<< T.FORMATION << '\t'
		<< T.DATATYPE << '\t'

		<< T.DIPDIR << '\t'
		<< T.DIP << '\t'
		<< T.LDIR << '\t'
		<< T.LDIP << '\t'
		<< T.corr.DIPDIR << '\t'
		<< T.corr.DIP << '\t'
		<< T.corrL.DIPDIR << '\t'
		<< T.corrL.DIP << '\t'

		<< T.PALEON << '\t'
		<< T.COMMENT << '\t'
		<< T.DATAGROUP << '\t'
		<< T.PSCOLOR << '\t'
		<< T.DASHED << '\t'

		<< T.ptnP.X << '\t' << T.ptnP.Y << '\t'<< T.ptnP.Z << '\t'
		<< T.ptnT.X << '\t' << T.ptnT.Y << '\t'<< T.ptnT.Z << '\t'
		<< T.ptnN.X << '\t' << T.ptnN.Y << '\t'<< T.ptnN.Z << '\t'

		<< T.ptnPd.DIPDIR << '\t'
		<< T.ptnPd.DIP << '\t'
		<< T.ptnTd.DIPDIR << '\t'
		<< T.ptnTd.DIP << '\t'
		<< T.ptnNd.DIPDIR << '\t'
		<< T.ptnNd.DIP << '\t'

		<< T.avD.X << '\t' << T.avD.Y << '\t'<< T.avD.Z << '\t'
		<< T.avS0D.X << '\t' << T.avS0D.Y << '\t'<< T.avS0D.Z << '\t'
		<< T.avS0N.X << '\t' << T.avS0N.Y << '\t'<< T.avS0N.Z << '\t'

		<< T.avS0d.DIPDIR << '\t'
		<< T.avS0d.DIP << '\t'
		<< T.avd.DIPDIR << '\t'
		<< T.avd.DIP << '\t'
		<< T.avS0offset << '\t'

		<< T.fold_great_circle_N.X << '\t'
		<< T.fold_great_circle_N.Y << '\t'
		<< T.fold_great_circle_N.Z << '\t'

		<< T.SHEAR_S.X << '\t' << T.SHEAR_S.Y << '\t'<< T.SHEAR_S.Z << '\t'
		<< T.NORMAL_S.X << '\t' << T.NORMAL_S.Y << '\t'<< T.NORMAL_S.Z << '\t'
		<< T.UPSILON.X << '\t' << T.UPSILON.Y << '\t'<< T.UPSILON.Z << '\t'

		<< T.lambda << '\t'
		<< T.ANG << '\t'
		<< T.RUP << '\t' << endl;

		dump_SF_to_file (T.SFV, o);
	}

	return;
}

void dump_SF_to_file (const vector <STRESSFIELD>& SF, ofstream& o) {

	//ofstream o;

	//o.open ((FN + ".txt").c_str());

	o
	<< fixed << setprecision (8)
	<< "EIGENVECTOR1.X" << '\t' << "EIGENVECTOR1.Y" << '\t' << "EIGENVECTOR1.Z" << '\t'
	<< "EIGENVECTOR2.X" << '\t' << "EIGENVECTOR2.Y" << '\t' << "EIGENVECTOR2.Z" << '\t'
	<< "EIGENVECTOR3.X" << '\t' << "EIGENVECTOR3.Y" << '\t' << "EIGENVECTOR3.Z" << '\t'

	<< "EIGENVALUE.X" << '\t' << "EIGENVALUE.Y" << '\t' << "EIGENVALUE.Z" << '\t'

	<< "S_1.DIPDIR" << '\t' << "S_1.DIP" << '\t'
	<< "S_2.DIPDIR" << '\t' << "S_2.DIP" << '\t'
	<< "S_3.DIPDIR" << '\t' << "S_3.DIP" << '\t'

	<< "stressratio" << '\t'
	<< "delvaux_str" << '\t'
	<< "regime" << '\t'
	<< "delvaux_rgm" << '\t'
	<< "shmax" << '\t'
	<< "shmin" << '\t' << endl;

	for (size_t i = 0; i < SF.size(); i++) {

		const STRESSFIELD S = SF.at(i);

		o
		<< fixed << setprecision (8)
		<< S.EIGENVECTOR1.X << '\t' << S.EIGENVECTOR1.Y << '\t' << S.EIGENVECTOR1.Z << '\t'
		<< S.EIGENVECTOR2.X << '\t' << S.EIGENVECTOR2.Y << '\t' << S.EIGENVECTOR2.Z << '\t'
		<< S.EIGENVECTOR3.X << '\t' << S.EIGENVECTOR3.Y << '\t' << S.EIGENVECTOR3.Z << '\t'

		<< S.EIGENVALUE.X << '\t' << S.EIGENVALUE.Y << '\t' << S.EIGENVALUE.Z << '\t'

		<< S.S_1.DIPDIR << '\t' << S.S_1.DIP << '\t'
		<< S.S_2.DIPDIR << '\t' << S.S_2.DIP << '\t'
		<< S.S_3.DIPDIR << '\t' << S.S_3.DIP << '\t'

		<< S.stressratio << '\t'
		<< S.delvaux_str << '\t'
		<< S.regime << '\t'
		<< S.delvaux_rgm << '\t'
		<< S.shmax << '\t'
		<< S.shmin << '\t' << endl;
	}
}

void dump_RECT_GRID_to_file (const vector <vector <GRID_CENTER> >& IN, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	o
	<< fixed << setprecision (8)
	<< "CENTER.X" << '\t' << "CENTER.Y" << '\t' << "CENTER.Z" << '\t'
	<< "COUNT" << endl;

	for (size_t i = 0; i < IN.size(); i++) {
		for (size_t j = 0; j < IN.at(i).size(); j++) {

			const GRID_CENTER I = IN.at(i).at(j);

			o
			<< fixed << setprecision (8)
			<< I.CENTER.X << '\t' << I.CENTER.Y << '\t' << I.CENTER.Z << '\t'
			<< I.COUNT << endl;
		}
	}
}

void dump_TRI_GRID_to_file (const vector <TRIANGLE>& T, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	o
	<< fixed << setprecision (8)
	<< "GROUP" << '\t'
	<< "CENTER.A.X" << '\t' << "CENTER.A.Y" << '\t' << "CENTER.A.Z" << '\t'
	<< "CENTER.B.X" << '\t' << "CENTER.B.Y" << '\t' << "CENTER.B.Z" << '\t'
	<< "CENTER.C.X" << '\t' << "CENTER.C.Y" << '\t' << "CENTER.C.Z" << '\t'
	<< "COUNT" << endl;

	for (size_t i = 0; i < T.size(); i++) {

		const TRIANGLE I = T.at(i);

		o
		<< fixed << setprecision (8)
		<< I.GROUP << '\t'
		<< I.A.X << '\t' << I.A.Y << '\t' << I.A.Z << '\t'
		<< I.B.X << '\t' << I.B.Y << '\t' << I.B.Z << '\t'
		<< I.C.X << '\t' << I.C.Y << '\t' << I.C.Z << '\t'
		<< I.COUNT << endl;
	}
}

void dump_DISTANCE_MATRIX_to_file (const vector <vector <double> >& DM, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	o << fixed << setprecision (8) << flush;

	for (size_t i = 0; i < DM.size(); i++) {
		for (size_t j = 0; j < DM.at(i).size(); j++) {

			o << DM.at(i).at(j) << '\t' << flush;
		}
		o << endl;
	}
	return;
}

void dump_WHICH_GROUP_to_file (const vector <size_t>& WG, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	for (size_t i = 0; i < WG.size(); i++) 	o << WG.at(i) << endl;

	return;
}

void dump_HISTOGRAM_to_file (const vector <HISTOGRAM>& H, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	o << fixed << setprecision (8) << flush;

	o
	<< "BIN_MIN" << '\t'
	<< "COUNT" << '\t'
	<< "GC" << endl;

	for (size_t i = 0; i < H.size(); i++) 	{

		const HISTOGRAM h = H.at(i);

		o
		<< h.BIN_MIN << '\t'
		<< h.COUNT << '\t'
		<< h.GC << endl;
	}
	return;
}

void dump_VALLEY_to_file (const vector <VALLEY>& V, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	o << fixed << setprecision (8) << flush;

	o
	<< "BIN_CENTER" << '\t'
	<< "BIN_ID" << '\t'
	<< "DIR" << endl;

	for (size_t i = 0; i < V.size(); i++) 	{

		VALLEY v = V.at(i);

		o
		<< v.BIN_CENTER << '\t'
		<< v.BIN_ID << '\t'
		<< v.DIR << endl;
	}
	return;
}

void dump_ROSENUMBER_to_file (const vector <ROSENUMBER>& R, const string FN) {

	ofstream o;

	o.open ((FN + ".txt").c_str());

	o << fixed << setprecision (8) << flush;

	o
	<< "PLN_NUM" << '\t'
	<< "LIN_NUM" << endl;

	for (size_t i = 0; i < R.size(); i++) 	{

		ROSENUMBER r = R.at(i);

		o
		<< r.PLN_NUM << '\t'
		<< r.LIN_NUM << endl;
	}
	return;
}

