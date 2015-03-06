
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef CHECKSETTINGFILECONTENT_HPP_
#define CHECKSETTINGFILECONTENT_HPP_

#include "common.h"

using namespace std;

string input_setting_decision_cmd ();
void list_settings_option_cmd (const vector <vector <vector <string> > >& DEF, const size_t k);
vector <vector <string> > GET_SETTINGS_CMD (const string projectname);

vector <vector <string> > is_SETTING_AVAILABE_or_USE_DEFAULT (const string projectname);

vector <vector <string> > READ_SETTINGS_FILE (string settingfilename);
vector <vector <string> > RETURN_HARDCODED_SETTINGS ();
vector <vector <string> > COMPLETE_SET_WITH_DEFAULT (const vector <vector <string> >& SET);

bool is_WHITE_CHARACTER (const string in);
bool is_RECORD_CORRECT (const vector<string>& SETrecord, const vector<vector<vector<string> > >& DEFconst, size_t j, size_t k);
bool is_SETTINGS_RECORD_CORRECT (const vector <string>& SET);
bool is_SETTINGS_FILE_CORRECT (const string settingfilename);

void load_settings_batch (const string projectname);
void load_settings_cmd (const string projectname);

void WRITE_SET_FILE (const vector <vector <string> >& SET, const string projectname);

void dump_settings (const vector <vector <string> >& SET);

void dbg_cout_settings_vector (const vector < vector <string> >& IN);

#endif
