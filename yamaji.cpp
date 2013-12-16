// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
//#include <cmath>
//#include <iostream>
//#include <iomanip>
#include <vector>

//#include "average.hpp"
//#include "allowed_keys.hpp"
//#include "assertions.hpp"
//#include "bingham.h"
//#include "brute_force.hpp"
//#include "checkrgffilecontent.h"
//#include "checkxycontent.h"
//#include "data_io.h"
#include "inversion.h"
//#include "ps.h"
//#include "random.hpp"
//#include "retilt.hpp"
//#include "rgf.h"
#include "structs.h"
#include "yamaji.hpp"

using namespace std;

vector <BRUTEFORCE_RESULT> YAMAJI_ENGINE (const vector <GDB>& inGDB, const VCTR& CNTR, const double& ANG, const double& PHI, const INPSET& inset) {

	/*
	 *
	 * else {

		CNTRVCTR = return_rakhmanov_points(256);

		ANGVCTR = generate_angle_vector_180 (0.0, 180.0, 16);

		PHIVCTR = generate_phi_vector (0.0, 1.0, 15);
	}
	 *
	 *
	 */



	vector <BRUTEFORCE_RESULT> OUT;

	for (size_t i = 0; i < inGDB.size() - 3; i++) {
		for (size_t j = i + 1; j < inGDB.size() - 2; j++) {
			for (size_t k = i + 2; k < inGDB.size() - 1; k++) {
				for (size_t l = i + 3; l < inGDB.size(); l++) {

					vector <GDB> tempGDB;

					tempGDB.push_back(inGDB.at(i));
					tempGDB.push_back(inGDB.at(j));
					tempGDB.push_back(inGDB.at(k));
					tempGDB.push_back(inGDB.at(l));

					STRESSTENSOR st = return_stresstensor_from_n1_ang_phi (CNTR, ANG, PHI);

					tempGDB = return_stressvector_estimators(st, tempGDB, "BRUTEFORCE", false);

					//double MISFIT = calculate_cumulative_misfit (tempGDB);

					BRUTEFORCE_RESULT buf;

					//buf.ANG = ANG;
					//buf.NRM = CNTR;
					//buf.PHI = PHI;
					////buf.MISFIT = MISFIT;

					OUT.push_back(buf);
				}
			}
		}
	}
	return OUT;
}
