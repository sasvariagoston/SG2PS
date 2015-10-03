// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef GENERATE_DEFAULT_SETTINGS_HPP_
#define GENERATE_DEFAULT_SETTINGS_HPP_

#include <string>
#include <vector>

using namespace std;

void pushbach_settings_item (vector < vector <string> >& defitem, const string key, const string output);
void pushbach_settings_option (vector < vector <string> >& defitem, const string value, const string opt1, const string opt2);
void pushbach_settings_range (vector < vector <string> >& defitem, const string min, const string max, const string opt1, const string opt2, const string opt3, const string dflt);

vector <vector < vector <string> > > RETURN_ALL_SETTINGS ();

void dbg_default_settings_database (const  vector <vector < vector <string> > >& DEF);

void dump_keys_values_into_settings_file (const vector <string>& KEY, const vector <string>& VAL, const string FN);

void dbg_generate_settings_file_list (const string ARG);

#endif
