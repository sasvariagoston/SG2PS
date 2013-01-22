// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "data_io.h"
#include "array_to_vector.hpp"
#include "platform_dep.hpp"
#include "ps.h"
#include "rgf.h"
#include "rose.h"
#include "run_mode.h"

PFN createprojectfoldernames (string projectname) {

	time_t current_time;
	struct tm * TM;
	const string bs = path_separator;

	time ( &current_time );
	TM = localtime ( &current_time );

	PFN output;

	output.datetime = int_to_string (TM->tm_year + 1900);

	if (TM->tm_mon < 9) output.datetime = output.datetime + "0";
	output.datetime = output.datetime + int_to_string (TM->tm_mon + 1);

	if (TM->tm_mday < 10) output.datetime = output.datetime + "0";
	output.datetime = output.datetime + int_to_string (TM->tm_mday) + "-";

	if (TM->tm_hour < 10) output.datetime = output.datetime + "0";
	output.datetime = output.datetime + int_to_string (TM->tm_hour);

	if (TM->tm_min < 10) output.datetime = output.datetime + "0";
	output.datetime = output.datetime + int_to_string (TM->tm_min);

	if (TM->tm_sec < 10) output.datetime = output.datetime + "0";
	output.datetime = output.datetime + int_to_string (TM->tm_sec);

	if (!is_DEBUG()) output.projectfolder 	= output.datetime + "_-_" + capslock(projectname);
	else output.projectfolder = capslock(projectname);

	output.projectname 		= projectname; // FIXME converting to uppercase is a bug on Linux
	output.original			= output.projectfolder +  bs + "1_original";
	output.completed		= output.projectfolder +  bs + "2_completed";
	output.average			= output.projectfolder +  bs + "3_average";
	output.rgfsep			= output.projectfolder +  bs + "4_rgf_separated";
	output.pssep			= output.projectfolder +  bs + "5_ps_separated";

	return output;
}

const string possible_folder_names[] = {
		"BOUDAIN",
		"CONTACT",
		"FOLDAXIS",
		"FOLDPLANE",
		"KINK",
		"LINEATION",
		"LITHOCLASE",
		"SC",
		"BEDDING",
		"S1",
		"S2",
		"S3",
		"S4",
		"S5",
		"FRACTURE",
		"STRIAE",
		"CROSSBEDDING",
		"VEIN",
		"FOLDSURFACE",
		"USERPLANE1",
		"USERPLANE2",
		"USERPLANE3",
		"USERPLANE4",
		"USERPLANE5",
		"USERLINEATION1",
		"USERLINEATION2",
		"USERLINEATION3",
		"USERLINEATION4",
		"USERLINEATION5"
};

const vector<string> possible_folders = from_array(possible_folder_names);

void make_dir(const string& dir_name) {

	string command = "mkdir "+dir_name;

	int ret = system(command.c_str());

	if (ret) {

		throw runtime_error("failed to execute command \""+command+"\"");
	}
}

void create_folders(const PFN& output, const string& dir) {

	make_dir(output.rgfsep + path_separator + dir);
	make_dir(output.pssep  + path_separator + dir);
}

void createprojectfolders (PFN output, vector <GDB> inGDB) {

	make_dir( output.projectfolder );
	make_dir( output.original      );
	make_dir( output.completed     );
	make_dir( output.average       );
	make_dir( output.rgfsep        );
	make_dir( output.pssep         );

	for (size_t i=0; i<possible_folders.size(); ++i) {

		const string& dir = possible_folders.at(i);

		if (existence(dir, inGDB)) {

			create_folders(output, dir);
		}
	}
}

namespace {

struct copy_status { enum code { OK, OPENING_INPUT_FAILED, FAILED }; };

copy_status::code copy_file(const string& source, const string& destination) {

	ifstream in(source.c_str(), ios::binary);

	if (!in.is_open()) {

		return copy_status::OPENING_INPUT_FAILED;
	}

	ofstream out(destination.c_str(), ios::binary);

	out << in.rdbuf();

	return (in.fail() || out.fail()) ? copy_status::FAILED : copy_status::OK;
};

void dispatch_on_error_code(copy_status::code status, const string& src_name, bool should_exist);

void back_up_file(string extension, const string& project_name, const string& new_path, bool should_exist = false) {

	string src  = project_name+extension;

	dispatch_on_error_code(copy_file(src, new_path+src), src, should_exist);
}

void dispatch_on_error_code(copy_status::code status, const string& src_name, bool should_exist) {

	const string EXTENSION = to_uppercase(src_name.substr(src_name.size()-3));

	if      (status == copy_status::OK) {

		cout << "  - Original input "+EXTENSION+" file has been successfully copied to the project folder." << endl;
	}
	else if (status == copy_status::OPENING_INPUT_FAILED && should_exist) {

		throw runtime_error("opening "+src_name+" failed");
	}
	else if (status == copy_status::FAILED) {

		cout << "  - Critical error: failed to copy input "+EXTENSION+" file to the project folder." << endl;

		throw runtime_error("copying "+src_name+" failed");
	}
}

}

void copyoriginalfiles (PFN output) {

	const string project_name = output.projectname;

	const string new_path = output.original + path_separator;

	const bool SHOULD_EXIST = true;

	back_up_file(".rgf", project_name, new_path, SHOULD_EXIST);

	back_up_file(".set", project_name, new_path); // TODO In batch and debug mode, failures were ignored
	                                              // TODO In batch mode, shouldn't it dump a set file?
	back_up_file(".xy", project_name, new_path);
}

void copy_log(const PFN& names) {

	if (!is_GUI()) {

		return;
	}

	cout.flush();

	string log_file = "log.txt";

	copy_file(log_file, names.original + path_separator + log_file);
}

void outputrgfheader (ofstream& o, INPSET inset) { // FIXME Duplication? Same as reserved_column_names?

	o
	<< "DATA_ID" << '\t'
	<< "GROUP_CODE" << '\t'
	<< "COLOR_CODE" << '\t'
	<< "LOCATION" << '\t'
	<< "LOC_X" << '\t'
	<< "LOC_Y" << '\t'
	<< "FORMATION" << '\t'
	<< "DATATYPE" << '\t'
	<< "DIP_DIR" << '\t'
	<< "DIP" << '\t'
	<< "L_DIP_DIR" << '\t'
	<< "L_DIP" << '\t'
	<< "SENSE" << '\t'
	<< "PALEONORTH"	<< '\t'
	<< "COMMENT" << endl;
}

void outputaverageheader (ofstream& o) { // TODO What is the extension? .rgf? How about .csv?

	o
	<< "DATA_ID" << '\t'
	<< "GC" << '\t'
	<< "COLOR" << '\t'
	<< "LOC" << '\t'
	<< "LOCX" << '\t'
	<< "LOCY" << '\t'
	<< "FORMATION" << '\t'
	<< "DATATYPE" << '\t'
	<< "averageDIPDIR" << '\t'
	<< "averageDIP" << '\t'
	<< "NODATA" << '\t'
	<< "NODATA" << '\t'
	<< "NODATA" << '\t'
	<< "NODATA" << '\t'
	<< "COMMENT" << endl;
}

void outputrecord (GDB i, ofstream& o, INPSET inpset) {

	o
	<< i.ID << '\t'
	<< i.GC << '\t'
	<< i.COLOR << '\t'
	<< i.LOC << '\t'
	<< fixed << setprecision (6) << i.LOCX << '\t'
	<< fixed << setprecision (6) << i.LOCY << '\t'
	<< i.FORMATION << '\t'
	<< i.DATATYPE << '\t' << flush;

	o << fixed << setprecision (1) << flush;

	if ((i.corr.DIPDIR > 361.0) || (i.DATATYPE == "LITHOLOGY")) o << "" << '\t' << flush;
	else {

		if (inpset.datarule == "R" ) 	o << german_to_right_hand_rule (i.corr.DIPDIR) << '\t' << flush;
		else							o << i.corr.DIPDIR << '\t' << flush;
	}

	if ((i.corr.DIP > 360.0) || (i.DATATYPE == "LITHOLOGY")) o << "" << '\t' << flush;
	else o << fixed << setprecision (1) << i.corr.DIP << '\t' << flush;


	if (i.corrL.DIPDIR > 360.0) o << "" << '\t' << flush;
	else o << i.corrL.DIPDIR << '\t' << flush;


	if (i.corrL.DIP > 360.0) o << "" << '\t' << flush;
	else o << i.corrL.DIP << '\t' << flush;

	if ((i.DATATYPE == "STRIAE") || (i.DATATYPE == "BEDDING")) {

		if (((i.DATATYPE == "BEDDING")) && (i.corrOFFSET == "NONE")) o << '\t' << flush;
		else o << i.corrOFFSET << '\t' << flush;
	}

	else o << "" << '\t' << flush;

	o
	<< i.PALEON	<< '\t'
	<< i.COMMENT<<

	flush;
}

void outputveragerecord (GDB i, ofstream& o) {

	o
	<< i.ID << '\t'
	<< i.GC << '\t'
	<< i.COLOR << '\t'
	<< i.LOC << '\t'
	<< setprecision (6) << i.LOCX << '\t'
	<< setprecision (6) << i.LOCY << '\t'
	<< i.FORMATION << '\t'
	<< i.DATATYPE << '\t' << flush;

	if (i.avd.DIPDIR > 361.0) o << "" << '\t' << flush;
	else o << i.avd.DIPDIR << '\t' << flush;

	if (i.avd.DIP > 361.0) o << "" << '\t' << flush;
	else o << i.avd.DIP << '\t' << flush;

	o << "" << '\t' << flush;
	o << "" << '\t' << flush;
	o << "" << '\t' << flush;
	o << "" << '\t' << flush;
	o << i.COMMENT<< '\t' << flush;
}

void outputresultrgf (PFN output, vector <GDB> outGDB, bool tilted, INPSET inset) {

	ofstream outputfile;
	string outputfilename;
	string bs = path_separator;

	size_t i = 0;

	outputfilename = output.completed + bs + capslock(output.projectname) + "_completed";
	if (tilted) outputfilename = outputfilename + "_tilted";
	outputfilename = outputfilename + ".rgf";

	outputfile.open (outputfilename.c_str());

	outputrgfheader (outputfile, inset);

	while (i < outGDB.size()) {

		outputrecord (outGDB.at(i), outputfile, inset);

		if (i < outGDB.size() - 1) outputfile << endl;

		i++;
	}

	outputfile.close();

	if (tilted) 	cout << "  - Tilted RGF file exported." <<  endl;
	else 			cout << "  - Completed RGF file exported." <<  endl;
}

void outputaveragergf (PFN output, vector <GDB> outGDB) {

	ofstream outputfile;
	string bs = path_separator;
	string outputfilename = output.average + bs + capslock(output.projectname) + "_average.rgf";

	size_t i = 0;
	size_t independentrecordcounter = 0;

	outputfile.open (outputfilename.c_str());

	outputaverageheader (outputfile);

	if ((outGDB.size() == 1) && (!((outGDB.at(0).DATATYPE == "STRIAE") || (outGDB.at(0).DATATYPE == "SC")))) {

		outputveragerecord (outGDB.at(0), outputfile);
		outputfile.close();
		cout << "  - Average RGF output completed." <<  endl;
	}

	do {

		do {

			i++;
			if (i == outGDB.size()) break;
		}

		while ((outGDB.at(i-1).DATATYPE == outGDB.at(i).DATATYPE) && (outGDB.at(i-1).LOC == outGDB.at(i).LOC));

		independentrecordcounter++;

		if (!((outGDB.at(i-1).DATATYPE == "STRIAE") || (outGDB.at(i-1).DATATYPE == "SC"))) {

			outputveragerecord (outGDB.at(i - 1), outputfile);
		}

		if (i < outGDB.size()) 	outputfile << endl;
	}

	while (i < outGDB.size());

	outputfile.close();

	cout << "  - Average RGF output completed." <<  endl;
}

void outputselected_ps_rgf (PFN output, vector <GDB> outGDB, vector <GDB> tiltoutGDB, INPSET inset) {

	vector <GDB> processGDB, tiltprocessGDB;
	size_t i = 0;
	size_t independentrecordcounter = 0;
	CENTER center;
	PAPER P = PS_dimensions ();

	center.X = P.O1X;
	center.Y = P.O1Y;
	center.radius = P.R;

	do {

		processGDB.clear();
		tiltprocessGDB.clear();

		do {

			processGDB.push_back(outGDB.at(i));
			tiltprocessGDB.push_back(tiltoutGDB.at(i));

			i++;

			if (i == outGDB.size()) break;

		} while (stopcriteria (outGDB.at(i-1).DATATYPE, outGDB.at(i).DATATYPE, outGDB.at(i-1).LOC, outGDB.at(i).LOC, outGDB.at(i-1).GC, outGDB.at(i).GC, inset));

		independentrecordcounter++;

		if (processGDB.at(0).DATATYPE != "LITHOLOGY") {

			if (inset.group == "N") {

				if (existence_of_groupcodes (processGDB)) {



					processGDB = 		colorcode_grom_groupcode (processGDB);
					tiltprocessGDB = 	colorcode_grom_groupcode (tiltprocessGDB);
				}
			}

			else {

				processGDB = 		black_colorcode (processGDB);
				tiltprocessGDB = 	black_colorcode (tiltprocessGDB);
			}

			output_to_rgf (output, processGDB, inset, false);

			output_to_rgf (output, tiltprocessGDB, inset, true);

			output_to_ps (output, processGDB, tiltprocessGDB, inset, P, center);
		}

	} while (i < outGDB.size());

	cout << "DATA EXPORT FROM '" << capslock(output.projectname) << ".RGF' DATABASE FILE" << endl;
	cout << "  - Postscript output completed for " << independentrecordcounter << " file containing " << i << " records." <<  endl;
	cout << "  - RGF output completed for " << independentrecordcounter << " file containing " << i << " records." <<  endl;
	cout << "  - Tilted RGF output completed for " << independentrecordcounter << " file containing " << i << " records." <<  endl;
}

void output_to_rgf (PFN output, vector <GDB> processGDB, INPSET inset, bool tilted) {

	ofstream output_rgf_file;
	string output_rgf_filename;
	string bs = path_separator;
	size_t j = 0;

	if (inset.group == "Y") output_rgf_filename = output.rgfsep + bs + processGDB.at(0).DATATYPE + bs + processGDB.at(0).LOC + "_" + processGDB.at(0).DATATYPE + "_" + processGDB.at(0).GC;
	else 					output_rgf_filename = output.rgfsep + bs + processGDB.at(0).DATATYPE + bs + processGDB.at(0).LOC + "_" + processGDB.at(0).DATATYPE;

	if (tilted) output_rgf_filename = output_rgf_filename + "_tilted";
	else output_rgf_filename = output_rgf_filename + "_original";

	output_rgf_filename = output_rgf_filename + ".rgf";

	output_rgf_file.open (output_rgf_filename.c_str());

	sort(processGDB.begin(), processGDB.end(), byiID);

	outputrgfheader (output_rgf_file, inset);

	do {

		outputrecord (processGDB.at(j), output_rgf_file, inset);
		if (j < processGDB.size()-1) output_rgf_file << endl;
		j++;

	} while (j < processGDB.size());

	output_rgf_file.close();
}

void output_to_ps (PFN output, vector <GDB> processGDB, vector <GDB> tiltprocessGDB, INPSET inset, PAPER P, CENTER center) {

	string output_ps_filename;
	string bs = path_separator;

	if (inset.group == "Y") output_ps_filename = output.pssep +  bs + processGDB.at(0).DATATYPE + bs + processGDB.at(0).LOC + "_" + processGDB.at(0).DATATYPE + "_" + processGDB.at(0).GC + ".eps";
	else 					output_ps_filename = output.pssep +  bs + processGDB.at(0).DATATYPE + bs + processGDB.at(0).LOC + "_" + processGDB.at(0).DATATYPE + ".eps";

	ofstream output_ps_file(output_ps_filename.c_str());

	PS_header (processGDB.at(0).DATATYPE, processGDB.at(0).LOC, output_ps_file, P);
	PS_SYMBOLS(processGDB, output_ps_file, inset, P);

	if (processGDB.at(0).DATATYPE == "STRIAE" && (inset.inversion != "N")) PS_stress_scale (output_ps_file, P);


	PS_border (processGDB.at(0), output_ps_file, inset, P);

	for (size_t j=0; j<processGDB.size(); ++j) {

		process_one_by_one (processGDB.at(j), tiltprocessGDB.at(j), output_ps_file, inset, center, P);
		if (j < processGDB.size()-1) output_ps_file << endl;
	}

	process_group_by_group (processGDB, tiltprocessGDB, output_ps_file, inset, center, P);

	PS_datanumber_averagebedding (processGDB.at(0), output_ps_file, inset, P, center, processGDB.size());

	PS_net (processGDB.at(0).DATATYPE, processGDB.at(0).LOC, output_ps_file, inset, P);
}

void process_group_by_group (vector <GDB> outGDB, vector <GDB> tiltoutGDB, ofstream& o, INPSET inset, CENTER center, PAPER P) {

	CENTER mohr_center;

	PS_draw_rose (outGDB, tiltoutGDB, o, inset, center, P);

	if ((inset.fracture == "B") && (outGDB.at(0).DATATYPE == "FRACTURE") && (outGDB.size() < 2)) return;
	if ((inset.fracture == "B") && (outGDB.at(0).DATATYPE == "FRACTURE") && (tiltoutGDB.size() < 2)) return;

	if ((inset.fracture == "B") && (outGDB.at(0).DATATYPE == "FRACTURE")) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << "fracture statistics after Bingham (1964): " << endl;

		if (outGDB.size() > 1) {

			cout << "    - Original : " << flush;
			center.X = P.O1X;
			center.Y = P.O1Y;
			outGDB = inversion ("BINGHAM", outGDB, o, inset, center, mohr_center, P);

			cout << "    - Corrected: " << flush;
			center.X = P.O2X;
			center.Y = P.O2Y;
			tiltoutGDB = inversion ("BINGHAM", tiltoutGDB, o, inset, center, mohr_center, P);
		}

		else cout << "less data than required to the statistics." << endl;
	}

	else if ((inset.inversion == "F") && ((outGDB.at(0).DATATYPE == "STRIAE"))) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << "regression after Fry (1999): " << endl;

		if (outGDB.size() > 5) {

			cout << "    - Original : " << flush;
			center.X = P.O1X;
			center.Y = P.O1Y;
			mohr_center.X = P.O7X;
			mohr_center.Y = P.O7Y;
			outGDB = inversion ("FRY", outGDB, o, inset, center, mohr_center, P);

			cout << "    - Corrected: " << flush;
			center.X = P.O2X;
			center.Y = P.O2Y;
			mohr_center.X = P.O8X;
			mohr_center.Y = P.O8Y;
			tiltoutGDB = inversion ("FRY", tiltoutGDB, o, inset, center, mohr_center, P);
		}

		else cout << "less data than required to the statistics." << endl;
	}

	else if ((inset.inversion == "M") && ((outGDB.at(0).DATATYPE == "STRIAE"))) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << "regression after Michael (1984): " << endl;

		if (outGDB.size() > 4) {

			cout << "    - Original : " << flush;
			center.X = P.O1X;
			center.Y = P.O1Y;
			mohr_center.X = P.O7X;
			mohr_center.Y = P.O7Y;
			outGDB = inversion ("MICHAEL", outGDB, o, inset, center, mohr_center, P);

			cout << "    - Corrected: " << flush;
			center.X = P.O2X;
			center.Y = P.O2Y;
			mohr_center.X = P.O8X;
			mohr_center.Y = P.O8Y;
			tiltoutGDB = inversion ("MICHAEL", tiltoutGDB, o, inset, center, mohr_center, P);
		}

		else cout << "less data than required to the statistics." << endl;
	}

	else if ((inset.inversion == "S") && ((outGDB.at(0).DATATYPE == "STRIAE"))) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << "regression after Shan et al. (2003): " << endl;

		if (outGDB.size() > 4) {

			cout << "    - Original : " << flush;
			center.X = P.O1X;
			center.Y = P.O1Y;
			mohr_center.X = P.O7X;
			mohr_center.Y = P.O7Y;
			outGDB = inversion ("SHAN", outGDB, o, inset, center, mohr_center, P);

			cout << "    - Corrected: " << flush;
			center.X = P.O2X;
			center.Y = P.O2Y;
			mohr_center.X = P.O8X;
			mohr_center.Y = P.O8Y;
			tiltoutGDB = inversion ("SHAN", tiltoutGDB, o, inset, center, mohr_center, P);
		}

		else cout << "less data than required to the statistics." << endl;
	}

	else if ((inset.inversion == "A") && ((outGDB.at(0).DATATYPE == "STRIAE"))) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << "inversion after Angelier (1990): " << endl;

		if (outGDB.size() > 3) {

			cout << "    - Original : " << flush;
			center.X = P.O1X;
			center.Y = P.O1Y;
			mohr_center.X = P.O7X;
			mohr_center.Y = P.O7Y;
			outGDB = inversion ("ANGELIER", outGDB, o, inset, center, mohr_center, P);

			cout << "    - Corrected: " << flush;
			center.X = P.O2X;
			center.Y = P.O2Y;
			mohr_center.X = P.O8X;
			mohr_center.Y = P.O8Y;
			tiltoutGDB = inversion ("ANGELIER", tiltoutGDB, o, inset, center, mohr_center, P);
		}

		else cout << "less data than required to the statistics." << endl;
	}

	else if ((inset.inversion == "O") && ((outGDB.at(0).DATATYPE == "STRIAE"))) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << "inversion after Mostafa (2005): " << endl;

		if (outGDB.size() > 3) {

			cout << "    - Original : " << flush;
			center.X = P.O1X;
			center.Y = P.O1Y;
			mohr_center.X = P.O7X;
			mohr_center.Y = P.O7Y;
			outGDB = inversion ("MOSTAFA", outGDB, o, inset, center, mohr_center, P);

			cout << "    - Corrected: " << flush;
			center.X = P.O2X;
			center.Y = P.O2Y;
			mohr_center.X = P.O8X;
			mohr_center.Y = P.O8Y;
			tiltoutGDB = inversion ("MOSTAFA", tiltoutGDB, o, inset, center, mohr_center, P);
		}

		else cout << "less data than required to the statistics." << endl;
	}

	else if ((inset.inversion == "D") && ((outGDB.at(0).DATATYPE == "STRIAE"))) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << "regression after Sprang (1972): " << endl;

		cout << "    - Original : " << flush;
		center.X = P.O1X;
		center.Y = P.O1Y;
		mohr_center.X = P.O7X;
		mohr_center.Y = P.O7Y;
		outGDB = inversion ("NDA", outGDB, o, inset, center, mohr_center, P);

		cout << "    - Corrected: " << flush;
		center.X = P.O2X;
		center.Y = P.O2Y;
		mohr_center.X = P.O8X;
		mohr_center.Y = P.O8Y;
		tiltoutGDB = inversion ("NDA", tiltoutGDB, o, inset, center, mohr_center, P);
	}

	else if ((inset.inversion == "P") && ((outGDB.at(0).DATATYPE == "STRIAE"))) {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << " regression after Turner (1953): " << endl;

		cout << "    - Original : " << flush;
		center.X = P.O1X;
		center.Y = P.O1Y;
		mohr_center.X = P.O7X;
		mohr_center.Y = P.O7Y;
		outGDB = inversion ("PTN", outGDB, o, inset, center, mohr_center, P);

		cout << "    - Corrected: " << flush;
		center.X = P.O2X;
		center.Y = P.O2Y;
		mohr_center.X = P.O8X;
		mohr_center.Y = P.O8Y;
		tiltoutGDB = inversion ("PTN", tiltoutGDB, o, inset, center, mohr_center, P);
	}

	else {}

	if  (outGDB.at(0).DATATYPE == "FOLDSURFACE") {

		cout << "  - For '" << outGDB.at(0).LOC << "' location" << flush;
		if (inset.group == "Y")	cout << ", '"<< outGDB.at(0).GC << "', " << flush;
		else 					cout << "," << flush;
		cout << " fold axis calculation: " << endl;

		if (outGDB.size() > 1) {

			cout << "    - Original : " << flush;
			center.X = P.O1X;
			center.Y = P.O1Y;
			fold_from_planes (outGDB, o, inset, center, P);

			cout << "    - Corrected: " << flush;
			center.X = P.O2X;
			center.Y = P.O2Y;
			fold_from_planes (tiltoutGDB, o, inset, center, P);
		}

		else cout << "less data than required to the statistics." << endl;
	}

}

void process_one_by_one (GDB processGDB, GDB tiltprocessGDB, ofstream& o, INPSET inset, CENTER center, PAPER P) {


	center.X = P.O1X;
	center.Y = P.O1Y;

	PS_DRAW_record (processGDB, o, inset, center);

	center.X = P.O2X;
	center.Y = P.O2Y;
	PS_DRAW_record (tiltprocessGDB, o, inset, center);
}
