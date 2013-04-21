// Copyright (C) 2012, 2013  Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef DATA_IO_HPP_
#define DATA_IO_HPP_

#include <iosfwd>

#include "common.h"

using namespace std;

PFN createprojectfoldernames (string projectname);
void createprojectfolders (PFN output, vector <GDB> inGDB);
void create_pointer_to_project_folder(const PFN& names);

void copyoriginalfiles (PFN output);
void copy_log(const PFN& names);

void outputrgfheader (ofstream& o);
void outputaverageheader (ofstream& o);

void outputrecord (GDB i, ofstream& o, INPSET inpset);
void outputveragerecord (GDB i, ofstream& o);

void outputresultrgf (PFN output, vector <GDB> outGDB, bool tilted, INPSET inset);

void outputaveragergf (PFN output, vector <GDB> outGDB);

void outputselected_ps_rgf (PFN output, vector <GDB> outGDB, vector <GDB> tiltoutGDB, INPSET inset);

void output_to_rgf (PFN output, vector <GDB> processGDB, INPSET inset, bool tilted);
void output_to_ps (PFN output, vector <GDB> processGDB, vector <GDB> tiltprocessGDB, INPSET inset, PAPER P, CENTER center);

vector <GDB> process_group_by_group (vector <GDB> outGDB, ofstream& o, INPSET inset, CENTER center, PAPER P, bool tilt);
void process_one_by_one (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center, PAPER P, bool tilt);

void dbg_cout_RGF_colors (vector <GDB> inGDB);

#endif
