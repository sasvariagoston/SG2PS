
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

////#include <algorithm>
#include <cmath>
//#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "assertions.hpp"
#include "common.h"
//#include "rup_clustering.hpp"
#include "structs.h"
//#include "valley_method.hpp"

using namespace std;

vector <VCTR> return_rakhmanov_points (size_t N) {

	N = N + 1;

	const size_t N_ORIG = N;

	const size_t N_MAX = N * 2;

	const double SQRT_N = sqrt(static_cast<double>(N_MAX));

	const double pi = 3.14159265359;

	const double C = 3.6;

	vector <VCTR> OUT;

	double phi_prev = 0.0;

	//cout << fixed << setprecision(6) << endl;

	for (size_t i = 1; i < N_ORIG + 1; i++) {

		double h = -1 + 2.0 * (i - 1) / (N_MAX - 1);

		double teta = acos (h);

		double phi = 0.0;

		if (i > 1) {

			phi = phi_prev + (C / SQRT_N) * 1.0 / sqrt (1.0 - h * h);

			phi = fmod (phi, (2.0 * pi));
		}

		VCTR buf = declare_vector(
				sin(teta) * cos(phi),
				sin(teta) * sin(phi),
				cos(teta));

		if (i > 1) OUT.push_back(buf);

		phi_prev = phi;
	}
	return OUT;  //CHECKED, OK
}
