// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef CHECKXYCONTENT_HPP_
#define CHECKXYCONTENT_HPP_

#include <string>
#include <vector>
#include <stdexcept>

#include "common.h"


using namespace std;

bool needxyfile ();
string inputxyfilename ();

bool xyEXISTENCEcheck (string xyname);
bool xyTABcheck (string xyname);
bool xyIDcheck (string xyname);
bool xyCOORDcheck (string xyname);

GDB insertxy (GDB inGDB, string xyfilename);

bool xyfile_correct (string projectname);
string check_xy_inputs (string inputfilename, bool batch);

#endif
