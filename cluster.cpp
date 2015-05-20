
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <fstream>
#include <iomanip>
#include <iostream>

#include "allowed_keys.hpp"
#include "color_management.hpp"
#include "common.h"
#include "cluster.h"
#include "data_io.h"
#include "random.hpp"
#include "rgf.h"
#include "run_mode.h"
#include "settings.hpp"
#include "standard_output.hpp"

using namespace std;

 vector <size_t> init_whichgroup_uni (const size_t cluster_number, const vector <GDB>& inGDB) {

	vector <size_t> which_group;

	size_t j = 0;

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (j == cluster_number) j = 1;
		else j++;

		which_group.push_back(j);
	}
	return which_group;
}

 vector <size_t> init_whichgroup_rnd (const size_t cluster_number, const vector <GDB>& inGDB) {

 	vector <size_t> which_group;

 	uniform_generator_reset();

 	for (size_t i = 0; i < inGDB.size(); i++) {

 		double rnd = uniform_0_1() * cluster_number;

 		for (size_t j = 0; j < cluster_number; j++) {

 			if (is_in_range(j, j + 1, rnd)) which_group.push_back (j+1);
 		}
 	}
 	return which_group;
 }

 vector <CENTR_VECT> init_centroid_vector (const size_t cluster_number) {

	 CENTR_VECT CV;
	 vector <CENTR_VECT> OUT;

	 for (size_t i = 0; i < cluster_number; i++) {

		 CV = declare_vector (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		 OUT.push_back(CV);
	 }
	 return OUT;
 }

 vector <CENTR_VECT> compute_centroid_from_which_group (const size_t cluster_number, const vector <size_t>& which_group, const vector <GDB>& inGDB) {

	 vector <CENTR_VECT> new_centroid = init_centroid_vector (cluster_number);

	 const bool SC = is_allowed_SC_datatype(inGDB.at(0).DATATYPE);
	 const bool STRIAE = is_allowed_striae_datatype(inGDB.at(0).DATATYPE);

	 for (size_t i = 1; i < cluster_number + 1; i++) {

		 size_t processed_data_number = 0;

		 for (size_t j = 0; j < which_group.size(); j++) {

			 const bool IDENTICAL = which_group.at(j) == i;

			 if (IDENTICAL) {

				 processed_data_number++;

				 new_centroid.at(i-1).U = new_centroid.at(i-1).U + inGDB.at(j).N.X;
				 new_centroid.at(i-1).V = new_centroid.at(i-1).V + inGDB.at(j).N.Y;
				 new_centroid.at(i-1).W = new_centroid.at(i-1).W + inGDB.at(j).N.Z;

				 if (STRIAE || SC) {

					 new_centroid.at(i-1).X = new_centroid.at(i-1).X + inGDB.at(j).DC.X;
					 new_centroid.at(i-1).Y = new_centroid.at(i-1).Y + inGDB.at(j).DC.Y;
					 new_centroid.at(i-1).Z = new_centroid.at(i-1).Z + inGDB.at(j).DC.Z;
				 }
			 }
		 }
		 if (processed_data_number > 0) new_centroid.at(i-1) = unitvector (new_centroid.at(i-1));
	 }
	 return new_centroid;
 }

 double compute_distance (const CENTR_VECT& centroid, const GDB& in) {

	 const bool SC = is_allowed_SC_datatype(in.DATATYPE);
	 const bool STRIAE = is_allowed_striae_datatype(in.DATATYPE);

	 const CENTR_VECT C = centroid;
	 const VCTR N = in.N;

	 const VCTR SV = in.DC;
	 //was: const VCTR SV = in._SV;

	 const VCTR DC = in.DC;

	 double distance = (C.U- N.X) * (C.U- N.X) + (C.V- N.Y) * (C.V- N.Y) + (C.W- N.Z) * (C.W- N.Z);

	 if (STRIAE) {

		 distance = distance + (C.X-SV.X) * (C.X-SV.X) + (C.Y-SV.Y) * (C.Y-SV.Y) + (C.Z-SV.Z) * (C.Z-SV.Z);
	 }
	 else if (SC) {

		 distance = distance + (C.X-DC.X) * (C.X-DC.X) + (C.Y-DC.Y) * (C.Y-DC.Y) + (C.Z-DC.Z) * (C.Z-DC.Z);
	 }
	 return distance * distance;
 }

vector <vector <double> > compute_distance_matrix_from_centroid (const vector <GDB>& inGDB, const vector <CENTR_VECT>& centroid) {

	vector <vector <double> > distance_matrix = init_matrix(inGDB.size(), centroid.size());

	for (size_t i = 0; i < inGDB.size(); i++) {
		for (size_t j = 0; j < centroid.size(); j++) {

			distance_matrix.at(i).at(j) = compute_distance (centroid.at(j), inGDB.at(i));
		}
	}
	return distance_matrix;
}

vector <size_t> compute_whichgroup_from_distances (const vector <vector <double> >& distance_matrix) {

	vector <size_t> which_group;

	size_t THIS_GROUP = 0;

	const size_t record_number = distance_matrix.size();
	const size_t cluster_number = distance_matrix.at(0).size();

	for (size_t i = 0; i < record_number; i++) {

		double DST_MIN = 36.0;

		for (size_t j = 0; j < cluster_number; j++) {

			if (distance_matrix.at(i).at(j) < DST_MIN)  {

				DST_MIN = distance_matrix.at(i).at(j);
				THIS_GROUP = j+1;
			}
		}
		which_group.push_back(THIS_GROUP);
	}
	return which_group;
}

double cumulative_distance (const vector <vector <double> >& distance_matrix, const vector <size_t>& which_group) {

	double cml_dst = 0.0;

	for (size_t i = 0; i < distance_matrix.size(); i++) {

		const size_t j = which_group.at(i);

		cml_dst = cml_dst + distance_matrix.at(i).at(j-1);
	}
	return cml_dst;
}

vector <vector <double> > clustering_cycle (const size_t cluster_number, const vector <GDB>& inGDB) {

	vector <vector <double> > distance_matrix_uni = init_matrix (inGDB.size(), cluster_number);
	vector <size_t> whichgroup_uni = init_whichgroup_uni (cluster_number, inGDB);
	vector <CENTR_VECT> centroid_uni;

	for (size_t i = 0; i < 10; i++) {

		centroid_uni = compute_centroid_from_which_group (cluster_number, whichgroup_uni, inGDB);
		distance_matrix_uni = compute_distance_matrix_from_centroid (inGDB, centroid_uni);
		whichgroup_uni = compute_whichgroup_from_distances (distance_matrix_uni);
	}
	const double cml_dst_uni = (cumulative_distance (distance_matrix_uni, whichgroup_uni) * 100.0) / (inGDB.size() * cluster_number);

	vector <vector <double> > distance_matrix_rnd = init_matrix(inGDB.size(), cluster_number);
	vector <size_t> whichgroup_rnd = init_whichgroup_rnd (cluster_number, inGDB);
	vector <CENTR_VECT> centroid_rnd;

	for (size_t i = 0; i < 10; i++) {

		centroid_rnd = compute_centroid_from_which_group (cluster_number, whichgroup_rnd, inGDB);
		distance_matrix_rnd = compute_distance_matrix_from_centroid (inGDB, centroid_rnd);
		whichgroup_rnd = compute_whichgroup_from_distances (distance_matrix_rnd);
	}
	const double cml_dst_rnd = (cumulative_distance (distance_matrix_rnd, whichgroup_rnd) * 100.0) / (inGDB.size() * cluster_number);

	vector <size_t> whichgroup;
	double cml_dst;
	vector <vector <double> > distance_matrix;

	if (cml_dst_uni < cml_dst_rnd) {

		whichgroup = whichgroup_uni;
		distance_matrix = distance_matrix_uni;
		cml_dst = cml_dst_uni;
	}
	else {

		whichgroup = whichgroup_rnd;
		distance_matrix = distance_matrix_rnd;
		cml_dst = cml_dst_rnd;
	}

	size_t group_counter = 0;

	for (size_t i = 0; i < 9; i++) {

		if (existence_of_group (i, whichgroup)) group_counter++;
	}

	const bool STRIAE = is_allowed_striae_datatype (inGDB.at(0).DATATYPE);

	if (STRIAE && (is_RUP_CLUSTERING_ANG() || is_RUP_CLUSTERING_RUP())) {

		if (!is_mode_DEBUG()) cout << "    - RUP clustering into "
		<< group_counter << " clusters with "
		<< fixed << setprecision (2) << cml_dst << "% error."
		<< endl;
	}
	else {

		if (!is_mode_DEBUG()) {

			if (is_CLUSTERING_AUTOMATIC())	cout << "  - Automatic k-means clustering of '" << flush;
			else 							cout << "  - User defined k-means clustering of '" << flush;

			cout
			<< inGDB.at(0).LOC << "' location, '"
			<< inGDB.at(0).DATATYPE << "' data set, '"
			<< inGDB.at(0).GC << "' group into "
			<< group_counter << " clusters with "
			<< fixed << setprecision (2) << cml_dst << "% error." << endl;
		}
	}
	return distance_matrix;
}

vector <GDB> k_means_clustering (const size_t cluster_number, const vector <GDB>& inGDB) {

	vector <GDB> outGDB = inGDB;
	vector <vector <double> > distance_matrix;
	vector <size_t> whichgroup;

	if (cluster_number > 1) {

		distance_matrix = clustering_cycle (cluster_number, outGDB);

		whichgroup = compute_whichgroup_from_distances (distance_matrix);

		if (is_CHK_K_MEANS()) {

			dump_DISTANCE_MATRIX_to_file (distance_matrix, "ST_K-MEANS");
			dump_WHICH_GROUP_to_file (whichgroup, "ST_K-MEANS");
		}
	}
	outGDB = attach_k_means_group_codes (whichgroup, outGDB); //ok

	return outGDB;
}

vector <GDB> K_MEANS (const vector <GDB>& inGDB) {

	size_t clusternumber = 0;

	if (is_CLUSTERING_AUTOMATIC()) clusternumber = 9;

	else clusternumber = string_to_int (is_CLUSTERNUMBER());

	vector <GDB> outGDB = k_means_clustering (clusternumber, inGDB);

	return outGDB;
}

void dbg_distance_matrix (const vector <vector <double> >& distance_matrix) {

	cout << fixed << setprecision(8) << endl;

	cout << "-------- START DISTANCE MATRIX --------" << endl;

	//note:
	//distance_matrix.size() 		= inGDB.size
	//distance_matrix.at(0).size() 	= cluster_number

	for (size_t i = 0; i < distance_matrix.size(); i++) {
		for (size_t j = 0; j < distance_matrix.at(0).size(); j++) {

			cout << distance_matrix.at(i).at(j) << '\t' << flush;
		}
		cout << endl;
	}
	cout << "-------- END DISTANCE MATRIX --------" << endl;

	return;
}

void dbg_whichgroup (const vector <size_t>& whichgroup) {

	cout << fixed << setprecision(8) << endl;

	cout << "-------- START WHICH GROUP --------" << endl;

	for (size_t i = 0; i < whichgroup.size(); i++) {

		cout << whichgroup.at(i) << endl;
	}
	cout << "-------- END WHICH GROUP --------" << endl;

	return;
}

void dbg_centroid_vector (const vector <CENTR_VECT>& centroid) {

	cout << fixed << setprecision(6) << endl;

	cout << "-------- START CENTROID VECTOR --------" << endl;

	for (size_t i = 0; i < centroid.size(); i++) {

		cout
		<< centroid.at(i).U << '\t'
		<< centroid.at(i).V << '\t'
		<< centroid.at(i).W << '\t'
		<< centroid.at(i).X << '\t'
		<< centroid.at(i).Y << '\t'
		<< centroid.at(i).Z << '\t'
		<< endl;
	}
	cout << "-------- END CENTROID VECTOR --------" << endl;

	return;
}
