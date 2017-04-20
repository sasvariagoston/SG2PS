// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef VALLEY_METHOD_HPP_
#define VALLEY_METHOD_HPP_

vector <HISTOGRAM> generate_DATA_histogram (const vector <double>& in, const size_t bin_number);

vector <VALLEY> generate_valley_graph (const vector <HISTOGRAM>& H);

vector <VALLEY> reduce_N_valley_graph (const vector <VALLEY>& inV);

vector <VALLEY> create_valley_graph (const vector <VALLEY>& V);

vector <VALLEY> return_valleys (const vector <double>& in, const size_t bin_number);

void dbg_cout_H (vector <HISTOGRAM> inH);

void dbg_cout_V (vector <VALLEY> inV);

bool by_COUNT(const HISTOGRAM& x, const HISTOGRAM& y);

vector <HISTOGRAM> sort_by_COUNT (const vector <HISTOGRAM>& H);

#endif
