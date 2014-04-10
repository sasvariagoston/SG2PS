// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "generate_default_settings.hpp"

using namespace std;

void pushbach_settings_item (vector  < vector <string> >& defitem, string key, string output) {

	defitem.clear();

	vector < vector <string> > buf;
	vector  <string> buf2;

	buf2.push_back(key);
	buf2.push_back(output);

	defitem.push_back(buf2);
}

void pushbach_settings_option (vector < vector <string> >& defitem, string value, string opt1, string opt2) {

	vector < vector <string> > buf;
	vector  <string> buf2;

	buf2.push_back(value);
	buf2.push_back(opt1);
	buf2.push_back(opt2);

	defitem.push_back(buf2);
}

void pushbach_settings_range (vector <vector <string> >& defitem, string min, string max, string opt1, string opt2, string opt3, string dflt) {

	vector < vector <string> > buf;
	vector  <string> buf2;

	buf2.push_back("999");
	buf2.push_back(min);
	buf2.push_back(max);

	buf2.push_back(opt1);
	buf2.push_back(opt2);
	buf2.push_back(opt3);

	buf2.push_back(dflt);

	defitem.push_back(buf2);
}

vector <vector < vector <string> > > return_default_settings_database () {

	vector <vector < vector <string> > > DEF;
	vector < vector <string> > defitem;


	pushbach_settings_item (defitem, "DATARULE:", "  - Data convention:");
	pushbach_settings_option (defitem,
			"G",
			"  - Data convention.............................: german dipdir-dip",
			"    - german with dip direction.............................[G],  ");
	pushbach_settings_option (defitem,
			"R",
			"  - Data convention ............................: right-hand rule",
			"    - or right-hand rule with strike........................[r]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "PLOT:", "  - Plot type:");
	pushbach_settings_option (defitem,
			"A",
			"  - Plot type ..................................: Angelier plot",
			"    - Angelier plot with planes.............................[A],  ");
	pushbach_settings_option (defitem,
			"H",
			"  - Plot type ..................................: Hoeppener plot",
			"    - or Hoeppener plot with poles..........................[h]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "PLOTTYPE:", "  - Net type:");
	pushbach_settings_option (defitem,
			"S",
			"  - Net type....................................: Schmidt-net",
			"    - equal are Schmidt-net.................................[S],  ");
	pushbach_settings_option (defitem,
			"W",
			"  - Net type....................................: Wulff-net",
			"    - or equal angle Wulff-net..............................[w]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem,"HEMISPHERE:", "  - Hemisphere:");
	pushbach_settings_option (defitem,
			"L",
			"  - Hemisphere..................................: lower hemisphere",
			"    - lower hemisphere......................................[L],  ");
	pushbach_settings_option (defitem,
			"U",
			"  - Hemisphere..................................: upper hemisphere",
			"    - or upper hemisphere...................................[u]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "TILTING:", "  - Correction of measured data with:");
	pushbach_settings_option (defitem,
			"A",
			"  - Tilting by..................................: bedding and paleo-north",
			"    - bedding and paleo-north direction.....................[A],  ");
	pushbach_settings_option (defitem,
			"B",
			"  - Tilting by..................................: bedding",
			"    - bedding...............................................[b],  ");
	pushbach_settings_option (defitem,
			"P",
			"  - Tilting by..................................: paleo-north direction",
			"    - or paleo-north direction..............................[p]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "GROUP:", "  - Using group codes:");
	pushbach_settings_option (defitem,
			"Y",
			"  - Groupcode...................................: use",
			"    - use and evaluate groups independently.................[Y],  ");
	pushbach_settings_option (defitem,
			"N",
			"  - Groupcode...................................: do not use",
			"    - or do not use and evaluate groups together............[n]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "CLUSTERNUMBER:", "  - Clustering:");
	pushbach_settings_option (defitem,
			"N",
			"  - Clustering..................................: do not use",
			"    - no clustering.........................................[N],  ");
	pushbach_settings_option (defitem,
			"A",
			"  - Clustering..................................: iteration for ideal cluster number",
			"    - automatic cluster number..............................[a],  ");


	pushbach_settings_range (defitem,
			"2", "9",
			"  - Clustering..................................: ",
			" clusters",
			"    - or 2..9 clusters...................................[2..9]?  ",
			"");


	/*
	pushbach_settings_option (defitem, "2", "", "");
	pushbach_settings_option (defitem, "3", "", "");
	pushbach_settings_option (defitem, "4", "", "");
	pushbach_settings_option (defitem, "5", "", "");
	pushbach_settings_option (defitem, "6", "", "");
	pushbach_settings_option (defitem, "7", "", "");
	pushbach_settings_option (defitem, "8", "", "");
	pushbach_settings_option (defitem, "9", "", "");
	*/

	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "LABELING:", "  - Labeling of stereonet:");
	pushbach_settings_option (defitem,
			"N",
			"  - Labeling....................................: do not label",
			"    - none..................................................[N],  ");
	pushbach_settings_option (defitem,
			"Y",
			"  - Labeling....................................: yes",
			"    - or labeling of measured data using data DATA_ID.......[y]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "INVERSION:", "  - Inversion of slickenside data:");
	pushbach_settings_option (defitem,
			"D",
			"  - Inversion...................................: using Sprang (1972) method",
			"    - regression using Sprang's (1972) NDA method...........[D],  ");
	pushbach_settings_option (defitem,
			"P","  - Inversion...................................: using Turner's (1953) method",
			"    - regression using Turner's (1953) PTN method...........[p],  ");
	pushbach_settings_option (defitem,
			"M",
			"  - Inversion...................................: using using Michael's (1984) method",
			"    - regression using Michael's (1984) method..............[m],  ");
	pushbach_settings_option (defitem,
			"S",
			"  - Inversion...................................: using Shan et al's (2003) method",
			"    - regression using Shan et al's (2003) method...........[s],  ");
	pushbach_settings_option (defitem,
			"F",
			"  - Inversion...................................: using Fry's (1999) method",
			"    - regression using Fry's (1999) method..................[f],  ");
	pushbach_settings_option (defitem,
			"A",
			"  - Inversion...................................: using Angelier's (1990) method",
			"    - inversion using Angelier's (1984) method..............[a],  ");
	pushbach_settings_option (defitem,
			"O",
			"  - Inversion...................................: using Mostafa's (2005) method",
			"    - inversion using Mostafa's (2005) method...............[o],  ");
	pushbach_settings_option (defitem,
			"B",
			"  - Inversion...................................: using brute force iteration",
			"    - inversion using brute force iteration.................[b],  ");
	/*
	pushbach_settings_option (defitem,
			"Y",
			"  - Inversion...................................: using Yamaji's (2000) iteration",
			"    - inversion using Yamaji's (2005) method................[y],  ");
	*/
	pushbach_settings_option (defitem,
			"N",
			"  - Inversion...................................: none",
			"    - or none...............................................[n]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "RUP_CLUSTERING:", "  - Clustering using stress estimators:");
	pushbach_settings_option (defitem,
			"N",
			"  - Clustering using RUP / ANG values...........: no clustering",
			"    - do not use stress estimators..........................[N],  ");
	pushbach_settings_option (defitem,
			"A",
			"  - Clustering using RUP / ANG values...........: use ANG values",
			"    - use angular misfit (ANG) for clustering...............[a],  ");
	pushbach_settings_option (defitem,
			"R",
			"  - Clustering using RUP / ANG values...........: use RUP values",
			"    - or relative upsilon (RUP) for clustering..............[r]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "VIRTUAL:", "  - Virtual symmetric striae set:");
	pushbach_settings_option (defitem,
			"N",
			"  - Virtual symmetrical striae set..............: do not use",
			"    - do not generate virtual symmetric set.................[N],  ");
	pushbach_settings_option (defitem,
			"Y",
			"  - Virtual symmetrical striae set..............: use",
			"    - or use virtual symmetric striae set...................[y]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "IDEALMOVEMENT:", "  - Ideal movement display for slickensides:");
	pushbach_settings_option (defitem,
			"N",
			"  - Ideal slickenside movement..................: do not display",
			"    - do not display........................................[N],  ");
	pushbach_settings_option (defitem,
			"Y",
			"  - Ideal slickenside movement..................: display",
			"    - or display............................................[y]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "STRESSANGLE:", "  - Angle between s1 and fault plane for regression: ");


	pushbach_settings_range (defitem,
			"10", "80",
			"  - Angle in degs between s1 and fault plane....: ",
			" degree(s)",
			"    - 10 to 80 degrees.................................[10..80]?  ",
			"");

	//pushbach_settings_option (defitem,"30","","");  //temp
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "BINGHAM:", "  - Virtual symmetric striae set:");
	pushbach_settings_option (defitem,
			"B",
			"  - Fracture statistics.........................: Bingham statistics",
			"    - Bingham statistics for weight point calculation.......[B],  ");
	pushbach_settings_option (defitem,
			"N",
			"  - Fracture statistics.........................: no statistics",
			"    - or none...............................................[n]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "LINEWIDTH:", "  - Linewidth in points (1/72 inch):");


	pushbach_settings_range (defitem,
			"0", "10",
			"  - Linewidth...................................: ",
			" points",
			"    - 0.1 to 1.0.............................[1...9, 0 for 1.0]?  ",
			"6");

	//pushbach_settings_option (defitem,"6","","");  //temp
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "ROSETYPE:", "  - Rose plot for data sets:");
	pushbach_settings_option (defitem,
			"S",
			"  - Rose plot type..............................: symmetrical",
			"    - symmetrical...........................................[S],  ");
	pushbach_settings_option (defitem,
			"A",
			"  - Rose plot type..............................: asymmetrical",
			"    - or asymmetrical.......................................[a]?  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "ROSEDIRECTION:", "  - Roses are indicating:");
	pushbach_settings_option (defitem,
			"S",
			"  - Plot data on rose diagram as:...............: strikes",
			"    - strike directions.....................................[S],  ");
	pushbach_settings_option (defitem,
			"D",
			"  - Plot data on rose diagram as................: dips",
			"    - dip directions........................................[d],  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "ROSEBINNING:", "  - Bin size on rose plot:");
	pushbach_settings_option (defitem,
			"C",
			"  - Data bin size on rose plot..................: 10.0 deg",
			"    - 10.0 degrees..........................................[C],  ");
	pushbach_settings_option (defitem,
			"A",
			"  - Data bin size on rose plot..................: 2.5 deg",
			"    - 2.5 degrees...........................................[a],  ");
	pushbach_settings_option (defitem,
			"B",
			"  - Data bin size on rose plot..................: 5.0 deg",
			"    - 5.0 degrees...........................................[b],  ");
	pushbach_settings_option (defitem,
			"D",
			"  - Data bin size on rose plot..................: 22.5 deg",
			"    - 22.5 degrees..........................................[d],  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "CONTOURING:", "  - Data density contouring on stereonet:");
	pushbach_settings_option (defitem,
			"N",
			"  - Contour of input data.......................: no contouring",
			"    - no contouring.........................................[N],  ");
	pushbach_settings_option (defitem,
			"D",
			"  - Contour of input data.......................: Dip direction / bearing",
			"    - Dip direction / bearing...............................[d],  ");
	pushbach_settings_option (defitem,
			"S",
			"  - Contour of input data.......................: Strike direction / bearing",
			"    - Strike direction / bearing............................[s],  ");
	pushbach_settings_option (defitem,
			"O",
			"  - Contour of input data.......................: Plane normal / bearing",
			"    - Plane normal / bearing................................[o],  ");
	pushbach_settings_option (defitem,
			"B",
			"  - Contour of input data.......................: Striae bearing / bearing",
			"    - Striae bearing / bearing..............................[b],  ");
	DEF.push_back(defitem);

	pushbach_settings_item (defitem, "GRAYSCALE:", "  - Color mode of the grapical output:");
	pushbach_settings_option (defitem,
			"N", // 0 1 0
			"  - Grayscale mode..............................: RGB coloured",
			"    - coloured output.......................................[N],  ");
	pushbach_settings_option (defitem,
			"Y", // 0 2 0
			"  - Grayscale mode..............................: grayscale",
			"    - grayscale output......................................[y],  ");
	DEF.push_back(defitem);

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

void dbg_generate_settings_file_list () {

	vector <vector <vector <string> > > DEF = return_default_settings_database();





	for (size_t i00 = 1; i00 < DEF.at(0).size(); i00++) {
		for (size_t i01 = 1; i01 < DEF.at(1).size(); i01++) {
			for (size_t i02 = 1; i02 < DEF.at(2).size(); i02++) {
				for (size_t i03 = 1; i03 < DEF.at(3).size(); i03++) {
					for (size_t i04 = 1; i04 < DEF.at(4).size(); i04++) {


						ofstream o;

						string filename =

								DEF.at(0).at(i00).at(0) +
								DEF.at(1).at(i01).at(0) +
								DEF.at(2).at(i02).at(0) +
								DEF.at(3).at(i03).at(0) +
								DEF.at(4).at(i04).at(0) +
								DEF.at(5).at(1).at(0) +
								DEF.at(6).at(1).at(0) +
								DEF.at(7).at(1).at(0) +
								DEF.at(8).at(1).at(0) +
								DEF.at(9).at(1).at(0) +
								DEF.at(10).at(1).at(0) +
								DEF.at(11).at(1).at(0) +
								DEF.at(12).at(1).at(0) +
								DEF.at(13).at(1).at(0) +
								DEF.at(14).at(1).at(0) +
								DEF.at(15).at(1).at(0) +
								DEF.at(16).at(1).at(0) +
								DEF.at(17).at(1).at(0) +
								DEF.at(18).at(1).at(0) +
								DEF.at(19).at(1).at(0);

						o.open ((filename + ".set").c_str());

						o << DEF.at(0).at(0).at(0) << '\t' << DEF.at(0).at(i00).at(0) << endl;
						o << DEF.at(1).at(0).at(0) << '\t' << DEF.at(1).at(i01).at(0) << endl;
						o << DEF.at(2).at(0).at(0) << '\t' << DEF.at(2).at(i02).at(0) << endl;
						o << DEF.at(3).at(0).at(0) << '\t' << DEF.at(3).at(i03).at(0) << endl;
						o << DEF.at(4).at(0).at(0) << '\t' << DEF.at(4).at(i04).at(0) << endl;
						o << DEF.at(5).at(0).at(0) << '\t' << DEF.at(5).at(1).at(0) << endl;
						o << DEF.at(6).at(0).at(0) << '\t' << DEF.at(6).at(1).at(0) << endl;
						o << DEF.at(7).at(0).at(0) << '\t' << DEF.at(7).at(1).at(0) << endl;
						o << DEF.at(8).at(0).at(0) << '\t' << DEF.at(8).at(1).at(0) << endl;
						o << DEF.at(9).at(0).at(0) << '\t' << DEF.at(9).at(1).at(0) << endl;
						o << DEF.at(10).at(0).at(0) << '\t' << DEF.at(10).at(1).at(0) << endl;
						o << DEF.at(11).at(0).at(0) << '\t' << DEF.at(11).at(1).at(0) << endl;
						o << DEF.at(12).at(0).at(0) << '\t' << DEF.at(12).at(1).at(0) << endl;
						o << DEF.at(13).at(0).at(0) << '\t' << DEF.at(13).at(1).at(0) << endl;
						o << DEF.at(14).at(0).at(0) << '\t' << DEF.at(14).at(1).at(0) << endl;
						o << DEF.at(15).at(0).at(0) << '\t' << DEF.at(15).at(1).at(0) << endl;
						o << DEF.at(16).at(0).at(0) << '\t' << DEF.at(16).at(1).at(0) << endl;
						o << DEF.at(17).at(0).at(0) << '\t' << DEF.at(17).at(1).at(0) << endl;
						o << DEF.at(18).at(0).at(0) << '\t' << DEF.at(18).at(1).at(0) << endl;
						o << DEF.at(19).at(0).at(0) << '\t' << DEF.at(19).at(1).at(0) << endl;

						o.close();
					}
				}
			}
		}
	}

	for (size_t i05 = 1; i05 < DEF.at(5).size(); i05++) {
		for (size_t i06 = 1; i06 < DEF.at(6).size(); i06++) {
			for (size_t i07 = 1; i07 < DEF.at(7).size(); i07++) {
				for (size_t i08 = 1; i08 < DEF.at(8).size(); i08++) {
					for (size_t i09 = 1; i09 < DEF.at(9).size(); i09++) {


						ofstream o;

						string filename =

								DEF.at(0).at(1).at(0) +
								DEF.at(1).at(1).at(0) +
								DEF.at(2).at(1).at(0) +
								DEF.at(3).at(1).at(0) +
								DEF.at(4).at(1).at(0) +
								DEF.at(5).at(i05).at(0) +
								DEF.at(6).at(i06).at(0) +
								DEF.at(7).at(i07).at(0) +
								DEF.at(8).at(i08).at(0) +
								DEF.at(9).at(i09).at(0) +
								DEF.at(10).at(1).at(0) +
								DEF.at(11).at(1).at(0) +
								DEF.at(12).at(1).at(0) +
								DEF.at(13).at(1).at(0) +
								DEF.at(14).at(1).at(0) +
								DEF.at(15).at(1).at(0) +
								DEF.at(16).at(1).at(0) +
								DEF.at(17).at(1).at(0) +
								DEF.at(18).at(1).at(0) +
								DEF.at(19).at(1).at(0);

						o.open ((filename + ".set").c_str());

						o << DEF.at(0).at(0).at(0) << '\t' << DEF.at(0).at(1).at(0) << endl;
						o << DEF.at(1).at(0).at(0) << '\t' << DEF.at(1).at(1).at(0) << endl;
						o << DEF.at(2).at(0).at(0) << '\t' << DEF.at(2).at(1).at(0) << endl;
						o << DEF.at(3).at(0).at(0) << '\t' << DEF.at(3).at(1).at(0) << endl;
						o << DEF.at(4).at(0).at(0) << '\t' << DEF.at(4).at(1).at(0) << endl;
						o << DEF.at(5).at(0).at(0) << '\t' << DEF.at(5).at(i05).at(0) << endl;
						o << DEF.at(6).at(0).at(0) << '\t' << DEF.at(6).at(i06).at(0) << endl;
						o << DEF.at(7).at(0).at(0) << '\t' << DEF.at(7).at(i07).at(0) << endl;
						o << DEF.at(8).at(0).at(0) << '\t' << DEF.at(8).at(i08).at(0) << endl;
						o << DEF.at(9).at(0).at(0) << '\t' << DEF.at(9).at(i09).at(0) << endl;
						o << DEF.at(10).at(0).at(0) << '\t' << DEF.at(10).at(1).at(0) << endl;
						o << DEF.at(11).at(0).at(0) << '\t' << DEF.at(11).at(1).at(0) << endl;
						o << DEF.at(12).at(0).at(0) << '\t' << DEF.at(12).at(1).at(0) << endl;
						o << DEF.at(13).at(0).at(0) << '\t' << DEF.at(13).at(1).at(0) << endl;
						o << DEF.at(14).at(0).at(0) << '\t' << DEF.at(14).at(1).at(0) << endl;
						o << DEF.at(15).at(0).at(0) << '\t' << DEF.at(15).at(1).at(0) << endl;
						o << DEF.at(16).at(0).at(0) << '\t' << DEF.at(16).at(1).at(0) << endl;
						o << DEF.at(17).at(0).at(0) << '\t' << DEF.at(17).at(1).at(0) << endl;
						o << DEF.at(18).at(0).at(0) << '\t' << DEF.at(18).at(1).at(0) << endl;
						o << DEF.at(19).at(0).at(0) << '\t' << DEF.at(19).at(1).at(0) << endl;

						o.close();
					}
				}
			}
		}
	}

	for (size_t i10 = 1; i10 < DEF.at(10).size(); i10++) {
		for (size_t i11 = 1; i11 < DEF.at(11).size(); i11++) {
			for (size_t i12 = 1; i12 < DEF.at(12).size(); i12++) {
				for (size_t i13 = 1; i13 < DEF.at(13).size(); i13++) {
					for (size_t i14 = 1; i14 < DEF.at(14).size(); i14++) {


						ofstream o;

						string filename =

								DEF.at(0).at(1).at(0) +
								DEF.at(1).at(1).at(0) +
								DEF.at(2).at(1).at(0) +
								DEF.at(3).at(1).at(0) +
								DEF.at(4).at(1).at(0) +
								DEF.at(5).at(1).at(0) +
								DEF.at(6).at(1).at(0) +
								DEF.at(7).at(1).at(0) +
								DEF.at(8).at(1).at(0) +
								DEF.at(9).at(1).at(0) +
								DEF.at(10).at(i10).at(0) +
								DEF.at(11).at(i11).at(0) +
								DEF.at(12).at(i12).at(0) +
								DEF.at(13).at(i13).at(0) +
								DEF.at(14).at(i14).at(0) +
								DEF.at(15).at(1).at(0) +
								DEF.at(16).at(1).at(0) +
								DEF.at(17).at(1).at(0) +
								DEF.at(18).at(1).at(0) +
								DEF.at(19).at(1).at(0);

						o.open ((filename + ".set").c_str());

						o << DEF.at(0).at(0).at(0) << '\t' << DEF.at(0).at(1).at(0) << endl;
						o << DEF.at(1).at(0).at(0) << '\t' << DEF.at(1).at(1).at(0) << endl;
						o << DEF.at(2).at(0).at(0) << '\t' << DEF.at(2).at(1).at(0) << endl;
						o << DEF.at(3).at(0).at(0) << '\t' << DEF.at(3).at(1).at(0) << endl;
						o << DEF.at(4).at(0).at(0) << '\t' << DEF.at(4).at(1).at(0) << endl;
						o << DEF.at(5).at(0).at(0) << '\t' << DEF.at(5).at(1).at(0) << endl;
						o << DEF.at(6).at(0).at(0) << '\t' << DEF.at(6).at(1).at(0) << endl;
						o << DEF.at(7).at(0).at(0) << '\t' << DEF.at(7).at(1).at(0) << endl;
						o << DEF.at(8).at(0).at(0) << '\t' << DEF.at(8).at(1).at(0) << endl;
						o << DEF.at(9).at(0).at(0) << '\t' << DEF.at(9).at(1).at(0) << endl;
						o << DEF.at(10).at(0).at(0) << '\t' << DEF.at(10).at(i10).at(0) << endl;
						o << DEF.at(11).at(0).at(0) << '\t' << DEF.at(11).at(i11).at(0) << endl;
						o << DEF.at(12).at(0).at(0) << '\t' << DEF.at(12).at(i12).at(0) << endl;
						o << DEF.at(13).at(0).at(0) << '\t' << DEF.at(13).at(i13).at(0) << endl;
						o << DEF.at(14).at(0).at(0) << '\t' << DEF.at(14).at(i14).at(0) << endl;
						o << DEF.at(15).at(0).at(0) << '\t' << DEF.at(15).at(1).at(0) << endl;
						o << DEF.at(16).at(0).at(0) << '\t' << DEF.at(16).at(1).at(0) << endl;
						o << DEF.at(17).at(0).at(0) << '\t' << DEF.at(17).at(1).at(0) << endl;
						o << DEF.at(18).at(0).at(0) << '\t' << DEF.at(18).at(1).at(0) << endl;
						o << DEF.at(19).at(0).at(0) << '\t' << DEF.at(19).at(1).at(0) << endl;

						o.close();
					}
				}
			}
		}
	}



	for (size_t i15 = 1; i15 < DEF.at(15).size(); i15++) {
		for (size_t i16 = 1; i16 < DEF.at(16).size(); i16++) {
			for (size_t i17 = 1; i17 < DEF.at(17).size(); i17++) {
				for (size_t i18 = 1; i18 < DEF.at(18).size(); i18++) {
					for (size_t i19 = 1; i19 < DEF.at(19).size(); i19++) {


						ofstream o;

						string filename =

								DEF.at(0).at(1).at(0) +
								DEF.at(1).at(1).at(0) +
								DEF.at(2).at(1).at(0) +
								DEF.at(3).at(1).at(0) +
								DEF.at(4).at(1).at(0) +
								DEF.at(5).at(1).at(0) +
								DEF.at(6).at(1).at(0) +
								DEF.at(7).at(1).at(0) +
								DEF.at(8).at(1).at(0) +
								DEF.at(9).at(1).at(0) +
								DEF.at(10).at(1).at(0) +
								DEF.at(11).at(1).at(0) +
								DEF.at(12).at(1).at(0) +
								DEF.at(13).at(1).at(0) +
								DEF.at(14).at(1).at(0) +
								DEF.at(15).at(i15).at(0) +
								DEF.at(16).at(i16).at(0) +
								DEF.at(17).at(i17).at(0) +
								DEF.at(18).at(i18).at(0) +
								DEF.at(19).at(i19).at(0);

						o.open ((filename + ".set").c_str());

						o << DEF.at(0).at(0).at(0) << '\t' << DEF.at(0).at(1).at(0) << endl;
						o << DEF.at(1).at(0).at(0) << '\t' << DEF.at(1).at(1).at(0) << endl;
						o << DEF.at(2).at(0).at(0) << '\t' << DEF.at(2).at(1).at(0) << endl;
						o << DEF.at(3).at(0).at(0) << '\t' << DEF.at(3).at(1).at(0) << endl;
						o << DEF.at(4).at(0).at(0) << '\t' << DEF.at(4).at(1).at(0) << endl;
						o << DEF.at(5).at(0).at(0) << '\t' << DEF.at(5).at(1).at(0) << endl;
						o << DEF.at(6).at(0).at(0) << '\t' << DEF.at(6).at(1).at(0) << endl;
						o << DEF.at(7).at(0).at(0) << '\t' << DEF.at(7).at(1).at(0) << endl;
						o << DEF.at(8).at(0).at(0) << '\t' << DEF.at(8).at(1).at(0) << endl;
						o << DEF.at(9).at(0).at(0) << '\t' << DEF.at(9).at(1).at(0) << endl;
						o << DEF.at(10).at(0).at(0) << '\t' << DEF.at(10).at(1).at(0) << endl;
						o << DEF.at(11).at(0).at(0) << '\t' << DEF.at(11).at(1).at(0) << endl;
						o << DEF.at(12).at(0).at(0) << '\t' << DEF.at(12).at(1).at(0) << endl;
						o << DEF.at(13).at(0).at(0) << '\t' << DEF.at(13).at(1).at(0) << endl;
						o << DEF.at(14).at(0).at(0) << '\t' << DEF.at(14).at(1).at(0) << endl;
						o << DEF.at(15).at(0).at(0) << '\t' << DEF.at(15).at(i15).at(0) << endl;
						o << DEF.at(16).at(0).at(0) << '\t' << DEF.at(16).at(i16).at(0) << endl;
						o << DEF.at(17).at(0).at(0) << '\t' << DEF.at(17).at(i17).at(0) << endl;
						o << DEF.at(18).at(0).at(0) << '\t' << DEF.at(18).at(i18).at(0) << endl;
						o << DEF.at(19).at(0).at(0) << '\t' << DEF.at(19).at(i19).at(0) << endl;

						o.close();
					}
				}
			}
		}
	}









	/*


	for (size_t i08 = 1; i08 < DEF.at(8).size(); i08++) {
		for (size_t i09 = 1; i09 < DEF.at(9).size(); i09++) {
			for (size_t i10 = 1; i10 < DEF.at(10).size(); i10++) {
				for (size_t i11 = 1; i11 < DEF.at(11).size(); i11++) {
					for (size_t i12 = 1; i12 < DEF.at(12).size(); i12++) {
						for (size_t i13 = 1; i13 < DEF.at(13).size(); i13++) {
							for (size_t i14 = 1; i14 < DEF.at(14).size(); i14++) {
								for (size_t i15 = 1; i15 < DEF.at(15).size(); i15++) {
									for (size_t i16 = 1; i16 < DEF.at(16).size(); i16++) {
										for (size_t i17 = 1; i17 < DEF.at(17).size(); i17++) {
											for (size_t i18 = 1; i18 < DEF.at(18).size(); i18++) {
												for (size_t i19 = 1; i19 < DEF.at(19).size(); i19++) {


													ofstream o;

													string filename =

															DEF.at(0).at(1).at(0) +
															DEF.at(1).at(1).at(0) +
															DEF.at(2).at(1).at(0) +
															DEF.at(3).at(1).at(0) +
															DEF.at(4).at(1).at(0) +
															DEF.at(5).at(1).at(0) +
															DEF.at(6).at(1).at(0) +
															DEF.at(7).at(1).at(0) +
															DEF.at(8).at(i08).at(0) +
															DEF.at(9).at(i09).at(0) +
															DEF.at(10).at(i10).at(0) +
															DEF.at(11).at(i11).at(0) +
															DEF.at(12).at(i12).at(0) +
															DEF.at(13).at(i13).at(0) +
															DEF.at(14).at(i14).at(0) +
															DEF.at(15).at(i15).at(0) +
															DEF.at(16).at(i16).at(0) +
															DEF.at(17).at(i17).at(0) +
															DEF.at(18).at(i18).at(0) +
															DEF.at(19).at(i19).at(0);

													o.open ((filename + ".set").c_str());

													o << DEF.at(0).at(0).at(0) << '\t' << DEF.at(0).at(1).at(0) << endl;
													o << DEF.at(1).at(0).at(0) << '\t' << DEF.at(1).at(1).at(0) << endl;
													o << DEF.at(2).at(0).at(0) << '\t' << DEF.at(2).at(1).at(0) << endl;
													o << DEF.at(3).at(0).at(0) << '\t' << DEF.at(3).at(1).at(0) << endl;
													o << DEF.at(4).at(0).at(0) << '\t' << DEF.at(4).at(1).at(0) << endl;
													o << DEF.at(5).at(0).at(0) << '\t' << DEF.at(5).at(1).at(0) << endl;
													o << DEF.at(6).at(0).at(0) << '\t' << DEF.at(6).at(1).at(0) << endl;
													o << DEF.at(7).at(0).at(0) << '\t' << DEF.at(7).at(1).at(0) << endl;
													o << DEF.at(8).at(0).at(0) << '\t' << DEF.at(8).at(i08).at(0) << endl;
													o << DEF.at(9).at(0).at(0) << '\t' << DEF.at(9).at(i09).at(0) << endl;
													o << DEF.at(10).at(0).at(0) << '\t' << DEF.at(10).at(i10).at(0) << endl;
													o << DEF.at(11).at(0).at(0) << '\t' << DEF.at(11).at(i11).at(0) << endl;
													o << DEF.at(12).at(0).at(0) << '\t' << DEF.at(12).at(i12).at(0) << endl;
													o << DEF.at(13).at(0).at(0) << '\t' << DEF.at(13).at(i13).at(0) << endl;
													o << DEF.at(14).at(0).at(0) << '\t' << DEF.at(14).at(i14).at(0) << endl;
													o << DEF.at(15).at(0).at(0) << '\t' << DEF.at(15).at(i15).at(0) << endl;
													o << DEF.at(16).at(0).at(0) << '\t' << DEF.at(16).at(i16).at(0) << endl;
													o << DEF.at(17).at(0).at(0) << '\t' << DEF.at(17).at(i17).at(0) << endl;
													o << DEF.at(18).at(0).at(0) << '\t' << DEF.at(18).at(i18).at(0) << endl;
													o << DEF.at(19).at(0).at(0) << '\t' << DEF.at(19).at(i19).at(0) << endl;

													o.close();
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}



	for (size_t i00 = 1; i00 < DEF.at(0).size(); i00++) {
		for (size_t i01 = 1; i01 < DEF.at(1).size(); i01++) {
			for (size_t i02 = 1; i02 < DEF.at(2).size(); i02++) {
				for (size_t i03 = 1; i03 < DEF.at(3).size(); i03++) {
					for (size_t i04 = 1; i04 < DEF.at(4).size(); i04++) {
						for (size_t i05 = 1; i05 < DEF.at(5).size(); i05++) {
							for (size_t i06 = 1; i06 < DEF.at(6).size(); i06++) {
								for (size_t i07 = 1; i07 < DEF.at(7).size(); i07++) {

									ofstream o;

									string filename =

											DEF.at(0).at(i00).at(0) +
											DEF.at(1).at(i01).at(0) +
											DEF.at(2).at(i02).at(0) +
											DEF.at(3).at(i03).at(0) +
											DEF.at(4).at(i04).at(0) +
											DEF.at(5).at(i05).at(0) +
											DEF.at(6).at(i06).at(0) +
											DEF.at(7).at(i07).at(0) +
											DEF.at(8).at(1).at(0) +
											DEF.at(9).at(1).at(0) +
											DEF.at(10).at(1).at(0) +
											DEF.at(11).at(1).at(0) +
											DEF.at(12).at(1).at(0) +
											DEF.at(13).at(1).at(0) +
											DEF.at(14).at(1).at(0) +
											DEF.at(15).at(1).at(0) +
											DEF.at(16).at(1).at(0) +
											DEF.at(17).at(1).at(0) +
											DEF.at(18).at(1).at(0) +
											DEF.at(19).at(1).at(0);

									o.open ((filename + ".set").c_str());

									o << DEF.at(0).at(0).at(0) << '\t' << DEF.at(0).at(i00).at(0) << endl;
									o << DEF.at(1).at(0).at(0) << '\t' << DEF.at(1).at(i01).at(0) << endl;
									o << DEF.at(2).at(0).at(0) << '\t' << DEF.at(2).at(i02).at(0) << endl;
									o << DEF.at(3).at(0).at(0) << '\t' << DEF.at(3).at(i03).at(0) << endl;
									o << DEF.at(4).at(0).at(0) << '\t' << DEF.at(4).at(i04).at(0) << endl;
									o << DEF.at(5).at(0).at(0) << '\t' << DEF.at(5).at(i05).at(0) << endl;
									o << DEF.at(6).at(0).at(0) << '\t' << DEF.at(6).at(i06).at(0) << endl;
									o << DEF.at(7).at(0).at(0) << '\t' << DEF.at(7).at(i07).at(0) << endl;
									o << DEF.at(8).at(0).at(0) << '\t' << DEF.at(8).at(1).at(0) << endl;
									o << DEF.at(9).at(0).at(0) << '\t' << DEF.at(9).at(1).at(0) << endl;
									o << DEF.at(10).at(0).at(0) << '\t' << DEF.at(10).at(1).at(0) << endl;
									o << DEF.at(11).at(0).at(0) << '\t' << DEF.at(11).at(1).at(0) << endl;
									o << DEF.at(12).at(0).at(0) << '\t' << DEF.at(12).at(1).at(0) << endl;
									o << DEF.at(13).at(0).at(0) << '\t' << DEF.at(13).at(1).at(0) << endl;
									o << DEF.at(14).at(0).at(0) << '\t' << DEF.at(14).at(1).at(0) << endl;
									o << DEF.at(15).at(0).at(0) << '\t' << DEF.at(15).at(1).at(0) << endl;
									o << DEF.at(16).at(0).at(0) << '\t' << DEF.at(16).at(1).at(0) << endl;
									o << DEF.at(17).at(0).at(0) << '\t' << DEF.at(17).at(1).at(0) << endl;
									o << DEF.at(18).at(0).at(0) << '\t' << DEF.at(18).at(1).at(0) << endl;
									o << DEF.at(19).at(0).at(0) << '\t' << DEF.at(19).at(1).at(0) << endl;

									o.close();
								}
							}
						}
					}
				}
			}
		}
	}

	*/
	return;
}
