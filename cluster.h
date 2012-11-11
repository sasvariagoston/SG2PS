// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef CLUSTER_HPP_
#define CLUSTER_HPP_

#include <string>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstdlib>


#include "structs.h"


vector <CENTR_VECT>  init_centriod (size_t cluster_number, vector <GDB> inGDB);
vector <vector <double> > init_distance_matrix (size_t cluster_number, vector <GDB> inGDB);
vector <int> init_whichgroup (size_t cluster_number, vector <GDB> inGDB);
vector <int> init_whichgroup_II (size_t cluster_number, vector <GDB> inGDB);

double compute_distance (CENTR_VECT centroid, GDB inGDB);

vector <CENTR_VECT> compute_centroid_from_which_group (size_t cluster_number, vector <int> which_group, vector <GDB> inGDB);
vector <vector <double> > compute_distance_matrix_from_centroid (vector <vector <double> > distance_matrix, vector <GDB> inGDB, vector <CENTR_VECT> centroid);
vector <int> compute_whichgroup_from_distances (vector <vector <double> > distance_matrix);

vector <GDB> attach_group_codes (vector <int> which_group, vector <GDB> inGDB);

double cumulative_distance (vector <vector <double> > distance_matrix, vector <int> which_group);

vector <vector <double> > clustering_cycle (size_t cluster_number, vector <GDB> inGDB, INPSET i);
size_t autoclustering_cycle (size_t cluster_number, vector <GDB> inGDB);

vector <GDB> k_means_clustering (size_t cluster_number, vector <GDB> inGDB,  INPSET i);
vector <GDB> k_means (INPSET i, vector <GDB> inGDB);

#endif
