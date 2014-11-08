// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
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

STRESSFIELD init_standard_GDB_PTN () {

	cout << "**** INVERSION TEST OF TURNER (1953): IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

    //- For 'ANG002' location, regression after Turner (1953):
    //- Original : s1: 252/83, s2: 053/07, s3: 143/01,   RADIAL EXTENSIVE, R: 0.121, R': 0.121, av. misfit: 14.3 deg.
    //- Corrected: s1: 048/17, s2: 240/72, s3: 140/03,      TRANSPRESSIVE, R: 0.121, R': 1.879, av. misfit: 14.3 deg.

	OUT.EIGENVECTOR1.X = -0.11748082;
	OUT.EIGENVECTOR1.Y = -0.03841782;
	OUT.EIGENVECTOR1.Z = -0.99233176;
	OUT.EIGENVECTOR2.X = 0.79697008;
	OUT.EIGENVECTOR2.Y = 0.59101492;
	OUT.EIGENVECTOR2.Z = -0.12465977;
	OUT.EIGENVECTOR3.X = 0.60008404;
	OUT.EIGENVECTOR3.Y = -0.7998593;
	OUT.EIGENVECTOR3.Z = -0.0111464;
	OUT.EIGENVALUE.X = 34.98031775;
	OUT.EIGENVALUE.Y = 5.06141371;
	OUT.EIGENVALUE.Z = 0.95915637;
	OUT.S_1.DIPDIR = 251.89155368;
	OUT.S_1.DIP = 82.89991927;
	OUT.S_2.DIPDIR = 53.44020761;
	OUT.S_2.DIP = 7.16110837;
	OUT.S_3.DIPDIR = 143.12141273;
	OUT.S_3.DIP = 0.63865501;
	OUT.stressratio = 0.12057958;
	OUT.delvaux_str = 0.12057958;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "RADIAL EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 143.12141273;
	return OUT;
}

STRESSFIELD init_standard_GDB_ANGELIER () {

	cout << "**** INVERSION TEST OF ANGELIER (1990): IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

	//- For 'ANG002' location, inversion after Angelier (1990):
	//- Original : s1: 227/85, s2: 047/05, s3: 137/00,     PURE EXTENSIVE, R: 0.266, R': 0.266, av. misfit: 14.3 deg.
	//- Corrected: s1: 054/10, s2: 144/03, s3: 249/80,      TRANSPRESSIVE, R: 0.163, R': 2.163, av. misfit: 18.0 deg.

	OUT.EIGENVECTOR1.X = -0.06570381;
	OUT.EIGENVECTOR1.Y = -0.06340412;
	OUT.EIGENVECTOR1.Z = -0.99582274;
	OUT.EIGENVECTOR2.X = 0.7240231;
	OUT.EIGENVECTOR2.Y = 0.6837064;
	OUT.EIGENVECTOR2.Z = -0.09130227;
	OUT.EIGENVECTOR3.X = -0.68663932;
	OUT.EIGENVECTOR3.Y = 0.72699757;
	OUT.EIGENVECTOR3.Z = -0.00098393;
	OUT.EIGENVALUE.X = 1.01055733;
	OUT.EIGENVALUE.Y = -0.27046209;
	OUT.EIGENVALUE.Z = -0.74009525;
	OUT.S_1.DIPDIR = 226.02045493;
	OUT.S_1.DIP = 84.76116458;
	OUT.S_2.DIPDIR = 46.64047527;
	OUT.S_2.DIP = 5.2385304;
	OUT.S_3.DIPDIR = 316.63530651;
	OUT.S_3.DIP = 0.05637501;
	OUT.stressratio = 0.26826177;
	OUT.delvaux_str = 0.26826177;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "PURE EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 316.63530651;
	return OUT;
}

STRESSFIELD init_standard_GDB_BINGHAM () {

	STRESSFIELD OUT;

	cout << "**** TEST OF BINGHAM STATISTICS: IF FAILS LISTED BELOW:" << endl;

	//- 230, using Bingham statistics
	//- Original : e1: 079/71 (73.7%), e2: 331/06 (25.7%), e3: 239/18 (00.7%)
	//- Corrected: e1: 045/17 (73.7%), e2: 142/23 (25.7%), e3: 282/61 (00.7%)

	OUT.EIGENVECTOR1.X = 0.31587385;
	OUT.EIGENVECTOR1.Y = 0.04126733;
	OUT.EIGENVECTOR1.Z = -0.94790333;
	OUT.EIGENVECTOR2.X = -0.43867015;
	OUT.EIGENVECTOR2.Y = 0.89221481;
	OUT.EIGENVECTOR2.Z = -0.107337;
	OUT.EIGENVECTOR3.X = -0.84130388;
	OUT.EIGENVECTOR3.Y = -0.44972185;
	OUT.EIGENVECTOR3.Z = -0.29993007;
	OUT.EIGENVALUE.X = 0.72908928;
	OUT.EIGENVALUE.Y = 0.26597632;
	OUT.EIGENVALUE.Z = 0.00493439;
	OUT.S_1.DIPDIR = 82.55675028;
	OUT.S_1.DIP = 71.4242512;
	OUT.S_2.DIPDIR = 333.81830258;
	OUT.S_2.DIP = 6.16182808;
	OUT.S_3.DIPDIR = 241.87311649;
	OUT.S_3.DIP = 17.45340302;
	OUT.stressratio = NaN();
	OUT.delvaux_str = NaN();
	OUT.regime = "";
	OUT.delvaux_rgm = "";
	OUT.shmax = NaN();
	OUT.shmin = NaN();

	return OUT;
}

STRESSFIELD init_standard_GDB_BRUTEFORCE () {

	cout << "**** INVERSION TEST OF BRUTEFORCE: IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

	//- For 'ANG002' location, brute force iteration
	//- Original : s1: 240/82, s2: 038/07, s3: 128/03,   RADIAL EXTENSIVE, R: 0.110, R': 0.110, av. misfit: 14.9 deg.
	//- Corrected: s1: 050/16, s2: 180/65, s3: 315/18,      TRANSPRESSIVE, R: 0.110, R': 1.890, av. misfit: 14.8 deg.

	OUT.EIGENVECTOR1.X = -0.08381015;
	OUT.EIGENVECTOR1.Y = 0.01123888;
	OUT.EIGENVECTOR1.Z = -0.99641836;
	OUT.EIGENVECTOR2.X = 0.33319494;
	OUT.EIGENVECTOR2.Y = 0.94269753;
	OUT.EIGENVECTOR2.Z = -0.01739255;
	OUT.EIGENVECTOR3.X = 0.93912565;
	OUT.EIGENVECTOR3.Y = -0.33345923;
	OUT.EIGENVECTOR3.Z = -0.08275236;
	OUT.EIGENVALUE.X = 1;
	OUT.EIGENVALUE.Y = 0.13;
	OUT.EIGENVALUE.Z = 0;
	OUT.S_1.DIPDIR = 277.63775809;
	OUT.S_1.DIP = 85.14925585;
	OUT.S_2.DIPDIR = 19.46587398;
	OUT.S_2.DIP = 0.9965698;
	OUT.S_3.DIPDIR = 109.54863468;
	OUT.S_3.DIP = 4.74678921;
	OUT.stressratio = 0.13;
	OUT.delvaux_str = 0.13;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "RADIAL EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 109.54863468;

	return OUT;
}

STRESSFIELD init_standard_GDB_FRY () {

	cout << "**** INVERSION TEST OF FRY (1999): IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

	//- For 'ANG002' location, regression after Fry (1999):
	//- Original : s1: 192/83, s2: 043/06, s3: 313/04,   RADIAL EXTENSIVE, R: 0.113, R': 0.113, av. misfit: 15.4 deg.
	//- Corrected: s1: 059/15, s2: 210/73, s3: 327/08,      TRANSPRESSIVE, R: 0.151, R': 1.849, av. misfit: 16.0 deg.

	OUT.EIGENVECTOR1.X = -0.02514541;
	OUT.EIGENVECTOR1.Y = -0.12080918;
	OUT.EIGENVECTOR1.Z = -0.99235722;
	OUT.EIGENVECTOR2.X = 0.68222109;
	OUT.EIGENVECTOR2.Y = 0.72351376;
	OUT.EIGENVECTOR2.Z = -0.10536713;
	OUT.EIGENVECTOR3.X = -0.73071341;
	OUT.EIGENVECTOR3.Y = 0.67965652;
	OUT.EIGENVECTOR3.Z = -0.06422552;
	OUT.EIGENVALUE.X = 0.82329284;
	OUT.EIGENVALUE.Y = -0.3378936;
	OUT.EIGENVALUE.Z = -0.48539924;
	OUT.S_1.DIPDIR = 191.75775611;
	OUT.S_1.DIP = 82.91172883;
	OUT.S_2.DIPDIR = 43.31745271;
	OUT.S_2.DIP = 6.04831885;
	OUT.S_3.DIPDIR = 312.92673719;
	OUT.S_3.DIP = 3.68238593;
	OUT.stressratio = 0.11271225;
	OUT.delvaux_str = 0.11271225;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "RADIAL EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 312.92673719;

	return OUT;
}

STRESSFIELD init_standard_GDB_MICHAEL () {

	cout << "**** INVERSION TEST OF MICHAEL (1984): IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

	//- For 'ANG002' location, regression after Michael (1984):
	//- Original : s1: 229/87, s2: 042/03, s3: 132/00,   RADIAL EXTENSIVE, R: 0.097, R': 0.097, av. misfit: 14.0 deg.
	//- Corrected: s1: 051/14, s2: 198/74, s3: 319/08,      TRANSPRESSIVE, R: 0.097, R': 1.903, av. misfit: 14.0 deg.

	OUT.EIGENVECTOR1.X = -0.0426383;
	OUT.EIGENVECTOR1.Y = -0.03832071;
	OUT.EIGENVECTOR1.Z = -0.9983554;
	OUT.EIGENVECTOR2.X = 0.67040216;
	OUT.EIGENVECTOR2.Y = 0.73980315;
	OUT.EIGENVECTOR2.Z = -0.05702838;
	OUT.EIGENVECTOR3.X = 0.74077184;
	OUT.EIGENVECTOR3.Y = -0.67173121;
	OUT.EIGENVECTOR3.Z = -0.00585366;
	OUT.EIGENVALUE.X = 1.51578306;
	OUT.EIGENVALUE.Y = -0.64114887;
	OUT.EIGENVALUE.Z = -0.87463419;
	OUT.S_1.DIPDIR = 228.05272084;
	OUT.S_1.DIP = 86.71354465;
	OUT.S_2.DIPDIR = 42.18255337;
	OUT.S_2.DIP = 3.26925933;
	OUT.S_3.DIPDIR = 132.20171162;
	OUT.S_3.DIP = 0.33539184;
	OUT.stressratio = 0.09767555;
	OUT.delvaux_str = 0.09767555;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "RADIAL EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 132.20171162;

	return OUT;
}

STRESSFIELD init_standard_GDB_MOSTAFA () {

	cout << "**** INVERSION TEST OF MOSTAFA (2004): IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

	//- For 'ANG002' location,inversion after Mostafa (2005):
	//- Original : s1: 226/85, s2: 043/05, s3: 133/00,   RADIAL EXTENSIVE, R: 0.099, R': 0.099, av. misfit: 14.6 deg.
	//- Corrected: s1: 062/13, s2: 197/72, s3: 329/12,      TRANSPRESSIVE, R: 0.126, R': 1.874, av. misfit: 16.6 deg.

	OUT.EIGENVECTOR1.X = -0.05456745;
	OUT.EIGENVECTOR1.Y = -0.05375304;
	OUT.EIGENVECTOR1.Z = -0.99706219;
	OUT.EIGENVECTOR2.X = 0.69077421;
	OUT.EIGENVECTOR2.Y = 0.71900516;
	OUT.EIGENVECTOR2.Z = -0.07656744;
	OUT.EIGENVECTOR3.X = 0.72100859;
	OUT.EIGENVECTOR3.Y = -0.69292293;
	OUT.EIGENVECTOR3.Z = -0.00210307;
	OUT.EIGENVALUE.X = 1.0084087;
	OUT.EIGENVALUE.Y = -0.42575155;
	OUT.EIGENVALUE.Z = -0.58265715;
	OUT.S_1.DIPDIR = 225.43077486;
	OUT.S_1.DIP = 85.60705144;
	OUT.S_2.DIPDIR = 43.85280028;
	OUT.S_2.DIP = 4.39128917;
	OUT.S_3.DIPDIR = 133.86205361;
	OUT.S_3.DIP = 0.12049709;
	OUT.stressratio = 0.09861666;
	OUT.delvaux_str = 0.09861666;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "RADIAL EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 133.86205361;

	return OUT;
}

STRESSFIELD init_standard_GDB_NDA () {

	cout << "**** INVERSION TEST OF SPANG (1972): IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

	//- For 'ANG002' location, regression after Sprang (1972):
    //- Original : s1: 246/83, s2: 053/06, s3: 143/02,     PURE EXTENSIVE, R: 0.423, R': 0.423, av. misfit: 18.6 deg.
    //- Corrected: s1: 049/17, s2: 238/73, s3: 140/02,   PURE STRIKE SLIP, R: 0.423, R': 1.577, av. misfit: 18.6 deg.

	OUT.EIGENVECTOR1.X = -0.10488389;
	OUT.EIGENVECTOR1.Y = -0.04612132;
	OUT.EIGENVECTOR1.Z = -0.99341441;
	OUT.EIGENVECTOR2.X = 0.79043381;
	OUT.EIGENVECTOR2.Y = 0.60232922;
	OUT.EIGENVECTOR2.Z = -0.11141774;
	OUT.EIGENVECTOR3.X = 0.60350126;
	OUT.EIGENVECTOR3.Y = -0.79691426;
	OUT.EIGENVECTOR3.Z = -0.02671878;
	OUT.EIGENVALUE.X = 18.88800625;
	OUT.EIGENVALUE.Y = -1.84291668;
	OUT.EIGENVALUE.Z = -17.04508956;
	OUT.S_1.DIPDIR = 246.26313403;
	OUT.S_1.DIP = 83.42079307;
	OUT.S_2.DIPDIR = 52.69174374;
	OUT.S_2.DIP = 6.39704867;
	OUT.S_3.DIPDIR = 142.8634404;
	OUT.S_3.DIP = 1.53105552;
	OUT.stressratio = 0.42306883;
	OUT.delvaux_str = 0.42306883;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "PURE EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 142.8634404;

	return OUT;
}

STRESSFIELD init_standard_GDB_SHAN () {

	cout << "**** INVERSION TEST OF SHAN (2003): IF FAILS LISTED BELOW" << endl;

	STRESSFIELD OUT;

	//- For 'ANG002' location, regression after Shan et al. (2003):
	//- Original : s1: 184/82, s2: 045/06, s3: 314/06,   RADIAL EXTENSIVE, R: 0.141, R': 0.141, av. misfit: 15.6 deg.
	//- Corrected: s1: 063/16, s2: 220/73, s3: 331/06,      TRANSPRESSIVE, R: 0.192, R': 1.808, av. misfit: 16.7 deg.

	OUT.EIGENVECTOR1.X = -0.00841557;
	OUT.EIGENVECTOR1.Y = -0.14550785;
	OUT.EIGENVECTOR1.Z = -0.98932131;
	OUT.EIGENVECTOR2.X = 0.7064687;
	OUT.EIGENVECTOR2.Y = 0.69932143;
	OUT.EIGENVECTOR2.Z = -0.10886462;
	OUT.EIGENVECTOR3.X = -0.70769425;
	OUT.EIGENVECTOR3.Y = 0.6998407;
	OUT.EIGENVECTOR3.Z = -0.09691155;
	OUT.EIGENVALUE.X = 1.3755024;
	OUT.EIGENVALUE.Y = -0.53247901;
	OUT.EIGENVALUE.Z = -0.84302339;
	OUT.S_1.DIPDIR = 183.31006248;
	OUT.S_1.DIP = 81.61922942;
	OUT.S_2.DIPDIR = 45.29129863;
	OUT.S_2.DIP = 6.24987028;
	OUT.S_3.DIPDIR = 314.68031281;
	OUT.S_3.DIP = 5.56135138;
	OUT.stressratio = 0.13997781;
	OUT.delvaux_str = 0.13997781;
	OUT.regime = "EXTENSIONAL";
	OUT.delvaux_rgm = "RADIAL EXTENSIVE";
	OUT.shmax = 999.99;
	OUT.shmin = 314.68031281;

	return OUT;
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

vector <vector <double> > return_standard_distance_matrix () {

	vector <vector <double> > out;

	vector <double> buf;

	buf.push_back (5.40986119);	buf.push_back (0.00256259);	buf.push_back (2.23599381);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.1144667);	buf.push_back (0.00025457);	buf.push_back (0.94356993);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00239749);	buf.push_back (5.26767964);	buf.push_back (0.14232946);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.94886461);	buf.push_back (0.00004647);	buf.push_back (1.37362102);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.15790412);	buf.push_back (0.588062);	buf.push_back (0.00269723);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.01963733);	buf.push_back (1.52157035);	buf.push_back (0.00022945);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00383105);	buf.push_back (2.21788182);	buf.push_back (0.00269656);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00130673);	buf.push_back (2.62630007);	buf.push_back (0.00704195);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00113063);	buf.push_back (4.92956279);	buf.push_back (0.11182325);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.0002135);	buf.push_back (3.31282266);	buf.push_back (0.02204409);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00113063);	buf.push_back (4.92956279);	buf.push_back (0.11182325);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00013449);	buf.push_back (3.68804723);	buf.push_back (0.03535144);	out.push_back (buf);	buf.clear ();
	buf.push_back (4.31060059);	buf.push_back (0.00011233);	buf.push_back (1.58014509);	out.push_back (buf);	buf.clear ();
	buf.push_back (2.40329656);	buf.push_back (0.00149219);	buf.push_back (0.62639955);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.12228927);	buf.push_back (0.66038364);	buf.push_back (0.00110092);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.05991206);	buf.push_back (0.96363688);	buf.push_back (0.0000842);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.04568192);	buf.push_back (1.0822822);	buf.push_back (0.00003293);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.01216204);	buf.push_back (1.65079172);	buf.push_back (0.00022194);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00397627);	buf.push_back (2.08537124);	buf.push_back (0.00153343);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00171044);	buf.push_back (2.37588727);	buf.push_back (0.00365778);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00003288);	buf.push_back (3.50222659);	buf.push_back (0.0279755);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00199815);	buf.push_back (5.29206609);	buf.push_back (0.14705014);	out.push_back (buf);	buf.clear ();
	buf.push_back (5.06513001);	buf.push_back (0.00125505);	buf.push_back (2.04779487);	out.push_back (buf);	buf.clear ();
	buf.push_back (4.22691787);	buf.push_back (0.00005982);	buf.push_back (1.5442182);	out.push_back (buf);	buf.clear ();
	buf.push_back (4.22691787);	buf.push_back (0.00005982);	buf.push_back (1.5442182);	out.push_back (buf);	buf.clear ();
	buf.push_back (4.22691787);	buf.push_back (0.00005982);	buf.push_back (1.5442182);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.58049836);	buf.push_back (0.00000027);	buf.push_back (1.19030552);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.45747223);	buf.push_back (0.00000067);	buf.push_back (1.12650707);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.01158696);	buf.push_back (1.63680616);	buf.push_back (0.00016379);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.14557208);	buf.push_back (0.56611535);	buf.push_back (0.00175317);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.04334785);	buf.push_back (1.06420239);	buf.push_back (0.00000492);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00726124);	buf.push_back (1.78713254);	buf.push_back (0.00037028);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00008527);	buf.push_back (2.97353178);	buf.push_back (0.01291379);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00000532);	buf.push_back (3.8215956);	buf.push_back (0.0418243);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00020659);	buf.push_back (4.49069564);	buf.push_back (0.08104884);	out.push_back (buf);	buf.clear ();
	buf.push_back (4.96711332);	buf.push_back (0.00120876);	buf.push_back (2.00519806);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.05612266);	buf.push_back (0.94038786);	buf.push_back (0.0000227);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00060093);	buf.push_back (2.51891205);	buf.push_back (0.00521471);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00000166);	buf.push_back (3.79007221);	buf.push_back (0.04110802);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.35397421);	buf.push_back (0.00000135);	buf.push_back (1.08954383);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.12106092);	buf.push_back (0.00001201);	buf.push_back (0.97224398);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.07172556);	buf.push_back (0.82779156);	buf.push_back (0.00010273);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.04196462);	buf.push_back (1.04868992);	buf.push_back (0.00000128);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.01574738);	buf.push_back (1.44713543);	buf.push_back (0.00002032);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00422241);	buf.push_back (1.92994615);	buf.push_back (0.00078927);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00000003);	buf.push_back (3.63324193);	buf.push_back (0.03502137);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00000077);	buf.push_back (3.75874219);	buf.push_back (0.0406183);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00004052);	buf.push_back (4.1473775);	buf.push_back (0.06137132);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.08956774);	buf.push_back (0.0000229);	buf.push_back (0.96242156);	out.push_back (buf);	buf.clear ();
	buf.push_back (2.86761694);	buf.push_back (0.0000875);	buf.push_back (0.85484766);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.28676314);	buf.push_back (0.00001966);	buf.push_back (1.06824634);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.13957073);	buf.push_back (0.55359387);	buf.push_back (0.00170956);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.07074864);	buf.push_back (0.81845673);	buf.push_back (0.0001297);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.04149003);	buf.push_back (1.0357101);	buf.push_back (0.00000615);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.01063055);	buf.push_back (1.57610462);	buf.push_back (0.00012963);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00426453);	buf.push_back (1.90161625);	buf.push_back (0.00085009);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00426453);	buf.push_back (1.90161625);	buf.push_back (0.00085009);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.001813);	buf.push_back (2.17039385);	buf.push_back (0.00232819);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.0000156);	buf.push_back (3.10184528);	buf.push_back (0.01811068);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00000211);	buf.push_back (3.33345761);	buf.push_back (0.0254488);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00005345);	buf.push_back (4.07799753);	buf.push_back (0.06074403);	out.push_back (buf);	buf.clear ();
	buf.push_back (3.25371599);	buf.push_back (0.00004874);	buf.push_back (1.05858333);	out.push_back (buf);	buf.clear ();
	buf.push_back (2.81044824);	buf.push_back (0.0001896);	buf.push_back (0.83928446);	out.push_back (buf);	buf.clear ();
	buf.push_back (1.39354287);	buf.push_back (0.01684067);	buf.push_back (0.2650616);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00462179);	buf.push_back (1.87556761);	buf.push_back (0.00106406);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00035642);	buf.push_back (2.62436725);	buf.push_back (0.00876618);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00014463);	buf.push_back (2.83410056);	buf.push_back (0.01303459);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00005422);	buf.push_back (3.05257252);	buf.push_back (0.01870875);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00002397);	buf.push_back (3.27957613);	buf.push_back (0.02606051);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00011416);	buf.push_back (4.00910811);	buf.push_back (0.0611722);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.0002891);	buf.push_back (4.26737192);	buf.push_back (0.07830264);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00240276);	buf.push_back (5.08178034);	buf.push_back (0.15081599);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00006318);	buf.push_back (3.60529099);	buf.push_back (0.04149463);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.24404208);	buf.push_back (0.3564937);	buf.push_back (0.0097388);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.16978506);	buf.push_back (0.47906841);	buf.push_back (0.00414348);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.02399861);	buf.push_back (1.26055875);	buf.push_back (0.00015557);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.0120715);	buf.push_back (1.53845561);	buf.push_back (0.00044756);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00060354);	buf.push_back (2.58549171);	buf.push_back (0.00973702);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00060354);	buf.push_back (2.58549171);	buf.push_back (0.00973702);	out.push_back (buf);	buf.clear ();
	buf.push_back (0.00011599);	buf.push_back (3.22696109);	buf.push_back (0.02741626);	out.push_back (buf);	buf.clear ();

	return out;
}

void check_standard_distance_matrix (vector <vector <double> > IN, vector <vector <double> > ST) {

	cout << "**** DISTANCE MATRIX CHECK FOR K-MEANS CLUSTERING: IF FAILS LISTED BELOW" << endl;

	cout << fixed << setprecision(8) << flush;

	for (size_t i = 0; i < IN.size(); i++) {
		for (size_t j = 0; j < IN.at(i).size(); j++) {

			const double I = IN.at(i).at(j);
			const double S = ST.at(i).at(j);

			if (! is_in_range (S, S, I)) cout << "Distance matrix check failed at element '.at(" << i << ").at(" << j << ")': " << S << " <> " << I << endl;
		}
	}

	cout << "**** END OF DISTANCE MATRIX CHECK FOR K-MEANS CLUSTERING" << endl;
}

vector <size_t> return_standard_whichgroup () {

	vector <size_t> out;

	out.push_back (2);
	out.push_back (2);
	out.push_back (1);
	out.push_back (2);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (2);
	out.push_back (2);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (2);
	out.push_back (2);
	out.push_back (2);
	out.push_back (2);
	out.push_back (2);
	out.push_back (2);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (2);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (2);
	out.push_back (2);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (2);
	out.push_back (2);
	out.push_back (2);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (2);
	out.push_back (2);
	out.push_back (2);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (3);
	out.push_back (1);
	out.push_back (1);
	out.push_back (1);

	return out;
}

void check_standard_whichgroup (vector <size_t> IN, vector <size_t> ST) {

	cout << "**** WHICHGROUP CHECK FOR K-MEANS CLUSTERING: IF FAILS LISTED BELOW:" << endl;

	for (size_t i = 0; i < IN.size(); i++) {

		const size_t I = IN.at(i);
		const size_t S = ST.at(i);

		if (! is_in_range (S, S, I)) cout << "Whichgroup check failed at element '.at(" << i << ")': " << S << " <> " << I << endl;
	}

	cout << "**** END OF WHICHGROUP CHECK FOR K-MEANS CLUSTERING" << endl;
}

void debug_TRI_GRID (const vector <GDB>& inGDB, const vector <TRIANGLE>& IN) {

	if (is_CHK_CONTOURING() && is_mode_DEBUG()) {

		vector <GDB> temp = sort_by_iID (inGDB);

		if (temp.at(0).ID == "BZ__0066") {

			vector <TRIANGLE> ST = return_standard_TRI_GRID ();
			check_standard_TRI_GRID (IN, ST);
		}
		else {
			cout << "Incorrect file to test TRI_GRID." << endl;
			ASSERT_DEAD_END();
		}
	}
	return;
}

void check_standard_TRI_GRID (const vector <TRIANGLE>& IN, const vector <TRIANGLE>& ST) {

	cout << fixed << setprecision (8) << endl;

	cout << "**** TRI_GRID CHECK FOR CONTOURING: IF FAILS LISTED BELOW:" << endl;

	for (size_t i = 0; i < IN.size(); i++) {

		const TRIANGLE I = IN.at(i);
		const TRIANGLE S = ST.at(i);

		if (! is_in_range (S.A.X, S.A.X, I.A.X)) cout << "TRI_GRID A.X test failed: " << I.A.X << " <> " << S.A.X << endl;
		if (! is_in_range (S.A.Y, S.A.Y, I.A.Y)) cout << "TRI_GRID A.Y test failed: " << I.A.Y << " <> " << S.A.Y << endl;
		if (! is_in_range (S.A.Z, S.A.Z, I.A.Z)) cout << "TRI_GRID A.Z test failed: " << I.A.Z << " <> " << S.A.Z << endl;

		if (! is_in_range (S.B.X, S.B.X, I.B.X)) cout << "TRI_GRID B.X test failed: " << I.B.X << " <> " << S.B.X << endl;
		if (! is_in_range (S.B.Y, S.B.Y, I.B.Y)) cout << "TRI_GRID B.Y test failed: " << I.B.Y << " <> " << S.B.Y << endl;
		if (! is_in_range (S.B.Z, S.B.Z, I.B.Z)) cout << "TRI_GRID B.Z test failed: " << I.B.Z << " <> " << S.B.Z << endl;

		if (! is_in_range (S.B.X, S.B.X, I.B.X)) cout << "TRI_GRID C.X test failed: " << I.C.X << " <> " << S.C.X << endl;
		if (! is_in_range (S.B.Y, S.B.Y, I.B.Y)) cout << "TRI_GRID C.Y test failed: " << I.C.Y << " <> " << S.C.Y << endl;
		if (! is_in_range (S.B.Z, S.B.Z, I.B.Z)) cout << "TRI_GRID C.Z test failed: " << I.C.Z << " <> " << S.C.Z << endl;

		if (! is_in_range (S.COUNT, S.COUNT, I.COUNT)) cout << "TRI_GRID COUNT test failed: " << I.COUNT << " <> " << S.COUNT << endl;

		if (! is_in_range (S.GROUP, S.GROUP, I.GROUP)) cout << "TRI_GRID test failed: " << I.GROUP << " <> " << S.GROUP << endl;
	}
	cout << "**** END TRI_GRID CHECK FOR CONTOURING" << endl;
}
vector <TRIANGLE> return_standard_TRI_GRID () {

	vector <TRIANGLE> out;

	TRIANGLE buf;

	buf.A.X = 0.58778525;	buf.A.Y = 0.80901699;	buf.A.Z = 0;	buf.B.X = 0.5;	buf.B.Y = 0.8660254;	buf.B.Z = 0;	buf.C.X = 0.49384417;	buf.C.Y = 0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.66913061;	buf.A.Y = 0.74314483;	buf.A.Z = 0;	buf.B.X = 0.58778525;	buf.B.Y = 0.80901699;	buf.B.Z = 0;	buf.C.X = 0.58980658;	buf.C.Y = 0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.74314483;	buf.A.Y = 0.66913061;	buf.A.Z = 0;	buf.B.X = 0.66913061;	buf.B.Y = 0.74314483;	buf.B.Z = 0;	buf.C.X = 0.67779286;	buf.C.Y = 0.71841847;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = 0.58778525;	buf.A.Z = 0;	buf.B.X = 0.74314483;	buf.B.Y = 0.66913061;	buf.B.Z = 0;	buf.C.X = 0.75661316;	buf.C.Y = 0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.8660254;	buf.A.Y = 0.5;	buf.A.Z = 0;	buf.B.X = 0.80901699;	buf.B.Y = 0.58778525;	buf.B.Z = 0;	buf.C.X = 0.82520157;	buf.C.Y = 0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.91354546;	buf.A.Y = 0.40673664;	buf.A.Z = 0;	buf.B.X = 0.8660254;	buf.B.Y = 0.5;	buf.B.Z = 0;	buf.C.X = 0.88263054;	buf.C.Y = 0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = 0.30901699;	buf.A.Z = 0;	buf.B.X = 0.91354546;	buf.B.Y = 0.40673664;	buf.B.Z = 0;	buf.C.X = 0.92812345;	buf.C.Y = 0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.9781476;	buf.A.Y = 0.20791169;	buf.A.Z = 0;	buf.B.X = 0.95105652;	buf.B.Y = 0.30901699;	buf.B.Z = 0;	buf.C.X = 0.96106507;	buf.C.Y = 0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.9945219;	buf.A.Y = 0.10452846;	buf.A.Z = 0;	buf.B.X = 0.9781476;	buf.B.Y = 0.20791169;	buf.B.Z = 0;	buf.C.X = 0.98100995;	buf.C.Y = 0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.58778525;	buf.A.Y = 0.80901699;	buf.A.Z = 0;	buf.B.X = 0.49384417;	buf.B.Y = 0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = 0.58980658;	buf.C.Y = 0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.66913061;	buf.A.Y = 0.74314483;	buf.A.Z = 0;	buf.B.X = 0.58980658;	buf.B.Y = 0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = 0.67779286;	buf.C.Y = 0.71841847;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.74314483;	buf.A.Y = 0.66913061;	buf.A.Z = 0;	buf.B.X = 0.67779286;	buf.B.Y = 0.71841847;	buf.B.Z = -0.15643447;	buf.C.X = 0.75661316;	buf.C.Y = 0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = 0.58778525;	buf.A.Z = 0;	buf.B.X = 0.75661316;	buf.B.Y = 0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = 0.82520157;	buf.C.Y = 0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.8660254;	buf.A.Y = 0.5;	buf.A.Z = 0;	buf.B.X = 0.82520157;	buf.B.Y = 0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = 0.88263054;	buf.C.Y = 0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.91354546;	buf.A.Y = 0.40673664;	buf.A.Z = 0;	buf.B.X = 0.88263054;	buf.B.Y = 0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = 0.92812345;	buf.C.Y = 0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = 0.30901699;	buf.A.Z = 0;	buf.B.X = 0.92812345;	buf.B.Y = 0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = 0.96106507;	buf.C.Y = 0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.9781476;	buf.A.Y = 0.20791169;	buf.A.Z = 0;	buf.B.X = 0.96106507;	buf.B.Y = 0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = 0.98100995;	buf.C.Y = 0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 1;	buf.A.Y = 0;	buf.A.Z = 0;	buf.B.X = 0.9945219;	buf.B.Y = 0.10452846;	buf.B.Z = 0;	buf.C.X = 0.98768834;	buf.C.Y = 0;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.98768834;	buf.A.Y = 0;	buf.A.Z = -0.15643447;	buf.B.X = 0.9945219;	buf.B.Y = 0.10452846;	buf.B.Z = 0;	buf.C.X = 0.98100995;	buf.C.Y = 0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.58980658;	buf.A.Y = 0.79224773;	buf.A.Z = -0.15643447;	buf.B.X = 0.49384417;	buf.B.Y = 0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = 0.47552826;	buf.C.Y = 0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.67779286;	buf.A.Y = 0.71841847;	buf.A.Z = -0.15643447;	buf.B.X = 0.58980658;	buf.B.Y = 0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = 0.57896652;	buf.C.Y = 0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.75661316;	buf.A.Y = 0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = 0.67779286;	buf.B.Y = 0.71841847;	buf.B.Z = -0.15643447;	buf.C.X = 0.67249851;	buf.C.Y = 0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.82520157;	buf.A.Y = 0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = 0.75661316;	buf.B.Y = 0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = 0.75452386;	buf.C.Y = 0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.88263054;	buf.A.Y = 0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = 0.82520157;	buf.B.Y = 0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = 0.8236391;	buf.C.Y = 0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.92812345;	buf.A.Y = 0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = 0.88263054;	buf.B.Y = 0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = 0.87866165;	buf.C.Y = 0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.96106507;	buf.A.Y = 0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = 0.92812345;	buf.B.Y = 0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = 0.91865005;	buf.C.Y = 0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.98100995;	buf.A.Y = 0.11466362;	buf.A.Z = -0.15643447;	buf.B.X = 0.96106507;	buf.B.Y = 0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = 0.9429201;	buf.C.Y = 0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.58980658;	buf.A.Y = 0.79224773;	buf.A.Z = -0.15643447;	buf.B.X = 0.47552826;	buf.B.Y = 0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = 0.57896652;	buf.C.Y = 0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.67779286;	buf.A.Y = 0.71841847;	buf.A.Z = -0.15643447;	buf.B.X = 0.57896652;	buf.B.Y = 0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = 0.67249851;	buf.C.Y = 0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.75661316;	buf.A.Y = 0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = 0.67249851;	buf.B.Y = 0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = 0.75452386;	buf.C.Y = 0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.82520157;	buf.A.Y = 0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = 0.75452386;	buf.B.Y = 0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = 0.8236391;	buf.C.Y = 0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.88263054;	buf.A.Y = 0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = 0.8236391;	buf.B.Y = 0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = 0.87866165;	buf.C.Y = 0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.92812345;	buf.A.Y = 0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = 0.87866165;	buf.B.Y = 0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = 0.91865005;	buf.C.Y = 0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.96106507;	buf.A.Y = 0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = 0.91865005;	buf.B.Y = 0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = 0.9429201;	buf.C.Y = 0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.98768834;	buf.A.Y = 0;	buf.A.Z = -0.15643447;	buf.B.X = 0.98100995;	buf.B.Y = 0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = 0.95105652;	buf.C.Y = 0;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = 0;	buf.A.Z = -0.30901699;	buf.B.X = 0.98100995;	buf.B.Y = 0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = 0.9429201;	buf.C.Y = 0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.57896652;	buf.A.Y = 0.75452386;	buf.A.Z = -0.30901699;	buf.B.X = 0.47552826;	buf.B.Y = 0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = 0.44550326;	buf.C.Y = 0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.67249851;	buf.A.Y = 0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = 0.57896652;	buf.B.Y = 0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = 0.55553348;	buf.C.Y = 0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.75452386;	buf.A.Y = 0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = 0.67249851;	buf.B.Y = 0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = 0.653154;	buf.C.Y = 0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.8236391;	buf.A.Y = 0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = 0.75452386;	buf.B.Y = 0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = 0.73618414;	buf.C.Y = 0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.87866165;	buf.A.Y = 0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = 0.8236391;	buf.B.Y = 0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = 0.80276914;	buf.C.Y = 0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.91865005;	buf.A.Y = 0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = 0.87866165;	buf.B.Y = 0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = 0.8514216;	buf.C.Y = 0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.9429201;	buf.A.Y = 0.12413779;	buf.A.Z = -0.30901699;	buf.B.X = 0.91865005;	buf.B.Y = 0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = 0.88105472;	buf.C.Y = 0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.57896652;	buf.A.Y = 0.75452386;	buf.A.Z = -0.30901699;	buf.B.X = 0.44550326;	buf.B.Y = 0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = 0.55553348;	buf.C.Y = 0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.67249851;	buf.A.Y = 0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = 0.55553348;	buf.B.Y = 0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = 0.653154;	buf.C.Y = 0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.75452386;	buf.A.Y = 0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = 0.653154;	buf.B.Y = 0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = 0.73618414;	buf.C.Y = 0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.8236391;	buf.A.Y = 0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = 0.73618414;	buf.B.Y = 0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = 0.80276914;	buf.C.Y = 0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.87866165;	buf.A.Y = 0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = 0.80276914;	buf.B.Y = 0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = 0.8514216;	buf.C.Y = 0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.91865005;	buf.A.Y = 0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = 0.8514216;	buf.B.Y = 0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = 0.88105472;	buf.C.Y = 0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = 0;	buf.A.Z = -0.30901699;	buf.B.X = 0.9429201;	buf.B.Y = 0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = 0.89100652;	buf.C.Y = 0;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.89100652;	buf.A.Y = 0;	buf.A.Z = -0.4539905;	buf.B.X = 0.9429201;	buf.B.Y = 0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = 0.88105472;	buf.C.Y = 0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.55553348;	buf.A.Y = 0.69661695;	buf.A.Z = -0.4539905;	buf.B.X = 0.44550326;	buf.B.Y = 0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = 0.4045085;	buf.C.Y = 0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.653154;	buf.A.Y = 0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = 0.55553348;	buf.B.Y = 0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = 0.5200261;	buf.C.Y = 0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.73618414;	buf.A.Y = 0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = 0.653154;	buf.B.Y = 0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = 0.61974297;	buf.C.Y = 0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.80276914;	buf.A.Y = 0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = 0.73618414;	buf.B.Y = 0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = 0.70062927;	buf.C.Y = 0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.8514216;	buf.A.Y = 0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = 0.80276914;	buf.B.Y = 0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = 0.7602273;	buf.C.Y = 0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.88105472;	buf.A.Y = 0.13279763;	buf.A.Z = -0.4539905;	buf.B.X = 0.8514216;	buf.B.Y = 0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = 0.79672621;	buf.C.Y = 0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.55553348;	buf.A.Y = 0.69661695;	buf.A.Z = -0.4539905;	buf.B.X = 0.4045085;	buf.B.Y = 0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = 0.5200261;	buf.C.Y = 0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.653154;	buf.A.Y = 0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = 0.5200261;	buf.B.Y = 0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = 0.61974297;	buf.C.Y = 0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.73618414;	buf.A.Y = 0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = 0.61974297;	buf.B.Y = 0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = 0.70062927;	buf.C.Y = 0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.80276914;	buf.A.Y = 0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = 0.70062927;	buf.B.Y = 0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = 0.7602273;	buf.C.Y = 0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.8514216;	buf.A.Y = 0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = 0.7602273;	buf.B.Y = 0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = 0.79672621;	buf.C.Y = 0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.89100652;	buf.A.Y = 0;	buf.A.Z = -0.4539905;	buf.B.X = 0.88105472;	buf.B.Y = 0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = 0.80901699;	buf.C.Y = 0;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = 0;	buf.A.Z = -0.58778525;	buf.B.X = 0.88105472;	buf.B.Y = 0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = 0.79672621;	buf.C.Y = 0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.5200261;	buf.A.Y = 0.61974297;	buf.A.Z = -0.58778525;	buf.B.X = 0.4045085;	buf.B.Y = 0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = 0.35355339;	buf.C.Y = 0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.61974297;	buf.A.Y = 0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = 0.5200261;	buf.B.Y = 0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = 0.47314679;	buf.C.Y = 0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.70062927;	buf.A.Y = 0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = 0.61974297;	buf.B.Y = 0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = 0.5720614;	buf.C.Y = 0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.7602273;	buf.A.Y = 0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = 0.70062927;	buf.B.Y = 0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = 0.64597419;	buf.C.Y = 0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.79672621;	buf.A.Y = 0.14048433;	buf.A.Z = -0.58778525;	buf.B.X = 0.7602273;	buf.B.Y = 0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = 0.6916548;	buf.C.Y = 0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.5200261;	buf.A.Y = 0.61974297;	buf.A.Z = -0.58778525;	buf.B.X = 0.35355339;	buf.B.Y = 0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = 0.47314679;	buf.C.Y = 0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.61974297;	buf.A.Y = 0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = 0.47314679;	buf.B.Y = 0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = 0.5720614;	buf.C.Y = 0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.70062927;	buf.A.Y = 0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = 0.5720614;	buf.B.Y = 0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = 0.64597419;	buf.C.Y = 0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.7602273;	buf.A.Y = 0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = 0.64597419;	buf.B.Y = 0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = 0.6916548;	buf.C.Y = 0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = 0;	buf.A.Z = -0.58778525;	buf.B.X = 0.79672621;	buf.B.Y = 0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = 0.70710678;	buf.C.Y = 0;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.70710678;	buf.A.Y = 0;	buf.A.Z = -0.70710678;	buf.B.X = 0.79672621;	buf.B.Y = 0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = 0.6916548;	buf.C.Y = 0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.47314679;	buf.A.Y = 0.52548275;	buf.A.Z = -0.70710678;	buf.B.X = 0.35355339;	buf.B.Y = 0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = 0.29389263;	buf.C.Y = 0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.5720614;	buf.A.Y = 0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = 0.47314679;	buf.B.Y = 0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = 0.41562694;	buf.C.Y = 0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.64597419;	buf.A.Y = 0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = 0.5720614;	buf.B.Y = 0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = 0.50903696;	buf.C.Y = 0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.6916548;	buf.A.Y = 0.14701577;	buf.A.Z = -0.70710678;	buf.B.X = 0.64597419;	buf.B.Y = 0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = 0.56775696;	buf.C.Y = 0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.47314679;	buf.A.Y = 0.52548275;	buf.A.Z = -0.70710678;	buf.B.X = 0.29389263;	buf.B.Y = 0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = 0.41562694;	buf.C.Y = 0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.5720614;	buf.A.Y = 0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = 0.41562694;	buf.B.Y = 0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = 0.50903696;	buf.C.Y = 0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.64597419;	buf.A.Y = 0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = 0.50903696;	buf.B.Y = 0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = 0.56775696;	buf.C.Y = 0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.70710678;	buf.A.Y = 0;	buf.A.Z = -0.70710678;	buf.B.X = 0.6916548;	buf.B.Y = 0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = 0.58778525;	buf.C.Y = 0;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.58778525;	buf.A.Y = 0;	buf.A.Z = -0.80901699;	buf.B.X = 0.6916548;	buf.B.Y = 0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = 0.56775696;	buf.C.Y = 0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.41562694;	buf.A.Y = 0.41562694;	buf.A.Z = -0.80901699;	buf.B.X = 0.29389263;	buf.B.Y = 0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = 0.22699525;	buf.C.Y = 0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.50903696;	buf.A.Y = 0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = 0.41562694;	buf.B.Y = 0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = 0.3477769;	buf.C.Y = 0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.56775696;	buf.A.Y = 0.15213002;	buf.A.Z = -0.80901699;	buf.B.X = 0.50903696;	buf.B.Y = 0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = 0.42661152;	buf.C.Y = 0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.41562694;	buf.A.Y = 0.41562694;	buf.A.Z = -0.80901699;	buf.B.X = 0.22699525;	buf.B.Y = 0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = 0.3477769;	buf.C.Y = 0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.50903696;	buf.A.Y = 0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = 0.3477769;	buf.B.Y = 0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = 0.42661152;	buf.C.Y = 0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.58778525;	buf.A.Y = 0;	buf.A.Z = -0.80901699;	buf.B.X = 0.56775696;	buf.B.Y = 0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = 0.4539905;	buf.C.Y = 0;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.4539905;	buf.A.Y = 0;	buf.A.Z = -0.89100652;	buf.B.X = 0.56775696;	buf.B.Y = 0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = 0.42661152;	buf.C.Y = 0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.3477769;	buf.A.Y = 0.29181947;	buf.A.Z = -0.89100652;	buf.B.X = 0.22699525;	buf.B.Y = 0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = 0.1545085;	buf.C.Y = 0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.42661152;	buf.A.Y = 0.1552739;	buf.A.Z = -0.89100652;	buf.B.X = 0.3477769;	buf.B.Y = 0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = 0.26761657;	buf.C.Y = 0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.3477769;	buf.A.Y = 0.29181947;	buf.A.Z = -0.89100652;	buf.B.X = 0.1545085;	buf.B.Y = 0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = 0.26761657;	buf.C.Y = 0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.4539905;	buf.A.Y = 0;	buf.A.Z = -0.89100652;	buf.B.X = 0.42661152;	buf.B.Y = 0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = 0.30901699;	buf.C.Y = 0;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = 0;	buf.A.Z = -0.95105652;	buf.B.X = 0.42661152;	buf.B.Y = 0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = 0.26761657;	buf.C.Y = 0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.26761657;	buf.A.Y = 0.1545085;	buf.A.Z = -0.95105652;	buf.B.X = 0.1545085;	buf.B.Y = 0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = 0.07821723;	buf.C.Y = 0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = 0;	buf.A.Z = -0.95105652;	buf.B.X = 0.26761657;	buf.B.Y = 0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = 0.15643447;	buf.C.Y = 0;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.15643447;	buf.A.Y = 0;	buf.A.Z = -0.98768834;	buf.B.X = 0.26761657;	buf.B.Y = 0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = 0.07821723;	buf.C.Y = 0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.15643447;	buf.A.Y = 0;	buf.A.Z = -0.98768834;	buf.B.X = 0.07821723;	buf.B.Y = 0.13547622;	buf.B.Z = -0.98768834;	buf.C.X = 0;	buf.C.Y = 0;	buf.C.Z = 1;	buf.COUNT = 0;	buf.GROUP = 5;	out.push_back (buf);
	buf.A.X = 0.5;	buf.A.Y = 0.8660254;	buf.A.Z = 0;	buf.B.X = 0.58778525;	buf.B.Y = 0.80901699;	buf.B.Z = 0;	buf.C.X = 0.58903571;	buf.C.Y = 0.90703546;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.58778525;	buf.A.Y = 0.80901699;	buf.A.Z = 0;	buf.B.X = 0.66913061;	buf.B.Y = 0.74314483;	buf.B.Z = 0;	buf.C.X = 0.68061994;	buf.C.Y = 0.84049563;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.66913061;	buf.A.Y = 0.74314483;	buf.A.Z = 0;	buf.B.X = 0.74314483;	buf.B.Y = 0.66913061;	buf.B.Z = 0;	buf.C.X = 0.76474715;	buf.C.Y = 0.76474715;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.74314483;	buf.A.Y = 0.66913061;	buf.A.Z = 0;	buf.B.X = 0.80901699;	buf.B.Y = 0.58778525;	buf.B.Z = 0;	buf.C.X = 0.84049563;	buf.C.Y = 0.68061994;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = 0.58778525;	buf.A.Z = 0;	buf.B.X = 0.8660254;	buf.B.Y = 0.5;	buf.B.Z = 0;	buf.C.X = 0.90703546;	buf.C.Y = 0.58903571;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.8660254;	buf.A.Y = 0.5;	buf.A.Z = 0;	buf.B.X = 0.91354546;	buf.B.Y = 0.40673664;	buf.B.Z = 0;	buf.C.X = 0.96363762;	buf.C.Y = 0.49099789;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.91354546;	buf.A.Y = 0.40673664;	buf.A.Z = 0;	buf.B.X = 0.95105652;	buf.B.Y = 0.30901699;	buf.B.Z = 0;	buf.C.X = 1.00968197;	buf.C.Y = 0.38758059;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = 0.30901699;	buf.A.Z = 0;	buf.B.X = 0.9781476;	buf.B.Y = 0.20791169;	buf.B.Z = 0;	buf.C.X = 1.04466403;	buf.C.Y = 0.27991688;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.9781476;	buf.A.Y = 0.20791169;	buf.A.Z = 0;	buf.B.X = 0.9945219;	buf.B.Y = 0.10452846;	buf.B.Z = 0;	buf.C.X = 1.06820053;	buf.C.Y = 0.16918634;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.9945219;	buf.A.Y = 0.10452846;	buf.A.Z = 0;	buf.B.X = 1;	buf.B.Y = 0;	buf.B.Z = 0;	buf.C.X = 1.08003361;	buf.C.Y = 0.05660216;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 7;	out.push_back (buf);
	buf.A.X = 0.9945219;	buf.A.Y = -0.10452846;	buf.A.Z = 0;	buf.B.X = 1;	buf.B.Y = 0;	buf.B.Z = 0;	buf.C.X = 0.98768834;	buf.C.Y = 0;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.9781476;	buf.A.Y = -0.20791169;	buf.A.Z = 0;	buf.B.X = 0.9945219;	buf.B.Y = -0.10452846;	buf.B.Z = 0;	buf.C.X = 0.98100995;	buf.C.Y = -0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = -0.30901699;	buf.A.Z = 0;	buf.B.X = 0.9781476;	buf.B.Y = -0.20791169;	buf.B.Z = 0;	buf.C.X = 0.96106507;	buf.C.Y = -0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.91354546;	buf.A.Y = -0.40673664;	buf.A.Z = 0;	buf.B.X = 0.95105652;	buf.B.Y = -0.30901699;	buf.B.Z = 0;	buf.C.X = 0.92812345;	buf.C.Y = -0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.8660254;	buf.A.Y = -0.5;	buf.A.Z = 0;	buf.B.X = 0.91354546;	buf.B.Y = -0.40673664;	buf.B.Z = 0;	buf.C.X = 0.88263054;	buf.C.Y = -0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = -0.58778525;	buf.A.Z = 0;	buf.B.X = 0.8660254;	buf.B.Y = -0.5;	buf.B.Z = 0;	buf.C.X = 0.82520157;	buf.C.Y = -0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.74314483;	buf.A.Y = -0.66913061;	buf.A.Z = 0;	buf.B.X = 0.80901699;	buf.B.Y = -0.58778525;	buf.B.Z = 0;	buf.C.X = 0.75661316;	buf.C.Y = -0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.66913061;	buf.A.Y = -0.74314483;	buf.A.Z = 0;	buf.B.X = 0.74314483;	buf.B.Y = -0.66913061;	buf.B.Z = 0;	buf.C.X = 0.67779286;	buf.C.Y = -0.71841846;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.58778525;	buf.A.Y = -0.80901699;	buf.A.Z = 0;	buf.B.X = 0.66913061;	buf.B.Y = -0.74314483;	buf.B.Z = 0;	buf.C.X = 0.58980658;	buf.C.Y = -0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.9945219;	buf.A.Y = -0.10452846;	buf.A.Z = 0;	buf.B.X = 0.98768834;	buf.B.Y = 0;	buf.B.Z = -0.15643447;	buf.C.X = 0.98100995;	buf.C.Y = -0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.9781476;	buf.A.Y = -0.20791169;	buf.A.Z = 0;	buf.B.X = 0.98100995;	buf.B.Y = -0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = 0.96106507;	buf.C.Y = -0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = -0.30901699;	buf.A.Z = 0;	buf.B.X = 0.96106507;	buf.B.Y = -0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = 0.92812345;	buf.C.Y = -0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.91354546;	buf.A.Y = -0.40673664;	buf.A.Z = 0;	buf.B.X = 0.92812345;	buf.B.Y = -0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = 0.88263054;	buf.C.Y = -0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.8660254;	buf.A.Y = -0.5;	buf.A.Z = 0;	buf.B.X = 0.88263054;	buf.B.Y = -0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = 0.82520157;	buf.C.Y = -0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = -0.58778525;	buf.A.Z = 0;	buf.B.X = 0.82520157;	buf.B.Y = -0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = 0.75661316;	buf.C.Y = -0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.74314483;	buf.A.Y = -0.66913061;	buf.A.Z = 0;	buf.B.X = 0.75661316;	buf.B.Y = -0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = 0.67779286;	buf.C.Y = -0.71841846;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.66913061;	buf.A.Y = -0.74314483;	buf.A.Z = 0;	buf.B.X = 0.67779286;	buf.B.Y = -0.71841846;	buf.B.Z = -0.15643447;	buf.C.X = 0.58980658;	buf.C.Y = -0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.5;	buf.A.Y = -0.8660254;	buf.A.Z = 0;	buf.B.X = 0.58778525;	buf.B.Y = -0.80901699;	buf.B.Z = 0;	buf.C.X = 0.49384417;	buf.C.Y = -0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.49384417;	buf.A.Y = -0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = 0.58778525;	buf.B.Y = -0.80901699;	buf.B.Z = 0;	buf.C.X = 0.58980658;	buf.C.Y = -0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.98100995;	buf.A.Y = -0.11466362;	buf.A.Z = -0.15643447;	buf.B.X = 0.98768834;	buf.B.Y = 0;	buf.B.Z = -0.15643447;	buf.C.X = 0.95105652;	buf.C.Y = 0;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.96106507;	buf.A.Y = -0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = 0.98100995;	buf.B.Y = -0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = 0.9429201;	buf.C.Y = -0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.92812345;	buf.A.Y = -0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = 0.96106507;	buf.B.Y = -0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = 0.91865005;	buf.C.Y = -0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.88263054;	buf.A.Y = -0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = 0.92812345;	buf.B.Y = -0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = 0.87866165;	buf.C.Y = -0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.82520157;	buf.A.Y = -0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = 0.88263054;	buf.B.Y = -0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = 0.8236391;	buf.C.Y = -0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.75661316;	buf.A.Y = -0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = 0.82520157;	buf.B.Y = -0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = 0.75452386;	buf.C.Y = -0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.67779286;	buf.A.Y = -0.71841846;	buf.A.Z = -0.15643447;	buf.B.X = 0.75661316;	buf.B.Y = -0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = 0.67249851;	buf.C.Y = -0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.58980658;	buf.A.Y = -0.79224773;	buf.A.Z = -0.15643447;	buf.B.X = 0.67779286;	buf.B.Y = -0.71841846;	buf.B.Z = -0.15643447;	buf.C.X = 0.57896652;	buf.C.Y = -0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.98100995;	buf.A.Y = -0.11466362;	buf.A.Z = -0.15643447;	buf.B.X = 0.95105652;	buf.B.Y = 0;	buf.B.Z = -0.30901699;	buf.C.X = 0.9429201;	buf.C.Y = -0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.96106507;	buf.A.Y = -0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = 0.9429201;	buf.B.Y = -0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = 0.91865005;	buf.C.Y = -0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.92812345;	buf.A.Y = -0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = 0.91865005;	buf.B.Y = -0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = 0.87866165;	buf.C.Y = -0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.88263054;	buf.A.Y = -0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = 0.87866165;	buf.B.Y = -0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = 0.8236391;	buf.C.Y = -0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.82520157;	buf.A.Y = -0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = 0.8236391;	buf.B.Y = -0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = 0.75452386;	buf.C.Y = -0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.75661316;	buf.A.Y = -0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = 0.75452386;	buf.B.Y = -0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = 0.67249851;	buf.C.Y = -0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.67779286;	buf.A.Y = -0.71841846;	buf.A.Z = -0.15643447;	buf.B.X = 0.67249851;	buf.B.Y = -0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = 0.57896652;	buf.C.Y = -0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.49384417;	buf.A.Y = -0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = 0.58980658;	buf.B.Y = -0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = 0.47552826;	buf.C.Y = -0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.47552826;	buf.A.Y = -0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = 0.58980658;	buf.B.Y = -0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = 0.57896652;	buf.C.Y = -0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.9429201;	buf.A.Y = -0.12413779;	buf.A.Z = -0.30901699;	buf.B.X = 0.95105652;	buf.B.Y = 0;	buf.B.Z = -0.30901699;	buf.C.X = 0.89100652;	buf.C.Y = 0;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.91865005;	buf.A.Y = -0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = 0.9429201;	buf.B.Y = -0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = 0.88105472;	buf.C.Y = -0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.87866165;	buf.A.Y = -0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = 0.91865005;	buf.B.Y = -0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = 0.8514216;	buf.C.Y = -0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.8236391;	buf.A.Y = -0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = 0.87866165;	buf.B.Y = -0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = 0.80276914;	buf.C.Y = -0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.75452386;	buf.A.Y = -0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = 0.8236391;	buf.B.Y = -0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = 0.73618414;	buf.C.Y = -0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.67249851;	buf.A.Y = -0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = 0.75452386;	buf.B.Y = -0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = 0.653154;	buf.C.Y = -0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.57896652;	buf.A.Y = -0.75452386;	buf.A.Z = -0.30901699;	buf.B.X = 0.67249851;	buf.B.Y = -0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = 0.55553348;	buf.C.Y = -0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.9429201;	buf.A.Y = -0.12413779;	buf.A.Z = -0.30901699;	buf.B.X = 0.89100652;	buf.B.Y = 0;	buf.B.Z = -0.4539905;	buf.C.X = 0.88105472;	buf.C.Y = -0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.91865005;	buf.A.Y = -0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = 0.88105472;	buf.B.Y = -0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = 0.8514216;	buf.C.Y = -0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.87866165;	buf.A.Y = -0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = 0.8514216;	buf.B.Y = -0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = 0.80276914;	buf.C.Y = -0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.8236391;	buf.A.Y = -0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = 0.80276914;	buf.B.Y = -0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = 0.73618414;	buf.C.Y = -0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.75452386;	buf.A.Y = -0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = 0.73618414;	buf.B.Y = -0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = 0.653154;	buf.C.Y = -0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.67249851;	buf.A.Y = -0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = 0.653154;	buf.B.Y = -0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = 0.55553348;	buf.C.Y = -0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.47552826;	buf.A.Y = -0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = 0.57896652;	buf.B.Y = -0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = 0.44550326;	buf.C.Y = -0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.44550326;	buf.A.Y = -0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = 0.57896652;	buf.B.Y = -0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = 0.55553348;	buf.C.Y = -0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.88105472;	buf.A.Y = -0.13279763;	buf.A.Z = -0.4539905;	buf.B.X = 0.89100652;	buf.B.Y = 0;	buf.B.Z = -0.4539905;	buf.C.X = 0.80901699;	buf.C.Y = 0;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.8514216;	buf.A.Y = -0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = 0.88105472;	buf.B.Y = -0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = 0.79672621;	buf.C.Y = -0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.80276914;	buf.A.Y = -0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = 0.8514216;	buf.B.Y = -0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = 0.7602273;	buf.C.Y = -0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.73618414;	buf.A.Y = -0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = 0.80276914;	buf.B.Y = -0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = 0.70062927;	buf.C.Y = -0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.653154;	buf.A.Y = -0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = 0.73618414;	buf.B.Y = -0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = 0.61974297;	buf.C.Y = -0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.55553348;	buf.A.Y = -0.69661695;	buf.A.Z = -0.4539905;	buf.B.X = 0.653154;	buf.B.Y = -0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = 0.5200261;	buf.C.Y = -0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.88105472;	buf.A.Y = -0.13279763;	buf.A.Z = -0.4539905;	buf.B.X = 0.80901699;	buf.B.Y = 0;	buf.B.Z = -0.58778525;	buf.C.X = 0.79672621;	buf.C.Y = -0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.8514216;	buf.A.Y = -0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = 0.79672621;	buf.B.Y = -0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = 0.7602273;	buf.C.Y = -0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.80276914;	buf.A.Y = -0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = 0.7602273;	buf.B.Y = -0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = 0.70062927;	buf.C.Y = -0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.73618414;	buf.A.Y = -0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = 0.70062927;	buf.B.Y = -0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = 0.61974297;	buf.C.Y = -0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.653154;	buf.A.Y = -0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = 0.61974297;	buf.B.Y = -0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = 0.5200261;	buf.C.Y = -0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.44550326;	buf.A.Y = -0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = 0.55553348;	buf.B.Y = -0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = 0.4045085;	buf.C.Y = -0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.4045085;	buf.A.Y = -0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = 0.55553348;	buf.B.Y = -0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = 0.5200261;	buf.C.Y = -0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.79672621;	buf.A.Y = -0.14048433;	buf.A.Z = -0.58778525;	buf.B.X = 0.80901699;	buf.B.Y = 0;	buf.B.Z = -0.58778525;	buf.C.X = 0.70710678;	buf.C.Y = 0;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.7602273;	buf.A.Y = -0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = 0.79672621;	buf.B.Y = -0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = 0.6916548;	buf.C.Y = -0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.70062927;	buf.A.Y = -0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = 0.7602273;	buf.B.Y = -0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = 0.64597419;	buf.C.Y = -0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.61974297;	buf.A.Y = -0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = 0.70062927;	buf.B.Y = -0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = 0.5720614;	buf.C.Y = -0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.5200261;	buf.A.Y = -0.61974297;	buf.A.Z = -0.58778525;	buf.B.X = 0.61974297;	buf.B.Y = -0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = 0.47314679;	buf.C.Y = -0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.79672621;	buf.A.Y = -0.14048433;	buf.A.Z = -0.58778525;	buf.B.X = 0.70710678;	buf.B.Y = 0;	buf.B.Z = -0.70710678;	buf.C.X = 0.6916548;	buf.C.Y = -0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.7602273;	buf.A.Y = -0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = 0.6916548;	buf.B.Y = -0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = 0.64597419;	buf.C.Y = -0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.70062927;	buf.A.Y = -0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = 0.64597419;	buf.B.Y = -0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = 0.5720614;	buf.C.Y = -0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.61974297;	buf.A.Y = -0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = 0.5720614;	buf.B.Y = -0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = 0.47314679;	buf.C.Y = -0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.4045085;	buf.A.Y = -0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = 0.5200261;	buf.B.Y = -0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = 0.35355339;	buf.C.Y = -0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.35355339;	buf.A.Y = -0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = 0.5200261;	buf.B.Y = -0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = 0.47314679;	buf.C.Y = -0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.6916548;	buf.A.Y = -0.14701577;	buf.A.Z = -0.70710678;	buf.B.X = 0.70710678;	buf.B.Y = 0;	buf.B.Z = -0.70710678;	buf.C.X = 0.58778525;	buf.C.Y = 0;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.64597419;	buf.A.Y = -0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = 0.6916548;	buf.B.Y = -0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = 0.56775696;	buf.C.Y = -0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.5720614;	buf.A.Y = -0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = 0.64597419;	buf.B.Y = -0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = 0.50903696;	buf.C.Y = -0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.47314679;	buf.A.Y = -0.52548275;	buf.A.Z = -0.70710678;	buf.B.X = 0.5720614;	buf.B.Y = -0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = 0.41562694;	buf.C.Y = -0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.6916548;	buf.A.Y = -0.14701577;	buf.A.Z = -0.70710678;	buf.B.X = 0.58778525;	buf.B.Y = 0;	buf.B.Z = -0.80901699;	buf.C.X = 0.56775696;	buf.C.Y = -0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.64597419;	buf.A.Y = -0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = 0.56775696;	buf.B.Y = -0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = 0.50903696;	buf.C.Y = -0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.5720614;	buf.A.Y = -0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = 0.50903696;	buf.B.Y = -0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = 0.41562694;	buf.C.Y = -0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.35355339;	buf.A.Y = -0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = 0.47314679;	buf.B.Y = -0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = 0.29389263;	buf.C.Y = -0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.29389263;	buf.A.Y = -0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = 0.47314679;	buf.B.Y = -0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = 0.41562694;	buf.C.Y = -0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.56775696;	buf.A.Y = -0.15213002;	buf.A.Z = -0.80901699;	buf.B.X = 0.58778525;	buf.B.Y = 0;	buf.B.Z = -0.80901699;	buf.C.X = 0.4539905;	buf.C.Y = 0;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.50903696;	buf.A.Y = -0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = 0.56775696;	buf.B.Y = -0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = 0.42661152;	buf.C.Y = -0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.41562694;	buf.A.Y = -0.41562694;	buf.A.Z = -0.80901699;	buf.B.X = 0.50903696;	buf.B.Y = -0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = 0.3477769;	buf.C.Y = -0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.56775696;	buf.A.Y = -0.15213002;	buf.A.Z = -0.80901699;	buf.B.X = 0.4539905;	buf.B.Y = 0;	buf.B.Z = -0.89100652;	buf.C.X = 0.42661152;	buf.C.Y = -0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.50903696;	buf.A.Y = -0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = 0.42661152;	buf.B.Y = -0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = 0.3477769;	buf.C.Y = -0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.29389263;	buf.A.Y = -0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = 0.41562694;	buf.B.Y = -0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = 0.22699525;	buf.C.Y = -0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.22699525;	buf.A.Y = -0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = 0.41562694;	buf.B.Y = -0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = 0.3477769;	buf.C.Y = -0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.42661152;	buf.A.Y = -0.1552739;	buf.A.Z = -0.89100652;	buf.B.X = 0.4539905;	buf.B.Y = 0;	buf.B.Z = -0.89100652;	buf.C.X = 0.30901699;	buf.C.Y = 0;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.3477769;	buf.A.Y = -0.29181947;	buf.A.Z = -0.89100652;	buf.B.X = 0.42661152;	buf.B.Y = -0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = 0.26761657;	buf.C.Y = -0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.42661152;	buf.A.Y = -0.1552739;	buf.A.Z = -0.89100652;	buf.B.X = 0.30901699;	buf.B.Y = 0;	buf.B.Z = -0.95105652;	buf.C.X = 0.26761657;	buf.C.Y = -0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.22699525;	buf.A.Y = -0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = 0.3477769;	buf.B.Y = -0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = 0.1545085;	buf.C.Y = -0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.1545085;	buf.A.Y = -0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = 0.3477769;	buf.B.Y = -0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = 0.26761657;	buf.C.Y = -0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.26761657;	buf.A.Y = -0.1545085;	buf.A.Z = -0.95105652;	buf.B.X = 0.30901699;	buf.B.Y = 0;	buf.B.Z = -0.95105652;	buf.C.X = 0.15643447;	buf.C.Y = 0;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.1545085;	buf.A.Y = -0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = 0.26761657;	buf.B.Y = -0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = 0.07821723;	buf.C.Y = -0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.07821723;	buf.A.Y = -0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = 0.26761657;	buf.B.Y = -0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = 0.15643447;	buf.C.Y = 0;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.07821723;	buf.A.Y = -0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = 0.15643447;	buf.B.Y = 0;	buf.B.Z = -0.98768834;	buf.C.X = 0;	buf.C.Y = 0;	buf.C.Z = 1;	buf.COUNT = 0;	buf.GROUP = 5;	out.push_back (buf);
	buf.A.X = 1;	buf.A.Y = 0;	buf.A.Z = 0;	buf.B.X = 0.9945219;	buf.B.Y = -0.10452846;	buf.B.Z = 0;	buf.C.X = 1.08003361;	buf.C.Y = -0.05660216;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.9945219;	buf.A.Y = -0.10452846;	buf.A.Z = 0;	buf.B.X = 0.9781476;	buf.B.Y = -0.20791169;	buf.B.Z = 0;	buf.C.X = 1.06820053;	buf.C.Y = -0.16918634;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.9781476;	buf.A.Y = -0.20791169;	buf.A.Z = 0;	buf.B.X = 0.95105652;	buf.B.Y = -0.30901699;	buf.B.Z = 0;	buf.C.X = 1.04466403;	buf.C.Y = -0.27991688;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.95105652;	buf.A.Y = -0.30901699;	buf.A.Z = 0;	buf.B.X = 0.91354546;	buf.B.Y = -0.40673664;	buf.B.Z = 0;	buf.C.X = 1.00968197;	buf.C.Y = -0.38758059;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.91354546;	buf.A.Y = -0.40673664;	buf.A.Z = 0;	buf.B.X = 0.8660254;	buf.B.Y = -0.5;	buf.B.Z = 0;	buf.C.X = 0.96363762;	buf.C.Y = -0.49099789;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.8660254;	buf.A.Y = -0.5;	buf.A.Z = 0;	buf.B.X = 0.80901699;	buf.B.Y = -0.58778525;	buf.B.Z = 0;	buf.C.X = 0.90703546;	buf.C.Y = -0.58903571;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.80901699;	buf.A.Y = -0.58778525;	buf.A.Z = 0;	buf.B.X = 0.74314483;	buf.B.Y = -0.66913061;	buf.B.Z = 0;	buf.C.X = 0.84049563;	buf.C.Y = -0.68061994;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.74314483;	buf.A.Y = -0.66913061;	buf.A.Z = 0;	buf.B.X = 0.66913061;	buf.B.Y = -0.74314483;	buf.B.Z = 0;	buf.C.X = 0.76474715;	buf.C.Y = -0.76474715;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.66913061;	buf.A.Y = -0.74314483;	buf.A.Z = 0;	buf.B.X = 0.58778525;	buf.B.Y = -0.80901699;	buf.B.Z = 0;	buf.C.X = 0.68061994;	buf.C.Y = -0.84049563;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.58778525;	buf.A.Y = -0.80901699;	buf.A.Z = 0;	buf.B.X = 0.5;	buf.B.Y = -0.8660254;	buf.B.Z = 0;	buf.C.X = 0.58903571;	buf.C.Y = -0.90703546;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 7;	out.push_back (buf);
	buf.A.X = 0.40673664;	buf.A.Y = -0.91354546;	buf.A.Z = 0;	buf.B.X = 0.5;	buf.B.Y = -0.8660254;	buf.B.Z = 0;	buf.C.X = 0.49384417;	buf.C.Y = -0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = -0.95105652;	buf.A.Z = 0;	buf.B.X = 0.40673664;	buf.B.Y = -0.91354546;	buf.B.Z = 0;	buf.C.X = 0.39120337;	buf.C.Y = -0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.20791169;	buf.A.Y = -0.9781476;	buf.A.Z = 0;	buf.B.X = 0.30901699;	buf.B.Y = -0.95105652;	buf.B.Z = 0;	buf.C.X = 0.28327221;	buf.C.Y = -0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.10452846;	buf.A.Y = -0.9945219;	buf.A.Z = 0;	buf.B.X = 0.20791169;	buf.B.Y = -0.9781476;	buf.B.Z = 0;	buf.C.X = 0.17151028;	buf.C.Y = -0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -1;	buf.A.Z = 0;	buf.B.X = 0.10452846;	buf.B.Y = -0.9945219;	buf.B.Z = 0;	buf.C.X = 0.05742897;	buf.C.Y = -0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.10452846;	buf.A.Y = -0.9945219;	buf.A.Z = 0;	buf.B.X = 0;	buf.B.Y = -1;	buf.B.Z = 0;	buf.C.X = -0.05742897;	buf.C.Y = -0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.20791169;	buf.A.Y = -0.9781476;	buf.A.Z = 0;	buf.B.X = -0.10452846;	buf.B.Y = -0.9945219;	buf.B.Z = 0;	buf.C.X = -0.17151028;	buf.C.Y = -0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = -0.95105652;	buf.A.Z = 0;	buf.B.X = -0.20791169;	buf.B.Y = -0.9781476;	buf.B.Z = 0;	buf.C.X = -0.28327221;	buf.C.Y = -0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.40673664;	buf.A.Y = -0.91354546;	buf.A.Z = 0;	buf.B.X = -0.30901699;	buf.B.Y = -0.95105652;	buf.B.Z = 0;	buf.C.X = -0.39120337;	buf.C.Y = -0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.40673664;	buf.A.Y = -0.91354546;	buf.A.Z = 0;	buf.B.X = 0.49384417;	buf.B.Y = -0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = 0.39120337;	buf.C.Y = -0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = -0.95105652;	buf.A.Z = 0;	buf.B.X = 0.39120337;	buf.B.Y = -0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = 0.28327221;	buf.C.Y = -0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.20791169;	buf.A.Y = -0.9781476;	buf.A.Z = 0;	buf.B.X = 0.28327221;	buf.B.Y = -0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = 0.17151028;	buf.C.Y = -0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.10452846;	buf.A.Y = -0.9945219;	buf.A.Z = 0;	buf.B.X = 0.17151028;	buf.B.Y = -0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = 0.05742897;	buf.C.Y = -0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -1;	buf.A.Z = 0;	buf.B.X = 0.05742897;	buf.B.Y = -0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = -0.05742897;	buf.C.Y = -0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.10452846;	buf.A.Y = -0.9945219;	buf.A.Z = 0;	buf.B.X = -0.05742897;	buf.B.Y = -0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = -0.17151028;	buf.C.Y = -0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.20791169;	buf.A.Y = -0.9781476;	buf.A.Z = 0;	buf.B.X = -0.17151028;	buf.B.Y = -0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = -0.28327221;	buf.C.Y = -0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = -0.95105652;	buf.A.Z = 0;	buf.B.X = -0.28327221;	buf.B.Y = -0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = -0.39120337;	buf.C.Y = -0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.5;	buf.A.Y = -0.8660254;	buf.A.Z = 0;	buf.B.X = -0.40673664;	buf.B.Y = -0.91354546;	buf.B.Z = 0;	buf.C.X = -0.49384417;	buf.C.Y = -0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.49384417;	buf.A.Y = -0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = -0.40673664;	buf.B.Y = -0.91354546;	buf.B.Z = 0;	buf.C.X = -0.39120337;	buf.C.Y = -0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.39120337;	buf.A.Y = -0.90691134;	buf.A.Z = -0.15643447;	buf.B.X = 0.49384417;	buf.B.Y = -0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = 0.47552826;	buf.C.Y = -0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.28327221;	buf.A.Y = -0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = 0.39120337;	buf.B.Y = -0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = 0.36395357;	buf.C.Y = -0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.17151028;	buf.A.Y = -0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = 0.28327221;	buf.B.Y = -0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = 0.24615154;	buf.C.Y = -0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.05742897;	buf.A.Y = -0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = 0.17151028;	buf.B.Y = -0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = 0.12413779;	buf.C.Y = -0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.05742897;	buf.A.Y = -0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = 0.05742897;	buf.B.Y = -0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = 0;	buf.C.Y = -0.95105652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.17151028;	buf.A.Y = -0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = -0.05742897;	buf.B.Y = -0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = -0.12413779;	buf.C.Y = -0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.28327221;	buf.A.Y = -0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = -0.17151028;	buf.B.Y = -0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = -0.24615154;	buf.C.Y = -0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.39120337;	buf.A.Y = -0.90691134;	buf.A.Z = -0.15643447;	buf.B.X = -0.28327221;	buf.B.Y = -0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = -0.36395357;	buf.C.Y = -0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.39120337;	buf.A.Y = -0.90691134;	buf.A.Z = -0.15643447;	buf.B.X = 0.47552826;	buf.B.Y = -0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = 0.36395357;	buf.C.Y = -0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.28327221;	buf.A.Y = -0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = 0.36395357;	buf.B.Y = -0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = 0.24615154;	buf.C.Y = -0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.17151028;	buf.A.Y = -0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = 0.24615154;	buf.B.Y = -0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = 0.12413779;	buf.C.Y = -0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.05742897;	buf.A.Y = -0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = 0.12413779;	buf.B.Y = -0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = 0;	buf.C.Y = -0.95105652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.05742897;	buf.A.Y = -0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = 0;	buf.B.Y = -0.95105652;	buf.B.Z = -0.30901699;	buf.C.X = -0.12413779;	buf.C.Y = -0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.17151028;	buf.A.Y = -0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = -0.12413779;	buf.B.Y = -0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = -0.24615154;	buf.C.Y = -0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.28327221;	buf.A.Y = -0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = -0.24615154;	buf.B.Y = -0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = -0.36395357;	buf.C.Y = -0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.49384417;	buf.A.Y = -0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = -0.39120337;	buf.B.Y = -0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = -0.47552826;	buf.C.Y = -0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.47552826;	buf.A.Y = -0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = -0.39120337;	buf.B.Y = -0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = -0.36395357;	buf.C.Y = -0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.36395357;	buf.A.Y = -0.87866165;	buf.A.Z = -0.30901699;	buf.B.X = 0.47552826;	buf.B.Y = -0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = 0.44550326;	buf.C.Y = -0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.24615154;	buf.A.Y = -0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = 0.36395357;	buf.B.Y = -0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = 0.32552124;	buf.C.Y = -0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.12413779;	buf.A.Y = -0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = 0.24615154;	buf.B.Y = -0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = 0.1982676;	buf.C.Y = -0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -0.95105652;	buf.A.Z = -0.30901699;	buf.B.X = 0.12413779;	buf.B.Y = -0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = 0.066585;	buf.C.Y = -0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.12413779;	buf.A.Y = -0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = 0;	buf.B.Y = -0.95105652;	buf.B.Z = -0.30901699;	buf.C.X = -0.066585;	buf.C.Y = -0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.24615154;	buf.A.Y = -0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = -0.12413779;	buf.B.Y = -0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = -0.1982676;	buf.C.Y = -0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.36395357;	buf.A.Y = -0.87866165;	buf.A.Z = -0.30901699;	buf.B.X = -0.24615154;	buf.B.Y = -0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = -0.32552124;	buf.C.Y = -0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.36395357;	buf.A.Y = -0.87866165;	buf.A.Z = -0.30901699;	buf.B.X = 0.44550326;	buf.B.Y = -0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = 0.32552124;	buf.C.Y = -0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.24615154;	buf.A.Y = -0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = 0.32552124;	buf.B.Y = -0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = 0.1982676;	buf.C.Y = -0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.12413779;	buf.A.Y = -0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = 0.1982676;	buf.B.Y = -0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = 0.066585;	buf.C.Y = -0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -0.95105652;	buf.A.Z = -0.30901699;	buf.B.X = 0.066585;	buf.B.Y = -0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = -0.066585;	buf.C.Y = -0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.12413779;	buf.A.Y = -0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = -0.066585;	buf.B.Y = -0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = -0.1982676;	buf.C.Y = -0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.24615154;	buf.A.Y = -0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = -0.1982676;	buf.B.Y = -0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = -0.32552124;	buf.C.Y = -0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.47552826;	buf.A.Y = -0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = -0.36395357;	buf.B.Y = -0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = -0.44550326;	buf.C.Y = -0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 1;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.44550326;	buf.A.Y = -0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = -0.36395357;	buf.B.Y = -0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = -0.32552124;	buf.C.Y = -0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.32552124;	buf.A.Y = -0.82941458;	buf.A.Z = -0.4539905;	buf.B.X = 0.44550326;	buf.B.Y = -0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = 0.4045085;	buf.C.Y = -0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.1982676;	buf.A.Y = -0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = 0.32552124;	buf.B.Y = -0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = 0.27670011;	buf.C.Y = -0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.066585;	buf.A.Y = -0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = 0.1982676;	buf.B.Y = -0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = 0.14048433;	buf.C.Y = -0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.066585;	buf.A.Y = -0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = 0.066585;	buf.B.Y = -0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = 0;	buf.C.Y = -0.80901699;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.1982676;	buf.A.Y = -0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = -0.066585;	buf.B.Y = -0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = -0.14048433;	buf.C.Y = -0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.32552124;	buf.A.Y = -0.82941458;	buf.A.Z = -0.4539905;	buf.B.X = -0.1982676;	buf.B.Y = -0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = -0.27670011;	buf.C.Y = -0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.32552124;	buf.A.Y = -0.82941458;	buf.A.Z = -0.4539905;	buf.B.X = 0.4045085;	buf.B.Y = -0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = 0.27670011;	buf.C.Y = -0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.1982676;	buf.A.Y = -0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = 0.27670011;	buf.B.Y = -0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = 0.14048433;	buf.C.Y = -0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.066585;	buf.A.Y = -0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = 0.14048433;	buf.B.Y = -0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = 0;	buf.C.Y = -0.80901699;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.066585;	buf.A.Y = -0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = 0;	buf.B.Y = -0.80901699;	buf.B.Z = -0.58778525;	buf.C.X = -0.14048433;	buf.C.Y = -0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.1982676;	buf.A.Y = -0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = -0.14048433;	buf.B.Y = -0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = -0.27670011;	buf.C.Y = -0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.44550326;	buf.A.Y = -0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = -0.32552124;	buf.B.Y = -0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = -0.4045085;	buf.C.Y = -0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.4045085;	buf.A.Y = -0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = -0.32552124;	buf.B.Y = -0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = -0.27670011;	buf.C.Y = -0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.27670011;	buf.A.Y = -0.7602273;	buf.A.Z = -0.58778525;	buf.B.X = 0.4045085;	buf.B.Y = -0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = 0.35355339;	buf.C.Y = -0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.14048433;	buf.A.Y = -0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = 0.27670011;	buf.B.Y = -0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = 0.21850801;	buf.C.Y = -0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -0.80901699;	buf.A.Z = -0.58778525;	buf.B.X = 0.14048433;	buf.B.Y = -0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = 0.07391279;	buf.C.Y = -0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.14048433;	buf.A.Y = -0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = 0;	buf.B.Y = -0.80901699;	buf.B.Z = -0.58778525;	buf.C.X = -0.07391279;	buf.C.Y = -0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.27670011;	buf.A.Y = -0.7602273;	buf.A.Z = -0.58778525;	buf.B.X = -0.14048433;	buf.B.Y = -0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = -0.21850801;	buf.C.Y = -0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.27670011;	buf.A.Y = -0.7602273;	buf.A.Z = -0.58778525;	buf.B.X = 0.35355339;	buf.B.Y = -0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = 0.21850801;	buf.C.Y = -0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.14048433;	buf.A.Y = -0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = 0.21850801;	buf.B.Y = -0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = 0.07391279;	buf.C.Y = -0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -0.80901699;	buf.A.Z = -0.58778525;	buf.B.X = 0.07391279;	buf.B.Y = -0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = -0.07391279;	buf.C.Y = -0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.14048433;	buf.A.Y = -0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = -0.07391279;	buf.B.Y = -0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = -0.21850801;	buf.C.Y = -0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.4045085;	buf.A.Y = -0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = -0.27670011;	buf.B.Y = -0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = -0.35355339;	buf.C.Y = -0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.35355339;	buf.A.Y = -0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = -0.27670011;	buf.B.Y = -0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = -0.21850801;	buf.C.Y = -0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.21850801;	buf.A.Y = -0.67249851;	buf.A.Z = -0.70710678;	buf.B.X = 0.35355339;	buf.B.Y = -0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = 0.29389263;	buf.C.Y = -0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.07391279;	buf.A.Y = -0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = 0.21850801;	buf.B.Y = -0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = 0.15213002;	buf.C.Y = -0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.07391279;	buf.A.Y = -0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = 0.07391279;	buf.B.Y = -0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = 0;	buf.C.Y = -0.58778525;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.21850801;	buf.A.Y = -0.67249851;	buf.A.Z = -0.70710678;	buf.B.X = -0.07391279;	buf.B.Y = -0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = -0.15213002;	buf.C.Y = -0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.21850801;	buf.A.Y = -0.67249851;	buf.A.Z = -0.70710678;	buf.B.X = 0.29389263;	buf.B.Y = -0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = 0.15213002;	buf.C.Y = -0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.07391279;	buf.A.Y = -0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = 0.15213002;	buf.B.Y = -0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = 0;	buf.C.Y = -0.58778525;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.07391279;	buf.A.Y = -0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = 0;	buf.B.Y = -0.58778525;	buf.B.Z = -0.80901699;	buf.C.X = -0.15213002;	buf.C.Y = -0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.35355339;	buf.A.Y = -0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = -0.21850801;	buf.B.Y = -0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = -0.29389263;	buf.C.Y = -0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.29389263;	buf.A.Y = -0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = -0.21850801;	buf.B.Y = -0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = -0.15213002;	buf.C.Y = -0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.15213002;	buf.A.Y = -0.56775696;	buf.A.Z = -0.80901699;	buf.B.X = 0.29389263;	buf.B.Y = -0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = 0.22699525;	buf.C.Y = -0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -0.58778525;	buf.A.Z = -0.80901699;	buf.B.X = 0.15213002;	buf.B.Y = -0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = 0.07883462;	buf.C.Y = -0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.15213002;	buf.A.Y = -0.56775696;	buf.A.Z = -0.80901699;	buf.B.X = 0;	buf.B.Y = -0.58778525;	buf.B.Z = -0.80901699;	buf.C.X = -0.07883462;	buf.C.Y = -0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.15213002;	buf.A.Y = -0.56775696;	buf.A.Z = -0.80901699;	buf.B.X = 0.22699525;	buf.B.Y = -0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = 0.07883462;	buf.C.Y = -0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -0.58778525;	buf.A.Z = -0.80901699;	buf.B.X = 0.07883462;	buf.B.Y = -0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = -0.07883462;	buf.C.Y = -0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.29389263;	buf.A.Y = -0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = -0.15213002;	buf.B.Y = -0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = -0.22699525;	buf.C.Y = -0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.22699525;	buf.A.Y = -0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = -0.15213002;	buf.B.Y = -0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = -0.07883462;	buf.C.Y = -0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.07883462;	buf.A.Y = -0.44709336;	buf.A.Z = -0.89100652;	buf.B.X = 0.22699525;	buf.B.Y = -0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = 0.1545085;	buf.C.Y = -0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.07883462;	buf.A.Y = -0.44709336;	buf.A.Z = -0.89100652;	buf.B.X = 0.07883462;	buf.B.Y = -0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = 0;	buf.C.Y = -0.30901699;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.07883462;	buf.A.Y = -0.44709336;	buf.A.Z = -0.89100652;	buf.B.X = 0.1545085;	buf.B.Y = -0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = 0;	buf.C.Y = -0.30901699;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.22699525;	buf.A.Y = -0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = -0.07883462;	buf.B.Y = -0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = -0.1545085;	buf.C.Y = -0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.1545085;	buf.A.Y = -0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = -0.07883462;	buf.B.Y = -0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = 0;	buf.C.Y = -0.30901699;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -0.30901699;	buf.A.Z = -0.95105652;	buf.B.X = 0.1545085;	buf.B.Y = -0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = 0.07821723;	buf.C.Y = -0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.1545085;	buf.A.Y = -0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = 0;	buf.B.Y = -0.30901699;	buf.B.Z = -0.95105652;	buf.C.X = -0.07821723;	buf.C.Y = -0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.07821723;	buf.A.Y = -0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = 0;	buf.B.Y = -0.30901699;	buf.B.Z = -0.95105652;	buf.C.X = 0.07821723;	buf.C.Y = -0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.07821723;	buf.A.Y = -0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = 0.07821723;	buf.B.Y = -0.13547622;	buf.B.Z = -0.98768834;	buf.C.X = 0;	buf.C.Y = 0;	buf.C.Z = 1;	buf.COUNT = 0;	buf.GROUP = 5;	out.push_back (buf);
	buf.A.X = 0.5;	buf.A.Y = -0.8660254;	buf.A.Z = 0;	buf.B.X = 0.40673664;	buf.B.Y = -0.91354546;	buf.B.Z = 0;	buf.C.X = 0.49099789;	buf.C.Y = -0.96363762;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.40673664;	buf.A.Y = -0.91354546;	buf.A.Z = 0;	buf.B.X = 0.30901699;	buf.B.Y = -0.95105652;	buf.B.Z = 0;	buf.C.X = 0.38758059;	buf.C.Y = -1.00968197;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = -0.95105652;	buf.A.Z = 0;	buf.B.X = 0.20791169;	buf.B.Y = -0.9781476;	buf.B.Z = 0;	buf.C.X = 0.27991688;	buf.C.Y = -1.04466403;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.20791169;	buf.A.Y = -0.9781476;	buf.A.Z = 0;	buf.B.X = 0.10452846;	buf.B.Y = -0.9945219;	buf.B.Z = 0;	buf.C.X = 0.16918634;	buf.C.Y = -1.06820053;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.10452846;	buf.A.Y = -0.9945219;	buf.A.Z = 0;	buf.B.X = 0;	buf.B.Y = -1;	buf.B.Z = 0;	buf.C.X = 0.05660216;	buf.C.Y = -1.08003361;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = -1;	buf.A.Z = 0;	buf.B.X = -0.10452846;	buf.B.Y = -0.9945219;	buf.B.Z = 0;	buf.C.X = -0.05660216;	buf.C.Y = -1.08003361;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.10452846;	buf.A.Y = -0.9945219;	buf.A.Z = 0;	buf.B.X = -0.20791169;	buf.B.Y = -0.9781476;	buf.B.Z = 0;	buf.C.X = -0.16918634;	buf.C.Y = -1.06820053;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.20791169;	buf.A.Y = -0.9781476;	buf.A.Z = 0;	buf.B.X = -0.30901699;	buf.B.Y = -0.95105652;	buf.B.Z = 0;	buf.C.X = -0.27991688;	buf.C.Y = -1.04466403;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = -0.95105652;	buf.A.Z = 0;	buf.B.X = -0.40673664;	buf.B.Y = -0.91354546;	buf.B.Z = 0;	buf.C.X = -0.38758059;	buf.C.Y = -1.00968197;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.40673664;	buf.A.Y = -0.91354546;	buf.A.Z = 0;	buf.B.X = -0.5;	buf.B.Y = -0.8660254;	buf.B.Z = 0;	buf.C.X = -0.49099789;	buf.C.Y = -0.96363762;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 7;	out.push_back (buf);
	buf.A.X = -0.58778525;	buf.A.Y = -0.80901699;	buf.A.Z = 0;	buf.B.X = -0.5;	buf.B.Y = -0.8660254;	buf.B.Z = 0;	buf.C.X = -0.49384417;	buf.C.Y = -0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.66913061;	buf.A.Y = -0.74314483;	buf.A.Z = 0;	buf.B.X = -0.58778525;	buf.B.Y = -0.80901699;	buf.B.Z = 0;	buf.C.X = -0.58980658;	buf.C.Y = -0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.74314483;	buf.A.Y = -0.66913061;	buf.A.Z = 0;	buf.B.X = -0.66913061;	buf.B.Y = -0.74314483;	buf.B.Z = 0;	buf.C.X = -0.67779286;	buf.C.Y = -0.71841847;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = -0.58778525;	buf.A.Z = 0;	buf.B.X = -0.74314483;	buf.B.Y = -0.66913061;	buf.B.Z = 0;	buf.C.X = -0.75661316;	buf.C.Y = -0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.8660254;	buf.A.Y = -0.5;	buf.A.Z = 0;	buf.B.X = -0.80901699;	buf.B.Y = -0.58778525;	buf.B.Z = 0;	buf.C.X = -0.82520157;	buf.C.Y = -0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.91354546;	buf.A.Y = -0.40673664;	buf.A.Z = 0;	buf.B.X = -0.8660254;	buf.B.Y = -0.5;	buf.B.Z = 0;	buf.C.X = -0.88263054;	buf.C.Y = -0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = -0.30901699;	buf.A.Z = 0;	buf.B.X = -0.91354546;	buf.B.Y = -0.40673664;	buf.B.Z = 0;	buf.C.X = -0.92812345;	buf.C.Y = -0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.9781476;	buf.A.Y = -0.20791169;	buf.A.Z = 0;	buf.B.X = -0.95105652;	buf.B.Y = -0.30901699;	buf.B.Z = 0;	buf.C.X = -0.96106507;	buf.C.Y = -0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.9945219;	buf.A.Y = -0.10452846;	buf.A.Z = 0;	buf.B.X = -0.9781476;	buf.B.Y = -0.20791169;	buf.B.Z = 0;	buf.C.X = -0.98100995;	buf.C.Y = -0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.58778525;	buf.A.Y = -0.80901699;	buf.A.Z = 0;	buf.B.X = -0.49384417;	buf.B.Y = -0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = -0.58980658;	buf.C.Y = -0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 1;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.66913061;	buf.A.Y = -0.74314483;	buf.A.Z = 0;	buf.B.X = -0.58980658;	buf.B.Y = -0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = -0.67779286;	buf.C.Y = -0.71841847;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.74314483;	buf.A.Y = -0.66913061;	buf.A.Z = 0;	buf.B.X = -0.67779286;	buf.B.Y = -0.71841847;	buf.B.Z = -0.15643447;	buf.C.X = -0.75661316;	buf.C.Y = -0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = -0.58778525;	buf.A.Z = 0;	buf.B.X = -0.75661316;	buf.B.Y = -0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = -0.82520157;	buf.C.Y = -0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.8660254;	buf.A.Y = -0.5;	buf.A.Z = 0;	buf.B.X = -0.82520157;	buf.B.Y = -0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = -0.88263054;	buf.C.Y = -0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.91354546;	buf.A.Y = -0.40673664;	buf.A.Z = 0;	buf.B.X = -0.88263054;	buf.B.Y = -0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = -0.92812345;	buf.C.Y = -0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = -0.30901699;	buf.A.Z = 0;	buf.B.X = -0.92812345;	buf.B.Y = -0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = -0.96106507;	buf.C.Y = -0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.9781476;	buf.A.Y = -0.20791169;	buf.A.Z = 0;	buf.B.X = -0.96106507;	buf.B.Y = -0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = -0.98100995;	buf.C.Y = -0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -1;	buf.A.Y = 0;	buf.A.Z = 0;	buf.B.X = -0.9945219;	buf.B.Y = -0.10452846;	buf.B.Z = 0;	buf.C.X = -0.98768834;	buf.C.Y = 0;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.98768834;	buf.A.Y = 0;	buf.A.Z = -0.15643447;	buf.B.X = -0.9945219;	buf.B.Y = -0.10452846;	buf.B.Z = 0;	buf.C.X = -0.98100995;	buf.C.Y = -0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.58980658;	buf.A.Y = -0.79224773;	buf.A.Z = -0.15643447;	buf.B.X = -0.49384417;	buf.B.Y = -0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = -0.47552826;	buf.C.Y = -0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 3;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.67779286;	buf.A.Y = -0.71841847;	buf.A.Z = -0.15643447;	buf.B.X = -0.58980658;	buf.B.Y = -0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = -0.57896652;	buf.C.Y = -0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.75661316;	buf.A.Y = -0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = -0.67779286;	buf.B.Y = -0.71841847;	buf.B.Z = -0.15643447;	buf.C.X = -0.67249851;	buf.C.Y = -0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.82520157;	buf.A.Y = -0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = -0.75661316;	buf.B.Y = -0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = -0.75452386;	buf.C.Y = -0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 4;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.88263054;	buf.A.Y = -0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = -0.82520157;	buf.B.Y = -0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = -0.8236391;	buf.C.Y = -0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 3;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.92812345;	buf.A.Y = -0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = -0.88263054;	buf.B.Y = -0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = -0.87866165;	buf.C.Y = -0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.96106507;	buf.A.Y = -0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = -0.92812345;	buf.B.Y = -0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = -0.91865005;	buf.C.Y = -0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.98100995;	buf.A.Y = -0.11466362;	buf.A.Z = -0.15643447;	buf.B.X = -0.96106507;	buf.B.Y = -0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = -0.9429201;	buf.C.Y = -0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.58980658;	buf.A.Y = -0.79224773;	buf.A.Z = -0.15643447;	buf.B.X = -0.47552826;	buf.B.Y = -0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = -0.57896652;	buf.C.Y = -0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 1;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.67779286;	buf.A.Y = -0.71841847;	buf.A.Z = -0.15643447;	buf.B.X = -0.57896652;	buf.B.Y = -0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = -0.67249851;	buf.C.Y = -0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 7;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.75661316;	buf.A.Y = -0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = -0.67249851;	buf.B.Y = -0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = -0.75452386;	buf.C.Y = -0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.82520157;	buf.A.Y = -0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = -0.75452386;	buf.B.Y = -0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = -0.8236391;	buf.C.Y = -0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 5;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.88263054;	buf.A.Y = -0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = -0.8236391;	buf.B.Y = -0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = -0.87866165;	buf.C.Y = -0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.92812345;	buf.A.Y = -0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = -0.87866165;	buf.B.Y = -0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = -0.91865005;	buf.C.Y = -0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 6;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.96106507;	buf.A.Y = -0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = -0.91865005;	buf.B.Y = -0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = -0.9429201;	buf.C.Y = -0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.98768834;	buf.A.Y = 0;	buf.A.Z = -0.15643447;	buf.B.X = -0.98100995;	buf.B.Y = -0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = -0.95105652;	buf.C.Y = 0;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = 0;	buf.A.Z = -0.30901699;	buf.B.X = -0.98100995;	buf.B.Y = -0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = -0.9429201;	buf.C.Y = -0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.57896652;	buf.A.Y = -0.75452386;	buf.A.Z = -0.30901699;	buf.B.X = -0.47552826;	buf.B.Y = -0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = -0.44550326;	buf.C.Y = -0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.67249851;	buf.A.Y = -0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = -0.57896652;	buf.B.Y = -0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = -0.55553348;	buf.C.Y = -0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 2;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.75452386;	buf.A.Y = -0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = -0.67249851;	buf.B.Y = -0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = -0.653154;	buf.C.Y = -0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 5;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.8236391;	buf.A.Y = -0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = -0.75452386;	buf.B.Y = -0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = -0.73618414;	buf.C.Y = -0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.87866165;	buf.A.Y = -0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = -0.8236391;	buf.B.Y = -0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = -0.80276914;	buf.C.Y = -0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 2;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.91865005;	buf.A.Y = -0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = -0.87866165;	buf.B.Y = -0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = -0.8514216;	buf.C.Y = -0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.9429201;	buf.A.Y = -0.12413779;	buf.A.Z = -0.30901699;	buf.B.X = -0.91865005;	buf.B.Y = -0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = -0.88105472;	buf.C.Y = -0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.57896652;	buf.A.Y = -0.75452386;	buf.A.Z = -0.30901699;	buf.B.X = -0.44550326;	buf.B.Y = -0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = -0.55553348;	buf.C.Y = -0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.67249851;	buf.A.Y = -0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = -0.55553348;	buf.B.Y = -0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = -0.653154;	buf.C.Y = -0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 2;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.75452386;	buf.A.Y = -0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = -0.653154;	buf.B.Y = -0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = -0.73618414;	buf.C.Y = -0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.8236391;	buf.A.Y = -0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = -0.73618414;	buf.B.Y = -0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = -0.80276914;	buf.C.Y = -0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.87866165;	buf.A.Y = -0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = -0.80276914;	buf.B.Y = -0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = -0.8514216;	buf.C.Y = -0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.91865005;	buf.A.Y = -0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = -0.8514216;	buf.B.Y = -0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = -0.88105472;	buf.C.Y = -0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = 0;	buf.A.Z = -0.30901699;	buf.B.X = -0.9429201;	buf.B.Y = -0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = -0.89100652;	buf.C.Y = 0;	buf.C.Z = -0.4539905;	buf.COUNT = 1;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.89100652;	buf.A.Y = 0;	buf.A.Z = -0.4539905;	buf.B.X = -0.9429201;	buf.B.Y = -0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = -0.88105472;	buf.C.Y = -0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.55553348;	buf.A.Y = -0.69661695;	buf.A.Z = -0.4539905;	buf.B.X = -0.44550326;	buf.B.Y = -0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = -0.4045085;	buf.C.Y = -0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.653154;	buf.A.Y = -0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = -0.55553348;	buf.B.Y = -0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = -0.5200261;	buf.C.Y = -0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.73618414;	buf.A.Y = -0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = -0.653154;	buf.B.Y = -0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = -0.61974297;	buf.C.Y = -0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.80276914;	buf.A.Y = -0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = -0.73618414;	buf.B.Y = -0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = -0.70062927;	buf.C.Y = -0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.8514216;	buf.A.Y = -0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = -0.80276914;	buf.B.Y = -0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = -0.7602273;	buf.C.Y = -0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.88105472;	buf.A.Y = -0.13279763;	buf.A.Z = -0.4539905;	buf.B.X = -0.8514216;	buf.B.Y = -0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = -0.79672621;	buf.C.Y = -0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.55553348;	buf.A.Y = -0.69661695;	buf.A.Z = -0.4539905;	buf.B.X = -0.4045085;	buf.B.Y = -0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = -0.5200261;	buf.C.Y = -0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.653154;	buf.A.Y = -0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = -0.5200261;	buf.B.Y = -0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = -0.61974297;	buf.C.Y = -0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.73618414;	buf.A.Y = -0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = -0.61974297;	buf.B.Y = -0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = -0.70062927;	buf.C.Y = -0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.80276914;	buf.A.Y = -0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = -0.70062927;	buf.B.Y = -0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = -0.7602273;	buf.C.Y = -0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.8514216;	buf.A.Y = -0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = -0.7602273;	buf.B.Y = -0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = -0.79672621;	buf.C.Y = -0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.89100652;	buf.A.Y = 0;	buf.A.Z = -0.4539905;	buf.B.X = -0.88105472;	buf.B.Y = -0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = -0.80901699;	buf.C.Y = 0;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = 0;	buf.A.Z = -0.58778525;	buf.B.X = -0.88105472;	buf.B.Y = -0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = -0.79672621;	buf.C.Y = -0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.5200261;	buf.A.Y = -0.61974297;	buf.A.Z = -0.58778525;	buf.B.X = -0.4045085;	buf.B.Y = -0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = -0.35355339;	buf.C.Y = -0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.61974297;	buf.A.Y = -0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = -0.5200261;	buf.B.Y = -0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = -0.47314679;	buf.C.Y = -0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.70062927;	buf.A.Y = -0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = -0.61974297;	buf.B.Y = -0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = -0.5720614;	buf.C.Y = -0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.7602273;	buf.A.Y = -0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = -0.70062927;	buf.B.Y = -0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = -0.64597419;	buf.C.Y = -0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.79672621;	buf.A.Y = -0.14048433;	buf.A.Z = -0.58778525;	buf.B.X = -0.7602273;	buf.B.Y = -0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = -0.6916548;	buf.C.Y = -0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.5200261;	buf.A.Y = -0.61974297;	buf.A.Z = -0.58778525;	buf.B.X = -0.35355339;	buf.B.Y = -0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = -0.47314679;	buf.C.Y = -0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.61974297;	buf.A.Y = -0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = -0.47314679;	buf.B.Y = -0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = -0.5720614;	buf.C.Y = -0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.70062927;	buf.A.Y = -0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = -0.5720614;	buf.B.Y = -0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = -0.64597419;	buf.C.Y = -0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.7602273;	buf.A.Y = -0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = -0.64597419;	buf.B.Y = -0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = -0.6916548;	buf.C.Y = -0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = 0;	buf.A.Z = -0.58778525;	buf.B.X = -0.79672621;	buf.B.Y = -0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = -0.70710678;	buf.C.Y = 0;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.70710678;	buf.A.Y = 0;	buf.A.Z = -0.70710678;	buf.B.X = -0.79672621;	buf.B.Y = -0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = -0.6916548;	buf.C.Y = -0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.47314679;	buf.A.Y = -0.52548275;	buf.A.Z = -0.70710678;	buf.B.X = -0.35355339;	buf.B.Y = -0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = -0.29389263;	buf.C.Y = -0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.5720614;	buf.A.Y = -0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = -0.47314679;	buf.B.Y = -0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = -0.41562694;	buf.C.Y = -0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.64597419;	buf.A.Y = -0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = -0.5720614;	buf.B.Y = -0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = -0.50903696;	buf.C.Y = -0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.6916548;	buf.A.Y = -0.14701577;	buf.A.Z = -0.70710678;	buf.B.X = -0.64597419;	buf.B.Y = -0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = -0.56775696;	buf.C.Y = -0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.47314679;	buf.A.Y = -0.52548275;	buf.A.Z = -0.70710678;	buf.B.X = -0.29389263;	buf.B.Y = -0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = -0.41562694;	buf.C.Y = -0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.5720614;	buf.A.Y = -0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = -0.41562694;	buf.B.Y = -0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = -0.50903696;	buf.C.Y = -0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.64597419;	buf.A.Y = -0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = -0.50903696;	buf.B.Y = -0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = -0.56775696;	buf.C.Y = -0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.70710678;	buf.A.Y = 0;	buf.A.Z = -0.70710678;	buf.B.X = -0.6916548;	buf.B.Y = -0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = -0.58778525;	buf.C.Y = 0;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.58778525;	buf.A.Y = 0;	buf.A.Z = -0.80901699;	buf.B.X = -0.6916548;	buf.B.Y = -0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = -0.56775696;	buf.C.Y = -0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.41562694;	buf.A.Y = -0.41562694;	buf.A.Z = -0.80901699;	buf.B.X = -0.29389263;	buf.B.Y = -0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = -0.22699525;	buf.C.Y = -0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.50903696;	buf.A.Y = -0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = -0.41562694;	buf.B.Y = -0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = -0.3477769;	buf.C.Y = -0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.56775696;	buf.A.Y = -0.15213002;	buf.A.Z = -0.80901699;	buf.B.X = -0.50903696;	buf.B.Y = -0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = -0.42661152;	buf.C.Y = -0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.41562694;	buf.A.Y = -0.41562694;	buf.A.Z = -0.80901699;	buf.B.X = -0.22699525;	buf.B.Y = -0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = -0.3477769;	buf.C.Y = -0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.50903696;	buf.A.Y = -0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = -0.3477769;	buf.B.Y = -0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = -0.42661152;	buf.C.Y = -0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.58778525;	buf.A.Y = 0;	buf.A.Z = -0.80901699;	buf.B.X = -0.56775696;	buf.B.Y = -0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = -0.4539905;	buf.C.Y = 0;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.4539905;	buf.A.Y = 0;	buf.A.Z = -0.89100652;	buf.B.X = -0.56775696;	buf.B.Y = -0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = -0.42661152;	buf.C.Y = -0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.3477769;	buf.A.Y = -0.29181947;	buf.A.Z = -0.89100652;	buf.B.X = -0.22699525;	buf.B.Y = -0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = -0.1545085;	buf.C.Y = -0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.42661152;	buf.A.Y = -0.1552739;	buf.A.Z = -0.89100652;	buf.B.X = -0.3477769;	buf.B.Y = -0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = -0.26761657;	buf.C.Y = -0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.3477769;	buf.A.Y = -0.29181947;	buf.A.Z = -0.89100652;	buf.B.X = -0.1545085;	buf.B.Y = -0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = -0.26761657;	buf.C.Y = -0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.4539905;	buf.A.Y = 0;	buf.A.Z = -0.89100652;	buf.B.X = -0.42661152;	buf.B.Y = -0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = -0.30901699;	buf.C.Y = 0;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = 0;	buf.A.Z = -0.95105652;	buf.B.X = -0.42661152;	buf.B.Y = -0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = -0.26761657;	buf.C.Y = -0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.26761657;	buf.A.Y = -0.1545085;	buf.A.Z = -0.95105652;	buf.B.X = -0.1545085;	buf.B.Y = -0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = -0.07821723;	buf.C.Y = -0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = 0;	buf.A.Z = -0.95105652;	buf.B.X = -0.26761657;	buf.B.Y = -0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = -0.15643447;	buf.C.Y = 0;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.15643447;	buf.A.Y = 0;	buf.A.Z = -0.98768834;	buf.B.X = -0.26761657;	buf.B.Y = -0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = -0.07821723;	buf.C.Y = -0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.15643447;	buf.A.Y = 0;	buf.A.Z = -0.98768834;	buf.B.X = -0.07821723;	buf.B.Y = -0.13547622;	buf.B.Z = -0.98768834;	buf.C.X = 0;	buf.C.Y = 0;	buf.C.Z = 1;	buf.COUNT = 0;	buf.GROUP = 5;	out.push_back (buf);
	buf.A.X = -0.5;	buf.A.Y = -0.8660254;	buf.A.Z = 0;	buf.B.X = -0.58778525;	buf.B.Y = -0.80901699;	buf.B.Z = 0;	buf.C.X = -0.58903571;	buf.C.Y = -0.90703546;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.58778525;	buf.A.Y = -0.80901699;	buf.A.Z = 0;	buf.B.X = -0.66913061;	buf.B.Y = -0.74314483;	buf.B.Z = 0;	buf.C.X = -0.68061994;	buf.C.Y = -0.84049563;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.66913061;	buf.A.Y = -0.74314483;	buf.A.Z = 0;	buf.B.X = -0.74314483;	buf.B.Y = -0.66913061;	buf.B.Z = 0;	buf.C.X = -0.76474715;	buf.C.Y = -0.76474715;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.74314483;	buf.A.Y = -0.66913061;	buf.A.Z = 0;	buf.B.X = -0.80901699;	buf.B.Y = -0.58778525;	buf.B.Z = 0;	buf.C.X = -0.84049563;	buf.C.Y = -0.68061994;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = -0.58778525;	buf.A.Z = 0;	buf.B.X = -0.8660254;	buf.B.Y = -0.5;	buf.B.Z = 0;	buf.C.X = -0.90703546;	buf.C.Y = -0.58903571;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.8660254;	buf.A.Y = -0.5;	buf.A.Z = 0;	buf.B.X = -0.91354546;	buf.B.Y = -0.40673664;	buf.B.Z = 0;	buf.C.X = -0.96363762;	buf.C.Y = -0.49099789;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.91354546;	buf.A.Y = -0.40673664;	buf.A.Z = 0;	buf.B.X = -0.95105652;	buf.B.Y = -0.30901699;	buf.B.Z = 0;	buf.C.X = -1.00968197;	buf.C.Y = -0.38758059;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = -0.30901699;	buf.A.Z = 0;	buf.B.X = -0.9781476;	buf.B.Y = -0.20791169;	buf.B.Z = 0;	buf.C.X = -1.04466403;	buf.C.Y = -0.27991688;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.9781476;	buf.A.Y = -0.20791169;	buf.A.Z = 0;	buf.B.X = -0.9945219;	buf.B.Y = -0.10452846;	buf.B.Z = 0;	buf.C.X = -1.06820053;	buf.C.Y = -0.16918634;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.9945219;	buf.A.Y = -0.10452846;	buf.A.Z = 0;	buf.B.X = -1;	buf.B.Y = 0;	buf.B.Z = 0;	buf.C.X = -1.08003361;	buf.C.Y = -0.05660216;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 7;	out.push_back (buf);
	buf.A.X = -0.9945219;	buf.A.Y = 0.10452846;	buf.A.Z = 0;	buf.B.X = -1;	buf.B.Y = 0;	buf.B.Z = 0;	buf.C.X = -0.98768834;	buf.C.Y = 0;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.9781476;	buf.A.Y = 0.20791169;	buf.A.Z = 0;	buf.B.X = -0.9945219;	buf.B.Y = 0.10452846;	buf.B.Z = 0;	buf.C.X = -0.98100995;	buf.C.Y = 0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = 0.30901699;	buf.A.Z = 0;	buf.B.X = -0.9781476;	buf.B.Y = 0.20791169;	buf.B.Z = 0;	buf.C.X = -0.96106507;	buf.C.Y = 0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.91354546;	buf.A.Y = 0.40673664;	buf.A.Z = 0;	buf.B.X = -0.95105652;	buf.B.Y = 0.30901699;	buf.B.Z = 0;	buf.C.X = -0.92812345;	buf.C.Y = 0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.8660254;	buf.A.Y = 0.5;	buf.A.Z = 0;	buf.B.X = -0.91354546;	buf.B.Y = 0.40673664;	buf.B.Z = 0;	buf.C.X = -0.88263054;	buf.C.Y = 0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = 0.58778525;	buf.A.Z = 0;	buf.B.X = -0.8660254;	buf.B.Y = 0.5;	buf.B.Z = 0;	buf.C.X = -0.82520157;	buf.C.Y = 0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.74314483;	buf.A.Y = 0.66913061;	buf.A.Z = 0;	buf.B.X = -0.80901699;	buf.B.Y = 0.58778525;	buf.B.Z = 0;	buf.C.X = -0.75661316;	buf.C.Y = 0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.66913061;	buf.A.Y = 0.74314483;	buf.A.Z = 0;	buf.B.X = -0.74314483;	buf.B.Y = 0.66913061;	buf.B.Z = 0;	buf.C.X = -0.67779286;	buf.C.Y = 0.71841846;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.58778525;	buf.A.Y = 0.80901699;	buf.A.Z = 0;	buf.B.X = -0.66913061;	buf.B.Y = 0.74314483;	buf.B.Z = 0;	buf.C.X = -0.58980658;	buf.C.Y = 0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.9945219;	buf.A.Y = 0.10452846;	buf.A.Z = 0;	buf.B.X = -0.98768834;	buf.B.Y = 0;	buf.B.Z = -0.15643447;	buf.C.X = -0.98100995;	buf.C.Y = 0.11466362;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.9781476;	buf.A.Y = 0.20791169;	buf.A.Z = 0;	buf.B.X = -0.98100995;	buf.B.Y = 0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = -0.96106507;	buf.C.Y = 0.22777661;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = 0.30901699;	buf.A.Z = 0;	buf.B.X = -0.96106507;	buf.B.Y = 0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = -0.92812345;	buf.C.Y = 0.33780931;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.91354546;	buf.A.Y = 0.40673664;	buf.A.Z = 0;	buf.B.X = -0.92812345;	buf.B.Y = 0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = -0.88263054;	buf.C.Y = 0.44327372;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.8660254;	buf.A.Y = 0.5;	buf.A.Z = 0;	buf.B.X = -0.88263054;	buf.B.Y = 0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = -0.82520157;	buf.C.Y = 0.54274361;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = 0.58778525;	buf.A.Z = 0;	buf.B.X = -0.82520157;	buf.B.Y = 0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = -0.75661316;	buf.C.Y = 0.63487383;	buf.C.Z = -0.15643447;	buf.COUNT = 1;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.74314483;	buf.A.Y = 0.66913061;	buf.A.Z = 0;	buf.B.X = -0.75661316;	buf.B.Y = 0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = -0.67779286;	buf.C.Y = 0.71841846;	buf.C.Z = -0.15643447;	buf.COUNT = 1;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.66913061;	buf.A.Y = 0.74314483;	buf.A.Z = 0;	buf.B.X = -0.67779286;	buf.B.Y = 0.71841846;	buf.B.Z = -0.15643447;	buf.C.X = -0.58980658;	buf.C.Y = 0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.5;	buf.A.Y = 0.8660254;	buf.A.Z = 0;	buf.B.X = -0.58778525;	buf.B.Y = 0.80901699;	buf.B.Z = 0;	buf.C.X = -0.49384417;	buf.C.Y = 0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.49384417;	buf.A.Y = 0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = -0.58778525;	buf.B.Y = 0.80901699;	buf.B.Z = 0;	buf.C.X = -0.58980658;	buf.C.Y = 0.79224773;	buf.C.Z = -0.15643447;	buf.COUNT = 1;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.98100995;	buf.A.Y = 0.11466362;	buf.A.Z = -0.15643447;	buf.B.X = -0.98768834;	buf.B.Y = 0;	buf.B.Z = -0.15643447;	buf.C.X = -0.95105652;	buf.C.Y = 0;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.96106507;	buf.A.Y = 0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = -0.98100995;	buf.B.Y = 0.11466362;	buf.B.Z = -0.15643447;	buf.C.X = -0.9429201;	buf.C.Y = 0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.92812345;	buf.A.Y = 0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = -0.96106507;	buf.B.Y = 0.22777661;	buf.B.Z = -0.15643447;	buf.C.X = -0.91865005;	buf.C.Y = 0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.88263054;	buf.A.Y = 0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = -0.92812345;	buf.B.Y = 0.33780931;	buf.B.Z = -0.15643447;	buf.C.X = -0.87866165;	buf.C.Y = 0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.82520157;	buf.A.Y = 0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = -0.88263054;	buf.B.Y = 0.44327372;	buf.B.Z = -0.15643447;	buf.C.X = -0.8236391;	buf.C.Y = 0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.75661316;	buf.A.Y = 0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = -0.82520157;	buf.B.Y = 0.54274361;	buf.B.Z = -0.15643447;	buf.C.X = -0.75452386;	buf.C.Y = 0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.67779286;	buf.A.Y = 0.71841846;	buf.A.Z = -0.15643447;	buf.B.X = -0.75661316;	buf.B.Y = 0.63487383;	buf.B.Z = -0.15643447;	buf.C.X = -0.67249851;	buf.C.Y = 0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.58980658;	buf.A.Y = 0.79224773;	buf.A.Z = -0.15643447;	buf.B.X = -0.67779286;	buf.B.Y = 0.71841846;	buf.B.Z = -0.15643447;	buf.C.X = -0.57896652;	buf.C.Y = 0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 1;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.98100995;	buf.A.Y = 0.11466362;	buf.A.Z = -0.15643447;	buf.B.X = -0.95105652;	buf.B.Y = 0;	buf.B.Z = -0.30901699;	buf.C.X = -0.9429201;	buf.C.Y = 0.12413779;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.96106507;	buf.A.Y = 0.22777661;	buf.A.Z = -0.15643447;	buf.B.X = -0.9429201;	buf.B.Y = 0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = -0.91865005;	buf.C.Y = 0.24615154;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.92812345;	buf.A.Y = 0.33780931;	buf.A.Z = -0.15643447;	buf.B.X = -0.91865005;	buf.B.Y = 0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = -0.87866165;	buf.C.Y = 0.36395357;	buf.C.Z = -0.30901699;	buf.COUNT = 1;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.88263054;	buf.A.Y = 0.44327372;	buf.A.Z = -0.15643447;	buf.B.X = -0.87866165;	buf.B.Y = 0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = -0.8236391;	buf.C.Y = 0.47552826;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.82520157;	buf.A.Y = 0.54274361;	buf.A.Z = -0.15643447;	buf.B.X = -0.8236391;	buf.B.Y = 0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = -0.75452386;	buf.C.Y = 0.57896652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.75661316;	buf.A.Y = 0.63487383;	buf.A.Z = -0.15643447;	buf.B.X = -0.75452386;	buf.B.Y = 0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = -0.67249851;	buf.C.Y = 0.67249851;	buf.C.Z = -0.30901699;	buf.COUNT = 6;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.67779286;	buf.A.Y = 0.71841846;	buf.A.Z = -0.15643447;	buf.B.X = -0.67249851;	buf.B.Y = 0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = -0.57896652;	buf.C.Y = 0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 3;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.49384417;	buf.A.Y = 0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = -0.58980658;	buf.B.Y = 0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = -0.47552826;	buf.C.Y = 0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.47552826;	buf.A.Y = 0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = -0.58980658;	buf.B.Y = 0.79224773;	buf.B.Z = -0.15643447;	buf.C.X = -0.57896652;	buf.C.Y = 0.75452386;	buf.C.Z = -0.30901699;	buf.COUNT = 2;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.9429201;	buf.A.Y = 0.12413779;	buf.A.Z = -0.30901699;	buf.B.X = -0.95105652;	buf.B.Y = 0;	buf.B.Z = -0.30901699;	buf.C.X = -0.89100652;	buf.C.Y = 0;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.91865005;	buf.A.Y = 0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = -0.9429201;	buf.B.Y = 0.12413779;	buf.B.Z = -0.30901699;	buf.C.X = -0.88105472;	buf.C.Y = 0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.87866165;	buf.A.Y = 0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = -0.91865005;	buf.B.Y = 0.24615154;	buf.B.Z = -0.30901699;	buf.C.X = -0.8514216;	buf.C.Y = 0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.8236391;	buf.A.Y = 0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = -0.87866165;	buf.B.Y = 0.36395357;	buf.B.Z = -0.30901699;	buf.C.X = -0.80276914;	buf.C.Y = 0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.75452386;	buf.A.Y = 0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = -0.8236391;	buf.B.Y = 0.47552826;	buf.B.Z = -0.30901699;	buf.C.X = -0.73618414;	buf.C.Y = 0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.67249851;	buf.A.Y = 0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = -0.75452386;	buf.B.Y = 0.57896652;	buf.B.Z = -0.30901699;	buf.C.X = -0.653154;	buf.C.Y = 0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.57896652;	buf.A.Y = 0.75452386;	buf.A.Z = -0.30901699;	buf.B.X = -0.67249851;	buf.B.Y = 0.67249851;	buf.B.Z = -0.30901699;	buf.C.X = -0.55553348;	buf.C.Y = 0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.9429201;	buf.A.Y = 0.12413779;	buf.A.Z = -0.30901699;	buf.B.X = -0.89100652;	buf.B.Y = 0;	buf.B.Z = -0.4539905;	buf.C.X = -0.88105472;	buf.C.Y = 0.13279763;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.91865005;	buf.A.Y = 0.24615154;	buf.A.Z = -0.30901699;	buf.B.X = -0.88105472;	buf.B.Y = 0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = -0.8514216;	buf.C.Y = 0.26262878;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.87866165;	buf.A.Y = 0.36395357;	buf.A.Z = -0.30901699;	buf.B.X = -0.8514216;	buf.B.Y = 0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = -0.80276914;	buf.C.Y = 0.38659324;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.8236391;	buf.A.Y = 0.47552826;	buf.A.Z = -0.30901699;	buf.B.X = -0.80276914;	buf.B.Y = 0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = -0.73618414;	buf.C.Y = 0.50192185;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.75452386;	buf.A.Y = 0.57896652;	buf.A.Z = -0.30901699;	buf.B.X = -0.73618414;	buf.B.Y = 0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = -0.653154;	buf.C.Y = 0.60603835;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.67249851;	buf.A.Y = 0.67249851;	buf.A.Z = -0.30901699;	buf.B.X = -0.653154;	buf.B.Y = 0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = -0.55553348;	buf.C.Y = 0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.47552826;	buf.A.Y = 0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = -0.57896652;	buf.B.Y = 0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = -0.44550326;	buf.C.Y = 0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.44550326;	buf.A.Y = 0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = -0.57896652;	buf.B.Y = 0.75452386;	buf.B.Z = -0.30901699;	buf.C.X = -0.55553348;	buf.C.Y = 0.69661695;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.88105472;	buf.A.Y = 0.13279763;	buf.A.Z = -0.4539905;	buf.B.X = -0.89100652;	buf.B.Y = 0;	buf.B.Z = -0.4539905;	buf.C.X = -0.80901699;	buf.C.Y = 0;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.8514216;	buf.A.Y = 0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = -0.88105472;	buf.B.Y = 0.13279763;	buf.B.Z = -0.4539905;	buf.C.X = -0.79672621;	buf.C.Y = 0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.80276914;	buf.A.Y = 0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = -0.8514216;	buf.B.Y = 0.26262878;	buf.B.Z = -0.4539905;	buf.C.X = -0.7602273;	buf.C.Y = 0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.73618414;	buf.A.Y = 0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = -0.80276914;	buf.B.Y = 0.38659324;	buf.B.Z = -0.4539905;	buf.C.X = -0.70062927;	buf.C.Y = 0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.653154;	buf.A.Y = 0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = -0.73618414;	buf.B.Y = 0.50192185;	buf.B.Z = -0.4539905;	buf.C.X = -0.61974297;	buf.C.Y = 0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.55553348;	buf.A.Y = 0.69661695;	buf.A.Z = -0.4539905;	buf.B.X = -0.653154;	buf.B.Y = 0.60603835;	buf.B.Z = -0.4539905;	buf.C.X = -0.5200261;	buf.C.Y = 0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.88105472;	buf.A.Y = 0.13279763;	buf.A.Z = -0.4539905;	buf.B.X = -0.80901699;	buf.B.Y = 0;	buf.B.Z = -0.58778525;	buf.C.X = -0.79672621;	buf.C.Y = 0.14048433;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.8514216;	buf.A.Y = 0.26262878;	buf.A.Z = -0.4539905;	buf.B.X = -0.79672621;	buf.B.Y = 0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = -0.7602273;	buf.C.Y = 0.27670011;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.80276914;	buf.A.Y = 0.38659324;	buf.A.Z = -0.4539905;	buf.B.X = -0.7602273;	buf.B.Y = 0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = -0.70062927;	buf.C.Y = 0.4045085;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.73618414;	buf.A.Y = 0.50192185;	buf.A.Z = -0.4539905;	buf.B.X = -0.70062927;	buf.B.Y = 0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = -0.61974297;	buf.C.Y = 0.5200261;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.653154;	buf.A.Y = 0.60603835;	buf.A.Z = -0.4539905;	buf.B.X = -0.61974297;	buf.B.Y = 0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = -0.5200261;	buf.C.Y = 0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.44550326;	buf.A.Y = 0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = -0.55553348;	buf.B.Y = 0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = -0.4045085;	buf.C.Y = 0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.4045085;	buf.A.Y = 0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = -0.55553348;	buf.B.Y = 0.69661695;	buf.B.Z = -0.4539905;	buf.C.X = -0.5200261;	buf.C.Y = 0.61974297;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.79672621;	buf.A.Y = 0.14048433;	buf.A.Z = -0.58778525;	buf.B.X = -0.80901699;	buf.B.Y = 0;	buf.B.Z = -0.58778525;	buf.C.X = -0.70710678;	buf.C.Y = 0;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.7602273;	buf.A.Y = 0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = -0.79672621;	buf.B.Y = 0.14048433;	buf.B.Z = -0.58778525;	buf.C.X = -0.6916548;	buf.C.Y = 0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.70062927;	buf.A.Y = 0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = -0.7602273;	buf.B.Y = 0.27670011;	buf.B.Z = -0.58778525;	buf.C.X = -0.64597419;	buf.C.Y = 0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.61974297;	buf.A.Y = 0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = -0.70062927;	buf.B.Y = 0.4045085;	buf.B.Z = -0.58778525;	buf.C.X = -0.5720614;	buf.C.Y = 0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.5200261;	buf.A.Y = 0.61974297;	buf.A.Z = -0.58778525;	buf.B.X = -0.61974297;	buf.B.Y = 0.5200261;	buf.B.Z = -0.58778525;	buf.C.X = -0.47314679;	buf.C.Y = 0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.79672621;	buf.A.Y = 0.14048433;	buf.A.Z = -0.58778525;	buf.B.X = -0.70710678;	buf.B.Y = 0;	buf.B.Z = -0.70710678;	buf.C.X = -0.6916548;	buf.C.Y = 0.14701577;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.7602273;	buf.A.Y = 0.27670011;	buf.A.Z = -0.58778525;	buf.B.X = -0.6916548;	buf.B.Y = 0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = -0.64597419;	buf.C.Y = 0.28760624;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.70062927;	buf.A.Y = 0.4045085;	buf.A.Z = -0.58778525;	buf.B.X = -0.64597419;	buf.B.Y = 0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = -0.5720614;	buf.C.Y = 0.41562694;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.61974297;	buf.A.Y = 0.5200261;	buf.A.Z = -0.58778525;	buf.B.X = -0.5720614;	buf.B.Y = 0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = -0.47314679;	buf.C.Y = 0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.4045085;	buf.A.Y = 0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = -0.5200261;	buf.B.Y = 0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = -0.35355339;	buf.C.Y = 0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.35355339;	buf.A.Y = 0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = -0.5200261;	buf.B.Y = 0.61974297;	buf.B.Z = -0.58778525;	buf.C.X = -0.47314679;	buf.C.Y = 0.52548275;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.6916548;	buf.A.Y = 0.14701577;	buf.A.Z = -0.70710678;	buf.B.X = -0.70710678;	buf.B.Y = 0;	buf.B.Z = -0.70710678;	buf.C.X = -0.58778525;	buf.C.Y = 0;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.64597419;	buf.A.Y = 0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = -0.6916548;	buf.B.Y = 0.14701577;	buf.B.Z = -0.70710678;	buf.C.X = -0.56775696;	buf.C.Y = 0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.5720614;	buf.A.Y = 0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = -0.64597419;	buf.B.Y = 0.28760624;	buf.B.Z = -0.70710678;	buf.C.X = -0.50903696;	buf.C.Y = 0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.47314679;	buf.A.Y = 0.52548275;	buf.A.Z = -0.70710678;	buf.B.X = -0.5720614;	buf.B.Y = 0.41562694;	buf.B.Z = -0.70710678;	buf.C.X = -0.41562694;	buf.C.Y = 0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.6916548;	buf.A.Y = 0.14701577;	buf.A.Z = -0.70710678;	buf.B.X = -0.58778525;	buf.B.Y = 0;	buf.B.Z = -0.80901699;	buf.C.X = -0.56775696;	buf.C.Y = 0.15213002;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.64597419;	buf.A.Y = 0.28760624;	buf.A.Z = -0.70710678;	buf.B.X = -0.56775696;	buf.B.Y = 0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = -0.50903696;	buf.C.Y = 0.29389263;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.5720614;	buf.A.Y = 0.41562694;	buf.A.Z = -0.70710678;	buf.B.X = -0.50903696;	buf.B.Y = 0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = -0.41562694;	buf.C.Y = 0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.35355339;	buf.A.Y = 0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = -0.47314679;	buf.B.Y = 0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = -0.29389263;	buf.C.Y = 0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.29389263;	buf.A.Y = 0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = -0.47314679;	buf.B.Y = 0.52548275;	buf.B.Z = -0.70710678;	buf.C.X = -0.41562694;	buf.C.Y = 0.41562694;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.56775696;	buf.A.Y = 0.15213002;	buf.A.Z = -0.80901699;	buf.B.X = -0.58778525;	buf.B.Y = 0;	buf.B.Z = -0.80901699;	buf.C.X = -0.4539905;	buf.C.Y = 0;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.50903696;	buf.A.Y = 0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = -0.56775696;	buf.B.Y = 0.15213002;	buf.B.Z = -0.80901699;	buf.C.X = -0.42661152;	buf.C.Y = 0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.41562694;	buf.A.Y = 0.41562694;	buf.A.Z = -0.80901699;	buf.B.X = -0.50903696;	buf.B.Y = 0.29389263;	buf.B.Z = -0.80901699;	buf.C.X = -0.3477769;	buf.C.Y = 0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.56775696;	buf.A.Y = 0.15213002;	buf.A.Z = -0.80901699;	buf.B.X = -0.4539905;	buf.B.Y = 0;	buf.B.Z = -0.89100652;	buf.C.X = -0.42661152;	buf.C.Y = 0.1552739;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.50903696;	buf.A.Y = 0.29389263;	buf.A.Z = -0.80901699;	buf.B.X = -0.42661152;	buf.B.Y = 0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = -0.3477769;	buf.C.Y = 0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.29389263;	buf.A.Y = 0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = -0.41562694;	buf.B.Y = 0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = -0.22699525;	buf.C.Y = 0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.22699525;	buf.A.Y = 0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = -0.41562694;	buf.B.Y = 0.41562694;	buf.B.Z = -0.80901699;	buf.C.X = -0.3477769;	buf.C.Y = 0.29181947;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.42661152;	buf.A.Y = 0.1552739;	buf.A.Z = -0.89100652;	buf.B.X = -0.4539905;	buf.B.Y = 0;	buf.B.Z = -0.89100652;	buf.C.X = -0.30901699;	buf.C.Y = 0;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.3477769;	buf.A.Y = 0.29181947;	buf.A.Z = -0.89100652;	buf.B.X = -0.42661152;	buf.B.Y = 0.1552739;	buf.B.Z = -0.89100652;	buf.C.X = -0.26761657;	buf.C.Y = 0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.42661152;	buf.A.Y = 0.1552739;	buf.A.Z = -0.89100652;	buf.B.X = -0.30901699;	buf.B.Y = 0;	buf.B.Z = -0.95105652;	buf.C.X = -0.26761657;	buf.C.Y = 0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.22699525;	buf.A.Y = 0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = -0.3477769;	buf.B.Y = 0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = -0.1545085;	buf.C.Y = 0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.1545085;	buf.A.Y = 0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = -0.3477769;	buf.B.Y = 0.29181947;	buf.B.Z = -0.89100652;	buf.C.X = -0.26761657;	buf.C.Y = 0.1545085;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.26761657;	buf.A.Y = 0.1545085;	buf.A.Z = -0.95105652;	buf.B.X = -0.30901699;	buf.B.Y = 0;	buf.B.Z = -0.95105652;	buf.C.X = -0.15643447;	buf.C.Y = 0;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.1545085;	buf.A.Y = 0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = -0.26761657;	buf.B.Y = 0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = -0.07821723;	buf.C.Y = 0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = -0.07821723;	buf.A.Y = 0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = -0.26761657;	buf.B.Y = 0.1545085;	buf.B.Z = -0.95105652;	buf.C.X = -0.15643447;	buf.C.Y = 0;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.07821723;	buf.A.Y = 0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = -0.15643447;	buf.B.Y = 0;	buf.B.Z = -0.98768834;	buf.C.X = 0;	buf.C.Y = 0;	buf.C.Z = 1;	buf.COUNT = 0;	buf.GROUP = 5;	out.push_back (buf);
	buf.A.X = -1;	buf.A.Y = 0;	buf.A.Z = 0;	buf.B.X = -0.9945219;	buf.B.Y = 0.10452846;	buf.B.Z = 0;	buf.C.X = -1.08003361;	buf.C.Y = 0.05660216;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.9945219;	buf.A.Y = 0.10452846;	buf.A.Z = 0;	buf.B.X = -0.9781476;	buf.B.Y = 0.20791169;	buf.B.Z = 0;	buf.C.X = -1.06820053;	buf.C.Y = 0.16918634;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.9781476;	buf.A.Y = 0.20791169;	buf.A.Z = 0;	buf.B.X = -0.95105652;	buf.B.Y = 0.30901699;	buf.B.Z = 0;	buf.C.X = -1.04466403;	buf.C.Y = 0.27991688;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.95105652;	buf.A.Y = 0.30901699;	buf.A.Z = 0;	buf.B.X = -0.91354546;	buf.B.Y = 0.40673664;	buf.B.Z = 0;	buf.C.X = -1.00968197;	buf.C.Y = 0.38758059;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.91354546;	buf.A.Y = 0.40673664;	buf.A.Z = 0;	buf.B.X = -0.8660254;	buf.B.Y = 0.5;	buf.B.Z = 0;	buf.C.X = -0.96363762;	buf.C.Y = 0.49099789;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.8660254;	buf.A.Y = 0.5;	buf.A.Z = 0;	buf.B.X = -0.80901699;	buf.B.Y = 0.58778525;	buf.B.Z = 0;	buf.C.X = -0.90703546;	buf.C.Y = 0.58903571;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.80901699;	buf.A.Y = 0.58778525;	buf.A.Z = 0;	buf.B.X = -0.74314483;	buf.B.Y = 0.66913061;	buf.B.Z = 0;	buf.C.X = -0.84049563;	buf.C.Y = 0.68061994;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.74314483;	buf.A.Y = 0.66913061;	buf.A.Z = 0;	buf.B.X = -0.66913061;	buf.B.Y = 0.74314483;	buf.B.Z = 0;	buf.C.X = -0.76474715;	buf.C.Y = 0.76474715;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.66913061;	buf.A.Y = 0.74314483;	buf.A.Z = 0;	buf.B.X = -0.58778525;	buf.B.Y = 0.80901699;	buf.B.Z = 0;	buf.C.X = -0.68061994;	buf.C.Y = 0.84049563;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.58778525;	buf.A.Y = 0.80901699;	buf.A.Z = 0;	buf.B.X = -0.5;	buf.B.Y = 0.8660254;	buf.B.Z = 0;	buf.C.X = -0.58903571;	buf.C.Y = 0.90703546;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 7;	out.push_back (buf);
	buf.A.X = -0.40673664;	buf.A.Y = 0.91354546;	buf.A.Z = 0;	buf.B.X = -0.5;	buf.B.Y = 0.8660254;	buf.B.Z = 0;	buf.C.X = -0.49384417;	buf.C.Y = 0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = 0.95105652;	buf.A.Z = 0;	buf.B.X = -0.40673664;	buf.B.Y = 0.91354546;	buf.B.Z = 0;	buf.C.X = -0.39120337;	buf.C.Y = 0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.20791169;	buf.A.Y = 0.9781476;	buf.A.Z = 0;	buf.B.X = -0.30901699;	buf.B.Y = 0.95105652;	buf.B.Z = 0;	buf.C.X = -0.28327221;	buf.C.Y = 0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.10452846;	buf.A.Y = 0.9945219;	buf.A.Z = 0;	buf.B.X = -0.20791169;	buf.B.Y = 0.9781476;	buf.B.Z = 0;	buf.C.X = -0.17151028;	buf.C.Y = 0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 1;	buf.A.Z = 0;	buf.B.X = -0.10452846;	buf.B.Y = 0.9945219;	buf.B.Z = 0;	buf.C.X = -0.05742897;	buf.C.Y = 0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.10452846;	buf.A.Y = 0.9945219;	buf.A.Z = 0;	buf.B.X = 0;	buf.B.Y = 1;	buf.B.Z = 0;	buf.C.X = 0.05742897;	buf.C.Y = 0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.20791169;	buf.A.Y = 0.9781476;	buf.A.Z = 0;	buf.B.X = 0.10452846;	buf.B.Y = 0.9945219;	buf.B.Z = 0;	buf.C.X = 0.17151028;	buf.C.Y = 0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = 0.95105652;	buf.A.Z = 0;	buf.B.X = 0.20791169;	buf.B.Y = 0.9781476;	buf.B.Z = 0;	buf.C.X = 0.28327221;	buf.C.Y = 0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.40673664;	buf.A.Y = 0.91354546;	buf.A.Z = 0;	buf.B.X = 0.30901699;	buf.B.Y = 0.95105652;	buf.B.Z = 0;	buf.C.X = 0.39120337;	buf.C.Y = 0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.40673664;	buf.A.Y = 0.91354546;	buf.A.Z = 0;	buf.B.X = -0.49384417;	buf.B.Y = 0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = -0.39120337;	buf.C.Y = 0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = 0.95105652;	buf.A.Z = 0;	buf.B.X = -0.39120337;	buf.B.Y = 0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = -0.28327221;	buf.C.Y = 0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.20791169;	buf.A.Y = 0.9781476;	buf.A.Z = 0;	buf.B.X = -0.28327221;	buf.B.Y = 0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = -0.17151028;	buf.C.Y = 0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.10452846;	buf.A.Y = 0.9945219;	buf.A.Z = 0;	buf.B.X = -0.17151028;	buf.B.Y = 0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = -0.05742897;	buf.C.Y = 0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 1;	buf.A.Z = 0;	buf.B.X = -0.05742897;	buf.B.Y = 0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = 0.05742897;	buf.C.Y = 0.98601733;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.10452846;	buf.A.Y = 0.9945219;	buf.A.Z = 0;	buf.B.X = 0.05742897;	buf.B.Y = 0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = 0.17151028;	buf.C.Y = 0.97268314;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.20791169;	buf.A.Y = 0.9781476;	buf.A.Z = 0;	buf.B.X = 0.17151028;	buf.B.Y = 0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = 0.28327221;	buf.C.Y = 0.94619507;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = 0.95105652;	buf.A.Z = 0;	buf.B.X = 0.28327221;	buf.B.Y = 0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = 0.39120337;	buf.C.Y = 0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.5;	buf.A.Y = 0.8660254;	buf.A.Z = 0;	buf.B.X = 0.40673664;	buf.B.Y = 0.91354546;	buf.B.Z = 0;	buf.C.X = 0.49384417;	buf.C.Y = 0.85536319;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.49384417;	buf.A.Y = 0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = 0.40673664;	buf.B.Y = 0.91354546;	buf.B.Z = 0;	buf.C.X = 0.39120337;	buf.C.Y = 0.90691134;	buf.C.Z = -0.15643447;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.39120337;	buf.A.Y = 0.90691134;	buf.A.Z = -0.15643447;	buf.B.X = -0.49384417;	buf.B.Y = 0.85536319;	buf.B.Z = -0.15643447;	buf.C.X = -0.47552826;	buf.C.Y = 0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.28327221;	buf.A.Y = 0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = -0.39120337;	buf.B.Y = 0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = -0.36395357;	buf.C.Y = 0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.17151028;	buf.A.Y = 0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = -0.28327221;	buf.B.Y = 0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = -0.24615154;	buf.C.Y = 0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.05742897;	buf.A.Y = 0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = -0.17151028;	buf.B.Y = 0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = -0.12413779;	buf.C.Y = 0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.05742897;	buf.A.Y = 0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = -0.05742897;	buf.B.Y = 0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = 0;	buf.C.Y = 0.95105652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.17151028;	buf.A.Y = 0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = 0.05742897;	buf.B.Y = 0.98601733;	buf.B.Z = -0.15643447;	buf.C.X = 0.12413779;	buf.C.Y = 0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.28327221;	buf.A.Y = 0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = 0.17151028;	buf.B.Y = 0.97268314;	buf.B.Z = -0.15643447;	buf.C.X = 0.24615154;	buf.C.Y = 0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.39120337;	buf.A.Y = 0.90691134;	buf.A.Z = -0.15643447;	buf.B.X = 0.28327221;	buf.B.Y = 0.94619507;	buf.B.Z = -0.15643447;	buf.C.X = 0.36395357;	buf.C.Y = 0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.39120337;	buf.A.Y = 0.90691134;	buf.A.Z = -0.15643447;	buf.B.X = -0.47552826;	buf.B.Y = 0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = -0.36395357;	buf.C.Y = 0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.28327221;	buf.A.Y = 0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = -0.36395357;	buf.B.Y = 0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = -0.24615154;	buf.C.Y = 0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.17151028;	buf.A.Y = 0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = -0.24615154;	buf.B.Y = 0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = -0.12413779;	buf.C.Y = 0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.05742897;	buf.A.Y = 0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = -0.12413779;	buf.B.Y = 0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = 0;	buf.C.Y = 0.95105652;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.05742897;	buf.A.Y = 0.98601733;	buf.A.Z = -0.15643447;	buf.B.X = 0;	buf.B.Y = 0.95105652;	buf.B.Z = -0.30901699;	buf.C.X = 0.12413779;	buf.C.Y = 0.9429201;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.17151028;	buf.A.Y = 0.97268314;	buf.A.Z = -0.15643447;	buf.B.X = 0.12413779;	buf.B.Y = 0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = 0.24615154;	buf.C.Y = 0.91865005;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.28327221;	buf.A.Y = 0.94619507;	buf.A.Z = -0.15643447;	buf.B.X = 0.24615154;	buf.B.Y = 0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = 0.36395357;	buf.C.Y = 0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.49384417;	buf.A.Y = 0.85536319;	buf.A.Z = -0.15643447;	buf.B.X = 0.39120337;	buf.B.Y = 0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = 0.47552826;	buf.C.Y = 0.8236391;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.47552826;	buf.A.Y = 0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = 0.39120337;	buf.B.Y = 0.90691134;	buf.B.Z = -0.15643447;	buf.C.X = 0.36395357;	buf.C.Y = 0.87866165;	buf.C.Z = -0.30901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.36395357;	buf.A.Y = 0.87866165;	buf.A.Z = -0.30901699;	buf.B.X = -0.47552826;	buf.B.Y = 0.8236391;	buf.B.Z = -0.30901699;	buf.C.X = -0.44550326;	buf.C.Y = 0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.24615154;	buf.A.Y = 0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = -0.36395357;	buf.B.Y = 0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = -0.32552124;	buf.C.Y = 0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.12413779;	buf.A.Y = 0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = -0.24615154;	buf.B.Y = 0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = -0.1982676;	buf.C.Y = 0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 0.95105652;	buf.A.Z = -0.30901699;	buf.B.X = -0.12413779;	buf.B.Y = 0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = -0.066585;	buf.C.Y = 0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.12413779;	buf.A.Y = 0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = 0;	buf.B.Y = 0.95105652;	buf.B.Z = -0.30901699;	buf.C.X = 0.066585;	buf.C.Y = 0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.24615154;	buf.A.Y = 0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = 0.12413779;	buf.B.Y = 0.9429201;	buf.B.Z = -0.30901699;	buf.C.X = 0.1982676;	buf.C.Y = 0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.36395357;	buf.A.Y = 0.87866165;	buf.A.Z = -0.30901699;	buf.B.X = 0.24615154;	buf.B.Y = 0.91865005;	buf.B.Z = -0.30901699;	buf.C.X = 0.32552124;	buf.C.Y = 0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.36395357;	buf.A.Y = 0.87866165;	buf.A.Z = -0.30901699;	buf.B.X = -0.44550326;	buf.B.Y = 0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = -0.32552124;	buf.C.Y = 0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.24615154;	buf.A.Y = 0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = -0.32552124;	buf.B.Y = 0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = -0.1982676;	buf.C.Y = 0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.12413779;	buf.A.Y = 0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = -0.1982676;	buf.B.Y = 0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = -0.066585;	buf.C.Y = 0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 0.95105652;	buf.A.Z = -0.30901699;	buf.B.X = -0.066585;	buf.B.Y = 0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = 0.066585;	buf.C.Y = 0.88851509;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.12413779;	buf.A.Y = 0.9429201;	buf.A.Z = -0.30901699;	buf.B.X = 0.066585;	buf.B.Y = 0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = 0.1982676;	buf.C.Y = 0.86866713;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.24615154;	buf.A.Y = 0.91865005;	buf.A.Z = -0.30901699;	buf.B.X = 0.1982676;	buf.B.Y = 0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = 0.32552124;	buf.C.Y = 0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.47552826;	buf.A.Y = 0.8236391;	buf.A.Z = -0.30901699;	buf.B.X = 0.36395357;	buf.B.Y = 0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = 0.44550326;	buf.C.Y = 0.77163428;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.44550326;	buf.A.Y = 0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = 0.36395357;	buf.B.Y = 0.87866165;	buf.B.Z = -0.30901699;	buf.C.X = 0.32552124;	buf.C.Y = 0.82941458;	buf.C.Z = -0.4539905;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.32552124;	buf.A.Y = 0.82941458;	buf.A.Z = -0.4539905;	buf.B.X = -0.44550326;	buf.B.Y = 0.77163428;	buf.B.Z = -0.4539905;	buf.C.X = -0.4045085;	buf.C.Y = 0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.1982676;	buf.A.Y = 0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = -0.32552124;	buf.B.Y = 0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = -0.27670011;	buf.C.Y = 0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.066585;	buf.A.Y = 0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = -0.1982676;	buf.B.Y = 0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = -0.14048433;	buf.C.Y = 0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.066585;	buf.A.Y = 0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = -0.066585;	buf.B.Y = 0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = 0;	buf.C.Y = 0.80901699;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.1982676;	buf.A.Y = 0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = 0.066585;	buf.B.Y = 0.88851509;	buf.B.Z = -0.4539905;	buf.C.X = 0.14048433;	buf.C.Y = 0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.32552124;	buf.A.Y = 0.82941458;	buf.A.Z = -0.4539905;	buf.B.X = 0.1982676;	buf.B.Y = 0.86866713;	buf.B.Z = -0.4539905;	buf.C.X = 0.27670011;	buf.C.Y = 0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.32552124;	buf.A.Y = 0.82941458;	buf.A.Z = -0.4539905;	buf.B.X = -0.4045085;	buf.B.Y = 0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = -0.27670011;	buf.C.Y = 0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.1982676;	buf.A.Y = 0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = -0.27670011;	buf.B.Y = 0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = -0.14048433;	buf.C.Y = 0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.066585;	buf.A.Y = 0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = -0.14048433;	buf.B.Y = 0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = 0;	buf.C.Y = 0.80901699;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.066585;	buf.A.Y = 0.88851509;	buf.A.Z = -0.4539905;	buf.B.X = 0;	buf.B.Y = 0.80901699;	buf.B.Z = -0.58778525;	buf.C.X = 0.14048433;	buf.C.Y = 0.79672621;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.1982676;	buf.A.Y = 0.86866713;	buf.A.Z = -0.4539905;	buf.B.X = 0.14048433;	buf.B.Y = 0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = 0.27670011;	buf.C.Y = 0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.44550326;	buf.A.Y = 0.77163428;	buf.A.Z = -0.4539905;	buf.B.X = 0.32552124;	buf.B.Y = 0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = 0.4045085;	buf.C.Y = 0.70062927;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.4045085;	buf.A.Y = 0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = 0.32552124;	buf.B.Y = 0.82941458;	buf.B.Z = -0.4539905;	buf.C.X = 0.27670011;	buf.C.Y = 0.7602273;	buf.C.Z = -0.58778525;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.27670011;	buf.A.Y = 0.7602273;	buf.A.Z = -0.58778525;	buf.B.X = -0.4045085;	buf.B.Y = 0.70062927;	buf.B.Z = -0.58778525;	buf.C.X = -0.35355339;	buf.C.Y = 0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.14048433;	buf.A.Y = 0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = -0.27670011;	buf.B.Y = 0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = -0.21850801;	buf.C.Y = 0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 0.80901699;	buf.A.Z = -0.58778525;	buf.B.X = -0.14048433;	buf.B.Y = 0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = -0.07391279;	buf.C.Y = 0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.14048433;	buf.A.Y = 0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = 0;	buf.B.Y = 0.80901699;	buf.B.Z = -0.58778525;	buf.C.X = 0.07391279;	buf.C.Y = 0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.27670011;	buf.A.Y = 0.7602273;	buf.A.Z = -0.58778525;	buf.B.X = 0.14048433;	buf.B.Y = 0.79672621;	buf.B.Z = -0.58778525;	buf.C.X = 0.21850801;	buf.C.Y = 0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.27670011;	buf.A.Y = 0.7602273;	buf.A.Z = -0.58778525;	buf.B.X = -0.35355339;	buf.B.Y = 0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = -0.21850801;	buf.C.Y = 0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.14048433;	buf.A.Y = 0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = -0.21850801;	buf.B.Y = 0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = -0.07391279;	buf.C.Y = 0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 0.80901699;	buf.A.Z = -0.58778525;	buf.B.X = -0.07391279;	buf.B.Y = 0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = 0.07391279;	buf.C.Y = 0.70323318;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.14048433;	buf.A.Y = 0.79672621;	buf.A.Z = -0.58778525;	buf.B.X = 0.07391279;	buf.B.Y = 0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = 0.21850801;	buf.C.Y = 0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.4045085;	buf.A.Y = 0.70062927;	buf.A.Z = -0.58778525;	buf.B.X = 0.27670011;	buf.B.Y = 0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = 0.35355339;	buf.C.Y = 0.61237244;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.35355339;	buf.A.Y = 0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = 0.27670011;	buf.B.Y = 0.7602273;	buf.B.Z = -0.58778525;	buf.C.X = 0.21850801;	buf.C.Y = 0.67249851;	buf.C.Z = -0.70710678;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.21850801;	buf.A.Y = 0.67249851;	buf.A.Z = -0.70710678;	buf.B.X = -0.35355339;	buf.B.Y = 0.61237244;	buf.B.Z = -0.70710678;	buf.C.X = -0.29389263;	buf.C.Y = 0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.07391279;	buf.A.Y = 0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = -0.21850801;	buf.B.Y = 0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = -0.15213002;	buf.C.Y = 0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.07391279;	buf.A.Y = 0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = -0.07391279;	buf.B.Y = 0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = 0;	buf.C.Y = 0.58778525;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.21850801;	buf.A.Y = 0.67249851;	buf.A.Z = -0.70710678;	buf.B.X = 0.07391279;	buf.B.Y = 0.70323318;	buf.B.Z = -0.70710678;	buf.C.X = 0.15213002;	buf.C.Y = 0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.21850801;	buf.A.Y = 0.67249851;	buf.A.Z = -0.70710678;	buf.B.X = -0.29389263;	buf.B.Y = 0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = -0.15213002;	buf.C.Y = 0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = -0.07391279;	buf.A.Y = 0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = -0.15213002;	buf.B.Y = 0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = 0;	buf.C.Y = 0.58778525;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.07391279;	buf.A.Y = 0.70323318;	buf.A.Z = -0.70710678;	buf.B.X = 0;	buf.B.Y = 0.58778525;	buf.B.Z = -0.80901699;	buf.C.X = 0.15213002;	buf.C.Y = 0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.35355339;	buf.A.Y = 0.61237244;	buf.A.Z = -0.70710678;	buf.B.X = 0.21850801;	buf.B.Y = 0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = 0.29389263;	buf.C.Y = 0.50903696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.29389263;	buf.A.Y = 0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = 0.21850801;	buf.B.Y = 0.67249851;	buf.B.Z = -0.70710678;	buf.C.X = 0.15213002;	buf.C.Y = 0.56775696;	buf.C.Z = -0.80901699;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.15213002;	buf.A.Y = 0.56775696;	buf.A.Z = -0.80901699;	buf.B.X = -0.29389263;	buf.B.Y = 0.50903696;	buf.B.Z = -0.80901699;	buf.C.X = -0.22699525;	buf.C.Y = 0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 0.58778525;	buf.A.Z = -0.80901699;	buf.B.X = -0.15213002;	buf.B.Y = 0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = -0.07883462;	buf.C.Y = 0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.15213002;	buf.A.Y = 0.56775696;	buf.A.Z = -0.80901699;	buf.B.X = 0;	buf.B.Y = 0.58778525;	buf.B.Z = -0.80901699;	buf.C.X = 0.07883462;	buf.C.Y = 0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.15213002;	buf.A.Y = 0.56775696;	buf.A.Z = -0.80901699;	buf.B.X = -0.22699525;	buf.B.Y = 0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = -0.07883462;	buf.C.Y = 0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 0.58778525;	buf.A.Z = -0.80901699;	buf.B.X = -0.07883462;	buf.B.Y = 0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = 0.07883462;	buf.C.Y = 0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.29389263;	buf.A.Y = 0.50903696;	buf.A.Z = -0.80901699;	buf.B.X = 0.15213002;	buf.B.Y = 0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = 0.22699525;	buf.C.Y = 0.39316731;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.22699525;	buf.A.Y = 0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = 0.15213002;	buf.B.Y = 0.56775696;	buf.B.Z = -0.80901699;	buf.C.X = 0.07883462;	buf.C.Y = 0.44709336;	buf.C.Z = -0.89100652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = -0.07883462;	buf.A.Y = 0.44709336;	buf.A.Z = -0.89100652;	buf.B.X = -0.22699525;	buf.B.Y = 0.39316731;	buf.B.Z = -0.89100652;	buf.C.X = -0.1545085;	buf.C.Y = 0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.07883462;	buf.A.Y = 0.44709336;	buf.A.Z = -0.89100652;	buf.B.X = -0.07883462;	buf.B.Y = 0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = 0;	buf.C.Y = 0.30901699;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = -0.07883462;	buf.A.Y = 0.44709336;	buf.A.Z = -0.89100652;	buf.B.X = -0.1545085;	buf.B.Y = 0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = 0;	buf.C.Y = 0.30901699;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 2;	out.push_back (buf);
	buf.A.X = 0.22699525;	buf.A.Y = 0.39316731;	buf.A.Z = -0.89100652;	buf.B.X = 0.07883462;	buf.B.Y = 0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = 0.1545085;	buf.C.Y = 0.26761657;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.1545085;	buf.A.Y = 0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = 0.07883462;	buf.B.Y = 0.44709336;	buf.B.Z = -0.89100652;	buf.C.X = 0;	buf.C.Y = 0.30901699;	buf.C.Z = -0.95105652;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 0.30901699;	buf.A.Z = -0.95105652;	buf.B.X = -0.1545085;	buf.B.Y = 0.26761657;	buf.B.Z = -0.95105652;	buf.C.X = -0.07821723;	buf.C.Y = 0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 1;	out.push_back (buf);
	buf.A.X = 0.1545085;	buf.A.Y = 0.26761657;	buf.A.Z = -0.95105652;	buf.B.X = 0;	buf.B.Y = 0.30901699;	buf.B.Z = -0.95105652;	buf.C.X = 0.07821723;	buf.C.Y = 0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 3;	out.push_back (buf);
	buf.A.X = 0.07821723;	buf.A.Y = 0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = 0;	buf.B.Y = 0.30901699;	buf.B.Z = -0.95105652;	buf.C.X = -0.07821723;	buf.C.Y = 0.13547622;	buf.C.Z = -0.98768834;	buf.COUNT = 0;	buf.GROUP = 4;	out.push_back (buf);
	buf.A.X = 0.07821723;	buf.A.Y = 0.13547622;	buf.A.Z = -0.98768834;	buf.B.X = -0.07821723;	buf.B.Y = 0.13547622;	buf.B.Z = -0.98768834;	buf.C.X = 0;	buf.C.Y = 0;	buf.C.Z = 1;	buf.COUNT = 0;	buf.GROUP = 5;	out.push_back (buf);
	buf.A.X = -0.5;	buf.A.Y = 0.8660254;	buf.A.Z = 0;	buf.B.X = -0.40673664;	buf.B.Y = 0.91354546;	buf.B.Z = 0;	buf.C.X = -0.49099789;	buf.C.Y = 0.96363762;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.40673664;	buf.A.Y = 0.91354546;	buf.A.Z = 0;	buf.B.X = -0.30901699;	buf.B.Y = 0.95105652;	buf.B.Z = 0;	buf.C.X = -0.38758059;	buf.C.Y = 1.00968197;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.30901699;	buf.A.Y = 0.95105652;	buf.A.Z = 0;	buf.B.X = -0.20791169;	buf.B.Y = 0.9781476;	buf.B.Z = 0;	buf.C.X = -0.27991688;	buf.C.Y = 1.04466403;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.20791169;	buf.A.Y = 0.9781476;	buf.A.Z = 0;	buf.B.X = -0.10452846;	buf.B.Y = 0.9945219;	buf.B.Z = 0;	buf.C.X = -0.16918634;	buf.C.Y = 1.06820053;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = -0.10452846;	buf.A.Y = 0.9945219;	buf.A.Z = 0;	buf.B.X = 0;	buf.B.Y = 1;	buf.B.Z = 0;	buf.C.X = -0.05660216;	buf.C.Y = 1.08003361;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0;	buf.A.Y = 1;	buf.A.Z = 0;	buf.B.X = 0.10452846;	buf.B.Y = 0.9945219;	buf.B.Z = 0;	buf.C.X = 0.05660216;	buf.C.Y = 1.08003361;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.10452846;	buf.A.Y = 0.9945219;	buf.A.Z = 0;	buf.B.X = 0.20791169;	buf.B.Y = 0.9781476;	buf.B.Z = 0;	buf.C.X = 0.16918634;	buf.C.Y = 1.06820053;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.20791169;	buf.A.Y = 0.9781476;	buf.A.Z = 0;	buf.B.X = 0.30901699;	buf.B.Y = 0.95105652;	buf.B.Z = 0;	buf.C.X = 0.27991688;	buf.C.Y = 1.04466403;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.30901699;	buf.A.Y = 0.95105652;	buf.A.Z = 0;	buf.B.X = 0.40673664;	buf.B.Y = 0.91354546;	buf.B.Z = 0;	buf.C.X = 0.38758059;	buf.C.Y = 1.00968197;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 6;	out.push_back (buf);
	buf.A.X = 0.40673664;	buf.A.Y = 0.91354546;	buf.A.Z = 0;	buf.B.X = 0.5;	buf.B.Y = 0.8660254;	buf.B.Z = 0;	buf.C.X = 0.49099789;	buf.C.Y = 0.96363762;	buf.C.Z = 999.99;	buf.COUNT = 0;	buf.GROUP = 7;	out.push_back (buf);

	return out;
}

void debug_RECT_GRID (const vector <GDB>& inGDB, const vector < vector <GRID_CENTER> >& IN) {

	if (is_CHK_CONTOURING() && is_mode_DEBUG()) {

		vector <GDB> temp = sort_by_iID (inGDB);

		if (temp.at(0).ID == "BZ__0066") {

			vector < vector <GRID_CENTER> > ST = return_standard_RECT_GRID ();
			check_standard_RECT_GRID (IN, ST);
			exit (66);
		}
		else cout << "To test RECT_GRID of contouring call '--debug standard_output16'." << endl;
	}
	return;
}

void check_standard_RECT_GRID (const vector < vector <GRID_CENTER> >& IN, const vector < vector <GRID_CENTER> >& ST) {

	cout << fixed << setprecision (8) << endl;

	cout << "**** RECT_GRID CHECK FOR CONTOURING: IF FAILS LISTED BELOW:" << endl;

	for (size_t i = 0; i < IN.size(); i++) {
		for (size_t j = 0; j < IN.at(i).size(); j++) {

			const GRID_CENTER I = IN.at(i).at(j);
			const GRID_CENTER S = ST.at(i).at(j);

			if (! is_in_range (S.CENTER.X, S.CENTER.X, I.CENTER.X)) cout << "RECT_GRID X test failed: " << I.CENTER.X << " <> " << S.CENTER.X << endl;
			if (! is_in_range (S.CENTER.Y, S.CENTER.Y, I.CENTER.Y)) cout << "RECT_GRID Y test failed: " << I.CENTER.Y << " <> " << S.CENTER.Y << endl;
			if (! is_in_range (S.COUNT, S.COUNT, I.COUNT)) cout << "RECT_GRID COUNT test failed: " << I.COUNT << " <> " << S.COUNT << endl;
		}
	}
	cout << "**** END RECT_GRID CHECK FOR CONTOURING" << endl;
}

vector < vector <GRID_CENTER> > return_standard_RECT_GRID () {

	vector < vector <GRID_CENTER> > OUT;

	vector <GRID_CENTER> buf2;

	GRID_CENTER buf;

	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -1.0137931;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 176; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 166; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 160; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 159; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 162; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 171; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 186; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 215; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 269; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.94137931;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 248; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 202; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 175; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 160; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 150; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 147; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 155; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 169; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 192; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 232; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 316; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.86896552;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 402; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 436; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 280; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 208; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 175; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 156; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 144; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 137; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 137; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 159; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 181; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 218; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 287; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 476; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.79655172;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 445; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 464; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 317; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 246; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 200; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 171; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 152; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 139; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 131; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 126; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 124; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 126; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 130; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 140; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 154; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 178; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 214; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 280; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 449; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.72413793;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 579; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 558; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 395; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 283; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 227; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 190; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 165; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 147; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 135; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 126; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 120; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 117; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 117; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 120; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 127; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 138; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 155; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 181; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 220; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 289; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 472; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.65172414;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 618; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 506; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 412; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 314; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 251; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 209; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 179; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 158; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 142; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 130; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 121; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 115; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 111; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 111; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 113; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 118; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 127; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 141; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 161; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 189; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 233; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 313; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 554; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.57931034;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 577; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 392; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 315; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 263; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 223; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 192; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 168; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 150; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 136; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 125; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 116; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 110; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 105; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 107; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 111; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 119; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 132; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 148; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 171; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 204; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 256; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 359; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.50689655;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 400; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 395; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 314; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 265; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 228; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 199; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 176; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 157; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 141; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 129; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 119; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 112; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 102; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 100; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 102; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 114; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 125; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 140; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 160; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 187; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 227; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 294; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 445; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.43448276;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 403; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 324; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 266; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 230; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 202; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 180; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 161; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 146; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 133; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 123; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 114; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 107; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 101; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 98; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 96; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 98; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 102; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 110; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 121; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 152; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 176; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 210; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 262; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 359; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.36206897;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 451; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 371; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 272; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 231; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 204; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 182; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 165; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 149; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 136; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 125; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 116; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 108; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 102; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 97; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 94; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 92; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 94; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 99; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 107; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 118; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 131; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 148; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 170; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 200; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 245; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 321; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 488; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.28965517;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 394; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 286; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 235; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 205; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 183; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 166; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 152; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 140; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 128; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 119; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 110; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 103; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 98; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 93; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 90; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 89; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 92; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 98; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 116; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 129; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 166; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 194; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 236; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 302; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 433; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.21724138;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 330; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 243; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 204; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 183; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 167; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 154; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 142; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 132; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 122; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 113; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 99; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 93; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 89; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 86; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 87; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 91; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 97; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 116; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 128; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 144; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 165; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 192; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 232; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 295; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 413; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.14482759;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 221; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 200; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 179; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 165; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 153; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 143; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 125; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 117; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 109; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 102; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 96; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 90; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 85; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 83; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 85; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 91; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 98; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 117; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 129; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 165; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 193; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 233; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 296; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 412; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = -0.07241379;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0;	buf.COUNT = 180; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0;	buf.COUNT = 171; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0;	buf.COUNT = 159; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0;	buf.COUNT = 150; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0;	buf.COUNT = 142; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0;	buf.COUNT = 127; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0;	buf.COUNT = 119; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0;	buf.COUNT = 112; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0;	buf.COUNT = 99; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0;	buf.COUNT = 93; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0;	buf.COUNT = 88; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0;	buf.COUNT = 83; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0;	buf.COUNT = 81; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0;	buf.COUNT = 86; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0;	buf.COUNT = 92; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0;	buf.COUNT = 99; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0;	buf.COUNT = 108; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0;	buf.COUNT = 118; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0;	buf.COUNT = 131; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0;	buf.COUNT = 147; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0;	buf.COUNT = 168; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0;	buf.COUNT = 197; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0;	buf.COUNT = 238; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0;	buf.COUNT = 303; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0;	buf.COUNT = 427; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 148; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 149; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 140; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 128; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 121; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 115; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 109; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 103; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 97; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 91; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 86; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 82; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 84; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 88; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 94; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 102; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 110; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 121; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 151; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 173; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 204; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 248; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 319; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 459; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.07241379;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 137; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 139; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 137; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 133; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 129; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 123; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 118; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 112; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 100; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 95; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 90; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 86; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 85; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 87; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 91; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 97; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 105; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 114; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 125; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 139; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 157; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 181; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 214; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 262; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 344; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 516; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.14482759;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 135; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 137; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 135; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 131; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 127; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 122; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 116; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 110; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 105; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 99; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 94; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 91; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 89; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 90; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 92; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 95; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 101; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 109; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 118; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 130; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 146; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 165; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 191; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 228; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 284; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 382; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 615; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.21724138;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 148; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 149; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 138; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 129; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 123; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 117; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 111; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 100; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 97; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 94; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 94; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 95; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 97; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 101; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 114; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 124; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 138; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 154; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 176; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 205; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 248; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 316; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 444; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 811; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.28965517;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 151; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 146; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 142; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 137; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 130; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 122; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 115; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 109; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 105; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 101; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 99; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 99; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 101; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 103; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 107; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 113; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 121; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 132; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 147; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 165; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 190; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 225; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 277; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 364; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 548; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.36206897;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 163; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 166; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 161; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 152; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 142; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 132; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 123; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 116; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 111; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 107; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 106; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 107; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 110; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 115; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 121; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 130; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 142; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 159; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 180; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 210; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 252; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 319; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 442; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 757; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.43448276;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 204; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 198; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 180; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 161; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 133; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 124; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 118; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 114; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 113; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 113; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 115; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 118; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 123; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 131; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 141; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 155; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 174; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 200; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 237; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 292; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 386; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 585; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.50689655;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 279; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 304; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 231; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 187; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 161; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 127; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 123; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 122; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 122; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 124; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 128; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 134; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 143; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 155; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 172; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 196; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 229; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 277; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 356; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 506; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 940; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.57931034;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 525; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 298; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 221; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 181; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 158; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 137; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 133; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 131; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 132; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 135; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 141; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 148; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 159; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 174; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 196; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 226; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 271; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 341; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 470; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 790; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.65172414;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 280; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 263; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 206; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 173; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 156; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 147; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 144; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 143; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 145; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 149; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 156; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 166; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 181; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 201; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 230; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 273; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 340; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 461; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 747; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.72413793;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 297; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 239; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 184; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 167; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 159; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 157; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 158; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 161; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 168; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 178; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 192; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 212; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 240; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 284; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 353; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 478; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 790; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.79655172;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 190; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 179; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 175; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 174; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 177; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 184; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 194; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 209; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 230; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 261; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 308; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 386; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 536; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.86896552;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 196; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 199; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 206; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 218; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 235; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 259; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 296; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 354; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 459; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 0.94137931;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();
	buf.CENTER.X = -1.0137931;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.94137931;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.86896552;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.79655172;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.72413793;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.65172414;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.57931034;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.50689655;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.43448276;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.36206897;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.28965517;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.21724138;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.14482759;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = -0.07241379;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.07241379;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.14482759;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.21724138;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.28965517;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.36206897;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.43448276;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.50689655;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.57931034;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.65172414;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.72413793;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.79655172;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.86896552;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 0.94137931;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0; buf2.push_back (buf);	buf.CENTER.X = 1.0137931;	buf.CENTER.Y = 1.0137931;	buf.COUNT = 0;buf2.push_back (buf);OUT.push_back (buf2);  buf2.clear ();

	return OUT;
}

void check_standard_output_striae (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB) {

	cout << fixed << setprecision(8) << endl;

	for (size_t i = 0; i < nGDB.size(); i++) {

		if (nGDB.at(0).ID == "ANG_001") {

			cout << "**** STRIAE TEST OF RECORD 'ANG_001': IF FAILS LISTED BELOW:" << endl;

			compare_GDB (nGDB.at(i), ST_nGDB.at(0));

			cout << "**** END STRIAE TEST OF RECORD 'ANG_001'" << endl;

			exit (100);

			return;
		}
	}
	cout << "Test record 'ANG_001' not found - incorrect test file." << endl;
	ASSERT_DEAD_END();

	return;
}

vector <GDB> init_standard_GDB_striae () {

	vector <GDB> OUT;
	GDB buf;

	buf.ID = "ANG_001";
	buf.iID = 0;
	buf.N.X = 0.42402405;		buf.N.Y = -0.76495963;		buf.N.Z =  0.48480962;
	buf.D.X = 0.23504037;		buf.D.Y = -0.42402405;		buf.D.Z = -0.87461971;
	buf.S.X = 0.87461971;		buf.S.Y =  0.48480962;		buf.S.Z =  0.00000000;

	buf.NC.X = 0.64992262;		buf.NC.Y = -0.56523789;		buf.NC.Z = 0.50804205;
	buf.DC.X = 0.38334569;		buf.DC.Y = -0.33339586;		buf.DC.Z = -0.86133227;
	buf.SC.X = 0.65623675;		buf.SC.Y = 0.75455505;		buf.SC.Z = 0;
	buf.SV.X = 0.38334569;		buf.SV.Y = -0.33339586;		buf.SV.Z = -0.86133227;

	buf.LPITCH = 80;
	buf.LPITCHSENSE = "E";
	buf.PITCHANGLE = 80;
	buf.MISFIT = NaN();
	buf.LINEATION = "PITCH";
	buf.UPWARD = 0;
	buf.OFFSET = "NORMAL";
	buf.UP = 0;
	buf.GC = "XXA";
	buf.COLOR = "";
	buf.LOC = "ANG002";
	buf.LOCX = 0;
	buf.LOCY = 0;
	buf.FORMATION = "";
	buf.DATATYPE = "STRIAE";
	buf.DIPDIR = 151;					buf.DIP = 61;
	buf.LDIR = NaN();					buf.LDIP = NaN();
	buf.corr.DIPDIR = 151;				buf.corr.DIP = 61;
	buf.corrL.DIPDIR = 131.01349355;	buf.corrL.DIP = 59.46650063;
	buf.PALEON = 30;

	buf.COMMENT = "";
	buf.DATAGROUP = "STRIAE";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";

	buf.ptnP.X = NaN();		buf.ptnP.Y = NaN();		buf.ptnP.Z = NaN();
	buf.ptnT.X = NaN();		buf.ptnT.Y = NaN();		buf.ptnT.Z = NaN();
	buf.ptnN.X = NaN();		buf.ptnN.Y = NaN();		buf.ptnN.Z = NaN();
	buf.ptnPd.DIPDIR = NaN();			buf.ptnPd.DIP = NaN();
	buf.ptnTd.DIPDIR = NaN();			buf.ptnTd.DIP = NaN();
	buf.ptnNd.DIPDIR = NaN();			buf.ptnNd.DIP = NaN();

	buf.avD.X = NaN();		buf.avD.Y = NaN();		buf.avD.Z = NaN();
	buf.avS0D.X = 0.13399615;		buf.avS0D.Y = 0.1104579;		buf.avS0D.Z = -0.98480662;
	buf.avS0N.X = 0.759901;			buf.avS0N.Y = 0.62641404;		buf.avS0N.Z = 0.17365459;

	//buf.rotavD.X = NaN();			buf.rotavD.Y = NaN();		buf.rotavD.Z = NaN();
	buf.avS0d.DIPDIR = 50.5;			buf.avS0d.DIP = 79.99962691;
	buf.avd.DIPDIR = NaN();				buf.avd.DIP = NaN();
	buf.avS0offset = "";
	buf.SHEAR_S.X = 2.24534933;		buf.SHEAR_S.Y = -6.88335744;	buf.SHEAR_S.Z = -12.82477169;
	buf.NORMAL_S.X = -4.09754565;	buf.NORMAL_S.Y = 7.39216802;	buf.NORMAL_S.Z = -4.68494548;

	buf.UPSILON.X = -1.91336222;	buf.UPSILON.Y = 6.59462816;		buf.UPSILON.Z = 12.07883606;

	buf.lambda = 0.8660254;
	buf.ANG = 15.35044481;
	buf.RUP = 22291.30407468;

	OUT.push_back(buf);

	return OUT;
}

void check_standard_output_fracture (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB) {

	cout << fixed << setprecision(8) << endl;

	for (size_t i = 0; i < nGDB.size(); i++) {

		if (nGDB.at(i).ID == "BZ0082") {

			cout << "**** FRACTURE TEST OF RECORD 'BZ0082': IF FAILS LISTED BELOW:" << endl;

			compare_GDB (nGDB.at(i), ST_nGDB.at(0));

			cout << "**** END OF FRACTURE TEST OF RECORD 'BZ0082'" << endl;

			exit (100);

			return;
		}
	}
	cout << "Test record 'BZ0082' not found - incorrect test file." << endl;
	ASSERT_DEAD_END();
}

vector <GDB> init_standard_GDB_fracture () {

	vector <GDB> OUT;
	GDB buf;

	buf.ID = "BZ0082";
	buf.iID = 13;
	buf.N.X = 0.83516434;		buf.N.Y = -0.5218686;		buf.N.Z = 0.17364818;
	buf.D.X = 0.14726201;		buf.D.Y = -0.09201951;		buf.D.Z = -0.98480775;
	buf.S.X = 0.52991926;		buf.S.Y = 0.8480481;		buf.S.Z = 0;

	buf.NC.X = NaN();		buf.NC.Y = NaN();		buf.NC.Z = NaN();
	buf.DC.X = NaN();		buf.DC.Y = NaN();		buf.DC.Z = NaN();
	buf.SC.X = NaN();		buf.SC.Y = NaN();		buf.SC.Z = NaN();
	buf.SV.X = NaN();		buf.SV.Y = NaN();		buf.SV.Z = NaN();

	buf.LPITCH = NaN();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN();
	buf.MISFIT = NaN();
	buf.LINEATION = "NONE";
	buf.UPWARD = 0;
	buf.OFFSET = "NONE";
	buf.UP = 0;
	buf.GC = "ABX";
	buf.COLOR = "B";
	buf.LOC = "230";
	buf.LOCX = 0;
	buf.LOCY = 0;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "FRACTURE";
	buf.DIPDIR = 122;			buf.DIP = 80;
	buf.LDIR = NaN();			buf.LDIP = NaN();
	buf.corr.DIPDIR = 122;		buf.corr.DIP = 80;
	buf.corrL.DIPDIR = NaN();	buf.corrL.DIP = NaN();

	buf.PALEON = 0;
	buf.COMMENT = "";
	buf.DATAGROUP = "PLANE";
	buf.PSCOLOR = "0.00 0.00 1.00";
	buf.DASHED = "   ";

	buf.ptnP.X = NaN();		buf.ptnP.Y = NaN();		buf.ptnP.Z = NaN();
	buf.ptnT.X = NaN();		buf.ptnT.Y = NaN();		buf.ptnT.Z = NaN();
	buf.ptnN.X = NaN();		buf.ptnN.Y = NaN();		buf.ptnN.Z = NaN();
	buf.ptnPd.DIPDIR = NaN();		buf.ptnPd.DIP = NaN();
	buf.ptnTd.DIPDIR = NaN();		buf.ptnTd.DIP = NaN();
	buf.ptnNd.DIPDIR = NaN();		buf.ptnNd.DIP = NaN();

	buf.avD.X = -0.20643402;		buf.avD.Y = -0.07877234;		buf.avD.Z = 0.97528453;
	buf.avS0D.X = 0.25287578;		buf.avS0D.Y = 0.4379937;		buf.avS0D.Z = -0.86267918;
	buf.avS0N.X = 0.43133959;		buf.avS0N.Y = 0.74710208;		buf.avS0N.Z = 0.50575156;
	buf.avS0d.DIPDIR = 30;					buf.avS0d.DIP = 59.61874486;
	buf.avd.DIPDIR = 249.11384473;			buf.avd.DIP = 77.23500525;
	buf.avS0offset = "";

	buf.SHEAR_S.X = NaN();		buf.SHEAR_S.Y = NaN();		buf.SHEAR_S.Z = NaN();
	buf.NORMAL_S.X = NaN();		buf.NORMAL_S.Y = NaN();		buf.NORMAL_S.Z = NaN();
	buf.UPSILON.X = NaN();		buf.UPSILON.Y = NaN();		buf.UPSILON.Z = NaN();
	buf.lambda = NaN();
	buf.ANG = NaN();
	buf.RUP = NaN();

	OUT.push_back(buf);

	return OUT;
}

void check_standard_output_lithology (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB) {

	cout << fixed << setprecision(8) << endl;

	for (size_t i = 0; i < nGDB.size(); i++) {

		if (nGDB.at(0).ID == "BZ0066") {

			cout << "**** LITHOLOGY TEST OF RECORD 'BZ0066': IF FAILS LISTED BELOW:" << endl;

			compare_GDB (nGDB.at(i), ST_nGDB.at(0));

			cout << "**** END LITHOLOGY TEST OF RECORD 'BZ0066'" << endl;

			exit (100);

			return;
		}
	}
	cout << "Test record 'BZ0066' not found - incorrect test file." << endl;
	ASSERT_DEAD_END();
}

vector <GDB> init_standard_GDB_lithology () {

	vector <GDB> OUT;
	GDB buf;

	buf.ID = "BZ0066";
	buf.iID = 0;
	buf.N.X = NaN();
	buf.N.Y = NaN();
	buf.N.Z = NaN();
	buf.D.X = NaN();
	buf.D.Y = NaN();
	buf.D.Z = NaN();
	buf.S.X = NaN();
	buf.S.Y = NaN();
	buf.S.Z = NaN();
	buf.NC.X = NaN();
	buf.NC.Y = NaN();
	buf.NC.Z = NaN();
	buf.DC.X = NaN();
	buf.DC.Y = NaN();
	buf.DC.Z = NaN();
	buf.SC.X = NaN();
	buf.SC.Y = NaN();
	buf.SC.Z = NaN();
	buf.SV.X = NaN();
	buf.SV.Y = NaN();
	buf.SV.Z = NaN();
	buf.LPITCH = NaN();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN();
	buf.MISFIT = NaN();
	buf.LINEATION = "NONE";
	buf.UPWARD = 0;
	buf.OFFSET = "NONE";
	buf.UP = 0;
	buf.GC = "AXX";
	buf.COLOR = "A";
	buf.LOC = "230";
	buf.LOCX = 0;
	buf.LOCY = 0;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "LITHOLOGY";
	buf.DIPDIR = NaN();
	buf.DIP = NaN();
	buf.LDIR = NaN();
	buf.LDIP = NaN();
	buf.corr.DIPDIR = NaN();
	buf.corr.DIP = NaN();
	buf.corrL.DIPDIR = NaN();
	buf.corrL.DIP = NaN();
	buf.PALEON = 0;
	buf.COMMENT = "";
	buf.DATAGROUP = "LITHOLOGY";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";
	buf.ptnP.X = NaN();
	buf.ptnP.Z = NaN();
	buf.ptnT.X = NaN();
	buf.ptnT.Y = NaN();
	buf.ptnT.Z = NaN();
	buf.ptnN.X = NaN();
	buf.ptnN.Y = NaN();
	buf.ptnN.Z = NaN();
	buf.ptnPd.DIPDIR = NaN();
	buf.ptnPd.DIP = NaN();
	buf.ptnTd.DIPDIR = NaN();
	buf.ptnTd.DIP = NaN();
	buf.ptnNd.DIPDIR = NaN();
	buf.ptnNd.DIP = NaN();
	buf.avD.X = NaN();
	buf.avD.Y = NaN();
	buf.avD.Z = NaN();
	buf.avS0D.X = NaN();
	buf.avS0D.Y = NaN();
	buf.avS0D.Z = NaN();
	buf.avS0N.X = NaN();
	buf.avS0N.Y = NaN();
	buf.avS0N.Z = NaN();
	buf.avS0d.DIPDIR = NaN();
	buf.avS0d.DIP = NaN();
	buf.avd.DIPDIR = NaN();
	buf.avd.DIP = NaN();
	buf.avS0offset = "";
	buf.SHEAR_S.X = NaN();
	buf.SHEAR_S.Y = NaN();
	buf.SHEAR_S.Z = NaN();
	buf.NORMAL_S.X = NaN();
	buf.NORMAL_S.Y = NaN();
	buf.NORMAL_S.Z = NaN();
	buf.UPSILON.X = NaN();
	buf.UPSILON.Y = NaN();
	buf.UPSILON.Z = NaN();
	buf.lambda = NaN();
	buf.ANG = NaN();
	buf.RUP = NaN();

	OUT.push_back(buf);

	return OUT;
}

void check_standard_output_SC (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB) {

	cout << fixed << setprecision(8) << endl;

	for (size_t i = 0; i < nGDB.size(); i++) {

		if (nGDB.at(i).ID == "BZ00_71") {

			cout << "**** SC TEST OF RECORD 'BZ00_71': IF FAILS LISTED BELOW:" << endl;

			compare_GDB (nGDB.at(i), ST_nGDB.at(0));

			cout << "**** END SC TEST OF RECORD 'BZ00_71'" << endl;

			exit (100);

			return;
		}
	}
	cout << "Test record 'BZ00_71' not found - incorrect test file." << endl;
	ASSERT_DEAD_END();
}

vector <GDB> init_standard_GDB_SC () {

	vector <GDB> OUT;
	GDB buf;

	buf.ID = "BZ00_71";
	buf.iID = 0;
	buf.N.X = 0.52339508;
	buf.N.Y = 0.83760722;
	buf.N.Z = 0.15643447;
	buf.D.X = 0.08289764;
	buf.D.Y = 0.13266395;
	buf.D.Z = -0.98768834;
	buf.S.X = -0.8480481;
	buf.S.Y = 0.52991926;
	buf.S.Z = 0;
	buf.NC.X = 0.61239937;
	buf.NC.Y = 0.47845883;
	buf.NC.Z = 0.62932039;
	buf.DC.X = 0.49591124;
	buf.DC.Y = 0.38744832;
	buf.DC.Z = -0.77714596;
	buf.SC.X = -0.61566148;
	buf.SC.Y = 0.78801075;
	buf.SC.Z = 0;
	buf.SV.X = NaN();
	buf.SV.Y = NaN();
	buf.SV.Z = NaN();
	buf.LPITCH = NaN();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN();
	buf.MISFIT = NaN();
	buf.LINEATION = "SC";
	buf.UPWARD = 0;
	buf.OFFSET = "NONE";
	buf.UP = 0;
	buf.GC = "XXX";
	buf.COLOR = "";
	buf.LOC = "230";
	buf.LOCX = 0;
	buf.LOCY = 0;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "SC";
	buf.DIPDIR = 32;
	buf.DIP = 81;
	buf.LDIR = 52;
	buf.LDIP = 51;
	buf.corr.DIPDIR = 32;
	buf.corr.DIP = 81;
	buf.corrL.DIPDIR = 52;
	buf.corrL.DIP = 51;
	buf.PALEON = 0;
	buf.COMMENT = "ALL_FRACTURE_ORIENTATION";
	buf.DATAGROUP = "SC";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";
	buf.ptnP.X = NaN();
	buf.ptnP.Y = NaN();
	buf.ptnP.Z = NaN();
	buf.ptnT.X = NaN();
	buf.ptnT.Y = NaN();
	buf.ptnT.Z = NaN();
	buf.ptnN.X = NaN();
	buf.ptnN.Y = NaN();
	buf.ptnN.Z = NaN();
	buf.ptnPd.DIPDIR = NaN();
	buf.ptnPd.DIP = NaN();
	buf.ptnTd.DIPDIR = NaN();
	buf.ptnTd.DIP = NaN();
	buf.ptnNd.DIPDIR = NaN();
	buf.ptnNd.DIP = NaN();
	buf.avD.X = NaN();
	buf.avD.Y = NaN();
	buf.avD.Z = NaN();
	buf.avS0D.X = 0.25287578;
	buf.avS0D.Y = 0.4379937;
	buf.avS0D.Z = -0.86267918;
	buf.avS0N.X = 0.43133959;
	buf.avS0N.Y = 0.74710208;
	buf.avS0N.Z = 0.50575156;
	buf.avS0d.DIPDIR = 30;
	buf.avS0d.DIP = 59.61874486;
	buf.avd.DIPDIR = NaN();
	buf.avd.DIP = NaN();
	buf.avS0offset = "";
	buf.SHEAR_S.X = NaN();
	buf.SHEAR_S.Y = NaN();
	buf.SHEAR_S.Z = NaN();
	buf.NORMAL_S.X = NaN();
	buf.NORMAL_S.Y = NaN();
	buf.NORMAL_S.Z = NaN();
	buf.UPSILON.X = NaN();
	buf.UPSILON.Y = NaN();
	buf.UPSILON.Z = NaN();
	buf.lambda = NaN();
	buf.ANG = NaN();
	buf.RUP = NaN();

	OUT.push_back(buf);

	return OUT;
}

void check_standard_output_lineation (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB) {

	cout << fixed << setprecision(8) << endl;

	for (size_t i = 0; i < nGDB.size(); i++) {

		if (nGDB.at(i).ID == "BZ0083") {

			cout << "**** LINEATION TEST OF RECORD 'BZ0083': IF FAILS LISTED BELOW:" << endl;

			compare_GDB (nGDB.at(i), ST_nGDB.at(0));

			cout << "**** END LINEATION TEST OF RECORD 'BZ0083'" << endl;

			exit (100);

			return;
		}
	}
	cout << "Test record 'BZ0083' not found - incorrect test file." << endl;
	ASSERT_DEAD_END();
}

vector <GDB> init_standard_GDB_lineation () {

	vector <GDB> OUT;
	GDB buf;

	buf.ID = "BZ0083";
	buf.iID = 0;
	buf.N.X = 0.95677273;
	buf.N.Y = 0.20336832;
	buf.N.Z = 0.20791169;
	buf.D.X = 0.20336832;
	buf.D.Y = 0.04322727;
	buf.D.Z = -0.9781476;
	buf.S.X = -0.20791169;
	buf.S.Y = 0.9781476;
	buf.S.Z = 0;
	buf.NC.X = NaN();
	buf.NC.Y = NaN();
	buf.NC.Z = NaN();
	buf.DC.X = NaN();
	buf.DC.Y = NaN();
	buf.DC.Z = NaN();
	buf.SC.X = NaN();
	buf.SC.Y = NaN();
	buf.SC.Z = NaN();
	buf.SV.X = NaN();
	buf.SV.Y = NaN();
	buf.SV.Z = NaN();
	buf.LPITCH = NaN();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN();
	buf.MISFIT = NaN();
	buf.LINEATION = "NONE";
	buf.UPWARD = 0;
	buf.OFFSET = "NONE";
	buf.UP = 0;
	buf.GC = "XXX";
	buf.COLOR = "";
	buf.LOC = "230";
	buf.LOCX = 0;
	buf.LOCY = 0;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "LINEATION";
	buf.DIPDIR = 78;
	buf.DIP = 78;
	buf.LDIR = NaN();
	buf.LDIP = NaN();
	buf.corr.DIPDIR = 78;
	buf.corr.DIP = 78;
	buf.corrL.DIPDIR = NaN();
	buf.corrL.DIP = NaN();
	buf.PALEON = 0;
	buf.COMMENT = "ALL_LINEATION_ORIENTATION";
	buf.DATAGROUP = "LINEATION";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";
	buf.ptnP.X = NaN();
	buf.ptnP.Y = NaN();
	buf.ptnP.Z = NaN();
	buf.ptnT.X = NaN();
	buf.ptnT.Y = NaN();
	buf.ptnT.Z = NaN();
	buf.ptnN.Y = NaN();
	buf.ptnN.Z = NaN();
	buf.ptnPd.DIPDIR = NaN();
	buf.ptnPd.DIP = NaN();
	buf.ptnTd.DIPDIR = NaN();
	buf.ptnTd.DIP = NaN();
	buf.ptnNd.DIPDIR = NaN();
	buf.ptnNd.DIP = NaN();
	buf.avD.X = -0.20799914;
	buf.avD.Y = -0.05074382;
	buf.avD.Z = 0.97681187;
	buf.avS0D.X = 0.25287578;
	buf.avS0D.Y = 0.4379937;
	buf.avS0D.Z = -0.86267918;
	buf.avS0N.X = 0.43133959;
	buf.avS0N.Y = 0.74710208;
	buf.avS0N.Z = 0.50575156;
	buf.avS0d.DIPDIR = 30;
	buf.avS0d.DIP = 59.61874486;
	buf.avd.DIPDIR = 256.28983555;
	buf.avd.DIP = 77.63730047;
	buf.avS0offset = "";
	buf.SHEAR_S.X = NaN();
	buf.SHEAR_S.Y = NaN();
	buf.SHEAR_S.Z = NaN();
	buf.NORMAL_S.X = NaN();
	buf.NORMAL_S.Y = NaN();
	buf.NORMAL_S.Z = NaN();
	buf.UPSILON.X = NaN();
	buf.UPSILON.Y = NaN();
	buf.UPSILON.Z = NaN();
	buf.ANG = NaN();
	buf.RUP = NaN();

	OUT.push_back(buf);

	return OUT;
}

void check_standard_RETILT (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB) {

	cout << fixed << setprecision(8) << endl;

	const vector <GDB> S_GDB = sort_by_iID (nGDB);

	cout << "**** RETILT TEST: IF FAILS LISTED BELOW:" << endl;

	if (S_GDB.at(0).ID == "BZ0066") {

		for (size_t i = 0; i < 5; i++) {

			const GDB t = nGDB.at(i);
			const GDB S = ST_nGDB.at(i);

			if (t.ID != S.ID) 	cout << "ID test failed: " << t.ID << " <> " << S.ID << endl;
			if (t.iID != S.iID)	cout << "iID test failed: " << t.iID << " <> " << S.iID << endl;

			if (!isnan(t.N.X) && !isnan(S.N.X) && !is_in_range (t.N.X, t.N.X, S.N.X)) cout << "N.X test failed: " << t.N.X << " <> " << S.N.X << endl;
			if (!isnan(t.N.Y) && !isnan(S.N.Y) && !is_in_range (t.N.Y, t.N.Y, S.N.Y)) cout << "N.Y test failed: " << t.N.Y << " <> " << S.N.Y << endl;
			if (!isnan(t.N.Z) && !isnan(S.N.Z) && !is_in_range (t.N.Z, t.N.Z, S.N.Z)) cout << "N.Z test failed: " << t.N.Z << " <> " << S.N.Z << endl;

			if (!isnan(t.D.X) && !isnan(S.D.X) && !is_in_range (t.D.X, t.D.X, S.D.X)) cout << "D.X test failed: " << t.D.X << " <> " << S.D.X << endl;
			if (!isnan(t.D.Y) && !isnan(S.D.Y) && !is_in_range (t.D.Y, t.D.Y, S.D.Y)) cout << "D.Y test failed: " << t.D.Y << " <> " << S.D.Y << endl;
			if (!isnan(t.D.Z) && !isnan(S.D.Z) && !is_in_range (t.D.Z, t.D.Z, S.D.Z)) cout << "D.Z test failed: " << t.D.Z << " <> " << S.D.Z << endl;

			if (!isnan(t.S.X) && !isnan(S.S.X) && !is_in_range (t.S.X, t.S.X, S.S.X)) cout << "S.X test failed: " << t.S.X << " <> " << S.S.X << endl;
			if (!isnan(t.S.Y) && !isnan(S.S.Y) && !is_in_range (t.S.Y, t.S.Y, S.S.Y)) cout << "S.Y test failed: " << t.S.Y << " <> " << S.S.Y << endl;
			if (!isnan(t.S.Z) && !isnan(S.S.Z) && !is_in_range (t.S.Z, t.S.Z, S.S.Z)) cout << "S.Z test failed: " << t.S.Z << " <> " << S.S.Z << endl;

			if (!isnan(t.NC.X) && !isnan(S.NC.X) && !is_in_range (t.NC.X, t.NC.X, S.NC.X)) cout << "NC.X test failed: " << t.NC.X << " <> " << S.NC.X << endl;
			if (!isnan(t.NC.Y) && !isnan(S.NC.Y) && !is_in_range (t.NC.Y, t.NC.Y, S.NC.Y)) cout << "NC.Y test failed: " << t.NC.Y << " <> " << S.NC.Y << endl;
			if (!isnan(t.NC.Z) && !isnan(S.NC.Z) && !is_in_range (t.NC.Z, t.NC.Z, S.NC.Z)) cout << "NC.Z test failed: " << t.NC.Z << " <> " << S.NC.Z << endl;

			if (!isnan(t.DC.X) && !isnan(S.DC.X) && !is_in_range (t.DC.X, t.DC.X, S.DC.X)) cout << "DC.X test failed: " << t.DC.X << " <> " << S.DC.X << endl;
			if (!isnan(t.DC.Y) && !isnan(S.DC.Y) && !is_in_range (t.DC.Y, t.DC.Y, S.DC.Y)) cout << "DC.Y test failed: " << t.DC.Y << " <> " << S.DC.Y << endl;
			if (!isnan(t.DC.Z) && !isnan(S.DC.Z) && !is_in_range (t.DC.Z, t.DC.Z, S.DC.Z)) cout << "DC.Z test failed: " << t.DC.Z << " <> " << S.DC.Z << endl;

			if (!isnan(t.SC.X) && !isnan(S.SC.X) && !is_in_range (t.SC.X, t.SC.X, S.SC.X)) cout << "SC.X test failed: " << t.SC.X << " <> " << S.SC.X << endl;
			if (!isnan(t.SC.Y) && !isnan(S.SC.Y) && !is_in_range (t.SC.Y, t.SC.Y, S.SC.Y)) cout << "SC.Y test failed: " << t.SC.Y << " <> " << S.SC.Y << endl;
			if (!isnan(t.SC.Z) && !isnan(S.SC.Z) && !is_in_range (t.SC.Z, t.SC.Z, S.SC.Z)) cout << "SC.Z test failed: " << t.SC.Z << " <> " << S.SC.Z << endl;

			if (!isnan(t.SV.X) && !isnan(S.SV.X) && !is_in_range (t.SV.X, t.SV.X, S.SV.X)) cout << "SV.X test failed: " << t.SV.X << " <> " << S.SV.X << endl;
			if (!isnan(t.SV.Y) && !isnan(S.SV.Y) && !is_in_range (t.SV.Y, t.SV.Y, S.SV.Y)) cout << "SV.Y test failed: " << t.SV.Y << " <> " << S.SV.Y << endl;
			if (!isnan(t.SV.Z) && !isnan(S.SV.Z) && !is_in_range (t.SV.Z, t.SV.Z, S.SV.Z)) cout << "SV.Z test failed: " << t.SV.Z << " <> " << S.SV.Z << endl;

			if (!isnan(t.avS0D.X) && !isnan(S.avS0D.X) && !is_in_range (t.avS0D.X, t.avS0D.X, S.avS0D.X)) cout << "avS0D.X test failed: " << t.avS0D.X << " <> " << S.avS0D.X << endl;
			if (!isnan(t.avS0D.Y) && !isnan(S.avS0D.Y) && !is_in_range (t.avS0D.Y, t.avS0D.Y, S.avS0D.Y)) cout << "avS0D.Y test failed: " << t.avS0D.Y << " <> " << S.avS0D.Y << endl;
			if (!isnan(t.avS0D.Z) && !isnan(S.avS0D.Z) && !is_in_range (t.avS0D.Z, t.avS0D.Z, S.avS0D.Z)) cout << "avS0D.Z test failed: " << t.avS0D.Z << " <> " << S.avS0D.Z << endl;

			if (!isnan(t.avS0N.X) && !isnan(S.avS0N.X) && !is_in_range (t.avS0N.X, t.avS0N.X, S.avS0N.X)) cout << "avS0N.X test failed: " << t.avS0N.X << " <> " << S.avS0N.X << endl;
			if (!isnan(t.avS0N.Y) && !isnan(S.avS0N.Y) && !is_in_range (t.avS0N.Y, t.avS0N.Y, S.avS0N.Y)) cout << "avS0N.Y test failed: " << t.avS0N.Y << " <> " << S.avS0N.Y << endl;
			if (!isnan(t.avS0N.Z) && !isnan(S.avS0N.Z) && !is_in_range (t.avS0N.Z, t.avS0N.Z, S.avS0N.Z)) cout << "avS0N.Z test failed: " << t.avS0N.Z << " <> " << S.avS0N.Z << endl;
		}
		cout << "**** END RETILT TEST" << endl;

		exit (100);

		return;
	}
	cout << "Test record 'BZ0066' not found - incorrect test file." << endl;
	ASSERT_DEAD_END();
}

vector <GDB> init_standard_RETILT () {

	vector <GDB> OUT;
	GDB buf;

	buf.ID = "BZ0066";
	buf.iID = 0;
	buf.N.X = 0.79859272;
	buf.N.Y = 0.53110831;
	buf.N.Z = 0.28314948;
	buf.D.X = 0.23576981;
	buf.D.Y = 0.15679996;
	buf.D.Z = -0.95907579;
	buf.S.X = -0.55377095;
	buf.S.Y = 0.83266904;
	buf.S.Z = 0;
	buf.NC.X = NaN();
	buf.NC.Y = NaN();
	buf.NC.Z = NaN();
	buf.DC.X = NaN();
	buf.DC.Y = NaN();
	buf.DC.Z = NaN();
	buf.SC.X = NaN();
	buf.SC.Y = NaN();
	buf.SC.Z = NaN();
	buf.SV.X = NaN();
	buf.SV.Y = NaN();
	buf.SV.Z = NaN();
	buf.avS0D.X = -0.97482388;
	buf.avS0D.Y = -0.22297619;
	buf.avS0D.Z = 0.00016748;
	buf.avS0N.X = -0.00016327;
	buf.avS0N.Y = -0.00003734;
	buf.avS0N.Z = 0.99999999;
	OUT.push_back(buf);

	buf.ID = "BZ0124";
	buf.iID = 1;
	buf.N.X = -0.27814663;
	buf.N.Y = 0.41421699;
	buf.N.Z = 0.86663645;
	buf.D.X = -0.48312811;
	buf.D.Y = 0.71947618;
	buf.D.Z = -0.49894014;
	buf.S.X = -0.83019376;
	buf.S.Y = -0.55747495;
	buf.S.Z = 0;
	buf.NC.X = NaN();
	buf.NC.Y = NaN();
	buf.NC.Z = NaN();
	buf.DC.X = NaN();
	buf.DC.Y = NaN();
	buf.DC.Z = NaN();
	buf.SC.X = NaN();
	buf.SC.Y = NaN();
	buf.SC.Z = NaN();
	buf.SV.X = NaN();
	buf.SV.Y = NaN();
	buf.SV.Z = NaN();
	buf.avS0D.X = -0.97482388;
	buf.avS0D.Y = -0.22297619;
	buf.avS0D.Z = 0.00016748;
	buf.avS0N.X = -0.00016327;
	buf.avS0N.Y = -0.00003734;
	buf.avS0N.Z = 0.99999999;
	OUT.push_back(buf);

	buf.ID = "BZ0125";
	buf.iID = 2;
	buf.N.X = NaN();
	buf.N.Y = NaN();
	buf.N.Z = NaN();
	buf.D.X = NaN();
	buf.D.Y = NaN();
	buf.D.Z = NaN();
	buf.S.X = NaN();
	buf.S.Y = NaN();
	buf.S.Z = NaN();
	buf.NC.X = NaN();
	buf.NC.Y = NaN();
	buf.NC.Z = NaN();
	buf.DC.X = NaN();
	buf.DC.Y = NaN();
	buf.DC.Z = NaN();
	buf.SC.X = NaN();
	buf.SC.Y = NaN();
	buf.SC.Z = NaN();
	buf.SV.X = NaN();
	buf.SV.Y = NaN();
	buf.SV.Z = NaN();
	buf.avS0D.X = -0.97482388;
	buf.avS0D.Y = -0.22297619;
	buf.avS0D.Z = 0.00016748;
	buf.avS0N.X = -0.00016327;
	buf.avS0N.Y = -0.00003734;
	buf.avS0N.Z = 0.99999999;
	OUT.push_back(buf);

	buf.ID = "BZ0126";
	buf.iID = 3;
	buf.N.X = -0.43405399;
	buf.N.Y = 0.09401607;
	buf.N.Z = 0.89596769;
	buf.D.X = -0.87566205;
	buf.D.Y = 0.18966835;
	buf.D.Z = -0.44411923;
	buf.S.X = -0.21169106;
	buf.S.Y = -0.97733663;
	buf.S.Z = 0;
	buf.NC.X = -0.32277882;
	buf.NC.Y = 0.10794188;
	buf.NC.Z = 0.94029909;
	buf.DC.X = -0.89175641;
	buf.DC.Y = 0.29821619;
	buf.DC.Z = -0.34034925;
	buf.SC.X = -0.31715035;
	buf.SC.Y = -0.94837527;
	buf.SC.Z = 0;
	buf.SV.X = NaN();
	buf.SV.Y = NaN();
	buf.SV.Z = NaN();
	buf.avS0D.X = -0.97482388;
	buf.avS0D.Y = -0.22297619;
	buf.avS0D.Z = 0.00016748;
	buf.avS0N.X = -0.00016327;
	buf.avS0N.Y = -0.00003734;
	buf.avS0N.Z = 0.99999999;
	OUT.push_back(buf);

	buf.ID = "BZ0127";
	buf.iID = 4;
	buf.N.X = -0.02048813;
	buf.N.Y = 0.23706393;
	buf.N.Z = 0.97127799;
	buf.D.X = -0.08363047;
	buf.D.Y = 0.96767085;
	buf.D.Z = -0.23794762;
	buf.S.X = -0.99628619;
	buf.S.Y = -0.08610354;
	buf.S.Z = 0;
	buf.NC.X = -0.04674466;
	buf.NC.Y = 0.23185681;
	buf.NC.Z = 0.97162614;
	buf.DC.X = -0.19202585;
	buf.DC.Y = 0.95246178;
	buf.DC.Z = -0.23652197;
	buf.SC.X = -0.98027599;
	buf.SC.Y = -0.19763347;
	buf.SC.Z = 0;
	buf.SV.X = -0.19202585;
	buf.SV.Y = 0.95246178;
	buf.SV.Z = -0.23652197;
	buf.avS0D.X = -0.97482388;
	buf.avS0D.Y = -0.22297619;
	buf.avS0D.Z = 0.00016748;
	buf.avS0N.X = -0.00016327;
	buf.avS0N.Y = -0.00003734;
	buf.avS0N.Z = 0.99999999;
	OUT.push_back(buf);

	return OUT;
}

void check_standard_AVERAGE (const vector <GDB>& nGDB, const vector <GDB>& ST_nGDB) {

	cout << fixed << setprecision(8) << endl;

	cout << "**** AVERAGE TEST OF RECORD 'BZ6004': IF FAILS LISTED BELOW:" << endl;

	if (nGDB.at(0).ID == "BZ6004") {

		for (size_t i = 0; i < nGDB.size(); i++) {

			compare_GDB (nGDB.at(i), ST_nGDB.at(i));

		}
		cout << "**** END AVERAGE TEST OF RECORD 'BZ6004'" << endl;

		exit (100);

		return;
	}
	cout << "Test record 'BZ6004' not found - incorrect test file." << endl;
	ASSERT_DEAD_END();
}


vector <GDB> init_standard_AVERAGE () {

	vector <GDB> OUT;
	GDB buf;

	buf.ID = "BZ6004";
	buf.iID = 0;
	buf.N.X = 0.29004953;
	buf.N.Y = 0.79690454;
	buf.N.Z = 0.52991926;
	buf.D.X = 0.18124306;
	buf.D.Y = 0.49796122;
	buf.D.Z = -0.8480481;
	buf.S.X = -0.93969262;
	buf.S.Y = 0.34202014;
	buf.S.Z = 0;
	buf.NC.X = NaN ();
	buf.NC.Y = NaN ();
	buf.NC.Z = NaN ();
	buf.DC.X = NaN ();
	buf.DC.Y = NaN ();
	buf.DC.Z = NaN ();
	buf.SC.X = NaN ();
	buf.SC.Y = NaN ();
	buf.SC.Z = NaN ();
	buf.SV.X = NaN ();
	buf.SV.Y = NaN ();
	buf.SV.Z = NaN ();
	buf.LPITCH = NaN ();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN ();
	buf.MISFIT = NaN ();
	buf.LINEATION = "NONE";
	buf.UPWARD = 0;
	buf.OFFSET = "OVERTURNED";
	buf.UP = 0;
	buf.GC = "XXX";
	buf.COLOR = "";
	buf.LOC = "230";
	buf.LOCX = 488902;
	buf.LOCY = 3953801;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "BEDDING";
	buf.DIPDIR = 20;
	buf.DIP = 58;
	buf.LDIR = NaN ();
	buf.LDIP = NaN ();
	buf.corr.DIPDIR = 20;
	buf.corr.DIP = 58;
	buf.corrL.DIPDIR = NaN ();
	buf.corrL.DIP = NaN ();
	buf.PALEON = 0;
	buf.COMMENT = "BEDDING";
	buf.DATAGROUP = "PLANE";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";
	buf.ptnP.X = NaN ();
	buf.ptnP.Y = NaN ();
	buf.ptnP.Z = NaN ();
	buf.ptnT.X = NaN ();
	buf.ptnT.Y = NaN ();
	buf.ptnT.Z = NaN ();
	buf.ptnN.X = NaN ();
	buf.ptnN.Y = NaN ();
	buf.ptnN.Z = NaN ();
	buf.ptnPd.DIPDIR = NaN ();
	buf.ptnPd.DIP = NaN ();
	buf.ptnTd.DIPDIR = NaN ();
	buf.ptnTd.DIP = NaN ();
	buf.ptnNd.DIPDIR = NaN ();
	buf.ptnNd.DIP = NaN ();
	buf.avD.X = -0.24614085;
	buf.avD.Y = -0.42922122;
	buf.avD.Z = 0.86901314;
	buf.avS0D.X = -0.24614085;
	buf.avS0D.Y = -0.42922122;
	buf.avS0D.Z = 0.86901314;
	buf.avS0N.X = -0.43230473;
	buf.avS0N.Y = -0.7538544;
	buf.avS0N.Z = 0.49478901;
	buf.avS0d.DIPDIR = 29.83251102;
	buf.avS0d.DIP = 60.3441614;
	buf.avd.DIPDIR = 29.83251102;
	buf.avd.DIP = 60.3441614;
	buf.avS0offset = "OVERTURNED";
	buf.fold_great_circle_N.X = NaN ();
	buf.fold_great_circle_N.Y = NaN ();
	buf.fold_great_circle_N.Z = NaN ();
	buf.SHEAR_S.X = NaN ();
	buf.SHEAR_S.Y = NaN ();
	buf.SHEAR_S.Z = NaN ();
	buf.NORMAL_S.X = NaN ();
	buf.NORMAL_S.Y = NaN ();
	buf.NORMAL_S.Z = NaN ();
	buf.UPSILON.X = NaN ();
	buf.UPSILON.Y = NaN ();
	buf.UPSILON.Z = NaN ();
	buf.lambda = NaN ();
	buf.ANG = NaN ();
	buf.RUP = NaN ();
	OUT.push_back(buf);

	buf.ID = "BZ6005";
	buf.iID = 1;
	buf.N.X = 0.56754777;
	buf.N.Y = 0.67637710;
	buf.N.Z = 0.46947156;
	buf.D.X = 0.3017705;
	buf.D.Y = 0.35963608;
	buf.D.Z = -0.88294759;
	buf.S.X = -0.76604444;
	buf.S.Y = 0.64278761;
	buf.S.Z = 0;
	buf.NC.X = NaN ();
	buf.NC.Y = NaN ();
	buf.NC.Z = NaN ();
	buf.DC.X = NaN ();
	buf.DC.Y = NaN ();
	buf.DC.Z = NaN ();
	buf.SC.X = NaN ();
	buf.SC.Y = NaN ();
	buf.SC.Z = NaN ();
	buf.SV.X = NaN ();
	buf.SV.Y = NaN ();
	buf.SV.Z = NaN ();
	buf.LPITCH = NaN ();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN ();
	buf.MISFIT = NaN ();
	buf.LINEATION = "NONE";
	buf.UPWARD = 0;
	buf.OFFSET = "OVERTURNED";
	buf.UP = 0;
	buf.GC = "XXX";
	buf.COLOR = "";
	buf.LOC = "230";
	buf.LOCX = 488902;
	buf.LOCY = 3953801;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "BEDDING";
	buf.DIPDIR = 40;
	buf.DIP = 62;
	buf.LDIR = NaN ();
	buf.LDIP = NaN ();
	buf.corr.DIPDIR = 40;
	buf.corr.DIP = 62;
	buf.corrL.DIPDIR = NaN ();
	buf.corrL.DIP = NaN ();
	buf.PALEON = 0;
	buf.COMMENT = "BEDDING";
	buf.DATAGROUP = "PLANE";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";
	buf.ptnP.X = NaN ();
	buf.ptnP.Y = NaN ();
	buf.ptnP.Z = NaN ();
	buf.ptnT.X = NaN ();
	buf.ptnT.Y = NaN ();
	buf.ptnT.Z = NaN ();
	buf.ptnN.X = NaN ();
	buf.ptnN.Y = NaN ();
	buf.ptnN.Z = NaN ();
	buf.ptnPd.DIPDIR = NaN ();
	buf.ptnPd.DIP = NaN ();
	buf.ptnTd.DIPDIR = NaN ();
	buf.ptnTd.DIP = NaN ();
	buf.ptnNd.DIPDIR = NaN ();
	buf.ptnNd.DIP = NaN ();
	buf.avD.X = -0.24614085;
	buf.avD.Y = -0.42922122;
	buf.avD.Z = 0.86901314;
	buf.avS0D.X = -0.24614085;
	buf.avS0D.Y = -0.42922122;
	buf.avS0D.Z = 0.86901314;
	buf.avS0N.X = -0.43230473;
	buf.avS0N.Y = -0.7538544;
	buf.avS0N.Z = 0.49478901;
	buf.avS0d.DIPDIR = 29.83251102;
	buf.avS0d.DIP = 60.3441614;
	buf.avd.DIPDIR = 29.83251102;
	buf.avd.DIP = 60.3441614;
	buf.avS0offset = "OVERTURNED";
	buf.fold_great_circle_N.X = NaN ();
	buf.fold_great_circle_N.Y = NaN ();
	buf.fold_great_circle_N.Z = NaN ();
	buf.SHEAR_S.X = NaN ();
	buf.SHEAR_S.Y = NaN ();
	buf.SHEAR_S.Z = NaN ();
	buf.NORMAL_S.X = NaN ();
	buf.NORMAL_S.Y = NaN ();
	buf.NORMAL_S.Z = NaN ();
	buf.UPSILON.X = NaN ();
	buf.UPSILON.Y = NaN ();
	buf.UPSILON.Z = NaN ();
	buf.lambda = NaN ();
	buf.ANG = NaN ();
	buf.RUP = NaN ();
	OUT.push_back(buf);

	buf.ID = "BZ6006";
	buf.iID = 2;
	buf.N.X = 0.31343567;
	buf.N.Y = 0.81652784;
	buf.N.Z = 0.48480962;
	buf.D.X = 0.17374023;
	buf.D.Y = 0.45260877;
	buf.D.Z = -0.87461971;
	buf.S.X = -0.93358043;
	buf.S.Y = 0.35836795;
	buf.S.Z = 0;
	buf.NC.X = NaN ();
	buf.NC.Y = NaN ();
	buf.NC.Z = NaN ();
	buf.DC.X = NaN ();
	buf.DC.Y = NaN ();
	buf.DC.Z = NaN ();
	buf.SC.X = NaN ();
	buf.SC.Y = NaN ();
	buf.SC.Z = NaN ();
	buf.SV.X = NaN ();
	buf.SV.Y = NaN ();
	buf.SV.Z = NaN ();
	buf.LPITCH = NaN ();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN ();
	buf.MISFIT = NaN ();
	buf.LINEATION = "NONE";
	buf.UPWARD = 0;
	buf.OFFSET = "OVERTURNED";
	buf.UP = 0;
	buf.GC = "XXX";
	buf.COLOR = "";
	buf.LOC = "230";
	buf.LOCX = 488902;
	buf.LOCY = 3953801;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "BEDDING";
	buf.DIPDIR = 21;
	buf.DIP = 61;
	buf.LDIR = NaN ();
	buf.LDIP = NaN ();
	buf.corr.DIPDIR = 21;
	buf.corr.DIP = 61;
	buf.corrL.DIPDIR = NaN ();
	buf.corrL.DIP = NaN ();
	buf.PALEON = 0;
	buf.COMMENT = "BEDDING";
	buf.DATAGROUP = "PLANE";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";
	buf.ptnP.X = NaN ();
	buf.ptnP.Y = NaN ();
	buf.ptnP.Z = NaN ();
	buf.ptnT.X = NaN ();
	buf.ptnT.Y = NaN ();
	buf.ptnT.Z = NaN ();
	buf.ptnN.X = NaN ();
	buf.ptnN.Y = NaN ();
	buf.ptnN.Z = NaN ();
	buf.ptnPd.DIPDIR = NaN ();
	buf.ptnPd.DIP = NaN ();
	buf.ptnTd.DIPDIR = NaN ();
	buf.ptnTd.DIP = NaN ();
	buf.ptnNd.DIPDIR = NaN ();
	buf.ptnNd.DIP = NaN ();
	buf.avD.X = -0.24614085;
	buf.avD.Y = -0.42922122;
	buf.avD.Z = 0.86901314;
	buf.avS0D.X = -0.24614085;
	buf.avS0D.Y = -0.42922122;
	buf.avS0D.Z = 0.86901314;
	buf.avS0N.X = -0.43230473;
	buf.avS0N.Y = -0.7538544;
	buf.avS0N.Z = 0.49478901;
	buf.avS0d.DIPDIR = 29.83251102;
	buf.avS0d.DIP = 60.3441614;
	buf.avd.DIPDIR = 29.83251102;
	buf.avd.DIP = 60.3441614;
	buf.avS0offset = "OVERTURNED";
	buf.fold_great_circle_N.X = NaN ();
	buf.fold_great_circle_N.Y = NaN ();
	buf.fold_great_circle_N.Z = NaN ();
	buf.SHEAR_S.X = NaN ();
	buf.SHEAR_S.Y = NaN ();
	buf.SHEAR_S.Z = NaN ();
	buf.NORMAL_S.X = NaN ();
	buf.NORMAL_S.Y = NaN ();
	buf.NORMAL_S.Z = NaN ();
	buf.UPSILON.X = NaN ();
	buf.UPSILON.Y = NaN ();
	buf.UPSILON.Z = NaN ();
	buf.lambda = NaN ();
	buf.ANG = NaN ();
	buf.RUP = NaN ();
	OUT.push_back(buf);

	buf.ID = "BZ6007";
	buf.iID = 3;
	buf.N.X = 0.53943286;
	buf.N.Y = 0.66614411;
	buf.N.Z = 0.51503807;
	buf.D.X = 0.32412396;
	buf.D.Y = 0.40025976;
	buf.D.Z = -0.8571673;
	buf.S.X = -0.77714596;
	buf.S.Y = 0.62932039;
	buf.S.Z = 0;
	buf.NC.X = NaN ();
	buf.NC.Y = NaN ();
	buf.NC.Z = NaN ();
	buf.DC.X = NaN ();
	buf.DC.Y = NaN ();
	buf.DC.Z = NaN ();
	buf.SC.X = NaN ();
	buf.SC.Y = NaN ();
	buf.SC.Z = NaN ();
	buf.SV.X = NaN ();
	buf.SV.Y = NaN ();
	buf.SV.Z = NaN ();
	buf.LPITCH = NaN ();
	buf.LPITCHSENSE = "NONE";
	buf.PITCHANGLE = NaN ();
	buf.MISFIT = NaN ();
	buf.LINEATION = "NONE";
	buf.UPWARD = 0;
	buf.OFFSET = "OVERTURNED";
	buf.UP = 0;
	buf.GC = "XXX";
	buf.COLOR = "";
	buf.LOC = "230";
	buf.LOCX = 488902;
	buf.LOCY = 3953801;
	buf.FORMATION = "LOWERFARS";
	buf.DATATYPE = "BEDDING";
	buf.DIPDIR = 39;
	buf.DIP = 59;
	buf.LDIR = NaN ();
	buf.LDIP = NaN ();
	buf.corr.DIPDIR = 39;
	buf.corr.DIP = 59;
	buf.corrL.DIPDIR = NaN ();
	buf.corrL.DIP = NaN ();
	buf.PALEON = 0;
	buf.COMMENT = "BEDDING";
	buf.DATAGROUP = "PLANE";
	buf.PSCOLOR = "0.00 0.00 0.00";
	buf.DASHED = "   ";
	buf.ptnP.X = NaN ();
	buf.ptnP.Y = NaN ();
	buf.ptnP.Z = NaN ();
	buf.ptnT.X = NaN ();
	buf.ptnT.Y = NaN ();
	buf.ptnT.Z = NaN ();
	buf.ptnN.X = NaN ();
	buf.ptnN.Y = NaN ();
	buf.ptnN.Z = NaN ();
	buf.ptnPd.DIPDIR = NaN ();
	buf.ptnPd.DIP = NaN ();
	buf.ptnTd.DIPDIR = NaN ();
	buf.ptnTd.DIP = NaN ();
	buf.ptnNd.DIPDIR = NaN ();
	buf.ptnNd.DIP = NaN ();
	buf.avD.X = -0.24614085;
	buf.avD.Y = -0.42922122;
	buf.avD.Z = 0.86901314;
	buf.avS0D.X = -0.24614085;
	buf.avS0D.Y = -0.42922122;
	buf.avS0D.Z = 0.86901314;
	buf.avS0N.X = -0.43230473;
	buf.avS0N.Y = -0.7538544;
	buf.avS0N.Z = 0.49478901;
	buf.avS0d.DIPDIR = 29.83251102;
	buf.avS0d.DIP = 60.3441614;
	buf.avd.DIPDIR = 29.83251102;
	buf.avd.DIP = 60.3441614;
	buf.avS0offset = "OVERTURNED";
	buf.fold_great_circle_N.X = NaN ();
	buf.fold_great_circle_N.Y = NaN ();
	buf.fold_great_circle_N.Z = NaN ();
	buf.SHEAR_S.X = NaN ();
	buf.SHEAR_S.Y = NaN ();
	buf.SHEAR_S.Z = NaN ();
	buf.NORMAL_S.X = NaN ();
	buf.NORMAL_S.Y = NaN ();
	buf.NORMAL_S.Z = NaN ();
	buf.UPSILON.X = NaN ();
	buf.UPSILON.Y = NaN ();
	buf.UPSILON.Z = NaN ();
	buf.lambda = NaN ();
	buf.ANG = NaN ();
	buf.RUP = NaN ();
	OUT.push_back(buf);

	return OUT;
}


void check_standard_FOLDSURFACE (const vector <GDB>& nGDB) {

	cout << "**** FOLDSURFACE TEST: IF FAILS LISTED BELOW:" << endl;

	cout << fixed << setprecision(8) << flush;

	const VCTR T = nGDB.at(0).fold_great_circle_N;

	const VCTR S = declare_vector (-0.61688255, -0.72986844, -0.29453009);

	if (nGDB.at(0).ID == "F_SURF_01") {

		if (!is_in_range (T.X, T.X, S.X)) cout << "fold_great_circle_N.X test failed: " << T.X << " <> " << S.X << endl;
		if (!is_in_range (T.Y, T.Y, S.Y)) cout << "fold_great_circle_N.Y test failed: " << T.Y << " <> " << S.Y << endl;
		if (!is_in_range (T.Z, T.Z, S.Z)) cout << "fold_great_circle_N.Z test failed: " << T.Z << " <> " << S.Z << endl;

		cout << "**** END FOLDSURFACE TEST" << endl;
	}
	else {

		cout << "Test record 'F_SURF_01' not found - incorrect test file." << endl;
		ASSERT_DEAD_END();
	}
	return;
}

void check_standard_ROSE (const vector <ROSENUMBER>& in, const vector <ROSENUMBER>& ST) {

	cout << "**** ROSEDIAGRAM TEST: IF FAILS LISTED BELOW:" << endl;

	cout << fixed << setprecision(8) << flush;

	for (size_t i = 0; i < in.size(); i++) {

		const double L = in.at(i).LIN_NUM;
		const double P = in.at(i).PLN_NUM;
		const double sL = ST.at(i).LIN_NUM;
		const double sP = ST.at(i).PLN_NUM;

		if (!is_in_range (sL, sL, L)) cout << "LIN test failed: " << sL << " <> " << L << endl;
		if (!is_in_range (sP, sP, P)) cout << "PLN test failed: " << sP << " <> " << P << endl;
	}
	cout << "**** END ROSEDIAGRAM TEST" << endl;

	exit (100);

	return;
}

vector <ROSENUMBER> generate_standard_ROSE () {

	vector <ROSENUMBER> OUT;

	ROSENUMBER buf;

	buf.LIN_NUM = 2;	buf.PLN_NUM = 1;	OUT.push_back (buf);
	buf.LIN_NUM = 1;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 1;	buf.PLN_NUM = 2;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 1;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 3;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 6;	buf.PLN_NUM = 2;	OUT.push_back (buf);
	buf.LIN_NUM = 5;	buf.PLN_NUM = 3;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 9;	OUT.push_back (buf);
	buf.LIN_NUM = 8;	buf.PLN_NUM = 3;	OUT.push_back (buf);
	buf.LIN_NUM = 4;	buf.PLN_NUM = 7;	OUT.push_back (buf);
	buf.LIN_NUM = 4;	buf.PLN_NUM = 7;	OUT.push_back (buf);
	buf.LIN_NUM = 2;	buf.PLN_NUM = 6;	OUT.push_back (buf);
	buf.LIN_NUM = 1;	buf.PLN_NUM = 3;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 0;	OUT.push_back (buf);
	buf.LIN_NUM = 8;	buf.PLN_NUM = 5;	OUT.push_back (buf);
	buf.LIN_NUM = 14;	buf.PLN_NUM = 10;	OUT.push_back (buf);
	buf.LIN_NUM = 15;	buf.PLN_NUM = 19;	OUT.push_back (buf);
	buf.LIN_NUM = 1;	buf.PLN_NUM = 8;	OUT.push_back (buf);
	buf.LIN_NUM = 0;	buf.PLN_NUM = 2;	OUT.push_back (buf);

	return OUT;
}

void check_standard_COLORS (const vector <GDB>& inGDB) {

	vector <string> ID_buf;
	vector <string> GC_buf;
	vector <string> COLOR_buf;
	vector <string> PSCOLOR_buf;

	const vector <GDB> SGDB = sort_by_iID (inGDB);

	ID_buf.push_back ("GRP_001");	GC_buf.push_back ("BXX");	COLOR_buf.push_back ("A");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_002");	GC_buf.push_back ("CXX");	COLOR_buf.push_back ("A");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_003");	GC_buf.push_back ("AXX");	COLOR_buf.push_back ("A");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_004");	GC_buf.push_back ("BXX");	COLOR_buf.push_back ("A");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_005");	GC_buf.push_back ("CXX");	COLOR_buf.push_back ("A");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_006");	GC_buf.push_back ("ACX");	COLOR_buf.push_back ("B");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_007");	GC_buf.push_back ("AAX");	COLOR_buf.push_back ("B");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_008");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("B");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_009");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("B");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_010");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("C");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_011");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("C");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_012");	GC_buf.push_back ("CCX");	COLOR_buf.push_back ("C");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_013");	GC_buf.push_back ("CAX");	COLOR_buf.push_back ("C");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_014");	GC_buf.push_back ("DCX");	COLOR_buf.push_back ("C");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_015");	GC_buf.push_back ("DAX");	COLOR_buf.push_back ("D");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_016");	GC_buf.push_back ("ECX");	COLOR_buf.push_back ("D");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_017");	GC_buf.push_back ("EAX");	COLOR_buf.push_back ("D");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_018");	GC_buf.push_back ("BXX");	COLOR_buf.push_back ("D");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_019");	GC_buf.push_back ("CXX");	COLOR_buf.push_back ("D");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_020");	GC_buf.push_back ("AXX");	COLOR_buf.push_back ("E");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_021");	GC_buf.push_back ("BXX");	COLOR_buf.push_back ("E");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_022");	GC_buf.push_back ("CXX");	COLOR_buf.push_back ("E");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_023");	GC_buf.push_back ("ACX");	COLOR_buf.push_back ("E");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_024");	GC_buf.push_back ("AAX");	COLOR_buf.push_back ("E");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_025");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("F");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_026");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("F");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_027");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("F");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_028");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("F");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_029");	GC_buf.push_back ("CCX");	COLOR_buf.push_back ("F");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_030");	GC_buf.push_back ("CAX");	COLOR_buf.push_back ("G");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_031");	GC_buf.push_back ("DCX");	COLOR_buf.push_back ("G");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_032");	GC_buf.push_back ("DAX");	COLOR_buf.push_back ("G");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_033");	GC_buf.push_back ("EXX");	COLOR_buf.push_back ("G");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_034");	GC_buf.push_back ("ACB");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_035");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_036");	GC_buf.push_back ("ACA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_037");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_038");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_039");	GC_buf.push_back ("ACB");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_040");	GC_buf.push_back ("ABB");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_041");	GC_buf.push_back ("ACA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_042");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_043");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_044");	GC_buf.push_back ("ACA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_045");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_046");	GC_buf.push_back ("ACA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_047");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_048");	GC_buf.push_back ("ACA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_049");	GC_buf.push_back ("ABB");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_050");	GC_buf.push_back ("ACA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_051");	GC_buf.push_back ("ABA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_052");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_053");	GC_buf.push_back ("BBA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_054");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_055");	GC_buf.push_back ("BBA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_056");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_057");	GC_buf.push_back ("BBA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_058");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_059");	GC_buf.push_back ("BBA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_060");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_061");	GC_buf.push_back ("BBB");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_062");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_063");	GC_buf.push_back ("BBA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_064");	GC_buf.push_back ("BAX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 0.00");
	ID_buf.push_back ("GRP_065");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_066");	GC_buf.push_back ("BBB");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_067");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");
	ID_buf.push_back ("GRP_068");	GC_buf.push_back ("BBA");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("0.00 0.00 1.00");
	ID_buf.push_back ("GRP_069");	GC_buf.push_back ("BCX");	COLOR_buf.push_back ("");	PSCOLOR_buf.push_back ("1.00 0.00 0.67");

	if (SGDB.at(0).ID == "GRP_001") {

		cout << "**** COLORS TEST: IF FAILS LISTED BELOW" << endl;

		for (size_t i = 0; i < SGDB.size(); i++) {

			if (ID_buf.at(i) != SGDB.at(i).ID) cout << "ID test failed: " << ID_buf.at(i) << " <> " << SGDB.at(i).ID << endl;
			if (GC_buf.at(i) != SGDB.at(i).GC) cout << "GC test failed: " << GC_buf.at(i) << " <> " << SGDB.at(i).GC << endl;
			if (COLOR_buf.at(i) != SGDB.at(i).COLOR) cout << "COLOR test failed: " << COLOR_buf.at(i) << " <> " << SGDB.at(i).COLOR << endl;
			if (PSCOLOR_buf.at(i) != SGDB.at(i).PSCOLOR) cout << "PSCOLOR test failed: " << PSCOLOR_buf.at(i) << " <> " << SGDB.at(i).PSCOLOR << endl;
		}
		cout << "**** END COLORS TEST" << endl;

		exit (100);

		return;
	}
	else {

		cout << "Test record 'GRP_001' not found - incorrect test file." << endl;
		ASSERT_DEAD_END();
	}
	return;
}

void compare_GDB (const GDB& nGDB, const GDB& ST_nGDB) {

	const GDB t = nGDB;
	const GDB S = ST_nGDB;

	if (t.ID != S.ID) cout << "ID test failed: " << t.ID << " <> " << S.ID << endl;
	if (!isnan (t.iID) && !isnan (S.iID) && !is_in_range (t.iID, t.iID, S.iID)) cout << "iID test failed: " << t.iID << " <> " << S.iID << endl;

	if (!isnan (t.N.X) && !isnan (S.N.X) && !is_in_range (t.N.X, t.N.X, S.N.X)) cout << "N.X test failed: " << t.N.X << " <> " << S.N.X << endl;
	if (!isnan (t.N.Y) && !isnan (S.N.Y) && !is_in_range (t.N.Y, t.N.Y, S.N.Y)) cout << "N.Y test failed: " << t.N.Y << " <> " << S.N.Y << endl;
	if (!isnan (t.N.Z) && !isnan (S.N.Z) && !is_in_range (t.N.Z, t.N.Z, S.N.Z)) cout << "N.Z test failed: " << t.N.Z << " <> " << S.N.Z << endl;

	if (!isnan (t.D.X) && !isnan (S.D.X) && !is_in_range (t.D.X, t.D.X, S.D.X)) cout << "D.X test failed: " << t.D.X << " <> " << S.D.X << endl;
	if (!isnan (t.D.Y) && !isnan (S.D.Y) && !is_in_range (t.D.Y, t.D.Y, S.D.Y)) cout << "D.Y test failed: " << t.D.Y << " <> " << S.D.Y << endl;
	if (!isnan (t.D.Z) && !isnan (S.D.Z) && !is_in_range (t.D.Z, t.D.Z, S.D.Z)) cout << "D.Z test failed: " << t.D.Z << " <> " << S.D.Z << endl;

	if (!isnan (t.S.X) && !isnan (S.S.X) && !is_in_range (t.S.X, t.S.X, S.S.X)) cout << "S.X test failed: " << t.S.X << " <> " << S.S.X << endl;
	if (!isnan (t.S.Y) && !isnan (S.S.Y) && !is_in_range (t.S.Y, t.S.Y, S.S.Y)) cout << "S.Y test failed: " << t.S.Y << " <> " << S.S.Y << endl;
	if (!isnan (t.S.Z) && !isnan (S.S.Z) && !is_in_range (t.S.Z, t.S.Z, S.S.Z)) cout << "S.Z test failed: " << t.S.Z << " <> " << S.S.Z << endl;

	if (!isnan (t.NC.X) && !isnan (S.NC.X) && !is_in_range (t.NC.X, t.NC.X, S.NC.X)) cout << "NC.X test failed: " << t.NC.X << " <> " << S.NC.X << endl;
	if (!isnan (t.NC.Y) && !isnan (S.NC.Y) && !is_in_range (t.NC.Y, t.NC.Y, S.NC.Y)) cout << "NC.Y test failed: " << t.NC.Y << " <> " << S.NC.Y << endl;
	if (!isnan (t.NC.Z) && !isnan (S.NC.Z) && !is_in_range (t.NC.Z, t.NC.Z, S.NC.Z)) cout << "NC.Z test failed: " << t.NC.Z << " <> " << S.NC.Z << endl;

	if (!isnan (t.DC.X) && !isnan (S.DC.X) && !is_in_range (t.DC.X, t.DC.X, S.DC.X)) cout << "DC.X test failed: " << t.DC.X << " <> " << S.DC.X << endl;
	if (!isnan (t.DC.Y) && !isnan (S.DC.Y) && !is_in_range (t.DC.Y, t.DC.Y, S.DC.Y)) cout << "DC.Y test failed: " << t.DC.Y << " <> " << S.DC.Y << endl;
	if (!isnan (t.DC.Z) && !isnan (S.DC.Z) && !is_in_range (t.DC.Z, t.DC.Z, S.DC.Z)) cout << "DC.Z test failed: " << t.DC.Z << " <> " << S.DC.Z << endl;

	if (!isnan (t.SC.X) && !isnan (S.SC.X) && !is_in_range (t.SC.X, t.SC.X, S.SC.X)) cout << "SC.X test failed: " << t.SC.X << " <> " << S.SC.X << endl;
	if (!isnan (t.SC.Y) && !isnan (S.SC.Y) && !is_in_range (t.SC.Y, t.SC.Y, S.SC.Y)) cout << "SC.Y test failed: " << t.SC.Y << " <> " << S.SC.Y << endl;
	if (!isnan (t.SC.Z) && !isnan (S.SC.Z) && !is_in_range (t.SC.Z, t.SC.Z, S.SC.Z)) cout << "SC.Z test failed: " << t.SC.Z << " <> " << S.SC.Z << endl;

	if (!isnan (t.SV.X) && !isnan (S.SV.X) && !is_in_range (t.SV.X, t.SV.X, S.SV.X)) cout << "SV.X test failed: " << t.SV.X << " <> " << S.SV.X << endl;
	if (!isnan (t.SV.Y) && !isnan (S.SV.Y) && !is_in_range (t.SV.Y, t.SV.Y, S.SV.Y)) cout << "SV.Y test failed: " << t.SV.Y << " <> " << S.SV.Y << endl;
	if (!isnan (t.SV.Z) && !isnan (S.SV.Z) && !is_in_range (t.SV.Z, t.SV.Z, S.SV.Z)) cout << "SV.Z test failed: " << t.SV.Z << " <> " << S.SV.Z << endl;

	if (!isnan (t.LPITCH) && !isnan (S.LPITCH) && !is_in_range (t.LPITCH, t.LPITCH, S.LPITCH)) cout << "LPITCH test failed: " << t.LPITCH << " <> " << S.LPITCH << endl;
	if (t.LPITCHSENSE != S.LPITCHSENSE) cout << "LPITCHSENSE test failed: " << t.LPITCHSENSE << " <> " << S.LPITCHSENSE << endl;
	if (!isnan (t.PITCHANGLE) && !isnan (S.PITCHANGLE) && !is_in_range (t.PITCHANGLE, t.PITCHANGLE, S.PITCHANGLE)) cout << "PITCHANGLE test failed: " << t.PITCHANGLE << " <> " << S.PITCHANGLE << endl;
	if (!isnan (t.MISFIT) && !isnan (S.MISFIT) && !is_in_range (t.MISFIT, t.MISFIT, S.MISFIT)) cout << "MISFIT test failed: " << t.MISFIT << " <> " << S.MISFIT << endl;

	if (t.LINEATION != S.LINEATION) cout << "LINEATION test failed: " << t.LINEATION << " <> " << S.LINEATION << endl;
	if (!isnan (t.UPWARD) && !isnan (S.UPWARD) && !is_in_range (t.UPWARD, t.UPWARD, S.UPWARD)) cout << "UPWARD test failed: " << t.UPWARD << " <> " << S.UPWARD << endl;
	if (t.OFFSET != S.OFFSET) cout << "OFFSET test failed: " << t.OFFSET << " <> " << S.OFFSET << endl;
	if (!isnan (t.UP) && !isnan (S.UP) && !is_in_range (t.UP, t.UP, S.UP)) cout << "UP test failed: " << t.UP << " <> " << S.UP << endl;
	if (t.GC != S.GC) cout << "GC test failed: " << t.GC << " <> " << S.GC << endl;
	if (t.COLOR != S.COLOR) cout << "COLOR test failed: " << t.COLOR << " <> " << S.COLOR << endl;
	if (t.LOC != S.LOC) cout << "LOC test failed: " << t.LOC << " <> " << S.LOC << endl;
	if (!isnan (t.LOCX) && !isnan (S.LOCX) && !is_in_range (t.LOCX, t.LOCX, S.LOCX)) cout << "LOCX test failed: " << t.LOCX << " <> " << S.LOCX << endl;
	if (!isnan (t.LOCY) && !isnan (S.LOCY) && !is_in_range (t.LOCY, t.LOCY, S.LOCY)) cout << "LOCY test failed: " << t.LOCY << " <> " << S.LOCY << endl;
	if (t.FORMATION != S.FORMATION) cout << "FORMATION test failed: " << t.FORMATION << " <> " << S.FORMATION << endl;
	if (t.DATATYPE != S.DATATYPE) cout << "DATATYPE test failed: " << t.DATATYPE << " <> " << S.DATATYPE << endl;

	if (!isnan (t.DIPDIR) && !isnan (S.DIPDIR) && !is_in_range (t.DIPDIR, t.DIPDIR, S.DIPDIR)) cout << "DIPDIR test failed: " << t.DIPDIR << " <> " << S.DIPDIR << endl;
	if (!isnan (t.DIP) && !isnan (S.DIP) && !is_in_range (t.DIP, t.DIP, S.DIP)) cout << "DIP test failed: " << t.DIP << " <> " << S.DIP << endl;
	if (!isnan (t.LDIR) && !isnan (S.LDIR) && !is_in_range (t.LDIR, t.LDIR, S.LDIR)) cout << "LDIR test failed: " << t.LDIR << " <> " << S.LDIR << endl;
	if (!isnan (t.LDIP) && !isnan (S.LDIP) && !is_in_range (t.LDIP, t.LDIP, S.LDIP)) cout << "LDIP test failed: " << t.LDIP << " <> " << S.LDIP << endl;

	if (!isnan (t.corr.DIPDIR) && !isnan (S.corr.DIPDIR) && !is_in_range (t.corr.DIPDIR, t.corr.DIPDIR, S.corr.DIPDIR)) cout << "corr.DIPDIR test failed: " << t.corr.DIPDIR << " <> " << S.corr.DIPDIR << endl;
	if (!isnan (t.corr.DIP) && !isnan (S.corr.DIP) && !is_in_range (t.corr.DIP, t.corr.DIP, S.corr.DIP)) cout << "corr.DIP test failed: " << t.corr.DIP << " <> " << S.corr.DIP << endl;
	if (!isnan (t.corrL.DIPDIR) && !isnan (S.corrL.DIPDIR) && !is_in_range (t.corrL.DIPDIR, t.corrL.DIPDIR, S.corrL.DIPDIR)) cout << "corrL.DIPDIR test failed: " << t.corrL.DIPDIR << " <> " << S.corrL.DIPDIR << endl;
	if (!isnan (t.corrL.DIP) && !isnan (S.corrL.DIP) && !is_in_range (t.corrL.DIP, t.corrL.DIP, S.corrL.DIP)) cout << "corrL.DIP test failed: " << t.corrL.DIP << " <> " << S.corrL.DIP << endl;

	if (!isnan (t.PALEON) && !isnan (S.PALEON) && !is_in_range (t.PALEON, t.PALEON, S.PALEON)) cout << "PALEON test failed: " << t.PALEON << " <> " << S.PALEON << endl;
	if (t.COMMENT != S.COMMENT) cout << "COMMENT test failed: " << t.COMMENT << " <> " << S.COMMENT << endl;
	if (t.DATAGROUP != S.DATAGROUP) cout << "DATAGROUP test failed: " << t.DATAGROUP << " <> " << S.DATAGROUP << endl;
	if (t.PSCOLOR != S.PSCOLOR) cout << "PSCOLOR test failed: " << t.PSCOLOR << " <> " << S.PSCOLOR << endl;
	if (t.DASHED != S.DASHED) cout << "DASHED test failed: " << t.DASHED << " <> " << S.DASHED << endl;

	if (!isnan (t.ptnP.X) && !isnan (S.ptnP.X) && !is_in_range (t.ptnP.X, t.ptnP.X, S.ptnP.X)) cout << "ptnP.X test failed: " << t.ptnP.X << " <> " << S.ptnP.X << endl;
	if (!isnan (t.ptnP.Y) && !isnan (S.ptnP.Y) && !is_in_range (t.ptnP.Y, t.ptnP.Y, S.ptnP.Y)) cout << "ptnP.Y test failed: " << t.ptnP.Y << " <> " << S.ptnP.Y << endl;
	if (!isnan (t.ptnP.Z) && !isnan (S.ptnP.Z) && !is_in_range (t.ptnP.Z, t.ptnP.Z, S.ptnP.Z)) cout << "ptnP.Z test failed: " << t.ptnP.Z << " <> " << S.ptnP.Z << endl;

	if (!isnan (t.ptnT.X) && !isnan (S.ptnT.X) && !is_in_range (t.ptnT.X, t.ptnT.X, S.ptnT.X)) cout << "ptnT.X test failed: " << t.ptnT.X << " <> " << S.ptnT.X << endl;
	if (!isnan (t.ptnT.Y) && !isnan (S.ptnT.Y) && !is_in_range (t.ptnT.Y, t.ptnT.Y, S.ptnT.Y)) cout << "ptnT.Y test failed: " << t.ptnT.Y << " <> " << S.ptnT.Y << endl;
	if (!isnan (t.ptnT.Z) && !isnan (S.ptnT.Z) && !is_in_range (t.ptnT.Z, t.ptnT.Z, S.ptnT.Z)) cout << "ptnT.Z test failed: " << t.ptnT.Z << " <> " << S.ptnT.Z << endl;

	if (!isnan (t.ptnN.X) && !isnan (S.ptnN.X) && !is_in_range (t.ptnN.X, t.ptnN.X, S.ptnN.X)) cout << "ptnN.X test failed: " << t.ptnN.X << " <> " << S.ptnN.X << endl;
	if (!isnan (t.ptnN.Y) && !isnan (S.ptnN.Y) && !is_in_range (t.ptnN.Y, t.ptnN.Y, S.ptnN.Y)) cout << "ptnN.Y test failed: " << t.ptnN.Y << " <> " << S.ptnN.Y << endl;
	if (!isnan (t.ptnN.Z) && !isnan (S.ptnN.Z) && !is_in_range (t.ptnN.Z, t.ptnN.Z, S.ptnN.Z)) cout << "ptnN.Z test failed: " << t.ptnN.Z << " <> " << S.ptnN.Z << endl;

	if (!isnan (t.ptnPd.DIPDIR) && !isnan (S.ptnPd.DIPDIR) && !is_in_range (t.ptnPd.DIPDIR, t.ptnPd.DIPDIR, S.ptnPd.DIPDIR)) cout << "ptnPd.DIPDIR test failed: " << t.ptnPd.DIPDIR << " <> " << S.ptnPd.DIPDIR << endl;
	if (!isnan (t.ptnPd.DIP) && !isnan (S.ptnPd.DIP) && !is_in_range (t.ptnPd.DIP, t.ptnPd.DIP, S.ptnPd.DIP)) cout << "ptnPd.DIP test failed: " << t.ptnPd.DIP << " <> " << S.ptnPd.DIP << endl;
	if (!isnan (t.ptnTd.DIPDIR) && !isnan (S.ptnTd.DIPDIR) && !is_in_range (t.ptnTd.DIPDIR, t.ptnTd.DIPDIR, S.ptnTd.DIPDIR)) cout << "ptnTd.DIPDIR test failed: " << t.ptnTd.DIPDIR << " <> " << S.ptnTd.DIPDIR << endl;
	if (!isnan (t.ptnTd.DIP) && !isnan (S.ptnTd.DIP) && !is_in_range (t.ptnTd.DIP, t.ptnTd.DIP, S.ptnTd.DIP)) cout << "ptnTd.DIP test failed: " << t.ptnTd.DIP << " <> " << S.ptnTd.DIP << endl;
	if (!isnan (t.ptnNd.DIPDIR) && !isnan (S.ptnNd.DIPDIR) && !is_in_range (t.ptnNd.DIPDIR, t.ptnNd.DIPDIR, S.ptnNd.DIPDIR)) cout << "ptnNd.DIPDIR test failed: " << t.ptnNd.DIPDIR << " <> " << S.ptnNd.DIPDIR << endl;
	if (!isnan (t.ptnNd.DIP) && !isnan (S.ptnNd.DIP) && !is_in_range (t.ptnNd.DIP, t.ptnNd.DIP, S.ptnNd.DIP)) cout << "ptnNd.DIP test failed: " << t.ptnNd.DIP << " <> " << S.ptnNd.DIP << endl;

	if (!isnan (t.fold_great_circle_N.X) && !isnan (S.fold_great_circle_N.X) && !is_in_range (t.fold_great_circle_N.X, t.fold_great_circle_N.X, S.fold_great_circle_N.X)) cout << "fold_great_circle_N.X test failed: " << t.fold_great_circle_N.X << " <> " << S.fold_great_circle_N.X << endl;
	if (!isnan (t.fold_great_circle_N.Y) && !isnan (S.fold_great_circle_N.Y) && !is_in_range (t.fold_great_circle_N.Y, t.fold_great_circle_N.Y, S.fold_great_circle_N.Y)) cout << "fold_great_circle_N.Y test failed: " << t.fold_great_circle_N.Y << " <> " << S.fold_great_circle_N.Y << endl;
	if (!isnan (t.fold_great_circle_N.Z) && !isnan (S.fold_great_circle_N.Z) && !is_in_range (t.fold_great_circle_N.Z, t.fold_great_circle_N.Z, S.fold_great_circle_N.Z)) cout << "fold_great_circle_N.Z test failed: " << t.fold_great_circle_N.Z << " <> " << S.fold_great_circle_N.Z << endl;

	if (!isnan (t.avD.X) && !isnan (S.avD.X) && !is_in_range (t.avD.X, t.avD.X, S.avD.X)) cout << "avD.X test failed: " << t.avD.X << " <> " << S.avD.X << endl;
	if (!isnan (t.avD.Y) && !isnan (S.avD.Y) && !is_in_range (t.avD.Y, t.avD.Y, S.avD.Y)) cout << "avD.Y test failed: " << t.avD.Y << " <> " << S.avD.Y << endl;
	if (!isnan (t.avD.Z) && !isnan (S.avD.Z) && !is_in_range (t.avD.Z, t.avD.Z, S.avD.Z)) cout << "avD.Z test failed: " << t.avD.Z << " <> " << S.avD.Z << endl;

	if (!isnan (t.avS0D.X) && !isnan (S.avS0D.X) && !is_in_range (t.avS0D.X, t.avS0D.X, S.avS0D.X)) cout << "avS0D.X test failed: " << t.avS0D.X << " <> " << S.avS0D.X << endl;
	if (!isnan (t.avS0D.Y) && !isnan (S.avS0D.Y) && !is_in_range (t.avS0D.Y, t.avS0D.Y, S.avS0D.Y)) cout << "avS0D.Y test failed: " << t.avS0D.Y << " <> " << S.avS0D.Y << endl;
	if (!isnan (t.avS0D.Z) && !isnan (S.avS0D.Z) && !is_in_range (t.avS0D.Z, t.avS0D.Z, S.avS0D.Z)) cout << "avS0D.Z test failed: " << t.avS0D.Z << " <> " << S.avS0D.Z << endl;

	if (!isnan (t.avS0N.X) && !isnan (S.avS0N.X) && !is_in_range (t.avS0N.X, t.avS0N.X, S.avS0N.X)) cout << "avS0N.X test failed: " << t.avS0N.X << " <> " << S.avS0N.X << endl;
	if (!isnan (t.avS0N.Y) && !isnan (S.avS0N.Y) && !is_in_range (t.avS0N.Y, t.avS0N.Y, S.avS0N.Y)) cout << "avS0N.Y test failed: " << t.avS0N.Y << " <> " << S.avS0N.Y << endl;
	if (!isnan (t.avS0N.Z) && !isnan (S.avS0N.Z) && !is_in_range (t.avS0N.Z, t.avS0N.Z, S.avS0N.Z)) cout << "avS0N.Z test failed: " << t.avS0N.Z << " <> " << S.avS0N.Z << endl;

	if (!isnan (t.avS0d.DIPDIR) && !isnan (S.avS0d.DIPDIR) && !is_in_range (t.avS0d.DIPDIR, t.avS0d.DIPDIR, S.avS0d.DIPDIR)) cout << "avS0d.DIPDIR test failed: " << t.avS0d.DIPDIR << " <> " << S.avS0d.DIPDIR << endl;
	if (!isnan (t.avS0d.DIP) && !isnan (S.avS0d.DIP) && !is_in_range (t.avS0d.DIP, t.avS0d.DIP, S.avS0d.DIP)) cout << "avS0d.DIP test failed: " << t.avS0d.DIP << " <> " << S.avS0d.DIP << endl;
	if (!isnan (t.avd.DIPDIR) && !isnan (S.avd.DIPDIR) && !is_in_range (t.avd.DIPDIR, t.avd.DIPDIR, S.avd.DIPDIR)) cout << "avd.DIPDIR test failed: " << t.avd.DIPDIR << " <> " << S.avd.DIPDIR << endl;
	if (!isnan (t.avd.DIP) && !isnan (S.avd.DIP) && !is_in_range (t.avd.DIP, t.avd.DIP, S.avd.DIP)) cout << "avd.DIP test failed: " << t.avd.DIP << " <> " << S.avd.DIP << endl;

	if (t.avS0offset != S.avS0offset) cout << "avS0offset test failed: " << t.avS0offset << " <> " << S.avS0offset << endl;

	if (!isnan (t.SHEAR_S.X) && !isnan (S.SHEAR_S.X) && !is_in_range (t.SHEAR_S.X, t.SHEAR_S.X, S.SHEAR_S.X)) cout << "SHEAR_S.X test failed: " << t.SHEAR_S.X << " <> " << S.SHEAR_S.X << endl;
	if (!isnan (t.SHEAR_S.Y) && !isnan (S.SHEAR_S.Y) && !is_in_range (t.SHEAR_S.Y, t.SHEAR_S.Y, S.SHEAR_S.Y)) cout << "SHEAR_S.Y test failed: " << t.SHEAR_S.Y << " <> " << S.SHEAR_S.Y << endl;
	if (!isnan (t.SHEAR_S.Z) && !isnan (S.SHEAR_S.Z) && !is_in_range (t.SHEAR_S.Z, t.SHEAR_S.Z, S.SHEAR_S.Z)) cout << "SHEAR_S.Z test failed: " << t.SHEAR_S.Z << " <> " << S.SHEAR_S.Z << endl;

	if (!isnan (t.NORMAL_S.X) && !isnan (S.NORMAL_S.X) && !is_in_range (t.NORMAL_S.X, t.NORMAL_S.X, S.NORMAL_S.X)) cout << "NORMAL_S.X test failed: " << t.NORMAL_S.X << " <> " << S.NORMAL_S.X << endl;
	if (!isnan (t.NORMAL_S.Y) && !isnan (S.NORMAL_S.Y) && !is_in_range (t.NORMAL_S.Y, t.NORMAL_S.Y, S.NORMAL_S.Y)) cout << "NORMAL_S.Y test failed: " << t.NORMAL_S.Y << " <> " << S.NORMAL_S.Y << endl;
	if (!isnan (t.NORMAL_S.Z) && !isnan (S.NORMAL_S.Z) && !is_in_range (t.NORMAL_S.Z, t.NORMAL_S.Z, S.NORMAL_S.Z)) cout << "NORMAL_S.Z test failed: " << t.NORMAL_S.Z << " <> " << S.NORMAL_S.Z << endl;

	if (!isnan (t.UPSILON.X) && !isnan (S.UPSILON.X) && !is_in_range (t.UPSILON.X, t.UPSILON.X, S.UPSILON.X)) cout << "UPSILON.X test failed: " << t.UPSILON.X << " <> " << S.UPSILON.X << endl;
	if (!isnan (t.UPSILON.Y) && !isnan (S.UPSILON.Y) && !is_in_range (t.UPSILON.Y, t.UPSILON.Y, S.UPSILON.Y)) cout << "UPSILON.Y test failed: " << t.UPSILON.Y << " <> " << S.UPSILON.Y << endl;
	if (!isnan (t.UPSILON.Z) && !isnan (S.UPSILON.Z) && !is_in_range (t.UPSILON.Z, t.UPSILON.Z, S.UPSILON.Z)) cout << "UPSILON.Z test failed: " << t.UPSILON.Z << " <> " << S.UPSILON.Z << endl;

	if (!isnan (t.lambda) && !isnan (S.lambda) && !is_in_range (t.lambda, t.lambda, S.lambda)) cout << "lambda test failed: " << t.lambda << " <> " << S.lambda << endl;
	if (!isnan (t.ANG) && !isnan (S.ANG) && !is_in_range (t.ANG, t.ANG, S.ANG)) cout << "ANG test failed: " << t.ANG << " <> " << S.ANG << endl;
	if (!isnan (t.RUP) && !isnan (S.RUP) && !is_in_range (t.RUP, t.RUP, S.RUP)) cout << "RUP test failed: " << t.RUP << " <> " << S.RUP << endl;

	return;
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

	if (is_CHK_LITHOLOGY_DT ()) 	check_standard_output_lithology(nGDB, init_standard_GDB_lithology ());
	else if (is_CHK_LINEATION_DT()) check_standard_output_lineation (nGDB, init_standard_GDB_lineation ());
	else if (is_CHK_PLANE_DT ()) 	check_standard_output_fracture (nGDB, init_standard_GDB_fracture ());
	else if (is_CHK_STRIAE_DT ())	check_standard_output_striae (nGDB, init_standard_GDB_striae ());
	else if (is_CHK_SC_DT ()) 		check_standard_output_SC(nGDB, init_standard_GDB_SC ());

	else if (is_CHK_ANGELIER() && is_INVERSION_ANGELIER()) 		check_standard_output_SF (nGDB, init_standard_GDB_ANGELIER());
	else if (is_CHK_BINGHAM() && is_BINGHAM_USE()) 				check_standard_output_SF (nGDB, init_standard_GDB_BINGHAM ());
	else if (is_CHK_BRUTEFORCE() && is_INVERSION_BRUTEFORCE()) 	check_standard_output_SF (nGDB, init_standard_GDB_BRUTEFORCE());
	else if (is_CHK_FRY() && is_INVERSION_FRY()) 				check_standard_output_SF (nGDB, init_standard_GDB_FRY());
	else if (is_CHK_MICHAEL() && is_INVERSION_MICHAEL()) 		check_standard_output_SF (nGDB, init_standard_GDB_MICHAEL());
	else if (is_CHK_MOSTAFA() && is_INVERSION_MOSTAFA()) 		check_standard_output_SF (nGDB, init_standard_GDB_MOSTAFA());
	else if (is_CHK_SPRANG() && is_INVERSION_SPRANG()) 			check_standard_output_SF (nGDB, init_standard_GDB_NDA());
	else if (is_CHK_SHAN() && is_INVERSION_SHAN()) 				check_standard_output_SF (nGDB, init_standard_GDB_SHAN());
	else if (is_CHK_TURNER() && is_INVERSION_TURNER()) 			check_standard_output_SF (nGDB, init_standard_GDB_PTN ());

	else if (is_CHK_RUP_ANG() && is_RUP_CLUSTERING_RUP()) {

		const vector <VALLEY> V_IN = return_valleygraph_for_dataset (nGDB, "RUP");
		const vector <VALLEY> V_ST = return_standard_valleygraph ();
		check_standard_output_valley(V_IN, V_ST);

		const size_t bin_number = return_DATA_ideal_bin_number (GDB_to_table (nGDB, "RUP"));

		const vector <HISTOGRAM> H_IN = generate_DATA_histogram (GDB_to_table (nGDB, "RUP"), bin_number);
		const vector <HISTOGRAM> H_ST = return_standard_histogram();
		check_standard_output_histogram (H_IN, H_ST);
	}
	else if (is_CHK_RETILT()) check_standard_RETILT (tGDB, init_standard_RETILT ());
	else if (is_CHK_AVERAGE())check_standard_AVERAGE (nGDB, init_standard_AVERAGE());
	else if (is_CHK_COLORS())check_standard_COLORS (nGDB);
	else {};
}

