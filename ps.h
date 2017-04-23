// Copyright (C) 2012-2017, Ágoston Sasvári
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

void PS_s1s2s3 (ofstream& o, const string COLOR, const bool ITER, const string AXIS);
void PS_extension_arrow (ofstream& o, const string COLOR);
void PS_compression_arrow (ofstream& o, const string COLOR);
void PS_stereonet_header (ofstream& o);
void PS_getstereonet (ofstream& o, CENTER center);
void PS_stress_scale (ofstream& o, const bool TILT);
void PS_border (const vector <GDB>& inGDB, ofstream& o);
void PS_dump_inversion_method (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);
string PS_stressdata (const vector <GDB>& inGDB, ofstream& o);
void PS_stressarrows (ofstream& o, const vector <GDB>& inGDB, const CENTER& center);
void PS_mohr_arcs (ofstream& o, const CENTER& mohrcenter, const double CNTR, const double RAD);
void PS_mohr_circle (const vector <GDB>& inGDB, ofstream& o, const CENTER& mohrcenter);
double PS_RUP_ANG_distribution (const vector <GDB>& inGDB, ofstream& o, const CENTER& center, const string method, const bool PERCENTAGE_ONLY, const bool TILT);
void PS_stress_axes (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);
void PS_idealmovement (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);
string PS_INVERSION_RESULTS (const vector <GDB>& inGDB, ofstream& o, const CENTER& center, const CENTER& mohr_center);

void PS_FOLD_GREAT_CIRCLE (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);

void PS_datanumber_averagebedding (const GDB& i, ofstream& o, const size_t datanumber);
void PS_net (ofstream& o);
void PS_eof (ofstream& o);

void PS_DRAW_lineation (const GDB& i, ofstream& o, const CENTER& center);
void PS_DRAW_plane (const GDB i, ofstream& o, const CENTER& center);
void PS_DRAW_striae (const GDB& i, ofstream& o, const CENTER& center);
void PS_DRAW_sc (const GDB& i, ofstream& o, const CENTER& center);

void PS_lineation (const GDB& i, ofstream& o, const CENTER& center, const STRESSFIELD& sf, const bool label, const string type);
void PS_plane (const GDB& i, ofstream& o, const double X, const double Y, const double R, const string TYPE);
void PS_polepoint (const GDB& i, ofstream& o, const double X, const double Y, const double R, const string TYPE);
void PS_striaearrow (const GDB& i, ofstream& o, const CENTER& center);
void PS_stress_state (ofstream& o, const vector <GDB>& inGDB, const CENTER& center);

void PS_rosesegment (ofstream& o, const CENTER center, const double percentage, const double degree, const bool c_plane, const string COLOR);
void PS_draw_rose_circle (ofstream& o, const CENTER& center, const double percent, const bool VERTICAL);

void newpath_PS (ofstream& o);
bool is_valid_color(const string& s);
void color_PS (ofstream& o, const string& RGB);
void text_to_PS (ofstream& o, const string text);
void text_PS (ofstream& o, const double X, const double Y, const int decimals, const string text);
void text_PS (ofstream& o, const string text);
void moveto_PS (ofstream& o, const double X, const double Y, const int decimals);
void curveto_PS (ofstream& o, const double AX, const double AY, const double BX, const double BY, const double CX, const double DY, const int decimals);
void lineto_PS (ofstream& o, const double X, const double Y, const int decimals);
void rlineto_PS (ofstream& o, const double X, const double Y, const int decimals);
void translate_PS (ofstream& o, const double X, const double Y, const int decimals);
void rotate_PS (ofstream& o, const double ANG, const int decimals);
void linewidth_PS (ofstream& o, const double LW, const int decimals);
void stroke_PS (ofstream& o);
void closepath_PS (ofstream& o);
void font_PS (ofstream& o, const string& font, const size_t size);
void arc_PS (ofstream& o, const double X, const double Y, const double R, const double ANG_STR, const double ANG_END, const int decimals);
void gsave_PS (ofstream& o);
void fill_PS (ofstream& o);
void grestore_PS (ofstream& o);
void setdash_PS (ofstream& o, const string DASH);

void PS_STEREONET_SYMBOLS (ofstream& o, const vector <vector <GDB> >& inGDB_G);
void PS_SYMBOLS_border (ofstream& o);
void PS_SYMBOLS_ROSE (const vector <GDB>& inGDB, ofstream& o);
void PS_SYMBOLS_LABEL (ofstream& o);
void PS_SYMBOLS_GROUPS (ofstream& o);
void PS_SYMBOL_draw_plane (ofstream& o, const double X, const double Y, const string& TYPE);
void PS_SYMBOLS_PLANE (const string& DATATYPE, ofstream& o);
void PS_SYMBOLS_BINGHAM (ofstream& o);
void PS_SYMBOLS_LINEATION (const string& DATATYPE, ofstream& o);
void PS_SYMBOLS_STRIAE (ofstream& o);
void PS_SYMBOLS_INVERSION (ofstream& o);

void ROSE (const vector <vector <GDB> >& inGDB_G, ofstream& o, const CENTER& rosecenter, const CENTER& vrosecenter, const bool TILT);
void ps_dump_stress_state (ofstream& o, const CENTER& center, const string DATATYPE_TO_DUMP, const vector <string>& INV, const vector <string>& CLR);
void PS_GDB (const vector <vector <GDB> >& inGDB, ofstream& o, bool TILT);
void PS_GDB_DATA (const vector <GDB>& inGDB, ofstream& o, const CENTER& center);

size_t anything_to_plot_on_ps (const vector <vector <vector <GDB> > >& inGDB_G);

vector <vector < vector <GDB> > > prepare_GDB_G_for_NO_multiple_groups (const vector <vector <GDB> >& in_GDB_G);
vector <vector < vector <GDB> > > prepare_GDB_G_for_multiple_groups (const vector <vector <GDB> >& in_GDB_G);
void OUTPUT_TO_PS (const vector <vector <GDB> >& in_GDB_G, const vector <vector <GDB> >& t_GDB_G);
void DUMP_TO_PS (const vector <vector <vector <GDB> > >& in_GDB_G, const vector <vector <vector <GDB> > >& t_GDB_G);

#endif
