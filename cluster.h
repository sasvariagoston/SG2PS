// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef CLUSTER_HPP_
#define CLUSTER_HPP_

#include <string>
#include <vector>

#include "structs.h"

vector <size_t> init_whichgroup_uni (const size_t cluster_number, const vector <GDB>& inGDB);
vector <size_t> init_whichgroup_rnd (const size_t cluster_number, const vector <GDB>& inGDB);

vector <CENTR_VECT> init_centroid_vector (const size_t cluster_number);
vector <CENTR_VECT> compute_centroid_from_which_group (const size_t cluster_number, const vector <size_t>& which_group, const vector <GDB>& inGDB);

double compute_distance (const CENTR_VECT& centroid, const GDB& in);
vector <vector <double> > compute_distance_matrix_from_centroid (const vector <GDB>& inGDB, const vector <CENTR_VECT>& centroid);

vector <size_t> compute_whichgroup_from_distances (const vector <vector <double> >& distance_matrix);

double cumulative_distance (const vector <vector <double> >& distance_matrix, const vector <size_t>& which_group);

vector <vector <double> > clustering_cycle (const size_t cluster_number, const vector <GDB>& inGDB);
vector <GDB> k_means_clustering (const size_t cluster_number, const vector <GDB>& inGDB);
vector <GDB> K_MEANS (const vector <GDB>& inGDB);

void dbg_distance_matrix (const vector <vector <double> >& distance_matrix);
void dbg_whichgroup (const vector <size_t>& whichgroup);
void dbg_centroid_vector (const vector <CENTR_VECT>& centroid);

#endif
