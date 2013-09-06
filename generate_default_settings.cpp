// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <string>
#include <vector>

#include "generate_default_settings.hpp"

using namespace std;

vector < vector < string > > return_dataconvention () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("DATARULE:");
	buf2.push_back("  - Data convention:");
	out.push_back (buf2);
	buf2.clear ();

	buf2.push_back ("G");
	buf2.push_back ("  - Data convention.............................: german dipdir-dip");
	buf2.push_back ("    - german with dip direction.............................[G],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("R");
	buf2.push_back ("  - Data convention ............................: right-hand rule");
	buf2.push_back ("    - or right-hand rule with strike........................[r]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_plot () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("PLOT:");
	buf2.push_back("  - Plot type:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("A");
	buf2.push_back ("  - Plot type ..................................: Angelier plot");
	buf2.push_back ("    - Angelier plot with planes.............................[A],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("H");
	buf2.push_back ("  - Plot type ..................................: Hoeppener plot");
	buf2.push_back ("    - or Hoeppener plot with poles..........................[h]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_plottype () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("PLOTTYPE:");
	buf2.push_back("  - Net type:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("S");
	buf2.push_back ("  - Net type....................................: Schmidt-net");
	buf2.push_back ("    - equal are Schmidt-net.................................[S],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("W");
	buf2.push_back ("  - Net type....................................: Wulff-net");
	buf2.push_back ("    - or equal angle Wulff-net..............................[w]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_hemisphere () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("HEMISPHERE:");
	buf2.push_back("  - Hemisphere:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("L");
	buf2.push_back ("  - Hemisphere..................................: lower hemisphere");
	buf2.push_back ("    - lower hemisphere......................................[L],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("U");
	buf2.push_back ("  - Hemisphere..................................: upper hemisphere");
	buf2.push_back ("    - or upper hemisphere...................................[u]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_tilting () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("TILTING:");
	buf2.push_back("  - Correction of measured data with:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("A");
	buf2.push_back ("  - Tilting by..................................: bedding and paleo-north");
	buf2.push_back ("    - bedding and paleo-north direction.....................[A],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("B");
	buf2.push_back ("  - Tilting by..................................: bedding");
	buf2.push_back ("    - bedding...............................................[b],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("P");
	buf2.push_back ("  - Tilting by..................................: paleo-north direction");
	buf2.push_back ("    - or paleo-north direction..............................[p]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_group () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("GROUP:");
	buf2.push_back("  - Using group codes:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("Y");
	buf2.push_back ("  - Groupcode...................................: use");
	buf2.push_back ("    - use and evaluate groups independently.................[Y],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Groupcode...................................: do not use");
	buf2.push_back ("    - or do not use and evaluate groups together............[n]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_clusternumber () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("CLUSTERNUMBER:");
	buf2.push_back("  - Clustering:");

	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Clustering..................................: do not use");
	buf2.push_back ("    - no clustering.........................................[N],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("A");
	buf2.push_back ("  - Clustering..................................: iteration for ideal cluster number");
	buf2.push_back ("    - automatic cluster number..............................[a],  ");
	out.push_back(buf2);
	buf2.clear();

	/*
	buf2.push_back ("999");
	buf2.push_back ("  - Clustering..................................: ");
	buf2.push_back (" clusters");
	buf2.push_back ("    - or 2..9 clusters...................................[2..9]?  ");
	buf2.push_back ("");
	buf2.push_back ("2");
	buf2.push_back ("9");
	out.push_back(buf2);
	*/


	buf2.push_back ("999");
	buf2.push_back ("2");
	buf2.push_back ("9");
	buf2.push_back ("  - Clustering..................................: ");
	buf2.push_back (" clusters");
	buf2.push_back ("    - or 2..9 clusters...................................[2..9]?  ");
	buf2.push_back ("");
	out.push_back(buf2);


	return out;
}

vector < vector <string> > return_labeling () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("LABELING:");
	buf2.push_back("  - Labeling of stereonet:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Labeling....................................: do not label");
	buf2.push_back ("    - none..................................................[N],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("Y");
	buf2.push_back ("  - Labeling....................................: yes");
	buf2.push_back ("    - or labeling of measured data using data DATA_ID.......[y]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_inversion () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("INVERSION:");
	buf2.push_back("  - Inversion of slickenside data:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("D");
	buf2.push_back ("  - Inversion...................................: using Sprang (1972) method");
	buf2.push_back ("    - regression using Sprang's (1972) NDA method...........[D],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("P");
	buf2.push_back ("  - Inversion...................................: using Turner's (1953) method");
	buf2.push_back ("    - regression using Turner's (1953) PTN method...........[p],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("M");
	buf2.push_back ("  - Inversion...................................: using using Michael's (1984) method");
	buf2.push_back ("    - regression using Michael's (1984) method..............[m],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("S");
	buf2.push_back ("  - Inversion...................................: using Shan et al's (2003) method");
	buf2.push_back ("    - regression using Shan et al's (2003) method...........[s],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("F");
	buf2.push_back ("  - Inversion...................................: using Fry's (1999) method");
	buf2.push_back ("    - regression using Fry's (1999) method..................[f],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("A");
	buf2.push_back ("  - Inversion...................................: using Angelier's (1990) method");
	buf2.push_back ("    - inversion using Angelier's (1984) method..............[a],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("O");
	buf2.push_back ("  - Inversion...................................: using Mostafa's (2005) method");
	buf2.push_back ("    - inversion Mostafa's (2005) method.....................[o],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Inversion...................................: none");
	buf2.push_back ("    - or none...............................................[n]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_RUP_clustering () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("RUP_CLUSTERING:");
	buf2.push_back("  - Clustering using stress estimators:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Clustering using RUP / ANG values...........: no clustering");
	buf2.push_back ("    - do not use stress estimators..........................[N],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("A");
	buf2.push_back ("  - Clustering using RUP / ANG values...........: use ANG values");
	buf2.push_back ("    - use angular misfit (ANG) for clustering...............[a],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("R");
	buf2.push_back ("  - Clustering using RUP / ANG values...........: use RUP values");
	buf2.push_back ("    - or relative upsilon (RUP) for clustering..............[r]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_virtual () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("VIRTUAL:");
	buf2.push_back("  - Virtual symmetric striae set:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Virtual symmetrical striae set..............: do not use");
	buf2.push_back ("    - do not generate virtual symmetric set.................[N],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("Y");
	buf2.push_back ("  - Virtual symmetrical striae set..............: use");
	buf2.push_back ("    - or use virtual symmetric striae set...................[y]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_idealmovement () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("IDEALMOVEMENT:");
	buf2.push_back("  - Ideal movement display for slickensides:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Ideal slickenside movement..................: do not display");
	buf2.push_back ("    - do not display........................................[N],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("Y");
	buf2.push_back ("  - Ideal slickenside movement..................: display");
	buf2.push_back ("    - or display............................................[y]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_stressangle () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("STRESSANGLE:");
	buf2.push_back("  - Angle between s1 and fault plane for regression: ");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("999");
	buf2.push_back ("10");
	buf2.push_back ("80");
	buf2.push_back ("  - Angle in degs between s1 and fault plane....: ");
	buf2.push_back (" degree(s)");
	buf2.push_back ("    - 10 to 80 degrees.................................[10..80]?  ");
	buf2.push_back ("");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_bingham () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("BINGHAM:");
	buf2.push_back("  - Virtual symmetric striae set:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("B");
	buf2.push_back ("  - Fracture statistics.........................: Bingham statistics");
	buf2.push_back ("    - Bingham statistics for weight point calculation.......[B],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Fracture statistics.........................: no statistics");
	buf2.push_back ("    - or none...............................................[n]?  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_linewidth () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("LINEWIDTH:");
	buf2.push_back("  - Linewidth in points (1/72 inch):");
	out.push_back (buf2);
	buf2.clear();

	/*
	buf2.push_back ("999");
	buf2.push_back ("0");
	buf2.push_back ("9");
	buf2.push_back ("  - Linewidth...................................: ");
	buf2.push_back (" points");
	buf2.push_back ("    - 0.1 to 1.0.............................[1...9, 0 for 1.0]?  ");
	buf2.push_back ("0.6");
	out.push_back(buf2);
	*/


	buf2.push_back ("999");
	buf2.push_back ("0");
	buf2.push_back ("9");
	buf2.push_back ("  - Linewidth...................................: ");
	buf2.push_back (" points");
	buf2.push_back ("    - 0.1 to 1.0.............................[1...9, 0 for 1.0]?  ");
	buf2.push_back ("0.6");
	out.push_back(buf2);


	return out;
}

vector < vector <string> > return_rosetype () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("ROSETYPE:");
	buf2.push_back("  - Rose plot for data sets:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("S");
	buf2.push_back ("  - Rose plot type..............................: symmetrical");
	buf2.push_back ("    - symmetrical...........................................[S],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("A");
	buf2.push_back ("  - Rose plot type..............................: asymmetrical");
	buf2.push_back ("    - or asymmetrical.......................................[a]?  ");
	out.push_back(buf2);

	return out;
}

/*
 vector < vector <string> > return_rosedirection () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("ROSEDIRECTION:");
	buf2.push_back("  - Roses are indicating:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("S");
	buf2.push_back ("  - Plot data on rose diagram as:...............: strikes");
	buf2.push_back ("    - strike directions.....................................[S],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("D");
	buf2.push_back ("  - Plot data on rose diagram as................: dips");
	buf2.push_back ("    - dip directions........................................[d],  ");
	out.push_back(buf2);

	return out;
}

 */

vector < vector <string> > return_rosebinning () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("ROSEBINNING:");
	buf2.push_back("  - Bin size on rose plot:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("C");
	buf2.push_back ("  - Data bin size on rose plot..................: 10.0 deg");
	buf2.push_back ("    - 10.0 degrees..........................................[C],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("A");
	buf2.push_back ("  - Data bin size on rose plot..................: 2.5 deg");
	buf2.push_back ("    - 2.5 degrees...........................................[a],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("B");
	buf2.push_back ("  - Data bin size on rose plot..................: 5.0 deg");
	buf2.push_back ("    - 5.0 degrees...........................................[b],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("D");
	buf2.push_back ("  - Data bin size on rose plot..................: 22.5 deg");
	buf2.push_back ("    - 22.5 degrees..........................................[d],  ");
	out.push_back(buf2);
	buf2.clear();

	return out;
}

vector < vector <string> > return_contouring () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("CONTOURING:");
	buf2.push_back("  - Data density contouring on stereonet:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Contour of input data.......................: no contouring");
	buf2.push_back ("    - no contouring.........................................[N],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("S");
	buf2.push_back ("  - Contour of input data.......................: Schmidt 1% method");
	buf2.push_back ("    - Schmidt 1% method.....................................[s],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("K");
	buf2.push_back ("  - Contour of input data.......................: Kamb's (1959) method");
	buf2.push_back ("    - Kamb's (1959) method..................................[k],  ");
	out.push_back(buf2);

	return out;
}

vector < vector <string> > return_grayscale () {

	vector < vector <string> > out;
	vector  <string> buf2;

	buf2.push_back("GRAYSCALE:");
	buf2.push_back("  - Color mode of the grapical output:");
	out.push_back (buf2);
	buf2.clear();

	buf2.push_back ("N");
	buf2.push_back ("  - Grayscale mode..............................: RGB coloured");
	buf2.push_back ("    - coloured output.......................................[N],  ");
	out.push_back(buf2);
	buf2.clear();

	buf2.push_back ("Y");
	buf2.push_back ("  - Grayscale mode..............................: grayscale");
	buf2.push_back ("    - grayscale output......................................[y],  ");
	out.push_back(buf2);

	return out;
}

vector <vector < vector <string> > > return_default_settings_database () {

	vector <vector < vector <string> > > DEF;

	DEF.push_back(return_dataconvention ());
	DEF.push_back(return_plot ());
	DEF.push_back(return_plottype ());
	DEF.push_back(return_hemisphere ());
	DEF.push_back(return_tilting ());
	DEF.push_back(return_group ());
	DEF.push_back(return_clusternumber ());
	DEF.push_back(return_labeling ());
	DEF.push_back(return_inversion ());
	DEF.push_back(return_RUP_clustering ());
	DEF.push_back(return_virtual ());
	DEF.push_back(return_idealmovement ());
	DEF.push_back(return_stressangle ());
	DEF.push_back(return_bingham ());
	DEF.push_back(return_linewidth ());
	DEF.push_back(return_rosetype ());
	DEF.push_back(return_rosebinning ());
	DEF.push_back(return_contouring ());
	DEF.push_back(return_grayscale ());

	return DEF;
}

void dbg_default_settings_database (vector <vector < vector <string> > > DEF) {

	for (size_t i = 0; i < DEF.size(); i++) {
		for (size_t j = 0; j < DEF.at(i).size(); j++) {
			for (size_t k = 0; k < DEF.at(i).at(j).size(); k++) {

				cout << DEF.at(i).at(j).at(k) << endl;;
			}

		}
		cout << "---" << endl;
	}
}





