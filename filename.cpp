// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <ctime>
#include <iostream>

#include "assertions.hpp"
#include "platform_dep.hpp"
#include "rgf.h"
#include "run_mode.h"
#include "settings.hpp"

namespace {

string PROJECTNAME = "";
string DATE_TIME = "";
string PROJECT_FOLDER = "";
string ORIGINAL_FOLDER = "";
string COMPLETED_FOLDER = "";
string AVERAGE_FOLDER = "";
string RGF_FOLDER = "";
string PS_FOLDER = "";
string WELL_PS_FOLDER = "";

string ACTUAL_DATATYPE = "";
string ACTUAL_LOCATION = "";
string ACTUAL_GROUPCODE = "";
string ACTUAL_FORMATION = "";

const string BS = path_separator;
const string US = "_";

}

void generate_DATE_TIME () {

	time_t current_time;
	struct tm * TM;

	time ( &current_time );
	TM = localtime ( &current_time );

	string DT = int_to_string (TM->tm_year + 1900);

	if (TM->tm_mon < 9) DT = DT + "0";
	DT = DT + int_to_string (TM->tm_mon + 1);

	if (TM->tm_mday < 10) DT = DT + "0";
	DT = DT + int_to_string (TM->tm_mday) + "-";

	if (TM->tm_hour < 10) DT = DT + "0";
	DT = DT + int_to_string (TM->tm_hour);

	if (TM->tm_min < 10) DT = DT + "0";
	DT = DT + int_to_string (TM->tm_min);

	if (TM->tm_sec < 10) DT = DT + "0";
	DT = DT + int_to_string (TM->tm_sec);

	DATE_TIME = DT;

	return;
}

string return_DATE_TIME () {

	return DATE_TIME;
}

void generate_PROJECTNAME (const string projectname) {

	PROJECTNAME = projectname;
}

string return_PROJECTNAME () {

	return PROJECTNAME;
}

void generate_PROJECT_FOLDER () {

	const string DT = return_DATE_TIME();
	const string PN = capslock (PROJECTNAME);

	string PF = "";

	if (!is_mode_DEBUG()) PF = DT + US + PN;
	else PF = PN;

	PROJECT_FOLDER = PF;

	return;
}

string return_PROJECT_FOLDER () {

	return PROJECT_FOLDER;
}

void generate_ORIGINAL_FOLDER () {

	ORIGINAL_FOLDER = PROJECT_FOLDER + BS + "1_ORIGINAL";
}

string return_ORIGINAL_FOLDER () {

	return ORIGINAL_FOLDER;
}

void generate_COMPLETED_FOLDER () {

	COMPLETED_FOLDER = PROJECT_FOLDER + BS + "2_COMPLETED";
}

string return_COMPLETED_FOLDER () {

	return COMPLETED_FOLDER;
}

void generate_AVERAGE_FOLDER () {

	AVERAGE_FOLDER = PROJECT_FOLDER + BS + "3_AVERAGE";
}

string return_AVERAGE_FOLDER () {

	return AVERAGE_FOLDER;
}

void generate_RGF_FOLDER () {

	RGF_FOLDER = PROJECT_FOLDER + BS + "4_RGF_SEPARATED";
}

string return_RGF_FOLDER () {

	return RGF_FOLDER;
}

void generate_PS_FOLDER () {

	PS_FOLDER = PROJECT_FOLDER + BS + "5_PS_SEPARATED";
}

string return_PS_FOLDER () {

	return PS_FOLDER;
}

void generate_WELL_PS_FOLDER () {

	WELL_PS_FOLDER = PROJECT_FOLDER + BS + "6_WELL_PS_SEPARATED";
}

string return_WELL_PS_FOLDER () {

	return WELL_PS_FOLDER;
}

void setup_ACTUAL_DATATYPE (const string DT) {

	ACTUAL_DATATYPE = DT;
}

string return_ACTUAL_DATATYPE () {

	return ACTUAL_DATATYPE;
}

void setup_ACTUAL_LOCATION (const string LOC) {

	ACTUAL_LOCATION = LOC;
}

string return_ACTUAL_LOCATION () {

	return ACTUAL_LOCATION;
}

void setup_ACTUAL_GROUPCODE (const string GC) {

	ACTUAL_GROUPCODE = GC;
}

string return_ACTUAL_GROUPCODE () {

	return ACTUAL_GROUPCODE;
}

void setup_ACTUAL_FORMATION (const string FM) {

	ACTUAL_FORMATION = FM;
}

string return_ACTUAL_FORMATION () {

	return ACTUAL_FORMATION;
}

void check_dataseparation_mode () {

	const bool IGNORE = is_GROUPSEPARATION_IGNORE ();
	const bool by_GROUPCODE = is_GROUPSEPARATION_GROUPCODE ();
	const bool by_KMEANS = is_GROUPSEPARATION_KMEANS ();
	const bool by_RUPANG = is_GROUPSEPARATION_RUPANG ();

	ASSERT_EXACTLY_ONE_TRUE (IGNORE, by_GROUPCODE, by_KMEANS, by_RUPANG);
}

string generate_ACTUAL_PS_NAME () {

	const string PS = return_PS_FOLDER();

	const string DT = return_ACTUAL_DATATYPE();
	const string LOC = return_ACTUAL_LOCATION();
	const string GC = return_ACTUAL_GROUPCODE();

	string PS_NAME = PS + BS + DT + BS + LOC + US + DT;

	if (is_PROCESS_AS_TRAJECTORY()) PS_NAME = PS_NAME + "_TRAJECTORY_CORRECTED";

	check_dataseparation_mode ();

	if (is_GROUPSEPARATION_GROUPCODE ())	PS_NAME = PS_NAME + US + GC.at(0);
	if (is_GROUPSEPARATION_KMEANS ())		PS_NAME = PS_NAME + US + GC.at(1);
	if (is_GROUPSEPARATION_RUPANG ())		PS_NAME = PS_NAME + US + GC.at(2);

	return PS_NAME + ".eps";
}

string generate_ACTUAL_AVERAGE_RGF_NAME () {

	string FN = AVERAGE_FOLDER + BS + capslock (PROJECTNAME);

	if (is_PROCESS_AS_TILTED()) FN = FN + "_TILTED";

	if (is_PROCESS_AS_TRAJECTORY()) FN = FN + "_TRAJECTORY_CORRECTED";

	return FN + "_AVERAGE.rgf";
}

string generate_ACTUAL_RGF_NAME () {

	check_dataseparation_mode ();

	const string PS = return_PS_FOLDER();

	const string DT = return_ACTUAL_DATATYPE();
	const string LOC = return_ACTUAL_LOCATION();
	const string GC = return_ACTUAL_GROUPCODE();

	string FN = RGF_FOLDER + BS + DT + BS + LOC + US + DT;

	ASSERT (!(is_GROUPSEPARATION_GROUPCODE ()	&& GC.size() < 1));
	ASSERT (!(is_GROUPSEPARATION_KMEANS () 		&& GC.size() < 2));
	ASSERT (!(is_GROUPSEPARATION_RUPANG () 		&& GC.size() < 3));

	if (is_GROUPSEPARATION_GROUPCODE ()) 	FN = FN + US + GC.at(0);
	if (is_GROUPSEPARATION_KMEANS ()) 		FN = FN + US + GC.at(1);
	if (is_GROUPSEPARATION_RUPANG ()) 		FN = FN + US + GC.at(2);

	if (is_PROCESS_AS_TILTED()) FN = FN + "_TILTED";

	if (is_PROCESS_AS_TRAJECTORY()) FN = FN + "_TRAJECTORY_CORRECTED";

	return FN + ".rgf";
}

string generate_ACTUAL_COMPLETED_RGF_NAME () {

	string FN = COMPLETED_FOLDER + BS + PROJECTNAME + "_COMPLETED";

	if (is_PROCESS_AS_TILTED()) FN = FN + "_TILTED";

	if (is_PROCESS_AS_TRAJECTORY()) FN = FN + "_TRAJECTORY_CORRECTED";

	return FN + ".rgf";
}

string generate_ACTUAL_WELL_PS_NAME () {

	check_dataseparation_mode();

	const string LOC = return_ACTUAL_LOCATION ();
	const string DT = return_ACTUAL_DATATYPE ();
	const string FM = return_ACTUAL_FORMATION ();
	const string GC = return_ACTUAL_GROUPCODE ();

	string PS_NAME = WELL_PS_FOLDER + BS + DT + BS + LOC;

	PS_NAME = PS_NAME + US + DT;

	if (is_GROUPSEPARATION_GROUPCODE ())	PS_NAME = PS_NAME + US + GC.at(0);
	if (is_GROUPSEPARATION_KMEANS ()) 		PS_NAME = PS_NAME + US + GC.at(1);
	if (is_GROUPSEPARATION_RUPANG ()) 		PS_NAME = PS_NAME + US + GC.at(2);

	if (is_PROCESS_AS_TILTED()) 	PS_NAME = PS_NAME + "_TILTED";
	if (is_PROCESS_AS_TRAJECTORY()) PS_NAME = PS_NAME + "_TRAJECTORY_CORRECTED";

	return PS_NAME + ".eps";
}

string generate_ACTUAL_RGF_NAME_FOR_STANDARD () {

	check_dataseparation_mode();

	const string LOC = return_ACTUAL_LOCATION ();
	const string DT = return_ACTUAL_DATATYPE ();
	const string FM = return_ACTUAL_FORMATION ();
	const string GC = return_ACTUAL_GROUPCODE ();

	string PS_NAME = LOC;

	PS_NAME = PS_NAME + US + DT;

	if (is_GROUPSEPARATION_GROUPCODE ())	PS_NAME = PS_NAME + US + GC.at(0);
	if (is_GROUPSEPARATION_KMEANS ()) 		PS_NAME = PS_NAME + US + GC.at(1);
	if (is_GROUPSEPARATION_RUPANG ()) 		PS_NAME = PS_NAME + US + GC.at(2);

	if (is_PROCESS_AS_TILTED()) 	PS_NAME = PS_NAME + "_TILTED";
	if (is_PROCESS_AS_TRAJECTORY()) PS_NAME = PS_NAME + "_TRAJECTORY_CORRECTED";

	return PS_NAME + ".CSV";
}

void GENERATE_FOLDER_NAMES (const string projectname) {

	generate_DATE_TIME ();
	generate_PROJECTNAME ( projectname);

	generate_PROJECT_FOLDER ();
	generate_ORIGINAL_FOLDER ();
	generate_COMPLETED_FOLDER ();
	generate_AVERAGE_FOLDER ();
	generate_RGF_FOLDER ();
	generate_PS_FOLDER ();
	generate_WELL_PS_FOLDER ();
}
