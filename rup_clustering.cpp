
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include "structs.h"
#include "rup_clustering.hpp"
#include "valley_method.hpp"


using namespace std;

namespace {

	const double SN = 10e-8;
}


size_t RUP_number_in_range (vector <GDB> inGDB, double range_min, double range_max) {

	size_t counter = 0;

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (range_min - SN <= inGDB.at(i).RUP && inGDB.at(i).RUP <= range_max + SN) counter++;
	}

	return counter;
}

double bin_size_for_RUP (vector <GDB> inGDB, size_t bin_number) {

	inGDB = sort_by_RUP (inGDB);

	double RUP_min = inGDB.at(0).RUP;
	double RUP_max = inGDB.at(inGDB.size() - 1).RUP;

	return (RUP_max - RUP_min) / bin_number;
}

RUP_table return_cost_function_member (vector <GDB> inGDB, size_t bin_number) {

	RUP_table out;

	out.clusternumber = bin_number;

	//const double init_bin_size = inGDB.at(inGDB.size() - 1).RUP - inGDB.at(0).RUP;

	//out.delta = init_bin_size / out.clusternumber;

	out.delta = bin_size_for_RUP (inGDB, out.clusternumber);

	double range_min = inGDB.at(0).RUP;
	double range_max = inGDB.at(0).RUP + out.delta;

	double cml_mean = 0.0;
	double cml_variance = 0.0;

	for (size_t i = 0; i < out.clusternumber; i++) {

		size_t k_i = RUP_number_in_range (inGDB, range_min, range_max);

		cml_mean = cml_mean + k_i;

		if (out.clusternumber == 7) {

			//cout << range_min << " -- " << range_max << " -- " << k_i << endl;
		}

		range_min = range_min + out.delta;

		range_max = range_max + out.delta;


	}

	out.k = cml_mean / out.clusternumber;


	range_min = inGDB.at(0).RUP;
	range_max = inGDB.at(0).RUP + out.delta;



	for (size_t i = 0; i < out.clusternumber; i++) {

		size_t k_i = RUP_number_in_range (inGDB, range_min, range_max);

		cml_variance = cml_variance + ((k_i - out.k) * (k_i - out.k));

		if (out.clusternumber == 7) {

			//cout << range_min << " -- " << range_max << " -- " << (k_i - out.k) * (k_i - out.k) << endl;
		}


		range_min = range_min + out.delta;

		range_max = range_max + out.delta;
	}

	out.v = cml_variance / out.clusternumber;

	out.C = ((2.0 * out.k) - out.v) / (out.delta * out.delta);

	return out;
}

bool by_RUP(const GDB& x, const GDB& y) {

	return x.RUP < y.RUP;
}

bool by_C(const RUP_table& x, const RUP_table& y) {

	return x.C < y.C;
}

vector <GDB> sort_by_RUP (vector <GDB> inGDB) {

	sort(inGDB.begin(), inGDB.end(), by_RUP);

	return inGDB;
}

vector <RUP_table> sort_by_C (vector <RUP_table> RT) {

	sort(RT.begin(), RT.end(), by_C);

	return RT;
}

size_t return_RUP_ideal_bin_number (vector <GDB > inGDB) {

	vector <GDB > outGDB = sort_by_RUP (inGDB);

	//input rendben

	//dbg_cout_GDB_RUP (outGDB);

	RUP_table buffer;

	vector <RUP_table> RT;

	for (size_t bin_number = 1; bin_number < outGDB.size(); bin_number++) {

		buffer = return_cost_function_member (outGDB, bin_number);

		RT.push_back(buffer);
	}

	RT = sort_by_C (RT);

	//dbg_cout_RUP_table (RT);

	return RT.at(0).clusternumber;
}

size_t return_clusternumber_for_dataset (vector <GDB> inGDB) {

	size_t ideal_bin_number = return_RUP_ideal_bin_number (inGDB);

	return return_ideal_clusternumber (inGDB, ideal_bin_number);
}

void dbg_cout_GDB_RUP (vector <GDB> inGDB) {

	cout << endl;

	cout << "DATA_ID" << '\t' << "RUP" << endl;

	cout << fixed << setprecision (4);

	for (size_t i = 0; i < inGDB.size(); i++) {

		cout << inGDB.at(i).ID << '\t' << inGDB.at(i).RUP << endl;
	}
}

void dbg_cout_RUP_table (vector <RUP_table> RT) {

	cout << endl;

	cout << fixed << setprecision (4);

	cout
	<< "CLUSTERNUMBER" << '\t'
	<< "DELTA" << '\t'
	<< "k" << '\t'
	<< "v" << '\t'
	<< "C" << endl;

	for (size_t i = 0; i < RT.size(); i++) {

		cout
		<< RT.at(i).clusternumber << '\t'
		<< RT.at(i).delta << '\t'
		<< RT.at(i).k << '\t'
		<< RT.at(i).v << '\t'
		<< RT.at(i).C << '\t'
		<< endl;
	}
}
