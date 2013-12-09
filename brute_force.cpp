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

	int counter = 0;

	//cout << "BF_START" << endl;

	size_t CNT_MAX = CNTRVCTR.size();
	size_t ANG_MAX = ANGVCTR.size();
	size_t PHI_MAX = PHIVCTR.size();

	vector <BRUTEFORCE_RESULT> OUT;

	for (size_t cntr = 0; cntr < CNT_MAX; cntr++) {

		double MIN_MISFIT = 10000000.0;
		BRUTEFORCE_RESULT buf;

		for (size_t ang = 0; ang < ANG_MAX; ang++) {
			for (size_t phi = 0; phi < PHI_MAX; phi++) {

				STRESSTENSOR st =  return_stresstensor_from_n1_ang_phi (CNTRVCTR.at(cntr), ANGVCTR.at(ang), PHIVCTR.at(phi));

				vector <GDB> tempGDB = return_stressvector_estimators (st, inGDB, "BRUTEFORCE", false);

				double MISFIT = calculate_cumulative_misfit (tempGDB);

				if (MISFIT < MIN_MISFIT) {

					//cout << MIN_MISFIT << endl;

					buf.ANG = ANGVCTR.at(ang);
					buf.NRM = CNTRVCTR.at(cntr);
					buf.PHI = PHIVCTR.at(phi);
					buf.MISFIT = MISFIT;

					MIN_MISFIT = MISFIT;
				}
				counter++;
			}
		}
		OUT.push_back(buf);
	}

//	string filename = "BRUTEFORCE.TXT";

//	ofstream o(filename.c_str());

//	o
	//<< "NRM.X" << '\t'
	//<< "NRM.Y" << '\t'
	//<< "NRM.Z" << '\t'
	//<< "ANG" << '\t'
	//<< "PHI" << '\t'
	//<< "MISFIT" << endl;

	//for (size_t i = 0; i < OUT.size(); i++) {

	//	cout
	//	<< OUT.at(i).NRM.X << '\t'
	//	<< OUT.at(i).NRM.Y << '\t'
	//	<< OUT.at(i).NRM.Z << '\t'
	//	<< OUT.at(i).ANG << '\t'
	//	<< OUT.at(i).PHI << '\t'
	//	<< OUT.at(i).MISFIT << '\n';
	//}

	//cout << "BF_END" << endl;
	return OUT;
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

