// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef RUP_CLUSTERING_HPP_
#define RUP_CLUSTERING_HPP_

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include "structs.h"

using namespace std;

vector <double> GDB_to_table (vector <GDB> inGDB, string field);

bool is_in_range (double range_min, double range_max, double in);

size_t DATA_number_in_range (vector <double> in, double range_min, double range_max);

double bin_size_for_DATA (vector <double> in, size_t bin_number);

RUP_table return_cost_function_member (vector <double> in, size_t bin_number);

bool by_RUP(const GDB& x, const GDB& y);
bool by_ANG(const GDB& x, const GDB& y);
bool by_C(const RUP_table& x, const RUP_table& y);

vector <GDB> sort_by_RUP (vector <GDB> inGDB);
vector <GDB> sort_by_ANG (vector <GDB> inGDB);
vector <RUP_table> sort_by_C (vector <RUP_table> RT);

size_t return_DATA_ideal_bin_number (vector <double> in);

vector <VALLEY> return_valleygraph_for_dataset (vector <GDB> inGDB, string field);

void dbg_cout_GDB_RUP (vector <GDB> inGDB);
void dbg_cout_IN_RUP (vector <double> in);
void dbg_cout_RUP_table (vector <RUP_table> RT);

vector <GDB> associate_GDB_RUP_clusters (vector <GDB> inGDB, vector <VALLEY> V);

#endif
