// Copyright (C) 2012-2017, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>

#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "color_management.hpp"
#include "data_io.h"
#include "data_sort.hpp"
#include "density.h"
#include "filename.hpp"
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

vector <string> possible_folders_name () {

	vector <string> OUT = allowed_lineation_datatype_vector();

	OUT = merge_datatypes(OUT, allowed_plane_datatype_vector());
	OUT = merge_datatypes(OUT, allowed_striae_datatype_vector());
	OUT = merge_datatypes(OUT, allowed_SC_datatype_vector());

	return OUT;
}

void make_dir (const string& dir_name) {

	string command = MKDIR + dir_name;

	int ret = system (command.c_str());

	if (ret) throw runtime_error("failed to execute command \""+command+"\"");
}


bool dir_exists(const string& dir) {
    struct stat sb;
    return stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
}

void create_required_folders (const vector <GDB>& inGDB) {

	make_dir (return_PROJECT_FOLDER());
	make_dir (return_ORIGINAL_FOLDER());
	make_dir (return_COMPLETED_FOLDER());
	make_dir (return_AVERAGE_FOLDER());
	make_dir (return_RGF_FOLDER());

	vector <string> possible_folders = possible_folders_name ();

	for (size_t i = 0; i < possible_folders.size(); ++i) {

		const string DIR = possible_folders.at(i);

		if (existence (DIR, inGDB)) {

			make_dir (return_RGF_FOLDER() + path_separator + capslock (DIR));
		}
	}
	return;
}

namespace {

struct copy_status { enum code { OK, OPENING_INPUT_FAILED, FAILED }; };

copy_status::code copy_file(const string& source, const string& destination) {

	ifstream in(source.c_str(), ios::binary);

	if (!in.is_open()) return copy_status::OPENING_INPUT_FAILED;

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

	if (status == copy_status::OK) {

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

void copy_original_files () {

	const string project_name = return_PROJECTNAME();

	const string new_path = return_ORIGINAL_FOLDER() + path_separator;

	const bool SHOULD_EXIST = true;

	back_up_file (".rgf", project_name, new_path, SHOULD_EXIST);

	back_up_file (".set", project_name, new_path);

	back_up_file (".xy", project_name, new_path);

	back_up_file (".trj", project_name, new_path);
}

void copy_rgf_to_test_file (const string FN, const string ARG) {

	back_up_file (".rgf", ARG, FN);
}

void copy_trj_to_test_file (const string FN, const string ARG) {

	ifstream i;

	i.open ((ARG + ".trj").c_str());

	if (i.is_open()) back_up_file (".trj", ARG, FN);
}

void copy_xy_to_test_file (const string FN, const string ARG) {

	ifstream i;

	i.open ((ARG + ".xy").c_str());

	if (i.is_open()) back_up_file (".xy", ARG, FN);
}

void copy_log () {

	if (is_mode_STD()) {

		cout.flush();

		string log_file = "log.txt";

		copy_file (log_file, return_ORIGINAL_FOLDER() + path_separator + log_file);
	}
}

void create_pointer_to_project_folder () {

	if (is_mode_STD()) {

		ofstream pointer_to ("project_folder_name");

		pointer_to << return_PROJECT_FOLDER() << '\n';
	}
}

void CREATE_PROJECT_FOLDERS (const vector <GDB>& inGDB) {

	create_required_folders (inGDB); // TODO Failures in creating project folders silently ignored

	create_pointer_to_project_folder ();

	copy_original_files ();
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

	const bool is_STRIAE = is_allowed_striae_datatype(i.DATATYPE);
	const bool is_SC = is_allowed_SC_datatype(i.DATATYPE);

	const bool is_LITHOLOGY = is_allowed_lithology_datatype(i.DATATYPE);
	const bool is_BEDDING = is_allowed_foldsurface_processing(i.DATATYPE);

	o
	<< i.ID << '\t';

	if (is_WELLDATA_USE()) o << i.DEPTH << '\t';
	else o << '\t';

	o
	<< i.GC << '\t'
	<< i.COLOR << '\t'
	<< i.LOC << '\t'
	<< fixed << setprecision (6)
	<< i.LOCX << '\t'
	<< i.LOCY << '\t'
	<< i.FORMATION << '\t'
	<< i.DATATYPE << '\t';

	if (AVERAGE) {

		if (!is_LITHOLOGY && !is_SC && !is_STRIAE && is_allowed_DIR (i.corr.DIPDIR)) o << i.corr.DIPDIR << '\t';
		else o << "" << '\t';

		if (!is_LITHOLOGY && !is_SC && !is_STRIAE && is_allowed_DIP (i.corr.DIP)) o << i.corr.DIP << '\t';
		else o << "" << '\t';

		o << "" << '\t';
		o << "" << '\t';
		o << "" << '\t';
		o << "" << '\t';
	}
	else {

		o << fixed << setprecision (1);

		if (!is_LITHOLOGY && is_allowed_DIP (i.corr.DIP)){

			if (is_DATARULE_RIGHT_HAND_RULE() ) o << german_to_right_hand_rule (i.corr.DIPDIR) << '\t';
			else o << i.corr.DIPDIR << '\t';
		}
		else o << "" << '\t';

		if (!is_LITHOLOGY && is_allowed_DIP (i.corr.DIP)) o << i.corr.DIP << '\t';
		else o << "" << '\t';

		if ((is_SC || is_STRIAE) && is_allowed_DIR(i.corrL.DIPDIR)) o << i.corrL.DIPDIR << '\t';
		else o << "" << '\t';

		if ((is_SC || is_STRIAE) && is_allowed_DIP(i.corrL.DIP)) o << i.corrL.DIP << '\t';
		else o << "" << '\t';

		if (is_STRIAE || is_BEDDING) {

			if (is_BEDDING && is_allowed_striae_none_sense(i.OFFSET)) o << '\t';
			else o << i.OFFSET << '\t';
		}
		else o << "" << '\t';

		if (is_allowed_DIR (fabs (i.PALEON))) o << i.PALEON	<< '\t';
		else o << '\t';
	}
	o << i.COMMENT << '\n';

	return;
}

void OUTPUT_COMPLETED_TO_RGF (const vector <GDB>& inGDB) {

	const string FN = generate_ACTUAL_COMPLETED_RGF_NAME ();

	ofstream O;
	O.open (FN.c_str());

	output_rgf_header (O, false);

	OUTPUT_GDB_to_RGF (O, inGDB, false);

	O.close();
}

void OUTPUT_AVERAGE_TO_RGF (const vector <GDB>& inGDB_G) {

	const string FN = generate_ACTUAL_AVERAGE_RGF_NAME();

	ofstream O;
	O.open (FN.c_str());

	output_rgf_header (O, true);

	vector <GDB> temp;

	temp.push_back (inGDB_G.at(0));

	const bool STRIAE = is_allowed_striae_datatype(temp.at(0).DATATYPE);
	const bool SC = is_allowed_SC_datatype(temp.at(0).DATATYPE);

	if (!STRIAE && !SC) OUTPUT_GDB_to_RGF (O, temp, true);

	O.close();

	return;
}

void OUTPUT_GROUPS_TO_RGF (const vector <vector <GDB> >& inGDB_G) {

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		vector <GDB> T = inGDB_G.at(i);

		setup_ACTUAL_DATATYPE (T.at(0).DATATYPE);
		setup_ACTUAL_GROUPCODE (T.at(0).GC);
		setup_ACTUAL_LOCATION (T.at(0).LOC);

		const string FN = generate_ACTUAL_RGF_NAME ();

		ofstream O;

		O.open (FN.c_str());

		output_rgf_header (O, false);

		T = SORT_GDB(T, "IID");

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

void OUTPUT_TO_RGF (const vector <vector <GDB> >& inGDB_G) {

	vector <GDB> processGDB = MERGE_GROUPS_TO_GDB (inGDB_G);

	OUTPUT_COMPLETED_TO_RGF (processGDB);

	vector <GDB> processGDB_G = MERGE_GROUPS_TO_GDB (inGDB_G);

	OUTPUT_AVERAGE_TO_RGF (processGDB_G);

	OUTPUT_GROUPS_TO_RGF (inGDB_G);

	return;
}

GDB return_dummy_GDB () {

	GDB dummy;

	const string DUMMY = "xxx__DUMMY__xxx";

	dummy.GC = DUMMY;
	dummy.FORMATION = DUMMY;
	dummy.GC = DUMMY;
	dummy.DATATYPE = DUMMY;
	dummy.LOC = DUMMY;

	return dummy;
}

bool ACT_NXT_EQ (const GDB& ACT, const GDB& NXT, const string METHOD) {

	ASSERT_ONE_OF(METHOD, "GROUPS", "FORMATION", "CLUSTER", "RUP_ANG", "DATATYPE", "LOCATION");

	if (METHOD == "GROUPS") 		return ACT.GC.at(0) == NXT.GC.at(0);
	else if (METHOD == "FORMATION") return ACT.FORMATION == NXT.FORMATION;
	else if (METHOD == "CLUSTER") 	return ACT.GC.at(1) == NXT.GC.at(1);
	else if (METHOD == "RUP_ANG")	return ACT.GC.at(2) == NXT.GC.at(2);
	else if (METHOD == "DATATYPE") 	return ACT.DATATYPE == NXT.DATATYPE;
	else 							return ACT.LOC == NXT.LOC;
}

vector <vector <GDB> > SEPARATE (const vector <GDB> & inGDB, const string METHOD) {

	vector <vector <GDB> > OUT;
	vector <GDB> buf;

	vector <GDB> temp = inGDB;

	temp.push_back (return_dummy_GDB());

	for (size_t i = 0; i < temp.size() - 1; i++) {

		const GDB ACT = temp.at(i);
		const GDB NXT = temp.at(i+1);

		buf.push_back(ACT);

		const bool EQ = ACT_NXT_EQ (ACT, NXT, METHOD);

		if (!EQ) {

			OUT.push_back (buf);
			buf.clear();
		}
	}
	return OUT;
}

vector <vector <GDB> > SEPARATE_DATASET (const vector <vector <GDB> >& inGDB_G, const string METHOD, const string SORT) {

	ASSERT (inGDB_G.size() > 0);

	vector <vector <GDB> > OUT;

	for (size_t i = 0; i < inGDB_G.size(); i++) {

		ASSERT (inGDB_G.at(i).size() > 0);

		const vector <GDB> processGDB = SORT_GDB (inGDB_G.at(i), SORT);

		const vector <vector <GDB> > SEP = SEPARATE (processGDB, METHOD);

		for (size_t j = 0; j < SEP.size(); j++) OUT.push_back(SEP.at(j));
	}
	return OUT;
}

vector <GDB> MERGE_GROUPS_TO_GDB (const vector <vector <GDB> >& GDB_G) {

	vector <GDB> outGDB;

	for (size_t i = 0; i < GDB_G.size(); i++) outGDB = merge_GDB (GDB_G.at(i), outGDB);

	outGDB = SORT_GDB (outGDB, "IID");

	return outGDB;
}

vector <GDB> combine_inversion_for_none_offset (const vector <GDB>& process_GDB, const vector <GDB>& hasoffset_GDB) {

	vector <GDB> OUT = process_GDB;

	const bool IS_STRIAE = is_allowed_striae_datatype(process_GDB.at(0).DATATYPE);
	const bool IS_SC = is_allowed_SC_datatype(process_GDB.at(0).DATATYPE);

	for (size_t i = 0; i < OUT.size(); i++) {
		for (size_t j = 0; j < hasoffset_GDB.size(); j++) {

			const string P_ID = OUT.at(i).ID;
			const string H_ID = hasoffset_GDB.at(j).ID;

			if (P_ID == H_ID && IS_STRIAE) OUT.at(i) = hasoffset_GDB.at(j);
			if (P_ID == H_ID && IS_SC){

				OUT.at(i).SFV = hasoffset_GDB.at(j).SFV;
				OUT.at(i).STV = hasoffset_GDB.at(j).STV;
			}
		}
	}
	return OUT;
}

vector <vector <GDB> > PROCESS_GROUPS (const vector <vector <GDB> >& inGDB_G, const bool TILT) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	outGDB_G = associate_empty_clustercode (outGDB_G, 2);

	for (size_t i = 0; i < outGDB_G.size(); i++) {

		vector <GDB> process_GDB = outGDB_G.at(i);

		const string DT = process_GDB.at(0).DATATYPE;
		const bool IS_STRIAE = is_allowed_striae_datatype(DT);
		const bool IS_SC = is_allowed_SC_datatype(DT);
		const bool FOLD_TO_PROCESS = is_allowed_foldsurface_processing(DT);

		const bool FRACTURE_TO_PROCESS = is_BINGHAM_USE() && DT == "FRACTURE";
		const bool SC_TO_PROCESS = !is_INVERSION_NONE() && IS_SC;
		const bool STRIAE_TO_PROCESS = !is_INVERSION_NONE() && IS_STRIAE;

		const bool ENOUGH_HOMOGENEOUS = has_inhomogeneous_enough(process_GDB);

		if (FRACTURE_TO_PROCESS || STRIAE_TO_PROCESS || SC_TO_PROCESS) {

			vector <GDB> hasoffset_GDB = process_GDB;

			if (IS_SC) hasoffset_GDB = convert_SC_to_striae_with_offset(process_GDB);
			if (IS_STRIAE) hasoffset_GDB = return_striae_with_offset (process_GDB);//was testGDB

			ASSERT_LE (hasoffset_GDB.size(), process_GDB.size());

			const bool ENOUGH_DATA = hasoffset_GDB.size() >= minimum_independent_dataset();

			if (ENOUGH_HOMOGENEOUS && ENOUGH_DATA) {

				cout_method_text (hasoffset_GDB);

				vector <GDB> hasoffset_virtual_GDB;

				if (is_VIRTUAL_USE() && (STRIAE_TO_PROCESS || SC_TO_PROCESS)) {

					hasoffset_virtual_GDB = generate_virtual_striae (hasoffset_GDB);
				}

				if (is_VIRTUAL_USE()) 	INVERSION (hasoffset_virtual_GDB);
				else 					INVERSION (hasoffset_GDB);

				const vector <STRESSTENSOR> STV = return_STV ();
				const vector <STRESSFIELD> SFV = return_SFV ();

				ASSERT_EQ (SFV.size(), STV.size());

				hasoffset_GDB = ASSOCIATE_STV_SFV (hasoffset_GDB, STV, SFV);

				const bool SUCCESSFULL = (STV.size() > 0 && SFV.size() > 0);

				const bool SUCC_STR = SUCCESSFULL && (STRIAE_TO_PROCESS || SC_TO_PROCESS);

				if (SUCC_STR) {

					const size_t MX = STV.size() - 1;

					hasoffset_GDB = apply_inversion_result (hasoffset_GDB, STV.at(MX));

					if (STRIAE_TO_PROCESS) hasoffset_GDB = apply_RUP_ANG_CLUSTERING_result (hasoffset_GDB);
				}
				cout_inversion_results (hasoffset_GDB, SFV);

				process_GDB = combine_inversion_for_none_offset (process_GDB, hasoffset_GDB);
			}
			else cout_less_than_required_text (hasoffset_GDB);
		}
		else if (FOLD_TO_PROCESS) process_GDB = CALCULATE_FOLDSURFACE_NORMAL (process_GDB);
		else {}

		outGDB_G.at(i) = process_GDB;
	}
	return outGDB_G;
}


/*
 *
 * vector <vector <GDB> > PROCESS_GROUPS (const vector <vector <GDB> >& inGDB_G, const bool TILT) {

	vector <vector <GDB> > outGDB_G = inGDB_G;

	outGDB_G = associate_empty_clustercode (outGDB_G, 2);

	for (size_t i = 0; i < outGDB_G.size(); i++) {

		vector <GDB> process_GDB = outGDB_G.at(i);

		const string DT = process_GDB.at(0).DATATYPE;
		const bool IS_STRIAE = is_allowed_striae_datatype(DT);
		const bool FOLD_TO_PROCESS = is_allowed_foldsurface_processing(DT);

		const bool FRACTURE_TO_PROCESS = is_BINGHAM_USE() && DT == "FRACTURE";
		const bool STRIAE_TO_PROCESS = !is_INVERSION_NONE() && IS_STRIAE;

		const bool ENOUGH_HOMOGENEOUS = has_inhomogeneous_enough (process_GDB);

		if (FRACTURE_TO_PROCESS || STRIAE_TO_PROCESS) {

			vector <GDB> hasoffset_GDB = process_GDB;

			if (IS_STRIAE) hasoffset_GDB = return_striae_with_offset (process_GDB);//was testGDB

			ASSERT_LE (hasoffset_GDB.size(), process_GDB.size());

			const bool ENOUGH_STRIAE = hasoffset_GDB.size() >= minimum_independent_dataset ();

			if (ENOUGH_HOMOGENEOUS && ENOUGH_STRIAE) {

				cout_method_text (hasoffset_GDB);

				vector <GDB> hasoffset_virtual_GDB;

				if (is_VIRTUAL_USE() && STRIAE_TO_PROCESS) {

					hasoffset_virtual_GDB = generate_virtual_striae (hasoffset_GDB);
				}

				if (is_VIRTUAL_USE()) 	INVERSION (hasoffset_virtual_GDB);
				else 					INVERSION (hasoffset_GDB);

				const vector <STRESSTENSOR> STV = return_STV ();
				const vector <STRESSFIELD> SFV = return_SFV ();

				ASSERT_EQ (SFV.size(), STV.size());

				hasoffset_GDB = ASSOCIATE_STV_SFV (hasoffset_GDB, STV, SFV);

				const bool SUCCESSFULL = (STV.size() > 0 && SFV.size() > 0);

				const bool SUCC_STR = SUCCESSFULL && STRIAE_TO_PROCESS;

				if (SUCC_STR) {

					const size_t MX = STV.size() - 1;

					hasoffset_GDB = apply_inversion_result (hasoffset_GDB, STV.at(MX));
				}

				cout_inversion_results (hasoffset_GDB, SFV);

				if (SUCC_STR) hasoffset_GDB = apply_RUP_ANG_CLUSTERING_result (hasoffset_GDB);

				process_GDB = combine_inversion_for_none_offset (process_GDB, hasoffset_GDB);
			}
			else cout_less_than_required_text (hasoffset_GDB);
		}
		else if (FOLD_TO_PROCESS) process_GDB = CALCULATE_FOLDSURFACE_NORMAL (process_GDB);
		else {}

		outGDB_G.at(i) = process_GDB;
	}
	return outGDB_G;
}
 */
void cout_method_text (const vector <GDB>& inGDB) {

	if (is_mode_DEBUG()) return;

	const string LOC = inGDB.at(0).LOC;
	const string GC = inGDB.at(0).GC;
	const string FM = inGDB.at(0).FORMATION;

	const bool GROUPS = is_GROUPS_USE();
	const bool FORMATION = is_FORMATION_USE();
	const bool CLUSTER = !is_CLUSTERING_NONE();
	const bool RUP = !is_RUP_CLUSTERING_NONE();
	const bool IS_STRIAE = is_allowed_striae_datatype (inGDB.at(0).DATATYPE);
	const bool IS_SC = is_allowed_SC_datatype(inGDB.at(0).DATATYPE);

	string OUT = "  - For '" + LOC + "' location";

	if (GROUPS || CLUSTER || RUP) {

		if (GROUPS) 		OUT = OUT + ", '" + GC.at(0) + "' group (using group code)";
		else if (CLUSTER) 	OUT = OUT + ", '" + GC.at(1) + "' group (using k-means clustering))";
		else				OUT = OUT + ",' " + GC.at(2) + "' group (using RUP/ANG clustering)";
	}

	if (FORMATION) OUT = OUT + ", '" + FM + "' formation";

	string M = "";

	if (is_INVERSION_ANGELIER() && (IS_STRIAE || IS_SC))		M = "inversion after Angelier (1990): ";
	else if (is_BINGHAM_USE() && !(IS_STRIAE || IS_SC)) 		M = "fracture statistics after Bingham (1964): ";
	else if (is_INVERSION_BRUTEFORCE() && (IS_STRIAE || IS_SC))	M = "brute force inversion: ";
	else if (is_INVERSION_FRY() && (IS_STRIAE || IS_SC))		M = "regression after Fry (1999): ";
	else if (is_INVERSION_MICHAEL() && (IS_STRIAE || IS_SC))	M = "regression after Michael (1984): ";
	else if (is_INVERSION_MOSTAFA() && (IS_STRIAE || IS_SC)) 	M = "inversion after Mostafa (2005): ";
	else if (is_INVERSION_SPRANG() && (IS_STRIAE || IS_SC))		M = "regression after Sprang (1972): ";
	else if (is_INVERSION_TURNER() && (IS_STRIAE || IS_SC)) 	M = "regression after Turner (1953): ";
	else if (is_INVERSION_SHAN() && (IS_STRIAE || IS_SC))		M = "regression after Shan et al. (2003): ";
	else if (is_INVERSION_YAMAJI() && (IS_STRIAE || IS_SC))		M = "iteration after Yamaji (2xxx): ";
	else if (is_allowed_foldsurface_processing(inGDB.at(0).DATATYPE)) {
		M = " fold axis calculation: ";
	}
	else {};

	cout << OUT << ", " << M << '\n';
}

void cout_original_tilted_text (const bool tilt) {

	if (is_mode_DEBUG()) return;

	if (!tilt) 	cout << "    - Original:  " << flush;
	else 		cout << "    - Corrected: " << flush;
}

void cout_less_than_required_text (const vector <GDB>& inGDB) {

	if (is_mode_DEBUG()) return;

	const size_t MIN = minimum_independent_dataset();

	ASSERT_GE (MIN, inGDB.size());

	cout << "    - less independent data (" << inGDB.size() << ") than required (" << MIN <<")." << endl;
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
