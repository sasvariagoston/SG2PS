// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef HOMOGENITY_CHECK_HPP_
#define HOMOGENITY_CHECK_HPP_

#include "structs.h"

using namespace std;

size_t minimum_independent_dataset ();

bool check_dataset_geometry_homogenity (const vector <GDB>& inGDB);

bool check_dataset_offset_homogenity (const vector <GDB>& inGDB);

bool check_dataset_homogenity (const vector <GDB>& inGDB);

vector <GDB> return_GDB_with_no_homogeneous_data (const vector <GDB>& inGDB);

bool has_inhomogeneous_enough (const vector <GDB>& inGDB);

#endif
