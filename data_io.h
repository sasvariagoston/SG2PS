// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef DATA_IO_HPP_
#define DATA_IO_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <map>
#include <cstdlib>

#include "common.h"

using namespace std;

PFN createprojectfoldernames (string projectname);
bool createprojectfolders (PFN output, vector <GDB> inGDB);
bool copyoriginalfile (PFN output);

void outputrgfheader (ofstream& o, INPSET inset);
void outputaverageheader (ofstream& o);

void outputrecord (GDB i, ofstream& o, INPSET inpset);
void outputveragerecord (GDB i, ofstream& o);

void outputresultrgf (PFN output, vector <GDB> outGDB, bool tilted, INPSET inset);

void outputaveragergf (PFN output, vector <GDB> outGDB);

void outputselected_ps_rgf (PFN output, vector <GDB> outGDB, vector <GDB> tiltoutGDB, INPSET inset);

void output_to_rgf (PFN output, vector <GDB> processGDB, INPSET inset, bool tilted);
void output_to_ps (PFN output, vector <GDB> processGDB, vector <GDB> tiltprocessGDB, INPSET inset, PAPER P, CENTER center);

void process_group_by_group (vector <GDB> outGDB, vector <GDB> tiltoutGDB, ofstream& o, INPSET inset, CENTER center, PAPER P);
void process_one_by_one (GDB processGDB, GDB tiltprocessGDB, ofstream& o, INPSET inset, CENTER center, PAPER P);

void output_elapsed_time (double elapsed_time);

#endif
