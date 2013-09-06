// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef GENERATE_DEFAULT_SETTINGS_HPP_
#define GENERATE_DEFAULT_SETTINGS_HPP_

#include <string>
#include <vector>

using namespace std;

vector < vector <string> > return_dataconvention ();
vector < vector <string> > return_plot ();
vector < vector <string> > return_plottype ();
vector < vector <string> > return_hemisphere ();
vector < vector <string> > return_tilting ();
vector < vector <string> > return_group ();
vector < vector <string> > return_clusternumber ();
vector < vector <string> > return_labeling ();
vector < vector <string> > return_inversion ();
vector < vector <string> > return_RUP_clustering ();
vector < vector <string> > return_virtual ();
vector < vector <string> > return_idealmovement ();
vector < vector <string> > return_stressangle ();
vector < vector <string> > return_bingham ();
vector < vector <string> > return_linewidth ();
vector < vector <string> > return_rosetype ();
//vector < vector <string> > return_rosedirection ()
vector < vector <string> > return_rosebinning ();
vector < vector <string> > return_contouring ();
vector < vector <string> > return_grayscale ();

vector <vector < vector <string> > > return_default_settings_database ();

void dbg_default_settings_database (vector <vector < vector <string> > > DEF);

#endif
