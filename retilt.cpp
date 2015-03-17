
// Copyright (C) 2012 - 2014 Ã�goston SasvÃ¡ri
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <math.h>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "common.h"
#include "iomanip"
#include "retilt.hpp"
#include "settings.hpp"

using namespace std;

VCTR return_tilting_axis (const GDB& in, const bool paleonorth) {

	if (paleonorth) return unitvector (declare_vector (0.0, 0.0, -1.0));
	else return unitvector (declare_vector (
			SIN (in.avS0d.DIPDIR + 90.0),
			COS (in.avS0d.DIPDIR + 90.0),
			0.0));
}

double return_tilting_angle (const GDB& in, const bool paleonorth) {

	if (paleonorth) return (- in.PALEON);
	else {

		if (in.avS0d.DIP <= 90.0 ) return in.avS0d.DIP;
		else {

			ASSERT_DEAD_END();
			return NaN ();
		}
	}
}

bool SV_has_to_invert (const VCTR& N, const VCTR& AXIS, const double ANGLE) {

	VCTR out = ROTATE (AXIS, N, ANGLE);

	return (out.Z < 0.0);
}

GDB TILT_DATA (const GDB& in, const bool TILT_BY_PALEONORTH) {

	GDB OUT = in;

	const bool IS_PLANE = 		is_allowed_plane_datatype (in.DATATYPE);
	const bool IS_LINEATION = 	is_allowed_lineation_datatype (in.DATATYPE);
	const bool IS_SC = 			is_allowed_SC_datatype (in.DATATYPE);
	const bool IS_STRIAE = 		is_allowed_striae_datatype (in.DATATYPE);

	if (!IS_PLANE && !IS_LINEATION && !IS_SC && !IS_STRIAE) ASSERT_DEAD_END();

	const bool OVERTURNED = is_allowed_bedding_overturned_sense (in.avS0offset);

	double ANGLE = return_tilting_angle (in, TILT_BY_PALEONORTH);
	const VCTR AXIS = return_tilting_axis (in, TILT_BY_PALEONORTH);

	if (fabs (ANGLE) >= 90.0) ASSERT_DEAD_END();

	if (OVERTURNED && !TILT_BY_PALEONORTH) ANGLE = 180.0 + ANGLE;

	if (is_allowed_handle_as_bedding (in.DATATYPE)) {

		OUT.OFFSET = "";
		OUT.avS0offset = "";
	}

	if (IS_LINEATION) {

		////!OUT.D = unitvector (flip_D_vector (ROTATE (AXIS, in.D, ANGLE)));
		////!OUT.N = unitvector (flip_N_vector (NXNYNZ_from_DXDYDZ (OUT.D)));
	}
	else  {

		////!OUT.N = unitvector (flip_N_vector (ROTATE (AXIS, in.N, ANGLE)));
		////!OUT.D = unitvector (flip_D_vector (DXDYDZ_from_NXNYNZ (OUT.N)));
	}
	OUT.S = crossproduct(OUT.N, OUT.D);
	OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

	if (IS_SC || IS_STRIAE) {

		if (IS_STRIAE) {

			////!OUT.DC = unitvector (flip_D_vector (ROTATE (AXIS, in.DC, ANGLE)));
			////!OUT.NC = unitvector (flip_N_vector (NXNYNZ_from_DXDYDZ (OUT.DC)));

			const bool NO_DIRECTION = is_allowed_striae_none_sense (in.OFFSET);

			////if (NO_DIRECTION) 	OUT.SV = declare_vector (NaN(), NaN(), NaN());
			////else 				OUT.SV = unitvector (ROTATE (AXIS, in.SV, ANGLE));

			////if (SV_has_to_invert (in.N, AXIS, ANGLE)) OUT.SV = invert_VCTR(OUT.SV);
		}
		else {

			////!OUT.NC = unitvector (flip_N_vector (ROTATE (AXIS, in.NC, ANGLE)));
			////!OUT.DC = unitvector (flip_D_vector (DXDYDZ_from_NXNYNZ (OUT.NC)));
		}
		OUT.SC = crossproduct(OUT.NC, OUT.DC);
		OUT.corrL = dipdir_dip_from_DXDYDZ (OUT.DC);
	}
	return OUT;
}

GDB S0_TILT (const GDB& in) {

	GDB out = in;

	out.N  = declare_vector (NaN(), NaN(), NaN());
	out.NC = declare_vector (NaN(), NaN(), NaN());
	out.D  = declare_vector (NaN(), NaN(), NaN());
	out.DC = declare_vector (NaN(), NaN(), NaN());
	out.S  = declare_vector (NaN(), NaN(), NaN());
	out.SC = declare_vector (NaN(), NaN(), NaN());

	if (is_TILTING_BEDDING_PALEONORTH()) {

		out = TILT_DATA (in, false);
		out = TILT_DATA (out, true);
	}
	else if (is_TILTING_BEDDING()) {

		out = TILT_DATA (in, false);
	}
	else if (is_TILTING_PALEONORTH()) {

		out = TILT_DATA (in, true);
	}
	else ASSERT_DEAD_END();

	return out;
}

vector < vector < vector < vector <GDB> > > > RETILT (const vector < vector < vector < vector <GDB> > > >& inGDB) {

	vector < vector < vector < vector <GDB> > > > outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {
		for (size_t j = 0; j < outGDB.at(i).size(); j++) {
			for (size_t k = 0; k < outGDB.at(i).at(j).size(); k++) {
				for (size_t l = 0; l < outGDB.at(i).at(j).at(k).size(); l++) {

					const GDB ACT = outGDB.at(i).at(j).at(k).at(l);

					const bool DIPDIR_OK =	is_allowed_DIR (ACT.avS0d.DIPDIR);
					const bool DIP_OK =		is_allowed_DIP (ACT.avS0d.DIP);

					const bool LITHOLOGY = is_allowed_lithology_datatype (ACT.DATATYPE);

					if (DIPDIR_OK && DIP_OK && !LITHOLOGY) {

						outGDB.at(i).at(j).at(k).at(l) = S0_TILT (ACT);
					}
				}
			}
		}
	}
	return outGDB;
}
