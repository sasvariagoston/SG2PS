// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
//#include <cmath>
#include <iostream>
//#include <iomanip>

#include "average.hpp"
#include "foldsurface.hpp"
//#include "rgf.h"
//#include "checkrgffilecontent.h"
//#include "checkxycontent.h"
//#include "data_io.h"
#include "ps.h"
//#include "random.hpp"

using namespace std;

void plot_fold_great_circle (VCTR GR_CRC, ofstream& o, INPSET inset, CENTER center) {

	GDB tempGDB;

	tempGDB.avD = DXDYDZ_from_NXNYNZ(GR_CRC);
	tempGDB.avd = dipdir_dip_from_DXDYDZ (tempGDB.avD);

	//cout << "plot_fold_great_circle: " << flush;
	//cout << tempGDB.avd.DIPDIR << "/" << tempGDB.avd.DIP << endl;

	if (inset.plot == "H") 	PS_polepoint (tempGDB, o, inset, center, false, "FOLD");
	else 					PS_plane     (tempGDB, o, inset, center, false, "FOLD");

	PS_folddata (tempGDB, o, center);

	return;
}

void calculate_foldsurface (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center) {

	vector <size_t> length_container = generate_block_length (inGDB, "FOLDSURFACE");

	inGDB = DATATYPE_AVERAGE (inGDB, length_container, "FOLDSURFACE");

	plot_fold_great_circle (inGDB.at(0).avD, o, inset, center);
}
