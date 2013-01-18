// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <string>
#include <ctime>
#include <map>
#include <stdexcept>
#include <sstream>
#include <iomanip>

#include "common.h"
#include "rgf.h"
#include "data_io.h"
#include "checksettingfilecontent.h"
#include "checkrgffilecontent.h"
#include "checkxycontent.h"
#include "cluster.h"
#include "platform_dep.hpp"
#include "run_mode.h"

using namespace std;

namespace {

string run_mode = "COMMANDLINE";

}

bool is_GUI() {

  return (run_mode == "GUI");
}

bool is_BATCH() {

  return (run_mode == "BATCH");
}

bool is_COMMANDLINE() {

  return (run_mode == "COMMANDLINE");
}

bool is_DEBUG() {

  return (run_mode == "DEBUG");
}

void print_banner();

void real_main(int argument_number, char *argv[]) {

	string inputrgfname, xy_filename, inputrgfname_only, temp;
	vector <GDB> geodatabase, tiltgeodatabase;
	INPSET inset;
	size_t j = 0;
	bool using_xy_files = false;
	vector <string> inputfilename_vector;

	inputfilename_vector = create_inputfilename_vector (argument_number, argv);

	inputfilename_vector.erase(inputfilename_vector.begin() + 0);

	if (inputfilename_vector.size() >= 1) {

		if (inputfilename_vector.at(0) == "-gui_calls") run_mode = "GUI";

		else if (inputfilename_vector.at(0) == "-debug") run_mode = "DEBUG";

		else run_mode = "BATCH";
	}

	if (!is_DEBUG()) print_banner();

	cout << "RUNNING SG2PS IN " << run_mode << " MODE." << endl;

	if (is_GUI() || is_DEBUG()) inputfilename_vector.erase(inputfilename_vector.begin() + 0);

	if (is_COMMANDLINE()) {

		inputrgfname = inputfilename();
		inputfilename_vector.push_back(inputrgfname);
	}

	inputfilename_vector = check_rgf_inputs (inputfilename_vector);

	if (!is_COMMANDLINE()) using_xy_files = true;
	else using_xy_files = needxyfile ();

	if (is_COMMANDLINE()) manage_settings_nobatch (inputfilename_vector.at(0));

	clock_t starttime = clock ();

	do {

		inset = manage_settings_batch (inputfilename_vector.at(j));

		if (using_xy_files) xy_filename = check_xy_inputs (inputfilename_vector.at(j));

		process_rgf (inputfilename_vector.at(j), xy_filename, inset);

		cout << "EVALUATION OF " << capslock(inputfilename_vector.at(j)) << ".RGF FILE COMPLETED." << endl;

		j++;

	} while (j < inputfilename_vector.size());

	clock_t finishtime = clock();

	double elapsed_time = (static_cast<double>(finishtime - starttime))/CLOCKS_PER_SEC;

	elapsed_time *= 1000;

	if (!is_DEBUG()) output_elapsed_time (elapsed_time);
}


void print_banner () {

	cout << endl << endl;

	cout << " ------------------------------------------------------------------ " << endl;
	cout << "|                                                                  |" << endl;
	cout << "|                             SG2PS                                |" << endl;
	cout << "|       Structural Geological data to PostScript converter         |" << endl;
	cout << "|                                                                  |" << endl;
	cout << "|        Data processing software coded by Agoston Sasvari         |" << endl;
	cout << "|                                                                  |" << endl;
	cout << "|            For further information check www.sg2ps.eu            |" << endl;
	cout << "|                                                                  |" << endl;
	cout << "|                       All rights reserved.                       |" << endl;
	cout << "|              This a free software, licence: GNU LGPL.            |" << endl;
	cout << "|                                                                  |" << endl;
	cout << " ------------------------------------------------------------------ " << endl;
	cout << "|                                                                  |" << endl;
	cout << "|                 Built on: " << return_build_date() << ", " << return_build_time() << "                  | "<< endl;
	cout << "|                                                                  |" << endl;
	cout << "|                       version: 0.2.0 beta                        |" << endl;
	cout << "|                                                                  |" << endl;
	cout << " ------------------------------------------------------------------ " << endl << endl;

	// TODO Why the version number?
}

