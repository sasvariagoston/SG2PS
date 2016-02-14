// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include "bingham.h"
#include "common.h"
#include "foldsurface.hpp"

using namespace std;

vector <GDB> CALCULATE_FOLDSURFACE_NORMAL (const vector <GDB>& inGDB) {

	vector <GDB> OUT = inGDB;

	const vector <VCTR> BNG = generate_Bingham_dataset(inGDB);

	const STRESSTENSOR ST = st_BINGHAM (BNG);

	const STRESSFIELD SF = sf_BINGHAM (ST);

	for (size_t i = 0; i < inGDB.size(); i++) OUT.at(i).fold_great_circle_N = flip_vector(SF.EIGENVECTOR3);

	return OUT;
}
