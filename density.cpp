
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "assertions.hpp"
#include "common.h"
#include "filename.hpp"
#include "homogenity_check.hpp"
#include "paper.hpp"
#include "run_mode.h"
#include "rgf.h"
#include "kaalsbeek.hpp"
#include "density.h"
#include "ps.h"
#include "settings.hpp"
#include "standard_output.hpp"
#include "structs.h"

vector < vector <GRID_CENTER> > generate_rectangular_grid_from_triange_center (const size_t cell_number) {

	vector < vector <GRID_CENTER> > out;

	double step = 2.1 / cell_number;

	double min = -1.05 + (step / 2.0);

	double counter_Y = min;

	for (size_t i = 0; i < cell_number; i++) {

		double counter_X = min;

		vector <GRID_CENTER> grid_buf;

		for (size_t j = 0; j < cell_number; j++) {

			GRID_CENTER buf;

			buf.CENTER.X = counter_X;
			buf.CENTER.Y = counter_Y;
			buf.COUNT = 0;

			grid_buf.push_back(buf);

			counter_X = counter_X + step;
		}

		counter_Y = counter_Y + step;

		out.push_back (grid_buf);
	}
	return out;
}

double return_grid_size (const vector < vector <GRID_CENTER> >& rect_grid) {

	double A1 = rect_grid.at(0).at(1).CENTER.X ;
	double A2 = rect_grid.at(0).at(0).CENTER.X ;

	return (A1 - A2);
}

vector < vector <GRID_CENTER> > calculate_grid_cell_values_from_triangle (vector < vector <GRID_CENTER> >& rect_grid, const vector <GRID_CENTER>& tri_center) {

	double GRID_SIZE = return_grid_size(rect_grid);

	size_t cell_number = rect_grid.at(0).size();

	for (size_t k = 0; k < cell_number; k++) {

		for (size_t j = 0; j < cell_number; j++) {

			double cml_value = 0.0;
			double distance = 0.0;

			double R_X = rect_grid.at(k).at(j).CENTER.X;
			double R_Y = rect_grid.at(k).at(j).CENTER.Y;

			for (size_t i = 0; i < tri_center.size(); i++) {

				double T_X = tri_center.at(i).CENTER.X;
				double T_Y = tri_center.at(i).CENTER.Y;

				distance = sqrt((R_X - T_X) * (R_X - T_X) + (R_Y - T_Y) * (R_Y - T_Y));

				if (distance < 0.0) ASSERT_DEAD_END()
				else if (is_in_range (0.0, 1.0, distance)) {}
				else if (is_in_range (1.0, 2.0, distance)) distance = 2.0 - distance;
				else if (is_in_range (2.0, 3.0, distance)) distance = 0.0;
				else ASSERT_DEAD_END();

				if (distance < (GRID_SIZE / 5.0)) distance = GRID_SIZE;

				cml_value = cml_value + (tri_center.at(i).COUNT / (distance));
			}
			const bool IN_RECT_CELL = (sqrt((R_X * R_X) + (R_Y * R_Y)) <= 1.0);

			if (IN_RECT_CELL) rect_grid.at(k).at(j).COUNT = cml_value;
			else {}
		}
	}
	return rect_grid;
}

size_t return_RECT_GRID_max_count (const vector < vector <GRID_CENTER> >& RECT_GRID) {

	size_t MAX = 0.0;

	for (size_t i = 0; i < RECT_GRID.size(); i++) {
		for (size_t j = 0; j < RECT_GRID.at(i).size(); j++) {

			if (RECT_GRID.at(i).at(j).COUNT > MAX) MAX = RECT_GRID.at(i).at(j).COUNT;
		}
	}
	return MAX;
}

vector <double> return_isoline_percentages (const double C_MN, const double C_MX) {

	vector <double> ISOLINE;

	double STEP = 0.1;

	double REC = 0.0;

	for (size_t i = 0; REC < C_MX; i++) {

		REC = C_MN + i * STEP;

		ISOLINE.push_back(REC);
	}
	return ISOLINE;
}

vector < vector <GRID_CENTER> > generate_empty_binary_rect_grid (const size_t cell_number) {

	vector < vector <GRID_CENTER> > out;

	for (size_t j = 0; j < cell_number; j++) {

		vector <GRID_CENTER > buf_row;

		for (size_t i = 0; i < cell_number; i++) {

			GRID_CENTER  buf;
			buf.COUNT = 0;
			buf.CENTER.X = NaN();
			buf.CENTER.Y = NaN();
			buf.CENTER.Z = NaN();
			buf_row.push_back(buf);
		}

		out.push_back(buf_row);
	}
	return out;
}

vector < vector <GRID_CENTER> > generate_binary_rect_grid (const vector < vector <GRID_CENTER> >& rect_grid, const size_t COUNT) {

	size_t cell_number = rect_grid.at(0).size();

	ASSERT2((rect_grid.size() == rect_grid.at(0).size()), "Error in binary rectangular grid while contouring");

	vector < vector <GRID_CENTER> > bin_grid = generate_empty_binary_rect_grid (cell_number);

	for (size_t j = 0; j < cell_number; j++) {
		for (size_t i = 0; i < cell_number; i++) {

			size_t ACT_CNT = rect_grid.at(j).at(i).COUNT;

			if (ACT_CNT > COUNT) 	bin_grid.at(j).at(i).COUNT = 1;
			else 					bin_grid.at(j).at(i).COUNT = 0;
		}
	}
	return bin_grid;
}

vector < vector <GRID_CENTER> > generate_marching_squares (const vector < vector <GRID_CENTER> >& bin_grid) {

	size_t cell_number = bin_grid.at(0).size() - 1;

	vector < vector <GRID_CENTER> > m_sq = generate_empty_binary_rect_grid (cell_number);

	for (size_t j = 0; j < cell_number; j++) {

		for (size_t i = 0; i < cell_number; i++) {

			double AX = bin_grid.at(j + 1).at(i + 0).CENTER.X;
			double BX = bin_grid.at(j + 1).at(i + 1).CENTER.X;
			double CX = bin_grid.at(j + 0).at(i + 1).CENTER.X;
			double DX = bin_grid.at(j + 0).at(i + 0).CENTER.X;

			double AY = bin_grid.at(j + 1).at(i + 0).CENTER.Y;
			double BY = bin_grid.at(j + 1).at(i + 1).CENTER.Y;
			double CY = bin_grid.at(j + 0).at(i + 1).CENTER.Y;
			double DY = bin_grid.at(j + 0).at(i + 0).CENTER.Y;

			size_t A_CNT = bin_grid.at(j + 1).at(i + 0).COUNT;
			size_t B_CNT = bin_grid.at(j + 1).at(i + 1).COUNT;
			size_t C_CNT = bin_grid.at(j + 0).at(i + 1).COUNT;
			size_t D_CNT = bin_grid.at(j + 0).at(i + 0).COUNT;

			m_sq.at(j).at(i).CENTER.X = (AX + BX + CX + DX) / 4.0;
			m_sq.at(j).at(i).CENTER.Y = (AY + BY + CY + DY) / 4.0;

			m_sq.at(j).at(i).COUNT = A_CNT * 1 + B_CNT * 2 + C_CNT * 4 + D_CNT * 8;
		}
	}
	return m_sq;
}

vector < vector <GRID_CENTER> > check_saddle (vector < vector <GRID_CENTER> >& m_sq, const vector < vector <GRID_CENTER> >& rect_grid, const double isoline) {

	size_t cell_number = m_sq.at(0).size();

	for (size_t j = 0; j < cell_number; j++) {

		for (size_t i = 0; i < cell_number; i++) {

			size_t SQ_ID = m_sq.at(j).at(i).COUNT;

			size_t A_CNT = rect_grid.at(j + 1).at(i + 0).COUNT;
			size_t B_CNT = rect_grid.at(j + 1).at(i + 1).COUNT;
			size_t C_CNT = rect_grid.at(j + 0).at(i + 1).COUNT;
			size_t D_CNT = rect_grid.at(j + 0).at(i + 0).COUNT;

			double SADDLE_CNT = (A_CNT + B_CNT + C_CNT + D_CNT) / 4.0;

			if (SQ_ID == 5) {

				if (SADDLE_CNT > isoline) 	m_sq.at(j).at(i).COUNT = 5;
				else 						m_sq.at(j).at(i).COUNT = 10;
			}
			else if (SQ_ID == 10) {

				if (SADDLE_CNT > isoline) 	m_sq.at(j).at(i).COUNT = 10;
				else 						m_sq.at(j).at(i).COUNT = 5;
			}
			else {}//ok
		}
	}
	return m_sq;
}

vector <LINE> return_line_from_m_sq_number (const vector <vector <GRID_CENTER> >& m_sq, const vector <vector <GRID_CENTER> >& rect_grid, const size_t j, const size_t i, const size_t isoline) {

	vector <LINE> out;

	LINE buf;

	size_t m_sq_id = m_sq.at(j).at(i).COUNT;

	double AX = rect_grid.at(j + 1).at(i + 0).CENTER.X;
	double BX = rect_grid.at(j + 1).at(i + 1).CENTER.X;
	double CX = rect_grid.at(j + 0).at(i + 1).CENTER.X;
	double DX = rect_grid.at(j + 0).at(i + 0).CENTER.X;

	double AY = rect_grid.at(j + 1).at(i + 0).CENTER.Y;
	double BY = rect_grid.at(j + 1).at(i + 1).CENTER.Y;
	double CY = rect_grid.at(j + 0).at(i + 1).CENTER.Y;
	double DY = rect_grid.at(j + 0).at(i + 0).CENTER.Y;

	double AW = rect_grid.at(j + 1).at(i + 0).COUNT;
	double BW = rect_grid.at(j + 1).at(i + 1).COUNT;
	double CW = rect_grid.at(j + 0).at(i + 1).COUNT;
	double DW = rect_grid.at(j + 0).at(i + 0).COUNT;

	double W = 0.5;
	double U = 0.5;
	double INTERVAL = 0.0;

	if (m_sq_id == 0) {}
	else if (m_sq_id == 1) {

		INTERVAL = fabs(AW - BW);
		U = fabs((AW - isoline) / INTERVAL);
		W = fabs((BW - isoline) / INTERVAL);
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 2) {

		INTERVAL = fabs(AW - BW);
		U = fabs(AW - isoline) / INTERVAL;
		W = fabs(BW - isoline) / INTERVAL;
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.B.X = (BX * W + CX * U);
		buf.B.Y = (BY * W + CY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 3) {

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.A.X = (BX * W + CX * U);
		buf.A.Y = (BY * W + CY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 4) {

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.A.X = (BX * W + CX * U);
		buf.A.Y = (BY * W + CY * U);

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.B.X = (CX * W + DX * U);
		buf.B.Y = (CY * W + DY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 5) {

		INTERVAL = fabs(AW - BW);
		U = fabs(AW - isoline) / INTERVAL;
		W = fabs(BW - isoline) / INTERVAL;
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.B.X = (BX * W + CX * U);
		buf.B.Y = (BY * W + CY * U);

		out.push_back(buf);

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.A.X = (CX * W + DX * U);
		buf.A.Y = (CY * W + DY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 6) {

		INTERVAL = fabs(AW - BW);
		U = fabs(AW - isoline) / INTERVAL;
		W = fabs(BW - isoline) / INTERVAL;
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.B.X = (CX * W + DX * U);
		buf.B.Y = (CY * W + DY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 7) {

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.A.X = (CX * W + DX * U);
		buf.A.Y = (CY * W + DY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 8) {

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.A.X = (CX * W + DX * U);
		buf.A.Y = (CY * W + DY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 9) {

		INTERVAL = fabs(AW - BW);
		U = fabs(AW - isoline) / INTERVAL;
		W = fabs(BW - isoline) / INTERVAL;
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.B.X = (CX * W + DX * U);
		buf.B.Y = (CY * W + DY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 10) {

		INTERVAL = fabs(AW - BW);
		U = fabs(AW - isoline) / INTERVAL;
		W = fabs(BW - isoline) / INTERVAL;
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.A.X = (BX * W + CX * U);
		buf.A.Y = (BY * W + CY * U);

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.B.X = (CX * W + DX * U);
		buf.B.Y = (CY * W + DY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 11) {

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.A.X = (BX * W + CX * U);
		buf.A.Y = (BY * W + CY * U);

		INTERVAL = fabs(CW - DW);
		U = fabs(CW - isoline) / INTERVAL;
		W = fabs(DW - isoline) / INTERVAL;
		buf.B.X = (CX * W + DX * U);
		buf.B.Y = (CY * W + DY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 12) {

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.A.X = (BX * W + CX * U);
		buf.A.Y = (BY * W + CY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 13) {

		INTERVAL = fabs(AW - BW);
		U = fabs(AW - isoline) / INTERVAL;
		W = fabs(BW - isoline) / INTERVAL;
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(BW - CW);
		U = fabs(BW - isoline) / INTERVAL;
		W = fabs(CW - isoline) / INTERVAL;
		buf.B.X = (BX * W + CX * U);
		buf.B.Y = (BY * W + CY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 14) {

		INTERVAL = fabs(AW - BW);
		U = fabs(AW - isoline) / INTERVAL;
		W = fabs(BW - isoline) / INTERVAL;
		buf.A.X = (AX * W + BX * U);
		buf.A.Y = (AY * W + BY * U);

		INTERVAL = fabs(DW - AW);
		U = fabs(DW - isoline) / INTERVAL;
		W = fabs(AW - isoline) / INTERVAL;
		buf.B.X = (DX * W + AX * U);
		buf.B.Y = (DY * W + AY * U);

		out.push_back(buf);
	}
	else if (m_sq_id == 15) {}
	else ASSERT_DEAD_END();

	return out;
}

vector <LINE> manage_points_outside (const vector <LINE>& L) {

	vector <LINE> OUT;

	if (L.size() < 1 && L.size() > 2) ASSERT_DEAD_END();

	for (size_t i = 0; i < L.size(); i++) {

		LINE ACT_L = L.at(i);

		bool A_IN = is_point_in_circle(ACT_L.A);
		bool B_IN = is_point_in_circle(ACT_L.B);

		bool IN  = (A_IN && B_IN);
		bool CRS = (A_IN || B_IN);
		bool OSD = (!A_IN && !B_IN);

		if (!IN && !OSD && !CRS) ASSERT_DEAD_END();

		if (IN) OUT.push_back(ACT_L);
		else if (CRS) {

			if  (A_IN) {

				VCTR NEW = interpolate_between_points (ACT_L.A, ACT_L.B);
				ACT_L.B = NEW;
				OUT.push_back (ACT_L);
			}
			else if (B_IN) {

				VCTR NEW = interpolate_between_points (ACT_L.A, ACT_L.B);
				ACT_L.A = NEW;
				OUT.push_back (ACT_L);
			}
			else {}
		}
		else {}
	}
	return OUT;
}

vector <LINE> generate_raw_lines (const vector <vector <GRID_CENTER> >& m_sq, const vector <vector <GRID_CENTER> >& rect_grid, const double isoline) {

	vector <LINE> out;

	size_t cell_number = m_sq.at(0).size();

	ASSERT2((m_sq.size() == m_sq.at(0).size()), "Incorrect marching square database: not rectangular");

	for (size_t j = 0; j < cell_number; j++) {
		for (size_t i = 0; i < cell_number; i++) {

			vector <LINE> buf;

			size_t CNT = m_sq.at(j).at(i).COUNT;

			if (CNT != 0 && CNT != 15) {

				buf = return_line_from_m_sq_number (m_sq, rect_grid, j, i, isoline);
			}

			buf = manage_points_outside (buf);

			out.insert(out.end(), buf.begin(), buf.end());
		}
	}
	return out;
}

vector <vector <LINE> > generate_line_vector (const vector <LINE>& in) {

	vector <vector <LINE> > out;

	for (size_t i = 0; i < in.size(); i++) {

		LINE buf = in.at(i);

		vector <LINE> row_buf;

		row_buf.push_back(buf);

		out.push_back(row_buf);
	}
	return out;
}

vector <LINE> turn_off_record (vector <LINE>& in) {

	for (size_t i = 0; i < in.size(); i++) {

		in.at(i).A.X = 999.99;
		in.at(i).A.Y = 999.99;
		in.at(i).B.X = 999.99;
		in.at(i).B.Y = 999.99;
	}
	return in;
}

vector <LINE> flip_line (const vector <LINE>& L) {

	vector <LINE> out = L;

	for (size_t i = 0; i < L.size(); i++) {

		LINE buf;

		buf.B.X = L.at(i).A.X;
		buf.B.Y = L.at(i).A.Y;
		buf.A.X = L.at(i).B.X;
		buf.A.Y = L.at(i).B.Y;

		out.at(L.size() - 1 - i) = buf;
	}
	return out;
}

vector <vector <LINE> > connect_vectors (vector <vector <LINE> >& LV) {

	size_t change_counter = 0;

	do {

		change_counter = 0;

		for (size_t j = 0; j < LV.size() - 1; j++) {
			for (size_t k = j + 1; k < LV.size(); k++) {

				size_t J_SIZE = LV.at(j).size();
				size_t K_SIZE = LV.at(k).size();

				VCTR J_FRST = LV.at(j).at(0).A;
				VCTR K_FRST = LV.at(k).at(0).A;

				VCTR J_LAST = LV.at(j).at(J_SIZE - 1).B;
				VCTR K_LAST = LV.at(k).at(K_SIZE - 1).B;

				bool HAS_VALUE = (
						(J_FRST.X < 900.0) &&
						(K_FRST.X < 900.0) &&
						(J_LAST.X < 900.0) &&
						(K_LAST.X < 900.0));

				bool FIRST_FIRST =
						is_in_range (J_FRST.X, J_FRST.X, K_FRST.X) &&
						is_in_range (J_FRST.Y, J_FRST.Y, K_FRST.Y);

				bool FIRST_LAST =
						is_in_range (J_FRST.X, J_FRST.X, K_LAST.X) &&
						is_in_range (J_FRST.Y, J_FRST.Y, K_LAST.Y);

				bool LAST_FIRST =
						is_in_range (J_LAST.X, J_LAST.X, K_FRST.X) &&
						is_in_range (J_LAST.Y, J_LAST.Y, K_FRST.Y);

				bool LAST_LAST =
						is_in_range (J_LAST.X, J_LAST.X, K_LAST.X) &&
						is_in_range (J_LAST.Y, J_LAST.Y, K_LAST.Y);

				if (HAS_VALUE && (FIRST_FIRST || FIRST_LAST || LAST_FIRST || LAST_LAST)) {

					if (FIRST_FIRST) LV.at(j) = flip_line (LV.at(j));

					else if (FIRST_LAST) {

						LV.at(j) = 	flip_line (LV.at(j));
						LV.at(k) = 	flip_line (LV.at(k));
					}
					else if (LAST_FIRST) {}
					else if (LAST_LAST) LV.at(k) = flip_line (LV.at(k));

					else ASSERT_DEAD_END();

					LV.at(j).insert(LV.at(j).end(), LV.at(k).begin(), LV.at(k).end());

					LV.at(k) = turn_off_record(LV.at(k));

					change_counter++;
				}
				else LV.at(k) = flip_line (LV.at(k));
			}
		}

	} while (change_counter > 0);

	return LV;
}

vector <vector <LINE> > eliminate_empty_short_records (vector <vector <LINE> >& LV) {

	vector <vector <LINE> > OUT;

	for (size_t i = 0; i < LV.size(); i++) {

		vector <LINE> buf = LV.at(i);

		bool HAS_VALUE = LV.at(i).at(0).A.X < 900.0;

		bool LENGTH_OK = LV.at(i).size() > 4;

		if (HAS_VALUE && LENGTH_OK) OUT.push_back(buf);
	}
	return OUT;
}

vector < vector <VCTR> > eliminate_short_distances (const vector < vector <VCTR> >& BZ) {

	vector < vector <VCTR> > out;

	for (size_t i = 0; i < BZ.size(); i++) {

		vector <double> DST;

		for (size_t j = 1; j < BZ.at(i).size(); j++) {

			VCTR ACT = BZ.at(i).at(j);
			VCTR PRW = BZ.at(i).at(j - 1);

			double D = sqrt(((ACT.X - PRW.X) * (ACT.X - PRW.X)) + ((ACT.Y - PRW.Y) * (ACT.Y - PRW.Y)));

			DST.push_back(D);
		}
		if (DST.size() + 1 != BZ.at(i).size()) ASSERT_DEAD_END ();

		vector <VCTR> buf;

		for (size_t j = 0; j < BZ.at(i).size(); j++) {

			if ((j == 0) || (j == BZ.at(i).size() - 1)) {

				buf.push_back(BZ.at(i).at(j));
			}
			else {

				if (DST.at(j - 1) > 0.02) buf.push_back(BZ.at(i).at(j));
			}
		}

		if (buf.size() > 5) out.push_back(buf);
	}
	return out;
}

vector < vector <VCTR> > generate_bezier_points (const vector <vector <LINE> >& LV) {

	vector < vector <VCTR> > out;

	for (size_t i = 0; i < LV.size(); i++) {

		vector <VCTR> row_buf;

		for (size_t j = 0; j < LV.at(i).size(); j++) {

			row_buf.push_back(LV.at(i).at(j).A);

			if (j == LV.at(i).size() - 1) row_buf.push_back(LV.at(i).at(j).B);
		}

		out.push_back(row_buf);
	}
	return out;
}

bool is_point_in_circle (const VCTR& in) {

	return (sqrt((in.X * in.X) + (in.Y * in.Y)) < 1.0);
}

VCTR interpolate_between_points (const VCTR& inA, const VCTR& inB) {

	bool is_A_in = is_point_in_circle(inA);
	bool is_B_in = is_point_in_circle(inB);

	if (!(is_A_in || is_B_in)) ASSERT_DEAD_END();

	bool CLIN_X = is_in_range(inA.X, inA.X, inB.X);
	bool CLIN_Y = is_in_range(inA.Y, inA.Y, inB.Y);

	if (CLIN_X || CLIN_Y) {

		if (CLIN_X) {

			double Y = sqrt (1.0 - (inA.X * inA.X));
			bool BTW = (is_in_range(inA.Y, inB.Y, Y) || is_in_range(inA.Y, inB.Y, Y));

			if (BTW) 	return declare_vector(inA.X,  Y, NaN());
			else 		return declare_vector(inA.X, -Y, NaN());
		}
		else {

			double X = sqrt (1.0 - (inA.Y * inA.Y));
			bool BTW = (is_in_range(inA.X, inB.X, X) || is_in_range(inA.X, inB.X, X));

			if (BTW) 	return declare_vector( X, inA.Y, NaN());
			else 		return declare_vector(-X, inA.Y, NaN());
		}
	}
	double X = 0.0;
	double Y = 0.0;

	double m = (inA.Y - inB.Y) / (inA.X - inB.X);
	double b = inA.Y - m * inA.X;

	double A = 1.0 + (m * m);
	double B = 2.0 * m * b;
	double C = (b * b) - 1.0;

	vector <double> QUAD_SOL = quadratic_solution(A, B, C);

	double X1 = QUAD_SOL.at(0);
	double X2 = QUAD_SOL.at(1);

	ASSERT(!isnan(X1));
	ASSERT(!isnan(X2));

	bool X1_FIT = false;
	bool X2_FIT = false;

	if (inA.X < inB.X) 	{

		X1_FIT = is_in_range(inA.X, inB.X, X1);
		X2_FIT = is_in_range(inA.X, inB.X, X2);
	}
	else {

		X1_FIT = is_in_range(inB.X, inA.X, X1);
		X2_FIT = is_in_range(inB.X, inA.X, X2);
	}

	if (X1_FIT) 		X = X1;
	else if (X2_FIT) 	X = X2;
	else ASSERT_DEAD_END();

	Y = m * X + b;

	ASSERT(!isnan(Y));

	return declare_vector(X, Y, NaN());
}

VCTR generate_new_start (const VCTR& A) {

	double length = sqrt((A.X * A.X ) +	(A.Y * A.Y));

	VCTR out;

	out.X = A.X / length;
	out.Y = A.Y / length;
	out.Z = NaN();

	return out;
}

bool is_closed_line (const VCTR& min_A, const VCTR& max_B) {

	return (is_in_range(min_A.X, min_A.X, max_B.X) && is_in_range(min_A.Y, min_A.Y, max_B.Y));
}

vector <VCTR> flip_BZ_line (const vector <VCTR>& BZ) {

	vector <VCTR> out = BZ;

	for (size_t i = 0; i < BZ.size(); i++) {

		VCTR buf;

		buf.X = BZ.at(i).X;
		buf.Y = BZ.at(i).Y;

		out.at(BZ.size() - 1 - i) = buf;
	}
	return out;
}

vector < vector <VCTR> > extrapolate_to_circle (const vector < vector <VCTR> >& inBZ) {

	vector < vector <VCTR> > OUT = inBZ;

	for (size_t i = 0; i < inBZ.size(); i++) {

		size_t MAX = inBZ.at(i).size();

		VCTR FIRST = inBZ.at(i).at(0);
		VCTR LAST  = inBZ.at(i).at(MAX - 1);

		bool CLOSED = is_closed_line (FIRST, LAST);
		bool FIRST_IN = is_point_in_circle(FIRST);
		bool  LAST_IN = is_point_in_circle(LAST);

		if (!CLOSED) {

			if (FIRST_IN) {

				FIRST = generate_new_start (FIRST);
				OUT.at(i).at(0) = FIRST;
			}
			else {}

			if (LAST_IN) {

				LAST  = generate_new_start (LAST);
				OUT.at(i).at(MAX - 1) = LAST;
			}
			else {}
		}
	}
	return OUT;
}

bool is_line_CCW (const vector <VCTR>& I) {

	double A = polygon_area (I);

	if (A > 0.0) return true;
	else return false;
}

vector <VCTR> flip_line (const vector <VCTR>& I) {

	vector <VCTR> OUT = I;

	const size_t SIZE = I.size();

	for (size_t i = 0; i < SIZE; i++ ) 	OUT.at(i) = I.at(SIZE - 1 - i);

	return OUT;
}

vector <VCTR> flip_curve_to_CCW (const vector <VCTR>& BZ) {

	if (!is_line_CCW (BZ)) return flip_line(BZ);
	else return BZ;
}

vector <BEZIER> generate_final_bezier (const vector < VCTR>& inBZ) {

	vector <BEZIER> OUT;

	for (size_t j = 1; j < inBZ.size() - 1; j++) {

		BEZIER buf;

		bool is_FIRST = (j == 1);
		bool is_LAST  = (j == inBZ.size() - 2);

		VCTR PRW = inBZ.at(j - 1);
		VCTR ACT = inBZ.at(j - 0);
		VCTR NXT = inBZ.at(j + 1);

		if (is_FIRST) {

			buf.A.X = PRW.X;
			buf.A.Y = PRW.Y;
			buf.A.Z = NaN();

			buf.B.X = (PRW.X + ACT.X) / 2.0;
			buf.B.Y = (PRW.Y + ACT.Y) / 2.0;
			buf.B.Z = NaN();

			buf.C.X = (3.0 * ACT.X + NXT.X) / 4.0;
			buf.C.Y = (3.0 * ACT.Y + NXT.Y) / 4.0;
			buf.C.Z = NaN();

			buf.D.X = (NXT.X + ACT.X) / 2.0;
			buf.D.Y = (NXT.Y + ACT.Y) / 2.0;
			buf.D.Z = NaN();
		}
		else if (is_LAST) {

			buf.A.X = (PRW.X + ACT.X) / 2.0;
			buf.A.Y = (PRW.Y + ACT.Y) / 2.0;
			buf.A.Z = NaN();

			buf.B.X = (PRW.X + 2.0 * ACT.X) / 3.0;
			buf.B.Y = (PRW.Y + 2.0 * ACT.Y) / 3.0;
			buf.B.Z = NaN();

			buf.C.X = (ACT.X + NXT.X) / 2.0;
			buf.C.Y = (ACT.Y + NXT.Y) / 2.0;
			buf.C.Z = NaN();

			buf.D.X = NXT.X;
			buf.D.Y = NXT.Y;
			buf.D.Z = NaN();
		}
		else {

			buf.A.X = (PRW.X + ACT.X) / 2.0;
			buf.A.Y = (PRW.Y + ACT.Y) / 2.0;
			buf.A.Z = NaN();

			buf.B.X = (PRW.X + 3.0 * ACT.X) / 4.0;
			buf.B.Y = (PRW.Y + 3.0 * ACT.Y) / 4.0;
			buf.B.Z = NaN();

			buf.C.X = (3.0 * ACT.X + NXT.X) / 4.0;
			buf.C.Y = (3.0 * ACT.Y + NXT.Y) / 4.0;
			buf.C.Z = NaN();

			buf.D.X = (NXT.X + ACT.X) / 2.0;
			buf.D.Y = (NXT.Y + ACT.Y) / 2.0;
			buf.D.Z = NaN();
		}
		OUT.push_back(buf);
	}
	return OUT;
}

double return_t (const VCTR& C, const VCTR& B, const VCTR& P) {

	double X21 = C.X - B.X;
	double Y21 = C.Y - B.Y;
	double X1P = B.X - P.X;
	double Y1P = B.Y - P.Y;

	double t = - (((X1P * X21) + (Y1P * Y21)) / ((X21 * X21) + (Y21 * Y21)));

	return t;
}

double return_distance_from_side (const VCTR& C, const VCTR& B, const VCTR& P) {

	double X21 = C.X - B.X;
	double Y21 = C.Y - B.Y;
	double X1P = B.X - P.X;
	double Y1P = B.Y - P.Y;

	double t = return_t (C, B, P);

	double d = ((X1P + t * X21) * (X1P + t * X21)) + ((Y1P + t * Y21) * (Y1P + t * Y21));

	return sqrt (d);
}

double return_distance_from_point (const VCTR& A, const VCTR& P) {

	double X1P = A.X - P.X;
	double Y1P = A.Y - P.Y;

	double d = (X1P * X1P) + (Y1P * Y1P);

	return sqrt(d);
}

double return_triangle_area (const VCTR& A, const VCTR& B, const VCTR& C) {

	vector <VCTR> t;

	t.push_back(A);
	t.push_back(B);
	t.push_back(C);

	return polygon_area(t);
}

bool is_point_inside_curve (const vector <VCTR>& I, const GRID_CENTER& GP) {

	vector <VCTR> temp = I;

	size_t MAX = temp.size();

	VCTR FRST = temp.at(0);
	VCTR LAST = temp.at(MAX - 1);

	bool CLOSED = is_closed_line(FRST, LAST);

	if (!CLOSED) temp.push_back(temp.at(0));
	temp.push_back(temp.at(1));

	double DISTANCE = 10e9;

	bool CLOSE_TO_VERTEX = false;
	bool CLOSE_TO_SIDE = false;

	MAX = temp.size();

	const VCTR P = GP.CENTER;

	VCTR sA, sB, sC, vA, vB, vC;

	for (size_t i = 1; i < temp.size() - 1; i++) {

		VCTR act_C = temp.at(i - 1);
		VCTR act_B = temp.at(i);
		VCTR act_A = temp.at(i + 1);

		double t = return_t (act_C, act_B, P);

		if (t <= 0) {

			double ACT_DST_VRTX = return_distance_from_point (act_B, P);

			if (ACT_DST_VRTX < DISTANCE) {

				DISTANCE = ACT_DST_VRTX;
				CLOSE_TO_VERTEX = true;
				CLOSE_TO_SIDE = false;

				vA = act_A;
				vB = act_B;
				vC = act_C;
			}
		}
		else if (t > 0.0 && t < 1.0) {

			double ACT_DST_SIDE = return_distance_from_side  (act_C, act_B, P);

			if (ACT_DST_SIDE < DISTANCE) {

				DISTANCE = ACT_DST_SIDE;
				CLOSE_TO_SIDE = true;
				CLOSE_TO_VERTEX = false;

				sA = act_A;
				sB = act_B;
				sC = act_C;
			}
		}
		else {}
	}

	if (CLOSE_TO_SIDE && !CLOSE_TO_VERTEX) {

		double A1 = return_triangle_area (sC, sB, P);
		bool AREA_POSITIVE =  (A1 > 0.0);

		if (AREA_POSITIVE) return true;
		else return false;
	}
	else if (!CLOSE_TO_SIDE && CLOSE_TO_VERTEX) {

		double A2 = return_triangle_area (vC, vB, vA);
		bool VERTEX_CONCAVE = (A2 < 0.0);

		if (VERTEX_CONCAVE) return true;
		else return false;
	}
	else {
		ASSERT_DEAD_END();
		return false;
	}
}

bool curve_contains_peak (const vector <VCTR>& I, const vector < vector <GRID_CENTER> >& RECT_GRID, const double& CONTOUR) {

	for (size_t i = 0; i < RECT_GRID.size(); i++) {
		for (size_t j = 0; j < RECT_GRID.at(i).size(); j++) {

			GRID_CENTER ACT = RECT_GRID.at(i).at(j);

			if (ACT.COUNT >= CONTOUR) if (is_point_inside_curve (I, ACT)) return true;
		}
	}
	return false;
}

double polygon_area (const vector <VCTR>& I) {

	vector <VCTR> t = I;

	double A = 0;

	VCTR FRST = t.at(0);
	VCTR LAST = t.at(t.size() - 1);

	bool CLOSED = is_closed_line(FRST, LAST);

	if (!CLOSED) t.push_back(t.at(0));

	for (size_t i = 0; i < t.size() - 1; i++) {

		VCTR ACT = t.at(i);
		VCTR NXT = t.at(i + 1);

		double area = ((ACT.X * NXT.Y) - (ACT.Y * NXT.X)) / 2.0;

		A = A + area;
	}
	return A;
}

bool is_line_close_unitcircle (const vector <VCTR>& I, const double CELL) {

	size_t CLOSE_COUNTER = 0;

	for (size_t i = 0; i < I.size(); i++) {

		double LENGTH = sqrt(I.at(i).X * I.at(i).X + I.at(i).Y * I.at(i).Y);

		if (LENGTH > 1.0 - (CELL / 2.0)) CLOSE_COUNTER++;
	}
	return ((CLOSE_COUNTER / I.size()) > 0.6);
}

vector <VCTR> close_contourline (const vector <VCTR>& I, const double START_ANGLE, const double END_ANGLE, const double CELL, const bool CHECK_DISTANCE) {

	vector <VCTR> NEW;
	vector <VCTR> OUT = I;

	ASSERT2(is_line_CCW(I), "CCW line is expected in 'close_contourline' function");
	//ASSERT2(! (is_in_range(START_ANGLE, END_ANGLE, END_ANGLE)), "open line expected in 'close_contourline' function");

	bool START_LESS_THAN_END = (START_ANGLE < END_ANGLE);

	VCTR END;
	double step = 0.0;

	if (START_LESS_THAN_END) {
		step = 360.0 - (END_ANGLE - START_ANGLE);
		END = I.at(0);
	}
	else {
		step = START_ANGLE - END_ANGLE;
		END = I.at(I.size() - 1);
	}

	step = step / 100.0;
	END.Z = 0.0;

	double RATIO = 1.0;

	if (CHECK_DISTANCE && is_line_close_unitcircle (I, CELL)) RATIO = 1.0 + (CELL * 2.0);

	VCTR AXIS = declare_vector(0.0, 0.0, 1.0);

	for (size_t i = 1; i < 100; i++) {

		VCTR t = ROTATE (AXIS, END, step * i);

		t.X = t.X * RATIO;
		t.Y = t.Y * RATIO;

		NEW.push_back(t);
	}

	if (START_LESS_THAN_END) {

		OUT = flip_BZ_line(OUT);

		OUT.insert(OUT.end(), NEW.begin(), NEW.end());

		if (!is_line_CCW(OUT)) OUT = flip_BZ_line(OUT);
	}
	else OUT.insert(OUT.end(), NEW.begin(), NEW.end());

	return OUT;
}

void contourline_to_ps (ofstream& o, const CENTER& center, const vector <VCTR>& BZ, const double& FRST_ANGLE, const double& LAST_ANGLE, const double& CONTOUR, const double& C_MN, const double& C_MX) {

	o << "%% contourline_to_ps" << endl;

	VCTR FRST_PNT = BZ.at(0);
	VCTR LAST_PNT = BZ.at(BZ.size() - 1);

	bool CLOSED = is_closed_line(FRST_PNT, LAST_PNT);

	if (!CLOSED && FRST_ANGLE > 900.0 && LAST_ANGLE > 900.0) ASSERT_DEAD_END();

	vector <BEZIER> B = generate_final_bezier (BZ);

	o << "  newpath " << endl;

	o << 0.9 - (((CONTOUR - C_MN) / (C_MX - C_MN)) / 2.0) << " 1.00 1.00 setrgbcolor " << endl;

	o
	<< B.at(0).A.X * center.radius + center.X << " "
	<< B.at(0).A.Y * center.radius + center.Y  << " moveto" << endl;

	for (size_t i = 0; i < B.size(); i++) {

		o
		<< B.at(i).B.X * center.radius + center.X  << " "
		<< B.at(i).B.Y * center.radius + center.Y  << " "
		<< B.at(i).C.X * center.radius + center.X  << " "
		<< B.at(i).C.Y * center.radius + center.Y  << " "
		<< B.at(i).D.X * center.radius + center.X  << " "
		<< B.at(i).D.Y * center.radius + center.Y  << " curveto" << endl;
	}

	if (!CLOSED) {

		o
		<< " " << center.X
		<< " " << center.Y
		<< " " << center.radius
		<< " " << 90.0 + 360.0 - FRST_ANGLE
		<< " " << 90.0 + 360.0 - LAST_ANGLE << " arc " << flush;
	}

	o << "  closepath fill stroke " << endl;

	o << "  newpath " << endl;
	o <<  "0.50 0.50 0.50 setrgbcolor " << endl;

	o
	<< B.at(0).A.X * center.radius + center.X << " "
	<< B.at(0).A.Y * center.radius + center.Y  << " moveto" << endl;

	for (size_t i = 0; i < B.size(); i++) {

		o
		<< B.at(i).B.X * center.radius + center.X  << " "
		<< B.at(i).B.Y * center.radius + center.Y  << " "
		<< B.at(i).C.X * center.radius + center.X  << " "
		<< B.at(i).C.Y * center.radius + center.Y  << " "
		<< B.at(i).D.X * center.radius + center.X  << " "
		<< B.at(i).D.Y * center.radius + center.Y  << " curveto" << endl;
	}

	o << " [2 2] 0 setdash 0.8 setlinewidth stroke " << endl;
	o << " [  ] 0 setdash " << endl;

}

void output_contourline (ofstream& o, const CENTER& center, vector <vector <VCTR> >& BZ,  vector < vector <GRID_CENTER> >& RECT_GRID, const double& CONTOUR, const double C_MN, const double C_MX, const double& MAX) {

	double CELL = return_grid_size(RECT_GRID);

	for (size_t i = 0; i < BZ.size(); i++) {

		vector <VCTR> A =  BZ.at(i);

		VCTR FRST_PNT = A.at(0);
		VCTR LAST_PNT = A.at(A.size() - 1);

		FRST_PNT.Z = 0.0;
		LAST_PNT.Z = 0.0;

		if (!is_closed_line (FRST_PNT, LAST_PNT)) {

			double FRST_ANGLE = dipdir_dip_from_DXDYDZ(FRST_PNT).DIPDIR;
			double LAST_ANGLE = dipdir_dip_from_DXDYDZ(LAST_PNT).DIPDIR;

			A = flip_curve_to_CCW (A);

			vector <VCTR> A1 = close_contourline (A, FRST_ANGLE, LAST_ANGLE, CELL, true);
			vector <VCTR> A2 = close_contourline (A, LAST_ANGLE, FRST_ANGLE, CELL, true);

			bool A1_HAS_PEAK = curve_contains_peak (A1, RECT_GRID, CONTOUR * MAX);
			bool A2_HAS_PEAK = curve_contains_peak (A2, RECT_GRID, CONTOUR * MAX);

			A1 = close_contourline (A, FRST_ANGLE, LAST_ANGLE, CELL, false);
			A2 = close_contourline (A, LAST_ANGLE, FRST_ANGLE, CELL, false);

			double A1_AREA = polygon_area (A1);
			double A2_AREA = polygon_area (A2);

			bool LARGER_HAS_PEAKS = (
					(A1_AREA > A2_AREA && A1_HAS_PEAK && !A2_HAS_PEAK) ||
					(A2_AREA > A1_AREA && A2_HAS_PEAK && !A1_HAS_PEAK));
			bool LARGE_AT_LEAST_DOUBLE = (
					A1_AREA > A2_AREA * 2.0 || A2_AREA > A1_AREA * 2.0);

			bool DRAW_IT = true;

			if (LARGE_AT_LEAST_DOUBLE && LARGER_HAS_PEAKS) DRAW_IT = false;

			if (DRAW_IT) {

				if (A1_HAS_PEAK && !A2_HAS_PEAK) 		contourline_to_ps (o, center, A, FRST_ANGLE, LAST_ANGLE, CONTOUR, C_MN, C_MX);
				else if (!A1_HAS_PEAK && A2_HAS_PEAK) 	contourline_to_ps (o, center, A, LAST_ANGLE, FRST_ANGLE, CONTOUR, C_MN, C_MX);
				else if (A1_HAS_PEAK && A2_HAS_PEAK) {

					if (A1_AREA < A2_AREA) 	contourline_to_ps (o, center, A, FRST_ANGLE, LAST_ANGLE, CONTOUR, C_MN, C_MX);
					else 					contourline_to_ps (o, center, A, LAST_ANGLE, FRST_ANGLE, CONTOUR, C_MN, C_MX);
				}
				else ASSERT_DEAD_END();
			}
			else {}
		}
		else contourline_to_ps (o, center, A, 999.99, 999.99, CONTOUR, C_MN, C_MX);
	}
}

bool is_processable_for_contouring (const vector <GDB>& inGDB) {

	vector <GDB> test = return_GDB_with_no_homogeneous_data (inGDB);

	if (test.size() < 2) return false;

	return true;
}

void CONTOURING (const vector <GDB>& inGDB, ofstream& o, const CENTER center, const bool TILT) {

	if (is_CONTOURING_NO()) return;
	if (!is_processable_for_contouring (inGDB)) return;

	vector <vector <vector <VCTR> > > NET = generate_net (9);

	vector <TRIANGLE> TRI_GRID = generate_net_count (inGDB, NET);

	ofstream r;

	vector <GRID_CENTER> TRI_CENTER = generate_triangle_center (TRI_GRID);

	size_t cell_number = round (sqrt (TRI_CENTER.size() / 0.78));

	TRI_CENTER = reduce_triangle_center(TRI_CENTER);

	vector < vector <GRID_CENTER> > RECT_GRID = generate_rectangular_grid_from_triange_center (cell_number);

	RECT_GRID = calculate_grid_cell_values_from_triangle (RECT_GRID, TRI_CENTER);

	if (is_CHK_CONTOURING()) {

		string T = return_ACTUAL_LOCATION();
		T = T + "_" + return_ACTUAL_FORMATION();
		T = T + "_" + return_ACTUAL_DATATYPE();
		T = T + "_" + return_ACTUAL_GROUPCODE();

		if (TILT) T = T + "_TLT";
		else T = T + "_NRM";

		if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

		dump_TRI_GRID_to_file (TRI_GRID, "ST_CONTOURING_"+T);

		dump_RECT_GRID_to_file (RECT_GRID, "ST_CONTOURING_"+T);
	}

	double C_MN = 0.5;
	double C_MX = 0.9;

	vector <double> CNTR_AT = return_isoline_percentages (C_MN, C_MX);

	size_t MAX = return_RECT_GRID_max_count (RECT_GRID);

	for (size_t i = 0; i < CNTR_AT.size(); i++) {

		vector < vector <GRID_CENTER> > BIN_GRID = generate_binary_rect_grid (RECT_GRID, CNTR_AT.at(i) * MAX);

		vector < vector <GRID_CENTER> > M_SQ = generate_marching_squares (BIN_GRID);

		if (M_SQ.size() + 1 != BIN_GRID.size()) ASSERT3("rectangular density grid and/or maching sqare grid error");

		M_SQ = check_saddle (M_SQ, RECT_GRID, CNTR_AT.at(i) * MAX);

		vector <LINE> C_LINE = generate_raw_lines (M_SQ, RECT_GRID, CNTR_AT.at(i) * MAX);

		vector <vector <LINE> > LINE_VCTR = generate_line_vector (C_LINE);

		LINE_VCTR = connect_vectors (LINE_VCTR);

		LINE_VCTR = eliminate_empty_short_records (LINE_VCTR);

		vector < vector <VCTR> > BZ = generate_bezier_points (LINE_VCTR);

		BZ = extrapolate_to_circle (BZ);

		BZ = eliminate_short_distances (BZ);

		output_contourline (o, center, BZ, RECT_GRID, CNTR_AT.at(i), C_MN, C_MX, MAX);
	}
}

void dbg_cout_NET (const vector <vector <vector <VCTR> > >& NET) {

	cout << "**** DEBUGGING KAALSBEEK NET FOR CONTOURING ****" << endl;
	cout << fixed << setprecision (6) << endl;

	for (size_t i = 0; i < NET.size(); i++) {
		for (size_t j = 0; j < NET.at(i).size(); j++) {
			for (size_t k = 0; k < NET.at(i).at(j).size(); k++) {

				cout
				<< NET.at(i).at(j).at(k).X << '\t'
				<< NET.at(i).at(j).at(k).Y << '\t'
				<< NET.at(i).at(j).at(k).Z << endl;
			}
		}
	}
	cout << "**** END DEBUGGING KAALSBEEK NET FOR CONTOURING ****" << endl;
}

void dbg_cout_TRI_GRID (const vector <TRIANGLE>& TRI_GRID) {

	cout << "**** DEBUGGING TRIANGULAR GRID WITH COUNTS FOR CONTOURING ****" << endl;
	cout << fixed << setprecision (8) << endl;

	cout
	<< "A.X" << '\t' << "A.Y" << '\t' << "A.Z" << '\t'
	<< "B.X" << '\t' << "B.Y" << '\t' << "B.Z" << '\t'
	<< "C.X" << '\t' << "C.Y" << '\t' << "C.Z" << '\t'
	<< "COUNT" << '\t'<< "GROUP" << endl;

	for (size_t i = 0; i < TRI_GRID.size(); i++) {

		cout
		<< TRI_GRID.at(i).A.X << '\t'
		<< TRI_GRID.at(i).A.Y << '\t'
		<< TRI_GRID.at(i).A.Z << '\t'

		<< TRI_GRID.at(i).B.X << '\t'
		<< TRI_GRID.at(i).B.Y << '\t'
		<< TRI_GRID.at(i).B.Z << '\t'

		<< TRI_GRID.at(i).C.X << '\t'
		<< TRI_GRID.at(i).C.Y << '\t'
		<< TRI_GRID.at(i).C.Z << '\t'

		<< TRI_GRID.at(i).COUNT << '\t'
		<< TRI_GRID.at(i).GROUP << endl;
	}
	cout << "**** END DEBUGGING TRIANGULAR GRID WITH COUNTS FORR CONTOURING ****" << endl << endl;
}

void dbg_cout_CONTOURS (const vector <size_t>& CNTR_AT) {

	cout << "**** DEBUGGING CONTOUR INTERVALS FOR CONTOURING ****" << endl;

	cout << fixed << setprecision (0) << endl;

	for (size_t i = 0; i < CNTR_AT.size(); i++) cout << CNTR_AT.at(i) << endl;

	cout << "**** END DEBUGGING CONTOUR INTERVALS FOR CONTOURING ****" << endl;
}

void dbg_cout_triangle_center (const vector <GRID_CENTER>& TRI_CENTER) {

	cout << "**** DEBUGGING TRIANGULAR GRID CENTERS ****" << endl;
	cout << "X" << '\t' << "Y" << '\t' << "Z" << '\t' << "COUNT" << endl;

	cout << fixed << setprecision(3) << endl;

	for (size_t i = 0; i < TRI_CENTER.size(); i++) {

		cout
		<< TRI_CENTER.at(i).CENTER.X << '\t'
		<< TRI_CENTER.at(i).CENTER.Y << '\t'
		<< TRI_CENTER.at(i).CENTER.Z << '\t'
		<< TRI_CENTER.at(i).COUNT << endl;
	}
	cout << "**** END DEBUGGING TRIANGULAR GRID CENTERS ****" << endl;
}

void dbg_cout_rect_grid (const vector < vector <GRID_CENTER> >& rect_grid, const bool display_coordinates) {

	size_t cell_number = rect_grid.at(0).size();

	cout << "-------- DEBUGGING RECTANGULAR DENSITY GRID --------" << endl;

	for (size_t i = 0; i < cell_number; i++) {
		for (size_t j = 0; j < cell_number; j++) {

			if (display_coordinates) {
				cout
				<< fixed << setprecision(8)
				<< rect_grid.at(i).at(j).CENTER.X << '\t'
				<< rect_grid.at(i).at(j).CENTER.Y << '\t'<< flush;
			}

			cout << fixed << setprecision(0)
			<< rect_grid.at(i).at(j).COUNT << '\t' << flush;
		}
		cout << endl;
	}
	cout << "-------- END DEBUGGING RECTANGULAR DENSITY GRID --------" << endl << endl;;
}

void dbg_cout_line (const vector <LINE>& L) {

	cout << "---- DEBUGGING LINES ----" << endl;

	cout
	<< "A.X" << '\t' << "A.Y" << '\t'
	<< "B.X" << '\t' << "B.Y" << endl;

	cout << fixed << setprecision(6) << endl;

	for(size_t i = 0; i < L.size(); i++) {

		cout
		<< L.at(i).A.X << '\t'
		<< L.at(i).A.Y << '\t'
		<< L.at(i).B.X << '\t'
		<< L.at(i).B.Y << endl;
	}
	cout << "---- END DEBUGGING LINES ----" << endl << endl;
}

void dbg_cout_line_vctr (const vector <vector <LINE> >& LV) {

	cout << "LINE VECTOR: " << endl;

	for(size_t i = 0; i < LV.size(); i++) {

		for(size_t j = 0; j < LV.at(i).size(); j++) {

			cout << LV.at(i).at(j).A.X << '\t' << flush;
			cout << LV.at(i).at(j).A.Y << '\t' << flush;
			cout << LV.at(i).at(j).B.X << '\t' << flush;
			cout << LV.at(i).at(j).B.Y << '\t' << flush;
		}
		cout << endl;
	}
}

void dbg_bezier_points (const vector <VCTR>& BZ) {

	cout << "**** BEZIER POINTS FOR CONTOURING ****" << endl;

	for (size_t j = 0; j < BZ.size(); j++) {

		cout << BZ.at(j).X << '\t' << BZ.at(j).Y << '\t' << endl;
	}
	cout << "**** END BEZIER POINTS FOR CONTOURING ****" << endl;
}

void dbg_bezier_curve (const vector <BEZIER>& B) {

	cout << "**** BEZIER CURVE FOR CONTOURING ****" << endl;

	cout
	<< "A.X" << '\t' << "A.Y" << '\t'
	<< "B.X" << '\t' << "B.Y" << '\t'
	<< "C.X" << '\t' << "C.Y" << '\t'
	<< "D.X" << '\t' << "D.Y" << '\t' << endl;

	for (size_t j = 0; j < B.size(); j++) {

		cout
		<< B.at(j).A.X << '\t' << B.at(j).A.Y << '\t'
		<< B.at(j).B.X << '\t' << B.at(j).B.Y << '\t'
		<< B.at(j).C.X << '\t' << B.at(j).C.Y << '\t'
		<< B.at(j).D.X << '\t' << B.at(j).D.Y << '\t' << endl;
	}

	cout << "**** END BEZIER CURVE FOR CONTOURING ****" << endl;
}

void cout_rect_grid_to_ps (const vector <vector < GRID_CENTER> >& rect_grid, ofstream& o) {

	for (size_t j = 0; j < rect_grid.size() - 1; j++) {

		for (size_t i = 0; i < rect_grid.at(0).size() - 1; i++) {

			double AX = rect_grid.at(j + 1).at(i + 0).CENTER.X;
			double BX = rect_grid.at(j + 1).at(i + 1).CENTER.X;
			double CX = rect_grid.at(j + 0).at(i + 1).CENTER.X;
			double DX = rect_grid.at(j + 0).at(i + 0).CENTER.X;

			double AY = rect_grid.at(j + 1).at(i + 0).CENTER.Y;
			double BY = rect_grid.at(j + 1).at(i + 1).CENTER.Y;
			double CY = rect_grid.at(j + 0).at(i + 1).CENTER.Y;
			double DY = rect_grid.at(j + 0).at(i + 0).CENTER.Y;

			double AW = rect_grid.at(j + 1).at(i + 0).COUNT;
			double BW = rect_grid.at(j + 1).at(i + 1).COUNT;
			double CW = rect_grid.at(j + 0).at(i + 1).COUNT;
			double DW = rect_grid.at(j + 0).at(i + 0).COUNT;

			double COUNT = ((AW + BW + CW + DW) / 4.0);

			const string COLOR = generate_stress_colors(COUNT);

			if (COUNT > 0.01) {

				o
				<< " " << AX * 200.0 + 220.0 << " " << AY * 200.0 + 220.0 << " moveto"
				<< " " << BX * 200.0 + 220.0 << " " << BY * 200.0 + 220.0 << " lineto"
				<< " " << CX * 200.0 + 220.0 << " " << CY * 200.0 + 220.0 << " lineto"
				<< " " << DX * 200.0 + 220.0 << " " << DY * 200.0 + 220.0 << " lineto"

				<< " " << COLOR

				<< " setrgbcolor fill stroke" << endl;
			}
		}
		cout << endl;
	}
}
