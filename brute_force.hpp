// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef BRUTE_FORCE_HPP_
#define BRUTE_FORCE_HPP_

#include "common.h"
#include "structs.h"

using namespace std;

vector <double> generate_angle_vector_180 (const double& ANG_MIN, const double& ANG_MAX, const size_t& INT_NUM);
vector <double> generate_phi_vector (const double& PHI_MIN, const double& PHI_MAX, const size_t& INT_NUM);

vector <vector <double> > DIR_MX1_from_n1 (const VCTR& n1, const double& angle);
vector <vector <double> > st_from_reduced_stresstensor (const vector <vector <double> >& DIR_MX1, const double& fi);

vector <VCTR> generate_centroids_net (const VCTR& ORIGO, const size_t POINTS_DISTANCE);

bool by_MISFIT_ANGLE (const BRUTEFORCE_RESULT& x, const BRUTEFORCE_RESULT& y);

vector <BRUTEFORCE_RESULT> return_minimum_misfits (vector <BRUTEFORCE_RESULT> IN, size_t records_number);

STRESSTENSOR calculate_stresstensor (const VCTR& n1, double ANG, double PHI);

vector <BRUTEFORCE_RESULT> BRUTEFORCE_ENGINE (const vector <GDB>& inGDB, const vector <VCTR>& CNTRVCTR, const vector <double>& ANGVCTR, const vector <double>& PHIVCTR);

STRESSTENSOR st_BRUTEFORCE (const vector <GDB>& inGDB);

STRESSFIELD sf_BRUTEFORCE (STRESSTENSOR& st);

void dbg_cout_matrix (vector <vector <double> > I);

#endif
