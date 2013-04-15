// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef INVERSION_H
#define INVERSION_H

/*#include "angelier.h"
#include "assertions.hpp"
#include "bingham.h"*/
#include "common.h"
/*#include "fry.h"
#include "inversion.h"
#include "michael.h"
#include "mostafa.h"
#include "nda.h"
#include "ps.h"
#include "ptn.h"
#include "rgf.h"
#include "shan.h"*/
#include "structs.h"

using namespace std;

size_t useful_striae_number (vector <GDB> inGDB);

vector <GDB> return_striae_with_offset (vector <GDB> inGDB);

vector <GDB> return_stressvector_estimators (STRESSTENSOR st, vector <GDB> inGDB, string method, bool compression_positive);

vector <GDB> generate_virtual_striae (vector <GDB> inGDB);

void ps_inversion (string method, STRESSTENSOR st, STRESSFIELD sf, vector <GDB> inGDB, ofstream& o, CENTER center, CENTER mohr_center, PAPER P);

void bingham_result_output (STRESSFIELD sf);

void inversion_result_output (STRESSFIELD sf, double average_misfit);

vector <GDB> inversion (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, CENTER mohr_center, PAPER P);

#endif
