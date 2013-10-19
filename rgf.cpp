// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "average.hpp"
#include "allowed_keys.hpp"
#include "assertions.hpp"
#include "bingham.h"
#include "checkrgffilecontent.h"
#include "checkxycontent.h"
#include "data_io.h"
#include "ps.h"
#include "random.hpp"
#include "retilt.hpp"
#include "rgf.h"

using namespace std;

vector <GDB> competeRGFcontect (string projectname, string inputxyfilename, INPSET inset) {

	vector <GDB> outGDB = create_GDB_from_rgf (projectname + ".rgf");

	for (size_t i = 0; i < outGDB.size(); i++) {

		if (outGDB.at(i).GC == "") outGDB.at(i).GC = "X";

		outGDB.at(i).PSCOLOR = complete_colorcode (outGDB.at(i).COLOR);

		if (inset.datarule == "R") outGDB.at(i).corr.DIPDIR = right_hand_rule_to_german (outGDB.at(i).corr.DIPDIR);

		if (capslock(inputxyfilename) != "NONE")  outGDB.at(i) = insertxy (outGDB.at(i));
	}

	return outGDB;
}

string complete_colorcode (string in) {

	if 		(in == "B" || in == "1") 	return "0.00 0.00 1.00";
	else if (in == "C" || in == "2")	return "1.00 0.00 0.67";
	else if (in == "D" || in == "3")	return "1.00 0.00 0.00";
	else if (in == "E" || in == "4")	return "1.00 0.50 0.00";
	else if (in == "F" || in == "5")	return "1.00 1.00 0.00";
	else if (in == "G" || in == "6")	return "0.00 1.00 0.00";
	else if (in == "H" || in == "7")	return "0.67 0.00 0.67";
	else if (in == "I" || in == "8")	return "0.50 1.00 1.00";
	else if (in == "J" || in == "9")	return "0.50 0.50 0.50";
	else 								return "0.00 0.00 0.00";
}

vector <GDB> colorcode_grom_groupcode (vector <GDB> inGDB, INPSET inset) {

	vector <GDB> outGDB = inGDB;

	if (inset.grayscale == "Y") {

		for (size_t i = 0; i < inGDB.size(); i++) {

			if (inGDB.at(i).GC == "X") outGDB.at(i).PSCOLOR = "0.20 0.20 0.20";
			if (inGDB.at(i).GC == "A") outGDB.at(i).PSCOLOR = "0.20 0.20 0.20";
			if (inGDB.at(i).GC == "B") outGDB.at(i).PSCOLOR = "0.20 0.20 0.20";
			if (inGDB.at(i).GC == "C") outGDB.at(i).PSCOLOR = "0.40 0.40 0.40";
			if (inGDB.at(i).GC == "D") outGDB.at(i).PSCOLOR = "0.40 0.40 0.40";
			if (inGDB.at(i).GC == "E") outGDB.at(i).PSCOLOR = "0.40 0.40 0.40";
			if (inGDB.at(i).GC == "F") outGDB.at(i).PSCOLOR = "0.40 0.40 0.40";
			if (inGDB.at(i).GC == "G") outGDB.at(i).PSCOLOR = "0.60 0.60 0.60";
			if (inGDB.at(i).GC == "H") outGDB.at(i).PSCOLOR = "0.60 0.60 0.60";
			if (inGDB.at(i).GC == "I") outGDB.at(i).PSCOLOR = "0.60 0.60 0.60";
		}

		for (size_t i = 0; i < inGDB.size(); i++) {

			if (inGDB.at(i).GC == "X") outGDB.at(i).DASHED = "   ";
			if (inGDB.at(i).GC == "A") outGDB.at(i).DASHED = "3 3";
			if (inGDB.at(i).GC == "B") outGDB.at(i).DASHED = "6 6";
			if (inGDB.at(i).GC == "C") outGDB.at(i).DASHED = "   ";
			if (inGDB.at(i).GC == "D") outGDB.at(i).DASHED = "3 3";
			if (inGDB.at(i).GC == "E") outGDB.at(i).DASHED = "3 6";
			if (inGDB.at(i).GC == "F") outGDB.at(i).DASHED = "6 6";
			if (inGDB.at(i).GC == "G") outGDB.at(i).DASHED = "   ";
			if (inGDB.at(i).GC == "H") outGDB.at(i).DASHED = "3 3";
			if (inGDB.at(i).GC == "I") outGDB.at(i).DASHED = "6 6";
		}
	}
	else {

		for (size_t i = 0; i < inGDB.size(); i++) {

			if (inGDB.at(i).GC == "X") outGDB.at(i).PSCOLOR = "0.00 0.00 0.00";
			if (inGDB.at(i).GC == "A") outGDB.at(i).PSCOLOR = "0.00 0.00 1.00";
			if (inGDB.at(i).GC == "B") outGDB.at(i).PSCOLOR = "1.00 0.00 0.67";
			if (inGDB.at(i).GC == "C") outGDB.at(i).PSCOLOR = "1.00 0.00 0.00";
			if (inGDB.at(i).GC == "D") outGDB.at(i).PSCOLOR = "1.00 0.50 0.00";
			if (inGDB.at(i).GC == "E") outGDB.at(i).PSCOLOR = "1.00 1.00 0.00";
			if (inGDB.at(i).GC == "F") outGDB.at(i).PSCOLOR = "0.00 1.00 0.00";
			if (inGDB.at(i).GC == "G") outGDB.at(i).PSCOLOR = "0.67 0.00 0.67";
			if (inGDB.at(i).GC == "H") outGDB.at(i).PSCOLOR = "0.50 1.00 1.00";
			if (inGDB.at(i).GC == "I") outGDB.at(i).PSCOLOR = "0.50 0.50 0.50";
		}
	}


	return outGDB;
}

vector <GDB> black_colorcode (vector <GDB> inGDB) {

	for (size_t j = 0; j < inGDB.size(); j++) {

		inGDB.at(j).PSCOLOR = "0.00 0.00 0.00";
	}

	return inGDB;
}

vector <GDB> cGc_NDS (vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;

	size_t i = 0;

	do {

		outGDB.at(i).N = declare_vector(999.99, 999.99, 999.99);
		outGDB.at(i).D = declare_vector(999.99, 999.99, 999.99);
		outGDB.at(i).S = declare_vector(999.99, 999.99, 999.99);

		if (outGDB.at(i).DATAGROUP != "NULL") {

			if (inGDB.at(i).corr.DIPDIR == 360.0) inGDB.at(i).corr.DIPDIR = 360.0 - 10e-8;
			if (inGDB.at(i).corr.DIPDIR ==   0.0) inGDB.at(i).corr.DIPDIR =         10e-8;

			if (inGDB.at(i).corr.DIP == 90.0) inGDB.at(i).corr.DIP = 90.0 - 10e-8;
			if (inGDB.at(i).corr.DIP ==  0.0) inGDB.at(i).corr.DIP =        10e-8;

			outGDB.at(i).N = NXNYNZ_from_dipdir_dip (inGDB.at(i).corr);
			outGDB.at(i).D = DXDYDZ_from_dipdir_dip (inGDB.at(i).corr);

			outGDB.at(i).S = crossproduct(outGDB.at(i).N, outGDB.at(i).D);
		}

		i++;
	}

	while (i < outGDB.size());

	cout << "  - Normal, dip and strike vectors were computed for " << i << " records." << endl;

	return outGDB;
}

GDB cGc_NCDCSC_LINEATION_SC (GDB inGDB) {

	GDB outGDB = inGDB;

	outGDB.NC = NXNYNZ_from_dipdir_dip (outGDB.corrL);

	if ((outGDB.N.X == outGDB.NC.X) && (outGDB.N.Y == outGDB.NC.Y) && (outGDB.N.Z == outGDB.NC.Z)) outGDB.N.X = outGDB.N.X + 10e-8;

	outGDB.DC = DXDYDZ_from_dipdir_dip (outGDB.corrL);

	if ((outGDB.D.X == outGDB.DC.X) && (outGDB.D.Y == outGDB.DC.Y) && (outGDB.D.Z == outGDB.DC.Z)) outGDB.D.X = outGDB.D.X + 10e-8;

	outGDB.SC = crossproduct (outGDB.NC, outGDB.DC);

	return outGDB;
}

vector <GDB> manipulate_N (vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;

	uniform_generator_reset();

	for (size_t i = 0; i < outGDB.size(); i++) {

		string DT = outGDB.at(i).DATATYPE;

		bool LITHOLOGY (is_allowed_lithology_datatype(DT));

		bool PLANE = (is_allowed_plane_datatype(DT));
		bool LINEATION = (is_allowed_lineation_datatype(DT));

		bool STRIAE = 	(is_allowed_striae_datatype(DT));
		bool SC = 		(is_allowed_SC_datatype(DT));

		if (!LITHOLOGY && !PLANE && !LINEATION && !STRIAE && !SC) ASSERT_DEAD_END();

		double SN = 0.01;
		double a = uniform_0_1();

		SN = a * SN;

		if (a <= 0.33333) {

			if (outGDB.at(i).N.X > 0.5) 	outGDB.at(i).N.X = outGDB.at(i).N.X - SN;
			else 							outGDB.at(i).N.X = outGDB.at(i).N.X + SN;

			if (outGDB.at(i).NC.X > 0.5) 	outGDB.at(i).NC.X = outGDB.at(i).NC.X - SN;
			else 							outGDB.at(i).NC.X = outGDB.at(i).NC.X + SN;
		}
		else if  (a >= 0.6666) {

			if (outGDB.at(i).N.Z > 0.5) 	outGDB.at(i).N.Z = outGDB.at(i).N.Z - SN;
			else 							outGDB.at(i).N.Z = outGDB.at(i).N.Z + SN;

			if (outGDB.at(i).NC.Z > 0.5) 	outGDB.at(i).NC.Z = outGDB.at(i).NC.Z - SN;
			else 							outGDB.at(i).NC.Z = outGDB.at(i).NC.Z + SN;
		}
		else {

			if (outGDB.at(i).N.Y > 0.5) 	outGDB.at(i).N.Y = outGDB.at(i).N.Y - SN;
			else 							outGDB.at(i).N.Y = outGDB.at(i).N.Y + SN;

			if (outGDB.at(i).NC.Y > 0.5) 	outGDB.at(i).NC.Y = outGDB.at(i).NC.Y - SN;
			else 							outGDB.at(i).NC.Y = outGDB.at(i).NC.Y + SN;
		}


		if (!LITHOLOGY) {

			outGDB.at(i).N = unitvector (outGDB.at(i).N);

			if (SC || STRIAE) outGDB.at(i).NC = unitvector (outGDB.at(i).NC);
			else {} // ok
		}
		else {} // ok
	}

	return outGDB;
}

GDB cGc_NCDCSC_PITCH (GDB inGDB) {

	GDB outGDB = inGDB;
	VCTR axis = outGDB.N;
	VCTR torotate = outGDB.S;
	VCTR geodetic, pitchvector, tempvctr;
	DIPDIR_DIP tempDDD;

	double angle = outGDB.LPITCH;;
	double d_sense = 0.0;
	double angle_S_SENSE;


	if (outGDB.LPITCHSENSE == "NNE") {d_sense = 22.5;}
	if (outGDB.LPITCHSENSE == "NE")  {d_sense = 45.0;}
	if (outGDB.LPITCHSENSE == "ENE") {d_sense = 67.5;}
	if (outGDB.LPITCHSENSE == "E")   {d_sense = 90.0;}
	if (outGDB.LPITCHSENSE == "ESE") {d_sense = 112.5;}
	if (outGDB.LPITCHSENSE == "SE")  {d_sense = 135.0;}
	if (outGDB.LPITCHSENSE == "SSE") {d_sense = 157.5;}
	if (outGDB.LPITCHSENSE == "S")   {d_sense = 180.0;}
	if (outGDB.LPITCHSENSE == "SSW") {d_sense = 202.5;}
	if (outGDB.LPITCHSENSE == "SW")  {d_sense = 225.0;}
	if (outGDB.LPITCHSENSE == "WSW") {d_sense = 247.5;}
	if (outGDB.LPITCHSENSE == "W")   {d_sense = 270.0;}
	if (outGDB.LPITCHSENSE == "WNW") {d_sense = 292.5;}
	if (outGDB.LPITCHSENSE == "NW")  {d_sense = 315.0;}
	if (outGDB.LPITCHSENSE == "NNW") {d_sense = 337.5;}

	geodetic = declare_vector (SIN (d_sense), COS (d_sense), 0.0);
	geodetic = unitvector (geodetic);

	angle_S_SENSE = ASIN (dotproduct(geodetic, outGDB.S, false));

	if (angle_S_SENSE < 0.0) angle_S_SENSE =  90.0 - angle_S_SENSE;

	if (angle_S_SENSE > 90.0) angle = - angle;

	angle = - angle;

	pitchvector = ROTATE (axis, torotate, angle);

	outGDB.DC = pitchvector;

	if (outGDB.DC.Z > 0.0) {

		outGDB.DC.X = - outGDB.DC.X;
		outGDB.DC.Y = - outGDB.DC.Y;
		outGDB.DC.Z = - outGDB.DC.Z;
	}

	tempvctr = outGDB.DC;

	tempDDD = dipdir_dip_from_DXDYDZ (tempvctr);

	outGDB.corrL = tempDDD;

	outGDB.LDIR = tempDDD.DIPDIR;
	outGDB.LDIP = tempDDD.DIP;

	outGDB.NC = NXNYNZ_from_dipdir_dip (outGDB.corrL);

	outGDB.SC = crossproduct (outGDB.NC, outGDB.DC);

	outGDB.PITCHANGLE = outGDB.LPITCH;

	return outGDB;
}

vector <GDB> cGc_NDS_DCNCSC (vector <GDB> inGDB) {

	vector <GDB> outGDB;
	outGDB = inGDB;	

	size_t i = 0;
	size_t n_pitch = 0;
	size_t n_lineation = 0;

	do	{

		outGDB.at(i).NC = declare_vector (999.99, 999.99, 999.99);
		outGDB.at(i).DC = declare_vector (999.99, 999.99, 999.99);
		outGDB.at(i).SC = declare_vector (999.99, 999.99, 999.99);

		if ((outGDB.at(i).LINEATION == "SC") || (outGDB.at(i).LINEATION == "LINEATION")) {

			outGDB.at(i) = cGc_NCDCSC_LINEATION_SC (outGDB.at(i));
			n_lineation++;
		}

		if (outGDB.at(i).LINEATION == "PITCH") {

			outGDB.at(i) = cGc_NCDCSC_PITCH (outGDB.at(i));
			n_pitch++;
		}

		i++;
	}

	while (i < outGDB.size());

	cout << "  - Pitch to lineation conversion has done for " << n_pitch << " pitches of " << n_lineation + n_pitch << " striae data." << endl;

	return outGDB;
}

vector <GDB> cGc_PITCHANGLE (vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;

	size_t i = 0;

	do {

		if ((outGDB.at(i).DATATYPE == "STRIAE") || (outGDB.at(i).DATATYPE == "SC")) {

			outGDB.at(i).PITCHANGLE = ACOS (dotproduct(outGDB.at(i).S, outGDB.at(i).SV, false));

			if (outGDB.at(i).UP) outGDB.at(i).PITCHANGLE = - outGDB.at(i).PITCHANGLE;
		}

		else outGDB.at(i).PITCHANGLE = 999.99;

		i++;

	} while (i < inGDB.size());

	return outGDB;
}

vector <GDB> cGc_MISFIT (vector <GDB> inGDB) {

	vector <GDB> outGDB;

	outGDB = inGDB;

	size_t i = 0;
	size_t j = 0;

	int mfc1 = 0;
	int mfc2 = 0;
	int mfc3 = 0;
	int mfc4 = 0;
	int total = 0;

	do {

		outGDB.at(i).MISFIT = 999.99;

		if ((outGDB.at(i).DATATYPE == "STRIAE") && (outGDB.at(i).LINEATION == "LINEATION")){

			outGDB.at(i).MISFIT = ASIN(dotproduct(outGDB.at(i).N, outGDB.at(i).DC, false));

			if (outGDB.at(i).MISFIT < 0.0) outGDB.at(i).MISFIT = - outGDB.at(i).MISFIT;

			if ((outGDB.at(i).MISFIT >=  0.1) && (outGDB.at(i).MISFIT < 5.0))	mfc1++;
			if ((outGDB.at(i).MISFIT >=  5.0) && (outGDB.at(i).MISFIT < 15.0))	mfc2++;
			if ((outGDB.at(i).MISFIT >= 15.0) && (outGDB.at(i).MISFIT < 45.0))	mfc3++;
			if  (outGDB.at(i).MISFIT >= 45.0)									mfc4++;

			j++;
		}

		i++;
	}

	while (i < outGDB.size());

	total = mfc1 + mfc2 + mfc3 + mfc4;

	if (total > 0) {

		cout << "  - Apparent angular misfit for " << j << " striae(s) were computed in " << total << " records." << endl;
		cout << "    - less  than 5 degrees apparent angular misfit: " 	<< mfc1 << " data." << endl;
		cout << "    - 5 to 15 degrees apparent angular misfit     : " 	<< mfc2 << " data." << endl;
		cout << "    - 15 to 45 degrees apparent angular misfit    : " 	<< mfc3 << " data." << endl;
		cout << "    - more than 45 degrees apparent angular misfit: " 	<< mfc4 << " data." << endl;
		cout << "  - Striae correction." << endl;
	}

	else cout << "  - No striae to correcting." << endl;

	return outGDB;
}

CORRECTSTRIAE cGc_correct_striae_DIPcor (GDB inGDB) {

	CORRECTSTRIAE corrSTR;
	VCTR cSTR;
	double misfit;

	if ((inGDB.N.X == inGDB.SC.X) && (inGDB.N.Y == inGDB.SC.Y) && (inGDB.N.Z == inGDB.SC.Z)) inGDB.N.X = inGDB.N.X + 10e-8;

	cSTR = crossproduct (inGDB.SC, inGDB.N);

	cSTR = flip_D_vector (cSTR);

	cSTR = unitvector (cSTR);

	misfit = ASIN (dotproduct(cSTR, inGDB.DC, false));

	if (misfit < 0.0) misfit = - misfit;

	misfit = 90.0 - misfit;

	corrSTR.X = cSTR.X;
	corrSTR.Y = cSTR.Y;
	corrSTR.Z = cSTR.Z;
	corrSTR.MISFIT = misfit;

	return corrSTR;
}

CORRECTSTRIAE cGc_correct_striae_DIPDIRcor (GDB inGDB) {

	CORRECTSTRIAE corrSTR;
	VCTR cSTR;
	double misfit;

	//if ((inGDB.N.X - inGDB.NC.X < 10e-4) && (inGDB.N.Y == inGDB.NC.Y< 10e-4) && (inGDB.N.Z == inGDB.NC.Z< 10e-4)) inGDB.N.X = inGDB.N.X + 10e-8;

	cSTR = crossproduct (inGDB.N, inGDB.NC);

	cSTR = flip_D_vector (cSTR);

	cSTR = unitvector (cSTR); // FIXME What if cSTR has approx 0 length?

	misfit = ASIN (dotproduct(cSTR, inGDB.DC, false));

	if (misfit < 0.0) misfit = - misfit;

	misfit = 90.0 - misfit;

	corrSTR.X = cSTR.X;
	corrSTR.Y = cSTR.Y;
	corrSTR.Z = cSTR.Z;
	corrSTR.MISFIT = misfit;

	return corrSTR;
}

vector <GDB> cGc_striae_correction (vector <GDB> inGDB) {

	vector <GDB> outGDB;
	GDB tempGDB;

	VCTR temp;
	DIPDIR_DIP ttemp;

	CORRECTSTRIAE corrSTR_DIPcor;
	CORRECTSTRIAE corrSTR_DIPDIRcor;

	outGDB = inGDB;
	double misfitangle;

	size_t i = 0;

	do {

		tempGDB = outGDB.at(i);


		outGDB.at(i).corr.DIPDIR;

		if (outGDB.at(i).LINEATION != "LINEATION") {

			corrSTR_DIPcor.MISFIT = 0.0;
			corrSTR_DIPDIRcor.MISFIT = 0.0;
		}

		else {

			corrSTR_DIPcor.MISFIT = 999.99;
			corrSTR_DIPDIRcor.MISFIT = 999.99;

			if (
					(fabs(dotproduct (inGDB.at(i).SC, inGDB.at(i).N)) > 0.9999 && fabs(dotproduct (inGDB.at(i).SC, inGDB.at(i).N)) < 1.0001)
					||
					(fabs(dotproduct (inGDB.at(i).NC, inGDB.at(i).N)) > 0.9999 && fabs(dotproduct (inGDB.at(i).NC, inGDB.at(i).N)) < 1.0001)
					) {

				corrSTR_DIPcor.MISFIT = 0.0;
				corrSTR_DIPDIRcor.MISFIT = 0.0;
			}
		}

		if ((outGDB.at(i).LINEATION == "LINEATION") && ((corrSTR_DIPcor.MISFIT > 0.1) || (corrSTR_DIPDIRcor.MISFIT > 0.1))){

			corrSTR_DIPcor 		= cGc_correct_striae_DIPcor 	(tempGDB);
			corrSTR_DIPDIRcor 	= cGc_correct_striae_DIPDIRcor 	(tempGDB);
		}



		if ((outGDB.at(i).LINEATION == "LINEATION") && ((corrSTR_DIPcor.MISFIT > 0.1) && (corrSTR_DIPDIRcor.MISFIT > 0.1))){

			if (corrSTR_DIPcor.MISFIT < corrSTR_DIPDIRcor.MISFIT) {

				outGDB.at(i).DC.X 	= corrSTR_DIPcor.X;
				outGDB.at(i).DC.Y 	= corrSTR_DIPcor.Y;
				outGDB.at(i).DC.Z 	= corrSTR_DIPcor.Z;
				misfitangle 		= corrSTR_DIPcor.MISFIT;
			}

			else {

				outGDB.at(i).DC.X 	= corrSTR_DIPDIRcor.X;
				outGDB.at(i).DC.Y 	= corrSTR_DIPDIRcor.Y;
				outGDB.at(i).DC.Z 	= corrSTR_DIPDIRcor.Z;
				misfitangle 		= corrSTR_DIPDIRcor.MISFIT;
			}


			temp = outGDB.at(i).DC;

			ttemp = dipdir_dip_from_DXDYDZ (temp);

			cout << "    - Record '" << outGDB.at(i).ID << "' corrected from "
					<< setfill ('0') << fixed << setw (3) << setprecision (0) << outGDB.at(i).corr.DIPDIR 	<< "/"
					<< setfill ('0') << fixed << setw (2) << setprecision (0) << outGDB.at(i).corr.DIP 		<< "//"
					<< setfill ('0') << fixed << setw (3) << setprecision (0) << outGDB.at(i).corrL.DIPDIR 		<< "/"
					<< setfill ('0') << fixed << setw (2) << setprecision (0) << outGDB.at(i).corrL.DIP 		<< " to " << flush;

			outGDB.at(i).corrL = ttemp;

			cout
			<< setfill ('0') << fixed << setw (3) << setprecision (0) << outGDB.at(i).corr.DIPDIR	<< "/"
			<< setfill ('0') << fixed << setw (2) << setprecision (0) << outGDB.at(i).corr.DIP 		<< "//"
			<< setfill ('0') << fixed << setw (3) << setprecision (0) << outGDB.at(i).corrL.DIPDIR 		<< "/"
			<< setfill ('0') << fixed << setw (2) << setprecision (0) << outGDB.at(i).corrL.DIP 		<< " (change: "
			<< setfill ('0') << fixed << 			 setprecision (1) << misfitangle << " deg) using" << flush;

			if (corrSTR_DIPcor.MISFIT < corrSTR_DIPDIRcor.MISFIT) cout << " 'DIP' correction." << endl;

			else cout << " 'DIPDIR' correction." << endl;
		}

		i++;
	}

	while (i < outGDB.size());

	return outGDB;
}

vector <GDB> cGc_UP (vector <GDB> inGDB) {

	vector <GDB> outGDB;
	outGDB = inGDB;

	VCTR D, DC, R, d;
	bool p;
	string o, to;
	size_t i = 0;

	do {

		outGDB.at(i).SV = declare_vector (999.99, 999.99, 999.99);

		if ((outGDB.at(i).DATATYPE == "STRIAE")  || (outGDB.at(i).DATATYPE == "SC")) {

			outGDB.at(i).UP = true;

			if (outGDB.at(i).DATATYPE == "STRIAE") {

				D  = outGDB.at(i).D;
				DC = outGDB.at(i).DC;
				R = crossproduct(D, DC);

				if (R.Z > 0.0) 	p = true;
				else p = false;

				o = outGDB.at(i).OFFSET;

				if ((p) && (o == "NORMAL")) 	outGDB.at(i).UP = false;
				if ((p) && (o == "SINISTRAL"))	outGDB.at(i).UP = false;
				if ((!p) && (o == "NORMAL")) 	outGDB.at(i).UP = false;
				if ((!p) && (o == "DEXTRAL")) 	outGDB.at(i).UP = false;

				outGDB.at(i).SV = outGDB.at(i).DC;
			}

			else {

				d = compute_d_for_SC (inGDB.at(i));

				outGDB.at(i).SV = d;

				D  = outGDB.at(i).D;
				DC = outGDB.at(i).SV;
				R = crossproduct(D, DC);

				if (R.Z > 0.0) 	p = true;
				else p = false;

				if (outGDB.at(i).SV.Z < 0.0) outGDB.at(i).UP = false;
			}

			if (outGDB.at(i).UP) outGDB.at(i).SV = declare_vector (- outGDB.at(i).SV.X, - outGDB.at(i).SV.Y, - outGDB.at(i).SV.Z);

			if ((outGDB.at(i).OFFSET == "NONE") && (outGDB.at(i).DATATYPE == "STRIAE")) outGDB.at(i).SV = declare_vector (0.0, 0.0, 0.0);
		}

		i++;
	}

	while (i < outGDB.size());

	return outGDB;
}

vector <GDB> cGc_tilted_UP (vector <GDB> inGDB) {

	vector <GDB> outGDB;
	outGDB = inGDB;

	string o, to;
	size_t i = 0;

	do {

		if ((outGDB.at(i).DATATYPE == "STRIAE")  || (outGDB.at(i).DATATYPE == "SC")) {

			if (outGDB.at(i).SV.Z < 0.0) 	outGDB.at(i).UP = false;
			else 							outGDB.at(i).UP = true;
		}

		i++;
	}

	while (i < outGDB.size());

	return outGDB;
}

vector <GDB> cGc_OFFSET (vector <GDB> inGDB) {

	vector <GDB> outGDB;
	outGDB = inGDB;

	double dp, pa;

	string o, to;
	size_t i = 0;

	int corrected_striae = 0;

	cout << "  - Striae offset correction" << endl;

	do {

		if (outGDB.at(i).DATATYPE == "STRIAE") {

			o = outGDB.at(i).OFFSET;
			dp = outGDB.at(i).corr.DIP;
			pa = outGDB.at(i).PITCHANGLE;

			if (dp <= 15.0) {

				if (outGDB.at(i).UP)	to = "INVERSE";
				else 					to = "NORMAL";
			}

			else {

				if 		((pa > -180.0) && (pa <= -135.0)) 	to = "DEXTRAL";

				else if ((pa > -135.0) && (pa <= - 45.0)) 	to = "INVERSE";

				else if ((pa > - 45.0) && (pa <=   45.0)) 	to = "SINISTRAL";

				else if ((pa >   45.0) && (pa <=  135.0)) 	to = "NORMAL";

				else										to = "DEXTRAL";
			}

			if (o == "NONE") to = "NONE";

			if (to != outGDB.at(i).OFFSET) {

				cout << "    - Striae offset in record '" << outGDB.at(i).ID << "' has change from " << outGDB.at(i).OFFSET << " to " << to << "." << endl;
				outGDB.at(i).corrOFFSET = to;
				corrected_striae++;
			}

			else outGDB.at(i).corrOFFSET = outGDB.at(i).OFFSET;
		}

		i++;
	}

	while (i < outGDB.size());

	if (corrected_striae == 0) cout << "    - No striae offset to correct." << endl;

	return outGDB;
}

vector <GDB> cGc_LAMBDA_STRESSVECTOR_ESTIMATORS (vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;

	size_t i = 0;

	do {

		outGDB.at(i).lambda = 999.99;
		outGDB.at(i).ANG    = 999.99;
		outGDB.at(i).RUP    = 999.99;

		outGDB.at(i).SHEAR_S  = declare_vector (999.99, 999.99, 999.99);
		outGDB.at(i).NORMAL_S = declare_vector (999.99, 999.99, 999.99);
		outGDB.at(i).UPSILON  = declare_vector (999.99, 999.99, 999.99);

		if (outGDB.at(i).DATATYPE == "STRIAE")	outGDB.at(i).lambda = sqrt(3.0) / 2.0;

		i++;

	} while (i < inGDB.size());

	return outGDB;
}

bool byLocType(const GDB& x, const GDB& y) {

	if (x.LOC != y.LOC) return x.LOC < y.LOC;
	return x.DATATYPE < y.DATATYPE;
}

bool byLocTypeGc(const GDB& x, const GDB& y) {

	if (x.LOC != y.LOC) return x.LOC < y.LOC;
	if (x.DATATYPE != y.DATATYPE) return x.DATATYPE < y.DATATYPE;
	return x.GC < y.GC;
}

bool byiID(const GDB& x, const GDB& y) {

	return x.iID < y.iID;
}

bool bycorrDIPDIRcorrDIP(const GDB& x, const GDB& y) {

	if (x.corr.DIPDIR != y.corr.DIPDIR) return x.corr.DIPDIR < y.corr.DIPDIR;
	return x.corr.DIP < y.corr.DIP;
}

bool bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP(const GDB& x, const GDB& y) {

	if (x.corr.DIPDIR != y.corr.DIPDIR) return x.corr.DIPDIR < y.corr.DIPDIR;
	if (x.corr.DIP != y.corr.DIP) return x.corr.DIP < y.corr.DIP;
	if (x.corrL.DIPDIR != y.corrL.DIPDIR) return x.corrL.DIPDIR < y.corrL.DIPDIR;
	return x.corrL.DIP < y.corrL.DIP;
}

/*
bool bycorrDIPDIR(const GDB& x, const GDB& y) {

	return x.corr.DIPDIR < y.corr.DIPDIR;
}

bool bycorrDIP(const GDB& x, const GDB& y) {

	return x.corr.DIP < y.corr.DIP;
}
*/

bool byeigenvalue(const sort_jacobi& x, const sort_jacobi& y) {

	return x.eigenvalue < y.eigenvalue;
}

vector <GDB> sort_by_iID (vector <GDB> inGDB) {

	vector <GDB> outGDB = inGDB;
	sort(outGDB.begin(), outGDB.end(), byiID);
	return outGDB;
}

bool stopcriteria (string prevDATATYPE, string DATATYPE, string prevLOC, string LOC, string prevGC, string GC, INPSET inset) {

	if (inset.group == "Y") {

		if ((prevDATATYPE == DATATYPE) && (prevLOC == LOC) && (prevGC == GC)) return true;
		else return false;
	}

	else {

		if ((prevDATATYPE == DATATYPE) && (prevLOC == LOC)) return true;
		else return false;
	}
}

bool stopcriteria (string prevDATATYPE, string DATATYPE, string prevLOC, string LOC) {

	if ((prevDATATYPE == DATATYPE) && (prevLOC == LOC)) return true;
	else return false;
}

size_t minimum_independent_dataset (INPSET inset) {

	if 		(inset.inversion == "D") return 1;
	else if (inset.inversion == "P") return 1;
	else if (inset.inversion == "M") return 4;
	else if (inset.inversion == "S") return 5;
	else if (inset.inversion == "F") return 6;
	else if (inset.inversion == "A") return 4;
	else if (inset.inversion == "O") return 4;
	else if (inset.inversion == "B") return 4;
	else if (inset.fracture == "B") return 4;
	else return 1;
}

vector <GDB> return_GDB_with_no_homogeneous_data (vector <GDB> inGDB) {

	vector <GDB> resultGDB;

	//cout << "IN: " << inGDB.size() << endl;

	for (size_t i = 0; i < inGDB.size() - 1; i++) {

		GDB comp1 = inGDB.at(i);
		GDB comp2 = inGDB.at(i + 1);

	//	cout << comp1.ID << '\t' << comp2.ID << endl;

		vector <GDB> testGDB;

		testGDB.push_back (comp1);
		testGDB.push_back (comp2);

		if (check_dataset_homogenity(testGDB)) resultGDB.push_back(comp1);
		if (i == inGDB.size() - 2) resultGDB.push_back(comp2);

		//cout << i << '\t' << resultGDB.size() << endl;
	}

	//cout << "OUT: "  << resultGDB.size() << endl;

	return resultGDB;
}

bool correct_inhomogeneous_number (vector <GDB> inGDB, INPSET inset) {

	if (inGDB.size() < 2) return false;

	bool STRIAE = 	(is_allowed_striae_datatype(inGDB.at(0).DATATYPE));
	bool SC = 		(is_allowed_SC_datatype(inGDB.at(0).DATATYPE));

	if (inset.inversion == "N" && STRIAE) return true;

	if (!(check_dataset_homogenity (inGDB))) return false;

	if (SC || STRIAE)	sort(inGDB.begin(), inGDB.end(), bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP);
	else 				sort(inGDB.begin(), inGDB.end(), bycorrDIPDIRcorrDIP);

	vector <GDB> test = return_GDB_with_no_homogeneous_data (inGDB);

	if (test.size() >= minimum_independent_dataset (inset)) return true;
	return false;
}

bool check_dataset_geometry_homogenity (vector <GDB> inGDB) {

	bool STRIAE = 	(is_allowed_striae_datatype(inGDB.at(0).DATATYPE));
	bool SC = 		(is_allowed_SC_datatype(inGDB.at(0).DATATYPE));
	bool BEDDING = 	((inGDB.at(0).DATATYPE) == "BEDDING");

	if (SC || STRIAE) 	sort(inGDB.begin(), inGDB.end(), bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP);
	else 				sort(inGDB.begin(), inGDB.end(), bycorrDIPDIRcorrDIP);

	double minDD = inGDB.at(0).corr.DIPDIR;
	double maxDD = inGDB.at(inGDB.size() - 1).corr.DIPDIR;
	double var1 = fabs(maxDD - minDD);

	double minD = inGDB.at(0).corr.DIP;
	double maxD = inGDB.at(inGDB.size() - 1).corr.DIP;
	double var2 = fabs(maxD - minD);

	double minLDD = inGDB.at(0).corrL.DIPDIR;
	double maxLDD = inGDB.at(inGDB.size() - 1).corrL.DIPDIR;
	double var3 = fabs(maxLDD - minLDD);

	double minLD = inGDB.at(0).corrL.DIP;
	double maxLD = inGDB.at(inGDB.size() - 1).corrL.DIP;
	double var4 = fabs(maxLD - minLD);

	if (SC || STRIAE) return (var1 > 0.1 || var2 > 0.1 || var3 > 0.1 || var4 > 0.1);
	else if (BEDDING) return (var1 > 0.1 || var2 > 0.1);
	else return (var1 > 0.1 || var2 > 0.1);
}

bool check_dataset_offset_homogenity (vector <GDB> inGDB) {

	string ofs1 = inGDB.at(0).OFFSET;
	string ofs2 = inGDB.at(inGDB.size() - 1).OFFSET;
	return (ofs1 != ofs2);
}

bool check_dataset_homogenity (vector <GDB> inGDB) {

	bool STRIAE = 	(is_allowed_striae_datatype(inGDB.at(0).DATATYPE));
	bool SC = 		(is_allowed_SC_datatype(inGDB.at(0).DATATYPE));
	bool BEDDING = 	((inGDB.at(0).DATATYPE) == "BEDDING");

	bool OFFSET = check_dataset_offset_homogenity (inGDB);
	bool GEOMETRY = check_dataset_geometry_homogenity (inGDB);

	if (SC || STRIAE || BEDDING) return (GEOMETRY || OFFSET);
	else return (GEOMETRY);
}
/*
void fold_from_planes (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center) {

	vector <GDB> buffer;
	GDB tempGDB;

	size_t i = 0;

	STRESSFIELD sf;

	VCTR great_circle_normal;

	do {

		buffer.clear();

		do {

			buffer.push_back(inGDB.at(i));

			i++;

			if (i == inGDB.size()) break;

		} while ((inGDB.at(i - 1).DATATYPE == inGDB.at(i).DATATYPE) && (inGDB.at(i - 1).LOC == inGDB.at(i).LOC));



		if (buffer.at(0).DATATYPE == "FOLDSURFACE") {

			//sf = sf_BINGHAM (st_BINGHAM (buffer)); ez volt

			STRESSTENSOR st = st_BINGHAM (inGDB); // ez lett

			sf = eigenvalue_eigenvector (st);

			sf = sf_BINGHAM (sf);

			sf = computestressfield_DXDYDZ (sf); //eddig

			great_circle_normal = flip_D_vector (sf.EIGENVECTOR1);

			tempGDB = buffer.at(0);

			tempGDB.avD = unitvector (DXDYDZ_from_NXNYNZ (great_circle_normal));
			tempGDB.avd = dipdir_dip_from_DXDYDZ (tempGDB.avD);

			if (inset.plot == "H") 	PS_polepoint (tempGDB, o, inset, center, false, "FOLD");
			else 					PS_plane     (tempGDB, o, inset, center, false, "FOLD");

			PS_folddata (tempGDB, o, center);
		}

	} while (i < inGDB.size());

	return;
}

*/

/*
GDB init_average (GDB inGDB) {

	inGDB.avD = declare_vector (0.0, 1.0, 0.0);
	inGDB.avS0D = declare_vector (0.0, 1.0, 0.0);

	inGDB.avd.DIPDIR = 0.0;
	inGDB.avd.DIP = 0.0;
	inGDB.avS0d.DIPDIR = 0.0;
	inGDB.avS0d.DIP = 0.0;

	return inGDB;
}


bool is_datatype_processable_for_average (vector <GDB> inGDB) {

	string DT = inGDB.at(0).DATATYPE;

	return ((DT != "STRIAE") && (DT != "LITHOLOGY") && (DT != "SC"));
}

bool is_processable_for_average_MT2 (vector <GDB> inGDB) {

	return ((inGDB.size() > 2) && is_datatype_processable_for_average (inGDB) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_EQ2 (vector <GDB> inGDB) {

	return ((inGDB.size() == 2) && is_datatype_processable_for_average (inGDB) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_EQ1 (vector <GDB> inGDB) {

	return ((inGDB.size() == 1) && is_datatype_processable_for_average (inGDB) && check_dataset_homogenity (inGDB));
}

bool is_processable_for_average_HOMOG (vector <GDB> inGDB) {

	return (is_datatype_processable_for_average (inGDB) &&  !check_dataset_homogenity (inGDB));
}

STRESSFIELD process_for_average_MT2 (vector <GDB> inGDB) {

	return sf_BINGHAM (st_BINGHAM (inGDB));
}

STRESSFIELD process_for_average_EQ2 (vector <GDB> inGDB) {

	STRESSFIELD sf;

	sf.EIGENVECTOR2 = declare_vector (
			inGDB.at(0).D.X + inGDB.at(1).D.X,
			inGDB.at(0).D.Y + inGDB.at(1).D.Y,
			inGDB.at(0).D.Z + inGDB.at(1).D.Z);

	sf.EIGENVECTOR2 = unitvector (sf.EIGENVECTOR2);

	sf.S_2 = dipdir_dip_from_DXDYDZ(sf.EIGENVECTOR2);

	return sf;
}

STRESSFIELD  (vector <GDB> inGDB) {

	STRESSFIELD sf;

	sf.EIGENVECTOR2 = inGDB.at(0).D;
	sf.S_2 = inGDB.at(0).corr;

	return sf;
}

vector <GDB> calculate_average_for_1 (vector <GDB> inGDB) {

	VCTR N = inGDB.at(0).N;

	inGDB.at(0) = init_average (inGDB.at(0));

	inGDB.at(0).avD = unitvector(flip_D_vector (DXDYDZ_from_NXNYNZ (N)));

	inGDB.at(0).avd = dipdir_dip_from_DXDYDZ (inGDB.at(0).avD);

	if ((inGDB.at(0).DATATYPE == "BEDDING") && (inGDB.at(0).OFFSET == "OVERTURNED")) inGDB.at(0).avS0offset = "OVERTURNED";

	return inGDB;
}
*/

/*

vector <GDB> cGc_s0_average (vector <GDB> inGDB) {

	vector <GDB> outGDB;

	outGDB = inGDB;

	size_t intervalbegin = 0;
	size_t independentrecordcounter = 0;
	size_t i = 0;
	size_t j = 0;

	VCTR temp, temp2;
	DIPDIR_DIP avs0;

	if (outGDB.size() == 1) {

		outGDB.at(0).avS0D = declare_vector (10e-8, 1.0 - 10e-8, 10e-8);
		outGDB.at(0).avS0N = declare_vector (10e-8, 10e-8, 1.0 - 10e-8);

		outGDB.at(0).avS0d.DIPDIR 	= 10e-8;
		outGDB.at(0).avS0d.DIP    	= 10e-8;

		if (outGDB.at(0).DATATYPE == "BEDDING") {

			outGDB.at(0).avS0D = outGDB.at(0).avD;

			temp = declare_vector (outGDB.at(0).avS0D.X, outGDB.at(0).avS0D.Y, outGDB.at(0).avS0D.Z);
			temp =  NXNYNZ_from_DXDYDZ (temp);

			outGDB.at(0).avS0N = temp;

			temp = outGDB.at(0).avS0D;

			avs0 = dipdir_dip_from_DXDYDZ (temp);

			outGDB.at(0).avS0d = avs0;

			cout << "  - Average bedding was computed for 1 independent data group in " << i << " records." <<  endl;
		}
		else {

			outGDB.at(0).avS0D = declare_vector (0.0, 1.0, 0.0);
			outGDB.at(0).avS0N = declare_vector (0.0, 0.0, 1.0);
			outGDB.at(0).avS0d.DIPDIR = 999.99;
			outGDB.at(0).avS0d.DIP = 99.99;

			cout << "  - No average bedding were computed" <<  endl;
		}
		return outGDB;
	}

	bool bedding_exists = false;

	do {

		temp = declare_vector (10e-8, 1.0 - 10e-8, 10e-8);

		do {

			if (outGDB.at(i).DATATYPE == "BEDDING") {

				temp = outGDB.at(i).avD;
				bedding_exists = true;
			}

			i++;

			if (i == outGDB.size()) break;

		} while ((outGDB.at(i-1).LOC == outGDB.at(i).LOC));

		j = intervalbegin;
		independentrecordcounter++;
		temp = unitvector (temp);

		do {

			if (bedding_exists) {

				outGDB.at(j).avS0D = temp;

				avs0 = dipdir_dip_from_DXDYDZ (temp);

				temp2 = declare_vector (outGDB.at(j).avS0D.X, outGDB.at(j).avS0D.Y, outGDB.at(j).avS0D.Z);
				temp2 =  NXNYNZ_from_DXDYDZ (temp2);

				outGDB.at(j).avS0N = temp2;

				outGDB.at(j).avS0d = avs0;

				//cout << "AVERAGE BEDDING :" << avs0.DIPDIR << "/" << avs0.DIP << endl;
			}
			else {

				outGDB.at(j).avS0D = declare_vector (0.0, 1.0, 0.0);
				outGDB.at(j).avS0N = declare_vector (0.0, 0.0, 1.0);
				outGDB.at(j).avS0d.DIPDIR = 999.99;
				outGDB.at(j).avS0d.DIP = 99.99;
			}

			j++;

		} while (j < i);

		intervalbegin = i;

	} while (i < outGDB.size());

	cout << "  - Average bedding was computed for " << independentrecordcounter << " independent data group(s) in " << i << " records." <<  endl;

	return outGDB;
}

*/

vector <GDB> ptn (vector <GDB> inGDB, INPSET inset) {

	vector <GDB> outGDB;
	outGDB = inGDB;

	double r = inset.angle / 90.0;
	double q = 1.0 - r;

	size_t i = 0;

	VCTR temp1, temp2, temp3;

	DIPDIR_DIP dd;

	do {

		if ((inGDB.at(i).DATAGROUP == "SC") || ((inGDB.at(i).DATAGROUP == "STRIAE") && (inGDB.at(i).OFFSET != "NONE")) ) {

			temp1.X = (q * inGDB.at(i).N.X + r * inGDB.at(i).SV.X);
			temp1.Y = (q * inGDB.at(i).N.Y + r * inGDB.at(i).SV.Y);
			temp1.Z = (q * inGDB.at(i).N.Z + r * inGDB.at(i).SV.Z);
			temp1 = unitvector (temp1);
			temp1 = flip_D_vector (temp1);
			dd = dipdir_dip_from_DXDYDZ (temp1);
			outGDB.at(i).ptnT = temp1;
			outGDB.at(i).ptnTd = dd;

			temp2.X = (q * inGDB.at(i).SV.X - r * inGDB.at(i).N.X);
			temp2.Y = (q * inGDB.at(i).SV.Y - r * inGDB.at(i).N.Y);
			temp2.Z = (q * inGDB.at(i).SV.Z - r * inGDB.at(i).N.Z);
			temp2 = unitvector (temp2);
			temp2 = flip_D_vector (temp2);
			dd = dipdir_dip_from_DXDYDZ (temp2);
			outGDB.at(i).ptnP = temp2;
			outGDB.at(i).ptnPd = dd;

			temp3 = crossproduct (temp2, temp1);
			temp3 = unitvector (temp3); // FIXME What if temp3 has approximately 0 length?
			temp3 = flip_D_vector (temp3);
			dd = dipdir_dip_from_DXDYDZ (temp3);
			outGDB.at(i).ptnN = temp3;
			outGDB.at(i).ptnNd = dd;
		}

		i++;

	} while (i < inGDB.size());

	return outGDB;
}

vector <GDB> clustering_GBD (INPSET inset, vector <GDB> inGDB) {

	vector <GDB> processGDB;
	vector <GDB> outGDB = inGDB;
	size_t i = 0;
	size_t j = 0;

	if ((inGDB.size() == 1) || (inset.clusternumber == "N")) {

		cout << "  - No clustering of geodatabase was required." << endl;
		return outGDB;
	}

	do {

		processGDB.clear();

		do {

			processGDB.push_back(outGDB.at(i));
			i++;

			if (i == outGDB.size()) break;

		} while (stopcriteria (outGDB.at(i-1).DATATYPE, outGDB.at(i).DATATYPE, outGDB.at(i-1).LOC, outGDB.at(i).LOC));

		j = 0;

		sort(processGDB.begin(), processGDB.end(), byiID);

		if (processGDB.at(0).DATATYPE != "LITHOLOGY") processGDB = k_means (inset,  processGDB);

		do {

			outGDB.at(i -  processGDB.size() + j) = processGDB.at(j);
			j++;

		} while (j < processGDB.size());

	} while (i < outGDB.size());

	return outGDB;
}

void process_rgf (string inputfilename, string XY_filename, INPSET inset) {

	vector <GDB> geodatabase, tiltgeodatabase;

	PFN	projectfoldername = createprojectfoldernames (inputfilename);

	cout << "GEODATABASE PROCESSING FOR '" << capslock(inputfilename)<< ".RGF' DATABASE FILE" << endl;
	if (XY_filename == "NONE") geodatabase = competeRGFcontect(inputfilename, "NONE", inset);
	else geodatabase = competeRGFcontect(inputfilename, XY_filename, inset);

	geodatabase = cGc_NDS (geodatabase);
	geodatabase = cGc_NDS_DCNCSC (geodatabase);
	geodatabase = manipulate_N (geodatabase);
	geodatabase = cGc_MISFIT (geodatabase);
	geodatabase = cGc_striae_correction (geodatabase);
	geodatabase = cGc_UP (geodatabase);
	geodatabase = cGc_PITCHANGLE (geodatabase);
	geodatabase = cGc_OFFSET (geodatabase);
	geodatabase = cGc_LAMBDA_STRESSVECTOR_ESTIMATORS (geodatabase);
	sort(geodatabase.begin(), geodatabase.end(), byLocTypeGc);

	cout << "K-MEANS CLUSTERING OF '" << capslock(inputfilename)<< ".RGF' DATABASE FILE" << endl;
	geodatabase = clustering_GBD (inset, geodatabase);


	sort(geodatabase.begin(), geodatabase.end(), byLocTypeGc);
	if (!(inset.clusternumber == "N"))  geodatabase = colorcode_grom_groupcode(geodatabase, inset);

	cout << "AVERAGE BEDDING COMPUTATION FOR '" << capslock(inputfilename)<< ".RGF' DATABASE FILE" << endl;
	geodatabase = cGc_average (geodatabase);
	//geodatabase = cGc_s0_average (geodatabase);
	geodatabase = ptn (geodatabase, inset);

	cout << "RETILTING OF '" << capslock(inputfilename)<< ".RGF' DATABASE FILE" << endl;
	tiltgeodatabase = cGc_RETILT (geodatabase, inset);
	tiltgeodatabase = cGc_tilted_UP (tiltgeodatabase);
	tiltgeodatabase = cGc_PITCHANGLE (tiltgeodatabase);
	tiltgeodatabase = cGc_OFFSET (tiltgeodatabase);
	tiltgeodatabase = cGc_average (tiltgeodatabase);
	//tiltgeodatabase = cGc_s0_average (tiltgeodatabase);
	tiltgeodatabase = ptn (tiltgeodatabase, inset);

	cout << "DATA EVALUATION AND EXPORT FROM '" << capslock(inputfilename) << ".RGF' DATABASE FILE" << endl;
	createprojectfolders (projectfoldername, geodatabase); // TODO Failures in creating project folders silently ignored
	create_pointer_to_project_folder(projectfoldername);
	copyoriginalfiles (projectfoldername);
	outputselected_ps_rgf (projectfoldername, geodatabase, tiltgeodatabase, inset);
	outputaveragergf (projectfoldername, geodatabase);
	geodatabase = sort_by_iID (geodatabase);
	tiltgeodatabase = sort_by_iID (tiltgeodatabase);
	outputresultrgf (projectfoldername, geodatabase, false, inset);
	copy_log(projectfoldername);
}

void dbg_vctr (vector <GDB> inGDB, string to_dump) {

	bool NORMAL = 	(to_dump == "N");
	bool DIP = 		(to_dump == "D");

	VCTR DUMP;

	cout
		<< "DATA_ID" << '\t'
		<< "DATATYPE" << '\t' << " ---- " << '\t'
		<< "X" << '\t'
		<< "Y" << '\t'
		<< "Z" << '\t'<< endl;

		for (size_t i = 0; i < inGDB.size(); i++) {

			if 		(NORMAL) 	DUMP = inGDB.at(i).N;
			else if (DIP) 		DUMP = inGDB.at(i).D;

			cout
			<< inGDB.at(i).ID << '\t'
			<< inGDB.at(i).DATATYPE << '\t' << " ---- " << '\t'
			<< DUMP.X << '\t'
			<< DUMP.Y << '\t'
			<< DUMP.Z << '\t' << endl;
		}
}
