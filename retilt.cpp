
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

VCTR return_tilting_axis (const GDB& in, const string METHOD) {

	const bool B = METHOD == "BEDDING";
	const bool P = METHOD == "PALEONORTH";
	const bool T = METHOD == "TRAJECTORY";

	if (P) return unitvector (declare_vector (0.0, 0.0, -1.0));
	else if (B) return unitvector (declare_vector (
			SIN (in.avS0d.DIPDIR + 90.0),
			COS (in.avS0d.DIPDIR + 90.0),
			0.0));
	else if (T) {

		const DIPDIR_DIP DDD = dipdir_dip_from_NXNYNZ(in.T);

		//cout << "for axis: " << DDD.DIPDIR << "/" << DDD.DIP << endl;

		//VCTR dummy = unitvector (declare_vector (
		//		SIN (DDD.DIPDIR + 90.0),
		//		COS (DDD.DIPDIR + 90.0),
		//		0.0));

		//cout << dummy.X << "  -  "<< dummy.Y << "  -  " << dummy.Z << endl;

		return unitvector (declare_vector (
				SIN (DDD.DIPDIR + 90.0),
				COS (DDD.DIPDIR + 90.0),
				0.0));
	}
	else {

		ASSERT_DEAD_END();
		return declare_vector (NaN(), NaN(), NaN());
	}
}

double return_tilting_angle (const GDB& in, const string METHOD) {

	const bool B = METHOD == "BEDDING";
	const bool P = METHOD == "PALEONORTH";
	const bool T = METHOD == "TRAJECTORY";

	if (P) return (- in.PALEON);
	else if (B) {

		if (in.avS0d.DIP <= 90.0 ) return in.avS0d.DIP;
		else {

			ASSERT_DEAD_END();
			return NaN ();
		}
	}
	else if (T) {

		const DIPDIR_DIP DDD = dipdir_dip_from_NXNYNZ(in.T);

		//cout << DDD.DIPDIR << "/" << DDD.DIP << endl;
		if (DDD.DIP <= 90.0 ) return -(90.0 - DDD.DIP);
		else {

			ASSERT_DEAD_END();
			return NaN ();

		}
	}
	else {
		ASSERT_DEAD_END();
		return NaN();
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

	//cout << "www1" << endl;

	//cout << " ---------------------------------- " << endl;
	//cout <<"AXIS"<<endl;
	//cout <<"===="<<endl;
	//cout << AXIS.X << " - " << AXIS.Y << " - " << AXIS.Z << endl;

	//cout <<"PLANE NORMAL"<<endl;
	//cout <<"============"<<endl;
	//cout << in.N.X << " - " << in.N.Y << " - " << in.N.Z << endl;

	//cout << "ANGLE: " << ANGLE << endl;
	//cout << " ---------------------------------- " << endl;

	OUT.N = unitvector (ROTATE (AXIS, in.N, ANGLE));

	//cout << "www2" << endl;

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

GDB TILT_DATA (const GDB& in, const string METHOD) {

	const bool B = METHOD == "BEDDING";
	const bool P = METHOD == "PALEONORTH";
	const bool T = METHOD == "TRAJECTORY";

	if (!B && !P && !T) ASSERT_DEAD_END();

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

	const bool O = is_allowed_bedding_overturned_sense (in.avS0offset);

	double ANGLE = return_tilting_angle (in, METHOD);

	//cout << in.ID << endl;
	//cout << fixed << setprecision(6) << ANGLE << endl;
	const VCTR AXIS = return_tilting_axis (in, METHOD);

	//cout << in.DATATYPE << "  -  " << in.corr.DIPDIR << "/" << in.corr.DIP << endl;

	if (O && !P && !T) ANGLE = 180.0 + ANGLE;

	if (IS_LN) 			return tilt_lineation (in, AXIS, ANGLE);
	else if (IS_PL) 	return tilt_plane (in, AXIS, ANGLE);
	else if (IS_ST) 	return tilt_striae (in, AXIS, ANGLE);
	else if (IS_SC)   	return tilt_SC (in, AXIS, ANGLE);
	else ASSERT_DEAD_END();

	cout << "4444" << endl;

	return in;
}

/*
GDB S0_TILT (const GDB& in, const string METHOD) {


	//const bool DD =	is_allowed_DIR (ACT.avS0d.DIPDIR);
	//const bool D = is_allowed_DIP (ACT.avS0d.DIP);
	//const bool L = is_allowed_lithology_datatype (ACT.DATATYPE);




	GDB out = in;

	if (is_TILTING_BEDDING_PALEONORTH()) {

		out = TILT_DATA (in, false);
		out = TILT_DATA (out, true);
	}
	else if (is_TILTING_BEDDING()) 		out = TILT_DATA (in, false);
	else if (is_TILTING_PALEONORTH()) 	out = TILT_DATA (in, true);
	else ASSERT_DEAD_END();

	return out;
}
*/
vector < vector <GDB> > RETILT (const vector < vector <GDB> >& inGDB, const string METHOD) {

	vector < vector <GDB> > outGDB = inGDB;

	for (size_t i = 0; i < outGDB.size(); i++) {
		for (size_t j = 0; j < outGDB.at(i).size(); j++) {

			outGDB.at(i).at(j) = TILT_DATA (outGDB.at(i).at(j), METHOD);
		}
	}
	return outGDB;
}

/*
 * vector < vector < vector < vector <GDB> > > > RETILT (const vector < vector < vector < vector <GDB> > > >& inGDB) {

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
 */
