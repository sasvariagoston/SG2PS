// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef VALLEY_METHOD_HPP_
#define VALLEY_METHOD_HPP_

vector <HISTOGRAM> generate_RUP_histogram (vector <GDB> inGDB, size_t bin_number);

vector <VALLEY> generate_valley_graph (vector <HISTOGRAM> H);

vector <VALLEY> reduce_N_valley_graph (vector <VALLEY> inV);

vector <VALLEY> reduce_UD_valley_graph (vector <VALLEY> inV);

size_t return_DU_number (vector <VALLEY> inV);

size_t return_ideal_clusternumber (vector <GDB> inGDB, size_t bin_number);

void dbg_cout_H (vector <HISTOGRAM> inH);

void dbg_cout_V (vector <VALLEY> inV);

#endif
