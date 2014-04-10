// Copyright (C) 2012 - 2014 Ã�goston SasvÃ¡ri
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <math.h>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "common.h"
#include "retilt.hpp"

using namespace std;

VCTR return_tilting_axis (GDB in, bool paleonorth) {

	VCTR AXIS;

	if (paleonorth) AXIS = declare_vector (
			0.0,
			0.0,
			-1.0);
	else AXIS = declare_vector (
			SIN (in.avS0d.DIPDIR + 90.0),
			COS (in.avS0d.DIPDIR + 90.0),
			0.0);

	return unitvector (AXIS);
}

double return_tilting_angle (GDB in, bool paleonorth) {

	if (paleonorth) return (- in.PALEON);
	else {

		if (in.avS0d.DIP <= 90.0) return in.avS0d.DIP;
		else return 999.99;
	}
}

VCTR ROT_generate_N_vector_plane (VCTR N, VCTR AXIS, double ANGLE) {

	VCTR out = ROTATE (AXIS, N, ANGLE);

	out =  flip_N_vector (out);

	return unitvector(out);
}

VCTR ROT_generate_N_vector_lineation (VCTR D) {

	VCTR out = NXNYNZ_from_DXDYDZ (D);

	out = flip_N_vector (out);

	return unitvector(out);
}

VCTR ROT_generate_D_vector_plane (VCTR N) {

	VCTR out = DXDYDZ_from_NXNYNZ (N);

	out = flip_D_vector (out);

	return unitvector(out);
}

VCTR ROT_generate_D_vector_lineation (VCTR D, VCTR AXIS, double ANGLE) {

	VCTR out = ROTATE (AXIS, D, ANGLE);

	out = flip_D_vector (out);

	return unitvector(out);
}

bool SV_has_to_invert (VCTR N, VCTR AXIS, double ANGLE) {

	VCTR out = ROTATE (AXIS, N, ANGLE);

	return (out.Z < 0.0);
}

GDB TILT_DATA (GDB in, INPSET inset, bool by_paleonorth) {

	GDB OUT = in;

	bool IS_PLANE = 		is_allowed_plane_datatype		(in.DATATYPE);
	bool IS_LINEATION = 	is_allowed_lineation_datatype	(in.DATATYPE);
	bool IS_SC = 			is_allowed_SC_datatype			(in.DATATYPE);
	bool IS_STRIAE = 		is_allowed_striae_datatype		(in.DATATYPE);

	if (!IS_PLANE && !IS_LINEATION && !IS_SC && !IS_STRIAE) ASSERT_DEAD_END();

	bool TILT_BY_PALEONORTH = by_paleonorth;

	bool OVERTURNED = is_allowed_bedding_overturned_sense(in.avS0offset);
	bool NO_DIRECTION = is_allowed_striae_none_sense(in.OFFSET);

	double ANGLE = 	return_tilting_angle (in, TILT_BY_PALEONORTH);
	VCTR AXIS = 	return_tilting_axis(in, TILT_BY_PALEONORTH);

	if (fabs(ANGLE) < 0.01 || fabs(ANGLE) > 900.00) return in;

	if (OVERTURNED && !TILT_BY_PALEONORTH) ANGLE = 180.0 + ANGLE;

	if (IS_PLANE) {

		OUT.N = ROT_generate_N_vector_plane (in.N, AXIS, ANGLE);
		OUT.D = ROT_generate_D_vector_plane (OUT.N);
		OUT.S = crossproduct(OUT.N, OUT.D);
		OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

		if ((OUT.DATATYPE) == "BEDDING") {
			OUT.OFFSET = "";
			OUT.avS0offset = "";
		}
		else{}
	}
	else if (IS_LINEATION) {

		OUT.D = ROT_generate_D_vector_lineation (in.D, AXIS, ANGLE);
		OUT.N = ROT_generate_N_vector_lineation (OUT.D);
		OUT.S = crossproduct(OUT.D, OUT.N);
		OUT.corr = dipdir_dip_from_NXNYNZ (OUT.N);
	}
	else if (IS_SC) {

		OUT.SV = unitvector(ROTATE (AXIS, in.SV, ANGLE));

		OUT.NC = ROT_generate_N_vector_plane (in.NC, AXIS, ANGLE);
		OUT.DC = ROT_generate_D_vector_plane (OUT.NC);
		OUT.SC = crossproduct(OUT.NC, OUT.DC);
		OUT.corrL = dipdir_dip_from_DXDYDZ (OUT.DC);

		OUT.N = ROT_generate_N_vector_plane (in.N, AXIS, ANGLE);
		OUT.D = ROT_generate_D_vector_plane (OUT.N);
		OUT.S = crossproduct(OUT.N, OUT.D);
		OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

		if (SV_has_to_invert (in.N, AXIS, ANGLE)) OUT.SV = invert_VCTR(OUT.SV);
	}
	else if (IS_STRIAE) {

		OUT.DC = ROT_generate_D_vector_lineation (in.DC, AXIS, ANGLE);
		OUT.NC = ROT_generate_N_vector_lineation (OUT.DC);
		OUT.SC = crossproduct(OUT.DC, OUT.NC);
		OUT.corrL = dipdir_dip_from_DXDYDZ (OUT.DC);

		if (NO_DIRECTION) 	OUT.SV = declare_vector (0.0, 0.0, 0.0);
		else 				OUT.SV = unitvector(ROTATE (AXIS, in.SV, ANGLE));

		OUT.N = ROT_generate_N_vector_plane (in.N, AXIS, ANGLE);
		OUT.D = ROT_generate_D_vector_plane (OUT.N);
		OUT.S = crossproduct(OUT.N, OUT.D);
		OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

		if (SV_has_to_invert (in.N, AXIS, ANGLE)) OUT.SV = invert_VCTR(OUT.SV);
	}
	else ASSERT_DEAD_END();

	return OUT;
}

GDB S0_TILT (GDB inGDB, INPSET inSET) {

	GDB outGDB;

	outGDB.N  = declare_vector (999.99, 999.99, 999.99);
	outGDB.NC = declare_vector (999.99, 999.99, 999.99);
	outGDB.D  = declare_vector (999.99, 999.99, 999.99);
	outGDB.DC = declare_vector (999.99, 999.99, 999.99);
	outGDB.S  = declare_vector (999.99, 999.99, 999.99);
	outGDB.SC = declare_vector (999.99, 999.99, 999.99);

	bool TILT_BY_BEDDING = 		(inSET.tilting == "B");
	bool TILT_BY_PALEONORTH = 	(inSET.tilting == "P");
	bool TILT_BY_ALL = 			(inSET.tilting == "A");
	if (!TILT_BY_BEDDING && !TILT_BY_PALEONORTH && !TILT_BY_ALL) ASSERT_DEAD_END();

	if (TILT_BY_ALL) {

		outGDB = TILT_DATA (inGDB, inSET, false);
		outGDB = TILT_DATA (outGDB, inSET, true);
	}
	else if (TILT_BY_BEDDING) outGDB = TILT_DATA (inGDB, inSET, false);
	else if (TILT_BY_PALEONORTH)outGDB = TILT_DATA (inGDB, inSET, true);
	else ASSERT_DEAD_END();

	return outGDB;
}

vector <GDB> cGc_RETILT (vector <GDB> inGDB, INPSET inSET) {

	vector <GDB> outGDB = inGDB;

	size_t i = 0;

	bool TILT_BY_BEDDING = 		(inSET.tilting == "B");
	bool TILT_BY_PALEONORTH = 	(inSET.tilting == "P");
	bool TILT_BY_ALL = 			(inSET.tilting == "A");
	if (!TILT_BY_BEDDING && !TILT_BY_PALEONORTH && !TILT_BY_ALL) ASSERT_DEAD_END();

	for (i = 0; i < inGDB.size(); i++) outGDB.at(i) = S0_TILT (inGDB.at(i), inSET);

	if (TILT_BY_BEDDING    || TILT_BY_ALL) 	cout << "  - Retilting all of  " << i << " data records by the bedding has done." <<  endl;
	if (TILT_BY_PALEONORTH || TILT_BY_ALL) 	cout << "  - Retilting all of  " << i << " data records by the Paleo-North data has done." <<  endl;

	return outGDB;
}
