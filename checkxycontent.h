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

bool xyfile_correct (string projectname);
bool input_xy (const string& projectname);
void read_in_xy(const string& file_name);





//bool xyEXISTENCEcheck (string xyname);
////bool xyTABcheck (string xyname);
//bool xyIDcheck (string xyname);
//bool xyCOORDcheck (string xyname);

GDB insertxy (GDB inGDB);


string check_xy_inputs (string inputfilename);

#endif
