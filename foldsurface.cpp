
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <stdlib.h>

#include "average.hpp"
#include "foldsurface.hpp"
#include "ps.h"
#include "rgf.h"
#include "settings.hpp"
#include "standard_output.hpp"

using namespace std;

vector <GDB> CALCULATE_FOLDSURFACE_NORMAL (const vector <GDB>& inGDB) {

	const vector <GDB> OUT = DATATYPE_AVERAGE (inGDB, "FOLD");

	//if (is_CHK_FOLDSURFACE()) {

	//	check_standard_FOLDSURFACE (sort_by_iID(OUT));
	//	exit (99);
	//}

	return OUT;
}
