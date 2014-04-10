// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include "allowed_keys.hpp"
#include "common.h"
#include "math.h"
#include "structs.h"
#include "rup_clustering.hpp"
#include "valley_method.hpp"


using namespace std;

namespace {

	const double SN = 10e-8;
}

vector <double> GDB_to_table (vector <GDB> inGDB, string field) {

	vector <double> out;

	if		(field == "ANG") inGDB = sort_by_ANG (inGDB);
	else if	(field == "RUP") inGDB = sort_by_RUP (inGDB);
	else {};

	for (size_t i = 0; i < inGDB.size(); i++) {

		if 		(field == "ANG") out.push_back(inGDB.at(i).ANG);
		else if (field == "RUP") out.push_back(inGDB.at(i).RUP);
		else {};
	}

	return out;
}

size_t DATA_number_in_range (vector <double> in, double range_min, double range_max) {

	size_t counter = 0;

	for (size_t i = 0; i < in.size(); i++) {

		if (is_in_range(range_min, range_max, in.at(i))) counter++;
	}

	return counter;
}

double bin_size_for_DATA (vector <double> in, size_t bin_number) {

	double min = in.at(0);
	double max = in.at(in.size() - 1);

	return (max - min) / bin_number;
}

RUP_table return_cost_function_member (vector <double> in, size_t bin_number) {

	RUP_table out;

	out.clusternumber = bin_number;

	out.delta = bin_size_for_DATA (in, out.clusternumber);

	double range_min = in.at(0);
	double range_max = in.at(0) + out.delta;

	double cml_mean = 0.0;
	double cml_variance = 0.0;

	for (size_t i = 0; i < out.clusternumber; i++) {

		size_t k_i = DATA_number_in_range (in, range_min, range_max);

		cml_mean = cml_mean + k_i;

		range_min = range_min + out.delta;

		range_max = range_max + out.delta;
	}

	out.k = cml_mean / out.clusternumber;


	range_min = in.at(0);
	range_max = in.at(0) + out.delta;



	for (size_t i = 0; i < out.clusternumber; i++) {

		size_t k_i = DATA_number_in_range (in, range_min, range_max);

		cml_variance = cml_variance + ((k_i - out.k) * (k_i - out.k));

		range_min = range_min + out.delta;

		range_max = range_max + out.delta;
	}

	out.v = cml_variance / out.clusternumber;

	out.C = ((2.0 * out.k) - out.v) / (out.delta * out.delta);

	return out;
}

bool by_GC(const GDB& x, const GDB& y) {

	return x.GC < y.GC;
}

bool by_RUP(const GDB& x, const GDB& y) {

	return x.RUP < y.RUP;
}

bool by_ANG(const GDB& x, const GDB& y) {

	return x.ANG < y.ANG;
}

bool by_C(const RUP_table& x, const RUP_table& y) {

	return x.C < y.C;
}

vector <GDB> sort_by_GC (vector <GDB> inGDB) {

	sort(inGDB.begin(), inGDB.end(), by_GC);

	return inGDB;
}

vector <GDB> sort_by_RUP (vector <GDB> inGDB) {

	sort(inGDB.begin(), inGDB.end(), by_RUP);

	return inGDB;
}

vector <GDB> sort_by_ANG (vector <GDB> inGDB) {

	sort(inGDB.begin(), inGDB.end(), by_ANG);

	return inGDB;
}

vector <RUP_table> sort_by_C (vector <RUP_table> RT) {

	sort(RT.begin(), RT.end(), by_C);

	return RT;
}

size_t return_DATA_ideal_bin_number (vector <double> in) {

	RUP_table buffer;

	vector <RUP_table> RT;

	for (size_t bin_number = 1; bin_number < sqrt(static_cast<double>(in.size())) * 2.0; bin_number++) {

		buffer = return_cost_function_member (in, bin_number);

		RT.push_back(buffer);
	}

	RT = sort_by_C (RT);

	return RT.at(0).clusternumber;
}

vector <VALLEY> return_valleygraph_for_dataset (vector <GDB> inGDB, string field) {

	vector <double> in = GDB_to_table (inGDB, field);

	size_t bin_number = return_DATA_ideal_bin_number (in);

	return return_valleys (in, bin_number);
}

void dbg_cout_GDB_RUP (vector <GDB> inGDB) {

	cout << endl;

	cout << "RUP/ANG" << endl;

	cout << fixed << setprecision (4);

	for (size_t i = 0; i < inGDB.size(); i++) {

		cout
		<< inGDB.at(i).ID << '\t'
		<< inGDB.at(i).ANG << '\t'
		<< inGDB.at(i).GC << '\t'
		<< endl;
	}
}


void dbg_cout_IN_RUP (vector <double> in) {

	cout << endl;

	cout << "RUP/ANG" << endl;

	cout << fixed << setprecision (4);

	for (size_t i = 0; i < in.size(); i++) {

		cout << in.at(i) << endl;
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

vector <GDB> associate_GDB_DATA_clusters (vector <GDB> inGDB, vector <VALLEY> V, INPSET inset, string method) {

	vector <string> GC = allowed_groupcode_str_vector();

	bool is_RUP = (method == "RUP");
	bool is_ANG = (method == "ANG");
	bool is_RUP_clustering = (inset.clustering_RUP_ANG == "R");
	bool is_ANG_clustering = (inset.clustering_RUP_ANG == "A");

	for (size_t j = 0; j < inGDB.size(); j++) {

		if ((is_RUP_clustering || is_ANG_clustering) && (V.size() == 0))	inGDB.at(j).GC = GC.at(0);
		else if (is_RUP && !is_RUP_clustering) 								inGDB.at(j).GC = GC.at(0);
		else if (is_ANG && !is_ANG_clustering) 								inGDB.at(j).GC = GC.at(0);
		else {

			for (size_t i = 0; i < V.size(); i++) {

				if 	(inGDB.at(j).RUP < V.at(0).BIN_CENTER) inGDB.at(j).GC = GC.at(0);
				else if (inGDB.at(j).RUP > V.at(V.size()-1).BIN_CENTER) inGDB.at(j).GC = GC.at(i+1);  //was i+=2
				else if (i > 0 && is_in_range (V.at(i-1).BIN_CENTER, V.at(i).BIN_CENTER, inGDB.at(j).RUP)) inGDB.at(j).GC = GC.at(i);  //was i+1
				else {}
			}
		}
	}

	return inGDB;
}
