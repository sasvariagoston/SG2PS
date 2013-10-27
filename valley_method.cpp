// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <vector>

#include "structs.h"
#include "rup_clustering.hpp"
#include "valley_method.hpp"

using namespace std;

vector <HISTOGRAM> generate_DATA_histogram (vector <double> in, size_t bin_number) {

	double bin_size = bin_size_for_DATA (in, bin_number);

	vector <HISTOGRAM> H;

	double range_min = in.at(0);
	double range_max = range_min + bin_size;

	for (size_t i = 0; i < bin_number; i++) {

		HISTOGRAM buffer;

		buffer.BIN_MIN = range_min;
		buffer.COUNT = DATA_number_in_range (in, range_min, range_max);

		H.push_back(buffer);

		range_min = range_min + bin_size;
		range_max = range_max + bin_size;
	}

	if (H.size() == 1) H.at(0).COUNT = 999;

	return H;
}

vector <VALLEY> generate_valley_graph (vector <HISTOGRAM> H) {

	vector <VALLEY> V;

	size_t bin_number = H.size() - 1;

	size_t bin_size = H.at(1).BIN_MIN - H.at(0).BIN_MIN;

	for (size_t i = 0; i < bin_number; i++) {

		VALLEY buffer;

		buffer.BIN_ID = i + 1;

		if 		(H.at(i).COUNT <  H.at(i+1).COUNT) 	buffer.DIR = "U";
		else if (H.at(i).COUNT == H.at(i+1).COUNT) 	buffer.DIR = "N";
		else 										buffer.DIR = "D";

		buffer.BIN_CENTER = H.at(i).BIN_MIN + (bin_size / 2);

		V.push_back(buffer);
	}
	return V;
}

vector <VALLEY> reduce_N_valley_graph (vector <VALLEY> inV) {

	vector <VALLEY> outV;

	size_t bin_number = inV.size();

	for (size_t i = 0; i < bin_number; i++) {

		VALLEY buffer;

		buffer.BIN_ID = i + 1;

		if (inV.at(i).DIR != "N") {

			buffer = inV.at(i);
			outV.push_back(buffer);
		}
		else {};
	}
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

			buffer = inV.at(i);

			prev_UD = inV.at(i).DIR;

			outV.push_back(buffer);
		}
		else {};
	}
	return outV;
}

vector <VALLEY> create_valley_graph (vector <VALLEY> V) {

	vector <VALLEY> out;

	VALLEY buffer;

	if (V.size() == 2 && V.at(0).DIR != "D" && V.at(1).DIR != "U") {

		buffer.BIN_CENTER = 0.0;
		buffer.BIN_ID = 999;
		buffer.DIR = "X";
	}
	else {

		for (size_t i = 0; i < V.size() - 1; i++) {

			if (V.at(i).DIR == "D" && V.at(i+1).DIR == "U") {

				buffer.BIN_CENTER = (V.at(i).BIN_CENTER + V.at(i+1).BIN_CENTER) / 2;
				buffer.BIN_ID = 999;
				buffer.DIR = "V";

				out.push_back(buffer);
			}
		}
	}
	return out;
}

vector <VALLEY> return_valleys (vector <double> in, size_t bin_number) {

	vector <HISTOGRAM> H = generate_DATA_histogram (in, bin_number);

	vector <VALLEY> V;

	VALLEY buffer;

	if (H.size() == 1 && H.at(0).COUNT == 999) {

		buffer.BIN_CENTER = 0.0;
		buffer.BIN_ID = 999;
		buffer.DIR = "X";

		V.push_back (buffer);

		return V;
	}

	V = generate_valley_graph (H);

	V = reduce_N_valley_graph (V);

	return create_valley_graph (V);
}

void dbg_cout_H (vector <HISTOGRAM> inH) {

	cout << endl;

	cout << fixed << setprecision (4) << endl;

	for (size_t i = 0; i < inH.size(); i++) {

		cout
		<< inH.at(i).BIN_MIN << '\t'
		<< inH.at(i).COUNT << '\t'
		<< inH.at(i).GC
		<< endl;
	}

	cout << endl;
}

void dbg_cout_V (vector <VALLEY> inV) {

	if (inV.size() == 0) cout << "Empty valley graph" << endl;

	cout << endl;

	cout << fixed << setprecision (4) << endl;

	for (size_t i = 0; i < inV.size(); i++) {

		cout
		<< inV.at(i).BIN_CENTER << '\t'
		<< inV.at(i).BIN_ID << '\t'
		<< inV.at(i).DIR << '\t'
		<< endl;
	}

	cout << endl;
}

bool by_COUNT(const HISTOGRAM& x, const HISTOGRAM& y) {

	return x.COUNT < y.COUNT;
}

vector <HISTOGRAM> sort_by_COUNT (vector <HISTOGRAM> H) {

	sort(H.begin(), H.end(), by_COUNT);

	return H;
}
