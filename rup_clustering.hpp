// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef RUP_CLUSTERING_HPP_
#define RUP_CLUSTERING_HPP_

#include "common.h"

using namespace std;

vector <double> GDB_to_table (const vector <GDB>& inGDB, const string field);

size_t DATA_number_in_range (const vector <double>& in, const double range_min, const double range_max);

double bin_size_for_DATA (const vector <double>& in, const size_t bin_number);

RUP_table return_cost_function_member (const vector <double>& in, const size_t bin_number);

size_t return_DATA_ideal_bin_number (const vector <double>& in);

vector <VALLEY> return_valleygraph_for_dataset (const vector <GDB>& inGDB, const string field);

vector <GDB> associate_GDB_DATA_clusters (const vector <GDB>& inGDB, const vector <VALLEY>& V, const string method);

vector <GDB> apply_RUP_ANG_CLUSTERING_result (const vector <GDB>& inGDB);

void dbg_cout_GDB_RUP (vector <GDB> inGDB);
void dbg_cout_IN_RUP (vector <double> in);
void dbg_cout_RUP_table (vector <RUP_table> RT);

#endif
