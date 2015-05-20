
// Copyright (C) 2012 - 2015 Ã�goston SasvÃ¡ri
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

VCTR return_tilting_axis (const GDB& in, const bool paleonorth) {


	//cout << in.avS0d.DIPDIR << "/" << in.avS0d.DIP << endl;

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

GDB tilt_lineation (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	GDB OUT = in;

	OUT.D = unitvector (ROTATE (AXIS, in.D, ANGLE));

	if (is_D_up (OUT.D)) OUT.D = flip_vector (OUT.D);

	OUT.N = unitvector (NXNYNZ_from_DXDYDZ (OUT.D));
	OUT.S = crossproduct (OUT.N, OUT.D);
	OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

	return OUT;
}

GDB tilt_plane (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	GDB OUT = in;

	const bool B = is_allowed_handle_as_bedding(in.DATATYPE);

	OUT.N = unitvector (ROTATE (AXIS, in.N, ANGLE));

	bool O = is_N_down (OUT.N);

	if (B) {

		OUT.OFFSET = "NORMAL";
		if (O) OUT.OFFSET = "OVERTURNED";
	}
	else {

		if (O) OUT.N = flip_vector(OUT.N);
	}
	OUT.D = unitvector (DXDYDZ_from_NXNYNZ (OUT.N));
	OUT.S = crossproduct(OUT.N, OUT.D);
	OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);

	return OUT;
}

GDB tilt_striae (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	const double ANG_IN = fabs (vector_angle (in.N, in.DC));

	GDB OUT = in;

	OUT.N = unitvector (ROTATE (AXIS, in.N, ANGLE));

	const bool O = is_N_down(OUT.N);

	if (O) OUT.N = flip_vector(OUT.N);

	OUT.D = unitvector (DXDYDZ_from_NXNYNZ (OUT.N));
	OUT.S = crossproduct(OUT.N, OUT.D);
	OUT.corr = dipdir_dip_from_DXDYDZ (OUT.D);


	OUT.DC = unitvector (ROTATE (AXIS, in.DC, ANGLE));

	if (O) OUT.DC  = flip_vector(OUT.DC);

	const bool UP = is_DC_up(OUT.DC);

	OUT.NC = unitvector (NXNYNZ_from_DXDYDZ (OUT.DC));

	OUT.SC = crossproduct(OUT.NC, OUT.DC);

	if (UP) OUT.corrL = dipdir_dip_from_DXDYDZ (flip_vector (OUT.DC));
	else OUT.corrL = dipdir_dip_from_DXDYDZ (OUT.DC);

	const double ANG_OUT = fabs (vector_angle (OUT.N, OUT.DC));

	if (! is_in_range(ANG_IN, ANG_IN, ANG_OUT)) ASSERT_DEAD_END();
	if (! is_in_range(090.0, 090.0, ANG_OUT)) ASSERT_DEAD_END();
	if (! is_in_range(090.0, 090.0, ANG_IN)) ASSERT_DEAD_END();

	return OUT;
}

GDB tilt_SC (const GDB& in, const VCTR& AXIS, const double ANGLE) {

	const double ANG_IN = fabs (vector_angle (in.N, in.NC));

	GDB OUT = in;

	OUT = tilt_plane (OUT, AXIS, ANGLE);

	OUT.NC = unitvector (ROTATE (AXIS, OUT.NC, ANGLE));

	const bool O = is_N_down (OUT.NC);

	if (O) OUT.NC = flip_vector(OUT.NC);

	OUT.DC = unitvector (DXDYDZ_from_NXNYNZ (OUT.NC));

	OUT.SC = crossproduct(OUT.NC, OUT.DC);

	OUT.corrL = dipdir_dip_from_DXDYDZ (OUT.DC);

	const double ANG_OUT = fabs (vector_angle (OUT.N, OUT.NC));

	if (! is_in_range(ANG_IN, ANG_IN, ANG_OUT)) ASSERT_DEAD_END();

	return OUT;
}

GDB TILT_DATA (const GDB& in, const bool TILT_BY_PALEONORTH) {

	const bool IS_PL = is_allowed_plane_datatype (in.DATATYPE);
	const bool IS_LN = is_allowed_lineation_datatype (in.DATATYPE);
	const bool IS_SC = is_allowed_SC_datatype (in.DATATYPE);
	const bool IS_ST = is_allowed_striae_datatype (in.DATATYPE);

	const bool O = is_allowed_bedding_overturned_sense (in.avS0offset);

	double ANGLE = return_tilting_angle (in, TILT_BY_PALEONORTH);
	const VCTR AXIS = return_tilting_axis (in, TILT_BY_PALEONORTH);

	//cout << "ANGLE : " << ANGLE << endl;
	//cout << "AXIS  : " << AXIS.X << " - " << AXIS.Y << " - " << AXIS.Z << " - " << endl;

	if (O && !TILT_BY_PALEONORTH) ANGLE = 180.0 + ANGLE;

	if (IS_LN) 			return tilt_lineation (in, AXIS, ANGLE);
	else if (IS_PL) 	return tilt_plane (in, AXIS, ANGLE);
	else if (IS_ST) 	return tilt_striae (in, AXIS, ANGLE);
	else if (IS_SC)   	return tilt_SC (in, AXIS, ANGLE);
	else ASSERT_DEAD_END();

	return in;
}

GDB S0_TILT (const GDB& in) {

	GDB out = in;

	//cout << "is_TILTING_BEDDING_PALEONORTH(): " << is_TILTING_BEDDING_PALEONORTH() << endl;
	//cout << "is_TILTING_BEDDING()           : " << is_TILTING_BEDDING() << endl;
	//cout << "is_TILTING_PALEONORTH()        : " << is_TILTING_PALEONORTH() << endl;

	if (is_TILTING_BEDDING_PALEONORTH()) {

		out = TILT_DATA (in, false);
		out = TILT_DATA (out, true);
	}
	else if (is_TILTING_BEDDING()) 		out = TILT_DATA (in, false);
	else if (is_TILTING_PALEONORTH()) 	out = TILT_DATA (in, true);
	else ASSERT_DEAD_END();

	return out;
}

vector < vector < vector < vector <GDB> > > > RETILT (const vector < vector < vector < vector <GDB> > > >& inGDB) {

	vector < vector < vector < vector <GDB> > > > outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {
		for (size_t j = 0; j < outGDB.at(i).size(); j++) {
			for (size_t k = 0; k < outGDB.at(i).at(j).size(); k++) {

				//dbg_cout_GDB_vector (outGDB.at(i).at(j).at(k));

				for (size_t l = 0; l < outGDB.at(i).at(j).at(k).size(); l++) {

					const GDB ACT = outGDB.at(i).at(j).at(k).at(l);

					const bool DD =	is_allowed_DIR (ACT.avS0d.DIPDIR);
					const bool D = is_allowed_DIP (ACT.avS0d.DIP);
					const bool L = is_allowed_lithology_datatype (ACT.DATATYPE);

					if (DD && D && !L) outGDB.at(i).at(j).at(k).at(l) = S0_TILT (ACT);

				}
				//dbg_cout_GDB_vector (outGDB.at(i).at(j).at(k));
			}
		}
	}
	return outGDB;
}
