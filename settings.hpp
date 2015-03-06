
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

bool is_WELLDATA_USE ();
bool is_WELLDATA_NO ();

//bool is_WELL_INTERVAL_METER ();
//bool is_WELL_INTERVAL_DATANUMBER ();

//double is_WELL_INTERVAL_LENGTH ();

//bool is_WELL_INTERVAL_MIDDLE_MEDIAN ();
//bool is_WELL_INTERVAL_MIDDLE_AVERAGE ();

bool is_DATARULE_GERMAN ();
bool is_DATARULE_RIGHT_HAND_RULE ();

bool is_PLOT_ANGELIER ();
bool is_PLOT_HOEPPENER ();

bool is_NET_SCHMIDT ();
bool is_NET_WULFF ();

bool is_HEMISPHERE_LOWER ();
bool is_HEMISPHERE_UPPER ();

bool is_CONTOURING_NO ();
bool is_CONTOURING_DIPDIR_BEARING ();
bool is_CONTOURING_STRIKEDIR_BEARING ();
bool is_CONTOURING_PLANE_NORMAL_BEARING ();
bool is_CONTOURING_STRIAE_BEARING_BEARING ();

bool is_AVCALC_OUTCROP ();
bool is_AVCALC_OUTCROP_GROUP ();

bool is_TILTING_BEDDING_PALEONORTH ();
bool is_TILTING_BEDDING ();
bool is_TILTING_PALEONORTH ();

bool is_INVERSION_SPRANG ();
bool is_INVERSION_TURNER ();
bool is_INVERSION_MICHAEL ();
bool is_INVERSION_SHAN ();
bool is_INVERSION_FRY ();
bool is_INVERSION_ANGELIER ();
bool is_INVERSION_MOSTAFA ();
bool is_INVERSION_BRUTEFORCE ();
bool is_INVERSION_YAMAJI ();
bool is_INVERSION_NONE ();

bool is_BINGHAM_USE ();
bool is_BINGHAM_NONE ();

double is_STRESSANGLE ();

bool is_VIRTUAL_NONE ();
bool is_VIRTUAL_USE ();

bool is_INPUTGROUP_NONE ();
bool is_INPUTGROUP_FIRST ();
bool is_INPUTGROUP_SECOND ();
bool is_INPUTGROUP_THIRD ();

bool is_GROUPS_USE ();
bool is_GROUPS_NONE ();

bool is_CLUSTERING_NONE ();
bool is_CLUSTERING_AUTOMATIC ();
string is_CLUSTERNUMBER ();

bool is_RUP_CLUSTERING_NONE ();
bool is_RUP_CLUSTERING_ANG ();
bool is_RUP_CLUSTERING_RUP ();

bool is_GROUPSEPARATION_IGNORE ();
bool is_GROUPSEPARATION_GROUPCODE ();
bool is_GROUPSEPARATION_KMEANS ();
bool is_GROUPSEPARATION_RUPANG ();

bool is_ROSETYPE_SYMMETRICAL ();
bool is_ROSETYPE_ASYMMETRICAL ();

bool is_ROSEDIRECTION_STRIKE ();
bool is_ROSEDIRECTION_DIP ();

bool is_ROSEBINSIZE_10_00 ();
bool is_ROSEBINSIZE_2_50 ();
bool is_ROSEBINSIZE_5_00 ();
bool is_ROSEBINSIZE_22_50 ();

bool is_IDEALMOVEMENT_NONE ();
bool is_IDEALMOVEMENT_USE ();

bool is_LABELLING_NONE ();
bool is_LABELLING_USE ();

double is_LINEWIDTH ();

bool is_COLOURING_IGNORE ();
bool is_COLOURING_COLORCODE ();
bool is_COLOURING_GROUPCODE ();
bool is_COLOURING_KMEANS ();
bool is_COLOURING_RUPANG ();

bool is_GRAYSCALE_NONE ();
bool is_GRAYSCALE_USE ();

void INIT_SETTINGS (const vector <vector <string> >& SET);

#endif
