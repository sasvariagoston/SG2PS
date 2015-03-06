
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "allowed_keys.hpp"
#include "array_to_vector.hpp"
#include "assertions.hpp"
#include "color_management.hpp"
#include "data_io.h"
#include "density.h"
#include "foldsurface.hpp"
#include "homogenity_check.hpp"
#include "kaalsbeek.hpp"
#include "platform_dep.hpp"
#include "ps.h"
#include "ReservedColumnNames.hpp"
#include "rgf.h"
#include "rose.h"
#include "run_mode.h"
#include "rup_clustering.hpp"
#include "settings.hpp"
#include "structs.h"

PFN create_project_folder_names (const string projectname) {

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

	if (!is_mode_DEBUG()) output.projectfolder 	= output.datetime + "_-_" + capslock(projectname);
	else output.projectfolder = capslock(projectname);

	output.projectname 		= projectname; // Converting to uppercase is a bug on Linux
	output.original			= output.projectfolder +  bs + "1_original";
	output.completed		= output.projectfolder +  bs + "2_completed";
	output.average			= output.projectfolder +  bs + "3_average";
	output.rgfsep			= output.projectfolder +  bs + "4_rgf_separated";
	output.pssep			= output.projectfolder +  bs + "5_ps_separated";

	if (is_WELLDATA_USE()) {

		output.well_ps = output.projectfolder +  bs + "6_ps_welldata";
	}

	return output;
}

vector <string> possible_folders_name () {

	vector <string> OUT = allowed_lineation_datatype_vector();

	OUT = merge_datatypes(OUT, allowed_plane_datatype_vector());
	OUT = merge_datatypes(OUT, allowed_striae_datatype_vector());
	OUT = merge_datatypes(OUT, allowed_SC_datatype_vector());

	return OUT;
}

void make_dir (const string& dir_name) {

	string command = "mkdir " + dir_name;

	int ret = system(command.c_str());

	if (ret) {

		throw runtime_error("failed to execute command \""+command+"\"");
	}
}

void create_folders (const PFN& output, const string& dir) {

	make_dir(output.rgfsep + path_separator + dir);
	make_dir(output.pssep  + path_separator + dir);
	if (is_WELLDATA_USE()) make_dir(output.well_ps  + path_separator + dir);
}

void createprojectfolders (const PFN& output, const vector <GDB>& inGDB) {

	make_dir( output.projectfolder);
	make_dir( output.original);
	make_dir( output.completed);
	make_dir( output.average);
	make_dir( output.rgfsep);
	make_dir( output.pssep);
	if (is_WELLDATA_USE()) make_dir( output.well_ps);

	vector <string> possible_folders = possible_folders_name ();

	for (size_t i = 0; i < possible_folders.size(); ++i) {

		const string& dir = possible_folders.at(i);

		if (existence(dir, inGDB)) create_folders(output, dir);
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
}

void dispatch_on_error_code(copy_status::code status, const string& src_name, bool should_exist);

void back_up_file(string extension, const string& project_name, const string& new_path, bool should_exist = false) {

	string src  = project_name+extension;

	dispatch_on_error_code(copy_file(src, new_path+src), src, should_exist);
}

void dispatch_on_error_code(copy_status::code status, const string& src_name, bool should_exist) {

	if (is_mode_DEBUG()) return;

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

void copyoriginalfiles (const PFN& output) {

	const string project_name = output.projectname;

	const string new_path = output.original + path_separator;

	const bool SHOULD_EXIST = true;

	back_up_file(".rgf", project_name, new_path, SHOULD_EXIST);

	back_up_file(".set", project_name, new_path); // TODO In batch and debug mode, failures were ignored
	                                              // TODO In batch mode, shouldn't it dump a set file?
	back_up_file(".xy", project_name, new_path);
}

void copy_log(const PFN& names) {

	if (is_mode_GUI()) {

		cout.flush();

		string log_file = "log.txt";

		copy_file(log_file, names.original + path_separator + log_file);
	}
}

void create_pointer_to_project_folder(const PFN& names) {

	if (is_mode_GUI()) {

		ofstream pointer_to("project_folder_name");

		pointer_to << names.projectfolder << '\n';
	}
}

void CREATE_PROJECT_FOLDER (const PFN& projectfoldername, const vector <GDB> inGDB) {

	createprojectfolders (projectfoldername, inGDB); // TODO Failures in creating project folders silently ignored

	create_pointer_to_project_folder(projectfoldername);

	copyoriginalfiles (projectfoldername);
}

void output_rgf_header (ofstream& o, const bool AVERAGE) {

	vector <string> CN = reserved_column_names();

	for (size_t i = 0; i < CN.size(); i++) {

		const bool EMPTY_RECORD = ((i >= 8) && (i <= 13));

		if (AVERAGE && EMPTY_RECORD) o << "" << '\t' << flush;
		else o << CN.at(i) << '\t' << flush;
	}
	o << endl;

	return;
}

void output_rgf_record (const GDB& i, ofstream& o, const bool AVERAGE) {

	o
	<< i.ID << '\t'
	<< i.GC << '\t'
	<< i.COLOR << '\t'
	<< i.LOC << '\t'
	<< fixed << setprecision (6)
	<< i.LOCX << '\t'
	<< i.LOCY << '\t'
	<< i.FORMATION << '\t'
	<< i.DATATYPE << '\t';

	if (AVERAGE) {

		if (!is_allowed_DIR (i.avd.DIPDIR)) o << "" << '\t';
		else o << i.avd.DIPDIR << '\t';

		if (!is_allowed_DIP(i.avd.DIP)) o << "" << '\t';
		else o << i.avd.DIP << '\t';

		o << "" << '\t';
		o << "" << '\t';
		o << "" << '\t';
		o << "" << '\t';
	}
	else {

		o << fixed << setprecision (1);

		if (!is_allowed_DIR(i.corr.DIPDIR) || i.DATATYPE == "LITHOLOGY") o << "" << '\t';
		else {

			if (is_DATARULE_RIGHT_HAND_RULE() ) o << german_to_right_hand_rule (i.corr.DIPDIR) << '\t';
			else o << i.corr.DIPDIR << '\t';
		}

		if (!is_allowed_DIP(i.corr.DIP) || i.DATATYPE == "LITHOLOGY") o << "" << '\t';
		else o << fixed << setprecision (1) << i.corr.DIP << '\t';


		if (!is_allowed_DIR(i.corrL.DIPDIR)) o << "" << '\t';
		else o << i.corrL.DIPDIR << '\t';

		if (!is_allowed_DIP(i.corrL.DIP)) o << "" << '\t';
		else o << i.corrL.DIP << '\t';

		const bool is_STRIAE = 	is_allowed_striae_datatype(i.DATATYPE);
		const bool is_BEDDING = 	is_allowed_foldsurface_processing(i.DATATYPE);

		if (is_STRIAE || is_BEDDING) {

			if (is_BEDDING && is_allowed_striae_none_sense(i.OFFSET)) o << '\t';
			else o << i.OFFSET << '\t';
		}
		else o << "" << '\t';

		o
		<< i.PALEON	<< '\t';
	}
	o << i.COMMENT << '\n';

	return;
}

void OUTPUT_COMPLETED_TO_RGF (const vector <GDB>& inGDB, const PFN& P, const bool TILT) {

	const string bs = path_separator;
	string FN = P.completed + bs + capslock(P.projectname) + "_completed";

	if (TILT) FN = FN + "_tilted";

	FN = FN + ".rgf";

	ofstream O;
	O.open (FN.c_str());

	output_rgf_header (O, false);

	OUTPUT_GDB_to_RGF (O, inGDB, false);

	O.close();
}

void OUTPUT_AVERAGE_TO_RGF (const vector <vector <GDB> >& inGDB_G, const PFN& P, const bool TILT) {

	const string bs = path_separator;
	string FN = P.average + bs + capslock(P.projectname);

	if (TILT) FN = FN + "_tilted";

	FN = FN + "_average.rgf";

	ofstream O;
	O.open (FN.c_str());

	output_rgf_header (O, true);

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		vector <GDB> temp;

		temp.push_back (inGDB_G.at(i).at(0));

		const bool STRIAE = is_allowed_striae_datatype(temp.at(0).DATATYPE);
		const bool SC = is_allowed_SC_datatype(temp.at(0).DATATYPE);

		if (!STRIAE && !SC) OUTPUT_GDB_to_RGF (O, temp, true);
	}
	O.close();

	return;
}

void OUTPUT_GROUPS_TO_RGF (const vector <vector <GDB> >& inGDB_G, const PFN& P, const bool TILTED) {

	const string bs = path_separator;

	const bool by_GROUPCODE = is_GROUPSEPARATION_GROUPCODE ();
	const bool by_KMEANS = is_GROUPSEPARATION_KMEANS ();
	const bool by_RUPANG = is_GROUPSEPARATION_RUPANG ();
	const bool IGNORE = is_GROUPSEPARATION_IGNORE ();

	if (!by_GROUPCODE && !by_KMEANS && !by_RUPANG && !IGNORE) ASSERT_DEAD_END();

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		vector <GDB> T = inGDB_G.at(i);

		const string DT =  T.at(0).DATATYPE;
		const string LOC = T.at(0).LOC;
		const string GC =  T.at(0).GC;

		string FN = P.rgfsep + bs + DT + bs + LOC + "_" + DT;

		if (by_GROUPCODE && GC.size() < 1) ASSERT_DEAD_END();
		if (by_KMEANS && GC.size() < 2) ASSERT_DEAD_END();
		if (by_RUPANG && GC.size() < 3) ASSERT_DEAD_END();

		if (by_GROUPCODE) FN = FN + "_" + GC.at(0);
		else if (by_KMEANS) FN = FN + "_" + GC.at(1);
		else if (by_RUPANG) FN = FN + "_" + GC.at(2);
		else if (is_GROUPSEPARATION_IGNORE()) {}
		else ASSERT_DEAD_END();

		if (TILTED) FN = FN + "_tilted";

		FN = FN + ".rgf";

		ofstream O;

		O.open (FN.c_str());

		output_rgf_header (O, false);

		sort(T.begin(), T.end(), byiID);

		OUTPUT_GDB_to_RGF (O, T, false);

		O.close();
	}
	return;
}

void OUTPUT_GDB_to_RGF (ofstream& O, const vector <GDB>& inGDB, const bool AVERAGE) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		output_rgf_record (inGDB.at(i), O, AVERAGE);
	}
	return;
}

void OUTPUT_TO_RGF (const vector <vector <vector <vector <GDB> > > >& inGDB_G, const PFN& projectfoldername, const bool TILT) {

	vector <GDB> processGDB = MERGE_GROUPS_TO_GDB (inGDB_G);

	OUTPUT_COMPLETED_TO_RGF (processGDB, projectfoldername, TILT);

	vector <vector <GDB> > processGDB_G = MERGE_GROUPS_TO_GDB_G (inGDB_G);

	OUTPUT_AVERAGE_TO_RGF (processGDB_G, projectfoldername, TILT);

	OUTPUT_GROUPS_TO_RGF (processGDB_G, projectfoldername, TILT);

	return;
}

vector < vector < vector <vector <GDB> > > > SEPARATE_DATASET_TO_GROUPS (const vector <GDB>& inGDB, const string METHOD) {

	const bool GROUPS = METHOD == "GROUPS";
	const bool CLUSTER = METHOD == "CLUSTER";
	const bool RUP = METHOD == "RUP";
	const bool NONE = METHOD == "NONE";

	if (!GROUPS && !CLUSTER && !RUP && !NONE) ASSERT_DEAD_END();

	vector < vector < vector <vector <GDB> > > > outGDB_G;

	vector <GDB> processGDB = inGDB;

	vector <GDB> buf1;
	vector <vector <GDB> > buf2;
	vector < vector <vector <GDB> > > buf3;

	sort (processGDB.begin(), processGDB.end(), byLocTypeGc);

	if (processGDB.size() == 0) {

		ASSERT_DEAD_END();
	}
	else if (processGDB.size() == 1) {

		buf2.push_back (processGDB);
		buf3.push_back (buf2);
		outGDB_G.push_back (buf3);
	}
	else {

		GDB dummy;

		dummy.LOC = "XXXX";
		dummy.GC = "XXXX";
		dummy.DATATYPE = "XXXX";

		processGDB.push_back(dummy);

		for (size_t i = 1; i < processGDB.size(); i++) {

			buf1.push_back(processGDB.at(i-1));

			const string p_DT = processGDB.at(i-1).DATATYPE;
			const string a_DT = processGDB.at(i-0).DATATYPE;
			const string p_LC = processGDB.at(i-1).LOC;
			const string a_LC = processGDB.at(i-0).LOC;

			string p_GC = "";
			string a_GC = "";

			if (GROUPS && !is_GROUPS_NONE()) {

				if (processGDB.at(i-1).GC.size() < 1) ASSERT_DEAD_END();
				if (processGDB.at(i-0).GC.size() < 1) ASSERT_DEAD_END();
				p_GC = processGDB.at(i-1).GC.at(0);
				a_GC = processGDB.at(i-0).GC.at(0);
			}
			else if (CLUSTER && !is_CLUSTERING_NONE()) {

				if (processGDB.at(i-1).GC.size() < 2) ASSERT_DEAD_END();
				if (processGDB.at(i-0).GC.size() < 2) ASSERT_DEAD_END();
				p_GC = processGDB.at(i-1).GC.at(1);
				a_GC = processGDB.at(i-0).GC.at(1);
			}
			else if (RUP && !is_RUP_CLUSTERING_NONE()) {

				if (processGDB.at(i-1).GC.size() < 3) ASSERT_DEAD_END();
				if (processGDB.at(i-0).GC.size() < 3) ASSERT_DEAD_END();
				p_GC = processGDB.at(i-1).GC.at(2);
				a_GC = processGDB.at(i-0).GC.at(2);
			}
			else {

				string p_GC = processGDB.at(i-1).GC;
				string a_GC = processGDB.at(i-0).GC;
			}

			const bool EQ_DT = a_DT == p_DT;
			const bool EQ_GC = a_GC == p_GC;
			const bool EQ_LC = a_LC == p_LC;

			const bool STOP_because_LC = !EQ_LC;

			bool STOP_because_GC;

			if (GROUPS || CLUSTER || RUP) 	STOP_because_GC = STOP_because_LC || !EQ_GC;
			else 							STOP_because_GC = STOP_because_LC;

			bool STOP_because_DT = STOP_because_GC || !EQ_DT;

			if (STOP_because_DT) {

				buf2.push_back(buf1);
				buf1.clear();
			}

			if (STOP_because_GC) {

				buf3.push_back(buf2);
				buf2.clear();
			}

			if (STOP_because_LC) {

				outGDB_G.push_back (buf3);
				buf3.clear();
			}
		}
	}
	return outGDB_G;
}

vector <GDB> MERGE_GROUPS_TO_GDB (const vector < vector <vector <vector <GDB> > > >& GDB_G) {

	vector <GDB> outGDB;

	for (size_t i = 0; i < GDB_G.size(); i++) {
		for (size_t j = 0; j < GDB_G.at(i).size(); j++) {
			for (size_t k = 0; k < GDB_G.at(i).at(j).size(); k++) {

				outGDB = merge_GDB (GDB_G.at(i).at(j).at(k), outGDB);
			}
		}
	}
	sort (outGDB.begin(), outGDB.end(), byiID);

	return outGDB;
}

vector <vector <GDB> > MERGE_GROUPS_TO_GDB_G (const vector < vector <vector <vector <GDB> > > >& GDB_G) {

	vector <vector <GDB> > outGDB_G;

	for (size_t i = 0; i < GDB_G.size(); i++) {
		for (size_t j = 0; j < GDB_G.at(i).size(); j++) {
			for (size_t k = 0; k < GDB_G.at(i).at(j).size(); k++) {

				outGDB_G.push_back(GDB_G.at(i).at(j).at(k));
			}
		}
	}
	return outGDB_G;
}

vector <GDB> combine_inversion_for_none_offset (const vector <GDB>& process_GDB, const vector <GDB>& hasoffset_GDB) {

	vector <GDB> OUT = process_GDB;

	for (size_t i = 0; i < OUT.size(); i++) {
		for (size_t j = 0; j < hasoffset_GDB.size(); j++) {

			const string P_ID = OUT.at(i).ID;
			const string H_ID = hasoffset_GDB.at(j).ID;

			if (P_ID == H_ID) OUT.at(i) = hasoffset_GDB.at(j);
		}
	}
	return OUT;
}

CENTER return_center (const PAPER P, const bool MOHR, const bool TILT) {

	CENTER center;

	if (MOHR) {
		if (TILT) {
			center.X = P.O8X;
			center.Y = P.O8Y;
		}
		else {
			center.X = P.O7X;
			center.Y = P.O7Y;
		}
	}
	else {
		if (TILT) {
			center.X = P.O2X;
			center.Y = P.O2Y;
		}
		else {
			center.X = P.O1X;
			center.Y = P.O1Y;
		}
	}
	return center;
}

vector <vector <vector <vector <GDB> > > > PROCESS_GROUPS (const vector <vector <vector <vector <GDB> > > >& inGDB_G, const bool TILT) {

	vector <vector <vector <vector <GDB> > > > outGDB_G = inGDB_G;

	outGDB_G = associate_empty_clustercode (outGDB_G, 2);

	for (size_t i = 0; i < outGDB_G.size(); i++) {
		for (size_t j = 0; j < outGDB_G.at(i).size(); j++) {
			for (size_t k = 0; k < outGDB_G.at(i).at(j).size(); k++) {

				vector <GDB> process_GDB = outGDB_G.at(i).at(j).at(k);

				const string DT = process_GDB.at(0).DATATYPE;
				const bool IS_STRIAE = is_allowed_striae_datatype(DT);
				const bool FOLD_TO_PROCESS = is_allowed_foldsurface_processing(DT);

				const bool FRACTURE_TO_PROCESS = is_BINGHAM_USE() && DT == "FRACTURE";
				const bool STRIAE_TO_PROCESS = !is_INVERSION_NONE() && IS_STRIAE;

				const bool ENOUGH_HOMOGENEOUS = has_inhomogeneous_enough (process_GDB);

				if (FRACTURE_TO_PROCESS || STRIAE_TO_PROCESS) {

					vector <GDB> hasoffset_GDB = process_GDB;

					if (IS_STRIAE) hasoffset_GDB = return_striae_with_offset (process_GDB);//was testGDB

					const bool ENOUGH_STRIAE = hasoffset_GDB.size() >= minimum_independent_dataset ();

					if (ENOUGH_HOMOGENEOUS && ENOUGH_STRIAE) {

						if (!TILT) cout_method_text (hasoffset_GDB);

						cout_original_tilted_text (TILT);

						if (is_VIRTUAL_USE()) hasoffset_GDB = generate_virtual_striae (hasoffset_GDB);

						INVERSION (hasoffset_GDB);

						const vector <STRESSTENSOR> STV = return_STV ();
						const vector <STRESSFIELD> SFV = return_SFV ();

						if (SFV.size() != STV.size()) ASSERT_DEAD_END();

						hasoffset_GDB = ASSOCIATE_STV_SFV (hasoffset_GDB, STV, SFV);

						const bool SUCCESSFULL = (STV.size() > 0 && SFV.size() > 0);

						if (SUCCESSFULL && STRIAE_TO_PROCESS) {

							const size_t MX = STV.size() - 1;

							hasoffset_GDB = apply_inversion_result (hasoffset_GDB, STV.at(MX));

							hasoffset_GDB = apply_RUP_ANG_CLUSTERING_result (hasoffset_GDB);
						}

						process_GDB = combine_inversion_for_none_offset (process_GDB, hasoffset_GDB);

						cout_inversion_results (process_GDB, SFV);
					}
					else {

						cout_less_than_required_text (TILT);
					}
				}
				else if (FOLD_TO_PROCESS) {

					process_GDB = CALCULATE_FOLDSURFACE_NORMAL (process_GDB);
				}
				else {}

				outGDB_G.at(i).at(j).at(k) = process_GDB;
			}
		}
	}
	return outGDB_G;
}

void OUTPUT_TO_PS (const vector <vector <vector <vector <GDB> > > > n_GDB_G, const vector <vector <vector <vector <GDB> > > > t_GDB_G, const PFN P) {

	const vector < vector <GDB> > n_prGDB_G = MERGE_GROUPS_TO_GDB_G (n_GDB_G);
	const vector < vector <GDB> > t_prGDB_G = MERGE_GROUPS_TO_GDB_G (t_GDB_G);

	const bool IGNORE = is_GROUPSEPARATION_IGNORE ();
	const bool by_GROUPCODE = is_GROUPSEPARATION_GROUPCODE ();
	const bool by_KMEANS = is_GROUPSEPARATION_KMEANS ();
	const bool by_RUPANG = is_GROUPSEPARATION_RUPANG ();

	if (!IGNORE && !by_GROUPCODE && !by_KMEANS && !by_RUPANG) ASSERT_DEAD_END() ;

	const string BS = path_separator;
	const string US = "_";

	for (size_t i = 0; i < n_prGDB_G.size(); i++) {

		const string LOC = n_prGDB_G.at(i).at(0).LOC;
		const string DT = n_prGDB_G.at(i).at(0).DATATYPE;

		const bool LITHOLOGY = is_allowed_lithology_datatype (DT);

		if (!LITHOLOGY) {

			string PS_NAME = P.pssep + BS + DT + BS + LOC + US + DT;

			if (by_GROUPCODE) 	PS_NAME = PS_NAME + US + n_prGDB_G.at(i).at(0).GC.at(0);
			else if (by_KMEANS) PS_NAME = PS_NAME + US + n_prGDB_G.at(i).at(0).GC.at(1);
			else if (by_RUPANG) PS_NAME = PS_NAME + US + n_prGDB_G.at(i).at(0).GC.at(2);
			else {}

			PS_NAME = PS_NAME + ".eps";

			ofstream OPS (PS_NAME.c_str());

			PS_stereonet_header (DT, LOC, OPS);

			const PAPER PPR = PS_dimensions (false);

			PS_STEREONET_SYMBOLS (n_prGDB_G.at(i), OPS, PPR);

			if (is_allowed_striae_datatype(DT) && ! is_INVERSION_NONE()) PS_stress_scale (OPS, PPR);

			PS_border (n_prGDB_G.at(i), OPS, PPR);

			PS_GDB (n_prGDB_G.at(i), OPS, PPR, false);
			PS_GDB (t_prGDB_G.at(i), OPS, PPR, true);

			PS_datanumber_averagebedding (n_prGDB_G.at(i).at(0), OPS, PPR, n_prGDB_G.at(i).size());

			PS_net (OPS, PPR);
		}
	}
	return;
}

void cout_method_text (const vector <GDB>& inGDB) {

	if (is_mode_DEBUG()) return;

	const string LOC = inGDB.at(0).LOC;
	const string GC = inGDB.at(0).GC;

	const bool GROUPS = is_GROUPS_USE();
	const bool CLUSTER = !is_CLUSTERING_NONE();
	const bool RUP = !is_RUP_CLUSTERING_NONE();
	const bool IS_STRIAE = is_allowed_striae_datatype (inGDB.at(0).DATATYPE);

	string OUT = "  - For '" + LOC + "' location";

	if (GROUPS || CLUSTER || RUP) OUT = OUT + ", " + GC;

	OUT = OUT + ", ";

	string M = "";

	if (is_INVERSION_ANGELIER() && IS_STRIAE)			M = "inversion after Angelier (1990): ";
	else if (is_BINGHAM_USE() && !IS_STRIAE) 			M = "fracture statistics after Bingham (1964): ";
	else if (is_INVERSION_BRUTEFORCE() && IS_STRIAE)	M = "brute force inversion: ";
	else if (is_INVERSION_FRY() && IS_STRIAE)			M = "regression after Fry (1999): ";
	else if (is_INVERSION_MICHAEL() && IS_STRIAE)		M = "regression after Michael (1984): ";
	else if (is_INVERSION_MOSTAFA() && IS_STRIAE) 		M = "inversion after Mostafa (2005): ";
	else if (is_INVERSION_SPRANG() && IS_STRIAE)		M = "regression after Sprang (1972): ";
	else if (is_INVERSION_TURNER() && IS_STRIAE) 		M = "regression after Turner (1953): ";
	else if (is_INVERSION_SHAN() && IS_STRIAE)			M = "regression after Shan et al. (2003): ";
	else if (is_INVERSION_YAMAJI() && IS_STRIAE)		M = "iteration after Yamaji (2xxx): ";
	else if (is_allowed_foldsurface_processing(inGDB.at(0).DATATYPE)) {
		M = " fold axis calculation: ";
	}
	else {};

	cout << OUT + " " + M << endl;
}

void cout_original_tilted_text (const bool tilt) {

	if (is_mode_DEBUG()) return;

	if (!tilt) 	cout << "    - Original : " << flush;
	else 		cout << "    - Corrected: " << flush;
}

void cout_less_than_required_text (const bool tilt) {

	if (is_mode_DEBUG()) return;

	if (!tilt) cout << "less (independent) data than required." << endl;
}

void PS_GDB_DATA (const vector <GDB>& inGDB, ofstream& o, const CENTER& center) {

	for (size_t i = 0; i < inGDB.size(); i++) {

		const string DG = inGDB.at(i).DATAGROUP;

		if (is_allowed_lineation_datatype (DG)) 	PS_DRAW_lineation (inGDB.at(i), o, center);
		else if (is_allowed_plane_datatype (DG)) 	PS_DRAW_plane (inGDB.at(i), o, center);
		else if (is_allowed_striae_datatype(DG)) 	PS_DRAW_striae (inGDB.at(i), o, center);
		else if (is_allowed_SC_datatype (DG)) 		PS_DRAW_sc (inGDB.at(i), o, center);
		else ASSERT_DEAD_END();
	}
	return;
}

void dbg_cout_RGF_colors (vector <GDB> inGDB) {

	cout << "RGF colors" << endl;

	for (size_t i = 0; i < inGDB.size(); i++) {

		cout
		<< inGDB.at(i).ID << '\t'
		<< inGDB.at(i).GC << '\t'
		<< inGDB.at(i).PSCOLOR << '\t'
		<< inGDB.at(i).COLOR << '\t'
		<< endl;
	}
}
