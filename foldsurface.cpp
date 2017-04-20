// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <stdlib.h>

#include "average.hpp"
#include "bingham.h"
#include "foldsurface.hpp"
#include "ps.h"
#include "rgf.h"
#include "settings.hpp"
#include "standard_output.hpp"

using namespace std;

vector <GDB> CALCULATE_FOLDSURFACE_NORMAL (const vector <GDB>& inGDB) {

	vector <GDB> OUT = inGDB;

	const vector <VCTR> BNG = generate_Bingham_dataset(inGDB);

	const STRESSTENSOR ST = st_BINGHAM (BNG);

	const STRESSFIELD SF = sf_BINGHAM (ST);

	for (size_t i = 0; i < inGDB.size(); i++) OUT.at(i).fold_great_circle_N = flip_vector(SF.EIGENVECTOR3);

	return OUT;
}
