// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef PS_HPP_
#define PS_HPP_

#include <string>
#include <vector>

#include "structs.h"

using namespace std;

PAPER PS_dimensions ();

void PS_header (string DATATYPE, INPSET inset, string LOC, ofstream& o);
void PS_border (GDB inGDB, ofstream& o, INPSET inset, PAPER P);
void PS_stress_scale (ofstream& o, INPSET inset, PAPER P);
void ps_clusteringresult (ofstream& o, INPSET inset, PAPER P, int clusternumber);

void PS_net (ofstream& o, INPSET inset, PAPER P);
void PS_stressdata (ofstream& o, CENTER center, PAPER P, STRESSFIELD sf, string method);
void PS_stressarrows (ofstream& o, CENTER center, PAPER P, STRESSFIELD sf);

void PS_mohr_circle (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER mohr_center, PAPER P, STRESSFIELD sf, STRESSTENSOR st, bool compression_positive);

bool by_border (const BRDR& x, const BRDR& y);
vector <BRDR> sort_by_border (vector <BRDR> in);

vector <line_RUP_ANG> generate_graph_histogram (vector <HISTOGRAM> H, vector <VALLEY> V, INPSET inset, string method, double DATA_MIN, double DATA_MAX);

void PS_RUP_ANG_distribution (vector <GDB> inGDB, INPSET inset, vector <VALLEY> V, ofstream& o, CENTER center, PAPER P, string method);

//void PS_RUP_distribution (vector <GDB> inGDB, INPSET inset, vector <VALLEY> V, ofstream& o, CENTER center, PAPER P);
//void PS_ANG_distribution (vector <GDB> inGDB, ofstream& o, CENTER center, PAPER P);

void PS_stress_state (ofstream& o, PAPER P, CENTER center, STRESSFIELD sf);

void PS_folddata (GDB in, ofstream& o, CENTER center);

void PS_lineation (GDB i, ofstream& o, INPSET inset, CENTER center, STRESSFIELD sf, bool label, string type);
void PS_plane     (GDB i, ofstream& o, INPSET inset, CENTER center, bool label, string type);
void PS_polepoint (GDB i, ofstream& o, INPSET inset, CENTER center, bool label, string type);

void PS_sc_arrow (GDB i, ofstream& o, INPSET inset, CENTER center, VCTR d);
void PS_striaearrow (GDB i, ofstream& o, INPSET inset, CENTER center, bool label);

void PS_getstereonet (ofstream& o, INPSET inset, CENTER center);

//void PS_rosesegment (ofstream& o, INPSET inset, CENTER center, double percentage, double degree, bool c_plane);
void PS_rosesegment (ofstream& o, INPSET inset, CENTER center, double percentage, double degree, bool c_plane, bool vertical);
void PS_draw_rose_circle_horizontal (ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent);
void PS_draw_rose_circle_vertical (ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent);

void PS_datanumber_averagebedding (GDB i, ofstream& o, INPSET inset, PAPER P, CENTER center, size_t j);

void PS_DRAW_PTN (GDB i, ofstream& o, INPSET inset, CENTER center);
void PS_DRAW_plane (GDB i, ofstream& o, INPSET inset, CENTER center);
void PS_DRAW_lineation (GDB i, ofstream& o, INPSET inset, CENTER center);
void PS_DRAW_striae (GDB i, ofstream& o, INPSET inset, CENTER center);
void PS_DRAW_sc (GDB i, ofstream& o, INPSET inset, CENTER center);

void PS_idealmovement (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);

void PS_DRAW_record (GDB i, ofstream& o, INPSET inset, CENTER center);

void PS_SYMBOLS_border (ofstream& o, PAPER P);
void PS_SYMBOL_draw_plane (double X, double Y, ofstream& o, INPSET inset, PAPER P, string type, string group);

void PS_SYMBOLS_STRIAE (ofstream& o, PAPER P);
void PS_SYMBOLS_SC (ofstream& o, INPSET inset, PAPER P);
void PS_SYMBOLS_PLANE (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P);
void PS_SYMBOLS_LINEATION (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P);
void PS_SYMBOLS_HOEPPNER (ofstream& o, PAPER P);

void PS_SYMBOLS_INVERSION (ofstream& o, PAPER P);
void PS_SYMBOLS_GROUPS (ofstream& o, INPSET inset, PAPER P);
void PS_SYMBOLS_BINGHAM (ofstream& o, PAPER P);
void PS_SYMBOLS_ROSE (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P);
void PS_SYMBOLS_LABEL (ofstream& o, INPSET inset, PAPER P);

void PS_SYMBOLS (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P);

void ps_plot_densities (DENSITY dens, size_t radius, ofstream&  o, INPSET inset, CENTER center, PAPER P);

#endif
