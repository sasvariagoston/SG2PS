
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <vector>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "common.h"
#include "kaalsbeek.hpp"
#include "settings.hpp"

using namespace std;

vector <VCTR> generate_arc (const size_t SEG_CNT, const size_t ARC_CNT, const size_t POINTS_DISTANCE) {

	VCTR buf;
	vector <VCTR> arc;

	ASSERT2(POINTS_DISTANCE <= 9, "size_t POINTS_DISTANCE variable must be less than 10!")

	DIPDIR_DIP DD;

	DD.DIPDIR = (SEG_CNT * 60.0) + 30.0 ;

	if (DD.DIPDIR > 360.0) DD.DIPDIR = DD.DIPDIR - 360.0;

	DD.DIP = 90.0 - (10.0  * POINTS_DISTANCE) + (POINTS_DISTANCE * ARC_CNT);

	size_t cnt_max = 10 - ARC_CNT;

	double step = 60.0 / cnt_max;

	for (size_t p_cnt = 0; p_cnt < cnt_max; p_cnt++) {

		buf = DXDYDZ_from_dipdir_dip(DD);

		arc.push_back(buf);

		DD.DIPDIR = DD.DIPDIR + step;
	}

	return arc;
}

vector <vector <VCTR> > generate_segment (const size_t SEG_CNT, const size_t POINTS_DISTANCE) {

	vector <VCTR > buf;
	vector <vector <VCTR> > out;

	for (size_t arc_cnt = 0; arc_cnt < 10; arc_cnt++) {

		buf = generate_arc(SEG_CNT, arc_cnt, POINTS_DISTANCE);

		out.push_back(buf);
	}

	return out;
}

vector <vector <vector <VCTR> > > generate_net (const size_t POINTS_DISTANCE) {

	vector <vector <vector <VCTR> > > NET;

	for (size_t seg_cnt = 0; seg_cnt < 6; seg_cnt++) {

		vector <vector <VCTR> > buf;

		buf = generate_segment(seg_cnt, POINTS_DISTANCE);

		NET.push_back(buf);
	}
	return NET;
}

vector <TRIANGLE>  generate_net_count (const vector <GDB>& inGDB, const vector <vector <vector <VCTR> > >& NET) {

	vector <vector <VCTR> > buf;

	vector <TRIANGLE> TRI = generate_triangle (NET);

	TRI = return_count_in_net (inGDB, TRI);

	return TRI;
}

vector <GRID_CENTER> generate_triangle_center (const vector <TRIANGLE>& net) {

	vector <GRID_CENTER> out;

	for (size_t i = 0; i < net.size(); i++) {

		GRID_CENTER buf;

		buf.CENTER.X = (net.at(i).A.X + net.at(i).B.X + net.at(i).C.X) / 3.0;
		buf.CENTER.Y = (net.at(i).A.Y + net.at(i).B.Y + net.at(i).C.Y) / 3.0;
		buf.CENTER.Z = (net.at(i).A.Z + net.at(i).B.Z + net.at(i).C.Z) / 3.0;

		buf.COUNT = net.at(i).COUNT;

		out.push_back(buf);
	}

	return out;
}

vector <GRID_CENTER> reduce_triangle_center (const vector <GRID_CENTER>& in) {

	vector <GRID_CENTER> out;

	for (size_t i = 0; i < in.size(); i++) {

		if (in.at(i).COUNT > 0) out.push_back(in.at(i));
		else {}
	}
	return out;
}

vector <TRIANGLE> merge_triangle (vector <TRIANGLE>& target, const vector <TRIANGLE>& record) {

	TRIANGLE buf;

	for (size_t i = 0; i < record.size(); i++) {

		buf = record.at(i);

		target.push_back(buf);
	}
	return target;
}

VCTR create_offnet_point (const VCTR& A, const VCTR& B) {

	VCTR out;

	out.X = ((A.X + B.X) / 2.0) * 1.083;
	out.Y = ((A.Y + B.Y) / 2.0) * 1.083;
	out.Z = 999.99;

	return out;
}

//6
vector <TRIANGLE> generate_triangle_offnet(const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT) {

	TRIANGLE buf;
	vector <TRIANGLE> out;

	for (size_t p_cnt = 0; p_cnt < 9; p_cnt++) {

		buf.A = net.at(SEG_CNT).at(0).at(p_cnt + 0);
		buf.B = net.at(SEG_CNT).at(0).at(p_cnt + 1);
		buf.C = create_offnet_point (buf.A, buf.B);

		buf.GROUP = 6;
		buf.COUNT = 0;

		//generates CCW triangles
		//dbg_test_triangle_points_direction (buf.A, buf.B, buf.C, "6th arc");
		//dbg_cout_triangle ("6 - OFFNET", buf.A, buf.B, buf.C, SEG_CNT, 0, p_cnt + 0, SEG_CNT, 0, p_cnt + 1, 33, 33, 33);

		out.push_back(buf);
	}

	return out; // OK
}

//7
vector <TRIANGLE> generate_triangle_offnet_between_segments (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT) {

	TRIANGLE buf;
	vector <TRIANGLE> out;

	size_t SEG_L = 0;
	size_t SEG_U = 0;

	if (SEG_CNT == 5) {

		SEG_L = SEG_CNT;
		SEG_U = 0;
	}
	else {

		SEG_L = SEG_CNT;
		SEG_U = SEG_CNT + 1;
	}

	buf.A = net.at(SEG_L).at(0).at(9);
	buf.B = net.at(SEG_U).at(0).at(0);
	buf.C = create_offnet_point (buf.A, buf.B);

	buf.GROUP = 7;
	buf.COUNT = 0;

	//dbg_test_triangle_points_direction (buf.A, buf.B, buf.C, "7th arc");
	//dbg_cout_triangle ("7 - OFF, BTW SEG", buf.A, buf.B, buf.C, SEG_L, 0, 9, SEG_U, 0, 0, 55, 55, 55);

	out.push_back(buf);

	return out; // OK
}

vector <TRIANGLE> generate_triangle_in_arc (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT, const size_t ARC_CNT) {

	TRIANGLE buf;
	vector <TRIANGLE> out;

	size_t cnt_max = 10 - ARC_CNT - 1;

	for (size_t p_cnt = 0; p_cnt < cnt_max; p_cnt++) {

		buf.A = net.at(SEG_CNT).at(ARC_CNT + 0).at(p_cnt + 1);
		buf.B = net.at(SEG_CNT).at(ARC_CNT + 0).at(p_cnt + 0);
		buf.C = net.at(SEG_CNT).at(ARC_CNT + 1).at(p_cnt + 0);

		buf.GROUP = 1;
		buf.COUNT = 0;

		//dbg_test_triangle_points_direction (buf.A, buf.B, buf.C, "1st arc");
		//dbg_cout_triangle ("1 - IN_ARC_I", buf.A, buf.B, buf.C, SEG_CNT, ARC_CNT + 0, p_cnt + 1, SEG_CNT, ARC_CNT + 0, p_cnt + 0, SEG_CNT, ARC_CNT + 1, p_cnt + 0);

		out.push_back(buf);
	}

	return out; //OK
}

vector <TRIANGLE> generate_triangle_in_arc_II (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT, const size_t ARC_CNT) {

	TRIANGLE buf;
	vector <TRIANGLE> out;

	size_t cnt_max = 10 - ARC_CNT - 2;

	for (size_t p_cnt = 0; p_cnt < cnt_max; p_cnt++) {

		buf.A = net.at(SEG_CNT).at(ARC_CNT + 0).at(p_cnt + 1);
		buf.B = net.at(SEG_CNT).at(ARC_CNT + 1).at(p_cnt + 0);
		buf.C = net.at(SEG_CNT).at(ARC_CNT + 1).at(p_cnt + 1);

		buf.GROUP = 2;
		buf.COUNT = 0;

		//dbg_test_triangle_points_direction (buf.A, buf.B, buf.C, "2nd arc");
		//dbg_cout_triangle ("2 - IN_ARC_II",	buf.A, buf.B, buf.C, SEG_CNT, ARC_CNT + 0, p_cnt + 1, SEG_CNT, ARC_CNT + 1, p_cnt + 0, SEG_CNT, ARC_CNT + 1, p_cnt + 1);

		out.push_back(buf);
	}

	return out; // OK
}

vector <TRIANGLE> generate_triangle_between_arcs (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT, const size_t ARC_CNT) {

	TRIANGLE buf;
	vector <TRIANGLE> out;

	size_t p_max = net.at(SEG_CNT).at(ARC_CNT).size() - 1;

	size_t SEG_L = 0;
	size_t SEG_U = 0;

	if (SEG_CNT == 5) {

		SEG_L = SEG_CNT;
		SEG_U = 0;
	}
	else {

		SEG_L = SEG_CNT;
		SEG_U = SEG_CNT + 1;
	}

	buf.A = net.at(SEG_U).at(ARC_CNT + 0).at(0);
	buf.B = net.at(SEG_L).at(ARC_CNT + 0).at(p_max);
	buf.C = net.at(SEG_U).at(ARC_CNT + 1).at(0);

	buf.GROUP = 3;
	buf.COUNT = 0;

	//dbg_test_triangle_points_direction (buf.A, buf.B, buf.C, "3rd arc");
	//dbg_cout_triangle ("3 - BTW_ARC_I",	buf.A, buf.B, buf.C, SEG_U, ARC_CNT + 0, 0, SEG_L, ARC_CNT + 0, p_max, SEG_U, ARC_CNT + 1, 0);

	out.push_back(buf);

	buf.A = net.at(SEG_U).at(ARC_CNT + 1).at(0);
	buf.B = net.at(SEG_L).at(ARC_CNT + 0).at(p_max);
	buf.C = net.at(SEG_L).at(ARC_CNT + 1).at(p_max - 1);

	buf.GROUP = 4;
	buf.COUNT = 0;

	//dbg_test_triangle_points_direction (buf.A, buf.B, buf.C, "4th arc");
	//dbg_cout_triangle ("4 - BTW_ARC_II", buf.A, buf.B, buf.C, SEG_U, ARC_CNT + 1, 0, SEG_L, ARC_CNT + 0, p_max, SEG_L, ARC_CNT + 1, p_max - 1);

	out.push_back(buf);

	return out; // OK
}

vector <TRIANGLE> generate_central_triangles (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT) {

	TRIANGLE buf;
	vector <TRIANGLE> out;

	size_t SEG_L = 0;
	size_t SEG_U = 0;

	if (SEG_CNT == 5) {

		SEG_L = SEG_CNT;
		SEG_U = 0;
	}
	else {

		SEG_L = SEG_CNT;
		SEG_U = SEG_CNT + 1;
	}

	buf.A = net.at(SEG_U).at(9).at(0);
	buf.B = net.at(SEG_L).at(9).at(0);
	buf.C = declare_vector (0.0, 0.0, 1.0);

	buf.GROUP = 5;
	buf.COUNT = 0;

	//dbg_test_triangle_points_direction (buf.A, buf.B, buf.C, "5th arc");
	//dbg_cout_triangle ("5 - CENTRAL", buf.A, buf.B, buf.C, SEG_U, 9, 0, SEG_L, 9, 0, 11, 11, 11);

	out.push_back(buf);

	return out; // OK
}

vector <TRIANGLE> generate_triangle_in_segment (const vector <vector <vector <VCTR> > >& net, const size_t SEG_CNT) {

	vector <TRIANGLE> buf;
	vector <TRIANGLE> out;

	for (size_t arc_cnt = 0; arc_cnt < 9; arc_cnt++) {

		//1:
		buf = generate_triangle_in_arc(net, SEG_CNT, arc_cnt);
		out = merge_triangle(out, buf);

		//2:
		buf = generate_triangle_in_arc_II(net, SEG_CNT, arc_cnt);
		out = merge_triangle(out, buf);

		//3 + 4:
		buf = generate_triangle_between_arcs (net, SEG_CNT, arc_cnt);// 3+4
		out = merge_triangle(out, buf);
	}

	return out;
}

vector <TRIANGLE> generate_triangle (const  vector <vector <vector <VCTR> > >& net) {

	vector <TRIANGLE> buf;
	vector <TRIANGLE> out;

	for (size_t seg_cnt = 0; seg_cnt < 6; seg_cnt++) {

		// 1 + 2 + 3 + 4
		buf = generate_triangle_in_segment (net, seg_cnt);
		out = merge_triangle(out, buf);

		//5:
		buf = generate_central_triangles (net, seg_cnt);
		out = merge_triangle(out, buf);

		//6
		buf = generate_triangle_offnet(net, seg_cnt);
		out = merge_triangle(out, buf);

		//7
		buf =  generate_triangle_offnet_between_segments (net, seg_cnt);
		out = merge_triangle(out, buf);
	}

	return out;
}

vector <TRIANGLE> convert_S_W_net (vector <TRIANGLE>& in) {

	for (size_t i = 0; i < in.size(); i++) {

		if (in.at(i).GROUP < 6) {

			if (is_NET_WULFF()) {

				if (in.at(i).A.Z < 1.00) {

					in.at(i).A.X = in.at(i).A.X / (1.00 - in.at(i).A.Z);
					in.at(i).A.Y = in.at(i).A.Y / (1.00 - in.at(i).A.Z);
				}

				if (in.at(i).B.Z < 1.00) {

					in.at(i).B.X = in.at(i).B.X / (1.00 - in.at(i).B.Z);
					in.at(i).B.Y = in.at(i).B.Y / (1.00 - in.at(i).B.Z);
				}

				if (in.at(i).C.Z < 1.00) {

					in.at(i).C.X = in.at(i).C.X / (1.00 - in.at(i).C.Z);
					in.at(i).C.Y = in.at(i).C.Y / (1.00 - in.at(i).C.Z);
				}
			}
			else {

				if (in.at(i).A.Z < 1.00) {

					in.at(i).A.X = in.at(i).A.X / sqrt (1.00 - in.at(i).A.Z);
					in.at(i).A.Y = in.at(i).A.Y / sqrt (1.00 - in.at(i).A.Z);
				}

				if (in.at(i).B.Z < 1.00) {

					in.at(i).B.X = in.at(i).B.X / sqrt (1.00 - in.at(i).B.Z);
					in.at(i).B.Y = in.at(i).B.Y / sqrt (1.00 - in.at(i).B.Z);
				}

				if (in.at(i).C.Z < 1.00) {

					in.at(i).C.X = in.at(i).C.X / sqrt (1.00 - in.at(i).C.Z);
					in.at(i).C.Y = in.at(i).C.Y / sqrt (1.00 - in.at(i).C.Z);
				}
			}
		}
	}

	return in;
}

vector <TRIANGLE> increase_triange_density (const vector <TRIANGLE>& in) {

	vector <TRIANGLE> out;
	TRIANGLE buf;

	for (size_t i = 0; i < in.size(); i++) {

		buf.GROUP = in.at(i).GROUP;
		buf.A = in.at(i).A;
		buf.B = declare_vector (
				(in.at(i).A.X + in.at(i).B.X) / 2.0,
				(in.at(i).A.Y + in.at(i).B.Y) / 2.0,
				(in.at(i).A.Z + in.at(i).B.Z) / 2.0
				);
		buf.C = declare_vector (
				(in.at(i).A.X + in.at(i).C.X) / 2.0,
				(in.at(i).A.Y + in.at(i).C.Y) / 2.0,
				(in.at(i).A.Z + in.at(i).C.Z) / 2.0
		);
		out.push_back(buf);


		buf.A = in.at(i).B;
		buf.B = declare_vector (
				(in.at(i).B.X + in.at(i).C.X) / 2.0,
				(in.at(i).B.Y + in.at(i).C.Y) / 2.0,
				(in.at(i).B.Z + in.at(i).C.Z) / 2.0
		);
		buf.C = declare_vector (
				(in.at(i).A.X + in.at(i).B.X) / 2.0,
				(in.at(i).A.Y + in.at(i).B.Y) / 2.0,
				(in.at(i).A.Z + in.at(i).B.Z) / 2.0
		);
		out.push_back(buf);

		buf.A = in.at(i).C;
		buf.B = declare_vector (
				(in.at(i).C.X + in.at(i).A.X) / 2.0,
				(in.at(i).C.Y + in.at(i).A.Y) / 2.0,
				(in.at(i).C.Z + in.at(i).A.Z) / 2.0
		);
		buf.C = declare_vector (
				(in.at(i).B.X + in.at(i).C.X) / 2.0,
				(in.at(i).B.Y + in.at(i).C.Y) / 2.0,
				(in.at(i).B.Z + in.at(i).C.Z) / 2.0
		);
		out.push_back(buf);

		buf.A = declare_vector (
				(in.at(i).A.X + in.at(i).B.X) / 2.0,
				(in.at(i).A.Y + in.at(i).B.Y) / 2.0,
				(in.at(i).A.Z + in.at(i).B.Z) / 2.0
		);
		buf.B = declare_vector (
				(in.at(i).B.X + in.at(i).C.X) / 2.0,
				(in.at(i).B.Y + in.at(i).C.Y) / 2.0,
				(in.at(i).B.Z + in.at(i).C.Z) / 2.0
		);
		buf.C = declare_vector (
				(in.at(i).A.X + in.at(i).C.X) / 2.0,
				(in.at(i).A.Y + in.at(i).C.Y) / 2.0,
				(in.at(i).A.Z + in.at(i).C.Z) / 2.0
		);
		out.push_back(buf);
	}

	return out;
}

bool is_data_in_triangle (const TRIANGLE& in, const VCTR& D) {

	VCTR A = in.A;
	VCTR B = in.B;
	VCTR C = in.C;

	bool B1 = ((D.X - B.X) * (A.Y - B.Y) - (A.X - B.X) * (D.Y - B.Y) < 0.0);
	bool B2 = ((D.X - C.X) * (B.Y - C.Y) - (B.X - C.X) * (D.Y - C.Y) < 0.0);
	bool B3 = ((D.X - A.X) * (C.Y - A.Y) - (C.X - A.X) * (D.Y - A.Y) < 0.0);

	return (B1 == B2 && B2 == B3);
}

bool is_neighbouring_internal_triange (const TRIANGLE& inTRI, const TRIANGLE& offTRI) {

	size_t fit_counter = 0;

	if (points_distance (inTRI.A, offTRI.A) < 10e-4) fit_counter++;
	if (points_distance (inTRI.A, offTRI.B) < 10e-4) fit_counter++;
	if (points_distance (inTRI.A, offTRI.C) < 10e-4) fit_counter++;
	if (points_distance (inTRI.B, offTRI.A) < 10e-4) fit_counter++;
	if (points_distance (inTRI.B, offTRI.B) < 10e-4) fit_counter++;
	if (points_distance (inTRI.B, offTRI.C) < 10e-4) fit_counter++;
	if (points_distance (inTRI.C, offTRI.A) < 10e-4) fit_counter++;
	if (points_distance (inTRI.C, offTRI.B) < 10e-4) fit_counter++;
	if (points_distance (inTRI.C, offTRI.C) < 10e-4) fit_counter++;

	if (fit_counter == 2) return true;
	else return false;
}

vector <TRIANGLE> add_external_to_internal (vector <TRIANGLE>& innet, const TRIANGLE& offnet) {

	for (size_t i = 0; i < innet.size(); i++) {

		if (is_neighbouring_internal_triange (innet.at(i), offnet)) innet.at(i).COUNT++;
	}

	return innet;
}

vector <TRIANGLE> return_count_in_net (const vector <GDB>& inGDB, vector <TRIANGLE>& innet) {

	for (size_t i = 0; i < innet.size(); i++) {

		size_t counter = 0;

		bool DIPDIR = is_CONTOURING_DIPDIR_BEARING();
		bool STRIKE = is_CONTOURING_STRIKEDIR_BEARING();
		bool NORMAL = is_CONTOURING_PLANE_NORMAL_BEARING();
		bool STRIAE = is_CONTOURING_STRIAE_BEARING_BEARING();

		for (size_t j = 0; j < inGDB.size(); j++) {

			VCTR TO_PROCESS;

			if (DIPDIR) TO_PROCESS = inGDB.at(j).D;

			else if (STRIKE) {

				DIPDIR_DIP DD = dipdir_dip_from_DXDYDZ(inGDB.at(j).D);

				if (is_in_range (0.0, 90.0, DD.DIPDIR)) DD.DIPDIR = DD.DIPDIR + 270.0;
				else DD.DIPDIR = DD.DIPDIR - 90.0;

				DD.DIP = 0.0;

				TO_PROCESS = DXDYDZ_from_dipdir_dip(DD);
			}
			else if (NORMAL) {

				DIPDIR_DIP DD = dipdir_dip_from_DXDYDZ(inGDB.at(j).D);

				if (DD.DIPDIR < 180.0) DD.DIPDIR = DD.DIPDIR + 180.0;
				else DD.DIPDIR = DD.DIPDIR - 180.0;

				DD.DIP = 90.0 - DD.DIP;

				TO_PROCESS = DXDYDZ_from_dipdir_dip(DD);
			}
			else if (STRIAE) {

				if (is_allowed_striae_datatype(inGDB.at(j).DATATYPE)) TO_PROCESS = inGDB.at(j).DC;
				else TO_PROCESS = inGDB.at(j).D;
			}
			else ASSERT_DEAD_END();


			if (is_data_in_triangle(innet.at(i), TO_PROCESS)) {

				if (innet.at(i).GROUP < 6) counter++;
				else innet = add_external_to_internal (innet, innet.at(i));
			}
		}

		innet.at(i).COUNT = counter;
	}
	return innet;
}

void dbg_cout_triangle (const string method, const VCTR& A, const VCTR& B, const VCTR& C, const size_t SC1, const size_t AC1, const size_t PC1, const size_t SC2, const size_t AC2, const size_t PC2, const size_t SC3, const size_t AC3, const size_t PC3) {

	cout << fixed << setprecision(4) << endl;

	cout
	<< method << ":" << '\t'
	<< A.X << '\t' << A.Y << '\t'
	<< B.X << '\t' << B.Y << '\t'
	<< C.X << '\t' << C.Y << '\t'
	<< SC1 << AC1 << PC1 << '\t'
	<< SC2 << AC2 << PC2 << '\t'
	<< SC3 << AC3 << PC3 << endl;
}

void dbg_test_triangle_points_direction (const VCTR& A, const VCTR& B, const VCTR& C, const string msg) {

	TRIANGLE T;

	T.A = A;
	T.B = B;
	T.C = C;

	VCTR pole = declare_vector(-2.0, -2.0, NaN());

	ASSERT2(!is_data_in_triangle(T, pole), msg);

	return;
}

void dbg_cout_triangle_coordinates (const vector <TRIANGLE>& in) {

	//660 triangles are in the normal Kaalsbeek grid

	cout << fixed << setprecision(3) << flush;

	cout
	<< "GROUP" 		<< '\t' << "|" << '\t'
	<< "  VCTR A" 	<< '\t' << "|" << '\t'
	<< "  VCTR B" 	<< '\t' << "|" << '\t'
	<< "  VCTR C" 	<< '\t' << "|" << '\t'
	<< "  COUNT" << endl;

	for (size_t i = 0; i < in.size(); i++) {

		cout
		<< in.at(i).GROUP << '\t' << "|" << '\t'
		<< in.at(i).A.X << '\t' << in.at(i).A.Y << '\t' << "|" << '\t'
		<< in.at(i).B.X << '\t' << in.at(i).B.Y << '\t' << "|" << '\t'
		<< in.at(i).C.X << '\t' << in.at(i).C.Y << '\t' << "|" << '\t'
		<< in.at(i).COUNT
		<< endl;
	}
}

void dbg_cout_kaalsbeek_ps (const vector <TRIANGLE>& in) {

	string filename = "KAALSBEEK.PS";

	ofstream o(filename.c_str());

	o << "%!PS-Adobe-3.0 EPSF-3.0" << endl;
	o << "%%BoundingBox:  0 0 440 440" << endl;

	for (size_t i = 0; i < in.size(); i++) {

		o
		<< "  newpath  "
		<< in.at(i).A.X * 200.0 + 220.0 << "  " << in.at(i).A.Y * 200.0 + 220.0 << " moveto "
		<< in.at(i).B.X * 200.0 + 220.0 << "  " << in.at(i).B.Y * 200.0 + 220.0 << " lineto" << endl;

		o
		<< in.at(i).C.X * 200.0 + 220.0 << "  " << in.at(i).C.Y * 200.0 + 220.0 << " lineto" << endl;

		if 		(in.at(i).GROUP == 1) o << " 0.50 0.50 0.50 " << flush;
		else if (in.at(i).GROUP == 2) o << " 0.50 0.50 1.00 " << flush;
		else if (in.at(i).GROUP == 3) o << " 1.00 0.50 0.83 " << flush;
		else if (in.at(i).GROUP == 4) o << " 1.00 0.50 0.50 " << flush;
		else if (in.at(i).GROUP == 5) o << " 1.00 0.75 0.50 " << flush;
		else if (in.at(i).GROUP == 6) o << " 1.00 1.00 0.50 " << flush;
		else if (in.at(i).GROUP == 7) o << " 0.50 1.00 0.50 " << flush;

		o << " setrgbcolor fill stroke " << endl;

		o << " 1 0 0 setrgbcolor 0.5 setlinewidth " << endl;

		o
		<< "  newpath  "
		<< in.at(i).A.X * 200.0 + 220.0 << "  " << in.at(i).A.Y * 200.0 + 220.0 << " moveto "
		<< in.at(i).B.X * 200.0 + 220.0 << "  " << in.at(i).B.Y * 200.0 + 220.0 << " lineto" << endl;

		o
		<< in.at(i).C.X * 200.0 + 220.0 << "  " << in.at(i).C.Y * 200.0 + 220.0 << " lineto " << endl;

		o
		<< in.at(i).A.X * 200.0 + 220.0 << "  " << in.at(i).A.Y * 200.0 + 220.0 << " lineto stroke" << endl;

		double X = ((in.at(i).A.X + in.at(i).B.X + in.at(i).C.X) / 3.0) * 200 + 220;
		double Y = ((in.at(i).A.Y + in.at(i).B.Y + in.at(i).C.Y) / 3.0) * 200 + 220;

		o << "/ArialNarrow-Bold findfont 4 scalefont setfont" << endl;

		o << " 0 0 0 setrgbcolor " << endl;

		o
		<< "  " << X - 2<< " " << Y - 2 << " moveto ("
		<< in.at(i).COUNT << ") show "<< endl;
	}
}
