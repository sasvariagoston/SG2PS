// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
//#include <cmath>
#include <iostream>
//#include <iomanip>
#include <vector>

//#include "average.hpp"
//#include "allowed_keys.hpp"
#include "assertions.hpp"
//#include "bingham.h"
#include "brute_force.hpp"
//#include "checkrgffilecontent.h"
//#include "checkxycontent.h"
//#include "data_io.h"
#include "inversion.h"
//#include "ps.h"
#include "rakhmanov.hpp"
//#include "random.hpp"
//#include "retilt.hpp"
//#include "rgf.h"
#include "stresstensor.hpp"
#include "structs.h"
#include "yamaji.hpp"

using namespace std;

vector <STRESSTENSOR> generate_stresstensors_for_yamaji () {

	vector <STRESSTENSOR> TNSR;

	const vector <VCTR> CNTRVCTR = return_rakhmanov_points(256); //OK
	const vector <double> ANGVCTR = generate_angle_vector_180 (0.0, 180.0, 16); //OK
	const vector <double> PHIVCTR = generate_phi_vector (0.0, 1.0, 15); //OK

	for (size_t cntr = 0; cntr < CNTRVCTR.size(); cntr++){
		for (size_t ang = 0; ang < ANGVCTR.size(); ang++){
			for (size_t phi = 0; phi < PHIVCTR.size(); phi++){

				STRESSTENSOR buf = return_stresstensor_from_n1_ang_phi (CNTRVCTR.at(cntr), ANGVCTR.at(ang), PHIVCTR.at(phi));

				TNSR.push_back(buf);
			}
		}
	}
	return TNSR;
}

vector <vector <VCTR> > generate_GDB_VECTORS_for_yamaji (const vector <GDB>& inGDB, const string& METHOD) {

	vector <vector <VCTR> > OUT;

	bool NORMALVECTOR = false;
	bool SLIPVECTOR = false;

	if (METHOD == "N") NORMALVECTOR = true;
	if (METHOD == "S") SLIPVECTOR = true;

	if (!NORMALVECTOR && !SLIPVECTOR) ASSERT_DEAD_END();

	for (size_t i = 0; i < inGDB.size() - 3; i++) {
		for (size_t j = i + 1; j < inGDB.size() - 2; j++) {
			for (size_t k = j + 1; k < inGDB.size() - 1; k++) {
				for (size_t l = k + 1; l < inGDB.size() - 0; l++) {

					vector <VCTR> tempVCTR;

					if (NORMALVECTOR) {

						tempVCTR.push_back(inGDB.at(i).N);
						tempVCTR.push_back(inGDB.at(j).N);
						tempVCTR.push_back(inGDB.at(k).N);
						tempVCTR.push_back(inGDB.at(l).N);
					}
					else {
						tempVCTR.push_back(inGDB.at(i).SV);
						tempVCTR.push_back(inGDB.at(j).SV);
						tempVCTR.push_back(inGDB.at(k).SV);
						tempVCTR.push_back(inGDB.at(l).SV);
					}
					OUT.push_back(tempVCTR);
				}
			}
		}
	}
	return OUT;
}

const static double SUBSET_MISFIT (const vector <VCTR>& N_GDB, const vector <VCTR>& S_GDB, const STRESSTENSOR& ST) {

	if (N_GDB.size() != 4) ASSERT_DEAD_END();
	if (S_GDB.size() != 4) ASSERT_DEAD_END();

	double MISFIT = 0.0;

	for (size_t i = 0; i < 4; i++) {

		double s11 = ST._11;
		double s12 = ST._12;
		double s13 = ST._13;
		double s22 = ST._22;
		double s23 = ST._23;
		double s33 = ST._33;

		double Nx = N_GDB.at(i).X;
		double Ny = N_GDB.at(i).Y;
		double Nz = N_GDB.at(i).Z;

		double Sx = S_GDB.at(i).X;
		double Sy = S_GDB.at(i).Y;
		double Sz = S_GDB.at(i).Z;

		VCTR iS = declare_vector(

		s11*Nx*Nx*Nx + s22*Nx*Ny*Ny + s33*Nx*Nz*Nz + 2.0*s12*Nx*Nx*Ny + 2.0*s13*Nx*Nx*Nz + 2.0*s23*Nx*Ny*Nz,
		s11*Nx*Nx*Ny + s22*Ny*Ny*Ny + s33*Ny*Nz*Nz + 2.0*s12*Nx*Ny*Ny + 2.0*s13*Nx*Ny*Nz + 2.0*s23*Ny*Ny*Nz,
		s11*Nx*Nx*Nz + s22*Ny*Ny*Nz + s33*Nz*Nz*Nz + 2.0*s12*Nx*Ny*Nz + 2.0*s13*Nx*Nz*Nz + 2.0*s23*Ny*Nz*Nz
		);

		iS = unitvector(iS);

		double ind_MISFIT = ACOS (iS.X * Sx + iS.Y * Sy + iS.Z * Sz);

		MISFIT = MISFIT + ind_MISFIT;
	}
	return MISFIT;
}

vector <BRUTEFORCE_RESULT> st_YAMAJI (const vector <GDB>& inGDB) {

	vector <BRUTEFORCE_RESULT> OUT;

	vector <STRESSTENSOR> TNSR = generate_stresstensors_for_yamaji ();

	vector <vector <VCTR> > N_GDB = generate_GDB_VECTORS_for_yamaji (inGDB, "N");
	vector <vector <VCTR> > S_GDB = generate_GDB_VECTORS_for_yamaji (inGDB, "S");

	cout << "YAMAJI INVERSION" << endl;

	for (size_t str = 0; str < TNSR.size(); str++) {
		for (size_t rec_cnt = 0; rec_cnt < N_GDB.size(); rec_cnt++) {

			//double MISFIT = SUBSET_MISFIT (N_GDB.at(rec_cnt), S_GDB.at(rec_cnt), TNSR.at(str));
		}
		cout << str << endl;
	}
	return OUT;
}
