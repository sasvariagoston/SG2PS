//#include <algorithm>

//#include <cmath>
//#include <ctime>
//#include <iomanip>
#include <iostream>
#include <iomanip>
#include <fstream>
//#include <sstream>
//#include <stdexcept>
//#include <vector>

//#include "array_to_vector.hpp"
//#include "assertions.hpp"
#include "brute_force.hpp"
#include "common.h"
#include "inversion.h"
#include "kaalsbeek.hpp"
//#include "rgf.h"
#include "structs.h"
#include "brute_force.hpp"

using namespace std;

vector <vector <double> > DIR_MX1_from_n1 (VCTR n1, double angle) {

	VCTR n2 = NXNYNZ_from_DXDYDZ(n1);

	n2 = ROTATE(n1, n2, angle);

	n2 = unitvector(n2);

	VCTR n3 = crossproduct(n1, n2);

	n3 = unitvector(n3);

	cout << fixed << setprecision (5) << endl;

	cout << " ------------------ " << endl;
	cout << angle << endl;
	cout << n1.X << '\t' << n1.Y << '\t' << n1.Z << endl;
	cout << n2.X << '\t' << n2.Y << '\t' << n2.Z << endl;
	cout << n3.X << '\t' << n3.Y << '\t' << n3.Z << endl;

	return (declare_3x3_matrix (
			n1.X, n1.X, n1.Z,
			n2.X, n2.Y, n2.Z,
			n3.X, n3.Y, n3.Z));
}

vector <vector <double> > st_from_reduced_stresstensor (vector <vector <double> > DIR_MX1, double fi) {

	//OK
	vector <vector <double> >  T = declare_3x3_matrix(
			0.0, 0.0, 0.0,
			0.0, fi , 0.0,
			0.0, 0.0, 1.0);

	vector <vector <double> >  DIR_MX2 = transpose(DIR_MX1);

	vector <vector <double> >  OUT = mult_mtrx (DIR_MX1, T);

	OUT = mult_mtrx(OUT, DIR_MX2);

	return OUT; //OK
}

vector <VCTR> net_to_centroids (vector <vector <vector <VCTR> > > NET) {

	vector <VCTR> OUT;

	for (size_t i = 0; i < NET.size(); i++) {
		for (size_t j = 0; j < NET.at(i).size(); j++) {
			for (size_t k = 0; k < NET.at(i).at(j).size(); k++) {

				OUT.push_back(NET.at(i).at(j).at(k));
			}
		}
	}

	OUT.push_back(declare_vector(10e-8, 10e-8, 1.0 - 10e-8));

	return OUT; //OK
}

void BRUTEFORCE (vector <GDB> inGDB, INPSET inset) {

	vector <vector <vector <VCTR> > > NET  = generate_net ();

	vector <VCTR> CNTR = net_to_centroids (NET);

	int counter = 0;

	STRESSTENSOR st;

	string filename = "BRUTEFORCE.TXT";

	ofstream o(filename.c_str());



	for (size_t i = 0; i < CNTR.size(); i++) {
		for (size_t fi = 0; fi < 11; fi++) {
			for (size_t angle = 0; angle < 175; angle+=10) {

				vector <vector <double> > M1 = DIR_MX1_from_n1 (CNTR.at(i), angle);

				vector <vector <double> > T = st_from_reduced_stresstensor (M1, fi / 10.0);

				st._11 = T.at(0).at(0);
				st._12 = T.at(0).at(1);
				st._13 = T.at(0).at(2);
				st._22 = T.at(1).at(1);
				st._23 = T.at(1).at(2);
				st._33 = T.at(2).at(2);

				inGDB = return_stressvector_estimators (st, inGDB, "BRUTEFORCE", false);

				double MISFIT = 0.0;

				for (size_t m = 0; m < inGDB.size (); m++) {

						MISFIT = MISFIT + inGDB.at(m).ANG;
				}

				/*

				if (fi == 0) {

					o << fixed << setprecision (3) << flush;

					o
					<< CNTR.at(i).X << '\t'
					<< CNTR.at(i).Y << '\t'
					<< CNTR.at(i).Z << '\t'
					<< angle << '\t'
					<< fi / 10.0 << '\t'
					<< MISFIT / inGDB.size() << endl;
				}
*/
				counter++;
			}
		}

		cout << "-------------" << i << "-------------" << endl;
	}

	cout << counter << endl;
}



void dbg_cout_matrix (vector <vector <double> > IN) {

	cout << fixed << setprecision(3) << endl;

	cout << "--------------------------------" << endl;

	for (size_t m = 0; m < IN.size(); m++) {
		for (size_t n = 0; n < IN.at(m).size(); n++) {

			cout << IN.at(m).at(n) << '\t' << flush;
		}
		cout << endl;
	}
}
