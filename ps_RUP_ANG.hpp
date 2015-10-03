
// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef PS_RUP_ANG_HPP_
#define PS_RUP_ANG_HPP_

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "rup_clustering.hpp"
#include "structs.h"
#include "valley_method.hpp"

using namespace std;

bool by_border (const BRDR& x, const BRDR& y);
vector <BRDR> sort_by_border (const vector <BRDR>& in);

double return_datamin (const vector <GDB>& inGDB, const string field);
double return_datamax (const vector <GDB>& inGDB, const string field);

size_t return_count_max (const vector <GDB>& inGDB, const string& method, const size_t bin_number);

size_t return_step (const double DATA_max);

void ps_draw_histogram_bars (const vector <GDB>& inGDB, const vector <line_RUP_ANG>& L_R_A, ofstream& o, const CENTER& center, const PAPER& P, const double DATA_min, const double DATA_max, const size_t bin_number, const double binsize, const string method);

void ps_RUP_ANG_header (ofstream& o, const CENTER& center, const PAPER& P);
void ps_percentage (ofstream& o, const CENTER& center, const PAPER& P, const string METHOD, const double DATA_max);
void ps_percentage_max (ofstream& o, const CENTER& center, const PAPER& P, const string METHOD, const double DATA_max);

vector <line_RUP_ANG> generate_graph_histogram (const vector <HISTOGRAM>& H, const vector <VALLEY>& V, const double DATA_MAX);

#endif
