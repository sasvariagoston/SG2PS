// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <map>
#include <iomanip>

#include "common.h"
#include "cluster.h"
#include "ps.h"

vector <CENTR_VECT>  init_centriod (size_t cluster_number, vector <GDB> inGDB) {

	CENTR_VECT CV;
	vector <CENTR_VECT> CENTR;
	size_t j = 0;

	if ((inGDB.at(0).DATAGROUP == "STRIAE") || (inGDB.at(0).DATAGROUP == "SC")) {

		do {

			srand(j + 2);

			CV.U = rand();

			CV.U = rand();		CV.U = (CV.U / RAND_MAX) * 2.0 - 1.0;
			CV.V = rand(); 		CV.V = (CV.V / RAND_MAX) * 2.0 - 1.0;
			CV.W = rand(); 		CV.W = (CV.W / RAND_MAX);
			CV.X = rand(); 		CV.X = (CV.X / RAND_MAX) * 2.0 - 1.0;
			CV.Y = rand(); 		CV.Y = (CV.Y / RAND_MAX) * 2.0 - 1.0;
			CV.Z = rand(); 		CV.Z = (CV.Z / RAND_MAX);

			CV = unitvector (CV);
			CENTR.push_back(CV);
			j++;

		} while (j < cluster_number + 1);
	}

	else {

		do {

			srand(j + 2);

			CV.U = rand();

			CV.U = rand();			CV.U = (CV.U / RAND_MAX) * 2.0 - 1.0;
			CV.V = rand();			CV.V = (CV.V / RAND_MAX) * 2.0 - 1.0;
			CV.W = rand();			CV.W = (CV.W / RAND_MAX);
			CV.X = 0.0;
			CV.Y = 0.0;
			CV.Z = 0.0;

			CV = unitvector (CV);

			CENTR.push_back(CV);
			j++;

		} while (j < cluster_number + 1);
	}

	return CENTR;
}

vector <vector <double> > init_distance_matrix (size_t cluster_number, vector <GDB> inGDB) {

	size_t j = 0;
	size_t k = 0;
	vector <double> distance_from_centroid;
	vector <vector <double> > distance_matrix;

	do {

		distance_from_centroid.push_back(0.0);

		j++;

	} while (j < cluster_number);

	do {

		distance_matrix.push_back(distance_from_centroid);

		k++;

	} while (k < inGDB.size());

	return distance_matrix;
}

 vector <int> init_whichgroup (size_t cluster_number, vector <GDB> inGDB) {

	vector <int> which_group;
	size_t k = 0;
	size_t j = 0;

	do {

		which_group.push_back(j);

		if (j == cluster_number - 1) j = 0;
		else j++;

		k++;

	} while (k < inGDB.size());

	return which_group;
}

 vector <int> init_whichgroup_II (size_t cluster_number, vector <GDB> inGDB) {

 	vector <int> which_group;
 	size_t k = 0;
 	double rnd = 0;
 	size_t l = 0;

 	srand (1);
 	rnd = rand();
 	rnd = rand();

 	do {

 		rnd = rand();
 		rnd = (rnd / RAND_MAX) * (cluster_number);

 		do {

 			if ((rnd <= l + 1) && (rnd > l)) which_group.push_back(l);

 			l++;

 		} while (l < cluster_number);

 		l = 0;
 		k++;

 	} while (k < inGDB.size());

 	return which_group;
 }

double compute_distance (CENTR_VECT centroid, GDB inGDB) {

	double distance;

	if ((inGDB.DATAGROUP == "STRIAE") || (inGDB.DATAGROUP == "SC"))

		distance =
				(	(centroid.U - inGDB.N.X)  * (centroid.U - inGDB.N.X) +
					(centroid.V - inGDB.N.Y)  * (centroid.V - inGDB.N.Y) +
					(centroid.W - inGDB.N.Z)  * (centroid.W - inGDB.N.Z) +
					(centroid.X - inGDB.NC.X) * (centroid.X - inGDB.SV.X) +
					(centroid.Y - inGDB.NC.Y) * (centroid.Y - inGDB.SV.Y) +
					(centroid.Z - inGDB.NC.Z) * (centroid.Z - inGDB.SV.Z));
	else

		distance =
				(	(centroid.U - inGDB.N.X)  * (centroid.U - inGDB.N.X) +
				    (centroid.V - inGDB.N.Y)  * (centroid.V - inGDB.N.Y) +
				    (centroid.W - inGDB.N.Z)  * (centroid.W - inGDB.N.Z));

	return distance * distance;

}

vector <vector <double> > compute_distance_matrix_from_centroid (vector <vector <double> > distance_matrix, vector <GDB> inGDB, vector <CENTR_VECT> centroid) {

	size_t j = 0;
	size_t k = 0;

	do {

		do {

			distance_matrix.at(k).at(j) = compute_distance (centroid.at(j), inGDB.at(k));

			j++;

		} while (j < centroid.size());

		j = 0;

		k++;

	} while (k <  inGDB.size());

	return distance_matrix;
}

vector <int> compute_whichgroup_from_distances (vector <vector <double> > distance_matrix) {

	size_t j = 0;
	size_t k = 0;
	int mingroup_j = 0;

	double min_distance = 6.0;

	size_t cluster_number = distance_matrix.at(0).size() - 1;
	size_t record_number = distance_matrix.size();

	vector <CENTR_VECT> new_centroid;
	vector <int> which_group;
	CENTR_VECT CV;

	do {

		CV = declare_vector (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		new_centroid.push_back(CV);

		j++;

	} while (j < cluster_number + 1);

	j = 0;

	do {

		do {

			if (distance_matrix.at(k).at(j) < min_distance)  {

				min_distance = distance_matrix.at(k).at(j);
				mingroup_j = j;
			}

			j++;

		} while (j < cluster_number + 1);

		which_group.push_back(mingroup_j);

		j = 0;
		min_distance = 6.0;
		k++;

	} while (k <  record_number);

	return which_group;
}

vector <CENTR_VECT> compute_centroid_from_which_group (size_t cluster_number, vector <int> which_group, vector <GDB> inGDB) {

	size_t j = 0;
	size_t k = 0;

	size_t record_number = inGDB.size();
	vector <CENTR_VECT> new_centroid;
	CENTR_VECT CV;

	do {

		CV = declare_vector (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		new_centroid.push_back(CV);
		j++;

	} while (j < cluster_number);

	j = 0;

	do {

		do {

			if ((which_group.at(k)) == j) {

				if ((inGDB.at(0).DATAGROUP == "STRIAE") || (inGDB.at(0).DATAGROUP == "SC")) {

					new_centroid.at(j).U = new_centroid.at(j).U + inGDB.at(k).N.X;
					new_centroid.at(j).V = new_centroid.at(j).V + inGDB.at(k).N.Y;
					new_centroid.at(j).W = new_centroid.at(j).W + inGDB.at(k).N.Z;
					new_centroid.at(j).X = new_centroid.at(j).X + inGDB.at(k).SV.X;
					new_centroid.at(j).Y = new_centroid.at(j).Y + inGDB.at(k).SV.Y;
					new_centroid.at(j).Z = new_centroid.at(j).Z + inGDB.at(k).SV.Z;
				}

				else {

					new_centroid.at(j).U = new_centroid.at(j).U + inGDB.at(k).N.X;
					new_centroid.at(j).V = new_centroid.at(j).V + inGDB.at(k).N.Y;
					new_centroid.at(j).W = new_centroid.at(j).W + inGDB.at(k).N.Z;
				}
			}

			k++;

		} while (k < record_number);

		new_centroid.at(j) = unitvector (new_centroid.at(j));

		k = 0;
		j++;

	} while (j < cluster_number);

	return new_centroid;
}

vector <GDB> attach_group_codes (vector <int> which_group, vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;
	size_t i = 0;

	do {

		if 		(which_group.at(i) == 1) outGDB.at(i).GC = "A";
		else if (which_group.at(i) == 2) outGDB.at(i).GC = "B";
		else if (which_group.at(i) == 3) outGDB.at(i).GC = "C";
		else if (which_group.at(i) == 4) outGDB.at(i).GC = "D";
		else if (which_group.at(i) == 5) outGDB.at(i).GC = "E";
		else if (which_group.at(i) == 6) outGDB.at(i).GC = "F";
		else if (which_group.at(i) == 7) outGDB.at(i).GC = "G";
		else if (which_group.at(i) == 8) outGDB.at(i).GC = "H";
		else if (which_group.at(i) == 9) outGDB.at(i).GC = "I";
		else inGDB.at(i).GC = "X";

		i++;

	} while (i < outGDB.size());

	return outGDB;
}

double cumulative_distance (vector <vector <double> > distance_matrix, vector <int> which_group) {

	double cml_dst = 0.0;

	size_t k = 0;
	size_t j;

	do {

		j = which_group.at(k);
		cml_dst = cml_dst + distance_matrix.at(k).at(j);

		k++;

	} while (k < distance_matrix.size());

	return cml_dst;
}

vector <vector <double> > clustering_cycle (size_t cluster_number, vector <GDB> inGDB, INPSET i) {

	vector <CENTR_VECT> centroid;
	vector <vector <double> > distance_matrix;
	vector <int> whichgroup;

	int group_counter = 0;

	size_t j = 0;

	double cml_dst_1 = 0;
	double cml_dst_2 = 0;

	distance_matrix = init_distance_matrix (cluster_number, inGDB);
	whichgroup = init_whichgroup (cluster_number, inGDB);

	do {

		centroid = compute_centroid_from_which_group (cluster_number, whichgroup, inGDB);
		distance_matrix = compute_distance_matrix_from_centroid (distance_matrix, inGDB, centroid);
		whichgroup = compute_whichgroup_from_distances (distance_matrix);

		j++;

	} while (j < 10);

	cml_dst_1 = (cumulative_distance (distance_matrix, whichgroup) * 100.0) / inGDB.size();

	j = 0;

	distance_matrix = init_distance_matrix (cluster_number, inGDB);
	whichgroup = init_whichgroup_II (cluster_number, inGDB);

	do {

		centroid = compute_centroid_from_which_group (cluster_number, whichgroup, inGDB);
		distance_matrix = compute_distance_matrix_from_centroid (distance_matrix, inGDB, centroid);
		whichgroup = compute_whichgroup_from_distances (distance_matrix);

		j++;

	} while (j < 10);

	cml_dst_2 = (cumulative_distance (distance_matrix, whichgroup) * 100.0) / inGDB.size();

	j = 0;

	distance_matrix = init_distance_matrix (cluster_number, inGDB);

	if (cml_dst_1 < cml_dst_2) 	whichgroup = init_whichgroup (cluster_number, inGDB);
	else 						whichgroup = init_whichgroup_II (cluster_number, inGDB);

	do {

		centroid = compute_centroid_from_which_group (cluster_number, whichgroup, inGDB);
		distance_matrix = compute_distance_matrix_from_centroid (distance_matrix, inGDB, centroid);
		whichgroup = compute_whichgroup_from_distances (distance_matrix);

		j++;

	} while (j < 10);

	cml_dst_1 = (cumulative_distance (distance_matrix, whichgroup) * 100.0) / inGDB.size();

	if (existence_of_group (0, whichgroup)) group_counter++;
	if (existence_of_group (1, whichgroup)) group_counter++;
	if (existence_of_group (2, whichgroup)) group_counter++;
	if (existence_of_group (3, whichgroup)) group_counter++;
	if (existence_of_group (4, whichgroup)) group_counter++;
	if (existence_of_group (5, whichgroup)) group_counter++;
	if (existence_of_group (6, whichgroup)) group_counter++;
	if (existence_of_group (7, whichgroup)) group_counter++;
	if (existence_of_group (8, whichgroup)) group_counter++;
	if (existence_of_group (9, whichgroup)) group_counter++;


	if (i.clusternumber == "A") cout << "  - Automatic k-means clustering of '" << flush;
	else  						cout << "  - User defined k-means clustering of '" << endl;

	cout

	<< inGDB.at(0).LOC << "' location, '"
	<< inGDB.at(0).DATATYPE << "' data set into "
	<< group_counter << " clusters with "
	<< fixed << setprecision (2) << cml_dst_1 << "% error." << endl;

	return distance_matrix;
}

vector <GDB> k_means_clustering (size_t cluster_number, vector <GDB> inGDB, INPSET i) {

	vector <vector <double> > distance_matrix;
	vector <int> whichgroup;
	vector <GDB> outGDB;

	if (cluster_number > 1) {

		distance_matrix = clustering_cycle (cluster_number, inGDB, i);
		whichgroup = compute_whichgroup_from_distances (distance_matrix);
		outGDB = attach_group_codes (whichgroup, inGDB);
		return outGDB;

	}

	else  {

		cout
		<< "  - No clustering of '"
		<< inGDB.at(0).LOC << "' location, '"
		<< inGDB.at(0).DATATYPE << "' data set." << endl;

		return inGDB;
	}
}

vector <GDB> k_means (INPSET i, vector <GDB> inGDB) {

	size_t clusternumber = 0;

	if (i.clusternumber == "1") clusternumber = 1;
	if (i.clusternumber == "2") clusternumber = 2;
	if (i.clusternumber == "3") clusternumber = 3;
	if (i.clusternumber == "4") clusternumber = 4;
	if (i.clusternumber == "5") clusternumber = 5;
	if (i.clusternumber == "6") clusternumber = 6;
	if (i.clusternumber == "7") clusternumber = 7;
	if (i.clusternumber == "8") clusternumber = 8;
	if (i.clusternumber == "9") clusternumber = 9;

	vector <GDB> outGDB = inGDB;

	if (i.clusternumber == "A") outGDB = k_means_clustering (9, inGDB, i);
	else 						outGDB = k_means_clustering (clusternumber, inGDB, i);

	return outGDB;
}
