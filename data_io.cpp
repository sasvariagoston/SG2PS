// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iomanip>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "data_io.h"
#include "ps.h"
#include "rose.h"
#include "rgf.h"
#include "angelier.h"
#include "cluster.h"
#include "platform_dep.hpp"
#include "exceptions.hpp"

using namespace std;

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

	output.projectfolder 	= output.datetime + "_-_" + capslock(projectname);
	output.projectname 		= capslock(projectname);
	output.original			= output.projectfolder +  bs + "1_original";
	output.completed		= output.projectfolder +  bs + "2_completed";
	output.average			= output.projectfolder +  bs + "3_average";
	output.rgfsep			= output.projectfolder +  bs + "4_rgf_separated";
	output.pssep			= output.projectfolder +  bs + "5_ps_separated";

	return output;
}

bool createprojectfolders (PFN output, vector <GDB> inGDB) {

	const string bs = path_separator;
	int returncode = 0;

	returncode = system (("mkdir " + output.projectfolder).c_str());
	returncode = system (("mkdir " + output.original).c_str());
	returncode = system (("mkdir " + output.completed).c_str());
	returncode = system (("mkdir " + output.average).c_str());
	returncode = system (("mkdir " + output.rgfsep).c_str());
	returncode = system (("mkdir " + output.pssep).c_str());

	if (existence ("BOUDAIN", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "BOUDAIN").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "BOUDAIN").c_str());
	}

	if (existence ("CONTACT", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "CONTACT").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "CONTACT").c_str());
	}

	if (existence ("FOLDAXIS", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "FOLDAXIS").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "FOLDAXIS").c_str());
	}

	if (existence ("FOLDPLANE", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "FOLDPLANE").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "FOLDPLANE").c_str());
	}

	if (existence ("KINK", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "KINK").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "KINK").c_str());
	}

	if (existence ("LINEATION", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "LINEATION").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "LINEATION").c_str());
	}

	if (existence ("LITHOCLASE", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "LITHOCLASE").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "LITHOCLASE").c_str());
	}

	if (existence ("SC", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "SC").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "SC").c_str());
	}

	if (existence ("BEDDING", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "BEDDING").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "BEDDING").c_str());
	}

	if (existence ("S1", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "S1").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "S1").c_str());
	}

	if (existence ("S2", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "S2").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "S2").c_str());
	}

	if (existence ("S3", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "S3").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "S3").c_str());
	}

	if (existence ("S4", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "S4").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "S4").c_str());
	}

	if (existence ("S5", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "S5").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "S5").c_str());
	}

	if (existence ("FRACTURE", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "FRACTURE").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "FRACTURE").c_str());
	}

	if (existence ("STRIAE", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "STRIAE").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "STRIAE").c_str());
	}

	if (existence ("CROSSBEDDING", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "CROSSBEDDING").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "CROSSBEDDING").c_str());
	}

	if (existence ("VEIN", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "VEIN").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "VEIN").c_str());
	}

	if (existence ("FOLDSURFACE", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "FOLDSURFACE").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "FOLDSURFACE").c_str());
	}

	if (existence ("USERPLANE4", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "USERPLANE4").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "USERPLANE4").c_str());
	}

	if (existence ("USERPLANE5", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "USERPLANE5").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "USERPLANE5").c_str());
	}

	if (existence ("USERLINEATION1", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "USERLINEATION1").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "USERLINEATION1").c_str());
	}

	if (existence ("USERLINEATION2", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "USERLINEATION2").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "USERLINEATION2").c_str());
	}

	if (existence ("USERLINEATION3", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "USERLINEATION3").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "USERLINEATION3").c_str());
	}

	if (existence ("USERLINEATION4", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "USERLINEATION4").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "USERLINEATION4").c_str());
	}

	if (existence ("USERLINEATION5", inGDB)) {

		returncode = system (("mkdir " + output.rgfsep + bs + "USERLINEATION5").c_str());
		returncode = system (("mkdir " + output.pssep + bs + "USERLINEATION5").c_str());
	}

	if (returncode != 0) {

		cout << "Cannot create project folder." << endl;

		return false;
	}

	return true;
}

bool copyoriginalfile (PFN output) {

	ifstream inrgffile, insetfile,  inxyfile;
	ofstream outrgffile, outsetfile, outxyfile;

	const string bs = path_separator;
	string buffer;

	bool xy_file_exist = true;
	bool set_file_exist = true;

	string inrgffilename =  (output.projectname + ".rgf").c_str();
	string outrgffilename = (output.original + bs + output.projectname + ".rgf").c_str();

	string insetfilename =  (output.projectname + ".set").c_str();
	string outsetfilename = (output.original + bs + output.projectname + ".set").c_str();

	string inxyfilename =  (output.projectname + ".xy").c_str();
	string outxyfilename = (output.original + bs + output.projectname + ".xy").c_str();



	inrgffile.open (inrgffilename.c_str());
	outrgffile.open (outrgffilename.c_str());

	insetfile.open (insetfilename.c_str());
	outsetfile.open (outsetfilename.c_str());

	inxyfile.open (inxyfilename.c_str());
	outxyfile.open (outxyfilename.c_str());


	if (!(insetfile.is_open())) set_file_exist = false;

	if (!(inxyfile.is_open())) xy_file_exist = false;




	if (!(outxyfile.is_open())) 	cout << "  - ERROR: cannot create output XY file in project destination folder " + output.original + "." << endl;



	do {

		getline (inrgffile, buffer);
		if (inrgffile.eof ()) {

			outrgffile << buffer;
			break;
		}

		else {

			outrgffile << buffer << endl;
		}

	} while (!(inrgffile.eof()));



	buffer.clear();



	if (set_file_exist) {

		do {

			getline (insetfile, buffer);
			if (insetfile.eof ()) {

				outsetfile << buffer;
				break;
			}

			else {

				outsetfile << buffer << endl;
			}

		}	while (!(insetfile.eof()));

	}



	buffer.clear();



	if (xy_file_exist) {

		do {

			getline (inxyfile, buffer);
			if (inxyfile.eof ()) {

				outxyfile << buffer;
				break;
			}

			else {

				outxyfile << buffer << endl;
			}

		}	while (!(inxyfile.eof()));
	}



	inrgffile.close();
	outrgffile.close();

	insetfile.close();
	outsetfile.close();

	inxyfile.close();
	outxyfile.close();


	return true;
}

void outputrgfheader (ofstream& o, INPSET inset) {

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

void outputaverageheader (ofstream& o) {

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

	outputfilename = output.completed + bs + output.projectname + "_completed";
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
	string outputfilename = output.average + bs + output.projectname + "_average.rgf";

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

	if (processGDB.at(0).DATATYPE == "STRIAE") PS_stress_scale (output_ps_file, P);

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

void output_elapsed_time (double elapsed_time) {

	if (elapsed_time < 1 * 1000.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " milliseconds." << endl;
	else {
		elapsed_time = elapsed_time / 1000.0;
		if (elapsed_time < 1 * 60.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " seconds." << endl;
		else {
			elapsed_time = elapsed_time / 60.0;
			if (elapsed_time < 1 * 60.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " minutes." << endl;
			else {
				elapsed_time = elapsed_time / 60.0;
				if (elapsed_time < 1 * 60.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " hours." << endl;
				else cout << "  - Elapsed time: " << fixed << setprecision (1) << elapsed_time / 60.0 << " days." << endl;
			}
		}
	}
}
