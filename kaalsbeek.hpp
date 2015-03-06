
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef KAALSBEEK_HPP_
#define KAALSBEEK_HPP_

#include <vector>

#include "structs.h"

using namespace std;

vector <VCTR> generate_arc (const size_t SEG_CNT, const size_t ARC_CNT, const size_t POINTS_DISTANCE);

vector <vector <VCTR> > generate_segment (const size_t SEG_CNT, const size_t POINTS_DISTANCE);

vector <vector <vector <VCTR> > > generate_net (const size_t POINTS_DISTANCE);

vector <TRIANGLE>  generate_net_count (const vector <GDB>& inGDB, const vector <vector <vector <VCTR> > >& NET);

vector <GRID_CENTER> generate_triangle_center (const vector <TRIANGLE>& net);

vector <GRID_CENTER> reduce_triangle_center (const vector <GRID_CENTER>& in);

vector <TRIANGLE> merge_triangle (vector <TRIANGLE>& target, const vector <TRIANGLE>& record);

VCTR create_offnet_point (const VCTR& A, const VCTR& B);

vector <TRIANGLE> generate_triangle_offnet(const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT);

vector <TRIANGLE> generate_triangle_in_arc (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT, const size_t ARC_CNT);

vector <TRIANGLE> generate_triangle_in_arc_II (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT, const size_t ARC_CNT);

vector <TRIANGLE> generate_triangle_between_arcs (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT, const size_t ARC_CNT);

vector <TRIANGLE> generate_central_triangles (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT);

vector <TRIANGLE> generate_triangle_in_segment (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT);

vector <TRIANGLE> generate_triangle (const vector <vector <vector <VCTR> > >& net) ;

vector <TRIANGLE> convert_S_W_net (vector <TRIANGLE>& in);

vector <TRIANGLE> increase_triange_density (const vector <TRIANGLE>& in);

bool is_data_in_triangle (const TRIANGLE& in, const VCTR& D);

bool is_neighbouring_internal_triange (const TRIANGLE& inTRI, const TRIANGLE& offTRI);

vector <TRIANGLE> add_external_to_internal (vector <TRIANGLE>& innet, const TRIANGLE& offnet);

vector <TRIANGLE> return_count_in_net (const vector <GDB>& inGDB, vector <TRIANGLE>& innet);

void dbg_cout_triangle (string method, const VCTR& A, const VCTR& B, const VCTR& C, const size_t SC1, const size_t AC1, const size_t PC1, const size_t SC2, const size_t AC2, const size_t PC2, const size_t SC3, const size_t AC3, const size_t PC3);

void dbg_test_triangle_points_direction (const VCTR& A, const VCTR& B, const VCTR& C, const string msg);

void dbg_cout_triangle_coordinates (const vector <TRIANGLE>& in);

void dbg_cout_kaalsbeek_ps (const vector <TRIANGLE>& in);

#endif
