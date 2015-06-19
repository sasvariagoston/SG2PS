
// Copyright (C) 2012 - 2015 Ágoston Sasvári
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

vector <double> GDB_to_table (const vector <GDB>& inGDB, const string field);//ok

size_t DATA_number_in_range (const vector <double>& in, const double range_min, const double range_max);//ok

double bin_size_for_DATA (const vector <double>& in, const size_t bin_number);//ok

RUP_table return_cost_function_member (const vector <double>& in, const size_t bin_number);//ok

//bool by_RUP(const GDB& x, const GDB& y);//ok
//bool by_ANG(const GDB& x, const GDB& y);//ok
//bool by_C(const RUP_table& x, const RUP_table& y);//ok

//vector <GDB> sort_by_RUP (const vector <GDB>& inGDB);//ok
//vector <GDB> sort_by_ANG (const vector <GDB>& inGDB);//ok
//vector <RUP_table> sort_by_C (const vector <RUP_table>& RT);//ok

size_t return_DATA_ideal_bin_number (const vector <double>& in);//ok

vector <VALLEY> return_valleygraph_for_dataset (const vector <GDB>& inGDB, const string field);//ok

vector <GDB> associate_GDB_DATA_clusters (const vector <GDB>& inGDB, const vector <VALLEY>& V, const string method);//ok

vector <GDB> apply_RUP_ANG_CLUSTERING_result (const vector <GDB>& inGDB);//ok

void dbg_cout_GDB_RUP (vector <GDB> inGDB);
void dbg_cout_IN_RUP (vector <double> in);
void dbg_cout_RUP_table (vector <RUP_table> RT);

#endif
