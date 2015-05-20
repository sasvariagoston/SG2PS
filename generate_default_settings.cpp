
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "assertions.hpp"
#include "common.h"
#include "exceptions.hpp"
#include "generate_default_settings.hpp"
#include "iomanip"
#include "math.h"
#include "run_mode.h"

using namespace std;

void pushbach_settings_item (vector  < vector <string> >& defitem, const string key, const string output) {

	defitem.clear();

	vector < vector <string> > buf;
	vector  <string> buf2;

	buf2.push_back(key);
	buf2.push_back(output);

	defitem.push_back(buf2);
}

void pushbach_settings_option (vector < vector <string> >& defitem, const string value, const string opt1, const string opt2) {

	vector < vector <string> > buf;
	vector  <string> buf2;

	buf2.push_back(value);
	buf2.push_back(opt1);
	buf2.push_back(opt2);

	defitem.push_back(buf2);
}

void pushbach_settings_range (vector <vector <string> >& defitem, const string min, const string max, const string opt1, const string opt2, const string opt3, const string dflt) {

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

vector <vector < vector <string> > > RETURN_ALL_SETTINGS () {

	vector <vector < vector <string> > > DEF;
	vector < vector <string> > defitem;

	const bool G = is_mode_GENERATE_TEST_FILES();

	//-----------------------WELL----------------------------------
	//00


	//pushbach_settings_item (defitem, "WELLDATA:", "  - Do you want to apply well data processing?");
	//pushbach_settings_option (defitem,
	//		"N",
	//		"  - Only process as field/other data.............................",
	//		"    - processing as well data set: .........................[N]?  ");
	/*
	pushbach_settings_option (defitem,
			"Y",
			"  - Process as well data as well.................................",
			"    - processing as well data set...........................[y],  ");

	*/
	//DEF.push_back(defitem);


	//01

	/*
	pushbach_settings_item (defitem, "WELLINTERVAL:", "  - Interval calculation for well data processing by metre or by data number?");
	pushbach_settings_option (defitem,
			"M",
			"  - Well data processing interval by meter.......................",
			"    - Well data processing interval by meter................[M],  ");
	pushbach_settings_option (defitem,
			"D",
			"  - Well data processing interval by data number.................",
			"    - well data processing interval by data number..........[d]?  ");
	DEF.push_back(defitem);
	//02
	pushbach_settings_item (defitem, "WELLINTERVAL_LENGTH:", "  - Well data processing interval length: ");
	if (!G) pushbach_settings_range (defitem,
			"1", "5000",
			"  - Length of a processed interval....: ",
			" meters / data number",
			"    - 1 to 50000 meter / data number.................................[1..5000]?  ",
			"");
	else pushbach_settings_option (defitem,"100","","");
	DEF.push_back(defitem);

	*/
	//03

	/*
	pushbach_settings_item (defitem, "WELLINTERVAL_MIDDLE:", "  - Interval mid point calculation?");
	pushbach_settings_option (defitem,
			"M",
			"  - Well interval mid point: median..............................",
			"    - well interval mid point: median.......................[M],  ");
	pushbach_settings_option (defitem,
			"A",
			"  - Well interval mid point: interval mid point..................",
			"    - well interval mid point: interval mid point...........[a]?  ");
	DEF.push_back(defitem);

	*/
	//-----------------------PLOT----------------------------------
	//04
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

	//05
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

	//06
	pushbach_settings_item (defitem, "PLOTTYPE:", "  - Projection:");
	pushbach_settings_option (defitem,
			"S",
			"  - Projection..................................: Schmidt-net",
			"    - equal are Schmidt-net.................................[S],  ");
	pushbach_settings_option (defitem,
			"W",
			"  - Projection..................................: Wulff-net",
			"    - or equal angle Wulff-net..............................[w]?  ");
	DEF.push_back(defitem);

	//07
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

	//08
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

	//-----------------------AVERAGE AND CORRECTIONS----------------------------------
	//xx
	//pushbach_settings_item (defitem, "AVCALCMETHOD:", "  - Average calculation methodology:");
	//pushbach_settings_option (defitem,
	//		"O",
	//		"  - Average calculation.........................: outcrop data",
	//		"    - outcrop data..........................................[O],  ");
	//pushbach_settings_option (defitem,
	//		"A",
	//		"  - Average calculation.........................: outcrop and active group data",
	//		"    - outcrop and active group data.........................[a],  ");
	//DEF.push_back(defitem);
	//
	//09
	pushbach_settings_item (defitem, "TILTING:", "  - Bedding and paleo North correction:");
	pushbach_settings_option (defitem,
			"A",
			"  - Correct by..................................: bedding and paleo-north",
			"    - bedding and paleo-north direction.....................[A],  ");
	pushbach_settings_option (defitem,
			"B",
			"  - Correct by..................................: bedding",
			"    - bedding...............................................[b],  ");
	pushbach_settings_option (defitem,
			"P",
			"  - Correct by..................................: paleo-north direction",
			"    - or paleo-north direction..............................[p]?  ");
	DEF.push_back(defitem);

	//-----------------------INVERSION----------------------------------
	//10
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

	//11
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

	//12
	pushbach_settings_item (defitem, "STRESSANGLE:", "  - Angle between s1 and fault plane for regression: ");
	if (!G) pushbach_settings_range (defitem,
			"10", "80",
			"  - Angle in degs between s1 and fault plane....: ",
			" degree(s)",
			"    - 10 to 80 degrees.................................[10..80]?  ",
			"");
	else pushbach_settings_option (defitem,"30","","");
	DEF.push_back(defitem);
	//13
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
	//-----------------------DATA GROUP MANAGEMENT----------------------------------
	//14
	pushbach_settings_item (defitem, "INPUTGROUP:", "  - How to import input file groups (if more then one available):");
	pushbach_settings_option (defitem,
			"N",
			"  - Don't import any record",
			"    - don't import any record...............................[N],  ");
	pushbach_settings_option (defitem,
			"F",
			"  - Import first record (used for group codes)",
			"    - import first record (used for group codes)............[f],  ");
	pushbach_settings_option (defitem,
			"S",
			"  - Import second record(used for clustering)",
			"    - import second record (used for clustering)............[s]?  ");
	pushbach_settings_option (defitem,
			"T",
			"  - Import third record (used for RUP/ANG clustering)",
			"    - import third record (used for RUP/ABG clustering).....[t]?  ");
	DEF.push_back(defitem);

	//15
	pushbach_settings_item (defitem, "GROUP:", "  - Groups from input file:");
	pushbach_settings_option (defitem,
			"Y",
			"  - Use initial groups of input file...........: yes",
			"    - use initial groups of the input file..................[Y],  ");
	pushbach_settings_option (defitem,
			"N",
			"  - Use initial groups of input file...........: do not use",
			"    - do not use initial groups of the input file...........[n]?  ");
	DEF.push_back(defitem);

	//16
	pushbach_settings_item (defitem, "CLUSTERNUMBER:", "  - Groups from k-means clustering:");
	pushbach_settings_option (defitem,
			"N",
			"  - Clustering..................................: do not use",
			"    - no clustering.........................................[N],  ");
	pushbach_settings_option (defitem,
			"A",
			"  - Clustering..................................: iteration for ideal cluster number",
			"    - automatic cluster number..............................[a],  ");
	if (!G) pushbach_settings_range (defitem,
			"2", "9",
			"  - Clustering..................................: ",
			" clusters",
			"    - or 2..9 clusters...................................[2..9]?  ",
			"");

	if (G) pushbach_settings_option (defitem, "2", "", "");
	if (G) pushbach_settings_option (defitem, "3", "", "");
	if (G) pushbach_settings_option (defitem, "4", "", "");
	if (G) pushbach_settings_option (defitem, "5", "", "");
	if (G) pushbach_settings_option (defitem, "6", "", "");
	if (G) pushbach_settings_option (defitem, "7", "", "");
	if (G) pushbach_settings_option (defitem, "8", "", "");
	if (G) pushbach_settings_option (defitem, "9", "", "");

	DEF.push_back(defitem);

	//17
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

	//18
	pushbach_settings_item (defitem, "GROUPSEPARATION:", "  - How to separate?:");
	pushbach_settings_option (defitem,
			"I",
			"  - Ignore all group codes",
			"    - ignore all group codes................................[I],  ");
	pushbach_settings_option (defitem,
			"G",
			"  - Use group code to separate",
			"    - Use group code to seaparate...........................[g]?  ");
	pushbach_settings_option (defitem,
			"K",
			"  - Use k-means group to separate",
			"    - Use k-means group to separate.........................[k]?  ");
	pushbach_settings_option (defitem,
			"R",
			"  - Use RUP/ANG group to separate",
			"    - Use RUP/ANG group to separate.........................[r]?  ");
	DEF.push_back(defitem);
	//-----------------------ROSE DIAGRAMS----------------------------------
	//19
	pushbach_settings_item (defitem, "ROSETYPE:", "  - Rose diagram type:");
	pushbach_settings_option (defitem,
			"S",
			"  - Rose plot type..............................: symmetrical",
			"    - symmetrical...........................................[S],  ");
	pushbach_settings_option (defitem,
			"A",
		"  - Rose plot type..............................: asymmetrical",
			"    - or asymmetrical.......................................[a]?  ");
	DEF.push_back(defitem);

	//20
	pushbach_settings_item (defitem, "ROSEDIRECTION:", "  - Rose diagram for:");
	pushbach_settings_option (defitem,
			"S",
			"  - Rose diagram for............................: strikes",
			"    - strike directions.....................................[S],  ");
	pushbach_settings_option (defitem,
			"D",
			"  - Rose diagram for............................: dips",
			"    - dip directions........................................[d],  ");
	DEF.push_back(defitem);

	//21
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

	//-----------------------ROSE DIAGRAMS----------------------------------
	//22
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

	//23
	pushbach_settings_item (defitem, "LABELING:", "  - Labelling on stereonet:");
	pushbach_settings_option (defitem,
			"N",
			"  - Labelling....................................: do not label",
			"    - none..................................................[N],  ");
	pushbach_settings_option (defitem,
			"Y",
			"  - Labelling....................................: yes",
			"    - or labelling of measured data using data DATA_ID......[y]?  ");
	DEF.push_back(defitem);

	//24
	pushbach_settings_item (defitem, "LINEWIDTH:", "  - Linewidth in points (1/72 inch):");
	if (!G) pushbach_settings_range (defitem,
			"0", "10",
			"  - Linewidth...................................: ",
			" points",
			"    - 0.1 to 1.0.............................[1...9, 0 for 1.0]?  ",
			"6");
	if (G) pushbach_settings_option (defitem,"6","","");  //temp
	DEF.push_back(defitem);

	//25
	pushbach_settings_item (defitem, "COLORING:", "  - How to colour?:");
	pushbach_settings_option (defitem,
			"I",
			"  - Ignore all colour codes",
			"    - ignore all colour codes...............................[I],  ");
	pushbach_settings_option (defitem,
			"C",
			"  - Use color codes",
			"    - Use colour codes......................................[c]?  ");
	pushbach_settings_option (defitem,
			"G",
			"  - Use group code to colour",
			"    - Use group code to colour..............................[g]?  ");
	pushbach_settings_option (defitem,
			"K",
			"  - Use k-means group to colour",
			"    - Use k-means group to colour...........................[k]?  ");
	pushbach_settings_option (defitem,
			"R",
			"  - Use RUP/ANG group to colour",
			"    - Use RUP/ANG group to colour...........................[r]?  ");
	DEF.push_back(defitem);
	//26
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

void dbg_default_settings_database (const vector <vector < vector <string> > >& DEF) {

	for (size_t i = 0; i < DEF.size(); i++) {
		for (size_t j = 0; j < DEF.at(i).size(); j++) {
			for (size_t k = 0; k < DEF.at(i).at(j).size(); k++) {

				cout << DEF.at(i).at(j).at(k) << endl;;
			}
		}
		cout << "---" << endl;
	}
}

void dump_keys_values_into_settings_file (const vector <string>& KEY, const vector <string>& VAL, const string FN) {

	if (KEY.size() != VAL.size()) ASSERT_DEAD_END();

	ofstream o;

	o.open ((FN + ".set").c_str());

	for (size_t i = 0; i < KEY.size(); i++) o << KEY.at(i) << '\t' << VAL.at(i) << endl;

	return;
}

void dbg_generate_settings_file_list () {

	if (!is_mode_GENERATE_TEST_FILES()) return;

	vector <vector <vector <string> > > DEF = RETURN_ALL_SETTINGS ();

	const size_t D = DEF.size();
	const size_t STP = 5;

	for (size_t i = 0; i < D - STP + 1; i++) {

		for (size_t c1 = 1; c1 < DEF.at(i+0).size(); c1++) {
			for (size_t c2 = 1; c2 < DEF.at(i+1).size(); c2++) {
				for (size_t c3 = 1; c3 < DEF.at(i+2).size(); c3++) {
					for (size_t c4 = 1; c4 < DEF.at(i+3).size(); c4++) {
						for (size_t c5 = 1; c5 < DEF.at(i+4).size(); c5++) {

							vector <string> KEY, VAL;

							string FN, val;

							for (size_t u = 0; u < i; u++) {
								val = DEF.at(u).at(1).at(0);
								KEY.push_back (DEF.at(u).at(0).at(0));
								VAL.push_back (val);
								FN = FN + val;
							}

							val = DEF.at(i+0).at(c1).at(0);
							KEY.push_back(DEF.at(i+0).at(0).at(0));
							VAL.push_back (val);
							FN = FN + val;

							val = DEF.at(i+1).at(c2).at(0);
							KEY.push_back(DEF.at(i+1).at(0).at(0));
							VAL.push_back (val);
							FN = FN + val;

							val = DEF.at(i+2).at(c3).at(0);
							KEY.push_back(DEF.at(i+2).at(0).at(0));
							VAL.push_back (val);
							FN = FN + val;

							val = DEF.at(i+3).at(c4).at(0);
							KEY.push_back(DEF.at(i+3).at(0).at(0));
							VAL.push_back (val);
							FN = FN + val;

							val = DEF.at(i+4).at(c5).at(0);
							KEY.push_back(DEF.at(i+4).at(0).at(0));
							VAL.push_back (val);
							FN = FN + val;

							for (size_t v = i+5; v < DEF.size(); v++) {
								val = DEF.at(v).at(1).at(0);
								KEY.push_back (DEF.at(v).at(0).at(0));
								VAL.push_back (val);
								FN = FN + val;
							}

							cout << FN << ".SET file has been generated. " << endl;

							dump_keys_values_into_settings_file (KEY, VAL, FN);
						}
					}
				}
			}
		}
	}
	throw exit_requested ();
}
