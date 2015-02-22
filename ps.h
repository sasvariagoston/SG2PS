// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef PS_HPP_
#define PS_HPP_

#include <string>
#include <vector>

#include "structs.h"

using namespace std;

void PS_folddata (GDB in, ofstream& o, CENTER center);

PAPER PS_dimensions (const bool WELL);
void PS_stereonet_header (const string DATATYPE, const string LOC, ofstream& o);
void PS_getstereonet (ofstream& o, CENTER center);
void PS_stress_scale (ofstream& o, const PAPER& P);
void PS_border (const vector <GDB>& inGDB, ofstream& o, const PAPER& P);
void PS_stressdata (const vector <GDB>& inGDB, ofstream& o, const CENTER& center, const PAPER& P, const STRESSFIELD& sf);
void PS_stressarrows (ofstream& o, const CENTER& center, const PAPER& P, const STRESSFIELD& sf);
void PS_mohr_circle (const vector <GDB>& inGDB, ofstream& o, const CENTER& mohrcenter, const PAPER& P);
void PS_RUP_ANG_distribution (const vector <GDB>& inGDB, ofstream& o, const CENTER& center, const PAPER& P, const string method);
void PS_stress_axes (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);
void PS_idealmovement (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);
void PS_INVERSION_RESULTS (const vector <GDB>& inGDB, ofstream& o, const CENTER& center, const CENTER& mohr_center, const PAPER& P);

void PS_FOLD_GREAT_CIRCLE (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);

void PS_datanumber_averagebedding (const GDB& i, ofstream& o, const PAPER& P, const size_t datanumber);
void PS_net (ofstream& o, const PAPER& P);

void PS_GDB (const vector <GDB>& inGDB, ofstream& o, PAPER P, bool TILT);

void PS_DRAW_lineation (const GDB& i, ofstream& o, const CENTER& center);
void PS_DRAW_plane (const GDB i, ofstream& o, const CENTER& center);
void PS_DRAW_striae (const GDB& i, ofstream& o, const CENTER& center);
void PS_DRAW_sc (const GDB& i, ofstream& o, const CENTER& center);

void PS_lineation (const GDB& i, ofstream& o, const CENTER& center, const STRESSFIELD& sf, const bool label, const string type);
void PS_plane (const GDB& i, ofstream& o, const double X, const double Y, const double R, const bool LABEL, const string TYPE);
void PS_polepoint (const GDB& i, ofstream& o, const double X, const double Y, const double R, const bool LABEL, const string TYPE);
void PS_striaearrow (const GDB& i, ofstream& o, const CENTER& center);
void PS_stress_state (ofstream& o, const PAPER P, const CENTER& center, const STRESSFIELD& sf);

void PS_rosesegment (ofstream& o, const CENTER center, const double percentage, const double degree, const bool c_plane);
void PS_draw_rose_circle (ofstream& o, const CENTER& center, const double percent, const double VERTICAL);

void newpath_PS (ofstream& o);
void color_PS (ofstream& o, const string& RGB);
void text_PS (ofstream& o, const double X, const double Y, const size_t decimals, const string text);
void text_PS (ofstream& o, const string text);
void moveto_PS (ofstream& o, const double X, const double Y, const size_t decimals);
void curveto_PS (ofstream& o, const double AX, const double AY, const double BX, const double BY, const double CX, const double DY, const size_t decimals);
void lineto_PS (ofstream& o, const double X, const double Y, const size_t decimals);
void rlineto_PS (ofstream& o, const double X, const double Y, const size_t decimals);
void translate_PS (ofstream& o, const double X, const double Y, const size_t decimals);
void rotate_PS (ofstream& o, const double ANG, const size_t decimals);
void linewidth_PS (ofstream& o, const double LW, const size_t decimals);
void stroke_PS (ofstream& o);
void closepath_PS (ofstream& o);
void font_PS (ofstream& o, const string& font, const size_t size);
void arc_PS (ofstream& o, const double X, const double Y, const double R, const double ANG_STR, const double ANG_END, const size_t decimals);
void gsave_PS (ofstream& o);
void fill_PS (ofstream& o);
void grestore_PS (ofstream& o);
void setdash_PS (ofstream& o, const string DASH);

void PS_STEREONET_SYMBOLS (const vector <GDB>& inGDB, ofstream& o, const PAPER& P);
void PS_SYMBOLS_border (ofstream& o, const PAPER& P);
void PS_SYMBOLS_ROSE (const vector <GDB>& inGDB, ofstream& o, const PAPER& P);
void PS_SYMBOLS_LABEL (ofstream& o, const PAPER& P);
void PS_SYMBOLS_GROUPS (ofstream& o, const PAPER& P);
void PS_SYMBOL_draw_plane (ofstream& o, const double X, const double Y, const PAPER& P, const string& TYPE);
void PS_SYMBOLS_PLANE (const string& DATATYPE, ofstream& o, const PAPER& P);
void PS_SYMBOLS_BINGHAM (ofstream& o, const PAPER& P);
void PS_SYMBOLS_LINEATION (const string& DATATYPE, ofstream& o, const PAPER& P);
void PS_SYMBOLS_STRIAE (ofstream& o, const PAPER& P);
void PS_SYMBOLS_INVERSION (ofstream& o, const PAPER& P);

#endif
