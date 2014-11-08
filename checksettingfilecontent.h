// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef CHECKSETTINGFILECONTENT_HPP_
#define CHECKSETTINGFILECONTENT_HPP_

#include "common.h"

using namespace std;

bool has_settings_file_data (string settingfilename);

vector < vector <string> > read_settingsfile_to_vector (string settingfilename);

bool is_whitecharacter (string in);
bool fit_of_records (const vector<string>& SETrecord, const vector<vector<vector<string> > >& DEFconst, size_t j, size_t k);
bool is_setting_record_correct (vector <string> SET);
bool is_settings_file_correct (string settingfilename);

vector <vector <string> > input_hardcoded ();
vector <vector <string> > apply_default (vector <vector <string> > SET);

string input_setting_decision ();
vector <vector <string> > decide_setting_status (string projectname);

void list_settings_options (vector <vector <vector <string> > > DEF, size_t k);
vector <vector <string> > inputsettings_manually (string projectname);

void manage_settings_batch (string projectname);
void manage_settings_nobatch (string projectname);

void outputsettingfile (vector <vector <string> > SET, string projectname);
void dump_actual_settings (vector <vector <string> >SET);

void dbg_cout_settings_vector (vector < vector <string> > IN);

#endif
