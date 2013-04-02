// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <vector>

#include "structs.h"
#include "rup_clustering.hpp"
#include "valley_method.hpp"

using namespace std;

vector <HISTOGRAM> generate_RUP_histogram (vector <GDB> inGDB, size_t bin_number) {

	inGDB = sort_by_RUP (inGDB);

	double bin_size = bin_size_for_RUP (inGDB, bin_number);

	vector <HISTOGRAM> H;

	double range_min = inGDB.at(0).RUP;
	double range_max = range_min + bin_size;

	for (size_t i = 0; i < bin_number; i++) {

		HISTOGRAM buffer;

		buffer.BIN_MIN = range_min;
		buffer.COUNT = RUP_number_in_range (inGDB, range_min, range_max);

		H.push_back(buffer);

		range_min = range_min + bin_size;
		range_max = range_max + bin_size;
	}

	return H;
}

vector <VALLEY> generate_valley_graph (vector <HISTOGRAM> H) {

	vector <VALLEY> V;

	size_t bin_number = H.size() - 1;

	for (size_t i = 0; i < bin_number; i++) {

		VALLEY buffer;

		buffer.BIN_ID = i + 1;

		if 		(H.at(i).COUNT <  H.at(i+1).COUNT) 	buffer.DIR = "U";
		else if (H.at(i).COUNT == H.at(i+1).COUNT) 	buffer.DIR = "N";
		else 										buffer.DIR = "D";

		V.push_back(buffer);
	}

	dbg_cout_V (V);

	return V;
}

vector <VALLEY> reduce_N_valley_graph (vector <VALLEY> inV) {

	vector <VALLEY> outV;

	size_t bin_number = inV.size();

	for (size_t i = 0; i < bin_number; i++) {

		VALLEY buffer;

		buffer.BIN_ID = i + 1;

		if (inV.at(i).DIR != "N") {

			buffer.DIR = inV.at(i).DIR;
			outV.push_back(buffer);
		}
		else {};
	}

	dbg_cout_V (outV);

	return outV;
}

vector <VALLEY> reduce_UD_valley_graph (vector <VALLEY> inV) {

	vector <VALLEY> outV;

	size_t bin_number = inV.size();

	string prev_UD = "";

	for (size_t i = 0; i < bin_number; i++) {

		VALLEY buffer;

		buffer.BIN_ID = i + 1;

		if (inV.at(i).DIR != prev_UD) {

			buffer.DIR = inV.at(i).DIR;

			prev_UD = inV.at(i).DIR;

			outV.push_back(buffer);
		}
		else {};
	}

	dbg_cout_V (outV);

	return outV;
}

size_t return_DU_number (vector <VALLEY> inV) {

	size_t DU_counter = 0;

	for (size_t i = 0; i < inV.size() - 1; i++) {

		if (inV.at(i).DIR == "D" && inV.at(i + 1).DIR == "U") DU_counter++;
	}

	cout << "DU_counter: " << DU_counter << endl;

	return DU_counter;
}

size_t return_ideal_clusternumber (vector <GDB> inGDB, size_t bin_number) {

	vector <HISTOGRAM> H = generate_RUP_histogram (inGDB, bin_number);

	vector <VALLEY> V = generate_valley_graph (H);

	V = reduce_N_valley_graph (V);

	V = reduce_UD_valley_graph (V);

	return return_DU_number (V);
}

void dbg_cout_H (vector <HISTOGRAM> inH) {

	cout << endl;

	cout << fixed << setprecision (4) << endl;

	for (size_t i = 0; i < inH.size(); i++) {

		cout
		<< inH.at(i).BIN_MIN << '\t'
		<< inH.at(i).COUNT
		<< endl;
	}

	cout << endl;
}

void dbg_cout_V (vector <VALLEY> inV) {

	cout << endl;

	cout << fixed << setprecision (4) << endl;

	for (size_t i = 0; i < inV.size(); i++) {

		cout
		<< inV.at(i).DIR
		<< flush;
	}

	cout << endl;
}
