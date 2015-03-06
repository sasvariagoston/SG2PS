
// Copyright (C) 2012- 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef AVERAGE_HPP_
#define AVERAGE_HPP_

#include <vector>

#include "structs.h"

VCTR N_to_S (const VCTR& in);
VCTR N_to_D (const VCTR& in);

bool is_overturned (const GDB& in);
bool is_bedding_present (const vector <GDB>& to_process);
bool is_plane_dataset_singular (const vector <GDB>& inGDB);
bool is_datatype_processable_for_average (const string DT);
bool is_processable_for_average_MT2 (const vector <GDB>& inGDB);
bool is_processable_for_average_EQ2 (const vector <GDB>& inGDB);
bool is_processable_for_average_EQ1 (const vector <GDB>& inGDB);
bool is_processable_for_average_HMG (const vector <GDB>& inGDB);

vector <GDB> generate_Bingham_dataset (const vector <GDB>& inGDB);

VCTR process_for_average_MT2 (const vector <GDB>& inGDB);
VCTR process_for_average_EQ2 (const vector <GDB>& inGDB);
VCTR process_for_average_EQ1 (const vector <GDB>& inGDB);

vector <GDB> init_average (const vector <GDB>& inGDB);

VCTR calculate_data_average_vector (const vector <GDB>& to_process);
vector <GDB> apply_data_average_vector (const vector <GDB>& to_process, const VCTR& AV_D, const string MODE);

vector <GDB> DATATYPE_AVERAGE (const vector <GDB>& inGDB, const string MODE);

vector < vector <GDB> > calculate_average_for_groups (const vector <vector <GDB> >& inGDB_G);

bool has_group_bedding (const vector <vector <GDB> >& inGDB_G);

VCTR return_group_bedding_vector (const vector <vector <GDB> >& inGDB_G);

vector <vector <GDB> > apply_group_bedding_vector (const vector <vector <GDB> >& inGDB_G, const VCTR AV_D);

vector < vector < vector < vector <GDB> > > > AVERAGE (const vector < vector < vector < vector <GDB> > > >& inGDB_G);

void dbg_averages (const vector <GDB>& inGDB);

#endif
