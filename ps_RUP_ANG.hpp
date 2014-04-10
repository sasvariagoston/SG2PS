// Copyright (C) 2012 - 2014 Ágoston Sasvári
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
vector <BRDR> sort_by_border (vector <BRDR> in);

double return_datamin (vector <GDB> inGDB, string field);
double return_datamax (vector <GDB> inGDB, string field);

size_t return_count_max (vector <GDB> inGDB, string method, size_t bin_number);

double return_step (double DATA_max);

void ps_draw_histogram_bars (vector <GDB> inGDB, vector <line_RUP_ANG> L_R_A, INPSET inset, ofstream& o, CENTER center, PAPER P, double DATA_min, double DATA_max, size_t bin_number, double binsize, string method);

void ps_RUP_ANG_header (ofstream& o, CENTER center, PAPER P, string method);
void ps_percentage (ofstream& o, CENTER center, PAPER P, string method, double DATA_max);
void ps_percentage_max (ofstream& o, CENTER center, PAPER P, string method, double DATA_max);

vector <line_RUP_ANG> generate_graph_histogram (vector <HISTOGRAM> H, vector <VALLEY> V, double DATA_MAX);

#endif
