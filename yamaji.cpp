// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
//#include <cmath>
#include <iostream>
//#include <iomanip>
#include <vector>

//#include "average.hpp"
//#include "allowed_keys.hpp"
//#include "assertions.hpp"
//#include "bingham.h"
#include "brute_force.hpp"
//#include "checkrgffilecontent.h"
//#include "checkxycontent.h"
//#include "data_io.h"
#include "inversion.h"
//#include "ps.h"
#include "rakhmanov.hpp"
//#include "random.hpp"
//#include "retilt.hpp"
//#include "rgf.h"
#include "structs.h"
#include "yamaji.hpp"

using namespace std;

vector <BRUTEFORCE_RESULT> st_YAMAJI (const vector <GDB>& inGDB, const INPSET& inset) {

	vector <VCTR> CNTRVCTR = return_rakhmanov_points(256);

	vector <double> ANGVCTR = generate_angle_vector_180 (0.0, 180.0, 16);

	vector <double> PHIVCTR = generate_phi_vector (0.0, 1.0, 15);

	size_t counter = 0;

	vector <BRUTEFORCE_RESULT> OUT;

	for (size_t i = 0; i < inGDB.size() - 3; i++) {
		for (size_t j = i + 1; j < inGDB.size() - 2; j++) {
			for (size_t k = j + 1; k < inGDB.size() - 1; k++) {
				for (size_t l = k + 1; l < inGDB.size(); l++) {

					vector <GDB> tempGDB;

					tempGDB.push_back(inGDB.at(i));
					tempGDB.push_back(inGDB.at(j));
					tempGDB.push_back(inGDB.at(k));
					tempGDB.push_back(inGDB.at(l));

					cout << i << " " << j << " " << k << " " << l << endl;

					//vector <BRUTEFORCE_RESULT> BR_RAW = BRUTEFORCE_ENGINE(tempGDB, CNTRVCTR, ANGVCTR, PHIVCTR, inset);

					//BR_RAW = return_minimum_misfits (BR_RAW, 1);

					//OUT.push_back(BR_RAW.at(0));

					counter++;
				}
			}
		}
		cout << counter << endl;
	}

	//cout << counter << endl;
	return OUT;
}
