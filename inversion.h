
// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef INVERSION_H
#define INVERSION_H

#include "common.h"
#include "structs.h"

using namespace std;

vector <GDB> return_striae_with_offset (const vector <GDB>& inGDB);

vector <GDB > generate_virtual_striae (const vector <GDB>& inGDB_G);

void cout_inversion_results (const vector <GDB>& inGDB, const vector <STRESSFIELD>& SFV);

vector <STRESSTENSOR> return_STV ();
vector <STRESSFIELD> return_SFV ();
void INVERSION (const vector <GDB>& inGDB);

void cout_dbg_sf (const vector <GDB>& inGDB);

vector <GDB> ASSOCIATE_STV_SFV (const vector <GDB>& inGDB, const vector <STRESSTENSOR>& STV, const vector <STRESSFIELD>& SFV);

vector <GDB> apply_inversion_result (const vector <GDB>& inGDB, const STRESSTENSOR st);

void cout_dbg_stressfield (const STRESSFIELD& sf);
void cout_dbg_stressfield (const vector <STRESSFIELD>& sf);

void cout_dbg_stresstensor (const STRESSTENSOR& in);
void cout_dbg_STV (const vector <STRESSTENSOR>& STV);

#endif
