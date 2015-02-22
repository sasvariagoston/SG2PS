// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
//#include <cmath>
//#include <iomanip>
//#include <fstream>
#include <iostream>

//#include "angelier.h"
#include "assertions.hpp"
//#include "allowed_keys.hpp"
//#include "brute_force.hpp"
//#include "bingham.h"
#include "common.h"
//#include "data_io.h"
//#include "fry.h"
//#include "inversion.h"
//#include "michael.h"
//#include "mostafa.h"
//#include "nda.h"
//#include "ps.h"
//#include "ptn.h"
//#include "rgf.h"
//#include "rup_clustering.hpp"
//#include "shan.h"
//#include "stresstensor.hpp"
//#include "structs.h"
//#include "valley_method.hpp"
//#include "yamaji.hpp"

using namespace std;

namespace {

bool WELLDATA_USE = false;
//bool WELLDATA_NO = false;

//bool WELL_INTERVAL_METER = false;
//bool WELL_INTERVAL_DATANUMBER = false;

//double WELL_INTERVAL_LENGTH = 999.99;

//bool WELL_INTERVAL_MIDDLE_MEDIAN = false;
//bool WELL_INTERVAL_MIDDLE_AVERAGE = false;

bool DATARULE_GERMAN = false;
bool DATARULE_RIGHT_HAND_RULE = false;

bool PLOT_ANGELIER = false;
bool PLOT_HOEPPENER = false;

bool NET_SCHMIDT = false;
bool NET_WULFF = false;

bool HEMISPHERE_LOWER = false;
bool HEMISPHERE_UPPER = false;

bool CONTOURING_NO = false;
bool CONTOURING_DIPDIR_BEARING = false;
bool CONTOURING_STRIKEDIR_BEARING = false;
bool CONTOURING_PLANE_NORMAL_BEARING = false;
bool CONTOURING_STRIAE_BEARING_BEARING = false;

bool TILTING_BEDDING_PALEONORTH = false;
bool TILTING_BEDDING = false;
bool TILTING_PALEONORTH = false;

bool INVERSION_SPRANG = false;
bool INVERSION_TURNER = false;
bool INVERSION_MICHAEL = false;
bool INVERSION_SHAN = false;
bool INVERSION_FRY = false;
bool INVERSION_ANGELIER = false;
bool INVERSION_MOSTAFA = false;
bool INVERSION_BRUTEFORCE = false;
bool INVERSION_YAMAJI = false;
bool INVERSION_NONE = false;

bool BINGHAM_USE = false;
bool BINGHAM_NONE = false;

double STRESSANGLE = 999.99;

bool VIRTUAL_NONE = false;
bool VIRTUAL_USE = false;

bool INPUTGROUP_NONE = false;
bool INPUTGROUP_FIRST = false;
bool INPUTGROUP_SECOND = false;
bool INPUTGROUP_THIRD = false;

bool GROUPS_USE = false;
bool GROUPS_NONE = false;

bool CLUSTERING_NONE = false;
bool CLUSTERING_AUTOMATIC = false;
string CLUSTERNUMBER = "999";

bool RUP_CLUSTERING_NONE = false;
bool RUP_CLUSTERING_ANG = false;
bool RUP_CLUSTERING_RUP = false;

bool GROUPSEPARATION_IGNORE = false;
bool GROUPSEPARATION_GROUPCODE = false;
bool GROUPSEPARATION_KMEANS = false;
bool GROUPSEPARATION_RUPANG = false;

bool ROSETYPE_SYMMETRICAL = false;
bool ROSETYPE_ASYMMETRICAL = false;

bool ROSEDIRECTION_STRIKE = false;
bool ROSEDIRECTION_DIP = false;

bool ROSEBINSIZE_10_00 = false;
bool ROSEBINSIZE_2_50 = false;
bool ROSEBINSIZE_5_00 = false;
bool ROSEBINSIZE_22_50 = false;

bool IDEALMOVEMENT_NONE = false;
bool IDEALMOVEMENT_USE = false;

bool LABELLING_NONE = false;
bool LABELLING_USE = false;

double LINEWIDTH = 999.99;

bool COLOURING_IGNORE = false;
bool COLOURING_COLORCODE = false;
bool COLOURING_GROUPCODE = false;
bool COLOURING_KMEANS = false;
bool COLOURING_RUPANG = false;

bool GRAYSCALE_NONE = false;
bool GRAYSCALE_USE = false;
}


bool is_WELLDATA_USE () {

	return false; //WELLDATA_USE;
}

bool is_WELLDATA_NO () {

	return true; //WELLDATA_NO;
}
/*
bool is_WELL_INTERVAL_METER () {

	return WELL_INTERVAL_METER;
}
bool is_WELL_INTERVAL_DATANUMBER () {

	return WELL_INTERVAL_DATANUMBER;
}

double is_WELL_INTERVAL_LENGTH () {

	return WELL_INTERVAL_LENGTH;
}

bool is_WELL_INTERVAL_MIDDLE_MEDIAN () {

	return WELL_INTERVAL_MIDDLE_MEDIAN;
}
bool is_WELL_INTERVAL_MIDDLE_AVERAGE () {

	return WELL_INTERVAL_MIDDLE_AVERAGE;
}
*/
bool is_DATARULE_GERMAN () {

	return DATARULE_GERMAN;
}
bool is_DATARULE_RIGHT_HAND_RULE () {

	return DATARULE_RIGHT_HAND_RULE;
}

bool is_PLOT_ANGELIER () {

	return PLOT_ANGELIER;
}
bool is_PLOT_HOEPPENER () {

	return PLOT_HOEPPENER;
}

bool is_NET_SCHMIDT () {

	return NET_SCHMIDT;
}
bool is_NET_WULFF () {

	return NET_WULFF;
}

bool is_HEMISPHERE_LOWER () {

	return HEMISPHERE_LOWER;
}
bool is_HEMISPHERE_UPPER () {

	return HEMISPHERE_UPPER;
}

bool is_CONTOURING_NO () {

	return CONTOURING_NO;
}
bool is_CONTOURING_DIPDIR_BEARING () {

	return CONTOURING_DIPDIR_BEARING;
}
bool is_CONTOURING_STRIKEDIR_BEARING () {

	return CONTOURING_STRIKEDIR_BEARING;
}
bool is_CONTOURING_PLANE_NORMAL_BEARING () {

	return CONTOURING_PLANE_NORMAL_BEARING;
}
bool is_CONTOURING_STRIAE_BEARING_BEARING () {

	return CONTOURING_STRIAE_BEARING_BEARING;
}

bool is_TILTING_BEDDING_PALEONORTH () {

	return TILTING_BEDDING_PALEONORTH;
}
bool is_TILTING_BEDDING () {

	return TILTING_BEDDING;
}
bool is_TILTING_PALEONORTH () {

	return TILTING_PALEONORTH;
}

bool is_INVERSION_SPRANG () {

	return INVERSION_SPRANG;
}
bool is_INVERSION_TURNER () {

	return INVERSION_TURNER;
}
bool is_INVERSION_MICHAEL () {

	return INVERSION_MICHAEL;
}
bool is_INVERSION_SHAN () {

	return INVERSION_SHAN;
}
bool is_INVERSION_FRY () {

	return INVERSION_FRY;
}
bool is_INVERSION_ANGELIER () {

	return INVERSION_ANGELIER;
}
bool is_INVERSION_MOSTAFA () {

	return INVERSION_MOSTAFA;
}
bool is_INVERSION_BRUTEFORCE () {

	return INVERSION_BRUTEFORCE;
}
bool is_INVERSION_YAMAJI () {

	return INVERSION_YAMAJI;
}
bool is_INVERSION_NONE () {

	return INVERSION_NONE;
}

bool is_BINGHAM_USE () {

	return BINGHAM_USE;
}
bool is_BINGHAM_NONE () {

	return BINGHAM_NONE;
}

double is_STRESSANGLE () {

	return STRESSANGLE;
}

bool is_VIRTUAL_NONE () {

	return VIRTUAL_NONE;
}
bool is_VIRTUAL_USE () {

	return VIRTUAL_USE;
}

bool is_INPUTGROUP_NONE () {

	return INPUTGROUP_NONE;
}
bool is_INPUTGROUP_FIRST () {

	return INPUTGROUP_FIRST;
}
bool is_INPUTGROUP_SECOND () {

	return INPUTGROUP_SECOND;
}
bool is_INPUTGROUP_THIRD () {

	return INPUTGROUP_THIRD;
}

bool is_GROUPS_USE () {

	return GROUPS_USE;
}
bool is_GROUPS_NONE () {

	return GROUPS_NONE;
}

bool is_CLUSTERING_NONE () {

	return CLUSTERING_NONE;
}
bool is_CLUSTERING_AUTOMATIC () {

	return CLUSTERING_AUTOMATIC;
}
string is_CLUSTERNUMBER () {

	return CLUSTERNUMBER;
}

bool is_RUP_CLUSTERING_NONE () {

	return RUP_CLUSTERING_NONE;
}
bool is_RUP_CLUSTERING_ANG () {

	return RUP_CLUSTERING_ANG;
}
bool is_RUP_CLUSTERING_RUP () {

	return RUP_CLUSTERING_RUP;
}

bool is_GROUPSEPARATION_IGNORE () {

	return GROUPSEPARATION_IGNORE;
}
bool is_GROUPSEPARATION_GROUPCODE () {

	return GROUPSEPARATION_GROUPCODE;
}
bool is_GROUPSEPARATION_KMEANS () {

	return GROUPSEPARATION_KMEANS;
}
bool is_GROUPSEPARATION_RUPANG () {

	return GROUPSEPARATION_RUPANG;
}

bool is_ROSETYPE_SYMMETRICAL () {

	return ROSETYPE_SYMMETRICAL;
}
bool is_ROSETYPE_ASYMMETRICAL () {

	return ROSETYPE_ASYMMETRICAL;
}

bool is_ROSEDIRECTION_STRIKE () {

	return ROSEDIRECTION_STRIKE;
}
bool is_ROSEDIRECTION_DIP () {

	return ROSEDIRECTION_DIP;
}

bool is_ROSEBINSIZE_10_00 () {

	return ROSEBINSIZE_10_00;
}
bool is_ROSEBINSIZE_2_50 () {

	return ROSEBINSIZE_2_50;
}
bool is_ROSEBINSIZE_5_00 () {

	return ROSEBINSIZE_5_00;
}
bool is_ROSEBINSIZE_22_50 () {

	return ROSEBINSIZE_22_50;
}

bool is_IDEALMOVEMENT_NONE () {

	return IDEALMOVEMENT_NONE;
}
bool is_IDEALMOVEMENT_USE () {

	return IDEALMOVEMENT_USE;
}

bool is_LABELLING_NONE () {

	return LABELLING_NONE;
}
bool is_LABELLING_USE () {

	return LABELLING_USE;
}

double is_LINEWIDTH () {

	return LINEWIDTH;
}

bool is_COLOURING_IGNORE () {

	return COLOURING_IGNORE;
}
bool is_COLOURING_COLORCODE () {

	return COLOURING_COLORCODE;
}
bool is_COLOURING_GROUPCODE () {

	return COLOURING_GROUPCODE;
}
bool is_COLOURING_KMEANS () {

	return COLOURING_KMEANS;
}
bool is_COLOURING_RUPANG () {

	return COLOURING_RUPANG;
}

bool is_GRAYSCALE_NONE () {

	return GRAYSCALE_NONE;
}
bool is_GRAYSCALE_USE () {

	return GRAYSCALE_USE;
}

void INIT_SETTINGS (const vector <vector <string> >& SET) {

	for (size_t i = 0; i < SET.size(); i++) {

		const string KEY = SET.at(i).at(0);
		const string VAL = SET.at(i).at(1);

		//if (KEY == "WELLDATA:") {
			//if 		(VAL == "Y") WELLDATA_USE = true;
			//else if (VAL == "N")
			//WELLDATA_NO = true;
			//else ASSERT_DEAD_END();
		//}
		/*
		else if (KEY == "WELLINTERVAL:") {
			if 		(VAL == "M") WELL_INTERVAL_METER = true;
			else if (VAL == "D") WELL_INTERVAL_DATANUMBER = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "WELLINTERVAL_LENGTH:") {
			WELL_INTERVAL_LENGTH = string_to_double (VAL);
		}
		else if (KEY == "WELLINTERVAL_MIDDLE:") {
			if  	(VAL == "M") WELL_INTERVAL_MIDDLE_MEDIAN = true;
			else if (VAL == "A") WELL_INTERVAL_MIDDLE_AVERAGE = true;
			else ASSERT_DEAD_END();
		}
		*/
		//else
		if (KEY == "DATARULE:") {
			if 		(VAL == "G") DATARULE_GERMAN = true;
			else if (VAL == "R") DATARULE_RIGHT_HAND_RULE = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "PLOT:") {
			if 		(VAL == "A") PLOT_ANGELIER = true;
			else if (VAL == "H") PLOT_HOEPPENER = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "PLOTTYPE:") {
			if 		(VAL == "S")  NET_SCHMIDT = true;
			else if (VAL == "W")  NET_WULFF = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "HEMISPHERE:") {
			if 		(VAL == "L") HEMISPHERE_LOWER = true;
			else if (VAL == "U") HEMISPHERE_UPPER = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "CONTOURING:") {
			if 		(VAL == "N") CONTOURING_NO = true;
			else if (VAL == "D") CONTOURING_DIPDIR_BEARING = true;
			else if (VAL == "S") CONTOURING_STRIKEDIR_BEARING = true;
			else if (VAL == "O") CONTOURING_PLANE_NORMAL_BEARING = true;
			else if (VAL == "B") CONTOURING_STRIAE_BEARING_BEARING = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "TILTING:")	{
			if 		(VAL == "A") TILTING_BEDDING_PALEONORTH = true;
			else if (VAL == "B") TILTING_BEDDING = true;
			else if (VAL == "P") TILTING_PALEONORTH = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "INVERSION:") {
			if 		(VAL == "D") INVERSION_SPRANG = true;
			else if (VAL == "P") INVERSION_TURNER = true;
			else if (VAL == "M") INVERSION_MICHAEL = true;
			else if (VAL == "S") INVERSION_SHAN = true;
			else if (VAL == "F") INVERSION_FRY = true;
			else if (VAL == "A") INVERSION_ANGELIER = true;
			else if (VAL == "O") INVERSION_MOSTAFA = true;
			else if (VAL == "B") INVERSION_BRUTEFORCE = true;
			else if (VAL == "Y") INVERSION_YAMAJI = true;
			else if (VAL == "N") INVERSION_NONE = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "BINGHAM:")	{
			if 		(VAL == "B") BINGHAM_USE = true;
			else if (VAL == "N") BINGHAM_NONE = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "STRESSANGLE:")	{
			STRESSANGLE = string_to_double (VAL);
		}
		else if (KEY == "VIRTUAL:")	{
			if 		(VAL == "N") VIRTUAL_NONE = true;
			else if (VAL == "Y") VIRTUAL_USE = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "INPUTGROUP:") {

			if 		(VAL == "N") INPUTGROUP_NONE = true;
			else if (VAL == "F") INPUTGROUP_FIRST = true;
			else if (VAL == "S") INPUTGROUP_SECOND = true;
			else if (VAL == "T") INPUTGROUP_THIRD = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "GROUP:") {
			if 		(VAL == "Y") GROUPS_USE = true;
			else if (VAL == "N") GROUPS_NONE = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "CLUSTERNUMBER:") {

			if 		(VAL == "N") CLUSTERING_NONE = true;
			else if (VAL == "A") CLUSTERING_AUTOMATIC = true;
			else if (VAL == "2" || VAL == "3" || VAL == "4" || VAL == "5" || VAL == "6" || VAL == "7" || VAL == "8" || VAL == "9") {
				CLUSTERNUMBER = VAL;
			}
			else ASSERT_DEAD_END();
		}
		else if (KEY == "RUP_CLUSTERING:") {
			if 		(VAL == "N") RUP_CLUSTERING_NONE = true;
			else if (VAL == "A") RUP_CLUSTERING_ANG = true;
			else if (VAL == "R") RUP_CLUSTERING_RUP = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "GROUPSEPARATION:")	{
			if 		(VAL == "I")  GROUPSEPARATION_IGNORE = true;
			else if (VAL == "G")  GROUPSEPARATION_GROUPCODE = true;
			else if (VAL == "K")  GROUPSEPARATION_KMEANS = true;
			else if (VAL == "R")  GROUPSEPARATION_RUPANG = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "ROSETYPE:") {
			if 		(VAL == "S") ROSETYPE_SYMMETRICAL = true;
			else if (VAL == "A") ROSETYPE_ASYMMETRICAL = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "ROSEDIRECTION:") {
			if 		(VAL == "S") ROSEDIRECTION_STRIKE = true;
			else if (VAL == "D") ROSEDIRECTION_DIP = true;
			else ASSERT_DEAD_END();
		}
		else if (SET.at(i).at(0) == "ROSEBINNING:")	{

			if 		(VAL == "C") ROSEBINSIZE_10_00 = true;
			else if (VAL == "A") ROSEBINSIZE_2_50 = true;
			else if (VAL == "B") ROSEBINSIZE_5_00 = true;
			else if (VAL == "D") ROSEBINSIZE_22_50 = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "IDEALMOVEMENT:") {
			if 		(VAL == "N") IDEALMOVEMENT_NONE = true;
			else if (VAL == "Y") IDEALMOVEMENT_USE = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "LABELING:") {
			if 		(VAL == "N") LABELLING_NONE = true;
			else if (VAL == "Y") LABELLING_USE = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "LINEWIDTH:") {
			LINEWIDTH = string_to_double(VAL) / 10.0;
		}
		else if (KEY == "COLORING:") {
			if (VAL == "I") COLOURING_IGNORE = true;
			else if (VAL == "C") COLOURING_COLORCODE = true;
			else if (VAL == "G") COLOURING_GROUPCODE = true;
			else if (VAL == "K") COLOURING_KMEANS = true;
			else if (VAL == "R") COLOURING_RUPANG = true;
			else ASSERT_DEAD_END();
		}
		else if (KEY == "GRAYSCALE:") {
			if 		(VAL == "N") GRAYSCALE_NONE = true;
			else if (VAL == "Y") GRAYSCALE_USE = true;
			else ASSERT_DEAD_END();
		}
		else ASSERT_DEAD_END();
	}
}
