// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef INVERSION_H
#define INVERSION_H

#include "common.h"
#include "structs.h"

using namespace std;

bool is_method_BINGHAM (vector <GDB> inGDB, INPSET inset);
bool is_method_FRY (vector <GDB> inGDB, INPSET inset);
bool is_method_MICHAEL (vector <GDB> inGDB, INPSET inset);
bool is_method_SHAN (vector <GDB> inGDB, INPSET inset);
bool is_method_ANGELIER (vector <GDB> inGDB, INPSET inset);
bool is_method_MOSTAFA (vector <GDB> inGDB, INPSET inset);
bool is_method_NDA (vector <GDB> inGDB, INPSET inset);
bool is_method_BRUTEFORCE (vector <GDB> inGDB, INPSET inset);
bool is_method_PTN (vector <GDB> inGDB, INPSET inset);

size_t useful_striae_number (vector <GDB> inGDB);

vector <GDB> return_striae_with_offset (vector <GDB> inGDB);

vector <GDB> return_stressvector_estimators (STRESSTENSOR st, vector <GDB> inGDB, string method, bool compression_positive);

vector <GDB> generate_virtual_striae (vector <GDB> inGDB);

string inversion_method (vector <GDB> inGDB, INPSET inset);

void ps_inversion (string method, STRESSTENSOR st, STRESSFIELD sf, vector <GDB> inGDB, vector <VALLEY> V, INPSET inset, ofstream& o, CENTER center, CENTER mohr_center, PAPER P);

void bingham_result_output (STRESSFIELD sf);

void inversion_result_output (STRESSFIELD sf, double average_misfit);

vector <GDB> inversion (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, CENTER mohr_center, PAPER P);

#endif
