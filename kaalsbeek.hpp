// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef KAALSBEEK_HPP_
#define KAALSBEEK_HPP_

using namespace std;

vector <VCTR> generate_arc (size_t SEG_CNT, size_t ARC_CNT);

vector <vector <VCTR> > generate_segment (size_t SEG_CNT);

vector <vector <vector <VCTR> > > generate_net ();


vector <TRIANGLE> merge_triangle (vector <TRIANGLE> target, vector <TRIANGLE> record);

VCTR create_offnet_point (VCTR A, VCTR B);

vector <TRIANGLE> generate_triangle_offnet(vector <vector <vector <VCTR> > > net, size_t SEG_CNT);

vector <TRIANGLE> generate_triangle_in_arc (vector <vector <vector <VCTR> > > net, size_t SEG_CNT, size_t ARC_CNT);

vector <TRIANGLE> generate_triangle_in_arc_II (vector <vector <vector <VCTR> > > net, size_t SEG_CNT, size_t ARC_CNT);

vector <TRIANGLE> generate_triangle_between_arcs (vector <vector <vector <VCTR> > > net, size_t SEG_CNT, size_t ARC_CNT);

vector <TRIANGLE> generate_central_triangles (vector <vector <vector <VCTR> > > net, size_t SEG_CNT);

vector <TRIANGLE> generate_triangle_in_segment (vector <vector <vector <VCTR> > > net, size_t SEG_CNT);

vector <TRIANGLE> generate_triangle (vector <vector <vector <VCTR> > > net) ;

vector <TRIANGLE> convert_S_W_net (vector <TRIANGLE> in);


void dbg_cout_triangle (string method, VCTR A, VCTR B, VCTR C, size_t SC1, size_t AC1, size_t PC1, size_t SC2, size_t AC2, size_t PC2, size_t SC3, size_t AC3, size_t PC3);

void dbg_cout_triangle_coordinates (vector <TRIANGLE> in);

void dbg_cout_kaalsbeek_ps (vector <TRIANGLE> in);

#endif
