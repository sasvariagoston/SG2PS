// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef CHECKSETTINGFILECONTENT_HPP_
#define CHECKSETTINGFILECONTENT_HPP_

#include "common.h"

using namespace std;

bool settingfilecorrect (string settingfilename);
INPSET loadsettingsfromsettingfile (string settingfilename);
void printsettingsonscreen (INPSET settings);
INPSET inputsettings_manually (string projectname);
bool outputsettingfile (INPSET _outputsettingfile, string projectname);
INPSET input_hardcoded ();
INPSET decide_setting_status (string projectname);
INPSET manage_settings_batch (string projectname);
INPSET manage_settings_nobatch (string projectname);
string input_setting_decision ();

#endif
