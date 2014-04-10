// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef KAALSBEEK_HPP_
#define KAALSBEEK_HPP_

#include <vector>

#include "structs.h"

using namespace std;

vector <VCTR> generate_arc (const size_t& SEG_CNT, const size_t& ARC_CNT, const size_t& POINTS_DISTANCE);

vector <vector <VCTR> > generate_segment (const size_t& SEG_CNT, const size_t& POINTS_DISTANCE);

vector <vector <vector <VCTR> > > generate_net (const size_t& POINTS_DISTANCE);

vector <TRIANGLE>  generate_net_count (const vector <GDB>& inGDB, const vector <vector <vector <VCTR> > >& NET, const INPSET& inset);

vector <GRID_CENTER> generate_triangle_center (const vector <TRIANGLE>& net);

vector <GRID_CENTER> reduce_triangle_center (const vector <GRID_CENTER>& in);

vector <TRIANGLE> merge_triangle (vector <TRIANGLE> target, vector <TRIANGLE> record);

VCTR create_offnet_point (VCTR A, VCTR B);

vector <TRIANGLE> generate_triangle_offnet(vector <vector <vector <VCTR> > > net, size_t SEG_CNT);

vector <TRIANGLE> generate_triangle_in_arc (vector <vector <vector <VCTR> > > net, size_t SEG_CNT, size_t ARC_CNT);

vector <TRIANGLE> generate_triangle_in_arc_II (vector <vector <vector <VCTR> > > net, size_t SEG_CNT, size_t ARC_CNT);

vector <TRIANGLE> generate_triangle_between_arcs (vector <vector <vector <VCTR> > > net, size_t SEG_CNT, size_t ARC_CNT);

vector <TRIANGLE> generate_central_triangles (vector <vector <vector <VCTR> > > net, size_t SEG_CNT);

vector <TRIANGLE> generate_triangle_in_segment (vector <vector <vector <VCTR> > > net, size_t SEG_CNT);

vector <TRIANGLE> generate_triangle (vector <vector <vector <VCTR> > > net) ;

vector <TRIANGLE> convert_S_W_net (vector <TRIANGLE> in, INPSET inset);

vector <TRIANGLE> increase_triange_density (vector <TRIANGLE> in);

bool is_data_in_triangle (TRIANGLE in, VCTR D);

bool is_neighbouring_internal_triange (TRIANGLE inTRI, TRIANGLE offTRI);

vector <TRIANGLE> add_external_to_internal (vector <TRIANGLE> innet, TRIANGLE offnet);

bool is_computing_for_dipdir_bearing (const INPSET& I);
bool is_computing_for_strike_bearing (const INPSET& I);
bool is_computing_for_planenormal_bearing (const INPSET& I);
bool is_computing_for_striaebearing_bearing (const INPSET& I);

vector <TRIANGLE> return_count_in_net (const vector <GDB>& inGDB, const INPSET& inset, vector <TRIANGLE>& innet);

void dbg_cout_triangle (string method, VCTR A, VCTR B, VCTR C, size_t SC1, size_t AC1, size_t PC1, size_t SC2, size_t AC2, size_t PC2, size_t SC3, size_t AC3, size_t PC3);

void dbg_test_triangle_points_direction (VCTR A, VCTR B, VCTR C, string msg);

void dbg_cout_triangle_coordinates (vector <TRIANGLE> in);

void dbg_cout_kaalsbeek_ps (vector <TRIANGLE> in);

#endif
