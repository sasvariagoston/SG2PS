// Copyright (C) 2012- 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef AVERAGE_HPP_
#define AVERAGE_HPP_

#include <vector>

#include "structs.h"

VCTR N_to_S (VCTR in);
VCTR N_to_D (VCTR in);

bool is_overturned (GDB in);

bool is_bedding_present (vector <GDB> to_process);

bool is_plane_dataset_singular (vector <GDB> inGDB, string METHOD);

bool is_datatype_processable_for_average (vector <GDB> inGDB);
bool is_processable_for_average_MT2 (vector <GDB> inGDB);
bool is_processable_for_average_EQ2 (vector <GDB> inGDB);
bool is_processable_for_average_EQ1 (vector <GDB> inGDB);
bool is_processable_for_average_HMG (vector <GDB> inGDB);

vector <GDB> generate_Bingham_dataset (vector <GDB> inGDB, string method);

VCTR process_for_average_MT2 (vector <GDB> inGDB, string method);
VCTR process_for_average_EQ2 (vector <GDB> inGDB);
VCTR process_for_average_EQ1 (vector <GDB> inGDB);

vector <GDB> init_average (vector <GDB> inGDB);

vector <size_t> generate_block_length (vector <GDB> inGDB, string METHOD);

VCTR calculate_bedding_average_vector (vector <GDB> to_process);
VCTR calculate_data_average_vector (vector <GDB> to_process, string method);

vector <GDB> DATATYPE_AVERAGE (vector <GDB> inGDB, vector <size_t> length_container, string METHOD);

vector <GDB> cGc_average (vector <GDB> inGDB);

void dbg_averages (vector <GDB> inGDB);


#endif
