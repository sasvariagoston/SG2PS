// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <string>
#include <ctime>

#include "common.h"
#include "rgf.h"
#include "data_io.h"
#include "checksettingfilecontent.h"
#include "checkrgffilecontent.h"
#include "checkxycontent.h"
#include "cluster.h"

using namespace std;

void real_main(int argument_number, char *argv[]) {

	string inputrgfname, xy_filename, inputrgfname_only, temp;
	vector <GDB> geodatabase, tiltgeodatabase;
	INPSET inset;
	size_t j = 1;
	bool batch = false;
	bool using_xy_files = false;
	vector <string> inputfilename_vector;

	header ();

	inputfilename_vector = create_inputfilename_vector (argument_number, argv);

	if (argument_number > 1) batch = true;

	else {

		inputrgfname = inputfilename();
		inputfilename_vector.push_back(inputrgfname);
	}

	inputfilename_vector = check_rgf_inputs (inputfilename_vector, batch);

	if (batch) using_xy_files = true;
	else using_xy_files = needxyfile ();

	if (!batch) manage_settings_nobatch (inputfilename_vector.at(1));

	clock_t starttime = clock ();

	do {

		inset = manage_settings_batch (inputfilename_vector.at(j));

		if (using_xy_files) xy_filename = check_xy_inputs (inputfilename_vector.at(j), batch);

		process_rgf (inputfilename_vector.at(j), xy_filename, inset);

		cout << "EVALUATION OF " << capslock(inputfilename_vector.at(j)) << ".RGF FILE COMPLETED." << endl;

		j++;

	} while (j < inputfilename_vector.size());

	clock_t finishtime = clock();

	double elapsed_time = (static_cast<double>(finishtime - starttime))/CLOCKS_PER_SEC;

	elapsed_time *= 1000; // FIXME Assumes it in ms

	output_elapsed_time (elapsed_time);
}


