// Copyright (C) 2012-2017,  Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef DATA_IO_HPP_
#define DATA_IO_HPP_

#include <iosfwd>

#include "common.h"

using namespace std;

vector <string> possible_folders_name ();
void make_dir (const string& dir_name);
bool dir_exists(const string& dir_name);
void create_required_folders (const vector <GDB>& inGDB);
void copy_original_files ();
void copy_log ();
void copy_rgf_to_test_file (const string FN, const string ARG);
void copy_trj_to_test_file (const string FN, const string ARG);
void copy_xy_to_test_file (const string FN, const string ARG);

void create_pointer_to_project_folder ();

void CREATE_PROJECT_FOLDERS (const vector <GDB>& inGDB);

GDB return_dummy_GDB ();
bool ACT_NXT_EQ (const GDB& ACT, const GDB& NXT, const string METHOD);
vector <vector <GDB> > SEPARATE (const vector <GDB> & inGDB, const string METHOD);
vector <vector <GDB> > SEPARATE_DATASET (const vector <vector <GDB> >& inGDB_G, const string METHOD, const string SORT);

vector <GDB> MERGE_GROUPS_TO_GDB (const vector <vector <GDB> >& GDB_G);

vector <GDB> combine_inversion_for_none_offset (const vector <GDB>& process_GDB, const vector <GDB>& hasoffset_GDB);

vector <vector <GDB> > PROCESS_GROUPS (const vector <vector <GDB> >& inGDB_G);

void output_rgf_header (ofstream& o, const bool AVERAGE);
void output_rgf_record (const GDB& i, ofstream& o, const bool AVERAGE);
void OUTPUT_AVERAGE_TO_RGF (const vector <GDB> & inGDB_G);
void OUTPUT_GROUPS_TO_RGF (const vector < vector <GDB> >& inGDB_G);
void OUTPUT_COMPLETED_TO_RGF (const vector <GDB>& outGDB);
void OUTPUT_GDB_to_RGF (ofstream& O, const vector <GDB>& inGDB, const bool AVERAGE);
void OUTPUT_TO_RGF (const vector <vector <GDB> >& inGDB_G);

void cout_method_text (const vector <GDB>& inGDB);
void cout_original_tilted_text (const bool tilt);
void cout_less_than_required_text (const vector <GDB>& inGD);

void dbg_cout_RGF_colors (vector <GDB> inGDB);

#endif
