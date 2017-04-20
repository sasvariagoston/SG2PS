// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "common.h"
#include "data_sort.hpp"
#include "math.h"
#include "settings.hpp"
#include "structs.h"
#include "rgf.h"
#include "run_mode.h"
#include "rup_clustering.hpp"
#include "standard_output.hpp"
#include "valley_method.hpp"

using namespace std;

bool has_GDB_RUP_ANG_values (const vector <GDB>& inGDB, const string METHOD) {

	ASSERT_GT (inGDB.size(), 0);

	const bool RUP = METHOD == "RUP";
	const bool ANG = METHOD == "ANG";
	ASSERT_EXACTLY_ONE_TRUE (RUP, ANG);

	for (size_t i = 0; i < inGDB.size(); i++) {

		double to_check = inGDB.at(i).ANG;
		if (RUP) to_check = inGDB.at(i).RUP;

		if (is_nan(to_check)) return false;
	}
	return true;
}

vector <double> GDB_to_table (const vector <GDB>& inGDB, const string field) {

	vector <double> OUT;

	vector <GDB> outGDB = inGDB;

	const bool RUP = field == "RUP";
	const bool ANG = field == "ANG";
	ASSERT_EXACTLY_ONE_TRUE (RUP, ANG);

	if (ANG)	outGDB = SORT_GDB (inGDB, "ANG");
	else 		outGDB = SORT_GDB (inGDB, "RUP");

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (ANG)OUT.push_back (outGDB.at(i).ANG);
		else 	OUT.push_back (outGDB.at(i).RUP);
	}
	return OUT;
}

size_t DATA_number_in_range (const vector <double>& in, const double range_min, const double range_max) {

	size_t counter = 0;

	for (size_t i = 0; i < in.size(); i++) {

		if (is_in_range (range_min, range_max, in.at(i))) counter++;
	}
	return counter;
}

double bin_size_for_DATA (const vector <double>& in, const size_t bin_number) {

	const double min = in.at(0);
	const double max = in.at(in.size() - 1);

	return (max - min) / bin_number;
}

RUP_table return_cost_function_member (const vector <double>& in, const size_t bin_number) {

	RUP_table out;

	out.clusternumber = bin_number;

	out.delta = bin_size_for_DATA (in, out.clusternumber);

	double range_min = in.at(0);
	double range_max = in.at(0) + out.delta;

	double cml_mean = 0.0;
	double cml_variance = 0.0;

	for (size_t i = 0; i < out.clusternumber; i++) {

		const size_t k_i = DATA_number_in_range (in, range_min, range_max);

		cml_mean = cml_mean + k_i;

		range_min = range_min + out.delta;
		range_max = range_max + out.delta;
	}

	out.k = cml_mean / out.clusternumber;

	range_min = in.at(0);
	range_max = in.at(0) + out.delta;

	for (size_t i = 0; i < out.clusternumber; i++) {

		const size_t k_i = DATA_number_in_range (in, range_min, range_max);

		cml_variance = cml_variance + ((k_i - out.k) * (k_i - out.k));

		range_min = range_min + out.delta;
		range_max = range_max + out.delta;
	}

	out.v = cml_variance / out.clusternumber;

	ASSERT_FINITE(out.k, out.v, out.delta);
	ASSERT_NE(out.delta, 0.0);
	out.C = ((2.0 * out.k) - out.v) / (out.delta * out.delta);

	return out;
}

bool is_homogeneous (const vector <double>& in) {

	const size_t MIN = in.at(0);
	const size_t MAX = in.at(in.size() - 1);


	return (is_in_range (MIN, MIN, MAX));
}

size_t return_DATA_ideal_bin_number (const vector <double>& in) {

	if (in.size() == 1) return 1;

	vector <RUP_table> RT;

	for (size_t bin_number = 1; bin_number < sqrt(static_cast<double>(in.size())) * 2.0; bin_number++) {

		RUP_table buffer;

		if (! is_homogeneous (in)) {

			buffer = return_cost_function_member (in, bin_number);

			RT.push_back (buffer);
		}
	}
	if (RT.size() == 0) return 1;

	RT = sort_by_C (RT);

	return RT.at(0).clusternumber;
}

vector <VALLEY> return_valleygraph_for_dataset (const vector <GDB>& inGDB, const string field) {

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

vector <GDB> associate_GDB_DATA_clusters (const vector <GDB>& inGDB, const vector <VALLEY>& V, const string method) {

	vector <GDB> outGDB = inGDB;
	vector <string> GC = allowed_basic_groupcode_str_vector();

	bool is_RUP = (method == "RUP");
	bool is_ANG = (method == "ANG");

	for (size_t j = 0; j < inGDB.size(); j++) {

		if ((is_RUP_CLUSTERING_RUP() || is_RUP_CLUSTERING_ANG()) && (V.size() == 0)) {

			outGDB.at(j).GC.at(2) = GC.at(0).at(0);
		}
		else if (is_RUP && !is_RUP_CLUSTERING_RUP()) {

			outGDB.at(j).GC.at(2) = GC.at(0).at(0);
		}
		else if (is_ANG && !is_RUP_CLUSTERING_ANG()){

			outGDB.at(j).GC.at(2) = GC.at(0).at(0);
		}
		else {

			for (size_t i = 0; i < V.size(); i++) {

				double ACT;

				ASSERT_EXACTLY_ONE_TRUE (is_ANG && is_RUP_CLUSTERING_ANG(), is_RUP && is_RUP_CLUSTERING_RUP());

				if (is_ANG && is_RUP_CLUSTERING_ANG()) 	ACT = outGDB.at(j).ANG;
				else 									ACT = outGDB.at(j).RUP;

				ASSERT_FINITE (ACT);

				if (ACT < V.at(0).BIN_CENTER) {
					outGDB.at(j).GC.at(2) = GC.at(1).at(0);
				}
				else if (ACT > V.at(V.size()-1).BIN_CENTER) {
					outGDB.at(j).GC.at(2) = GC.at(i+2).at(0);
				}
				else if (i > 0 && is_in_range (V.at(i-1).BIN_CENTER, V.at(i).BIN_CENTER, ACT)){
					outGDB.at(j).GC.at(2) = GC.at(i+1).at(0);
				}
				else {}
			}
		}
	}
	return outGDB;
}

vector <GDB> apply_RUP_ANG_CLUSTERING_result (const vector <GDB>& inGDB) {

	ASSERT (is_allowed_striae_datatype (inGDB.at(0).DATATYPE));

	string FIELD = "";

	if 		(is_RUP_CLUSTERING_ANG()) FIELD = "ANG";
	else if (is_RUP_CLUSTERING_RUP()) FIELD = "RUP";
	else return inGDB;

	vector <VALLEY> V = return_valleygraph_for_dataset (inGDB, FIELD);

	if (is_CHK_RUP_ANG()) {

		vector <double> in = GDB_to_table (inGDB, FIELD);

		size_t bin_number = return_DATA_ideal_bin_number (in);

		const vector <HISTOGRAM> H = generate_DATA_histogram (in, bin_number);

		string T = "ST_RUP_ANG_" +  inGDB.at(0).LOC;
		T = T + "_" + inGDB.at(0).FORMATION;
		T = T + "_" + inGDB.at(0).DATATYPE;
		T = T + "_" + inGDB.at(0).GC;

		if (is_PROCESS_AS_TILTED()) T = T + "_TLT";
		else T = T + "_NRM";

		if (is_PROCESS_AS_TRAJECTORY()) T = T + "_TRJ";

		dump_HISTOGRAM_to_file (H, T);
		dump_VALLEY_to_file (V, T);
	}

	if (V.size() == 1 && V.at(0).DIR == "X") V.clear();

	if (is_RUP_CLUSTERING_RUP() || is_RUP_CLUSTERING_ANG()) {

		if (!is_mode_DEBUG()) {

			string METHOD = "RUP";
			if (is_RUP_CLUSTERING_ANG ()) METHOD = "ANG";

			if 		(V.size() == 0) cout << "    - Cannot cluster input data set using RUP / ANG values." << endl;
			else if (V.size() > 9) 	cout << "    - Clustering result not reliable: more than 9 clusters." << endl;
			else					cout << "    - Input data set separated into " << V.size() + 1 << " clusters using '" << METHOD << "' parameter." << endl;
		}
		if 		(is_RUP_CLUSTERING_RUP()) 	return associate_GDB_DATA_clusters (inGDB, V, FIELD);
		else if (is_RUP_CLUSTERING_ANG()) 	return associate_GDB_DATA_clusters (inGDB, V, FIELD);
		else    		 					return associate_GDB_DATA_clusters (inGDB, V, "");
	}
	return inGDB;
}
