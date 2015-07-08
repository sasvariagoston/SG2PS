/*
 * filename.hpp
 *
 *  Created on: 6 Jul 2015
 *      Author: sasvariagoston_admin
 */

#ifndef FILENAME_HPP_
#define FILENAME_HPP_

void generate_DATE_TIME (const string projectname);
string return_DATE_TIME ();

void generate_PROJECTNAME (string projectname);
string return_PROJECTNAME ();

void generate_PROJECT_FOLDER ();
string return_PROJECT_FOLDER ();

void generate_ORIGINAL_FOLDER ();
string return_ORIGINAL_FOLDER ();

void generate_COMPLETED_FOLDER ();
string return_COMPLETED_FOLDER ();

void generate_AVERAGE_FOLDER ();
string return_AVERAGE_FOLDER ();

void generate_RGF_FOLDER ();
string return_RGF_FOLDER ();

void generate_PS_FOLDER ();
string return_PS_FOLDER ();

void generate_WELL_PS_FOLDER ();
string return_WELL_PS_FOLDER ();

void setup_ACTUAL_DATATYPE (const string DT);
string return_ACTUAL_DATATYPE ();

void setup_ACTUAL_LOCATION (const string LOC);
string return_ACTUAL_LOCATION ();

void setup_ACTUAL_GROUPCODE (const string GC);
string return_ACTUAL_GROUPCODE ();

void setup_ACTUAL_FORMATION (const string FM);
string return_ACTUAL_FORMATION ();

void check_dataseparation_mode ();

string generate_ACTUAL_PS_NAME ();
string generate_ACTUAL_AVERAGE_RGF_NAME ();
string generate_ACTUAL_RGF_NAME ();
string generate_ACTUAL_COMPLETED_RGF_NAME ();
string generate_ACTUAL_WELL_PS_NAME ();
string generate_ACTUAL_RGF_NAME_FOR_STANDARD ();

void GENERATE_FOLDER_NAMES (const string projectname);

#endif /* FILENAME_HPP_ */
