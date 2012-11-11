// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

#include "ps.h"
#include "common.h"
#include "rgf.h"


using namespace std;

PAPER PS_dimensions () {

	PAPER P;

	P.X = mm_to_point (420);

	P.Y = P.X * 0.707143;
	P.A = P.X * 0.033670;
	P.B = P.A / 2.0;
	P.C = P.B / 2.0;

	P.R = P.X * 0.235726  * 0.5;

	P.O1X = 1.5 * P.A  + P.R;
	P.O1Y = P.Y - P.A - P.A - P.B - P.R;

	P.O2X = P.O1X;
	P.O2Y = P.O1X + P.B;

	P.O3X = P.O1X + P.R + P.B + P.A + P.R + 3.0 * P.B;
	P.O3Y = P.O1Y;

	P.O4X = P.O3X;
	P.O4Y = P.O2Y;

	P.O5X = P.O3X + P.R + 0.5 * P.B + P.A;
	P.O5Y = P.O3Y + P.R;

	P.O6X = P.O5X;
	P.O6Y = P.O4Y;

	P.O7X = P.O5X + P.R - 0.0 * P.B;
	P.O7Y = P.O3Y + P.B;

	P.O8X = P.O7X;
	P.O8Y = P.O6Y - P.R + P.B;





	P.S1X = P.X - (10.0 * P.A);
	P.S1Y = (P.Y / 2.0) + (3.5 * P.A);

	P.S2X = P.X - (1.00 * P.A);
	P.S2Y = P.S1Y;

	P.S3X = P.X - (1.00 * P.A);
	P.S3Y = (P.Y / 2.0) - (3.5 * P.A);

	P.S4X = P.S1X;
	P.S4Y = P.S3Y;

	P.S5X = P.S1X + 3.0 * P.A;
	P.S5Y = P.S1Y;

	P.S6X = P.S2X - 3.0 * P.A;
	P.S6Y = P.S1Y;

	return P;
}

void PS_header (string DATATYPE, string LOC, ofstream& o, PAPER P) {

	string filename;

	filename = LOC + "_" + DATATYPE + ".PS";
	o << "%% Generated as " << filename 									<< endl << endl;
	o << "<</PageSize [ " << P.X << " " << P.Y << " ]>> setpagedevice " 	<< endl << endl;

	if ((DATATYPE == "STRIAE") || (DATATYPE == "FRACTURE" ) || (DATATYPE == "SC")) {

		o << "/normalarrow {" 											<< endl;
		o << "  newpath  0.0  -6.0 moveto  0.0 6.0 lineto " 			<< endl;
		o << "    1 setlinewidth stroke"								<< endl;
		o << "  newpath -2.0  3.0 moveto  2.0 10.0 rlineto " 			<< endl;
		o << "    2.0  -10.0 rlineto " 									<< endl;
		o << "    -2.0 3.0 rlineto " 									<< endl;
		o << "    -2.0 -3.0 rlineto "			 						<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "    fill stroke" 											<< endl;
		o << "    0.7 setlinewidth" 									<< endl;
		o << "    0.0 0.0 1.5 0.0 360.0 arc stroke"						<< endl;
		o << "    1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth" 			<< endl;
		o << "    0.0 0.0 0.7 0.0 360.0 arc stroke"						<< endl;
		o << "} def" 													<< endl << endl;

		o << "/extension_arrow {" 										<< endl;
		o << "  newpath"												<< endl;
		o << "    -3.0   0.0 moveto "									<< endl;
		o << "     0.0   5.0 rlineto" 									<< endl;
		o << "    -2.0   0.0 rlineto" 									<< endl;
		o << "     5.0   5.0 rlineto" 									<< endl;
		o << "     5.0  -5.0 rlineto" 									<< endl;
		o << "    -2.0   0.0 rlineto" 									<< endl;
		o << "     0.0  -5.0 rlineto closepath" 						<< endl;
		o << "    0 0 0 setrgbcolor 1 setlinewidth  stroke" 			<< endl;
		o << "} def" 													<< endl << endl;

		o << "/compression_arrow {" 									<< endl;
		o << "  newpath"												<< endl;
		o << "    -5.0   5.0 moveto" 									<< endl;
		o << "     2.0   0.0 rlineto" 									<< endl;
		o << "     0.0   5.0 rlineto"									<< endl;
		o << "     6.0   0.0 rlineto"									<< endl;
		o << "     0.0  -5.0 rlineto" 									<< endl;
		o << "     2.0   0.0 rlineto" 									<< endl;
		o << "    -5.0  -5.0 rlineto closepath" 						<< endl;
		o << "    0 0 0 setrgbcolor fill stroke" 						<< endl;
		o << "} def" 													<< endl << endl;

		o << "/s1_axis {" 												<< endl;
		o << "  newpath"												<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;
		o << "    1.0 setlinewidth"										<< endl;

		o << "    1 1 1 setrgbcolor stroke" 							<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;

		o << "    1 0 0 setrgbcolor" 									<< endl;
		o << "    fill stroke" 											<< endl;
		o << "} def" 													<< endl << endl;

		o << "/s1_iter_axis {" 												<< endl;
		o << "  newpath"												<< endl;
		o << "    1 1 1 setrgbcolor stroke" 							<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;
		o << "    1 0 0 setrgbcolor 1 setlinewidth stroke" 			<< endl;
		o << "} def" 													<< endl << endl;

		o << "/s2_axis {" 												<< endl;
		o << "  newpath"												<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;
		o << "    1.0 setlinewidth"										<< endl;

		o << "    1 1 1 setrgbcolor stroke" 							<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;

		o << "    0 1 0 setrgbcolor" 									<< endl;
		o << "    fill stroke" 											<< endl;
		o << "} def" 													<< endl << endl;

		o << "/s2_iter_axis {" 											<< endl;
		o << "  newpath"												<< endl;
		o << "    1 1 1 setrgbcolor stroke" 							<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;
		o << "    0 1 0 setrgbcolor 1 setlinewidth stroke" 				<< endl;
		o << "} def" 													<< endl << endl;


		o << "/s3_axis {" 												<< endl;
		o << "  newpath"												<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;
		o << "    1.0 setlinewidth"										<< endl;

		o << "    1 1 1 setrgbcolor stroke" 							<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;

		o << "    0 0 1 setrgbcolor" 									<< endl;
		o << "    fill stroke" 											<< endl;
		o << "} def" 													<< endl << endl;


		o << "/s3_iter_axis {" 											<< endl;
		o << "  newpath"												<< endl;
		o << "    1 1 1 setrgbcolor stroke" 							<< endl;
		o << "    -2.0   0.0 moveto "									<< endl;
		o << "     4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0   4.0 rlineto" 									<< endl;
		o << "    -4.0  -4.0 rlineto closepath" 						<< endl;

		o << "    0 0 1 setrgbcolor 1 setlinewidth stroke" 				<< endl;
		o << "} def" 													<< endl << endl;



		o << "/dextralarrow {" 											<< endl;
		o << "  newpath  0.0 -1.5 moveto -6.0 -1.5 lineto" 				<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "    stroke" 												<< endl;
		o << "  newpath  -11.0 -1.2 moveto -4.0 -3.5 lineto" 			<< endl;
		o << "    -6.0 -1.2 lineto" 									<< endl;
		o << "    -11.0 -1.2 lineto" 									<< endl;
		o << "    1 setlinewidth "										<< endl;
		o << "    fill " 												<< endl;
		o << "    stroke" 												<< endl;
		o << "  newpath  0.0 1.5 moveto 6.0 1.5 lineto" 				<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "    stroke" 												<< endl;
		o << "  newpath  11.0 1.2 moveto 4.0 3.5 lineto" 				<< endl;
		o << "    6.0 1.2 lineto" 										<< endl;
		o << "    11.0 1.2 lineto" 										<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "    fill " 												<< endl;
		o << "    stroke" 												<< endl;
		o << "    0.0 0.0 0.0 setrgbcolor 0.7 setlinewidth" 			<< endl;
		o << "    0.0 0.0 1.5 0.0 360.0 arc stroke"						<< endl;
		o << "    1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth" 			<< endl;
		o << "    0.0 0.0 0.7 0.0 360.0 arc stroke"						<< endl;
		o << "} def" 													<< endl << endl;

		o << "/sinistralarrow {" 										<< endl;
		o << "  newpath  0.0 1.5 moveto -6.0 1.5 lineto" 				<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "    stroke" 												<< endl;
		o << "  newpath  -11.0 1.2 moveto -4.0 3.5 lineto" 				<< endl;
		o << "    -6.0 1.2 lineto" 										<< endl;
		o << "    -11.0 1.2 lineto" 									<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "     fill " 												<< endl;
		o << "    stroke" 												<< endl;
		o << "  newpath  0.0 -1.5 moveto 6.0 -1.5 lineto" 				<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "    stroke" 												<< endl;
		o << "  newpath  11.0 -1.2 moveto 4.0 -3.5 lineto" 				<< endl;
		o << "    6.0 -1.2 lineto" 										<< endl;
		o << "    11.0 -1.2 lineto" 									<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "   fill " 												<< endl;
		o << "    stroke" 												<< endl;
		o << "    0.0 0.0 0.0 setrgbcolor 0.7 setlinewidth" 			<< endl;
		o << "    0.0 0.0 1.5 0.0 360.0 arc stroke"						<< endl;
		o << "    1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth" 			<< endl;
		o << "    0.0 0.0 0.7 0.0 360.0 arc stroke"						<< endl;
		o << "} def" 													<< endl << endl;

		o << "/nonearrow {" 											<< endl;
		o << "  newpath  0.0  -8.0 moveto  0.0 8.0 lineto " 			<< endl;
		o << "    1 setlinewidth"										<< endl;
		o << "    stroke" 												<< endl;
		o << "    0.7 setlinewidth" 									<< endl;
		o << "    0.0 0.0 1.5 0.0 360.0 arc"							<< endl;
		o << "    gsave 1.00 1.00 1.00 setrgbcolor fill grestore"		<< endl;
		o << "    stroke" 												<< endl;
		o << "} def" 													<< endl << endl;

		o << "/p_axis {" 							<< endl;
		o << "  newpath"							<< endl;
		o << "    -1.0  -1.0 moveto "				<< endl;
		o << "    0.0   2.0 rlineto" 				<< endl;
		o << "    2.0   0.0 rlineto" 				<< endl;
		o << "    0.0  -2.0 rlineto closepath" 		<< endl;
		o << "    0.5 setlinewidth"					<< endl;
		o << "    1 0 0 setrgbcolor" 				<< endl;
		o << "    fill stroke" 						<< endl;
		o << "} def" 								<< endl << endl;

		o << "/t_axis {" 							<< endl;
		o << "  newpath"							<< endl;
		o << "   -1.0  -1.0 moveto "				<< endl;
		o << "	  0.0   2.0 rlineto" 				<< endl;
		o << "	  2.0   0.0 rlineto" 				<< endl;
		o << "	  0.0  -2.0 rlineto closepath" 		<< endl;
		o << "    0.5 setlinewidth"					<< endl;
		o << "    0 0 1 setrgbcolor" 				<< endl;
		o << "    fill stroke" 						<< endl;
		o << "} def" 								<< endl << endl;

		o << "/n_axis {" 							<< endl;
		o << "  newpath"							<< endl;
		o << "   -1.0  -1.0 moveto "				<< endl;
		o << "	  0.0   2.0 rlineto" 				<< endl;
		o << "	  2.0   0.0 rlineto" 				<< endl;
		o << "	  0.0  -2.0 rlineto closepath" 		<< endl;
		o << "    0.5 setlinewidth"					<< endl;
		o << "    0 1 0 setrgbcolor" 				<< endl;
		o << "    fill stroke" 						<< endl;
		o << "} def" 								<< endl << endl;
	}

}

void PS_border (GDB inGDB, ofstream& o, INPSET inset, PAPER P) {

	o << "newpath" << endl;

	o << "  " << fixed << setprecision (3) << P.A << " " << fixed << setprecision (3) << P.Y - P.A - 10.0 << " moveto "
			<< fixed << setprecision (3) << P.A << " " << fixed << setprecision (3) << P.A << " lineto" << endl;
	o << "  " << fixed << setprecision (3) << P.X - P.A << " " << fixed << setprecision (3) << P.A << " lineto" << endl;
	o << "  " << fixed << setprecision (3) << P.X - P.A << " " << fixed << setprecision (3) << P.Y - P.A << " lineto" << endl;
	o << "  " << fixed << setprecision (3) << P.A + 600.0 << " " << fixed << setprecision (3) << P.Y - P.A << " lineto" << endl;
	o << "  1.5 setlinewidth" << endl;
	o << "  0 0 0 setrgbcolor stroke" << endl << endl;

	o << "/ArialNarrow-Bold findfont 36 scalefont setfont" 					<< endl;
	o << "  " << P.O5X + (5.0 * P.A) << " " << (P.O5Y + P.B)  << " moveto " << endl;
	o << "  (ORIGINAL)  0.8 0.8 0.8 setrgbcolor show"  << endl;

	o << "  " << P.O6X + (4.0 * P.A) << " " <<  (P.O6Y + P.B)  << " moveto " << endl;
	o << "  (CORRECTED) 0.8 0.8 0.8 setrgbcolor show"  << endl;

	o << "/ArialNarrow-Bold findfont 12 scalefont setfont" 					<< endl;
	o
	<< "  " << P.A - P.B + 20.0 << " " << P.Y - P.A - 5.5 << " moveto "
	<< "(ALL OBSERVED " << inGDB.DATATYPE << " DATA FROM DATA POINT " << inGDB.LOC << flush;

	if (inset.group == "Y") {

		o << ", GROUP '" << inGDB.GC << "'" << flush;
	}

	if (inGDB.FORMATION != "") {

		o << " - " << inGDB.FORMATION << " FORMATION" << flush;

	}

	o << ") " << endl;

	o << "0 0 0 setrgbcolor show" << endl << endl;

	o << "  0.3 setlinewidth 0.5 0.5 0.5 setrgbcolor" << endl;
}

void PS_stress_scale (ofstream& o, PAPER P) {

	double value = 0.0;
	double step = 1.0 / 100.0;

	VCTR color;

	do {

		value = value + step;

		color = generarte_stress_colors (value);

		o << fixed << setprecision (2) << flush;
		o
		<< "  " << fixed << setprecision (2) << color.X
		<< " "  << fixed << setprecision (2) << color.Y
		<< " "  << fixed << setprecision (2) << color.Z

		<< " setrgbcolor 2 setlinewidth" << endl << endl;

		o
		<< "  newpath "
		<< fixed << setprecision (3) << P.O1X + P.R + 1.0 * P.B << " " << P.O1Y - P.R + (0.666 * P.R * value) << " moveto "
		<< fixed << setprecision (3) << P.O1X + P.R + 1.5 * P.B << " " << P.O1Y - P.R + (0.666 * P.R * value)
		<< " lineto stroke" << endl;

		o
		<< "  newpath "
		<< fixed << setprecision (3) << P.O2X + P.R + 1.0 * P.B << " " << P.O2Y - P.R + (0.666 * P.R * value)<< " moveto "
		<< fixed << setprecision (3) << P.O2X + P.R + 1.5 * P.B << " " << P.O2Y - P.R + (0.666 * P.R * value)
		<< " lineto stroke" << endl;

	} while (value < 3.0);

	o << "/ArialNarrow-Bold findfont 6 scalefont setfont" << endl;

	o << "  0 0 0 setrgbcolor " << endl;

	o
	<< "  " << fixed << setprecision (3) << P.O1X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O1Y - 1.0 * P.R << " moveto "
	<<" 90 rotate (EXTENSION) show 270 rotate"<< endl;

	o
	<< "  " << fixed << setprecision (3) << P.O1X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O1Y + 1.0 * P.R - 0.92 * P.A << " moveto "
	<< " 90 rotate (COMPRESSION) show 270 rotate "<< endl;

	o
	<< "  " << fixed << setprecision (3) << P.O2X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O2Y - 1.0 * P.R << " moveto "
	<<" 90 rotate (EXTENSION) show 270 rotate"<< endl;

	o
	<< "  " << fixed << setprecision (3) << P.O2X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O2Y + 1.0 * P.R - 0.92 * P.A << " moveto "
	<< " 90 rotate (COMPRESSION) show 270 rotate "<< endl;
}

void ps_clusteringresult (ofstream& o, INPSET inset, PAPER P, int clusternumber) {


	o << "/ArialNarrow findfont 8 scalefont setfont" << endl;
	o
	<< "  " << fixed << setprecision (3) << P.O1X - P.R - P.A
	<< " "  << fixed << setprecision (3) << P.O1Y - P.R - 33.0 << " moveto" << endl;

	if (inset.clusternumber == "A") o << "  (Automatic clustering " << flush;
	else							o << "  (User defined clustering " << flush;

	o
	<< " into " << clusternumber << " clusters with "
	<< fixed << setprecision (1) << " % error) show"<< endl;


	o
	<< "  " << fixed << setprecision (3) << P.O2X - P.R - P.A
	<< " "  << fixed << setprecision (3) << P.O2Y - P.R - 33.0 << " moveto" << endl;

	if (inset.clusternumber == "A") o << "  (Automatic clustering " << flush;
	else							o << "  (User defined clustering " << flush;

	o
	<< " into " << clusternumber << " clusters with "
	<< fixed << setprecision (1) << " % error) show"<< endl;
}

void PS_net (string DATATYPE, string LOC, ofstream& o, INPSET inset, PAPER P) {

	int angle;

	string filename = LOC + "_" + DATATYPE + ".PS";

	string nethemisphere = "Schmidt-net,";

	if (inset.plottype == "W")	nethemisphere = "Wulff-net,";

	if (inset.hemisphere == "U") nethemisphere = nethemisphere + " upper hemisphere";
	else nethemisphere = nethemisphere + " lower hemisphere";

	if (inset.plot == "H")  nethemisphere = "Hoeppener-plot, " + nethemisphere;
	else nethemisphere = "Angelier-plot, " + nethemisphere;


	o << endl;
	o << "  0.00 0.00 0.00 setrgbcolor 1 setlinewidth" << endl << endl;

	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O1X << " "
	<< fixed << setprecision (3) << P.O1Y << " " << P.R << " 0 360 arc stroke" << endl;
	o
	<< "  1.2 setlinewidth" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y + P.R + P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X + P.R << " " << P.O1Y << " moveto "
	<< fixed << setprecision (3) << P.O1X + P.R + P.C << " " << P.O1Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y - P.R - P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X - P.R << " " << P.O1Y << " moveto "
	<< fixed << setprecision (3) << P.O1X - P.R - P.C << " " << P.O1Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y - P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X + P.C << " " << P.O1Y << " moveto "
	<< fixed << setprecision (3) << P.O1X - P.C << " " << P.O1Y << " lineto stroke" << endl << endl;


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O2X << " "
	<< fixed << setprecision (3) << P.O2Y << " " << P.R << " 0 360 arc stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y + P.R + P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X + P.R << " " << P.O2Y << " moveto "
	<< fixed << setprecision (3) << P.O2X + P.R + P.C << " " << P.O2Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y - P.R - P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X - P.R << " " << P.O2Y << " moveto "
	<< fixed << setprecision (3) << P.O2X - P.R - P.C << " " << P.O2Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y - P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X + P.C << " " << P.O2Y << " moveto "
	<< fixed << setprecision (3) << P.O2X - P.C << " " << P.O2Y << " lineto stroke" << endl << endl;


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O3X << " "
	<< fixed << setprecision (3) << P.O3Y << " " << P.R << " 0 360 arc stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y + P.R + P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X + P.R << " " << P.O3Y << " moveto "
	<< fixed << setprecision (3) << P.O3X + P.R + P.C << " " << P.O3Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y - P.R - P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X - P.R << " " << P.O3Y << " moveto "
	<< fixed << setprecision (3) << P.O3X - P.R - P.C << " " << P.O3Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y - P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X + P.C << " " << P.O3Y << " moveto "
	<< fixed << setprecision (3) << P.O3X - P.C << " " << P.O3Y << " lineto stroke" << endl << endl;


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O4X << " "
	<< fixed << setprecision (3) << P.O4Y << " " << P.R << " 0 360 arc stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y + P.R + P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X + P.R << " " << P.O4Y << " moveto "
	<< fixed << setprecision (3) << P.O4X + P.R + P.C << " " << P.O4Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y - P.R - P.C << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X - P.R << " " << P.O4Y << " moveto "
	<< fixed << setprecision (3) << P.O4X - P.R - P.C << " " << P.O4Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y - P.C 		<< " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X + P.C << " " << P.O4Y << " moveto "
	<< fixed << setprecision (3) << P.O4X - P.C << " " << P.O4Y 		<< " lineto stroke" << endl << endl;


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O5X << " "
	<< fixed << setprecision (3) << P.O5Y << " " << P.R << " 270 360 arc stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O5X << " " << P.O5Y << " moveto "
	<< fixed << setprecision (3) << P.O5X + P.R + P.C << " " << P.O5Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O5X << " " << P.O5Y << " moveto "
	<< fixed << setprecision (3) << P.O5X << " " << P.O5Y - P.R - P.C << " lineto stroke" << endl;
	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O6X << " "
	<< fixed << setprecision (3) << P.O6Y << " " << P.R << " 270 360 arc stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O6X << " " << P.O6Y << " moveto "
	<< fixed << setprecision (3) << P.O6X + P.R + P.C << " " << P.O6Y << " lineto stroke" << endl;
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O6X << " " << P.O6Y << " moveto "
	<< fixed << setprecision (3) << P.O6X << " " << P.O6Y - P.R - P.C << " lineto stroke" << endl;


	angle = 0;

	o
	<< "  "  << fixed << setprecision (3) << P.O3X
	<< " "   << fixed << setprecision (3) << P.O3Y << " translate"  << endl << endl;

	do {

		o << "  0.0 "  << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << endl;

		angle = angle + 10;

	} while (angle < 360);

	o << "  " << fixed << setprecision (3) << - P.O3X
	  << " "  << fixed << setprecision (3) << - P.O3Y << " translate"  << endl;

	angle = 0;

	o << "  " << fixed << setprecision (3) << P.O4X
	  << " "  << fixed << setprecision (3) << P.O4Y << " translate"  << endl << endl;

	do {

		o << "  0.0 " << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << endl;

		angle = angle + 10;

	} while (angle < 360);

	o << "  "  << fixed << setprecision (3) << - P.O4X
	  << " "   << fixed << setprecision (3) << - P.O4Y << " translate"  << endl << endl;

	o << "  " << fixed << setprecision (3) << P.O5X
	  << " "  << fixed << setprecision (3) << P.O5Y << " translate"  << endl << endl;

	o << "  180 rotate" << endl;

	angle = 270;

	do {

		o << "  0.0 " << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << endl;

		angle = angle + 10;

	} while (angle < 360);

	o << "  -270 rotate" << endl;
	o << "  "  << fixed << setprecision (3) << - P.O5X
	  << " "   << fixed << setprecision (3) << - P.O5Y << " translate"  << endl << endl;

	o << "  " << fixed << setprecision (3) << P.O6X
	  << " "  << fixed << setprecision (3) << P.O6Y << " translate"  << endl << endl;
	o << "  180 rotate" << endl;

	angle = 270;

	do {

		o << "  0.0 " << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << endl;

		angle = angle + 10;

	} while (angle < 360);

	o << "  -270 rotate" << endl;
	o << "  "  << fixed << setprecision (3) << - P.O6X
	  << " "   << fixed << setprecision (3) << - P.O6Y << " translate"  << endl << endl;



	o << "/ArialNarrow-Bold findfont 12 scalefont setfont" << endl;
	o << "  " <<  fixed << setprecision (3) << P.O1X - 3.5 << " "
			  <<  fixed << setprecision (3) << P.O1Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << endl;
	o << "  " <<  fixed << setprecision (3) << P.O2X - 3.5 << " "
			  <<  fixed << setprecision (3) << P.O2Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << endl;
	o << "  " <<  fixed << setprecision (3) << P.O3X - 3.5 << " "
			  <<  fixed << setprecision (3) << P.O3Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << endl;
	o << "  " <<  fixed << setprecision (3) << P.O4X - 3.5 << " "
	 		  <<  fixed << setprecision (3) << P.O4Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << endl;



	o << "/ArialNarrow findfont 8 scalefont setfont" << endl;
	o << "  " <<  fixed << setprecision (3) << P.O1X - P.R - 0.2 * P.B
	  << " "  <<  fixed << setprecision (3) << P.O1Y - P.R - 20.0 << " moveto"
	  << "  (" << nethemisphere << ") show" << endl;
	o << "  " <<  fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
	  << " "  <<  fixed << setprecision (3) << P.O2Y - P.R - 20.0 << " moveto"
	  << "  (" << nethemisphere << ") show" << endl;

	o << 20.0 * P.A << " " << P.A + 5.0 << " moveto"
	  << "  (Plotted by SG2PS software, version 0.2.0 - for reference see www.kadath.hu/sg2ps webpage.) show" << endl << endl;
}

void PS_stressdata (ofstream& o, CENTER center, PAPER P, STRESSFIELD sf, string method) {

	o
	<< "/ArialNarrow findfont 8 scalefont setfont" << endl;
	o
	<< "0 0 0 setrgbcolor" << endl;
	o
	<< "  " << fixed << setprecision (3) << center.X + (center.radius / 2.0) - P.A
	<< " "  << fixed << setprecision (3) << center.Y - center.radius - 20.0 << " translate" << endl;
	o
	<< "   0 0 moveto " << endl;

	if 		(method == "BINGHAM")		o << "(Fracture statistics after Bingham (1964) ) show" << endl;
	else if (method == "NDA")  			o << "(Regression after Sprang (1972) ) show" << endl;
	else if (method == "FRY") 			o << "(Regression after Fry (1999) ) show" << endl;
	else if (method == "SHAN") 			o << "(Regression after Shan et al. (2003) ) show" << endl;
	else if (method == "ANGELIER") 		o << "(Inversion after Angelier (1990) ) show" << endl;
	else if (method == "PTN") 			o << "(Regression after Turner (1953) ) show" << endl;
	else if (method == "MICHAEL") 		o << "(Inversion after Michael (1984) ) show" << endl;
	else if (method == "MOSTAFA") 		o << "(Inversion after Mostafa (2005) ) show" << endl;
	else {}

	o
	<< "   0.0 -8.0 moveto" << endl;

	if (method == "BINGHAM")  {

		o
		<< "(e1: "  << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_1.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_1.DIP
		<< ", e2: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_2.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_2.DIP
		<< ", e3: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_3.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_3.DIP << ") show" <<endl;
	}

	else {

		o
		<< "(S1: "  << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_1.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_1.DIP
		<< ", S2: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_2.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_2.DIP
		<< ", S3: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_3.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_3.DIP << ") show" <<endl;
	}

	if (method != "BINGHAM") {

		o
		<< "   0.0 -16.0 moveto" << endl;

		o
		<< "(R: "   << setw (4) << fixed << setprecision (3) << sf.stressratio
		<< ", R': " << setw (4) << fixed << setprecision (3) << sf.delvaux_str
		<< ", "     << sf.delvaux_rgm << ")  show" <<endl;
	}

	else  {

		o
		<< "   0.0 -16.0 moveto" << endl;

		o
		<<  "(E1: " << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.X * 100.0 << "%"
		<< ", E2: " << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Y * 100.0 << "%"
		<< ", E3: " << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Z * 100.0 << "%) show" <<endl;
	}

	o
	<< "  " << fixed << setprecision (3) << - center.X - (center.radius / 2.0) + P.A
	<< " "  << fixed << setprecision (3) << - center.Y + center.radius + 20.0 << " translate" << endl;

}

void PS_stressarrows (ofstream& o, CENTER center, PAPER P, STRESSFIELD sf) {

	if ((sf.regime == "COMPRESSIONAL") || (sf.regime == "STRIKE-SLIP")) {

		o << fixed << setprecision (3) << center.X << " "
		  << fixed << setprecision (3) << center.Y << " translate" << endl;

		o << "  " << - sf.shmax << fixed << setprecision (3) << " rotate" << endl;
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<endl;
		o << "  newpath compression_arrow" << endl;
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<endl;
		o << "  " << sf.shmax << fixed << setprecision (3) << " rotate" << endl;

		o << "  180 rotate" << endl;

		o << "  " << - sf.shmax << fixed << setprecision (3) << " rotate" << endl;
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<endl;
		o << "  newpath compression_arrow" << endl;
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<endl;
		o << "  " << sf.shmax << fixed << setprecision (3) << " rotate" << endl;

		o << "  180 rotate" << endl;

		o << fixed << setprecision (3) << - center.X << " "
		  << fixed << setprecision (3) << - center.Y << " translate" << endl;
	}

	if ((sf.regime == "EXTENSIONAL") || (sf.regime == "STRIKE-SLIP")) {

		o << fixed << setprecision (3) << center.X << " "
		  << fixed << setprecision (3) << center.Y << " translate" << endl;

		o << "  " << - sf.shmin << fixed << setprecision (3) << " rotate" << endl;
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<endl;
		o << "  newpath extension_arrow" << endl;
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<endl;
		o << "  " << sf.shmin << fixed << setprecision (3) << " rotate" << endl;

		o << "  180 rotate" << endl;

		o << "  " << - sf.shmin << fixed << setprecision (3) << " rotate" << endl;
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<endl;
		o << "  newpath extension_arrow" << endl;
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<endl;
		o << "  " << sf.shmin << fixed << setprecision (3) << " rotate" << endl;

		o << "  180 rotate" << endl;

		o << fixed << setprecision (3) << - center.X << " "
		  << fixed << setprecision (3) << - center.Y << " translate" << endl;
	}
}

void PS_mohr_circle (vector <GDB> inGDB, ofstream& o, CENTER mohr_center, PAPER P, STRESSFIELD sf, STRESSTENSOR st, bool compression_positive) {

	const double X = 5.0 * P.A;
	const double fi = sf.stressratio;

	const double S1 = sf.EIGENVALUE.X;
	const double S3 = sf.EIGENVALUE.Z;

	size_t j = 0;

	double normal_s = 0.0;
	double shear_s = 0.0;

	double stress_magnitude;

	VCTR stressvector;
	VCTR color;

	color = generarte_stress_colors (sf.delvaux_str);

	o << "newpath" << endl;
	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + 2.5 * P.A
	<< " "  << fixed << setprecision (3) << mohr_center.Y
	<< " "  << fixed << setprecision (3) << 2.5 * P.A
	<< " 0 180 arc 3 setlinewidth "
	<< 		  fixed << setprecision (2) << color.X
	<< " " << fixed << setprecision (2) << color.Y
	<< " " << fixed << setprecision (2) << color.Z
	<< " setrgbcolor stroke " << endl;

	o << "newpath" << endl;
	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + (0.5 * fi * X)
	<< " "  << fixed << setprecision (3) << mohr_center.Y
	<< " "  << fixed << setprecision (3) << 0.5 * fi * X
	<< " 0 180 arc 3 setlinewidth "
	<< 		  fixed << setprecision (2) << color.X
	<< " " << fixed << setprecision (2) << color.Y
	<< " " << fixed << setprecision (2) << color.Z
	<< " setrgbcolor stroke " << endl;

	o << "newpath" << endl;
	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + 2.5 * P.A + (0.5 * fi * X)
	<< " "  << fixed << setprecision (3) << mohr_center.Y
	<< " "  << fixed << setprecision (3) << 2.5 * P.A - (0.5 * fi * X)
	<< " 0 180 arc 3 setlinewidth "
	<< 		  fixed << setprecision (2) << color.X
	<< " " << fixed << setprecision (2) << color.Y
	<< " " << fixed << setprecision (2) << color.Z
	<< " setrgbcolor stroke " << endl;
	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;

	o
	<< "  " << fixed << setprecision (3) << mohr_center.X - 4.0
	<< " "  << fixed << setprecision (3) << mohr_center.Y - 10.0
	<< "  moveto (s3) 0 0 0 setrgbcolor show " << endl;

	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + X - 4.0
	<< " "  << fixed << setprecision (3) << mohr_center.Y - 10.0
	<< "  moveto (s1) 0 0 0 setrgbcolor show " << endl;

	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + (fi * X) - 4.0
	<< " "  << fixed << setprecision (3) << mohr_center.Y - 10.0
	<< "  moveto (s2) 0 0 0 setrgbcolor show " << endl;

	do {

		stressvector = return_stressvector (st, inGDB.at(j), compression_positive);

		stress_magnitude =
				(inGDB.at(j).N.X * stressvector.X) +
				(inGDB.at(j).N.Y * stressvector.Y) +
				(inGDB.at(j).N.Z * stressvector.Z);

		if (inGDB.at(j).DATATYPE == "STRIAE") {

			normal_s = sqrt(
					inGDB.at(j).NORMAL_S.X * inGDB.at(j).NORMAL_S.X +
					inGDB.at(j).NORMAL_S.Y * inGDB.at(j).NORMAL_S.Y +
					inGDB.at(j).NORMAL_S.Z * inGDB.at(j).NORMAL_S.Z);


			if ((!(compression_positive)) && (stress_magnitude > 0.0)) normal_s = - normal_s;

			if (( (compression_positive)) && (stress_magnitude < 0.0)) normal_s = - normal_s;

			normal_s = ((normal_s - S3) / (S1 - S3));

			shear_s = sqrt(
					inGDB.at(j).SHEAR_S.X * inGDB.at(j).SHEAR_S.X +
					inGDB.at(j).SHEAR_S.Y * inGDB.at(j).SHEAR_S.Y +
					inGDB.at(j).SHEAR_S.Z * inGDB.at(j).SHEAR_S.Z);

			shear_s = (shear_s / (S1 - S3));

			o
			<< "newpath "
			<< fixed << setprecision (3) << mohr_center.X + X * normal_s << " "
			<< fixed << setprecision (3) << mohr_center.Y + X * shear_s
			<< " 0.7 0 360 arc 3 setlinewidth 1 1 1 setrgbcolor stroke" << endl;

			o
			<< "newpath "
			<< fixed << setprecision (3) << mohr_center.X + X * normal_s << " "
			<< fixed << setprecision (3) << mohr_center.Y + X * shear_s
			<< " 0.7 0 360 arc 2 setlinewidth 0 0 0 setrgbcolor stroke" << endl;
		}

		j++;

	} while (j < inGDB.size());
}

void PS_RUP_distribution (vector <GDB>  inGDB, ofstream& o, CENTER center, PAPER P) {

	double RUP_max = 0.0;

	size_t i = 0;
	size_t j = 0;

	double step = 0.0;
	double counter = 0.0;

	int data_counter;

	do {

		if (inGDB.at(i).RUP > RUP_max) RUP_max = inGDB.at(i).RUP;

		i++;

	} while (i < inGDB.size());

	step = RUP_max / 10.0;
	counter = 0.0;
	i = 0;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;

	o
	<< "  " << fixed << setprecision (3) << center.X + P.R + 2.0 * P.B + 5.0
	<< " "  << fixed << setprecision (3) << center.Y + P.R + 7.0
	<< "  moveto (RUP) 	0 0 0 setrgbcolor show " << endl;

	do {

		data_counter = 0;
		i = 0;

		do {

			if ((inGDB.at(i).RUP > counter) && (inGDB.at(i).RUP <= counter + step)) data_counter++;
			i++;

		} while (i < inGDB.size());

		o << "  0.8 0.8 0.8 setrgbcolor " << data_counter << " setlinewidth" << endl;

		o
		<< "  newpath "
		<< fixed << setprecision (3) << center.X + P.R + 2.0 * P.B + data_counter / 2.0 << " " << center.Y - P.R + 2.0 * P.R * (counter / RUP_max) << " moveto "
		<< fixed << setprecision (3) << center.X + P.R + 2.0 * P.B + data_counter / 2.0 << " " << center.Y - P.R + 2.0 * P.R * ((counter + step) / RUP_max)  << " lineto stroke" << endl;

		counter = counter + step;

		j++;

	} while (j < 10);

	counter = 0.0;
	step = 0.0;

	if (RUP_max < 1.0) step = 0.2;
	else {

		if (RUP_max < 5.0) step = 1;
		else {

			if (RUP_max < 10) step = 2;
			else {

				if (RUP_max < 20) step = 4;
				else {

					if (RUP_max < 50) step = 10;
					else {

						if (RUP_max < 100) step = 20;
						else {

							if (RUP_max < 250) step = 50;
							else step = RUP_max / 5.0;
						}
					}
				}
			}
		}
	}

	o << fixed << setprecision (0) << endl;

	o << "/ArialNarrow-Bold findfont 6 scalefont setfont" << endl;
	o << "  0.0 0.0 0.0 setrgbcolor 0.5 setlinewidth" << endl;

	do {

		o << "  0.0 0.0 0.0 setrgbcolor 0.5 setlinewidth" << endl;
		o
		<< "  newpath "
		<< fixed << setprecision (3) << center.X + P.R + 2.0 * P.B << " " << center.Y - P.R + 2.0 * P.R * (counter / RUP_max) << " moveto "
		<< fixed << setprecision (3) << center.X + P.R + 2.2 * P.B << " " << center.Y - P.R + 2.0 * P.R * (counter / RUP_max)  << " lineto stroke" << endl;

		o
		<< "  " << fixed << setprecision (3) << center.X + P.R + 2.4 * P.B
		<< " "  << fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R * (counter / RUP_max) - 1.0
		<< fixed << setprecision (0)
		<< "  moveto (" << counter << "%) 0 0 0 setrgbcolor show " << endl;

		counter = counter + step;

	} while (counter < RUP_max);

	o
	<< "  newpath "
	<< fixed << setprecision (3) << center.X + P.R + 2.0 * P.B << " " << center.Y - P.R + 2.0 * P.R  << " moveto "
	<< fixed << setprecision (3) << center.X + P.R + 2.2 * P.B << " " << center.Y - P.R + 2.0 * P.R  << " lineto stroke" << endl;

	o
	<< "  " << fixed << setprecision (3) << center.X + P.R + 2.4 * P.B
	<< " "  << fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R - 1.0
	<< fixed << setprecision (0)
	<< "  moveto (" << RUP_max << "%) 0 0 0 setrgbcolor show " << endl;

	i = 0;

	do {

		o
		<< "newpath "
		<< fixed << setprecision (3) << center.X + P.R + 2.0 * P.B << " "
		<< fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R * (inGDB.at(i).RUP / RUP_max)
		<< " 0.7 0 360 arc 3 setlinewidth 1 1 1 setrgbcolor stroke" << endl;

		o
		<< "newpath "
		<< fixed << setprecision (3) << center.X + P.R + 2.0 * P.B << " "
		<< fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R * (inGDB.at(i).RUP / RUP_max)
		<< " 0.7 0 360 arc 2 setlinewidth 0 0 0 setrgbcolor stroke" << endl;

		i++;

		} while (i < inGDB.size());
}

void PS_ANG_distribution (vector <GDB>  inGDB, ofstream& o, CENTER center, PAPER P) {

	double ANG_max = 0.0;

	size_t i = 0;
	size_t j = 0;

	double step = 0.0;
	double  counter = 0.0;
	int data_counter;

	do {

		if (inGDB.at(i).ANG > ANG_max) ANG_max = inGDB.at(i).ANG;

		i++;

	} while (i < inGDB.size());

	step = ANG_max / 10.0;
	counter = 0.0;
	i = 0;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;

		o
		<< "  " << fixed << setprecision (3) << center.X + P.R + 4.0 * P.B + 5.0
		<< " "  << fixed << setprecision (3) << center.Y + P.R + 7.0
		<< "  moveto (ANG) 	0 0 0 setrgbcolor show " << endl;

	do {

		data_counter = 0;
		i = 0;

		do {

			if ((inGDB.at(i).ANG > counter) && (inGDB.at(i).ANG <= counter + step)) data_counter++;
			i++;

		} while (i < inGDB.size());

		o << "  0.8 0.8 0.8 setrgbcolor " << data_counter << " setlinewidth" << endl;

		o
		<< "  newpath "
		<< fixed << setprecision (3) << center.X + P.R + 4.0 * P.B + data_counter / 2.0 << " " << center.Y - P.R + 2.0 * P.R * (counter / ANG_max) << " moveto "
		<< fixed << setprecision (3) << center.X + P.R + 4.0 * P.B + data_counter / 2.0 << " " << center.Y - P.R + 2.0 * P.R * ((counter + step) / ANG_max)  << " lineto stroke" << endl;

		counter = counter + step;

		j++;

	} while (j < 10);

	counter = 0.0;
	step = 0.0;

	if (ANG_max < 1.0) step = 0.2;
	else {

		if (ANG_max < 5.0) step = 1;
		else {

			if (ANG_max < 10) step = 2;
			else {

				if (ANG_max < 20) step = 4;
				else {

					if (ANG_max < 50) step = 10;
					else {

						if (ANG_max < 100) step = 20;
						else {

							if (ANG_max < 250) step = 50;
							else step = ANG_max / 5.0;
						}
					}
				}
			}
		}
	}

	o << fixed << setprecision (0) << endl;

	o << "/ArialNarrow-Bold findfont 6 scalefont setfont" << endl;
	o << "  0.0 0.0 0.0 setrgbcolor 0.5 setlinewidth" << endl;

	do {

		o << "  0.0 0.0 0.0 setrgbcolor 0.5 setlinewidth" << endl;

		o
		<< "  newpath "
		<< fixed << setprecision (3) << center.X + P.R + 4.0 * P.B << " " << center.Y - P.R + 2.0 * P.R * (counter / ANG_max) << " moveto "
		<< fixed << setprecision (3) << center.X + P.R + 4.2 * P.B << " " << center.Y - P.R + 2.0 * P.R * (counter / ANG_max)  << " lineto stroke" << endl;

		o
		<< "  " << fixed << setprecision (3) << center.X + P.R + 4.4 * P.B
		<< " "  << fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R * (counter / ANG_max) - 1.0
		<< fixed << setprecision (0)
		<< "  moveto (" << counter << " deg) 0 0 0 setrgbcolor show " << endl;

		counter = counter + step;

	} while (counter < ANG_max);

	o
	<< "  newpath "
	<< fixed << setprecision (3) << center.X + P.R + 4.0 * P.B << " " << center.Y - P.R + 2.0 * P.R  << " moveto "
	<< fixed << setprecision (3) << center.X + P.R + 4.2 * P.B << " " << center.Y - P.R + 2.0 * P.R  << " lineto stroke" << endl;

	o
	<< "  " << fixed << setprecision (3) << center.X + P.R + 4.4 * P.B
	<< " "  << fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R - 1.0
	<< fixed << setprecision (0)
	<< "  moveto (" << ANG_max << " deg) 0 0 0 setrgbcolor show " << endl;

	i = 0;

	do {

		o
		<< "newpath "
		<< fixed << setprecision (3) << center.X + P.R + 4.0 * P.B << " "
		<< fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R * (inGDB.at(i).ANG / ANG_max)
		<< " 0.7 0 360 arc 3 setlinewidth 1 1 1 setrgbcolor stroke" << endl;

		o
		<< "newpath "
		<< fixed << setprecision (3) << center.X + P.R + 4.0 * P.B << " "
		<< fixed << setprecision (3) << center.Y - P.R + 2.0 * P.R * (inGDB.at(i).ANG / ANG_max)
		<< " 0.7 0 360 arc 2 setlinewidth 0 0 0 setrgbcolor stroke" << endl;

		i++;

		} while (i < inGDB.size());
}

void PS_stress_state (ofstream& o, PAPER P, CENTER center, STRESSFIELD sf) {

	double value = sf.delvaux_str;

	o << " 1 1 1 setrgbcolor 5 setlinewidth" << endl;

	o
	<< "  newpath "
	<< fixed << setprecision (3) << center.X + P.R + 0.9 * P.B << " " << center.Y - P.R + (0.666 * P.R * value) << " moveto "
	<< fixed << setprecision (3) << center.X + P.R + 1.6 * P.B << " " << center.Y - P.R + (0.666 * P.R * value)
	<< " lineto stroke" << endl;
}

void PS_folddata (GDB in, ofstream& o, CENTER center) {

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;

	o << "0 0 0 setrgbcolor" << endl;

	o
	<< "  " << fixed << setprecision (3) << center.X + (center.radius / 2.0)
	<< " "  << fixed << setprecision (3) << center.Y - center.radius << " translate" << endl;

	o
	<< "   0 0 moveto "
	<< "(Fold great circle: "
	<< setfill ('0') << setw (3) << fixed << setprecision (0) << in.avd.DIPDIR << "/"
	<< setfill ('0') << setw (2) << fixed << setprecision (0) << in.avd.DIP
	<< ") show" <<endl;

	o
	<< "  " << fixed << setprecision (3) << - center.X - (center.radius / 2.0)
	<< " "  << fixed << setprecision (3) << - center.Y + center.radius << " translate" << endl;
}

void PS_lineation (GDB i, ofstream& o, INPSET inset, CENTER center, STRESSFIELD sf, bool label, string type) {

	VCTR temp;
	DIPDIR_DIP dd;

	if		((type == "S1") || (type == "S1_ITER")) temp = sf.EIGENVECTOR1;

	else if	((type == "S2") || (type == "S2_ITER"))	temp = sf.EIGENVECTOR2;

	else if	((type == "S3") || (type == "S3_ITER"))	temp = sf.EIGENVECTOR3;


	else if (type == "P") {

		temp = i.ptnP;
		dd = i.ptnPd;
	}

	else if (type == "N") {

		temp = i.ptnN;
		dd = i.ptnNd;
	}

	else if (type == "T") {

		temp = i.ptnT;
		dd = i.ptnTd;
	}

	else {

		temp = i.D;
		dd = i.corr;
	}

	if (temp.Z > 0.0) temp.Z = - temp.Z;

	double X = (temp.X / (1.00 - temp.Z)) * center.radius;
	double Y = (temp.Y / (1.00 - temp.Z)) * center.radius;

	if (inset.plottype == "S") {

		X = (temp.X / sqrt (1.00 - temp.Z)) * center.radius;
		Y = (temp.Y / sqrt (1.00 - temp.Z)) * center.radius;
	}

	if (inset.hemisphere == "U") {

		X = - X;
		Y = - Y;
	}

	X = X + center.X;
	Y = Y + center.Y;

	if (type == "S1") {

		o
		<< "  " << fixed << setprecision (5) << X
		<< "  " << fixed << setprecision (5) << Y << " translate " << endl;
		o
		<< "   newpath s1_axis" << endl;
		o
		<< "  " << fixed << setprecision (5) << -X
		<< "  " << fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else if (type == "S1_ITER") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath s1_iter_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else if (type == "S2") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath s2_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else if (type == "S2_ITER") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath s2_iter_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else if (type == "S3") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath s3_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else if (type == "S3_ITER") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath s3_iter_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << endl;
	}


	else if (type == "P") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath p_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else if (type == "N") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				  << fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath n_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				  << fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else if (type == "T") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				  << fixed << setprecision (5) <<  Y << " translate " << endl;
		o << "  newpath t_axis" << endl;
		o << "  " << fixed << setprecision (5) << -X << " "
				  << fixed << setprecision (5) << -Y << " translate " << endl;
	}

	else {

		o << "  " << i.PSCOLOR << " setrgbcolor newpath "
		  << fixed << setprecision (5) << X << " "
		  << fixed << setprecision (5) << Y << " 0.7 0 360 arc 0 fill stroke" << endl;
		o << "%%  " << setfill ('0') << setw (3) << setprecision (3) << dd.DIPDIR << "/" << setfill ('0') << setw (3) << setprecision (3) << dd.DIP << endl << endl;
		o << "  " << i.PSCOLOR << " setrgbcolor newpath "
		  << fixed << setprecision (5) << X << " "
		  << fixed << setprecision (5) << Y << " 0.7 0 360 arc 0 stroke" << endl;
	}

	if (label) {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << endl;
		o
		<< "  " << fixed << setprecision (3) << X << " " << fixed << setprecision (3) << Y  <<" translate" << endl;

		o
		<< "  0.0 0.0 moveto 10.0 10.0 lineto stroke" << endl;
		o
		<< "  10.0 10.0 moveto ("	<< i.ID << ") show"	<< endl;

		o
		<< "  " << fixed << setprecision (3) << - X << " " << fixed << setprecision (3) << - Y  <<" translate" << endl;
	}
}

void PS_plane (GDB i, ofstream& o, INPSET inset, CENTER center, bool label, string type) {

	int j = 0;
	int steps = 2;
	double X_A, Y_A, X_B, Y_B, X_C, Y_C, X_O, Y_O;
	double b , c, alfa, r;

	vector <VCTR> PP;
	VCTR temp, torotate, axis;

	if (inset.plottype == "S") steps = 120;

	if (type == "C") {

		i.corr = i.corrL;
		i.D.Z = i.DC.Z;
		i.N = i.NC;
	}

	else if (type == "AV") {

		i.corr = i.avS0d;
		i.D.Z = i.avS0D.Z;
		i.N = i.avS0N;
	}

	else if (type == "FOLD") {

		i.corr = i.avd;

		i.D  = 	i.avD;

		i.N    = 	unitvector(NXNYNZ_from_DXDYDZ (i.D));
	}

	else {}


	double DIPDIR = i.corr.DIPDIR;

	if (inset.hemisphere == "U") DIPDIR = DIPDIR + 180.0;

	torotate = declare_vector(SIN(i.corr.DIPDIR - 90.0), COS(i.corr.DIPDIR - 90.0), 0.0);
	axis = i.N;

	j = 0;

	do {

		temp = ROTATE (axis, torotate, - (180.0 / steps) * j);
		PP.push_back(temp);
		j++;

	} while (j < steps + 1);

	j = 0;

	if (inset.hemisphere == "U") {

		do {

			PP.at(j).X = - PP.at(j).X;
			PP.at(j).Y = - PP.at(j).Y;
			j++;

		} while (j < steps + 1);
	}

	j = 0;

	if (inset.plottype == "W") {

		do {

			PP.at(j).X = (PP.at(j).X / (1.00 - PP.at(j).Z)) * center.radius + center.X;
			PP.at(j).Y = (PP.at(j).Y / (1.00 - PP.at(j).Z)) * center.radius + center.Y;
			j++;

		} while (j < steps + 1);
	}

	else {

		do {

			PP.at(j).X = (PP.at(j).X / sqrt(1.00 - PP.at(j).Z)) * center.radius + center.X;
			PP.at(j).Y = (PP.at(j).Y / sqrt(1.00 - PP.at(j).Z)) * center.radius + center.Y;
			j++;

		} while (j < steps + 1);
	}

	X_A = PP.at(steps / 2).X;	Y_A = PP.at(steps / 2).Y;
	X_B = PP.at(steps).X;		Y_B = PP.at(steps).Y;
	X_C = PP.at(0).X;			Y_C = PP.at(0).Y;

	b = sqrt ((X_A - X_C) * (X_A - X_C) + (Y_A - Y_C) * (Y_A - Y_C));
	c = sqrt ((X_C - X_B) * (X_C - X_B) + (Y_C - Y_B) * (Y_C - Y_B)) / 2.0;

	alfa = ACOS (c / b) * 2.0;

	r =  c / SIN(alfa );

	X_O = X_A - r * SIN(DIPDIR);
	Y_O = Y_A - r * COS(DIPDIR);

	if ((i.corr.DIPDIR > 90.0) && (i.corr.DIPDIR < 180.0)) {

		X_O = X_A - r * SIN(180.0 - DIPDIR);
		Y_O = Y_A + r * COS(DIPDIR - 180.0);
	}

	if ((i.corr.DIPDIR > 180.0) && (i.corr.DIPDIR < 270.0)) {

		X_O = X_A + r * SIN(DIPDIR - 180.0);
		Y_O = Y_A + r * COS(DIPDIR - 180.0);
	}

	if ((i.corr.DIPDIR > 270.0) && (i.corr.DIPDIR < 360.0)) {

		X_O = X_A + r * SIN(360.0 - DIPDIR);
		Y_O = Y_A - r * COS(DIPDIR - 360.0);
	}

	if (type == "AV") {

		o
		<< "%%____" << setfill ('0') << fixed << setprecision (5) << i.corr.DIPDIR <<
				"/" << setfill ('0') << fixed << setprecision (5) << i.corr.DIP << endl;
		o << "  1.00 0.00 0.00 setrgbcolor 1 setlinewidth newpath " << endl;
	}

	else if (type == "FOLD") {

		o << "%%____" << setfill ('0') << fixed << setprecision (5) << i.corr.DIPDIR <<
				  "/" << setfill ('0') << fixed << setprecision (5) << i.corr.DIP << endl;
		o << "  0.00 0.00 1.00 setrgbcolor 1 setlinewidth newpath " << endl;
	}

	else {

		o << "%%____" << setfill ('0') << fixed << setprecision (5) << i.corr.DIPDIR <<
				"/" << setfill ('0') << fixed << setprecision (5) << i.corr.DIP << endl;
		o << "  " << i.PSCOLOR << " setrgbcolor " << inset.linewidth << " setlinewidth newpath " << endl;
	}

	if (inset.plottype == "W") {

		double begin_arc = 0.0;

		if 	    ((X_B > X_O) && (Y_B > Y_O)) begin_arc =         ASIN((Y_B - Y_O) / r);
		else if ((X_B < X_O) && (Y_B > Y_O)) begin_arc = 180.0 - ASIN((Y_B - Y_O) / r);
		else if ((X_B < X_O) && (Y_B < Y_O)) begin_arc = 180.0 + ASIN((Y_O - Y_B) / r);
		else begin_arc = 360.0 - ASIN((Y_O - Y_B) / r);

		double end_arc = begin_arc + (2.0 * alfa);

		o
		<< "  " << fixed << setprecision (5) << X_O
		<< " "  << fixed << setprecision (5) << Y_O
		<< " "  << fixed << setprecision (5) << r
		<< " "  << fixed << setprecision (5) << begin_arc
		<< " "  << fixed << setprecision (5) << end_arc << " arc "
		<< endl;
	}

	else {

		j = 1;
		o << PP.at(0).X << " " << PP.at(0).Y << " moveto" << endl;

		do {

			o << PP.at(j).X << " " << PP.at(j).Y << " " << flush;
			j++;

			o << PP.at(j).X << " " << PP.at(j).Y << " " << flush;
			j++;

			o << PP.at(j).X << " " << PP.at(j).Y << " curveto" << endl;
			j++;

		} while (j < steps + 1);
	}

	if (i.OFFSET == "OVERTURNED") {

		o << "  [6 6] 0 setdash stroke" << endl;
		o << "  [   ] 0 setdash " << endl;
	}

	else if (type == "FOLD") {

		o << "  [3 3] 0 setdash stroke" << endl;
		o << "  [   ] 0 setdash " << endl;
	}

	else if (type == "C") {

		o << "  [3 3] 0 setdash stroke" << endl;
		o << "  [   ] 0 setdash " << endl;
	}

	else {

		o << "stroke" << endl << endl;
	}

	if (label) {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << endl;
		o
		<< "  " << fixed << setprecision (3) << X_C << " " << fixed << setprecision (3) << Y_C  <<" translate" << endl;
		o
		<< "  " <<  - i.corr.DIPDIR + 180.0 << " rotate" << endl;
		o
		<< "  0.0 0.0 moveto 10.0 0.0 lineto stroke" << endl;
		o
		<< "  10.0 -3.0 moveto ("	<< i.ID << ") show"	<< endl;

		o
		<< "  " << + i.corr.DIPDIR - 180.0 << " rotate" << endl;

		o
		<< "  " << fixed << setprecision (3) << - X_C << " " << fixed << setprecision (3) << - Y_C  <<" translate" << endl;
	}
}

void PS_polepoint (GDB i, ofstream& o, INPSET inset, CENTER center, bool label, string type) {

	VCTR temp;
	VCTR in;
	DIPDIR_DIP dd;

	if (type == "FOLD") {

		dd = i.corr;
		temp = NXNYNZ_from_dipdir_dip (dd);
		in.X = - temp.X;
		in.Y = - temp.Y;
		in.Z =   temp.Z;
	}

	else if (type == "AV")  {

		temp = i.avS0D;
		temp = NXNYNZ_from_DXDYDZ (temp);
		in.X = - temp.X;
		in.Y = - temp.Y;
		in.Z =   temp.Z;
	}

	else {

		in.X = - i.N.X;
		in.Y = - i.N.Y;
		in.Z =   i.N.Z;
	}

	if (inset.plottype == "S") {

		in.X = (in.X / sqrt (1.00 + in.Z)) * center.radius;
		in.Y = (in.Y / sqrt (1.00 + in.Z)) * center.radius;
	}

	else {

		in.X = (in.X / (1.00 + in.Z)) * center.radius;
		in.Y = (in.Y / (1.00 + in.Z)) * center.radius;
	}

	if (inset.hemisphere == "U") {

		in.X = - in.X;
		in.Y = - in.Y;
	}

	in.X = in.X + center.X;
	in.Y = in.Y + center.Y;

	o << "%%" << endl << "%%" << endl;
	o << "%%  " << setfill ('0') << setw (3) << setprecision (3) << i.corr.DIPDIR 	<<
			 "/"<< setfill ('0') << setw (3) << setprecision (3) << i.corr.DIP 		<< "----------------------------------------" << endl;

	if (type == "AV") {

		o << "  1.0 0.0 0.0 setrgbcolor 0.7 setlinewidth" << endl;
		o
		<< "  newpath " << fixed << setprecision (3) << in.X
		<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << endl;
		o << "   0 0 0 setrgbcolor" << endl;
	}

	else if (type == "FOLD") {

		o << "  0.0 0.0 1.0 setrgbcolor 0.7 setlinewidth" << endl;
		o
		<< "  newpath " << fixed << setprecision (3) << in.X
		<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << endl;
		o << "   0 0 0 setrgbcolor" << endl;
	}

	else if (type == "C") {

		o << "  0.0 0.0 1.0 setrgbcolor 0.7 setlinewidth" << endl;
		o
		<< "  newpath " << fixed << setprecision (3) << in.X
		<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 0.9 0.9 0.9 setrgbcolor fill grestore stroke" << endl;
		o << "  0 0 0 setrgbcolor" << endl;
	}

	else {

		o << "  " << i.PSCOLOR << " setrgbcolor 0.7 setlinewidth" << endl;
		o << "  newpath " << fixed << setprecision (3) << in.X
				<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << endl;
		o << "  0 0 0 setrgbcolor" << endl;
	}

	if (label) {

		o << "/ArialNarrow-Italic findfont 6 scalefont setfont" << endl
		  << "  " << i.PSCOLOR << " setrgbcolor " << in.X + 3.0 << " " << in.Y - 3.0 << " moveto ("
		  << setfill ('0') << setw (3) << setprecision (3) << i.corr.DIPDIR << "/"
		  << setfill ('0') << setw (3) << setprecision (3) << i.corr.DIP << ") show" << endl
		  << "0 0 0 setrgbcolor" << endl;
	}
}

void PS_sc_arrow (GDB i, ofstream& o, INPSET inset, CENTER center, VCTR d) {

	DIPDIR_DIP dd = dipdir_dip_from_DXDYDZ (d);

	double X, Y;

	VCTR temp1, temp2;

	temp1 = i.DC;
	temp2 = i.N;

	temp1 = crossproduct (temp1, temp2);

	if (inset.plot == "H") {

		d.X = - i.NC.X;
		d.Y = - i.NC.Y;
		d.Z = - i.NC.Z;
	}

	X = (d.X / (1.00 - d.Z)) * center.radius;
	Y = (d.Y / (1.00 - d.Z)) * center.radius;

	if (inset.plottype == "S") {

		X = (d.X / (sqrt(1.00 - d.Z))) * center.radius;
		Y = (d.Y / (sqrt(1.00 - d.Z))) * center.radius;
	}

	if (inset.hemisphere == "U") {

		X = - X;
		Y = - Y;
	}

	X = X + center.X;
	Y = Y + center.Y;

	o << "  " << inset.linewidth << "  setlinewidth" << endl;
	o << "  " << i.PSCOLOR << "  setrgbcolor newpath "
			<< setw (3) << setprecision (5) << X << " "
			<< setw (3) << setprecision (5) << Y << " 1 0 360 arc 0 " << endl;
	o << "  closepath fill stroke" << endl;
	o << "  " << setw (3) << setprecision (5) << X << " "
			<< setw (3) << setprecision (5) << Y << " translate " << endl;

	if (d.Z > 0.0) {

		o << - dd.DIPDIR + 180.0 << " rotate" << endl;
		o << "  newpath normalarrow" << endl;
		o << "  " << dd.DIPDIR - 180.0 << " rotate " << endl;
	}

	else {

		o << - dd.DIPDIR + 000.0 << " rotate" << endl;
		o << "  newpath normalarrow" << endl;
		o << "  " << dd.DIPDIR + 000.0 << " rotate " << endl;
	}

	o  << fixed << setprecision (3) << -X << " "	<< fixed << setprecision (5)<< -Y << " translate " << endl;
}

void PS_striaearrow (GDB i, ofstream& o, INPSET inset, CENTER center, bool label) {

	double DIPDIR = i.corrL.DIPDIR;

	VCTR temp1 = i.DC;
	VCTR temp2 = i.N;

	temp1 = crossproduct (temp1, temp2);

	if (inset.plot == "H") {

		i.DC.X = - i.N.X;
		i.DC.Y = - i.N.Y;
		i.DC.Z = - i.N.Z;
	}

	double X = (i.DC.X / (1.00 - i.DC.Z)) * center.radius;
	double Y = (i.DC.Y / (1.00 - i.DC.Z)) * center.radius;

	if (inset.plottype == "S") {

		X = (i.DC.X / (sqrt(1.00 - i.DC.Z))) * center.radius;
		Y = (i.DC.Y / (sqrt(1.00 - i.DC.Z))) * center.radius;
	}

	if (inset.hemisphere == "U") {

		X = - X;
		Y = - Y;
	}

	X = X + center.X;
	Y = Y + center.Y;

	o << "  " << i.PSCOLOR << " setrgbcolor" << endl;

	if ((inset.plot == "H") && (!(i.OFFSET == "NONE"))){

		if ((i.UP)) DIPDIR = DIPDIR + 180.0;

		o
		<< "  " << setw (6) << fixed << setprecision (2) << X
		<< "  "	<< setw (5) << fixed << setprecision (2) << Y << " translate " << endl;
		o
		<< "  " << - DIPDIR  << " rotate" << endl;
		o
		<< "  " << "  newpath normalarrow" << endl;
		o
		<< "  " <<   DIPDIR  << " rotate " << endl;
		o
		<< "  " << setw (6) << fixed << setprecision (2) << -X
		<< "  " << setw (5) << fixed << setprecision (2) << -Y << " translate " << endl;
	}

	else {

		if (i.corrOFFSET == "INVERSE") {

			o
			<< "  " << setw (6) << fixed << setprecision (2) << X
			<< " "	<< setw (5) << fixed << setprecision (2) << Y
			<< " translate " << - DIPDIR + 180.0 << " rotate" << endl;
			o << "  newpath normalarrow" << endl;
			o << "  " <<   DIPDIR + 180.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2) << -Y << " translate " << endl;
		}

		if (i.corrOFFSET == "NORMAL") {

			o << "  " << setw (6) << fixed << setprecision (2) << X << " "
					<< setw (5) << fixed << setprecision (2) << Y << " translate "
					<< - DIPDIR + 000.0 << " rotate" << endl;
			o << "  newpath normalarrow" << endl;
			o << "  " <<   DIPDIR - 000.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2 )<< -Y << " translate " << endl;
		}

		if (i.corrOFFSET == "DEXTRAL") {

			o << "  " << setw (6) << fixed << setprecision (2) << X   << " "
					<< setw (5) << fixed << setprecision (2) << Y << " translate "
					<< - DIPDIR + 90.0 << " rotate" << endl;
			o << "  newpath dextralarrow" << endl;
			o << "  " <<   DIPDIR - 90.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2) << -Y << " translate " << endl;
		}

		if (i.corrOFFSET == "SINISTRAL") {

			o << "  " << setw (6) << fixed << setprecision (2) << X << " "
					<< setw (5) << fixed << setprecision (2) << Y << " translate "
					<< - DIPDIR + 90.0 << " rotate" << endl;
			o << "  newpath sinistralarrow" << endl;
			o << "  " <<   DIPDIR - 90.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2) << -Y << " translate" << endl;
		}

		if (i.OFFSET == "NONE") {

			o << "  " << setw (3) << setprecision (5) << X << " "
					<< setw (3) << setprecision (5) << Y << " translate "
					<<  - DIPDIR << " rotate" << endl;
			o << "  newpath nonearrow" << endl;
			o << "  " <<  DIPDIR << " rotate "
					<< setw (3) << setprecision (5) << -X << " "
					<< setw (3) << setprecision (5) << -Y << " translate" << endl;
		}
}


	if (label) {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << endl;
		o
		<< "  " << fixed << setprecision (3) << X << " " << fixed << setprecision (3) << Y  <<" translate" << endl;

		o
		<< "  0.0 0.0 moveto 10.0 10.0 lineto stroke" << endl;
		o
		<< "  10.0 10.0 moveto ("	<< i.ID << ") show"	<< endl;

		o
		<< "  " << fixed << setprecision (3) << - X << " " << fixed << setprecision (3) << - Y  <<" translate" << endl;
	}

	o << "0 0 0 setrgbcolor" << endl;
}

void PS_getstereonet (ofstream& o, INPSET inset, CENTER center) {

	o << "newpath " << center.X << " " << center.Y << " " << center.radius << " " << " 0 360 arc stroke" << endl;

	o << "newpath " << center.X + center.radius	<< " " << center.Y << " moveto " << center.X + center.radius + 10.0	<< " " << center.Y << " lineto stroke" << endl;
	o << "newpath " << center.X << " " << center.Y + center.radius << " moveto " << center.X << " " << center.Y + center.radius + 10.0 << " lineto stroke" << endl;
	o << "newpath " << center.X - center.radius << " " << center.Y << " moveto " << center.X - center.radius - 10.0 << " " << center.Y << " lineto stroke" << endl;
	o << "newpath " << center.X << " " << center.Y - center.radius << " moveto " << center.X << " " << center.Y - center.radius - 10.0 << " lineto stroke" << endl;

	o << "newpath " << center.X << " " << center.Y - 10.0  << " moveto " << center.X << " " << center.Y + 10.0  << " lineto 0.7 setlinewidth stroke" << endl;
	o << "newpath " << center.X - 10.0 << " " << center.Y  << " moveto " << center.X + 10.0 << " " << center.Y  << " lineto 0.7 setlinewidth stroke" << endl;

	o << "/ArialNarrow-Bold findfont 20 scalefont setfont" << endl;
	o << center.X - 6.0 << " " << center.Y + center.radius + 15.0 << " moveto (N) show" << endl;

	o << "/ArialNarrow-Italic findfont 10 scalefont setfont" << endl;

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Schmidt-net, lower hemisphere) show" << endl ;
	}

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Schmidt-net, upper hemisphere) show" << endl ;
	}

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Wulff-net, lower hemisphere) show" << endl ;
	}

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Wulff-net, upper hemisphere) show" << endl ;
	}
}

void PS_datanumber_averagebedding (GDB i, ofstream& o, INPSET inset, PAPER P, CENTER center, size_t datanumber) {

	o << endl;

	if (inset.plot == "H") PS_polepoint (i, o, inset, center, false, "AV");
	else PS_plane (i, o, inset, center, false, "AV");

	if (inset.datarule == "R") i.avS0d.DIPDIR = german_to_right_hand_rule (i.avS0d.DIPDIR);

	o << endl;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;
	o << "  0 0 0 setrgbcolor" << endl;

	o
	<< "  " << fixed << setprecision (3) << P.O1X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O1Y + P.R + 1.8 * P.B
	<< " moveto (Data number: "<< datanumber << ") show" << endl;

	o
	<< "  " << fixed << setprecision (3) << P.O1X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O1Y + P.R + 1.3 * P.B
	<< " moveto (Average bedding: "
	<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
	<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP << flush;

	if (i.avS0offset == "OVERTURNED") o << ", overturned" << flush;

	o << ") show" << endl << endl;

	o
	<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.8 * P.B
	<< " moveto (Data number: "<< datanumber << ") show" << endl;

	if (inset.tilting == "B") {

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding "
		<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
		<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP << flush;

		if (i.avS0offset == "OVERTURNED") o << ", overturned" << flush;

		o << ") show" << endl << endl;
	}

	else if (inset.tilting == "P") {

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << endl << endl;
	}

	else {

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding "
		<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
		<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP << flush;

		if (i.avS0offset == "OVERTURNED") o << ", overturned" << flush;

		o << ") show" << endl << endl;

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 0.8 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << endl << endl;
	}

	o
	<< "  " << fixed << setprecision (3) << P.O3X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O3Y + P.R + 1.8 * P.B
	<< " moveto (Rose plot for measured data) show" << endl;

	o
	<< "  " << fixed << setprecision (3) << P.O3X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O3Y + P.R + 1.3 * P.B
	<< " moveto (Average bedding: "
	<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
	<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP << flush;

	if (i.avS0offset == "OVERTURNED") o << ", overturned" << flush;
	o << ") show" << endl << endl;

	o
	<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.8 * P.B
	<< " moveto (Rose plot for dip corrected data) show" << endl;

	if (inset.tilting == "B") {

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding "
		<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
		<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP << flush;

		if (i.avS0offset == "OVERTURNED") o << ", overturned" << flush;

		o << ") show" << endl << endl;
	}

	else if (inset.tilting == "P") {

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << endl << endl;
	}

	else {

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding "
		<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
		<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP << flush;

		if (i.avS0offset == "OVERTURNED") o << ", overturned" << flush;

		o << ") show" << endl << endl;

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 0.8 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << endl << endl;
	}
}

void PS_rosesegment (ofstream& o, INPSET inset, CENTER center, double percentage, double degree, bool c_plane) {

	double step_angle = 2.5;

	if (inset.rosebinning == "B") step_angle =  5.0;
	if (inset.rosebinning == "C") step_angle = 10.0;
	if (inset.rosebinning == "D") step_angle = 22.5;

	double angle = 90.0 - step_angle;

	double radius = center.radius * percentage;

	o <<         fixed << setprecision (2) << center.X
	  << " "  << fixed << setprecision (2) << center.Y << " translate"
      << " "  << fixed << setprecision (2) << - degree << " rotate " << endl;

	if (c_plane) o << "1.0 setlinewidth 0.00 0.00 1.00 setrgbcolor " << endl;
	else o << "0.7 setlinewidth 0.00 0.50 0.00 setrgbcolor " << endl;

	o << " newpath 0.0 0.0 moveto "
	  << fixed << setprecision (2) << + (radius * SIN(step_angle)) << " "
	  << fixed << setprecision (2) <<    radius * COS(step_angle)  << " lineto" << endl;
	o << "  0.0 0.0 " << fixed << setprecision (3) << radius
	  << " "     << fixed << setprecision (3) << angle << " 90.0 arc" << endl;
	o << "  0.0 0.0 moveto 0.0 " << fixed << setprecision (3) << radius << " lineto" << endl;
	o << "  closepath " << endl;

	if (c_plane) o << " stroke" << endl;
	else o << " gsave 0.00 1.00 0.00 setrgbcolor fill grestore stroke" << endl;

	o << " "  << fixed << setprecision (2) << degree    << " rotate "
	  << "  " << fixed << setprecision (2) << -center.X
	  << " "  << fixed << setprecision (2) << -center.Y << " translate" << endl << endl;
}

void PS_draw_rose_circle_horizontal (ofstream& o, CENTER center, ROSENUMBER percent) {

	double i, step;

	if (percent.PLN_NUM < 0.01) return;

	if (percent.PLN_NUM < 0.2) 	i = 0.05;
	else if ((percent.PLN_NUM >= 0.2) && (percent.PLN_NUM < 0.5)) i = 0.1;
	else i = 0.2;

	step = i;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;
	o << fixed << setprecision (3) << center.X << " "
	  << fixed << setprecision (3) << center.Y << " translate" << endl;
	o << "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << endl;
	o << "  [3 3] 0 setdash" << endl;

	do {

		o << "   newpath 0.0 0.0 "
		  << fixed << setprecision (3) << center.radius *  i / percent.PLN_NUM
		  << " 0.0 360.0 arc stroke" << endl;
		o << "  -5.0 "
		  << fixed << setprecision (3) << (center.radius * i / percent.PLN_NUM) + 2.0 << " moveto"
		  << " (" << fixed << setprecision (0) << i * 100.0 << " %) show" << endl;

		i =  i + step;

	} while (i < percent.PLN_NUM);

	o << "  stroke [   ] 0 setdash" << endl;
	o << "  " << fixed << setprecision (3) << - center.X << " "
	          << fixed << setprecision (3) << - center.Y << " translate" << endl;
}

void PS_draw_rose_circle_vertical (ofstream& o, CENTER center, ROSENUMBER percent) {

	double i, step;

	if (percent.PLN_NUM < 0.01) return;

	if (percent.PLN_NUM < 0.2) 	i = 0.05;
	else if ((percent.PLN_NUM >= 0.2) && (percent.PLN_NUM < 0.5)) i = 0.1;
	else i = 0.2;

	step = i;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << endl;
	o << fixed << setprecision (3) << center.X << " "
	  << fixed << setprecision (3) << center.Y << " translate" << endl;
	o << "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << endl;
	o << "  [3 3] 0 setdash" << endl;

	do {

		o << "   newpath 0.0 0.0 "
		  << fixed << setprecision (3) << center.radius *  i / percent.PLN_NUM
		  << " 270.0 360.0 arc stroke" << endl;
		o << "  " << fixed << setprecision (3) << (center.radius * i / percent.PLN_NUM) - 4.0 << " 5.0 moveto"
		  << " (" << fixed << setprecision (0) << i * 100.0 << " %) show" << endl;

		i =  i + step;

	} while (i < percent.PLN_NUM);

	o << "  stroke [   ] 0 setdash" << endl;
	o << "  " << fixed << setprecision (3) << - center.X << " "
	          << fixed << setprecision (3) << - center.Y << " translate" << endl;
}

void PS_DRAW_PTN (GDB i, ofstream& o, INPSET inset, CENTER center) {

	STRESSFIELD empty_sf;

	PS_lineation (i, o, inset, center, empty_sf, false, "P");
	PS_lineation (i, o, inset, center, empty_sf, false, "T");
	PS_lineation (i, o, inset, center, empty_sf, false, "N");
}

void PS_DRAW_plane (GDB i, ofstream& o, INPSET inset, CENTER center) {

	if (inset.plot =="H") {

		if (inset.labeling == "Y")	PS_polepoint	(i, o, inset, center, true, "");
		else 						PS_polepoint 	(i, o, inset, center, false, "");
	}

	else {

		if (inset.labeling == "Y") 	PS_plane 		(i, o, inset, center, true, "");
		else 						PS_plane 		(i, o, inset, center, false, "");
	}
}

void PS_DRAW_lineation (GDB i, ofstream& o, INPSET inset, CENTER center) {

	STRESSFIELD empty_sf;

	if (inset.labeling == "Y") 		PS_lineation 	(i, o, inset, center, empty_sf, true, "");
	else 							PS_lineation 	(i, o, inset, center, empty_sf, false, "");
}

void PS_DRAW_striae (GDB i, ofstream& o, INPSET inset, CENTER center) {

	string offset = i.corrOFFSET;

	if (inset.plot =="H") {

		if (inset.labeling == "Y") 	PS_striaearrow	(i, o, inset, center, true);
		else 						PS_striaearrow (i, o, inset, center, false);

	}

	else {

		if (inset.labeling == "Y") {

			PS_plane 		(i, o, inset, center, false, "");
			PS_striaearrow	(i, o, inset, center, true);
		}

		else {

			PS_plane 		(i, o, inset, center, false, "");
			PS_striaearrow	(i, o, inset, center, false);
		}
	}
}

void PS_DRAW_sc (GDB i, ofstream& o, INPSET inset, CENTER center) {

	VCTR d;

	d = compute_d_for_SC (i);

	if (inset.plot =="H") {

		if (!(inset.labeling == "N")) PS_sc_arrow	(i, o, inset, center, d);
		else PS_sc_arrow (i, o, inset, center, d);

	}

	else {

		if (inset.labeling == "Y") {

			PS_plane 		(i, o, inset, center, true, "C");
			PS_plane 		(i, o, inset, center, false, "");
			PS_sc_arrow 	(i, o, inset, center, d );
		}

		else {

			PS_plane 		(i, o, inset, center, false, "C");
			PS_plane 		(i, o, inset, center, false, "");
			PS_sc_arrow 	(i, o, inset, center, d );
		}
	}
}


void PS_idealmovement (vector <GDB > inGDB, ofstream& o, INPSET inset, CENTER center) {

	size_t i = 0;
	VCTR shear_vector;
	STRESSFIELD empty_sf;

	do {

		shear_vector = unitvector (inGDB.at(i).SHEAR_S);

		inGDB.at(i).N = flip_N_vector (shear_vector);

		inGDB.at(i).PSCOLOR = "0.0 0.0 0.5";

		PS_polepoint (inGDB.at(i), o, inset, center, false, "");

		i++;

	} while (i < inGDB.size());
}

void PS_DRAW_record (GDB i, ofstream& o, INPSET inset, CENTER center) {

	if (i.DATAGROUP == "LINEATION") PS_DRAW_lineation	(i, o, inset, center);
	if (i.DATAGROUP == "PLANE") 	PS_DRAW_plane		(i, o, inset, center);
	if (i.DATAGROUP == "STRIAE") 	PS_DRAW_striae 		(i, o, inset, center);
	if (i.DATAGROUP == "SC") 		PS_DRAW_sc 			(i, o, inset, center);
}

void PS_SYMBOLS_border (ofstream& o, PAPER P) {

	o << "newpath" << endl;

	o
	<< "  " << fixed << setprecision (3) << P.A
	<< " " << fixed << setprecision (3) << (P.Y / 2.0) - 0.15 * P.A  << " moveto "
	<< fixed << setprecision (3) << P.X - (10.0 * P.A)
	<< " " << fixed << setprecision (3) << (P.Y / 2.0) - 0.15 * P.A << " lineto" << endl;

	o
	<< "  "<< fixed << setprecision (3) << P.S2X
	<< " " << fixed << setprecision (3) << P.S2Y  << " moveto "
		   << fixed << setprecision (3) << P.S6X
	<< " " << fixed << setprecision (3) << P.S6Y << " lineto" << endl;

	o
	<< "  "<< fixed << setprecision (3) << P.S5X
	<< " " << fixed << setprecision (3) << P.S5Y << " moveto "
		   << fixed << setprecision (3) << P.S1X
	<< " " << fixed << setprecision (3) << P.S1Y << " lineto "
		   << fixed << setprecision (3) << P.S4X
	<< " " << fixed << setprecision (3) << P.S4Y << " lineto "
		   << fixed << setprecision (3) << P.S3X
	<< " " << fixed << setprecision (3) << P.S3Y << " lineto " << endl;

	o << "  3.0 setlinewidth" << endl;
	o << "  0.8 0.8 0.8 setrgbcolor stroke" << endl << endl;

	o
	<< "  "<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.25
	<< " " << fixed << setprecision (3) << P.S1Y - 0.3 * P.A << " moveto "
	<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.25
	<< " " << fixed << setprecision (3) << P.S4Y + 0.3 * P.A << " lineto" << endl;

	o
	<< "  "<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.5
	<< " " << fixed << setprecision (3) << P.S1Y - 0.3 * P.A << " moveto "
	<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.5
	<< " " << fixed << setprecision (3) << P.S4Y + 0.3 * P.A << " lineto" << endl;

	o
	<< "  "<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.75
	<< " " << fixed << setprecision (3) << P.S1Y - 0.3 * P.A << " moveto "
	<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.75
	<< " " << fixed << setprecision (3) << P.S4Y + 0.3 * P.A << " lineto" << endl;

	o << "  1.0 setlinewidth" << endl;
	o << "  0.8 0.8 0.8 setrgbcolor stroke" << endl << endl;

	o << "/ArialNarrow-Bold findfont 12 scalefont setfont" 					<< endl;
	o << "  " << P.X - (6.1 * P.A) << " " << P.Y / 2.0 + 3.5 * P.A  << " moveto " << endl;
	o << "  (SYMBOLS) 0 0 0 setrgbcolor show"  << endl;
}

void PS_SYMBOL_draw_plane (double X, double Y, ofstream& o, PAPER P, string type, string group) {

	string color = "0.00 0.00 0.00";

	o <<  "   newpath " << P.S1X + 1.2 * X << " " << P.S1Y - 3.5 * Y << " " << 3.0 * P.A << " 80 110 arc" << endl;

	if (group == "A") color = "0.00 0.00 1.00";
	if (group == "B") color = "1.00 0.00 0.67";
	if (group == "C") color = "1.00 0.00 0.00";
	if (group == "D") color = "1.00 0.50 0.00";
	if (group == "E") color = "1.00 1.00 0.00";
	if (group == "F") color = "0.00 1.00 0.00";
	if (group == "G") color = "0.67 0.00 0.67";
	if (group == "H") color = "0.50 1.00 1.00";
	if (group == "I") color = "0.50 0.50 0.50";

	if 			(type == "AV") 		o << "  1.00 0.00 0.00 setrgbcolor 1.5 setlinewidth " << endl;
	else if		(type == "AV_O") 	o << "  1.00 0.00 0.00 setrgbcolor 1.5 setlinewidth " << endl;
	else if 	(type == "FOLD") 	o << "  0.00 0.00 1.00 setrgbcolor 1 setlinewidth " << endl;
	else 							o << "  " << color << " setrgbcolor 1 setlinewidth " << endl;

	if 			(type == "AV") 		o << "  [   ] 0 setdash stroke  [   ] 0 setdash " << endl;
	if 			(type == "AV_O") 	o << "  [6 6] 0 setdash stroke  [   ] 0 setdash " << endl;
	if 			(type == "O") 		o << "  [6 6] 0 setdash stroke  [   ] 0 setdash " << endl;
	else if 	(type == "FOLD") 	o << "  [6 6] 0 setdash stroke  [   ] 0 setdash " << endl;
	else if 	(type == "C") 		o << "  [3 3] 0 setdash stroke  [   ] 0 setdash " << endl;
	else 							o << "stroke" << endl << endl;
}



void PS_SYMBOLS_STRIAE (ofstream& o, PAPER P) {

	double arrow_X, arrow_Y;
	double step = 0.30;

	arrow_X = P.S1X + 0.6 * P.A;
	arrow_Y = P.S1Y - 3.355 * P.A;

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.50 * P.A) << " moveto (Fault plane) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.10 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.40 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.22 * P.A) << " moveto (Overturned) 0 0 0 setrgbcolor show"  << endl;

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.00 * P.A, o, P, "", "");

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, P, "AV", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.60 * P.A, o, P, "AV_O", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.80 * P.A, o, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.97 * P.A, o, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.14 * P.A, o, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.31 * P.A, o, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.48 * P.A, o, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.65 * P.A, o, P, "", "");


	o << "  " << arrow_X << " " << arrow_Y << "  translate 20 rotate" << endl;
	o << "  newpath normalarrow" << endl;
	o << "  -20 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y << " moveto (Normal fault) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate 160 rotate" << endl;
	o << "  newpath normalarrow" << endl;
	o << "  -160 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Reverse fault) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate 30 rotate" << endl;
	o << "  newpath dextralarrow" << endl;
	o << "  -30 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Dextral fault) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate 60 rotate" << endl;
	o << "  newpath sinistralarrow" << endl;
	o << "  -60 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Sinistral fault) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate -20 rotate" << endl;
	o << "  newpath nonearrow" << endl;
	o << "  20 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Unknown fault) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (step * P.A);


	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Ideal movement) 0 0 0 setrgbcolor show"  << endl;


	o << "  0.0 0.0 0.5 setrgbcolor 0.7 setlinewidth" << endl;
	o
	<< "  newpath " << fixed << setprecision (3) << arrow_X
			 << " " << fixed << setprecision (3) << arrow_Y + 12.5
			 << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << endl;
	o << "  0 0 0 setrgbcolor" << endl;
}

void PS_SYMBOLS_SC (ofstream& o, PAPER P) {

	double arrow_X, arrow_Y;

	arrow_X = P.S1X + 0.6 * P.A;
	arrow_Y = P.S1Y - 3.355 * P.A;

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.5 * P.A) << " moveto (Schistosity plane) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << endl;

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.1 * P.A) << " moveto (Cleavage plane) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  - 5.0 << " " << arrow_Y + (0.92 * P.A) << " moveto (with movement direction) 0 0 0 setrgbcolor show"  << endl;

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.00 * P.A, o, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, P, "C", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.80 * P.A, o, P, "AV", "");



	o << " 0.00 0.00 0.00 setrgbcolor " << endl;
	o << "  " << arrow_X  << " " << arrow_Y + 1.4 * P.A << "  translate 20 rotate" << endl;
	o << "  newpath normalarrow" << endl;
	o << "  -20 rotate " << - arrow_X << " " << - (arrow_Y + 1.4 * P.A) << "  translate " << endl;

	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (0.4 * P.A);
}

void PS_SYMBOLS_PLANE (vector <GDB> inGDB, ofstream& o, PAPER P) {

	double arrow_X = P.S1X + 0.6 * P.A;
	double arrow_Y = P.S1Y - 3.355 * P.A;

	string type = "";

	if 		(inGDB.at(0).DATATYPE == "CONTACT") 		type = "Contact plane";
	else if (inGDB.at(0).DATATYPE == "FOLDPLANE") 		type = "Fold plane";
	else if (inGDB.at(0).DATATYPE == "LITHOCLASE") 		type = "Lithoclase";
	else if	(inGDB.at(0).DATATYPE == "FRACTURE") 		type = "Fracture plane";
	else if	(inGDB.at(0).DATATYPE == "BEDDING") 		type = "Bedding plane";
	else if	(inGDB.at(0).DATATYPE == "S1") 				type = "S1 plane";
	else if	(inGDB.at(0).DATATYPE == "S2") 				type = "S2 plane";
	else if (inGDB.at(0).DATATYPE == "S3") 				type = "S3 plane";
	else if (inGDB.at(0).DATATYPE == "S4") 				type = "S4 plane";
	else if (inGDB.at(0).DATATYPE == "S5") 				type = "S5 plane";
	else if (inGDB.at(0).DATATYPE == "CROSSBEDDING") 	type = "Cross bedding";
	else if (inGDB.at(0).DATATYPE == "VEIN") 			type = "Vein";
	else if (inGDB.at(0).DATATYPE == "FOLDSURFACE") 	type = "Fold surface";
	else if (inGDB.at(0).DATATYPE == "USERPLANE4") 		type = "User defined";
	else 												type = "User defined";

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.50 * P.A) << " moveto (" << type << ") 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.10 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.40 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.22 * P.A) << " moveto (Overturned) 0 0 0 setrgbcolor show"  << endl;

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.00 * P.A, o, P, "", "");
	if (type == "BEDDING")
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.20 * P.A, o, P, "O", "");

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, P, "AV", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.60 * P.A, o, P, "AV_O", "");

	if (inGDB.at(0).DATATYPE == "FOLDSURFACE") {

		PS_SYMBOL_draw_plane (1.0 * P.A, 1.80 * P.A, o, P, "FOLD", "");
		o  << "  " << arrow_X + 5.0 << " " << arrow_Y - (0.3 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	}
}

void PS_SYMBOLS_LINEATION (vector <GDB> inGDB, ofstream& o, PAPER P) {

	double arrow_X = P.S1X + 0.6 * P.A;
	double arrow_Y = P.S1Y - 3.355 * P.A;

	string type = "";

	if 		(inGDB.at(0).DATATYPE == "BOUDAIN") 		type = "Boudain axis";
	else if (inGDB.at(0).DATATYPE == "FOLDAXIS") 		type = "Fold axis";
	else if (inGDB.at(0).DATATYPE == "KINK") 			type = "Kink axis";
	else if	(inGDB.at(0).DATATYPE == "LINEATION") 		type = "Lineation";
	else 												type = "User defined";

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.50 * P.A) << " moveto (" << type << ") 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.10 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.40 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.22 * P.A) << " moveto (Overturned) 0 0 0 setrgbcolor show"  << endl;

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, P, "AV", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.60 * P.A, o, P, "AV_O", "");

	o
	<< "  " << " 0.0 0.0 0.0 setrgbcolor 1.5 setlinewidth newpath "
	<< fixed << setprecision (5) << arrow_X - 0.35 * P.A << " "
	<< fixed << setprecision (5) << arrow_Y + 2.8 * P.A << " 1.5 0 360 arc 0 fill stroke" << endl;
}

void PS_SYMBOLS_HOEPPNER (ofstream& o, PAPER P) {

	double arrow_X = P.S1X + 0.6 * P.A;
	double arrow_Y = P.S1Y - 3.355 * P.A;

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.5 * P.A) << " moveto (Plane pole point) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.1 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << endl;

	o << "  " << " 0.0 0.0 0.0 setrgbcolor 1.5 setlinewidth newpath "
	  << fixed << setprecision (5) << arrow_X - 0.35 * P.A << " "
	  << fixed << setprecision (5) << arrow_Y + 2.8 * P.A << " 1.5 0 360 arc 0 stroke" << endl;

	o << "  " << " 1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth  newpath "
	  << fixed << setprecision (5) << arrow_X - 0.35 * P.A << " "
	  << fixed << setprecision (5) << arrow_Y + 2.8 * P.A << " 0.7 0 360 arc 0 stroke" << endl;

	o << "  1.0 0.0 0.0 setrgbcolor 0.7 setlinewidth newpath" << endl;

	o << fixed << setprecision (3) << arrow_X - 0.35 * P.A<< " "
	  << fixed << setprecision (3) << arrow_Y + 1.40 * P.A
	  << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << endl;
	o << "   0 0 0 setrgbcolor" << endl;

	o << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << "  translate 20 rotate" << endl;
	o << "  newpath normalarrow" << endl;
	o << "  -20 rotate " << - (arrow_X - 0.3 * P.A)<< " " << - arrow_Y << "  translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X  + 4.0 << " " << arrow_Y << " moveto (Hanging wall) 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 4.0 << " " << arrow_Y - 0.18 * P.A << " moveto (movement direction) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (0.4 * P.A);
}

void PS_SYMBOLS_GROUPS (ofstream& o, PAPER P) {

	double arrow_X = P.S1X + 0.6 * P.A + 6.0 * P.A;;
	double arrow_Y = P.S1Y - 3.355 * P.A;

	PS_SYMBOL_draw_plane (6.8 * P.A, 1.00 * P.A, o, P, "", "A");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.20 * P.A, o, P, "", "B");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.40 * P.A, o, P, "", "C");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.60 * P.A, o, P, "", "D");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.80 * P.A, o, P, "", "E");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.00 * P.A, o, P, "", "F");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.20 * P.A, o, P, "", "G");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.40 * P.A, o, P, "", "H");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.60 * P.A, o, P, "", "I");

	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (2.5 * P.A) << " moveto (Group 'A') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (1.8 * P.A) << " moveto (Group 'B') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (1.1 * P.A) << " moveto (Group 'C') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (0.4 * P.A) << " moveto (Group 'D') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (0.3 * P.A) << " moveto (Group 'E') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (1.0 * P.A) << " moveto (Group 'F') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (1.7 * P.A) << " moveto (Group 'G') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (2.4 * P.A) << " moveto (Group 'H') 0 0 0 setrgbcolor show"  << endl;
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (3.1 * P.A) << " moveto (Group 'I') 0 0 0 setrgbcolor show"  << endl;
}

void PS_SYMBOLS_INVERSION (ofstream& o, PAPER P) {

	double arrow_X = P.S1X + (0.6 * P.A) + 2.8 * P.A;
	double arrow_Y = P.S1Y - (3.355 * P.A) + 2.5 * P.A - 0.4 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate " << endl;
	o << "   newpath s1_axis" << endl;
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << " moveto (S1 axis) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << endl;
	o << "   newpath s2_axis" << endl;
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << " moveto (S2 axis) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << endl;
	o << "   newpath s3_axis" << endl;
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << " moveto (S3 axis) 0 0 0 setrgbcolor show"  << endl;

	arrow_Y = arrow_Y - (0.2 * P.A) - 0.7 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate 90 rotate 0 10 translate" << endl;
	o << "   newpath compression_arrow" << endl;
	o << "   0 -10 translate 180 rotate 0 10 translate " << endl;
	o << "   newpath compression_arrow" << endl;
	o << "   0 -10 translate 180 rotate" << endl;
	o << "   -90 rotate " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.5 * P.A);
	o  << "  " << arrow_X - P.A << " " << arrow_Y << " moveto (Maximum horizontal stress) 0 0 0 setrgbcolor show"  << endl;

	arrow_Y = arrow_Y - (0.2 * P.A) - 0.7 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate 90 rotate 0 10 translate" << endl;
	o << "   newpath extension_arrow" << endl;
	o << "   0 -10 translate 180 rotate 0 10 translate " << endl;
	o << "   newpath extension_arrow" << endl;
	o << "   0 -10 translate 180 rotate" << endl;
	o << "   -90 rotate " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.5 * P.A);
	o  << "  " << arrow_X - P.A << " " << arrow_Y << " moveto (Minimum horizontal stress) 0 0 0 setrgbcolor show"  << endl;
}

void PS_SYMBOLS_BINGHAM (ofstream& o, PAPER P) {

	double arrow_X = P.S1X + (0.6 * P.A) + 2.8 * P.A;
	double arrow_Y = P.S1Y - (3.355 * P.A) + 2.5 * P.A - 0.4 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate " << endl;
	o << "   newpath s1_axis" << endl;
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.8 * P.A << " " << arrow_Y << " moveto (Maximum weight point) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << endl;
	o << "   newpath s2_axis" << endl;
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.8 * P.A << " " << arrow_Y << " moveto (Intermediate weight point) 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << endl;
	o << "   newpath s3_axis" << endl;
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << endl;
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.8 * P.A << " " << arrow_Y << " moveto (Minimum weight point) 0 0 0 setrgbcolor show"  << endl;
}

void PS_SYMBOLS_ROSE (vector <GDB> inGDB, ofstream& o, PAPER P) {

	double angle = 80.0;
	double radius = 80.0;

	double arrow_X = P.S1X + (0.6 * P.A) + 2.8 * P.A;
	double arrow_Y = P.S1Y - (3.355 * P.A) + 2.5 * P.A - 1.1 * P.A;

	string outtext1 = "Plane dip direction";
	if (inGDB.at(0).DATAGROUP == "LINEATION") outtext1 = "Lineation direction";
	if (inGDB.at(0).DATAGROUP == "SC") outtext1 = "Schistosity dip";

	o
	<< "  " << fixed << setprecision (2) << arrow_X + 1.5 * P.A
	<< " "  << fixed << setprecision (2) << arrow_Y - 1.0 * P.A << " translate -30 rotate" << endl;
	o << "0.7 setlinewidth 0.00 0.50 0.00 setrgbcolor " << endl;
	o << " newpath 0.0 0.0 moveto "
	  << fixed << setprecision (2) << radius * SIN(10.0) << " "
	  << fixed << setprecision (2) << radius * COS(10.0) << " lineto" << endl;
	o
	<< "  0.0 0.0 " << fixed << setprecision (3) << radius
	<< " "     << fixed << setprecision (3) << angle << " 90.0 arc" << endl;
	o << "  0.0 0.0 moveto 0.0 " << fixed << setprecision (3) << radius << " lineto" << endl;
	o << "  closepath " << endl;
	o << " gsave 0.00 1.00 0.00 setrgbcolor fill grestore stroke" << endl;
	o
	<< "  30 rotate "
	<< fixed << setprecision (2) << - (arrow_X + 1.5 * P.A) << " "
	<< fixed << setprecision (2) << - (arrow_Y - 1.0 * P.A) << " translate" << endl;

	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 1.6 * P.A << " " << arrow_Y - (2.0 * 0.7 * P.A) << " moveto (" << outtext1 << ") 0 0 0 setrgbcolor show"  << endl;
	arrow_Y = arrow_Y - (0.4 * P.A);

	if ((inGDB.at(0).DATAGROUP == "LINEATION") || (inGDB.at(0).DATAGROUP == "PLANE")) return;

	string outtext2 = "Lineation dip direction";
	if (inGDB.at(0).DATAGROUP == "SC") outtext2 = "Cleavage dip";

	arrow_Y = arrow_Y - (3.0 * 0.7 * P.A);

	o
	<< "  " << fixed << setprecision (2) << arrow_X + 1.5 * P.A
	<< " "  << fixed << setprecision (2) << arrow_Y - 1.0 * P.A << " translate -30 rotate" << endl;

	o << "1.0 setlinewidth 0.00 0.00 1.00 setrgbcolor " << endl;
	o << " newpath 0.0 0.0 moveto "
			<< fixed << setprecision (2) << radius * SIN(10.0) << " "
			<< fixed << setprecision (2) << radius * COS(10.0) << " lineto" << endl;
	o
	<< "  0.0 0.0 " << fixed << setprecision (3) << radius
	<< " "     << fixed << setprecision (3) << angle << " 90.0 arc" << endl;

	o << "  0.0 0.0 moveto 0.0 " << fixed << setprecision (3) << radius << " lineto" << endl;
	o << "  closepath stroke " << endl;

	o
	<< "  30 rotate "
	<< fixed << setprecision (2) << - (arrow_X + 1.5 * P.A) << " "
	<< fixed << setprecision (2) << - (arrow_Y - 1.0 * P.A) << " translate" << endl;

	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 1.5 * P.A << " " << arrow_Y - (2.0 * 0.7 * P.A)<< " moveto (" << outtext2 << ") 0 0 0 setrgbcolor show"  << endl;
}

void PS_SYMBOLS_LABEL (ofstream& o, INPSET inset, PAPER P) {

	double arrow_X = P.S1X + 0.3 * P.A;
	double arrow_Y = P.S1Y - 1.55 * P.A;

	if (inset.labeling == "Y") {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << endl;
		o
		<< "  " << fixed << setprecision (3) << arrow_X
		<< " "  << fixed << setprecision (3) << arrow_Y  <<" translate 90 rotate" << endl;
		o
		<< "  28.0 2.0 moveto 35.0 2.0 lineto stroke" << endl;
		o
		<< "  0.0 0.0 moveto (OXAM_001) show"	<< endl;
		o
		<< "  -90 rotate " << fixed << setprecision (3) << - arrow_X
		<< " "  << fixed << setprecision (3) << - arrow_Y  <<" translate" << endl;

		arrow_X = P.S1X + 0.6 * P.A;
		arrow_Y = P.S1Y - 3.355 * P.A;

		o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.14 * P.A) << " moveto (with labeling) 0 0 0 setrgbcolor show"  << endl;
	}

	else return;
}

void PS_SYMBOLS (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P) {

	PS_SYMBOLS_border (o, P);
	PS_SYMBOLS_LABEL (o, inset, P);

	o << "/ArialNarrow findfont 8 scalefont setfont" 					<< endl;



	o << "  " << P.S1X + 5.2 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
	o << "  (ROSE PLOT) 0 0 0 setrgbcolor show"  << endl;
	PS_SYMBOLS_ROSE (inGDB, o, P);

	o << "  " << P.S1X + 7.4 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
	o << "  (GROUPS) 0 0 0 setrgbcolor show"  << endl;
	PS_SYMBOLS_GROUPS (o, P);

	if (inGDB.at(0).DATAGROUP == "PLANE") {

		o << "  " << P.S1X + 0.8 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
		o << "  (PLANES) 0 0 0 setrgbcolor show"  << endl;
		PS_SYMBOLS_PLANE (inGDB, o, P);

		o << "  " << P.S1X + 2.5 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
		o << "  (BINGHAM STATISTICS) 0 0 0 setrgbcolor show"  << endl;

		if ((inset.fracture == "B") && (inGDB.at(0).DATATYPE == "FRACTURE")) PS_SYMBOLS_BINGHAM (o, P);

		return;
	}

	if (inGDB.at(0).DATAGROUP == "STRIAE") {

		o << "  " << P.S1X + 0.1 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
		o << "  (FAULT AND STRIAE DATA) 0 0 0 setrgbcolor show"  << endl;

		if (inset.plot == "A")	PS_SYMBOLS_STRIAE (o, P);
		else PS_SYMBOLS_HOEPPNER (o, P);

		o << "  " << P.S1X + 2.6 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
		o << "  (STRESS INVERSION) 0 0 0 setrgbcolor show"  << endl;
		if (inset.inversion != "N") PS_SYMBOLS_INVERSION (o, P);

		return;
	}

	if (inGDB.at(0).DATAGROUP == "SC") {

		o << "  " << P.S1X + 0.1 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
		o << "  (SCHISTOSITY, CLEAVEGE) 0 0 0 setrgbcolor show"  << endl;
		PS_SYMBOLS_SC (o, P);

		o << "  " << P.S1X + 2.6 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
		o << "  (STRESS INVERSION) 0 0 0 setrgbcolor show"  << endl;

		return;
	}

	if (inGDB.at(0).DATAGROUP == "LINEATION") {

		o << "  " << P.S1X + 0.6 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << endl;
		o << "  (LINEATION) 0 0 0 setrgbcolor show"  << endl;
		PS_SYMBOLS_LINEATION (inGDB, o, P);

		return;
	}
}
