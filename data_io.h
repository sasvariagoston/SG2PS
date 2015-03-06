
// Copyright (C) 2012 - 2014  Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef DATA_IO_HPP_
#define DATA_IO_HPP_

#include <iosfwd>

#include "common.h"

using namespace std;

PFN create_project_folder_names (const string projectname);
vector <string> possible_folders_name ();
void make_dir (const string& dir_name);
void create_folders(const PFN& output, const string& dir);
void createprojectfolders (const PFN& output, const vector <GDB>& inGDB);
void copyoriginalfiles (const PFN& output);
void copy_log(const PFN& names);
void create_pointer_to_project_folder(const PFN& names);

void CREATE_PROJECT_FOLDER (const PFN& projectfoldername, const vector <GDB> inGDB);

vector < vector < vector <vector <GDB> > > > SEPARATE_DATASET_TO_GROUPS (const vector <GDB>& inGDB, const string METHOD);

vector <GDB> MERGE_GROUPS_TO_GDB (const vector < vector <vector <vector <GDB> > > >& GDB_G);
vector <vector <GDB> > MERGE_GROUPS_TO_GDB_G (const vector < vector <vector <vector <GDB> > > >& GDB_G);

vector <GDB> combine_inversion_for_none_offset (const vector <GDB>& process_GDB, const vector <GDB>& hasoffset_GDB);

vector <vector <vector <vector <GDB> > > > PROCESS_GROUPS (const vector <vector <vector <vector <GDB> > > >& inGDB_G, const bool TILT);

void output_rgf_header (ofstream& o, const bool AVERAGE);
void output_rgf_record (const GDB& i, ofstream& o, const bool AVERAGE);
void OUTPUT_AVERAGE_TO_RGF (const vector < vector <GDB> >& inGDB_G, const PFN& P, const bool TILT);
void OUTPUT_GROUPS_TO_RGF (const vector < vector <GDB> >& inGDB_G, const PFN& P, const bool TILTED);
void OUTPUT_COMPLETED_TO_RGF (const vector <GDB>& outGDB, const PFN& P, const bool TILT);
void OUTPUT_GDB_to_RGF (ofstream& O, const vector <GDB>& inGDB, const bool AVERAGE);
void OUTPUT_TO_RGF (const vector <vector <vector <vector <GDB> > > >& inGDB_G, const PFN& projectfoldername, const bool TILT);

void cout_method_text (const vector <GDB>& inGDB);
void cout_original_tilted_text (const bool tilt);
void cout_less_than_required_text (const bool tilt);

void PS_GDB_DATA (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);
void OUTPUT_TO_PS (const vector <vector <vector <vector <GDB> > > > n_GDB_G, const vector <vector <vector <vector <GDB> > > > t_GDB_G, const PFN P);

void dbg_cout_RGF_colors (vector <GDB> inGDB);

#endif
