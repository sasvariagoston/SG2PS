// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef CHECKRGFFILECONTENT_HPP_
#define CHECKRGFFILECONTENT_HPP_

#include <string>

#include "structs.h"
#include "common.h"

using namespace std;

string inputfilename ();

bool linenumbercheck (string projectname);
bool rgf_file_existence (string projectname);
bool tabcheck (string projectname);
bool IDcheck (string projectname);
bool GCcheck (string projectname);
bool COLORcheck (string projectname);
bool LOCcheck (string projectname);
bool XYcheck (string projectname);
bool DATATYPEcheck (string projectname);
bool DIPDIRcheck (string projectname);
bool DIPcheck (string projectname);
string SENSEcheck (string SENSE);
bool STRIAE_MISFIT_check (string dipdir, string dip, string ldir, string ldip);
bool STRIAE_SC_check (string projectname);
bool PALEONcheck (string projectname);

vector <string> check_rgf_inputs (vector <string> inputfilename_vector, bool batch);

vector <string> create_inputfilename_vector (int argc, char *argv[]);

bool rgffile_correct (string projectname);

#endif
