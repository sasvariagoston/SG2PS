// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

//#include <algorithm>
//#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

//#include "average.hpp"
//#include "allowed_keys.hpp"
#include "assertions.hpp"
//#include "bingham.h"
#include "brute_force.hpp"
//#include "checkrgffilecontent.h"
//#include "checkxycontent.h"
#include "common.h"
//#include "data_io.h"
#include "inversion.h"
//#include "ps.h"
#include "rakhmanov.hpp"
//#include "random.hpp"
//#include "retilt.hpp"
//#include "rgf.h"
#include "stdlib.h"
#include "stresstensor.hpp"
#include "structs.h"
#include "yamaji.hpp"

using namespace std;

struct YAMAJI {

	STRESSTENSOR ST;
	double AXIS_ID;
	double ANGLE_ID;
	double PHI_ID;
	size_t COUNT;

};

namespace {

vector <VCTR> AXES;
vector <double> ANGLES;
vector <double> PHI;

//vector <vector <size_t> > U;

const size_t CLUSTER_SIZE = 5;

}
/*
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


*/
/*

	vector <vector <VCTR> > OUT;

	bool NORMALVECTOR = false;
	bool SLIPVECTOR = false;

	if (METHOD == "N") NORMALVECTOR = true;
	if (METHOD == "S") SLIPVECTOR = true;

	//if (!NORMALVECTOR && !SLIPVECTOR) ASSERT_DEAD_END();

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
						tempVCTR.push_back(inGDB.at(i).DC);
						tempVCTR.push_back(inGDB.at(j).DC);
						tempVCTR.push_back(inGDB.at(k).DC);
						tempVCTR.push_back(inGDB.at(l).DC);
					}
					OUT.push_back(tempVCTR);
				}
			}
		}
	}
	return OUT;
}

*/
/*
const static double SUBSET_MISFIT (const vector <VCTR>& N_GDB, const vector <VCTR>& S_GDB, const STRESSTENSOR& ST) {

	//if (N_GDB.size() != 4) ASSERT_DEAD_END();
	//if (S_GDB.size() != 4) ASSERT_DEAD_END();

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
*/
/*
vector <vector <GDB> > init_subsets (const size_t GDB_SIZE, const size_t CLUSTER_SIZE) {

	const double N = factorial (GDB_SIZE);//ok

	const double k = factorial (CLUSTER_SIZE);//ok

	const double N_k = factorial (GDB_SIZE - CLUSTER_SIZE);//ok

	const double C = N / (k * N_k);

	cout << C << endl;

	vector <vector <GDB> > OUT;

	for (size_t i = 1; i < C; i++) {

		vector <GDB> buf;

		OUT.push_back (buf);
	}
	return OUT;
}
*/

void combine_subsets (const size_t GDB_SIZE, const size_t CLUSTER_SIZE, vector <size_t> SV, vector <vector <size_t> >& U) {

	for (size_t i = GDB_SIZE; i >= CLUSTER_SIZE; i--) {

		SV.at (CLUSTER_SIZE - 1) = i;

		if (CLUSTER_SIZE > 1) combine_subsets (i - 1, CLUSTER_SIZE - 1, SV, U);

		else U.push_back (SV);
	}
}

vector <vector <size_t> > generate_subsets_vector (const vector <GDB>& inGDB) {

	const size_t GDB_SIZE = inGDB.size();

	const double N = factorial (GDB_SIZE);
	const double k = factorial (CLUSTER_SIZE);
	const double N_k = factorial (GDB_SIZE - CLUSTER_SIZE);

	const double C = N / (k * N_k);

	vector <size_t> SV = {999, 999, 999, 999, 999};

	vector <vector <size_t> > OUT;

	combine_subsets (GDB_SIZE, CLUSTER_SIZE, SV, OUT);

	ASSERT (is_in_range (C, C, OUT.size()));

	return OUT;
}

vector <YAMAJI>	generate_stresstensors_for_yamaji () {

	cout << AXES.size() * ANGLES.size() * PHI.size() << endl;

	vector <YAMAJI> OUT;

	for (size_t i = 0; i < AXES.size(); i++) {
		for (size_t j = 0; j < ANGLES.size(); j++) {
			for (size_t k = 0; k < PHI.size(); k++) {

				YAMAJI buf;

				buf.ST = calculate_stresstensor (AXES.at(i), ANGLES.at(j), PHI.at(k));
				buf.AXIS_ID = i;
				buf.ANGLE_ID = j;
				buf.PHI_ID = k;
				buf.COUNT = 0;

				OUT.push_back (buf);
			}
		}
	}
	return OUT;
}

bool compatible_subset (const vector <VCTR>& test_normal) {

	const size_t S = test_normal.size();

	for (size_t i = 0; i < S; i++) {
		for (size_t j = i+1; j < S; j++) {

			const VCTR N1 =  test_normal.at(i);
			const VCTR N2 =  test_normal.at(j);

			const double ANG = N1.X * N2.X + N1.Y * N2.Y +N1.Z * N2.Z;

			if (ANG < 0.939) return false;
		}
	}
	return true;
}

vector <vector <size_t> > narrow_subsets (const vector <vector <size_t> >& S, const vector <GDB>& inGDB) {

	vector <vector <size_t> > OUT;

	for (size_t i = 0; i < S.size(); i++) {

		vector <VCTR> test_normal;

		for (size_t j = 0; j < S.at(i).size(); j++) {

			const size_t THIS = S.at(i).at(j);

			test_normal.push_back (inGDB.at(THIS - 1).N);
		}
		if (compatible_subset (test_normal)) OUT.push_back (S.at(i));
	}
	return OUT;
}

void MISFIT () {

	AXES = return_rakhmanov_points (128);//256
	ANGLES = generate_angle_vector_180 (0.0, 180.0, 8);//16
	PHI = generate_phi_vector (0.0, 0.1, 1);//15

	vector <VCTR> AXS_V = AXES;
	vector <double> ANG_V = ANGLES;
	vector <double> PHI_V = PHI;
	vector <VCTR> N1_V = AXES;
	vector <VCTR> N2_V = AXES;

	double COUNTER = 0.0;
	double WRONG = 0.0;

	cout << fixed << setprecision (6) << endl;

	for (size_t ax = 0; ax < AXS_V.size(); ax++) {
		for (size_t an = 0; an < ANG_V.size(); an++) {
			for (size_t ph = 0; ph < PHI_V.size(); ph++) {

				STRESSTENSOR S = calculate_stresstensor (AXES.at(ax), ANGLES.at(an), PHI.at(ph));

				for (size_t n1 = 0; n1 < N1_V.size(); n1++) {
					for (size_t n2 = 0; n2 < N2_V.size(); n2++) {

						COUNTER++;

						const VCTR N1 = N1_V.at (n1);
						const VCTR N2 = N2_V.at (n2);

						const VCTR T1 = unitvector (return_shearstress (S, N1), false);
						const VCTR T2 = unitvector (return_shearstress (S, N2), false);

						const double T_ANG = T1.X*T2.X + T1.Y*T2.Y + T1.Z*T2.Z;
						const double N_ANG = N1.X*N2.X + N1.Y*N2.Y + N1.Z*N2.Z;

						if (T_ANG > 0.939 && N_ANG < 0.965) {

							cout_dbg_stresstensor (S);

							cout << T1.X << " - " << T1.Y << " - " << T1.Z << endl;
							cout << T2.X << " - " << T2.Y << " - " << T2.Z << endl;
							cout << ACOS (T_ANG) << endl;
							cout << N1.X << " - " << N1.Y << " - " << N1.Z << endl;
							cout << N2.X << " - " << N2.Y << " - " << N2.Z << endl;
							cout << ACOS (N_ANG) << endl;

							//cout << T_ANG << '\t' << N_ANG << endl;
							WRONG++;
						}
					}
				}
			}
		}
		cout << "x" << flush;
	}

	cout << COUNTER << endl;
	cout << WRONG << endl;
}

vector <VCTR>  generate_N_to_test (const vector <GDB>& inGDB, const vector <size_t>& s) {

	vector <VCTR> OUT;

	for (size_t i = 0; i < s.size(); i++) {

		OUT.push_back(inGDB.at(i).N);
	}
	return OUT;
}

bool test_subset_stressfield (const vector <VCTR>& N_to_process, const YAMAJI& y) {

	const double ANG = 0.939;

	const STRESSTENSOR ST = y.ST;

	for (size_t i = 0; i < N_to_process.size(); i++) {

		const VCTR N = N_to_process.at(i);

		const VCTR SH = unitvector (return_shearstress(ST, N), true);

		const double TEST_ANG = SH.X*N.X + SH.Y*N.Y + SH.Z*N.Z;

		if (TEST_ANG < ANG) return false;
	}
	return true;
}


STRESSTENSOR st_YAMAJI (const vector <GDB>& inGDB) {

	AXES = return_rakhmanov_points(256);
	ANGLES = generate_angle_vector_180 (0.0, 180.0, 16);
	PHI = generate_phi_vector (0.0, 1.0, 15);

	//MISFIT ();
	//exit (22);

	const vector <YAMAJI> Y = generate_stresstensors_for_yamaji ();

	const vector <vector <size_t> > S = generate_subsets_vector (inGDB);

	const vector <vector <size_t> > V = S;//narrow_subsets (S, inGDB);

	double counter = 0.0;

	double MAX = Y.size();

	MAX = MAX * V.size();

	vector <GDB> dummy;

	cout << "ORIGINAL SUBSET SIZE: " << S.size() << endl;
	cout << "NARROW SUBSET SIZE  : " << V.size() << endl;


	for (size_t i = 0; i < Y.size(); i++) {
		for (size_t j = 0; j < S.size(); j++) {

			const vector <VCTR> N_to_process = generate_N_to_test (inGDB, S.at(j));

			//bool is_set_compatible = test_subset_stressfield (N_to_process, Y.at(i));

			counter = counter + 1.0;

		}
		progress_bar (20, counter, MAX);
	}

	STRESSTENSOR OUT;

	//vector <STRESSTENSOR> TNSR = generate_stresstensors_for_yamaji ();



	//for (size_t str = 0; str < TNSR.size(); str++) {
		//for (size_t rec_cnt = 0; rec_cnt < N_GDB.size(); rec_cnt++) {

			//double MISFIT = SUBSET_MISFIT (N_GDB.at(rec_cnt), S_GDB.at(rec_cnt), TNSR.at(str));
		//}
		//cout << str << endl;
	//}
	return OUT;
}
