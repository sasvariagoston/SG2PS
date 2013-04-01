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

size_t RUP_number_in_range (vector <GDB> inGDB, double range_min, double range_max);

RUP_table return_cost_function_member (vector <GDB> inGDB, size_t bin_number);

bool by_RUP(const GDB& x, const GDB& y);

bool by_C(const RUP_table& x, const RUP_table& y);

vector <GDB> sort_by_RUP (vector <GDB> inGDB);

vector <RUP_table> sort_by_C (vector <RUP_table> RT);

int return_RUP_clusters (vector <GDB > inGDB);

void dbg_cout_GDB_RUP (vector <GDB> inGDB);

void dbg_cout_RUP_table (vector <RUP_table> RT);

#endif
