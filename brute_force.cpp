// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

/*
S1: 278/85 (-0.083810,  0.011239, -0.996418)
S2: 019/01 ( 0.333195,  0.942698, -0.017393)
S3: 110/05 ( 0.939126, -0.333459, -0.082752)
EIGENVALUES: 1.000000,  0.130000,  0.000000

S1: 015/12 ( 0.246408,  0.948213, -0.200440)
S2: 126/60 ( 0.403765, -0.288451, -0.868200)
S3: 279/27 (-0.881055,  0.133001, -0.453931)
EIGENVALUES: 1.000000,  0.110000, -0.000000
*/

//#include <algorithm>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>

#include "assertions.hpp"
#include "brute_force.hpp"
#include "common.h"
#include "inversion.h"
#include "kaalsbeek.hpp"
#include "rakhmanov.hpp"
#include "settings.hpp"
#include "structs.h"
#include "stresstensor.hpp"

using namespace std;

vector <double> generate_angle_vector_180 (const double& ANG_MIN, const double& ANG_MAX, const size_t& INT_NUM) {

	ASSERT2(ANG_MIN >= 0.0, "Minimum angle smaller than 0.0 deg");
	ASSERT2(ANG_MAX <= 180.0, "Maximum angle larger than 180.0 deg");

	vector <double> ANG_CONTAINER;

	double step = (ANG_MAX - ANG_MIN) / INT_NUM;

	double ANG = ANG_MIN;

	for (size_t i = 0; i < INT_NUM; i++) {

		ANG_CONTAINER.push_back (ANG);

		ANG = ANG + step;
	}
	return ANG_CONTAINER;
}

vector <double> generate_phi_vector (const double& PHI_MIN, const double& PHI_MAX, const size_t& INT_NUM) {

	ASSERT2(PHI_MIN >= 0.0, "Minimum PHI value smaller than 0.0");
	ASSERT2(PHI_MAX <= 1.0, "Maximum PHI value larger than 1.0");

	vector <double> PHI_CONTAINER;

	double step = (PHI_MAX - PHI_MIN) / INT_NUM;

	double PHI = PHI_MIN;

	for (size_t i = 0; i < INT_NUM + 1; i++) {

		PHI_CONTAINER.push_back (PHI);

		PHI = PHI + step;
	}
	return PHI_CONTAINER;
}

vector <vector <double> > DIR_MX1_from_n1 (const VCTR& n1, const double& angle) {

	VCTR n2 = DXDYDZ_from_NXNYNZ(n1);

	n2 = ROTATE(n1, n2, angle);

	n2 = unitvector(n2);

	VCTR n3 = crossproduct(n1, n2);

	n3 = unitvector(n3);

	return (declare_3x3_matrix (
			n1.X, n1.Y, n1.Z,
			n2.X, n2.Y, n2.Z,
			n3.X, n3.Y, n3.Z));
}

vector <vector <double> > st_from_reduced_stresstensor (const vector <vector <double> >& DIR_MX1, const double& fi) {

	vector <vector <double> >  T = declare_3x3_matrix (
			0.0, 0.0, 0.0,
			0.0, fi , 0.0,
			0.0, 0.0, 1.0);

	vector <vector <double> >  DIR_MX2 = transpose (DIR_MX1);

	vector <vector <double> >  OUT = mult_mtrx (DIR_MX2, T);

	return mult_mtrx (OUT, DIR_MX1);
}

vector <VCTR> generate_centroids_net (const VCTR& ORIGO, const size_t POINTS_DISTANCE) {

	vector <vector <vector <VCTR> > > NET  = generate_net (POINTS_DISTANCE);

	VCTR AXIS = unitvector(ORIGO);

	VCTR ABOVE = declare_vector(0.0, 0.0, 1.0);

	VCTR D = DXDYDZ_from_NXNYNZ(AXIS);

	VCTR STRIKE = ROTATE(AXIS, D, -90.0);

	STRIKE = unitvector(STRIKE);

	double ANGLE = ACOS(dotproduct(AXIS, ABOVE));

	vector <VCTR> OUT;

	for (size_t i = 0; i < NET.size(); i++) {
		for (size_t j = 0; j < NET.at(i).size(); j++) {
			for (size_t k = 0; k < NET.at(i).at(j).size(); k++) {

				VCTR T = NET.at(i).at(j).at(k);

				T = ROTATE(STRIKE, T, ANGLE);

				if (T.Z > 0.0) T = flip_vector (T);

				T = unitvector(T);

				OUT.push_back(T);
			}
		}
	}
	OUT.push_back(AXIS);

	return OUT;
}

bool by_MISFIT_ANGLE (const BRUTEFORCE_RESULT& x, const BRUTEFORCE_RESULT& y) {

	return x.MISFIT < y.MISFIT;
}

vector <BRUTEFORCE_RESULT> return_minimum_misfits (vector <BRUTEFORCE_RESULT> IN, size_t records_number) {

	vector <BRUTEFORCE_RESULT> TEMP = IN;

	vector <BRUTEFORCE_RESULT> OUT;

	stable_sort (TEMP.begin(), TEMP.end(), by_MISFIT_ANGLE);

	for (size_t i = 0; i < records_number; i++) {

		OUT.push_back(TEMP.at(i));
	}
	return OUT;
}

static double dotprod(const VCTR& a, const VCTR& b) {

    return a.X*b.X + a.Y*b.Y + a.Z*b.Z;
}

static VCTR make_unit(const VCTR& in) {

    VCTR OUT;

    double vectorlength = sqrt(in.X*in.X + in.Y*in.Y + in.Z*in.Z + 1.0e-6);

    OUT.X = (in.X / vectorlength);
    OUT.Y = (in.Y / vectorlength);
    OUT.Z = (in.Z / vectorlength);

    return OUT;
}

static double deg_to_rad(double alpha) {

    return 0.01745329252*alpha;
}

// TODO Another representation of rotation may need less floating point operations
static VCTR rotate(const VCTR& ax, const VCTR& torotate, double alpha) {

    VCTR result;
    VCTR A_1, A_2, A_3;

//    const double cos_a = COS(alpha);
//    const double sin_a = SIN(alpha);
//---------------------------------------
//    float s(0), c(0);
//
//    sincosf(deg_to_rad(alpha), &s, &c);
//
//    const double cos_a = c;
//    const double sin_a = s;
//----------------------------------------
    const double alpha_deg = deg_to_rad(alpha);
    const double cos_a = cos(alpha_deg);
    const double sin_a = sin(alpha_deg);

    A_1.X =      cos_a         + (1.0 -  cos_a) * ax.X * ax.X;
    A_1.Y = (1.0-cos_a) * ax.Y * ax.X - (sin_a)        * ax.Z;
    A_1.Z = (1.0-cos_a) * ax.Z * ax.X + (sin_a)        * ax.Y;

    A_2.X = (1.0-cos_a) * ax.Y * ax.X + (sin_a)        * ax.Z;
    A_2.Y =      cos_a         + (1.0 -  cos_a) * ax.Y * ax.Y;
    A_2.Z = (1.0-cos_a) * ax.Y * ax.Z - (sin_a)        * ax.X;

    A_3.X = (1.0-cos_a) * ax.Z * ax.X - (sin_a)        * ax.Y;
    A_3.Y = (1.0-cos_a) * ax.Y * ax.Z + (sin_a)        * ax.X;
    A_3.Z =      cos_a         + (1.0 -  cos_a) * ax.Z * ax.Z;

    result.X = dotprod(torotate, A_1);
    result.Y = dotprod(torotate, A_2);
    result.Z = dotprod(torotate, A_3);

    return make_unit(result);
}

static void multiply(const double x[3][3], const double y[3][3], double z[3][3]) {

    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            z[i][j] = 0.0;
            for (int k=0; k<3; ++k) {
                z[i][j] += x[i][k]*y[k][j];
            }
        }
    }
}

static STRESSTENSOR calculate_stresstensor(const VCTR& n1, double ANG, double PHI) {

    // vector <vector <double> > M1 = DIR_MX1_from_n1 (N1, ANG);
    // DIR_MX1_from_n1 (const VCTR& n1, const double& angle)
    //-----------------------------------------------------------------

    VCTR n2 = DXDYDZ_from_NXNYNZ(n1); // FIXME dipdir_dip_from_NXNYNZ should most likely call atan2

    //n2 = unitvector (n2);

    n2 = rotate(n1, n2, ANG); // TODO Discuss this, perhaps there is a more efficient way

    n2 = make_unit(n2);

    VCTR n3 = crossproduct(n1, n2);

    n3 = make_unit(n3);

    const double DIR_MX1[3][3] = { { n1.X, n1.Y, n1.Z },
                                   { n2.X, n2.Y, n2.Z },
                                   { n3.X, n3.Y, n3.Z } };
    //-----------------------------------------------------------------
    //vector <vector <double> > T = st_from_reduced_stresstensor (M1, PHI);
    //st_from_reduced_stresstensor(const vector<vector<double>>& DIR_MX1, const double& fi)

    const double T[3][3] = { { 0.0, 0.0, 0.0 },
                             { 0.0, PHI, 0.0 },
                             { 0.0, 0.0, 1.0 } };

    //vector <vector <double> >  DIR_MX2 = transpose(DIR_MX1);

    const double DIR_MX2[3][3]  = { { n1.X, n2.X, n3.X },
                                    { n1.Y, n2.Y, n3.Y },
                                    { n1.Z, n2.Z, n3.Z } };

    //vector <vector <double> >  OUT = mult_mtrx (DIR_MX2, T);

    double OUT[3][3];

    multiply(DIR_MX2, T, OUT); // FIXME Most likely wasteful as T has only 2 nonzero elements

    double res[3][3];

    multiply(OUT, DIR_MX1, res); // res was T in the original code

    STRESSTENSOR st;

    st._11 = res[0][0];
    st._12 = res[0][1];
    st._13 = res[0][2];
    st._22 = res[1][1];
    st._23 = res[1][2];
    st._33 = res[2][2];

    return st;
}

/*
static double misfit (const STRESSTENSOR& st, const VCTR& N, const VCTR& SV, bool compression_positive) {

    VCTR shearstress = return_shearstress (st, N); // TODO Seems to compute the same twice

    //double prod = dotprod(SV, shearstress);

    //return 1.0 - prod*prod/(length_sqr(SV)*length_sqr(shearstress));

    return ACOS (dotproduct (SV, shearstress, true));
}
*/

vector <BRUTEFORCE_RESULT> BRUTEFORCE_ENGINE (const vector <GDB>& inGDB, const vector <VCTR>& CNTRVCTR, const vector <double>& ANGVCTR, const vector <double>& PHIVCTR) {

	if (!is_INVERSION_BRUTEFORCE() && ! is_INVERSION_YAMAJI()) ASSERT_DEAD_END();

	size_t CNT_MAX = CNTRVCTR.size();
	size_t ANG_MAX = ANGVCTR.size();
	size_t PHI_MAX = PHIVCTR.size();

	size_t DATANUMBER = inGDB.size();

	vector <VCTR> N;
	vector <VCTR> SV;

	for (size_t q = 0; q < DATANUMBER; q++) {

		N.push_back(inGDB.at(q).N);

		SV.push_back(inGDB.at(q).DC);
	}

	vector <BRUTEFORCE_RESULT> OUT;

	OUT.reserve(CNT_MAX);

	for (size_t cntr = 0; cntr < CNT_MAX; cntr++) {

		double MIN_MISFIT = 10000000.0;
		BRUTEFORCE_RESULT buf;

		for (size_t ang = 0; ang < ANG_MAX; ang++) {
			for (size_t phi = 0; phi < PHI_MAX; phi++) {

			    STRESSTENSOR st = calculate_stresstensor(CNTRVCTR.at(cntr), ANGVCTR.at(ang), PHIVCTR.at(phi));

                double MISFIT = 0.0;

				for (size_t z = 0; z < DATANUMBER; ++z) {

				   MISFIT = MISFIT + return_ANG (st, N.at(z), SV.at(z));

					//MISFIT += misfit(st, N.at(z), SV.at(z), false);
				}

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

STRESSTENSOR st_BRUTEFORCE (const vector <GDB>& inGDB) {

	vector <VCTR> CNTRVCTR = generate_centroids_net (declare_vector(0.0, 0.0, 1.0), 9);
	vector <double> ANGVCTR = generate_angle_vector_180 (0.0, 180.0, 18);
	vector <double> PHIVCTR = generate_phi_vector (0.0, 1.0, 10);

	vector <BRUTEFORCE_RESULT> BR_RAW = BRUTEFORCE_ENGINE (inGDB, CNTRVCTR, ANGVCTR, PHIVCTR);

	BR_RAW = return_minimum_misfits (BR_RAW, 10);

	vector <BRUTEFORCE_RESULT> BR_FINAL;

	for (size_t i = 0; i < BR_RAW.size(); i++) {

		VCTR ORIGO = BR_RAW.at(i).NRM;
		size_t POINTS_DISTANCE = 1;
		vector <VCTR> CNTRVCTR_2 = generate_centroids_net(ORIGO, POINTS_DISTANCE);

		double ANG_MIN = BR_RAW.at(i).ANG - 5.0;
		double ANG_MAX = BR_RAW.at(i).ANG + 5.0;
		if (ANG_MIN <   0.0) ANG_MIN = 0.0;
		if (ANG_MAX > 180.0) ANG_MAX = 180.0;
		vector <double> ANGVCTR_2 = generate_angle_vector_180 (ANG_MIN, ANG_MAX, 5);

		double PHI_MIN = BR_RAW.at(i).PHI - 0.05;
		double PHI_MAX = BR_RAW.at(i).PHI + 0.05;
		if (PHI_MIN < 0.0) PHI_MIN = 0.0;
		if (PHI_MAX > 1.0) PHI_MAX = 1.0;
		vector <double> PHIVCTR_2 = generate_phi_vector (PHI_MIN, PHI_MAX, 5);

		vector <BRUTEFORCE_RESULT> BR_FINE = BRUTEFORCE_ENGINE (inGDB, CNTRVCTR_2, ANGVCTR_2, PHIVCTR_2);

		BR_FINE = return_minimum_misfits (BR_FINE, 1);

		BRUTEFORCE_RESULT buf = BR_FINE.at(0);

		BR_FINAL.push_back(buf);
	}

	BR_FINAL = return_minimum_misfits (BR_FINAL, 1);

	VCTR   MIN_N1  = BR_FINAL.at(0).NRM;
	double MIN_ANG = BR_FINAL.at(0).ANG;
	double MIN_PHI = BR_FINAL.at(0).PHI;

	return (return_stresstensor_from_n1_ang_phi (MIN_N1, MIN_ANG, MIN_PHI));
}

STRESSFIELD sf_BRUTEFORCE (STRESSTENSOR& st) {

	STRESSFIELD	sf = eigenvalue_eigenvector (st);

	sf = correct_SF_to_fit_D (sf);

	sf = computestressfield_DXDYDZ (sf);

	return stress_regime (sf);
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
