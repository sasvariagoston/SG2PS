//#include <algorithm>

#include <algorithm>
//#include <cmath>
//#include <ctime>
//#include <iomanip>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
//#include <sstream>
//#include <stdexcept>
//#include <vector>

//#include "array_to_vector.hpp"
#include "assertions.hpp"
#include "brute_force.hpp"
#include "common.h"
#include "inversion.h"
#include "kaalsbeek.hpp"
#include "rakhmanov.hpp"
//#include "rgf.h"
#include "structs.h"
#include "brute_force.hpp"

using namespace std;

vector <double> generate_angle_vector_180 (const double& ANG_MIN, const double& ANG_MAX, const size_t& INT_NUM) {

	ASSERT2(ANG_MIN >= 0.0, "Minimum angle smaller than 0.0 deg");
	ASSERT2(ANG_MAX <= 180.0, "Maximum angle larger than 180.0 deg");

	vector <double> ANG_CONTAINER;

	double step = (ANG_MAX - ANG_MIN) / INT_NUM;

	double ANG = ANG_MIN;

	for (size_t i = 0; i < INT_NUM; i++) {

		ANG_CONTAINER.push_back (ANG);

		//cout << ANG << endl;

		ANG = ANG + step;
	}
	return ANG_CONTAINER; // NUMERICALLY OK
}

vector <double> generate_phi_vector (const double& PHI_MIN, const double& PHI_MAX, const size_t& INT_NUM) {

	ASSERT2(PHI_MIN >= 0.0, "Minimum PHI value smaller than 0.0");
	ASSERT2(PHI_MAX <= 1.0, "Maximum PHI value larger than 1.0");

	vector <double> PHI_CONTAINER;

	double step = (PHI_MAX - PHI_MIN) / INT_NUM;

	double PHI = PHI_MIN;

	for (size_t i = 0; i < INT_NUM + 1; i++) {

		PHI_CONTAINER.push_back (PHI);

		//cout << PHI << endl;

		PHI = PHI + step;
	}
	return PHI_CONTAINER; // NUMERICALLY OK
}

vector <vector <double> > DIR_MX1_from_n1 (const VCTR& n1, const double& angle) {

	VCTR n2 = DXDYDZ_from_NXNYNZ(n1);

	n2 = unitvector (n2);

	n2 = ROTATE(n1, n2, angle);

	n2 = unitvector(n2);

	VCTR n3 = crossproduct(n1, n2);

	n3 = unitvector(n3);

	return (declare_3x3_matrix (
			n1.X, n1.Y, n1.Z,
			n2.X, n2.Y, n2.Z,
			n3.X, n3.Y, n3.Z));
} //CHECKED, NUMERICALLY OK

vector <vector <double> > st_from_reduced_stresstensor (const vector <vector <double> >& DIR_MX1, const double& fi) {

	vector <vector <double> >  T = declare_3x3_matrix(
			0.0, 0.0, 0.0,
			0.0, fi , 0.0,
			0.0, 0.0, 1.0);

	vector <vector <double> >  DIR_MX2 = transpose(DIR_MX1);

	vector <vector <double> >  OUT = mult_mtrx (DIR_MX2, T);



	return mult_mtrx(OUT, DIR_MX1); //NUMERICALLY CHECKED, OK
}

vector <VCTR> generate_centroids_net (const VCTR& ORIGO, const size_t POINTS_DISTANCE, const INPSET& inset) {

	vector <vector <vector <VCTR> > > NET  = generate_net (POINTS_DISTANCE);

	VCTR AXIS = unitvector(ORIGO);

	VCTR ABOVE = declare_vector(0.0, 0.0, 1.0);

	VCTR D = DXDYDZ_from_NXNYNZ(AXIS);

	VCTR STRIKE = ROTATE(AXIS, D, -90.0);

	STRIKE = unitvector(STRIKE);

	double ANGLE = ACOS(dotproduct(AXIS, ABOVE));

	//cout << fixed << setprecision(3) << endl;

	//cout
	//<< AXIS.X << '\t'
	//<< AXIS.Y << '\t'
	//<< AXIS.Z << endl;

	//cout
	//<< STRIKE.X << '\t'
	//<< STRIKE.Y << '\t'
	//<< STRIKE.Z << endl;

	//cout << ANGLE << endl;

	//cout << fixed << setprecision(6) << endl;

	vector <VCTR> OUT;

	for (size_t i = 0; i < NET.size(); i++) {
		for (size_t j = 0; j < NET.at(i).size(); j++) {
			for (size_t k = 0; k < NET.at(i).at(j).size(); k++) {

				VCTR T = NET.at(i).at(j).at(k);

				T = ROTATE(STRIKE, T, ANGLE);

				if (T.Z > 0.0) T = invert_VCTR(T);



				//T.X = T.X * RADIUS;
				//T.Y = T.Y * RADIUS;
				//T.Z =  - sqrt(1.0 - (T.X*T.X) - (T.Y*T.Y));

				T = unitvector(T);

				OUT.push_back(T);
			}
		}
	}

	OUT.push_back(AXIS);

	for (size_t i = 0; i < OUT.size(); i++) {

	//	cout
	//	<< OUT.at(i).X << '\t'
	//	<< OUT.at(i).Y << '\t'
	//	<< OUT.at(i).Z << '\t'
	//	<< '\n';

	}

	//cout << "END" << endl;

	//exit (1);

	return OUT; //OK
}

STRESSTENSOR convert_matrix_to_stresstensor (const vector <vector <double> >& IN) {

	ASSERT2(IN.size() == 3, "3x3 matrix expected for stress tensor conversion");
	ASSERT2(IN.at(0).size() == 3, "3x3 matrix expected for stress tensor conversion");

	STRESSTENSOR OUT;

	OUT._11 = IN.at(0).at(0);
	OUT._12 = IN.at(0).at(1);
	OUT._13 = IN.at(0).at(2);
	OUT._22 = IN.at(1).at(1);
	OUT._23 = IN.at(1).at(2);
	OUT._33 = IN.at(2).at(2);

	return OUT;
}

double calculate_cumulative_misfit (const vector <GDB>& inGDB) {

	double MISFIT = 0.0;

	for (size_t i = 0; i < inGDB.size (); i++) {

		MISFIT = MISFIT + inGDB.at(i).ANG;
	}
	return MISFIT;
}


bool by_MISFIT_ANGLE (const BRUTEFORCE_RESULT& x, const BRUTEFORCE_RESULT& y) {

	return x.MISFIT < y.MISFIT;
}

vector <BRUTEFORCE_RESULT> return_minimum_misfits (vector <BRUTEFORCE_RESULT> IN, size_t records_number) {

	vector <BRUTEFORCE_RESULT> TEMP = IN;

	vector <BRUTEFORCE_RESULT> OUT;

	sort(TEMP.begin(), TEMP.end(), by_MISFIT_ANGLE);

	for (size_t i = 0; i < records_number; i++) {

		OUT.push_back(TEMP.at(i));
	}
	return OUT;
}

STRESSTENSOR return_stresstensor_from_n1_ang_phi (const VCTR& N1, const double& ANG, const double& PHI) {

	vector <vector <double> > M1 = DIR_MX1_from_n1 (N1, ANG);

	vector <vector <double> > T = st_from_reduced_stresstensor (M1, PHI);

	return convert_matrix_to_stresstensor (T);
}

vector <BRUTEFORCE_RESULT> BRUTEFORCE_ENGINE (const vector <GDB>& inGDB, const vector <VCTR>& CNTRVCTR, const vector <double>& ANGVCTR, const vector <double>& PHIVCTR, const INPSET& inset) {

	size_t CNT_MAX = CNTRVCTR.size();
	size_t ANG_MAX = ANGVCTR.size();
	size_t PHI_MAX = PHIVCTR.size();

	bool BRUTEFORCE 	= is_method_BRUTEFORCE(inGDB, inset);
	bool YAMAJI 		= is_method_YAMAJI(inGDB, inset);

	if (!BRUTEFORCE && !YAMAJI) ASSERT_DEAD_END();

	vector <BRUTEFORCE_RESULT> OUT;

	for (size_t cntr = 0; cntr < CNT_MAX; cntr++) {

		double MIN_MISFIT = 10000000.0;
		BRUTEFORCE_RESULT buf;

		for (size_t ang = 0; ang < ANG_MAX; ang++) {
			for (size_t phi = 0; phi < PHI_MAX; phi++) {

				STRESSTENSOR st =  return_stresstensor_from_n1_ang_phi (CNTRVCTR.at(cntr), ANGVCTR.at(ang), PHIVCTR.at(phi));

				//vector <BRUTEFORCE_RESULT> YAMAJI_RESULT = YAMAJI_ENGINE (const vector <GDB>& inGDB, const vector <VCTR>& CNTRVCTR, const vector <double>& ANGVCTR, const vector <double>& PHIVCTR, const INPSET& inset)

				vector <GDB> tempGDB = return_stressvector_estimators (st, inGDB, "BRUTEFORCE", false);

				double MISFIT = calculate_cumulative_misfit (tempGDB);

				if (MISFIT < MIN_MISFIT) {

					buf.ANG = ANGVCTR.at(ang);
					buf.NRM = CNTRVCTR.at(cntr);
					buf.PHI = PHIVCTR.at(phi);
					buf.MISFIT = MISFIT;

					MIN_MISFIT = MISFIT;
				}
			}
		}
		OUT.push_back(buf);
	}

	return OUT;
}

vector <BRUTEFORCE_RESULT> YAMAJI_ENGINE (const vector <GDB>& inGDB, const VCTR& CNTR, const double& ANG, const double& PHI, const INPSET& inset) {

	vector <BRUTEFORCE_RESULT> OUT;

	for (size_t i = 0; i < inGDB.size() - 3; i++) {
		for (size_t j = i + 1; j < inGDB.size() - 2; j++) {
			for (size_t k = i + 2; k < inGDB.size() - 1; k++) {
				for (size_t l = i + 3; l < inGDB.size(); l++) {

					vector <GDB> tempGDB;

					tempGDB.push_back(inGDB.at(i));
					tempGDB.push_back(inGDB.at(j));
					tempGDB.push_back(inGDB.at(k));
					tempGDB.push_back(inGDB.at(l));

					STRESSTENSOR st = return_stresstensor_from_n1_ang_phi (CNTR, ANG, PHI);

					tempGDB = return_stressvector_estimators(st, tempGDB, "BRUTEFORCE", false);

					double MISFIT = calculate_cumulative_misfit (tempGDB);

					BRUTEFORCE_RESULT buf;

					buf.ANG = ANG;
					buf.NRM = CNTR;
					buf.PHI = PHI;
					buf.MISFIT = MISFIT;

					OUT.push_back(buf);
				}
			}
		}
	}
	return OUT;
}


STRESSTENSOR st_BRUTEFORCE (const vector <GDB>& inGDB, const INPSET& inset) {

	bool BRUTEFORCE 	= is_method_BRUTEFORCE(inGDB, inset);
	bool YAMAJI 		= is_method_YAMAJI(inGDB, inset);

	if (!BRUTEFORCE && !YAMAJI) ASSERT_DEAD_END();

	vector <VCTR> CNTRVCTR;
	vector <double> ANGVCTR;
	vector <double> PHIVCTR;

	if (BRUTEFORCE) {

		CNTRVCTR = generate_centroids_net (declare_vector(0.0, 0.0, 1.0), 9, inset);

		ANGVCTR = generate_angle_vector_180 (0.0, 180.0, 18);

		PHIVCTR = generate_phi_vector (0.0, 1.0, 10);
	}
	else {

		CNTRVCTR = return_rakhmanov_points(256);

		ANGVCTR = generate_angle_vector_180 (0.0, 180.0, 16);

		PHIVCTR = generate_phi_vector (0.0, 1.0, 15);
	}

	vector <BRUTEFORCE_RESULT> BR_RAW = BRUTEFORCE_ENGINE (inGDB, CNTRVCTR, ANGVCTR, PHIVCTR, inset);

	if (BRUTEFORCE) BR_RAW = return_minimum_misfits (BR_RAW, 10);
	else {};

	vector <BRUTEFORCE_RESULT> BR_FINAL;

	for (size_t i = 0; i < BR_RAW.size(); i++) {

		VCTR ORIGO = BR_RAW.at(i).NRM;
		size_t POINTS_DISTANCE = 1;
		vector <VCTR> CNTRVCTR = generate_centroids_net(ORIGO, POINTS_DISTANCE, inset);

		double ANG_MIN = BR_RAW.at(i).ANG - 7.0;
		double ANG_MAX = BR_RAW.at(i).ANG + 7.0;
		if (ANG_MIN <   0.0) ANG_MIN = 0.0;
		if (ANG_MAX > 180.0) ANG_MAX = 180.0;
		vector <double> ANGVCTR = generate_angle_vector_180 (ANG_MIN, ANG_MAX, 14);

		double PHI_MIN = BR_RAW.at(i).PHI - 0.07;
		double PHI_MAX = BR_RAW.at(i).PHI + 0.07;
		if (PHI_MIN < 0.0) PHI_MIN = 0.0;
		if (PHI_MAX > 1.0) PHI_MAX = 1.0;
		vector <double> PHIVCTR = generate_phi_vector (PHI_MIN, PHI_MAX, 10);

		vector <BRUTEFORCE_RESULT> BR_FINE = BRUTEFORCE_ENGINE (inGDB, CNTRVCTR, ANGVCTR, PHIVCTR, inset);

		BR_FINE = return_minimum_misfits (BR_FINE, 1);

		BRUTEFORCE_RESULT buf = BR_FINE.at(0);

		BR_FINAL.push_back(buf);
	}

	BR_FINAL = return_minimum_misfits (BR_FINAL, 1);

	VCTR   MIN_N1  = BR_FINAL.at(0).NRM;
	double MIN_ANG = BR_FINAL.at(0).ANG;
	double MIN_PHI = BR_FINAL.at(0).PHI;

	return (return_stresstensor_from_n1_ang_phi (MIN_N1, MIN_ANG, MIN_PHI));

	//0.195515	0.112962	-0.183388
	//0.112962	0.065266	-0.106299
	//-0.183388	-0.106299	 0.939219
}

void dbg_cout_matrix (vector <vector <double> > I) {

	if (I.size() < 1) return;
	if (I.at(0).size() < 1) return;

	cout << fixed << setprecision(6) << endl;

	cout << " ---- MATRIX ---- " << endl;

	for (size_t i = 0; i < I.size(); i++) {
		for (size_t j = 0; j < I.size(); j++) {

			cout << I.at(j).at(i) << '\t' << flush;

		}
		cout << endl;
	}
	cout << endl;

	cout << " ---- END MATRIX ---- " << endl;
}

