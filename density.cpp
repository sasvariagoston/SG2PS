// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "assertions.hpp"
#include "common.h"
#include "kaalsbeek.hpp"
#include "density.h"
#include "ps.h"
#include "structs.h"

XY stereonet_coordinate_from_DIPDIR_DIP (DIPDIR_DIP in, CENTER center, INPSET inset) {

	XY out;

	VCTR D = DXDYDZ_from_dipdir_dip(in);

	if (D.Z > 0.0) D.Z = - D.Z;

	if (inset.plottype == "S") {

		out.X = (D.X / sqrt (1.00 - D.Z)) * center.radius;
		out.Y = (D.Y / sqrt (1.00 - D.Z)) * center.radius;
	}
	else {

		out.X = (D.X / (1.00 - D.Z)) * center.radius;
		out.Y = (D.Y / (1.00 - D.Z)) * center.radius;
	}

	if (inset.hemisphere == "U") {

		out.X = - out.X;
		out.Y = - out.Y;
	}
	else {} //OK

	out.X = out.X + center.X;
	out.Y = out.Y + center.Y;

	return out;
}

VCTR density_color_from_percentage (double percentage) {

	VCTR minimum = declare_vector(1.00, 1.00, 1.00);
	VCTR maximum = declare_vector(1.00, 0.00, 1.00);

	return declare_vector(
	minimum.X * (1.0 - percentage) + maximum.X * percentage,
	minimum.Y * (1.0 - percentage) + maximum.Y * percentage,
	minimum.Z * (1.0 - percentage) + maximum.Z * percentage);
}

DENSITY density_in_cell (vector <GDB> in, size_t search_dipdir, size_t search_dip, size_t radius) {

	DENSITY out;

	size_t counter = 0;

	double min_dipdir =		search_dipdir - radius;
	double max_dipdir = 	search_dipdir + radius;
	double min_dip = 		search_dip - radius;
	double max_dip = 		search_dip + radius;

	for (size_t i = 0; i < in.size(); i++) {

		double DD = in.at(i).corr.DIPDIR;
		double D = in.at(i).corr.DIP;

		bool DD_IN_RANGE = (is_in_range(min_dipdir, max_dipdir, DD));
		bool D_IN_RANGE = (is_in_range(min_dip, max_dip, D));

		if (DD_IN_RANGE && D_IN_RANGE) counter++;
		else {};
	}

	out.direction.DIPDIR = search_dipdir;
	out.direction.DIP = search_dip;
	out.percentage = counter;

	return out;
}

vector < vector <GRID_CENTER> > generate_rectangular_grid_from_triange_center (size_t cell_number) {

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

vector < vector <GRID_CENTER> > calculate_grid_cell_values_from_triangle (vector < vector <GRID_CENTER> > rect_grid, vector <GRID_CENTER> tri_center) {

	size_t cell_number = rect_grid.at(0).size();

	for (size_t k = 0; k < cell_number; k++) {

		for (size_t j = 0; j < cell_number; j++) {

			double cml_value = 0.0;

			double R_X = rect_grid.at(k).at(j).CENTER.X;
			double R_Y = rect_grid.at(k).at(j).CENTER.Y;

			for (size_t i = 0; i < tri_center.size(); i++) {

				double T_X = tri_center.at(i).CENTER.X;
				double T_Y = tri_center.at(i).CENTER.Y;

				double distance = sqrt((R_X - T_X) * (R_X - T_X) + (R_Y - T_Y) * (R_Y - T_Y));

				cml_value = cml_value + (tri_center.at(i).COUNT / (distance));
			}

			bool IN_RECT_CELL = (sqrt((R_X * R_X) + (R_Y * R_Y)) <= 1.0);

			if (IN_RECT_CELL)	rect_grid.at(k).at(j).COUNT = cml_value;
			else {} // OK
		}
	}
	return rect_grid;
}

bool by_GRID_COUNT (const GRID_CENTER& x, const GRID_CENTER& y) {

	return x.COUNT < y.COUNT;
}

bool by_VALUE (const double& x, const double& y) {

	return x < y;
}

vector <GRID_CENTER> sort_by_GRID_COUNT (vector <GRID_CENTER> GC) {

	sort(GC.begin(), GC.end(), by_GRID_COUNT);

	return GC;
}

vector <size_t> sort_by_VALUE (vector <size_t> IN) {

	sort(IN.begin(), IN.end(), by_VALUE);

	return IN;
}

size_t return_TRI_GRID_max_count (vector <GRID_CENTER> TRI_CENTER) {

	TRI_CENTER = sort_by_GRID_COUNT (TRI_CENTER);

	return TRI_CENTER.at(TRI_CENTER.size() - 1).COUNT;
}

size_t return_RECT_GRID_max_count (vector < vector <GRID_CENTER> > RECT_GRID) {

	double MAX = 0;

	for (size_t i = 0; i < RECT_GRID.size(); i++) {

		for (size_t j = 0; j < RECT_GRID.at(i).size(); j++) {

			if (RECT_GRID.at(i).at(j).COUNT > MAX) MAX = RECT_GRID.at(i).at(j).COUNT;

		}
	}
	return MAX;
}

vector < vector <GRID_CENTER> > normalize_grid_cell_values (vector < vector <GRID_CENTER> > rect_grid, vector <GRID_CENTER> tri_center) {

	size_t max_count = return_TRI_GRID_max_count (tri_center);

	double rect_max = return_RECT_GRID_max_count(rect_grid);

	for (size_t i = 0; i < rect_grid.size(); i++) {

		for (size_t j = 0; j < rect_grid.at(0).size(); j++) {

			rect_grid.at(i).at(j).COUNT = (rect_grid.at(i).at(j).COUNT / rect_max) * max_count;
		}
	}
	return rect_grid;
}

double return_contour_step (double TRI_MAX_COUNT) {

	return TRI_MAX_COUNT / 10.0;
}

vector <size_t> return_isoline (vector <vector <GRID_CENTER> > RECT_GRID) {

	vector <size_t> ISOLINE;

	vector <size_t> COUNTS;

	size_t CELLS = RECT_GRID.size();

	for (size_t i = 0; i < CELLS; i++) {
		for (size_t j = 0; j < CELLS; j++) {

			if (RECT_GRID.at(i).at(j).COUNT > 0) COUNTS.push_back(RECT_GRID.at(i).at(j).COUNT);
		}
	}

	COUNTS = sort_by_VALUE (COUNTS);

	//for (size_t i = 0; i < COUNTS.size(); i++) {
	//	cout << COUNTS.at(i) << endl;
	//}

	size_t TEN_PERCENT =  COUNTS.size() / 10.0;

	size_t MIN = 0;
	size_t MAX = 0;

	for (size_t i = 0; i < 9; i++) {

		MIN =  i      * TEN_PERCENT;
		MAX = (i + 1) * TEN_PERCENT;

		ISOLINE.push_back((COUNTS.at(MAX) + COUNTS.at(MIN)) / 2.0);

		//cout << ISOLINE.at(i) << " -- " << MIN << " -- " << MAX << " -- " << COUNTS.size() << " ------ " << COUNTS.at(MAX) << " -- " << COUNTS.at(MIN) << endl;
	}

	ISOLINE.push_back((COUNTS.at(COUNTS.size() - 1) + COUNTS.at(MAX)) / 2.0);

	//for (size_t i = 0; i < ISOLINE.size(); i++) {

	//	cout << ISOLINE.at(i) << endl;
	//}

	return ISOLINE;
}

vector < vector <GRID_CENTER> > generate_empty_binary_rect_grid (size_t cell_number) {

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

vector < vector <GRID_CENTER> > generate_binary_rect_grid (vector < vector <GRID_CENTER> > rect_grid, size_t COUNT) {

	size_t cell_number = rect_grid.at(0).size();

	ASSERT2((rect_grid.size() == rect_grid.at(0).size()), "Error in binary rectangular grid while contouring");

	vector < vector <GRID_CENTER> > bin_grid = generate_empty_binary_rect_grid (cell_number);

	for (size_t j = 0; j < cell_number; j++) {

		for (size_t i = 0; i < cell_number; i++) {

			if (rect_grid.at(j).at(i).COUNT > COUNT) bin_grid.at(j).at(i).COUNT = 1;
			else rect_grid.at(j).at(i).COUNT = 0;
		}
	}

	return bin_grid;
}

vector < vector <GRID_CENTER> > generate_marching_squares (vector < vector <GRID_CENTER> > bin_grid) {

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

	return m_sq; // ok
}

// ---------------------- eddig ok

vector < vector <GRID_CENTER> > check_saddle (vector < vector <GRID_CENTER> > m_sq, vector < vector <GRID_CENTER> > rect_grid, size_t isoline) {

	size_t cell_number = m_sq.at(0).size();

	for (size_t j = 0; j < cell_number; j++) {

		for (size_t i = 0; i < cell_number; i++) {

			size_t SQ_ID = m_sq.at(j).at(i).COUNT;

			double A_CNT = rect_grid.at(j + 1).at(i + 0).COUNT;
			double B_CNT = rect_grid.at(j + 1).at(i + 1).COUNT;
			double C_CNT = rect_grid.at(j + 0).at(i + 1).COUNT;
			double D_CNT = rect_grid.at(j + 0).at(i + 0).COUNT;

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

vector <LINE> return_line_from_m_sq_number (vector <vector <GRID_CENTER> > m_sq, vector <vector <GRID_CENTER> > rect_grid, size_t j, size_t i, size_t isoline) {

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

vector <LINE> generate_raw_lines (vector <vector <GRID_CENTER> > m_sq, vector <vector <GRID_CENTER> > rect_grid, size_t isoline) {

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

			out.insert(out.end(), buf.begin(), buf.end());
		}
	}
	return out;
}

vector <vector <LINE> > generate_line_vector (vector <LINE> in) {

	vector <vector <LINE> > out;

	for (size_t i = 0; i < in.size(); i++) {

		LINE buf = in.at(i);

		vector <LINE> row_buf;

		row_buf.push_back(buf);

		out.push_back(row_buf);
	}

	return out;
}

vector <LINE> turn_off_record (vector <LINE> in) {

	for (size_t i = 0; i < in.size(); i++) {

		in.at(i).A.X = 999.99;
		in.at(i).A.Y = 999.99;
		in.at(i).B.X = 999.99;
		in.at(i).B.Y = 999.99;
	}

	return in;
}

vector <LINE> flip_line (vector <LINE> L) {

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

vector <vector <LINE> > connect_vectors(vector <vector <LINE> > LV) {

	size_t change_counter = 0;

	do {

		change_counter = 0;

		for(size_t k = 0; k < LV.size(); k++) {

			for(size_t j = 0; j < LV.size(); j++) {

				LINE to_flip_1 = LV.at(j).at(0);
				LINE to_flip_2 = LV.at(k).at(0);

				bool FLIP = (
						is_in_range (to_flip_1.A.X, to_flip_1.A.X, to_flip_2.A.X) &&
						is_in_range (to_flip_1.A.Y, to_flip_1.A.Y, to_flip_2.A.Y) &&
						(to_flip_1.A.X < 900.0) && (to_flip_1.A.Y < 900.0) &&
						(to_flip_2.B.X < 900.0) && (to_flip_2.B.Y < 900.0) &&
						(j != k)

				);

				if (FLIP) {

					if (j < k) LV.at(j) = 	flip_line (LV.at(j));
					else LV.at(k) = 		flip_line (LV.at(k));
				}

				LINE iter_1 = LV.at(j).at(0);
				LINE iter_2 = LV.at(k).at(LV.at(k).size() - 1);

				bool CONNECTION = (
						is_in_range (iter_1.A.X, iter_1.A.X, iter_2.B.X) &&
						is_in_range (iter_1.A.Y, iter_1.A.Y, iter_2.B.Y) &&
						(iter_1.A.X < 900.0) &&	(iter_1.A.Y < 900.0) &&
						(iter_2.B.X < 900.0) && (iter_2.B.Y < 900.0) &&
						(j != k)
				);

				if (CONNECTION) {

					LV.at(k).insert(LV.at(k).end(), LV.at(j).begin(), LV.at(j).end());

					LV.at(j) = turn_off_record(LV.at(j));

					change_counter++;
				}
				else {}
			}
		}

	} while (change_counter > 0);

	return LV;
}

vector <vector <LINE> > tidy_vectors(vector <vector <LINE> > LV) {

	vector <vector <LINE> > out;

	for (size_t i = 0; i < LV.size(); i++) {

		vector <LINE> buf = LV.at(i);

		if (LV.at(i).at(0).A.X < 900.0) out.push_back(buf);
	}

	return out;
}

vector < vector <VCTR> > generate_bezier_points (vector <vector <LINE> > LV) {

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

bool is_point_in_circle (VCTR in) {

	return (sqrt((in.X * in.X) + (in.Y * in.Y)) < 1.0);
}

bool is_circle_border (VCTR A, VCTR B) {

	if (
			(is_point_in_circle(A) && !is_point_in_circle(B)) ||
			(is_point_in_circle(B) && !is_point_in_circle(A))
			) return true;
	else return false;
}

vector < vector <VCTR> > fit_to_circle_I (vector < vector <VCTR> > inBZ) {

	vector < vector <VCTR> > out;

	for (size_t i = 0; i < inBZ.size(); i++) {

		vector <VCTR> buf;

		for (size_t j = 0; j < inBZ.at(i).size() - 1; j++) {

			VCTR act = inBZ.at(i).at(j);
			VCTR next = inBZ.at(i).at(j + 1);

			bool border = false;

			border = is_circle_border(act, next);

			buf.push_back(act);


			if (border) {

				buf.push_back(next);

				out.push_back(buf);

				buf.clear();

				buf.push_back(act);
			}
			else {}

			if (j == inBZ.at(i).size() - 2) {

				buf.push_back(next);

				out.push_back(buf);

				buf.clear();
			}
			else {}
		}
	}

	return out;
}

VCTR interpolate_between_points (VCTR inA, VCTR inB, bool linestart) {

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

	cout << A << "  "  << B << "  "  << C << "  "  << X1 << "  "  << X2 << endl;

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

	//cout << inA.X << '\t' << inA.Y << endl;
	//cout << X << '\t' << Y << endl;
	//cout << inB.X << '\t' << inB.Y << endl;


	ASSERT(!isnan(Y));

	return declare_vector(X, Y, NaN());
}

VCTR generate_new_start (VCTR A) {

	double length = sqrt((A.X * A.X ) +	(A.Y * A.Y));

	VCTR out;

	out.X = A.X / length;
	out.Y = A.Y / length;
	out.Z = NaN();

	return out;
}

bool is_closed_line (VCTR min_A, VCTR max_B) {

	return (is_in_range(min_A.X, min_A.X, max_B.X) && is_in_range(min_A.Y, min_A.Y, max_B.Y));
}

vector <VCTR> flip_BZ_line (vector <VCTR> BZ) {

	vector <VCTR> out = BZ;

	for (size_t i = 0; i < BZ.size(); i++) {

		VCTR buf;

		buf.X = BZ.at(i).X;
		buf.Y = BZ.at(i).Y;

		out.at(BZ.size() - 1 - i) = buf;
	}

	return out;
}

vector <VCTR> turn_off_BZ_record (vector <VCTR> in) {

	for (size_t i = 0; i < in.size(); i++) {

		in.at(i).X = 999.99;
		in.at(i).Y = 999.99;
	}

	return in;
}

bool BZ_to_connect (VCTR A, VCTR B) {

	return (
			is_in_range (A.X, A.X, B.X) &&
			is_in_range (A.Y, A.Y, B.Y) &&
			(A.X < 900.0) && (A.Y < 900.0) &&
			(B.X < 900.0) && (B.Y < 900.0) &&
			is_point_in_circle(A) &&
			is_point_in_circle(B)
	);
}

bool BZ_to_flip (VCTR A, VCTR B) {

	return (
			is_in_range (A.X, A.X, B.X) &&
			is_in_range (A.Y, A.Y, B.Y) &&
			(A.X < 900.0) && (A.Y < 900.0) &&
			(B.X < 900.0) && (B.Y < 900.0)
	);
}

vector < vector <VCTR> > connect_bezier_segments (vector < vector <VCTR> > inBZ){

	size_t change_counter = 0;

	do {

		change_counter = 0;

		for(size_t k = 0; k < inBZ.size(); k++) {

			for(size_t j = 0; j < inBZ.size(); j++) {

				VCTR to_flip_1 = inBZ.at(j).at(0);
				VCTR to_flip_2 = inBZ.at(k).at(0);

				if (BZ_to_flip (to_flip_1, to_flip_2) && (j != k)) {

					cout << "NEED TO FLIP" << endl;

					if (j < k) inBZ.at(j) = flip_BZ_line (inBZ.at(j));
					else inBZ.at(k) = 		flip_BZ_line (inBZ.at(k));

				}

				VCTR iter_1 = inBZ.at(j).at(0);
				VCTR iter_2 = inBZ.at(k).at(inBZ.at(k).size() - 1);

				if (BZ_to_connect(iter_1, iter_2) && (j != k)) {

					inBZ.at(k).insert(inBZ.at(k).end(), inBZ.at(j).begin(), inBZ.at(j).end());

					inBZ.at(j) = turn_off_BZ_record(inBZ.at(j));

					change_counter++;
				}
				else {}
			}
		}

	} while (change_counter > 0);

	return inBZ;
}

vector < vector <VCTR> > fit_to_circle_II (vector < vector <VCTR> > inBZ) {

	//vector < vector <VCTR> > out;

	for (size_t i = 0; i < inBZ.size(); i++) {

		vector <VCTR>  buf;

		VCTR min_A = inBZ.at(i).at(0);
		VCTR min_B = inBZ.at(i).at(1);

		VCTR max_A = inBZ.at(i).at(inBZ.at(i).size() - 2);
		VCTR max_B = inBZ.at(i).at(inBZ.at(i).size() - 1);

		bool is_start_crossing =	is_circle_border(min_A, min_B);
		bool is_end_crossing = 		is_circle_border(max_A, max_B);


		bool closed_line = (is_closed_line (min_A, max_B) &&

				min_A.X < 900.0 &&
				min_A.Y < 900.0 &&
				max_B.X < 900.0 &&
				max_B.Y < 900.0);


		if (is_start_crossing) {

			//cout << "is_start_crossing" << endl;

			inBZ.at(i).at(0) = interpolate_between_points (min_A, min_B, true);
		}
		else if (!is_start_crossing && !closed_line) {

			inBZ.at(i).at(0) = generate_new_start (min_A);
		}
		else {}



		if (is_end_crossing) {

			//cout << "is_end_crossing" << endl;

			//cout << max_A.X << "  " << max_A.Y << "  " << endl;
			//cout << max_B.X << "  " << max_B.Y << "  " << endl;

			inBZ.at(i).at(inBZ.at(i).size() - 1) = interpolate_between_points (max_A, max_B, false);
		}
		else if (!is_end_crossing && !closed_line) {

			inBZ.at(i).at(inBZ.at(i).size() - 1) = generate_new_start (max_B);
		}
		else {}

		//cout << min_A.X << '\t' << min_A.Y << (min_A.X * min_A.X + min_A.Y * min_A.Y) << endl;
		//cout << max_B.X << '\t' << max_B.Y << (max_B.X * max_B.X + max_B.Y * max_B.Y) << endl;

		//if (!is_start_crossing && !closed_line) buf.push_back (new_start);
		//else {}

		//buf.insert(buf.end(), inBZ.at(i).begin(), inBZ.at(i).end());

		//if (!is_end_crossing && !closed_line) buf.push_back (new_end);
		//else {}

		//out.push_back(buf);
	}

	return inBZ;
}

vector < vector <VCTR> > shorten_bezier (vector < vector <VCTR> > inBZ) {

	vector < vector <VCTR> > out;

	for (size_t i = 0; i < inBZ.size(); i++) {

		if (inBZ.at(i).size() < 10) {}
		else {

			vector <VCTR> buf;

			VCTR start  = inBZ.at(i).at(0);
			VCTR end  = inBZ.at(i).at(inBZ.at(i).size() - 1);

			buf.push_back(start);

			for (size_t k = 0; k < inBZ.at(i).size(); k+=5) {

				buf.push_back(inBZ.at(i).at(k));

			}

			if (
					buf.at(buf.size() - 1).Y != end.Y &&
					buf.at(buf.size() - 1).Y != end.Y) {

				buf.push_back(end);
			}

			out.push_back(buf);
		}
	}

	return out;
}


VCTR vctr_average (VCTR A, VCTR B) {

	VCTR out;

	out.X = (A.X + B.X) / 2.0;
	out.Y = (A.Y + B.Y) / 2.0;
	out.Z = (A.Z + B.Z) / 2.0;

	return out;
}

vector < vector <VCTR> > generate_final_bezier (vector < vector <VCTR> > inBZ) {

	vector < vector <VCTR> > out;

	for (size_t i = 0; i < inBZ.size(); i++) {

		vector <VCTR> buf;

		for (size_t j = 0; j < inBZ.at(i).size() - 2; j++) {

			VCTR A = inBZ.at(i).at(j);
			VCTR B = inBZ.at(i).at(j + 1);
			VCTR C = inBZ.at(i).at(j + 2);

			VCTR line_start = inBZ.at(i).at(0);
			VCTR line_end = inBZ.at(i).at(inBZ.at(i).size() - 1);
			VCTR before_line_end = inBZ.at(i).at(inBZ.at(i).size() - 2);

			VCTR E = vctr_average(A, B);
			VCTR F = vctr_average(B, C);
			VCTR G = vctr_average(before_line_end, A);

			bool closed_line = is_closed_line (line_start, line_end);

			if (j == 0) {

				if (closed_line) {

					buf.push_back(G);
					buf.push_back(vctr_average(G, A));
					buf.push_back(vctr_average(A, E));
					buf.push_back(E);

					buf.push_back(E);
					buf.push_back(vctr_average(E, B));
					buf.push_back(vctr_average(B, F));
					buf.push_back(F);
				}
				else {

					buf.push_back(line_start);
					buf.push_back(vctr_average(line_start, B));
					buf.push_back(vctr_average(B, F));
					buf.push_back(F);
				}
			}

			else if (j == inBZ.at(i).size() - 3 && !closed_line)  {

				buf.push_back(E);
				buf.push_back(vctr_average(E, B));
				buf.push_back(vctr_average(B, C));
				buf.push_back(C);
			}
			else {

				buf.push_back(E);
				buf.push_back(vctr_average(E, B));
				buf.push_back(vctr_average(B, F));
				buf.push_back(F);
			}
		}
		out.push_back(buf);
	}

	return out;
}

vector < vector <VCTR> > eliminate_empty_records (vector < vector <VCTR> > inBZ) {

	vector < vector <VCTR> > out;

	for (size_t i = 0; i < inBZ.size(); i++) {

		if (inBZ.at(i).size() > 3 && inBZ.at(i).at(0).X < 900.0) out.push_back(inBZ.at(i));
	}

	return out;
}

vector < vector <VCTR> > eliminate_out_circle_curves (vector < vector <VCTR> > inBZ) {

	vector < vector <VCTR> > out;

	for (size_t i = 0; i < inBZ.size(); i++) {

		bool to_eliminate = false;

		if (inBZ.at(i).size() < 5) to_eliminate = true;

		if (inBZ.at(i).size() >= 5 && !is_point_in_circle (inBZ.at(i).at(1))) to_eliminate = true;

		if (!to_eliminate) out.push_back(inBZ.at(i));
	}

	return out;
}





























void contouring (vector <GDB> inGDB, INPSET inset) {

	vector <vector <vector <VCTR> > > NET = generate_net (9);

	vector <TRIANGLE> TRI_GRID = generate_net_count (inGDB, NET, inset);

	vector <GRID_CENTER> TRI_CENTER = generate_triangle_center (TRI_GRID);

	size_t cell_number = sqrt (TRI_CENTER.size() / 0.78);

	TRI_CENTER = reduce_triangle_center(TRI_CENTER);
	//dbg_cout_triangle_center(TRI_CENTER);

	vector < vector <GRID_CENTER> > RECT_GRID = generate_rectangular_grid_from_triange_center (cell_number);
	//dbg_cout_rect_grid(RECT_GRID);

	RECT_GRID = calculate_grid_cell_values_from_triangle (RECT_GRID, TRI_CENTER);
	//dbg_cout_rect_grid(RECT_GRID);

	size_t TRI_MAX_COUNT = return_TRI_GRID_max_count (TRI_CENTER);
	//cout << "TRI_MAX_COUNT: " << TRI_MAX_COUNT << endl;

	//RECT_GRID = normalize_grid_cell_values (RECT_GRID, TRI_CENTER);
	//dbg_cout_rect_grid(RECT_GRID);

	string outputfilename = "LINE2.PS";
	ofstream o (outputfilename.c_str());
	cout_line_to_ps_header (o);
	//cout_rect_grid_to_ps (RECT_GRID, o, inset);

	//double contour_step = return_contour_step (TRI_MAX_COUNT);

	vector <size_t> CNTR_AT = return_isoline (RECT_GRID);



	//cout << "******** CONTOURING ********" << endl;
	//cout << "contour_step: " << contour_step << endl;

	//for (size_t k = 0; k < 5; k++) {
	for (size_t i = 0; i < CNTR_AT.size(); i++) {

		size_t RECT_MAX_COUNT = return_RECT_GRID_max_count (RECT_GRID);


		vector < vector <GRID_CENTER> > BIN_GRID = generate_binary_rect_grid (RECT_GRID, CNTR_AT.at(i));
		//dbg_cout_rect_grid(BIN_GRID);

		vector < vector <GRID_CENTER> > M_SQ = generate_marching_squares (BIN_GRID);

		M_SQ = check_saddle (M_SQ, RECT_GRID, CNTR_AT.at(i));
		//dbg_cout_rect_grid(M_SQ);

		vector <LINE> C_LINE = generate_raw_lines (M_SQ, RECT_GRID, CNTR_AT.at(i));

		vector <vector <LINE> > LINE_VCTR = generate_line_vector (C_LINE);

		//LINE_VCTR = connect_vectors(LINE_VCTR);
		//LINE_VCTR = tidy_vectors(LINE_VCTR);
		//dbg_cout_line_vctr(LINE_VCTR);

		vector < vector <VCTR> > BZ = generate_bezier_points (LINE_VCTR);
		//dbg_bezier_points(BZ);

		//BZ = fit_to_circle_I (BZ);

		//BZ = eliminate_out_circle_curves (BZ);

		//BZ = connect_bezier_segments (BZ);

		//BZ = eliminate_empty_records (BZ);
		//dbg_bezier_points(BZ);

		//BZ = fit_to_circle_II (BZ);
		//dbg_bezier_points(BZ);

		//BZ = shorten_bezier (BZ); //not used, not tested

		//BZ = generate_final_bezier (BZ);
		//dbg_bezier_points(BZ);

		//*****EDDIG OK
		// **** eddig ellenorizve, ertheto, amit csinal ****








		//cout_bezier_to_ps(BZ, o, CNTR_AT.size(), RECT_MAX_COUNT);
		cout_line_to_ps (C_LINE, o, CNTR_AT.at(i), RECT_MAX_COUNT);

		//cout << BZ.size() << endl;
		//cout << C_LINE.size() << endl;
	}
}

/*vector <TRIANGLE> TRI = generate_net (processGDB, inset);

//cout << TRI.size() << endl;

vector <GRID_CENTER> tri_center = generate_triangle_center (TRI);

//cout << tri_center.size() << endl;

size_t cell_number = sqrt (tri_center.size()) / 0.78;

//cout << cell_number << endl;

tri_center = reduce_triangle_center(tri_center);

vector < vector <GRID_CENTER> > RECT_grid = generate_rectangular_grid_from_triange_center (cell_number);



RECT_grid = calculate_grid_cell_values_from_triangle (RECT_grid, tri_center);

RECT_grid = normalize_grid_cell_values (RECT_grid, tri_center);
*/







void dbg_cout_triangle_center (vector <GRID_CENTER> TRI_CENTER) {

	cout << fixed << setprecision(3) << endl;

	cout << " --------- TRIANGULAR GRID  --------- " << endl;

	cout
	<< "  X  " << '\t'
	<< "  Y  " << '\t'
	<< "  Z  " << '\t'
	<< "COUNT" << endl;

	for (size_t i = 0; i < TRI_CENTER.size(); i++) {

		cout
		<< TRI_CENTER.at(i).CENTER.X << '\t'
		<< TRI_CENTER.at(i).CENTER.Y << '\t'
		<< TRI_CENTER.at(i).CENTER.Z << '\t'
		<< TRI_CENTER.at(i).COUNT << endl;
	}

	cout << " ------ end triangulat grid ------ " << endl;
}

void dbg_cout_rect_grid (vector < vector <GRID_CENTER> > rect_grid) {

	size_t cell_number = rect_grid.at(0).size();

	cout << "-------- BINARY GRID --------" << endl;

	cout << fixed << setw(2) << setprecision(2) << endl;

	for (size_t j = 0; j < cell_number; j++) {

		for (size_t i = 0; i < cell_number; i++) {

			cout
			//<< rect_grid.at(j).at(i).CENTER.X << '\t'
			//<< rect_grid.at(j).at(i).CENTER.Y << '\t'
			<< rect_grid.at(j).at(i).COUNT << '\t' << flush;
		}

		cout << endl;
	}
}

void cout_line_to_ps_header (ofstream& o) {

	o << "%!PS-Adobe-3.0 EPSF-3.0" << endl;
	o << "%%BoundingBox:  0 0 440 440" << endl;

	o << "  220.0 220.0 200.0 0.0 360.0 arc stroke"	<< endl;
}

void cout_line_to_ps (vector <LINE> l, ofstream& o, size_t isoline, size_t max_COUNT) {

	for (size_t i = 0; i < l.size(); i++) {

		o
		<< "  newpath  "
		<< l.at(i).A.X * 200.0 + 220.0 << "  " << l.at(i).A.Y * 200.0 + 220.0 << " moveto "
		<< l.at(i).B.X * 200.0 + 220.0 << "  " << l.at(i).B.Y * 200.0 + 220.0 << " lineto" << endl;

		o
		<<  isoline / max_COUNT << " "
		<<  isoline / max_COUNT << " "
		<<  " 1 setrgbcolor stroke " << endl;
	}
}

void cout_bezier_to_ps (vector < vector<VCTR> > BZ, ofstream& o, size_t isoline, double max_COUNT) {

	for (size_t i = 0; i < BZ.size(); i++) {

		o << "  newpath  " << endl;

		if (BZ.at(i).size() > 3) {

			for (size_t j = 0; j < BZ.at(i).size() - 3; j+=4) {

				o
				<< BZ.at(i).at(j + 0).X * 200.0 + 220.0 << " "
				<< BZ.at(i).at(j + 0).Y * 200.0 + 220.0 << " moveto" << endl;

				o
				<< BZ.at(i).at(j + 1).X * 200.0 + 220.0 << " "
				<< BZ.at(i).at(j + 1).Y * 200.0 + 220.0 << " "
				<< BZ.at(i).at(j + 2).X * 200.0 + 220.0 << " "
				<< BZ.at(i).at(j + 2).Y * 200.0 + 220.0 << " "
				<< BZ.at(i).at(j + 3).X * 200.0 + 220.0 << " "
				<< BZ.at(i).at(j + 3).Y * 200.0 + 220.0 << " curveto" << endl;

				o
				<<  isoline / max_COUNT << " "
				<<  isoline / max_COUNT << " "
				<<  " 1 setrgbcolor stroke " << endl;
			}
		}
		else {}
	}
}

void dbg_cout_line_vctr (vector <vector <LINE> > LV) {

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

void dbg_bezier_points (vector < vector <VCTR> > BZ) {

	cout << "BEZIER: " << endl;

	for(size_t i = 0; i < BZ.size(); i++) {

		cout << " ---------------LINE " << i << endl;

		for(size_t j = 0; j < BZ.at(i).size(); j++) {

			bool IN = BZ.at(i).at(j).X * BZ.at(i).at(j).X + BZ.at(i).at(j).Y * BZ.at(i).at(j).Y < 1.0;

			string 		in_out = "IN";
			if (!IN)	in_out = "OUT";

			cout << BZ.at(i).at(j).X << '\t' << flush;
			cout << BZ.at(i).at(j).Y << '\t' << flush;
			cout << in_out << endl;
		}

		cout << endl;
	}
}

void cout_rect_grid_to_ps (vector <vector < GRID_CENTER> > rect_grid, ofstream& o, INPSET inset) {

	//size_t cell_number = rect_grid.at(0).size();

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

			//double COUNT = 30.0 * (AW + BW + CW + DW) / (4.0 * max_COUNT);
			double COUNT = ((AW + BW + CW + DW) / 4.0);

			//cout << COUNT << endl;

			inset.grayscale = "N";

			VCTR COLOR = generate_stress_colors(COUNT, inset);

			if (COUNT > 0.01) {

				o
				<< " " << AX * 200.0 + 220.0 << " " << AY * 200.0 + 220.0 << " moveto"
				<< " " << BX * 200.0 + 220.0 << " " << BY * 200.0 + 220.0 << " lineto"
				<< " " << CX * 200.0 + 220.0 << " " << CY * 200.0 + 220.0 << " lineto"
				<< " " << DX * 200.0 + 220.0 << " " << DY * 200.0 + 220.0 << " lineto"

				<< " " << COLOR.X << " " << COLOR.Y << " " << COLOR.Z

				<< " setrgbcolor fill stroke" << endl;
			}
		}

		cout << endl;
	}
}
