// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef GENERATE_DEFAULT_SETTINGS_HPP_
#define GENERATE_DEFAULT_SETTINGS_HPP_

#include <string>
#include <vector>

using namespace std;

void pushbach_settings_item (vector < vector <string> >& defitem, string key, string output);
void pushbach_settings_option (vector < vector <string> >& defitem, string value, string opt1, string opt2);
void pushbach_settings_range (vector < vector <string> >& defitem, string min, string max, string opt1, string opt2, string opt3, string dflt);

vector <vector < vector <string> > > return_default_settings_database ();

void dbg_default_settings_database (vector <vector < vector <string> > > DEF);

void dbg_generate_settings_file_list ();

#endif
