// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <math.h>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "common.h"
#include "iomanip"
#include "retilt.hpp"
#include "rgf.h"
#include "settings.hpp"

using namespace std;

VCTR return_tilting_axis (const GDB& in, const string METHOD) {

	const bool B = METHOD == "BEDDING";
	const bool P = METHOD == "PALEONORTH";
	const bool T = METHOD == "TRAJECTORY";

	ASSERT (B || P || T);

	if (P) return unitvector (declare_vector (0.0, 0.0, -1.0), true);
	else if (B) return unitvector (declare_vector (
			SIN (in.avS0d.DIPDIR + 90.0),
			COS (in.avS0d.DIPDIR + 90.0),
			0.0), true);
	else {

		const DIPDIR_DIP DDD = dipdir_dip_from_NXNYNZ(in.T);

		return unitvector (declare_vector (
				SIN (DDD.DIPDIR + 90.0),
				COS (DDD.DIPDIR + 90.0),
				0.0), true);
	}
}

double return_tilting_angle (const GDB& in, const string METHOD) {

	const bool B = METHOD == "BEDDING";
	const bool P = METHOD == "PALEONORTH";
	const bool T = METHOD == "TRAJECTORY";

	ASSERT (B || P || T);

	if (P) return (- in.PALEON);
	else if (B) {

		ASSERT (in.avS0d.DIP <= 90.0);

		return in.avS0d.DIP;

	}
	else {

		const DIPDIR_DIP DDD = dipdir_dip_from_NXNYNZ(in.T);

		ASSERT (DDD.DIP <= 90.0);

		return -(90.0 - DDD.DIP);
	}
}

GDB tilt_lineation (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	GDB OUT = in;

	OUT.D = ROTATE (AXIS, in.D, ANGLE);

	if (is_D_up (OUT.D)) OUT.D = flip_vector (OUT.D);

	OUT.N = unitvector (NXNYNZ_from_DXDYDZ (OUT.D), true);
	OUT.S = crossproduct (OUT.N, OUT.D);
	OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

	return OUT;
}

GDB tilt_plane (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	GDB OUT = in;

	const bool B = is_allowed_handle_as_bedding(in.DATATYPE);

	OUT.N = ROTATE (AXIS, in.N, ANGLE);

	bool O = is_N_down (OUT.N);

	if (B) {

		OUT.OFFSET = "NORMAL";
		if (O) OUT.OFFSET = "OVERTURNED";
	}
	else {

		if (O) OUT.N = flip_vector(OUT.N);
	}
	OUT.D = unitvector (DXDYDZ_from_NXNYNZ (OUT.N), true);
	OUT.S = crossproduct(OUT.N, OUT.D);
	OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

	return OUT;
}

GDB tilt_striae (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	vector <GDB> TEMP;

	TEMP.push_back(in);

	const double ANG_IN = fabs (vector_angle (in.N, in.DC));

	GDB OUT = in;

	OUT.N = ROTATE (AXIS, in.N, ANGLE);

	const bool O = is_N_down(OUT.N);

	if (O) OUT.N = flip_vector(OUT.N);

	OUT.D = unitvector (DXDYDZ_from_NXNYNZ (OUT.N), true);
	OUT.S = crossproduct(OUT.N, OUT.D);
	OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

	OUT.DC = ROTATE (AXIS, in.DC, ANGLE);

	if (O) OUT.DC  = flip_vector(OUT.DC);

	const bool UP = is_DC_up(OUT.DC);

	OUT.NC = unitvector (NXNYNZ_from_DXDYDZ (OUT.DC), true);

	OUT.SC = crossproduct(OUT.NC, OUT.DC);

	if (UP) OUT.corrL = dipdir_dip_from_DXDYDZ (flip_vector (OUT.DC));
	else OUT.corrL = dipdir_dip_from_DXDYDZ (OUT.DC);

	const double ANG_OUT = fabs (vector_angle (OUT.N, OUT.DC));

	TEMP.push_back(OUT);

	ASSERT (is_in_range(ANG_IN, ANG_IN, ANG_OUT));

	ASSERT (is_in_range(089.9999, 090.0001, ANG_OUT));

	ASSERT (is_in_range(089.9999, 090.0001, ANG_IN));

	return OUT;
}

GDB tilt_SC (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	GDB OUT = in;

	OUT = tilt_plane (OUT, AXIS, ANGLE);

	OUT.NC = ROTATE (AXIS, OUT.NC, ANGLE);

	const bool O = is_N_down (OUT.NC);

	if (O) OUT.NC = flip_vector(OUT.NC);

	OUT.DC = unitvector (DXDYDZ_from_NXNYNZ (OUT.NC), true);

	OUT.SC = crossproduct(OUT.NC, OUT.DC);

	OUT.corrL = dipdir_dip_from_DXDYDZ (OUT.DC);

	return OUT;
}

GDB TILT_DATA (const GDB& in, const string METHOD) {

	const bool LITH = is_allowed_lithology_datatype (in.DATATYPE);

	const bool NO_NORTH = (in.PALEON < -1000) ;

	if (LITH) return in;

	const bool B = METHOD == "BEDDING";
	const bool P = METHOD == "PALEONORTH";
	const bool T = METHOD == "TRAJECTORY";

	ASSERT (B || P || T);

	ASSERT (in.HAS_BEDDING == 0 || in.HAS_BEDDING == 1 );

	if (in.HAS_BEDDING == 0 && B) return in;

	if (NO_NORTH && P) return in;

	if (B) {

		const bool DD =	is_allowed_DIR (in.avS0d.DIPDIR);

		const bool D = is_allowed_DIP (in.avS0d.DIP);

		const bool L = is_allowed_lithology_datatype (in.DATATYPE);

		if (!DD || !D || L) return in;
	}
	else if (P) {

		const bool PN =	is_allowed_DIR (fabs(in.PALEON));

		if (!PN) return in;
	}
	else if (T) {

		const bool TR = is_allowed_N_vector (in.T);

		if (!TR) return in;
	}
	else {}

	const bool IS_PL = is_allowed_plane_datatype (in.DATATYPE);
	const bool IS_LN = is_allowed_lineation_datatype (in.DATATYPE);
	const bool IS_SC = is_allowed_SC_datatype (in.DATATYPE);
	const bool IS_ST = is_allowed_striae_datatype (in.DATATYPE);

	ASSERT_EXACTLY_ONE_TRUE (IS_PL, IS_LN, IS_SC, IS_ST);

	const bool O = is_allowed_bedding_overturned_sense (in.avS0offset);

	double ANGLE = return_tilting_angle (in, METHOD);

	const VCTR AXIS = return_tilting_axis (in, METHOD);

	if (O && !P && !T) ANGLE = 180.0 + ANGLE;

	if (IS_LN) 			return tilt_lineation (in, AXIS, ANGLE);
	else if (IS_PL) 	return tilt_plane (in, AXIS, ANGLE);
	else if (IS_ST) 	return tilt_striae (in, AXIS, ANGLE);
	else 			  	return tilt_SC (in, AXIS, ANGLE);

	return in;
}

vector < vector <GDB> > RETILT (const vector < vector <GDB> >& inGDB, const string METHOD) {

	vector < vector <GDB> > outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {

		for (size_t j = 0; j < outGDB.at(i).size(); j++) {

			outGDB.at(i).at(j) = TILT_DATA (outGDB.at(i).at(j), METHOD);
		}
	}
	return outGDB;
}
