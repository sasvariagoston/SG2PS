// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "allowed_keys.hpp"
#include "density.h"
#include "ps.h"
#include "rgf.h"
#include "common.h"
#include "ps_RUP_ANG.hpp"
#include "run_mode.h"
#include "rup_clustering.hpp"
#include "valley_method.hpp"

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

void PS_header (string DATATYPE, INPSET inset, string LOC, ofstream& o) {

	string filename;

	filename = LOC + "_" + DATATYPE + ".EPS";

	o << "%!PS-Adobe-3.0 EPSF-3.0" << '\n';
	o << "%%BoundingBox:  0 0 1191 842" << '\n';
	o << "%%Title: " << filename << '\n';
	o << "%%Creator: SG2PS" ;

	if (!(is_DEBUG())) o << " (" << version_id() << ")" ;

	o << '\n';
	o << "%%EndComments" << '\n' << '\n';
	o << "<</PageSize [ 1191 842 ]>> setpagedevice " << '\n';

	if ((DATATYPE == "STRIAE") || (DATATYPE == "FRACTURE" ) || (DATATYPE == "SC")) {

		o << "/extension_arrow {" 										<< '\n';
		o << "  newpath"												<< '\n';
		o << "    -3.0   0.0 moveto "									<< '\n';
		o << "     0.0   5.0 rlineto" 									<< '\n';
		o << "    -2.0   0.0 rlineto" 									<< '\n';
		o << "     5.0   5.0 rlineto" 									<< '\n';
		o << "     5.0  -5.0 rlineto" 									<< '\n';
		o << "    -2.0   0.0 rlineto" 									<< '\n';
		o << "     0.0  -5.0 rlineto closepath" 						<< '\n';
		o << "    0 0 0 setrgbcolor 1 setlinewidth  stroke" 			<< '\n';
		o << "} def" 													<< '\n' << '\n';

		o << "/compression_arrow {" 									<< '\n';
		o << "  newpath"												<< '\n';
		o << "    -5.0   5.0 moveto" 									<< '\n';
		o << "     2.0   0.0 rlineto" 									<< '\n';
		o << "     0.0   5.0 rlineto"									<< '\n';
		o << "     6.0   0.0 rlineto"									<< '\n';
		o << "     0.0  -5.0 rlineto" 									<< '\n';
		o << "     2.0   0.0 rlineto" 									<< '\n';
		o << "    -5.0  -5.0 rlineto closepath" 						<< '\n';
		o << "    0 0 0 setrgbcolor fill stroke" 						<< '\n';
		o << "} def" 													<< '\n' << '\n';

		o << "/s1_axis {" 												<< '\n';
		o << "  newpath"												<< '\n';
		o << "    -2.0   0.0 moveto "									<< '\n';
		o << "     4.0   4.0 rlineto" 									<< '\n';
		o << "    -4.0   4.0 rlineto" 									<< '\n';
		o << "    -4.0  -4.0 rlineto closepath" 						<< '\n';
		o << "    1.0 setlinewidth"										<< '\n';
		o << "    1 1 1 setrgbcolor stroke" << '\n';
		o << "    -2.0   0.0 moveto " << '\n';
		o << "     4.0   4.0 rlineto" << '\n';
		o << "    -4.0   4.0 rlineto" << '\n';
		o << "    -4.0  -4.0 rlineto closepath" << '\n';
		if (inset.grayscale == "Y") o << "    0 0 0 setrgbcolor" << '\n';
		else 						o << "    1 0 0 setrgbcolor" << '\n';
		o << "    fill stroke" << '\n';
		o << "} def" << '\n' << '\n';

		o << "/s1_iter_axis {" 											<< '\n';
		o << "  newpath"												<< '\n';
		o << "    1 1 1 setrgbcolor stroke" << '\n';
		o << "    -2.0   0.0 moveto "									<< '\n';
		o << "     4.0   4.0 rlineto" 									<< '\n';
		o << "    -4.0   4.0 rlineto" 									<< '\n';
		o << "    -4.0  -4.0 rlineto closepath" 						<< '\n';
		if (inset.grayscale == "Y") o << "    0 0 0 setrgbcolor " << '\n';
		else 						o << "    1 0 0 setrgbcolor " << '\n';
		o << " 1 setlinewidth stroke" << '\n';
		o << "} def" << '\n' << '\n';

		o << "/s2_axis {" << '\n';
		o << "  newpath" << '\n';
		o << "    -2.0   0.0 moveto " << '\n';
		o << "     4.0   4.0 rlineto" << '\n';
		o << "    -4.0   4.0 rlineto" << '\n';
		o << "    -4.0  -4.0 rlineto closepath" << '\n';
		o << "    1.0 setlinewidth"	<< '\n';
		o << "    1 1 1 setrgbcolor stroke" << '\n';
		o << "    -2.0   0.0 moveto " << '\n';
		o << "     4.0   4.0 rlineto" << '\n';
		o << "    -4.0   4.0 rlineto" << '\n';
		o << "    -4.0  -4.0 rlineto closepath" << '\n';
		if (inset.grayscale == "Y")	o << "   0.5 0.5 0.5 setrgbcolor" << '\n';
		else 						o << "    0 1 0 setrgbcolor" << '\n';
		o << "    fill stroke" << '\n';
		o << "} def" << '\n' << '\n';

		o << "/s2_iter_axis {" << '\n';
		o << "  newpath" << '\n';
		o << "    1 1 1 setrgbcolor stroke" << '\n';
		o << "    -2.0   0.0 moveto " << '\n';
		o << "     4.0   4.0 rlineto" << '\n';
		o << "    -4.0   4.0 rlineto" << '\n';
		o << "    -4.0  -4.0 rlineto closepath" << '\n';
		if (inset.grayscale == "Y")	o << "   0.5 0.5 0.5 setrgbcolor" << '\n';
		else 						o << "   0 1 0 setrgbcolor" << '\n';
		o << " 1 setlinewidth stroke" << '\n';
		o << "} def" << '\n' << '\n';

		o << "/s3_axis {" << '\n';
		o << "  newpath" << '\n';
		o << "    -2.0   0.0 moveto " << '\n';
		o << "     4.0   4.0 rlineto" << '\n';
		o << "    -4.0   4.0 rlineto" << '\n';
		o << "    -4.0  -4.0 rlineto closepath" << '\n';
		o << "    1.0 setlinewidth"	<< '\n';
		if (inset.grayscale == "Y") o << "    0 0 0 setrgbcolor stroke" << '\n';
		else 						o << "    1 1 1 setrgbcolor stroke" << '\n';
		o << "    -2.0   0.0 moveto " << '\n';
		o << "     4.0   4.0 rlineto" << '\n';
		o << "    -4.0   4.0 rlineto" << '\n';
		o << "    -4.0  -4.0 rlineto closepath" << '\n';
		if (inset.grayscale == "Y") o << "    1 1 1 setrgbcolor" << '\n';
		else 						o << "    0 0 1 setrgbcolor" << '\n';
		o << "    fill stroke" << '\n';
		o << "} def" << '\n' << '\n';

		o << "/s3_iter_axis {" << '\n';
		o << "  newpath" << '\n';
		if (inset.grayscale == "Y") o << "    0 0 0 setrgbcolor stroke" << '\n';
		else 						o << "    1 1 1 setrgbcolor stroke" << '\n';
		o << "    -2.0   0.0 moveto " << '\n';
		o << "     4.0   4.0 rlineto" << '\n';
		o << "    -4.0   4.0 rlineto" << '\n';
		o << "    -4.0  -4.0 rlineto closepath" << '\n';
		if (inset.grayscale == "Y") o << "    1 1 1 setrgbcolor" << '\n';
		else 						o << "    0 0 1 setrgbcolor" << '\n';
		o << " 1 setlinewidth stroke" << '\n';
		o << "} def"  << '\n' << '\n';

		o << "/normalarrow {" << '\n';
		o << "  newpath  0.0  -6.0 moveto  0.0 6.0 lineto " << '\n';
		o << "    1 setlinewidth stroke" << '\n';
		o << "  newpath -2.0  3.0 moveto  2.0 10.0 rlineto " << '\n';
		o << "    2.0  -10.0 rlineto " << '\n';
		o << "    -2.0 3.0 rlineto " << '\n';
		o << "    -2.0 -3.0 rlineto " << '\n';
		o << "    1 setlinewidth" << '\n';
		o << "    fill stroke" << '\n';
		o << "    0.7 setlinewidth" << '\n';
		o << "    0.0 0.0 1.5 0.0 360.0 arc stroke"	<< '\n';
		o << "    1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth" << '\n';
		o << "    0.0 0.0 0.7 0.0 360.0 arc stroke"	<< '\n';
		o << "} def" << '\n' << '\n';

		o << "/dextralarrow {" 											<< '\n';
		o << "  newpath  0.0 -1.5 moveto -6.0 -1.5 lineto" 				<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "    stroke" 												<< '\n';
		o << "  newpath  -11.0 -1.2 moveto -4.0 -3.5 lineto" 			<< '\n';
		o << "    -6.0 -1.2 lineto" 									<< '\n';
		o << "    -11.0 -1.2 lineto" 									<< '\n';
		o << "    1 setlinewidth "										<< '\n';
		o << "    fill " 												<< '\n';
		o << "    stroke" 												<< '\n';
		o << "  newpath  0.0 1.5 moveto 6.0 1.5 lineto" 				<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "    stroke" 												<< '\n';
		o << "  newpath  11.0 1.2 moveto 4.0 3.5 lineto" 				<< '\n';
		o << "    6.0 1.2 lineto" 										<< '\n';
		o << "    11.0 1.2 lineto" 										<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "    fill stroke" 											<< '\n';
		o << "    0.7 setlinewidth" 									<< '\n'; // 0.0 0.0 0.0 setrgbcolor  removed
		o << "    0.0 0.0 1.5 0.0 360.0 arc stroke"						<< '\n';
		o << "    1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth" 			<< '\n';
		o << "    0.0 0.0 0.7 0.0 360.0 arc stroke"						<< '\n';
		o << "} def" 													<< '\n' << '\n';

		o << "/sinistralarrow {" 										<< '\n';
		o << "  newpath  0.0 1.5 moveto -6.0 1.5 lineto" 				<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "    stroke" 												<< '\n';
		o << "  newpath  -11.0 1.2 moveto -4.0 3.5 lineto" 				<< '\n';
		o << "    -6.0 1.2 lineto" 										<< '\n';
		o << "    -11.0 1.2 lineto" 									<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "     fill " 												<< '\n';
		o << "    stroke" 												<< '\n';
		o << "  newpath  0.0 -1.5 moveto 6.0 -1.5 lineto" 				<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "    stroke" 												<< '\n';
		o << "  newpath  11.0 -1.2 moveto 4.0 -3.5 lineto" 				<< '\n';
		o << "    6.0 -1.2 lineto" 										<< '\n';
		o << "    11.0 -1.2 lineto" 									<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "    fill stroke" 											<< '\n';

		o << "    0.7 setlinewidth" 									<< '\n'; // 0.0 0.0 0.0 setrgbcolor  removed
		o << "    0.0 0.0 1.5 0.0 360.0 arc stroke"						<< '\n';
		o << "    1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth" 			<< '\n';
		o << "    0.0 0.0 0.7 0.0 360.0 arc stroke"						<< '\n';
		o << "} def" 													<< '\n' << '\n';

		o << "/nonearrow {" 											<< '\n';
		o << "  newpath  0.0  -8.0 moveto  0.0 8.0 lineto " 			<< '\n';
		o << "    1 setlinewidth"										<< '\n';
		o << "    stroke" 												<< '\n';
		o << "    0.7 setlinewidth" 									<< '\n';
		o << "    0.0 0.0 1.5 0.0 360.0 arc"							<< '\n';
		o << "    gsave 1.00 1.00 1.00 setrgbcolor fill grestore"		<< '\n';
		o << "    stroke" 												<< '\n';
		o << "} def" 													<< '\n' << '\n';

		o << "/p_axis {" 							<< '\n';
		o << "  newpath"							<< '\n';
		o << "    -1.0  -1.0 moveto "				<< '\n';
		o << "    0.0   2.0 rlineto" 				<< '\n';
		o << "    2.0   0.0 rlineto" 				<< '\n';
		o << "    0.0  -2.0 rlineto closepath" 		<< '\n';
		o << "    0.5 setlinewidth"					<< '\n';
		if (inset.grayscale == "Y") o << "    0 0 0 setrgbcolor" << '\n';
		else 						o << "    1 0 0 setrgbcolor" << '\n';
		o << "    fill stroke" 						<< '\n';
		o << "} def" 								<< '\n' << '\n';

		o << "/t_axis {" 							<< '\n';
		o << "  newpath"							<< '\n';
		o << "   -1.0  -1.0 moveto "				<< '\n';
		o << "	  0.0   2.0 rlineto" 				<< '\n';
		o << "	  2.0   0.0 rlineto" 				<< '\n';
		o << "	  0.0  -2.0 rlineto closepath" 		<< '\n';
		o << "    0.5 setlinewidth"					<< '\n';
		if (inset.grayscale == "Y") o << "    0.33 0.33 0.33 setrgbcolor" << '\n';
		else 						o << "    0 0 1 setrgbcolor" << '\n';
		o << "    fill stroke" 						<< '\n';
		o << "} def" 								<< '\n' << '\n';

		o << "/n_axis {" 							<< '\n';
		o << "  newpath"							<< '\n';
		o << "   -1.0  -1.0 moveto "				<< '\n';
		o << "	  0.0   2.0 rlineto" 				<< '\n';
		o << "	  2.0   0.0 rlineto" 				<< '\n';
		o << "	  0.0  -2.0 rlineto closepath" 		<< '\n';
		o << "    0.5 setlinewidth"					<< '\n';
		if (inset.grayscale == "Y") o << "    0.66 0.66 0.66 setrgbcolor" << '\n';
		else 						o << "    0 1 0 setrgbcolor" << '\n';
		o << "    fill stroke" 						<< '\n';
		o << "} def" 								<< '\n' << '\n';
	}

}

void PS_border (GDB inGDB, ofstream& o, INPSET inset, PAPER P) {

	o << "newpath" << '\n';

	o << "  " << fixed << setprecision (3) << P.A << " " << fixed << setprecision (3) << P.Y - P.A - 10.0 << " moveto "
			<< fixed << setprecision (3) << P.A << " " << fixed << setprecision (3) << P.A << " lineto" << '\n';
	o << "  " << fixed << setprecision (3) << P.X - P.A << " " << fixed << setprecision (3) << P.A << " lineto" << '\n';
	o << "  " << fixed << setprecision (3) << P.X - P.A << " " << fixed << setprecision (3) << P.Y - P.A << " lineto" << '\n';
	o << "  " << fixed << setprecision (3) << P.A + 600.0 << " " << fixed << setprecision (3) << P.Y - P.A << " lineto" << '\n';
	o << "  1.5 setlinewidth" << '\n';
	o << "  0 0 0 setrgbcolor stroke" << '\n' << '\n';

	o << "/ArialNarrow-Bold findfont 36 scalefont setfont" 					<< '\n';
	o << "  " << P.O5X + (5.0 * P.A) << " " << (P.O5Y + P.B)  << " moveto " << '\n';
	o << "  (ORIGINAL)  0.8 0.8 0.8 setrgbcolor show"  << '\n';

	o << "  " << P.O6X + (4.0 * P.A) << " " <<  (P.O6Y + P.B)  << " moveto " << '\n';
	o << "  (CORRECTED) 0.8 0.8 0.8 setrgbcolor show"  << '\n';

	o << "/ArialNarrow-Bold findfont 12 scalefont setfont" 					<< '\n';
	o
	<< "  " << P.A - P.B + 20.0 << " " << P.Y - P.A - 5.5 << " moveto "
	<< "(ALL OBSERVED " << inGDB.DATATYPE << " DATA FROM DATA POINT " << inGDB.LOC ;

	if (inset.group == "Y") 	o << ", GROUP '" << inGDB.GC << "'" ;

	if (inGDB.FORMATION != "") 	o << " - " << inGDB.FORMATION << " FORMATION" ;

	o << ") " << '\n';

	o << "0 0 0 setrgbcolor show" << '\n' << '\n';

	o << "  0.3 setlinewidth 0.5 0.5 0.5 setrgbcolor" << '\n';
}

void PS_stress_scale (ofstream& o, INPSET inset, PAPER P) {

	double value = 0.0;
	double step = 1.0 / 100.0;

	VCTR color;

	do {

		value = value + step;

		color = generate_stress_colors (value, inset);

		o << fixed << setprecision (2) ;
		o
		<< "  " << fixed << setprecision (2) << color.X
		<< " "  << fixed << setprecision (2) << color.Y
		<< " "  << fixed << setprecision (2) << color.Z

		<< " setrgbcolor 2 setlinewidth" << '\n' << '\n';

		o
		<< "  newpath "
		<< fixed << setprecision (3) << P.O1X + P.R + 1.0 * P.B << " " << P.O1Y - P.R + (0.666 * P.R * value) << " moveto "
		<< fixed << setprecision (3) << P.O1X + P.R + 1.5 * P.B << " " << P.O1Y - P.R + (0.666 * P.R * value)
		<< " lineto stroke" << '\n';

		o
		<< "  newpath "
		<< fixed << setprecision (3) << P.O2X + P.R + 1.0 * P.B << " " << P.O2Y - P.R + (0.666 * P.R * value)<< " moveto "
		<< fixed << setprecision (3) << P.O2X + P.R + 1.5 * P.B << " " << P.O2Y - P.R + (0.666 * P.R * value)
		<< " lineto stroke" << '\n';

	} while (value < 3.0);

	o << "/ArialNarrow-Bold findfont 6 scalefont setfont" << '\n';

	o << "  0 0 0 setrgbcolor " << '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O1X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O1Y - 1.0 * P.R << " moveto "
	<<" 90 rotate (EXTENSION) show 270 rotate"<< '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O1X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O1Y + 1.0 * P.R - 0.92 * P.A << " moveto "
	<< " 90 rotate (COMPRESSION) show 270 rotate "<< '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O2X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O2Y - 1.0 * P.R << " moveto "
	<<" 90 rotate (EXTENSION) show 270 rotate"<< '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O2X + 1.0 * P.R + 0.9 * P.B
	<< " "  << fixed << setprecision (3) << P.O2Y + 1.0 * P.R - 0.92 * P.A << " moveto "
	<< " 90 rotate (COMPRESSION) show 270 rotate "<< '\n';
}

void ps_clusteringresult (ofstream& o, INPSET inset, PAPER P, int clusternumber) {

	o << "/ArialNarrow findfont 8 scalefont setfont" << '\n';
	o
	<< "  " << fixed << setprecision (3) << P.O1X - P.R - P.A
	<< " "  << fixed << setprecision (3) << P.O1Y - P.R - 33.0 << " moveto" << '\n';

	if (inset.clusternumber == "A") o << "  (Automatic clustering " ;
	else							o << "  (User defined clustering " ;

	o
	<< " into " << clusternumber << " clusters with "
	<< fixed << setprecision (1) << " % error) show"<< '\n';


	o
	<< "  " << fixed << setprecision (3) << P.O2X - P.R - P.A
	<< " "  << fixed << setprecision (3) << P.O2Y - P.R - 33.0 << " moveto" << '\n';

	if (inset.clusternumber == "A") o << "  (Automatic clustering " ;
	else							o << "  (User defined clustering " ;

	o
	<< " into " << clusternumber << " clusters with "
	<< fixed << setprecision (1) << " % error) show"<< '\n';
}

void PS_net (ofstream& o, INPSET inset, PAPER P) {

	int angle;

	string nethemisphere = "Schmidt-net,";

	if (inset.plottype == "W")	nethemisphere = "Wulff-net,";

	if (inset.hemisphere == "U") nethemisphere = nethemisphere + " upper hemisphere";
	else nethemisphere = nethemisphere + " lower hemisphere";

	if (inset.plot == "H")  nethemisphere = "Hoeppener-plot, " + nethemisphere;
	else nethemisphere = "Angelier-plot, " + nethemisphere;

	o << '\n';
	o << "  0.00 0.00 0.00 setrgbcolor 1 setlinewidth" << '\n' << '\n';

	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O1X << " "
	<< fixed << setprecision (3) << P.O1Y << " " << P.R << " 0 360 arc stroke" << '\n';
	o
	<< "  1.2 setlinewidth" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y + P.R + P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X + P.R << " " << P.O1Y << " moveto "
	<< fixed << setprecision (3) << P.O1X + P.R + P.C << " " << P.O1Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y - P.R - P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X - P.R << " " << P.O1Y << " moveto "
	<< fixed << setprecision (3) << P.O1X - P.R - P.C << " " << P.O1Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O1X << " " << P.O1Y - P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O1X + P.C << " " << P.O1Y << " moveto "
	<< fixed << setprecision (3) << P.O1X - P.C << " " << P.O1Y << " lineto stroke" << '\n' << '\n';


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O2X << " "
	<< fixed << setprecision (3) << P.O2Y << " " << P.R << " 0 360 arc stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y + P.R + P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X + P.R << " " << P.O2Y << " moveto "
	<< fixed << setprecision (3) << P.O2X + P.R + P.C << " " << P.O2Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y - P.R - P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X - P.R << " " << P.O2Y << " moveto "
	<< fixed << setprecision (3) << P.O2X - P.R - P.C << " " << P.O2Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O2X << " " << P.O2Y - P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O2X + P.C << " " << P.O2Y << " moveto "
	<< fixed << setprecision (3) << P.O2X - P.C << " " << P.O2Y << " lineto stroke" << '\n' << '\n';


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O3X << " "
	<< fixed << setprecision (3) << P.O3Y << " " << P.R << " 0 360 arc stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y + P.R + P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X + P.R << " " << P.O3Y << " moveto "
	<< fixed << setprecision (3) << P.O3X + P.R + P.C << " " << P.O3Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y - P.R - P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X - P.R << " " << P.O3Y << " moveto "
	<< fixed << setprecision (3) << P.O3X - P.R - P.C << " " << P.O3Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O3X << " " << P.O3Y - P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O3X + P.C << " " << P.O3Y << " moveto "
	<< fixed << setprecision (3) << P.O3X - P.C << " " << P.O3Y << " lineto stroke" << '\n' << '\n';


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O4X << " "
	<< fixed << setprecision (3) << P.O4Y << " " << P.R << " 0 360 arc stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y + P.R << " moveto "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y + P.R + P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X + P.R << " " << P.O4Y << " moveto "
	<< fixed << setprecision (3) << P.O4X + P.R + P.C << " " << P.O4Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y - P.R << " moveto "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y - P.R - P.C << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X - P.R << " " << P.O4Y << " moveto "
	<< fixed << setprecision (3) << P.O4X - P.R - P.C << " " << P.O4Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y + P.C << " moveto "
	<< fixed << setprecision (3) << P.O4X << " " << P.O4Y - P.C 		<< " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O4X + P.C << " " << P.O4Y << " moveto "
	<< fixed << setprecision (3) << P.O4X - P.C << " " << P.O4Y 		<< " lineto stroke" << '\n' << '\n';


	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O5X << " "
	<< fixed << setprecision (3) << P.O5Y << " " << P.R << " 270 360 arc stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O5X << " " << P.O5Y << " moveto "
	<< fixed << setprecision (3) << P.O5X + P.R + P.C << " " << P.O5Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O5X << " " << P.O5Y << " moveto "
	<< fixed << setprecision (3) << P.O5X << " " << P.O5Y - P.R - P.C << " lineto stroke" << '\n';
	o
	<< "newpath "
	<< fixed << setprecision (3) << P.O6X << " "
	<< fixed << setprecision (3) << P.O6Y << " " << P.R << " 270 360 arc stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O6X << " " << P.O6Y << " moveto "
	<< fixed << setprecision (3) << P.O6X + P.R + P.C << " " << P.O6Y << " lineto stroke" << '\n';
	o
	<< "  newpath "
	<< fixed << setprecision (3) << P.O6X << " " << P.O6Y << " moveto "
	<< fixed << setprecision (3) << P.O6X << " " << P.O6Y - P.R - P.C << " lineto stroke" << '\n';


	angle = 0;

	o
	<< "  "  << fixed << setprecision (3) << P.O3X
	<< " "   << fixed << setprecision (3) << P.O3Y << " translate"  << '\n' << '\n';

	do {

		o << "  0.0 "  << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << '\n';

		angle = angle + 10;

	} while (angle < 360);

	o << "  " << fixed << setprecision (3) << - P.O3X
	  << " "  << fixed << setprecision (3) << - P.O3Y << " translate"  << '\n';

	angle = 0;

	o << "  " << fixed << setprecision (3) << P.O4X
	  << " "  << fixed << setprecision (3) << P.O4Y << " translate"  << '\n' << '\n';

	do {

		o << "  0.0 " << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << '\n';

		angle = angle + 10;

	} while (angle < 360);

	o << "  "  << fixed << setprecision (3) << - P.O4X
	  << " "   << fixed << setprecision (3) << - P.O4Y << " translate"  << '\n' << '\n';

	o << "  " << fixed << setprecision (3) << P.O5X
	  << " "  << fixed << setprecision (3) << P.O5Y << " translate"  << '\n' << '\n';

	o << "  180 rotate" << '\n';

	angle = 270;

	do {

		o << "  0.0 " << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << '\n';

		angle = angle + 10;

	} while (angle < 360);

	o << "  -270 rotate" << '\n';
	o << "  "  << fixed << setprecision (3) << - P.O5X
	  << " "   << fixed << setprecision (3) << - P.O5Y << " translate"  << '\n' << '\n';

	o << "  " << fixed << setprecision (3) << P.O6X
	  << " "  << fixed << setprecision (3) << P.O6Y << " translate"  << '\n' << '\n';
	o << "  180 rotate" << '\n';

	angle = 270;

	do {

		o << "  0.0 " << fixed << setprecision (3) << P.R - 6.0 << " moveto 0.0 "
		  << fixed << setprecision (3) << P.R + 0.0 << " lineto stroke 10 rotate" << '\n';

		angle = angle + 10;

	} while (angle < 360);

	o << "  -270 rotate" << '\n';
	o << "  "  << fixed << setprecision (3) << - P.O6X
	  << " "   << fixed << setprecision (3) << - P.O6Y << " translate"  << '\n' << '\n';

	o << "/ArialNarrow-Bold findfont 12 scalefont setfont" << '\n';
	o << "  " <<  fixed << setprecision (3) << P.O1X - 3.5 << " "
			  <<  fixed << setprecision (3) << P.O1Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << '\n';
	o << "  " <<  fixed << setprecision (3) << P.O2X - 3.5 << " "
			  <<  fixed << setprecision (3) << P.O2Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << '\n';
	o << "  " <<  fixed << setprecision (3) << P.O3X - 3.5 << " "
			  <<  fixed << setprecision (3) << P.O3Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << '\n';
	o << "  " <<  fixed << setprecision (3) << P.O4X - 3.5 << " "
	 		  <<  fixed << setprecision (3) << P.O4Y + P.R + 14.0 << " moveto "
	  << "  (N) 0 0 0 setrgbcolor show" << '\n';

	o << "/ArialNarrow findfont 8 scalefont setfont" << '\n';
	o << "  " <<  fixed << setprecision (3) << P.O1X - P.R - 0.2 * P.B
	  << " "  <<  fixed << setprecision (3) << P.O1Y - P.R - 20.0 << " moveto"
	  << "  (" << nethemisphere << ") show" << '\n';
	o << "  " <<  fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
	  << " "  <<  fixed << setprecision (3) << P.O2Y - P.R - 20.0 << " moveto"
	  << "  (" << nethemisphere << ") show" << '\n';

	o << 20.0 * P.A << " " << P.A + 5.0 << " moveto"
	  << "  (Plotted by SG2PS - for reference see www.sg2ps.eu webpage.) show" << '\n' << '\n';
}

void PS_stressdata (ofstream& o, CENTER center, PAPER P, STRESSFIELD sf, string method) {

	o
	<< "/ArialNarrow findfont 8 scalefont setfont" << '\n';
	o
	<< "0 0 0 setrgbcolor" << '\n';
	o
	<< "  " << fixed << setprecision (3) << center.X + (center.radius / 2.0) - P.A
	<< " "  << fixed << setprecision (3) << center.Y - center.radius - 20.0 << " translate" << '\n';
	o
	<< "   0 0 moveto " << '\n';

	if 		(method == "BINGHAM")		o << "(Fracture statistics after Bingham (1964) ) show" << '\n';
	else if (method == "NDA")  			o << "(Regression after Sprang (1972) ) show" << '\n';
	else if (method == "FRY") 			o << "(Regression after Fry (1999) ) show" << '\n';
	else if (method == "SHAN") 			o << "(Regression after Shan et al. (2003) ) show" << '\n';
	else if (method == "ANGELIER") 		o << "(Inversion after Angelier (1990) ) show" << '\n';
	else if (method == "PTN") 			o << "(Regression after Turner (1953) ) show" << '\n';
	else if (method == "MICHAEL") 		o << "(Inversion after Michael (1984) ) show" << '\n';
	else if (method == "MOSTAFA") 		o << "(Inversion after Mostafa (2005) ) show" << '\n';
	else {}

	o
	<< "   0.0 -8.0 moveto" << '\n';

	if (method == "BINGHAM")  {

		o
		<< "(e1: "  << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_1.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_1.DIP
		<< ", e2: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_2.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_2.DIP
		<< ", e3: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_3.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_3.DIP << ") show" <<'\n';
	}

	else {

		o
		<< "(S1: "  << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_1.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_1.DIP
		<< ", S2: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_2.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_2.DIP
		<< ", S3: " << setfill ('0') << setw (3) << fixed << setprecision (0) << sf.S_3.DIPDIR
		<< "/"      << setfill ('0') << setw (2) << fixed << setprecision (0) << sf.S_3.DIP << ") show" <<'\n';
	}

	if (method != "BINGHAM") {

		o
		<< "   0.0 -16.0 moveto" << '\n';

		o
		<< "(R: "   << setw (4) << fixed << setprecision (3) << sf.stressratio
		<< ", R': " << setw (4) << fixed << setprecision (3) << sf.delvaux_str
		<< ", "     << sf.delvaux_rgm << ")  show" <<'\n';
	}

	else  {

		o
		<< "   0.0 -16.0 moveto" << '\n';

		o
		<<  "(E1: " << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.X * 100.0 << "%"
		<< ", E2: " << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Y * 100.0 << "%"
		<< ", E3: " << setw (4) << fixed << setprecision (1) << sf.EIGENVALUE.Z * 100.0 << "%) show" <<'\n';
	}

	o
	<< "  " << fixed << setprecision (3) << - center.X - (center.radius / 2.0) + P.A
	<< " "  << fixed << setprecision (3) << - center.Y + center.radius + 20.0 << " translate" << '\n';
}

void PS_stressarrows (ofstream& o, CENTER center, PAPER P, STRESSFIELD sf) {

	if ((sf.regime == "COMPRESSIONAL") || (sf.regime == "STRIKE-SLIP")) {

		o << fixed << setprecision (3) << center.X << " "
		  << fixed << setprecision (3) << center.Y << " translate" << '\n';

		o << "  " << - sf.shmax << fixed << setprecision (3) << " rotate" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<'\n';
		o << "  newpath compression_arrow" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<'\n';
		o << "  " << sf.shmax << fixed << setprecision (3) << " rotate" << '\n';

		o << "  180 rotate" << '\n';

		o << "  " << - sf.shmax << fixed << setprecision (3) << " rotate" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<'\n';
		o << "  newpath compression_arrow" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<'\n';
		o << "  " << sf.shmax << fixed << setprecision (3) << " rotate" << '\n';

		o << "  180 rotate" << '\n';

		o << fixed << setprecision (3) << - center.X << " "
		  << fixed << setprecision (3) << - center.Y << " translate" << '\n';
	}

	if ((sf.regime == "EXTENSIONAL") || (sf.regime == "STRIKE-SLIP")) {

		o << fixed << setprecision (3) << center.X << " "
		  << fixed << setprecision (3) << center.Y << " translate" << '\n';

		o << "  " << - sf.shmin << fixed << setprecision (3) << " rotate" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<'\n';
		o << "  newpath extension_arrow" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<'\n';
		o << "  " << sf.shmin << fixed << setprecision (3) << " rotate" << '\n';

		o << "  180 rotate" << '\n';

		o << "  " << - sf.shmin << fixed << setprecision (3) << " rotate" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << center.radius + (0.2 * P.B) << " translate" <<'\n';
		o << "  newpath extension_arrow" << '\n';
		o << "  0.0 " << fixed << setprecision (3) << - center.radius - (0.2 * P.B) << " translate" <<'\n';
		o << "  " << sf.shmin << fixed << setprecision (3) << " rotate" << '\n';

		o << "  180 rotate" << '\n';

		o << fixed << setprecision (3) << - center.X << " "
		  << fixed << setprecision (3) << - center.Y << " translate" << '\n';
	}
}

void PS_mohr_circle (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER mohr_center, PAPER P, STRESSFIELD sf, STRESSTENSOR st, bool compression_positive) {

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

	color = generate_stress_colors (sf.delvaux_str, inset);

	o << "newpath" << '\n';
	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + 2.5 * P.A
	<< " "  << fixed << setprecision (3) << mohr_center.Y
	<< " "  << fixed << setprecision (3) << 2.5 * P.A
	<< " 0 180 arc 3 setlinewidth "
	<< 		  fixed << setprecision (2) << color.X
	<< " " << fixed << setprecision (2) << color.Y
	<< " " << fixed << setprecision (2) << color.Z
	<< " setrgbcolor stroke " << '\n';

	o << "newpath" << '\n';
	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + (0.5 * fi * X)
	<< " "  << fixed << setprecision (3) << mohr_center.Y
	<< " "  << fixed << setprecision (3) << 0.5 * fi * X
	<< " 0 180 arc 3 setlinewidth "
	<< 		  fixed << setprecision (2) << color.X
	<< " " << fixed << setprecision (2) << color.Y
	<< " " << fixed << setprecision (2) << color.Z
	<< " setrgbcolor stroke " << '\n';

	o << "newpath" << '\n';
	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + 2.5 * P.A + (0.5 * fi * X)
	<< " "  << fixed << setprecision (3) << mohr_center.Y
	<< " "  << fixed << setprecision (3) << 2.5 * P.A - (0.5 * fi * X)
	<< " 0 180 arc 3 setlinewidth "
	<< 		  fixed << setprecision (2) << color.X
	<< " " << fixed << setprecision (2) << color.Y
	<< " " << fixed << setprecision (2) << color.Z
	<< " setrgbcolor stroke " << '\n';
	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << '\n';

	o
	<< "  " << fixed << setprecision (3) << mohr_center.X - 4.0
	<< " "  << fixed << setprecision (3) << mohr_center.Y - 10.0
	<< "  moveto (s3) 0 0 0 setrgbcolor show " << '\n';

	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + X - 4.0
	<< " "  << fixed << setprecision (3) << mohr_center.Y - 10.0
	<< "  moveto (s1) 0 0 0 setrgbcolor show " << '\n';

	o
	<< "  " << fixed << setprecision (3) << mohr_center.X + (fi * X) - 4.0
	<< " "  << fixed << setprecision (3) << mohr_center.Y - 10.0
	<< "  moveto (s2) 0 0 0 setrgbcolor show " << '\n';

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
			<< " 0.7 0 360 arc 3 setlinewidth 1 1 1 setrgbcolor stroke" << '\n';

			o
			<< "newpath "
			<< fixed << setprecision (3) << mohr_center.X + X * normal_s << " "
			<< fixed << setprecision (3) << mohr_center.Y + X * shear_s
			<< " 0.7 0 360 arc 2 setlinewidth 0 0 0 setrgbcolor stroke" << '\n';
		}

		j++;

	} while (j < inGDB.size());
}

void PS_RUP_ANG_distribution (vector <GDB> inGDB, INPSET inset, vector <VALLEY> V, ofstream& o, CENTER center, PAPER P, string method) {

	double DATA_min = return_datamin (inGDB, method);
	double DATA_max = return_datamax (inGDB, method);

	size_t bin_number = return_DATA_ideal_bin_number (GDB_to_table(inGDB, method));

	double binsize = (DATA_max - DATA_min) / bin_number;

	vector <HISTOGRAM> H = generate_DATA_histogram (GDB_to_table (inGDB, method), bin_number);

	H = sort_by_COUNT (H);

	vector <line_RUP_ANG> L_R_A = generate_graph_histogram (H, V, DATA_max);

	ps_RUP_ANG_header (o, center, P, method);

	ps_draw_histogram_bars (inGDB, L_R_A, inset, o, center, P, DATA_min, DATA_max, bin_number, binsize, method);

	ps_percentage (o, center, P, method, DATA_max);

	ps_percentage_max (o, center, P, method, DATA_max);
}

void PS_stress_state (ofstream& o, PAPER P, CENTER center, STRESSFIELD sf) {

	double value = sf.delvaux_str;

	o << " 1 1 1 setrgbcolor 5 setlinewidth" << '\n';

	o
	<< "  newpath "
	<< fixed << setprecision (3) << center.X + P.R + 0.9 * P.B << " " << center.Y - P.R + (0.666 * P.R * value) << " moveto "
	<< fixed << setprecision (3) << center.X + P.R + 1.6 * P.B << " " << center.Y - P.R + (0.666 * P.R * value)
	<< " lineto stroke" << '\n';
}

void PS_folddata (GDB in, ofstream& o, CENTER center) {

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << '\n';

	o << "0 0 0 setrgbcolor" << '\n';

	o
	<< "  " << fixed << setprecision (3) << center.X + (center.radius / 2.0)
	<< " "  << fixed << setprecision (3) << center.Y - center.radius << " translate" << '\n';

	o
	<< "   0 0 moveto "
	<< "(Fold great circle: "
	<< setfill ('0') << setw (3) << fixed << setprecision (0) << in.avd.DIPDIR << "/"
	<< setfill ('0') << setw (2) << fixed << setprecision (0) << in.avd.DIP
	<< ") show" <<'\n';

	o
	<< "  " << fixed << setprecision (3) << - center.X - (center.radius / 2.0)
	<< " "  << fixed << setprecision (3) << - center.Y + center.radius << " translate" << '\n';
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
		<< "  " << fixed << setprecision (5) << Y << " translate " << '\n';
		o
		<< "   newpath s1_axis" << '\n';
		o
		<< "  " << fixed << setprecision (5) << -X
		<< "  " << fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else if (type == "S1_ITER") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath s1_iter_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else if (type == "S2") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath s2_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else if (type == "S2_ITER") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath s2_iter_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else if (type == "S3") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath s3_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else if (type == "S3_ITER") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath s3_iter_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << '\n';
	}


	else if (type == "P") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				<< fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath p_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				<< fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else if (type == "N") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				  << fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath n_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				  << fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else if (type == "T") {

		o << "  " << fixed << setprecision (5) <<  X << " "
				  << fixed << setprecision (5) <<  Y << " translate " << '\n';
		o << "  newpath t_axis" << '\n';
		o << "  " << fixed << setprecision (5) << -X << " "
				  << fixed << setprecision (5) << -Y << " translate " << '\n';
	}

	else {

		o << "  " << i.PSCOLOR << " setrgbcolor newpath "
		  << fixed << setprecision (5) << X << " "
		  << fixed << setprecision (5) << Y << " 0.7 0 360 arc 0 fill stroke" << '\n';
		o << "%%  " << setfill ('0') << setw (3) << setprecision (3) << dd.DIPDIR << "/" << setfill ('0') << setw (3) << setprecision (3) << dd.DIP << '\n' << '\n';
		o << "  " << i.PSCOLOR << " setrgbcolor newpath "
		  << fixed << setprecision (5) << X << " "
		  << fixed << setprecision (5) << Y << " 0.7 0 360 arc 0 stroke" << '\n';
	}

	if (label) {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << '\n';
		o
		<< "  " << fixed << setprecision (3) << X << " " << fixed << setprecision (3) << Y  <<" translate" << '\n';

		o
		<< "  0.0 0.0 moveto 10.0 10.0 lineto stroke" << '\n';
		o
		<< "  10.0 10.0 moveto ("	<< i.ID << ") show"	<< '\n';

		o
		<< "  " << fixed << setprecision (3) << - X << " " << fixed << setprecision (3) << - Y  <<" translate" << '\n';
	}
}

void PS_plane (GDB i, ofstream& o, INPSET inset, CENTER center, bool label, string type) {

	int j = 0;
	int steps = 2;
	double X_A, Y_A, X_B, Y_B, X_C, Y_C, X_O, Y_O;
	double b , c, alfa, r;

	vector <VCTR> PP;
	VCTR temp, torotate, axis;

	bool OT = (i.avS0offset == "OVERTURNED" || i.avS0offset == "O");
	bool OTB = (i.OFFSET == "OVERTURNED" || i.OFFSET == "O");

	bool OTAB = (OT && type == "AV");

	if (inset.plottype == "S") steps = 120;

	if (type == "C") {

		i.corr = i.corrL;
		i.D.Z = i.DC.Z;
		i.N = i.NC;
	}
	else if (type == "AV") {

		i.corr = 	i.avS0d;
		i.D =  		flip_D_vector(i.avS0D);
		i.N = 		unitvector(NXNYNZ_from_DXDYDZ (i.D));
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
		<< "%%____" << setfill ('0') << fixed << setprecision (2) << i.corr.DIPDIR <<
				"/" << setfill ('0') << fixed << setprecision (2) << i.corr.DIP << '\n';

		if (inset.grayscale == "Y") o << "  0.20 0.20 0.20 setrgbcolor 2 setlinewidth newpath " << '\n';
		else 						o << "  1.00 0.00 0.00 setrgbcolor 1 setlinewidth newpath " << '\n';
	}

	else if (type == "FOLD") {

		o << "%%____" << setfill ('0') << fixed << setprecision (2) << i.corr.DIPDIR <<
				  "/" << setfill ('0') << fixed << setprecision (2) << i.corr.DIP << '\n';
		if (inset.grayscale == "Y")	o << "  0.60 0.60 0.60 setrgbcolor 2 setlinewidth newpath " << '\n';
		else 						o << "  0.00 0.00 1.00 setrgbcolor 1 setlinewidth newpath " << '\n';
	}

	else {

		o << "%%____" << setfill ('0') << fixed << setprecision (2) << i.corr.DIPDIR <<
				"/" << setfill ('0') << fixed << setprecision (2) << i.corr.DIP << '\n';
		o << "  " << i.PSCOLOR << " setrgbcolor " << inset.linewidth << " setlinewidth newpath " << '\n';
	}


	if (inset.plottype == "W") {

		double begin_arc = 0.0;

		if 	    ((X_B > X_O) && (Y_B > Y_O)) begin_arc =         ASIN((Y_B - Y_O) / r);
		else if ((X_B < X_O) && (Y_B > Y_O)) begin_arc = 180.0 - ASIN((Y_B - Y_O) / r);
		else if ((X_B < X_O) && (Y_B < Y_O)) begin_arc = 180.0 + ASIN((Y_O - Y_B) / r);
		else begin_arc = 360.0 - ASIN((Y_O - Y_B) / r);

		double end_arc = begin_arc + (2.0 * alfa);

		o
		<< "  " << fixed << setprecision (3) << X_O
		<< " "  << fixed << setprecision (3) << Y_O
		<< " "  << fixed << setprecision (3) << r
		<< " "  << fixed << setprecision (3) << begin_arc
		<< " "  << fixed << setprecision (3) << end_arc << " arc "
		<< '\n';
	}
	else {
		j = 1;
		o << PP.at(0).X << " " << PP.at(0).Y << " moveto" << '\n';

		do {
			o << PP.at(j).X << " " << PP.at(j).Y << " " ;
			j++;

			o << PP.at(j).X << " " << PP.at(j).Y << " " ;
			j++;

			o << PP.at(j).X << " " << PP.at(j).Y << " curveto" << '\n';
			j++;

		} while (j < steps + 1);
	}

	if (OTAB) {

		o << "  [6 6] 0 setdash stroke" << '\n';
		o << "  [   ] 0 setdash " << '\n';
	}

	else if (type == "FOLD") {

		o << "  [3 3] 0 setdash stroke" << '\n';
		o << "  [   ] 0 setdash " << '\n';
	}
	else if (OTB) {

			o << "  [3 3] 0 setdash stroke" << '\n';
			o << "  [   ] 0 setdash " << '\n';
		}
	else if (type == "C") {

		o << "  [3 3] 0 setdash stroke" << '\n';
		o << "  [   ] 0 setdash " << '\n';
	}

	else if (type == "AV") {

		o << "stroke" << '\n' << '\n';
	}

	else {

		if (inset.grayscale == "Y")  {

			o << "  [" << i.DASHED << "] 0 setdash stroke" << '\n';
			o << "  [   ] 0 setdash " << '\n';
		}

		else o << "stroke" << '\n' << '\n';
	}

	if (label) {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << '\n';
		o
		<< "  " << fixed << setprecision (3) << X_C << " " << fixed << setprecision (3) << Y_C  <<" translate" << '\n';
		o
		<< "  " <<  - i.corr.DIPDIR + 180.0 << " rotate" << '\n';
		o
		<< "  0.0 0.0 moveto 10.0 0.0 lineto stroke" << '\n';
		o
		<< "  10.0 -3.0 moveto ("	<< i.ID << ") show"	<< '\n';

		o
		<< "  " << + i.corr.DIPDIR - 180.0 << " rotate" << '\n';

		o
		<< "  " << fixed << setprecision (3) << - X_C << " " << fixed << setprecision (3) << - Y_C  <<" translate" << '\n';
	}
}

void PS_polepoint (GDB i, ofstream& o, INPSET inset, CENTER center, bool label, string type) {

	VCTR temp;
	VCTR in;
	DIPDIR_DIP dd;

	if (type == "FOLD") {

		dd = i.avd;
		in = i.avD;
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

	o << "%%" << '\n' << "%%" << '\n';
	o << "%%  " << setfill ('0') << setw (3) << setprecision (3) << i.corr.DIPDIR 	<<
			 "/"<< setfill ('0') << setw (3) << setprecision (3) << i.corr.DIP 		<< "----------------------------------------" << '\n';

	if (type == "AV") {

		o << "  1.0 0.0 0.0 setrgbcolor 0.7 setlinewidth" << '\n';
		o
		<< "  newpath " << fixed << setprecision (3) << in.X
		<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << '\n';
		o << "   0 0 0 setrgbcolor" << '\n';
	}

	else if (type == "FOLD") {

		o << "  0.0 0.0 1.0 setrgbcolor 0.7 setlinewidth" << '\n';
		o
		<< "  newpath " << fixed << setprecision (3) << in.X
		<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << '\n';
		o << "   0 0 0 setrgbcolor" << '\n';
	}

	else if (type == "C") {

		o << "  0.0 0.0 1.0 setrgbcolor 0.7 setlinewidth" << '\n';
		o
		<< "  newpath " << fixed << setprecision (3) << in.X
		<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 0.9 0.9 0.9 setrgbcolor fill grestore stroke" << '\n';
		o << "  0 0 0 setrgbcolor" << '\n';
	}

	else {

		o << "  " << i.PSCOLOR << " setrgbcolor 0.7 setlinewidth" << '\n';
		o << "  newpath " << fixed << setprecision (3) << in.X
				<< " " << fixed << setprecision (3) << in.Y << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << '\n';
		o << "  0 0 0 setrgbcolor" << '\n';
	}

	if (label) {

		o << "/ArialNarrow-Italic findfont 6 scalefont setfont" << '\n'
		  << "  " << i.PSCOLOR << " setrgbcolor " << in.X + 3.0 << " " << in.Y - 3.0 << " moveto ("
		  << setfill ('0') << setw (3) << setprecision (3) << i.corr.DIPDIR << "/"
		  << setfill ('0') << setw (3) << setprecision (3) << i.corr.DIP << ") show" << '\n'
		  << "0 0 0 setrgbcolor" << '\n';
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

	o << "  " << inset.linewidth << "  setlinewidth" << '\n';
	o << "  " << i.PSCOLOR << "  setrgbcolor newpath "
			<< setw (3) << setprecision (3) << X << " "
			<< setw (3) << setprecision (3) << Y << " 1 0 360 arc 0 " << '\n';
	o << "  closepath fill stroke" << '\n';
	o << "  " << setw (3) << setprecision (5) << X << " "
			<< setw (3) << setprecision (5) << Y << " translate " << '\n';

	if (d.Z > 0.0) {

		o << - dd.DIPDIR + 180.0 << " rotate" << '\n';
		o << "  newpath normalarrow" << '\n';
		o << "  " << dd.DIPDIR - 180.0 << " rotate " << '\n';
	}

	else {

		o << - dd.DIPDIR + 000.0 << " rotate" << '\n';
		o << "  newpath normalarrow" << '\n';
		o << "  " << dd.DIPDIR + 000.0 << " rotate " << '\n';
	}

	o  << fixed << setprecision (3) << -X << " "	<< fixed << setprecision (5)<< -Y << " translate " << '\n';
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

	o << "  " << i.PSCOLOR << " setrgbcolor" << '\n';

	if ((inset.plot == "H") && (!(i.OFFSET == "NONE"))){

		if ((i.UP)) DIPDIR = DIPDIR + 180.0;

		o
		<< "  " << fixed << setprecision (2) << X
		<< "  "	<< fixed << setprecision (2) << Y << " translate " << '\n';
		o
		<< "  " << - DIPDIR  << " rotate" << '\n';
		o
		<< "  " << "  newpath normalarrow" << '\n';
		o
		<< "  " <<   DIPDIR  << " rotate " << '\n';
		o
		<< "  " << fixed << setprecision (2) << -X
		<< "  " << fixed << setprecision (2) << -Y << " translate " << '\n';
	}

	else {

		if (i.corrOFFSET == "INVERSE") {

			o
			<< "  " << fixed << setprecision (2) << X
			<< " "	<< fixed << setprecision (2) << Y
			<< " translate " << - DIPDIR + 180.0 << " rotate" << '\n';
			o << "  newpath normalarrow" << '\n';
			o << "  " <<   DIPDIR + 180.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2) << -Y << " translate " << '\n';
		}

		if (i.corrOFFSET == "NORMAL") {

			o
			<< "  " << fixed << setprecision (2) << X
			<< " "	<< fixed << setprecision (2) << Y << " translate "
					<< - DIPDIR + 000.0 << " rotate" << '\n';
			o << "  newpath normalarrow" << '\n';
			o << "  " <<   DIPDIR - 000.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2 )<< -Y << " translate " << '\n';
		}

		if (i.corrOFFSET == "DEXTRAL") {

			o
			<< "  " << setw (6) << fixed << setprecision (2) << X
			<< " "	<< setw (5) << fixed << setprecision (2) << Y << " translate "
					<< - DIPDIR + 90.0 << " rotate" << '\n';
			o << "  newpath dextralarrow" << '\n';
			o << "  " <<   DIPDIR - 90.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2) << -Y << " translate " << '\n';
		}

		if (i.corrOFFSET == "SINISTRAL") {

			o
			<< "  " << setw (6) << fixed << setprecision (2) << X
			<< " "	<< setw (5) << fixed << setprecision (2) << Y << " translate "
					<< - DIPDIR + 90.0 << " rotate" << '\n';
			o << "  newpath sinistralarrow" << '\n';
			o << "  " <<   DIPDIR - 90.0 << " rotate "
					<< setw (6) << fixed << setprecision (2) << -X << " "
					<< setw (5) << fixed << setprecision (2) << -Y << " translate" << '\n';
		}

		if (i.OFFSET == "NONE") {

			o
			<< "  " << setprecision (5) << X
			<< " "	<< setprecision (5) << Y << " translate "
					<<  - DIPDIR << " rotate" << '\n';
			o << "  newpath nonearrow" << '\n';
			o << "  " <<  DIPDIR << " rotate "
					<< setprecision (5) << -X << " "
					<< setprecision (5) << -Y << " translate" << '\n';
		}
	}


	if (label) {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << '\n';
		o
		<< "  " << fixed << setprecision (3) << X << " " << fixed << setprecision (3) << Y  <<" translate" << '\n';

		o
		<< "  0.0 0.0 moveto 10.0 10.0 lineto stroke" << '\n';
		o
		<< "  10.0 10.0 moveto ("	<< i.ID << ") show"	<< '\n';

		o
		<< "  " << fixed << setprecision (3) << - X << " " << fixed << setprecision (3) << - Y  <<" translate" << '\n';
	}

	o << "0 0 0 setrgbcolor" << '\n';
}

void PS_getstereonet (ofstream& o, INPSET inset, CENTER center) {

	o << "newpath " << center.X << " " << center.Y << " " << center.radius << " " << " 0 360 arc stroke" << '\n';

	o << "newpath " << center.X + center.radius	<< " " << center.Y << " moveto " << center.X + center.radius + 10.0	<< " " << center.Y << " lineto stroke" << '\n';
	o << "newpath " << center.X << " " << center.Y + center.radius << " moveto " << center.X << " " << center.Y + center.radius + 10.0 << " lineto stroke" << '\n';
	o << "newpath " << center.X - center.radius << " " << center.Y << " moveto " << center.X - center.radius - 10.0 << " " << center.Y << " lineto stroke" << '\n';
	o << "newpath " << center.X << " " << center.Y - center.radius << " moveto " << center.X << " " << center.Y - center.radius - 10.0 << " lineto stroke" << '\n';

	o << "newpath " << center.X << " " << center.Y - 10.0  << " moveto " << center.X << " " << center.Y + 10.0  << " lineto 0.7 setlinewidth stroke" << '\n';
	o << "newpath " << center.X - 10.0 << " " << center.Y  << " moveto " << center.X + 10.0 << " " << center.Y  << " lineto 0.7 setlinewidth stroke" << '\n';

	o << "/ArialNarrow-Bold findfont 20 scalefont setfont" << '\n';
	o << center.X - 6.0 << " " << center.Y + center.radius + 15.0 << " moveto (N) show" << '\n';

	o << "/ArialNarrow-Italic findfont 10 scalefont setfont" << '\n';

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Schmidt-net, lower hemisphere) show" << '\n' ;
	}

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Schmidt-net, upper hemisphere) show" << '\n' ;
	}

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Wulff-net, lower hemisphere) show" << '\n' ;
	}

	if ((inset.plottype=="S") && (inset.hemisphere=="L")) {

		o << center.X - center.radius << " " << center.Y - center.radius << " moveto (Wulff-net, upper hemisphere) show" << '\n' ;
	}
}

void PS_datanumber_averagebedding (GDB i, ofstream& o, INPSET inset, PAPER P, CENTER center, size_t datanumber) {

	o << '\n';

	bool HAS_BEDDING = (i.avS0d.DIPDIR < 900.0 && i.avS0d.DIP < 900.0);

	if (HAS_BEDDING) {

		if (inset.plot == "H") 	PS_polepoint (i, o, inset, center, false, "AV");
		else 					PS_plane     (i, o, inset, center, false, "AV");

		if (inset.datarule == "R") i.avS0d.DIPDIR = german_to_right_hand_rule (i.avS0d.DIPDIR);
	}

	o << '\n';

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << '\n';
	o << "  0 0 0 setrgbcolor" << '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O1X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O1Y + P.R + 1.8 * P.B
	<< " moveto (Data number: "<< datanumber << ") show" << '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O1X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O1Y + P.R + 1.3 * P.B
	<< " moveto (Average bedding: " << flush;

	if (!HAS_BEDDING) {

		o << "no bedding measured";
	}
	else {
		o
		<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
		<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP ;
	}

	if (i.avS0offset == "OVERTURNED") o << ", overturned" ;

	o << ") show" << '\n' << '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.8 * P.B
	<< " moveto (Data number: "<< datanumber << ") show" << '\n';

	if (inset.tilting == "B") {

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding " << flush;


		if (!HAS_BEDDING) {

			o << "no bedding measured";
		}
		else {
			o
			<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
			<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP ;
		}


		if (i.avS0offset == "OVERTURNED") o << ", overturned" ;

		o << ") show" << '\n' << '\n';
	}

	else if (inset.tilting == "P") {

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << '\n' << '\n';
	}

	else {

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding " << flush;

		if (!HAS_BEDDING) {

			o << "- no bedding measured";
		}
		else {
			o
			<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
			<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP ;
		}

		if (i.avS0offset == "OVERTURNED") o << ", overturned" ;

		o << ") show" << '\n' << '\n';

		o
		<< "  " << fixed << setprecision (3) << P.O2X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O2Y + P.R + 0.8 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << '\n' << '\n';
	}

	o
	<< "  " << fixed << setprecision (3) << P.O3X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O3Y + P.R + 1.8 * P.B
	<< " moveto (Rose plot for measured data) show" << '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O3X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O3Y + P.R + 1.3 * P.B
	<< " moveto (Average bedding: " << flush;

	if (!HAS_BEDDING) {

		o << "no bedding measured";
	}
	else {
		o
		<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
		<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP ;
	}

	if (i.avS0offset == "OVERTURNED") o << ", overturned" ;
	o << ") show" << '\n' << '\n';

	o
	<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
	<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.8 * P.B
	<< " moveto (Rose plot for dip corrected data) show" << '\n';

	if (inset.tilting == "B") {

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding " << flush;

		if (!HAS_BEDDING) {

			o << "- no bedding measured";
		}
		else {
			o
			<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
			<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP ;
		}

		if (i.avS0offset == "OVERTURNED") o << ", overturned" ;

		o << ") show" << '\n' << '\n';
	}

	else if (inset.tilting == "P") {

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << '\n' << '\n';
	}

	else {

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 1.3 * P.B
		<< " moveto (Corrected by average bedding " << flush;

		if (!HAS_BEDDING) {

			o << "- no bedding measured";
		}
		else {
			o
			<< setfill ('0') << setw (3) << fixed << setprecision (0) << i.avS0d.DIPDIR << "/"
			<< setfill ('0') << setw (2) << fixed << setprecision (0) << i.avS0d.DIP ;
		}

		if (i.avS0offset == "OVERTURNED") o << ", overturned" ;

		o << ") show" << '\n' << '\n';

		o
		<< "  " << fixed << setprecision (3) << P.O4X - P.R - 0.2 * P.B
		<< "  " << fixed << setprecision (3) << P.O4Y + P.R + 0.8 * P.B
		<< " moveto (Corrected by palaeo north directions ) show" << '\n' << '\n';

		o << "%% This is thee end of the sg2ps ddemo data post script file header" << '\n';
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
			<< " "  << fixed << setprecision (2) << - degree << " rotate " << '\n';

	if (inset.grayscale == "Y") {

		if (c_plane)	o << "1.0 setlinewidth 0.00 0.00 0.00 setrgbcolor " << '\n';
		else 			o << "0.7 setlinewidth 0.50 0.50 0.50 setrgbcolor " << '\n';
	}
	else {

		if (c_plane)	o << "1.0 setlinewidth 0.00 0.00 1.00 setrgbcolor " << '\n';
		else 			o << "0.7 setlinewidth 0.00 0.50 0.00 setrgbcolor " << '\n';
	}

	o << " newpath 0.0 0.0 moveto "
	  << fixed << setprecision (2) << + (radius * SIN(step_angle)) << " "
	  << fixed << setprecision (2) <<    radius * COS(step_angle)  << " lineto" << '\n';
	o << "  0.0 0.0 " << fixed << setprecision (3) << radius
	  << " "     << fixed << setprecision (3) << angle << " 90.0 arc" << '\n';
	o << "  0.0 0.0 moveto 0.0 " << fixed << setprecision (3) << radius << " lineto" << '\n';
	o << "  closepath " << '\n';

	if (inset.grayscale == "Y") {
		if (c_plane)	o << " stroke" << '\n';
		else 			o << " gsave 0.80 0.80 0.80 setrgbcolor fill grestore stroke" << '\n';
	}
	else {
		if (c_plane)	o << " stroke" << '\n';
		else			o << " gsave 0.00 1.00 0.00 setrgbcolor fill grestore stroke" << '\n';
	}


	o << " "  << fixed << setprecision (2) << degree    << " rotate "
	  << "  " << fixed << setprecision (2) << -center.X
	  << " "  << fixed << setprecision (2) << -center.Y << " translate" << '\n' << '\n';
}

void PS_draw_rose_circle_horizontal (ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent) {

	double i, step;

	if (inset.rosetype == "S") {

		percent.PLN_NUM = percent.PLN_NUM / 2.0;
		percent.LIN_NUM = percent.LIN_NUM / 2.0;
	}

	if (percent.PLN_NUM < 0.01) return;

	if (percent.PLN_NUM < 0.2) 	i = 0.05;
	else if ((percent.PLN_NUM >= 0.2) && (percent.PLN_NUM < 0.5)) i = 0.1;
	else i = 0.2;

	step = i;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << '\n';
	o << fixed << setprecision (3) << center.X << " "
	  << fixed << setprecision (3) << center.Y << " translate" << '\n';
	if (inset.grayscale == "Y") o << "  0.5 setlinewidth 0.00 0.00 0.00 setrgbcolor" << '\n';
	else 						o << "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << '\n';
	o << "  [3 3] 0 setdash" << '\n';

	do {

		o << "   newpath 0.0 0.0 "
				<< fixed << setprecision (3) << center.radius *  i / percent.PLN_NUM
				<< " 0.0 360.0 arc stroke" << '\n';
		o << "  -5.0 "
				<< fixed << setprecision (3) << (center.radius * i / percent.PLN_NUM) + 2.0 << " moveto"
				<< " (" << fixed << setprecision (0) << i * 100.0 << " %) show" << '\n';

		i =  i + step;

	} while (i < percent.PLN_NUM);

	o << "  stroke [   ] 0 setdash" << '\n';
	o << "  " << fixed << setprecision (3) << - center.X << " "
	          << fixed << setprecision (3) << - center.Y << " translate" << '\n';
}

void PS_draw_rose_circle_vertical (ofstream& o, INPSET inset, CENTER center, ROSENUMBER percent) {

	double i, step;

	if (percent.PLN_NUM < 0.01) return;

	if (percent.PLN_NUM < 0.2) 	i = 0.05;
	else if ((percent.PLN_NUM >= 0.2) && (percent.PLN_NUM < 0.5)) i = 0.1;
	else i = 0.2;

	step = i;

	o << "/ArialNarrow-Bold findfont 8 scalefont setfont" << '\n';
	o
	<< fixed << setprecision (3) << center.X << " "
	<< fixed << setprecision (3) << center.Y << " translate" << '\n';
	if (inset.grayscale == "Y") o << "  0.5 setlinewidth 0.00 0.00 0.00 setrgbcolor" << '\n';
	else 						o << "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << '\n';
	o << "  [3 3] 0 setdash" << '\n';

	do {

		o << "   newpath 0.0 0.0 "
		  << fixed << setprecision (3) << center.radius *  i / percent.PLN_NUM
		  << " 270.0 360.0 arc stroke" << '\n';
		o << "  " << fixed << setprecision (3) << (center.radius * i / percent.PLN_NUM) - 4.0 << " 5.0 moveto"
		  << " (" << fixed << setprecision (0) << i * 100.0 << " %) show" << '\n';

		i =  i + step;

	} while (i < percent.PLN_NUM);

	o << "  stroke [   ] 0 setdash" << '\n';
	o << "  " << fixed << setprecision (3) << - center.X << " "
	          << fixed << setprecision (3) << - center.Y << " translate" << '\n';
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

	VCTR d = compute_d_for_SC (i);

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

	if (inset.idealmovement == "N") return;
	if (inset.plot == "H") return;

	do {

		if (vectorlength (inGDB.at(i).SHEAR_S) > 10e-5) {

			shear_vector = unitvector (inGDB.at(i).SHEAR_S);

			inGDB.at(i).N = flip_N_vector (shear_vector);

			if (inset.grayscale == "Y") inGDB.at(i).PSCOLOR = "0.0 0.0 0.0";
			else 						inGDB.at(i).PSCOLOR = "0.0 0.0 0.5";

			PS_polepoint (inGDB.at(i), o, inset, center, false, "");
		}

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

	o << "newpath" << '\n';

	o << "  " << fixed << setprecision (3);

	o << "  " << P.A << " " << (P.Y / 2.0) - 0.15 * P.A  << " moveto "
	<< P.X - (10.0 * P.A) << " " << (P.Y / 2.0) - 0.15 * P.A << " lineto" << '\n';

	o
	<< "  " << P.S2X << " " << P.S2Y  << " moveto "
	<< P.S6X << " " << P.S6Y << " lineto" << '\n';

	o
	<< "  "<< fixed << setprecision (3) << P.S5X
	<< " " << fixed << setprecision (3) << P.S5Y << " moveto "
		   << fixed << setprecision (3) << P.S1X
	<< " " << fixed << setprecision (3) << P.S1Y << " lineto "
		   << fixed << setprecision (3) << P.S4X
	<< " " << fixed << setprecision (3) << P.S4Y << " lineto "
		   << fixed << setprecision (3) << P.S3X
	<< " " << fixed << setprecision (3) << P.S3Y << " lineto " << '\n';

	o << "  3.0 setlinewidth" << '\n';
	o << "  0.8 0.8 0.8 setrgbcolor stroke" << '\n' << '\n';

	o
	<< "  "<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.25
	<< " " << fixed << setprecision (3) << P.S1Y - 0.3 * P.A << " moveto "
	<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.25
	<< " " << fixed << setprecision (3) << P.S4Y + 0.3 * P.A << " lineto" << '\n';

	o
	<< "  "<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.5
	<< " " << fixed << setprecision (3) << P.S1Y - 0.3 * P.A << " moveto "
	<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.5
	<< " " << fixed << setprecision (3) << P.S4Y + 0.3 * P.A << " lineto" << '\n';

	o
	<< "  "<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.75
	<< " " << fixed << setprecision (3) << P.S1Y - 0.3 * P.A << " moveto "
	<< fixed << setprecision (3) << P.S1X + (P.S2X - P.S1X) * 0.75
	<< " " << fixed << setprecision (3) << P.S4Y + 0.3 * P.A << " lineto" << '\n';

	o << "  1.0 setlinewidth" << '\n';
	o << "  0.8 0.8 0.8 setrgbcolor stroke" << '\n' << '\n';

	o << "/ArialNarrow-Bold findfont 12 scalefont setfont" 					<< '\n';
	o << "  " << P.X - (6.1 * P.A) << " " << P.Y / 2.0 + 3.5 * P.A  << " moveto " << '\n';
	o << "  (SYMBOLS) 0 0 0 setrgbcolor show"  << '\n';
}

void PS_SYMBOL_draw_plane (double X, double Y, ofstream& o, INPSET inset, PAPER P, string type, string group) {

	string color = "0.00 0.00 0.00";
	string dash = "   ";

	o <<  "   newpath " << P.S1X + 1.2 * X << " " << P.S1Y - 3.5 * Y << " " << 3.0 * P.A << " 80 110 arc" << '\n';

	if (inset.grayscale == "Y") {

		if 		(group == "X") color = "0.20 0.20 0.20";
		else if (group == "A") color = "0.20 0.20 0.20";
		else if (group == "B") color = "0.20 0.20 0.20";
		else if (group == "C") color = "0.40 0.40 0.40";
		else if (group == "D") color = "0.40 0.40 0.40";
		else if (group == "E") color = "0.40 0.40 0.40";
		else if (group == "F") color = "0.40 0.40 0.40";
		else if (group == "G") color = "0.60 0.60 0.60";
		else if (group == "H") color = "0.60 0.60 0.60";
		else if (group == "I") color = "0.60 0.60 0.60";
		else {}

		if (group == "X") dash = "   ";
		else if (group == "A") dash = "3 3";
		else if (group == "B") dash = "6 6";
		else if (group == "C") dash = "   ";
		else if (group == "D") dash = "3 3";
		else if (group == "E") dash = "3 6";
		else if (group == "F") dash = "6 6";
		else if (group == "G") dash = "   ";
		else if (group == "H") dash = "3 3";
		else if (group == "I") dash = "6 6";
		else {}
	}
	else {
		if (group == "A") color = "0.00 0.00 1.00";
		else if (group == "B") color = "1.00 0.00 0.67";
		else if (group == "C") color = "1.00 0.00 0.00";
		else if (group == "D") color = "1.00 0.50 0.00";
		else if (group == "E") color = "1.00 1.00 0.00";
		else if (group == "F") color = "0.00 1.00 0.00";
		else if (group == "G") color = "0.67 0.00 0.67";
		else if (group == "H") color = "0.50 1.00 1.00";
		else if (group == "I") color = "0.50 0.50 0.50";
		else {}
	}

	if 			(type == "AV") 		{

		if (inset.grayscale == "Y") o << "  0.20 0.20 0.20 setrgbcolor 2 setlinewidth  " << '\n';
		else						o << "  1.00 0.00 0.00 setrgbcolor 1.5 setlinewidth  " << '\n';
	}
	else if		(type == "AV_O") 	{
		if (inset.grayscale == "Y") o << "  0.20 0.20 0.20 setrgbcolor 2 setlinewidth  " << '\n';
		else 						o << "  1.00 0.00 0.00 setrgbcolor 1.5 setlinewidth  " << '\n';
	}
	else if 	(type == "FOLD") 	{
		if (inset.grayscale == "Y")	o << "  0.60 0.60 0.60 setrgbcolor 2 setlinewidth  " << '\n';
		else 						o << "  0.00 0.00 1.00 setrgbcolor 1 setlinewidth  " << '\n';
	}

	else 							o << "  " << color << " setrgbcolor 1 setlinewidth " << '\n';

	if 			(type == "AV") 			o << "  [   ] 0 setdash stroke  [   ] 0 setdash " << '\n';
	else if 	(type == "AV_O") 		o << "  [6 6] 0 setdash stroke  [   ] 0 setdash " << '\n';
	else if 	(type == "O") 			o << "  [6 6] 0 setdash stroke  [   ] 0 setdash " << '\n';
	else if 	(type == "FOLD") 		o << "  [6 6] 0 setdash stroke  [   ] 0 setdash " << '\n';
	else if 	(type == "C") 			o << "  [3 3] 0 setdash stroke  [   ] 0 setdash " << '\n';
	else if (inset.grayscale == "Y")	o << "  [" << dash << "] 0 setdash stroke  [   ] 0 setdash " << '\n';
	else 								o << "stroke" << '\n' << '\n';
}

void PS_SYMBOLS_STRIAE (ofstream& o, INPSET inset, PAPER P) {

	double arrow_X, arrow_Y;
	double step = 0.30;

	arrow_X = P.S1X + 0.6 * P.A;
	arrow_Y = P.S1Y - 3.355 * P.A;

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.50 * P.A) << " moveto (Fault plane) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.10 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.40 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.22 * P.A) << " moveto (Overturned) 0 0 0 setrgbcolor show"  << '\n';

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.00 * P.A, o, inset, P, "", "");

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, inset, P, "AV", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.60 * P.A, o, inset, P, "AV_O", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.80 * P.A, o, inset, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.97 * P.A, o, inset, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.14 * P.A, o, inset, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.31 * P.A, o, inset, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.48 * P.A, o, inset, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 2.65 * P.A, o, inset, P, "", "");


	o << "  " << arrow_X << " " << arrow_Y << "  translate 20 rotate" << '\n';
	o << "  newpath normalarrow" << '\n';
	o << "  -20 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y << " moveto (Normal fault) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate 160 rotate" << '\n';
	o << "  newpath normalarrow" << '\n';
	o << "  -160 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Reverse fault) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate 30 rotate" << '\n';
	o << "  newpath dextralarrow" << '\n';
	o << "  -30 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Dextral fault) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate 60 rotate" << '\n';
	o << "  newpath sinistralarrow" << '\n';
	o << "  -60 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Sinistral fault) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (step * P.A);

	o << "  " << arrow_X << " " << arrow_Y << "  translate -20 rotate" << '\n';
	o << "  newpath nonearrow" << '\n';
	o << "  20 rotate " << - arrow_X << " " << - arrow_Y << "  translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Unknown fault) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (step * P.A);


	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 5.0 << " " << arrow_Y << " moveto (Ideal movement) 0 0 0 setrgbcolor show"  << '\n';

	if (inset.grayscale == "Y") o << "  0.0 0.0 0.0 setrgbcolor 0.7 setlinewidth" << '\n';
	else 						o << "  0.0 0.0 0.5 setrgbcolor 0.7 setlinewidth" << '\n';

	o
	<< "  newpath " << fixed << setprecision (3) << arrow_X
	<< " " << fixed << setprecision (3) << arrow_Y + 12.5
	<< " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << '\n';
	o << "  0 0 0 setrgbcolor" << '\n';
}

void PS_SYMBOLS_SC (ofstream& o, INPSET inset, PAPER P) {

	double arrow_X, arrow_Y;

	arrow_X = P.S1X + 0.6 * P.A;
	arrow_Y = P.S1Y - 3.355 * P.A;

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.5 * P.A) << " moveto (Schistosity plane) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << '\n';

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.1 * P.A) << " moveto (Cleavage plane) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  - 5.0 << " " << arrow_Y + (0.92 * P.A) << " moveto (with movement direction) 0 0 0 setrgbcolor show"  << '\n';

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.00 * P.A, o, inset, P, "", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, inset, P, "C", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.80 * P.A, o, inset, P, "AV", "");

	o << " 0.00 0.00 0.00 setrgbcolor " << '\n';
	o << "  " << arrow_X  << " " << arrow_Y + 1.4 * P.A << "  translate 20 rotate" << '\n';
	o << "  newpath normalarrow" << '\n';
	o << "  -20 rotate " << - arrow_X << " " << - (arrow_Y + 1.4 * P.A) << "  translate " << '\n';

	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (0.4 * P.A);
}

void PS_SYMBOLS_PLANE (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P) {

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

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.50 * P.A) << " moveto (" << type << ") 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.10 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.40 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.22 * P.A) << " moveto (Overturned) 0 0 0 setrgbcolor show"  << '\n';

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.00 * P.A, o, inset, P, "", "");

	if (type == "BEDDING") {

		PS_SYMBOL_draw_plane (1.0 * P.A, 1.20 * P.A, o, inset, P, "O", "");
	}

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, inset, P, "AV", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.60 * P.A, o, inset, P, "AV_O", "");

	if (inGDB.at(0).DATATYPE == "FOLDSURFACE") {

		PS_SYMBOL_draw_plane (1.0 * P.A, 1.80 * P.A, o, inset, P, "FOLD", "");
		o  << "  " << arrow_X + 5.0 << " " << arrow_Y - (0.3 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	}
}

void PS_SYMBOLS_LINEATION (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P) {

	double arrow_X = P.S1X + 0.6 * P.A;
	double arrow_Y = P.S1Y - 3.355 * P.A;

	string type = "";

	if 		(inGDB.at(0).DATATYPE == "BOUDAIN") 		type = "Boudain axis";
	else if (inGDB.at(0).DATATYPE == "FOLDAXIS") 		type = "Fold axis";
	else if (inGDB.at(0).DATATYPE == "KINK") 			type = "Kink axis";
	else if	(inGDB.at(0).DATATYPE == "LINEATION") 		type = "Lineation";
	else 												type = "User defined";

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.50 * P.A) << " moveto (" << type << ") 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.10 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.40 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (0.22 * P.A) << " moveto (Overturned) 0 0 0 setrgbcolor show"  << '\n';

	PS_SYMBOL_draw_plane (1.0 * P.A, 1.40 * P.A, o, inset, P, "AV", "");
	PS_SYMBOL_draw_plane (1.0 * P.A, 1.60 * P.A, o, inset, P, "AV_O", "");

	o
	<< "  " << " 0.0 0.0 0.0 setrgbcolor 1.5 setlinewidth newpath "
	<< fixed << setprecision (5) << arrow_X - 0.35 * P.A << " "
	<< fixed << setprecision (5) << arrow_Y + 2.8 * P.A << " 1.5 0 360 arc 0 fill stroke" << '\n';
}

void PS_SYMBOLS_HOEPPNER (ofstream& o, PAPER P) {

	double arrow_X = P.S1X + 0.6 * P.A;
	double arrow_Y = P.S1Y - 3.355 * P.A;

	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.5 * P.A) << " moveto (Plane pole point) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.32 * P.A) << " moveto (Group 'X') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (1.1 * P.A) << " moveto (Average bedding) 0 0 0 setrgbcolor show"  << '\n';

	o << "  " << " 0.0 0.0 0.0 setrgbcolor 1.5 setlinewidth newpath "
	  << fixed << setprecision (5) << arrow_X - 0.35 * P.A << " "
	  << fixed << setprecision (5) << arrow_Y + 2.8 * P.A << " 1.5 0 360 arc 0 stroke" << '\n';

	o << "  " << " 1.0 1.0 1.0 setrgbcolor 0.7 setlinewidth  newpath "
	  << fixed << setprecision (5) << arrow_X - 0.35 * P.A << " "
	  << fixed << setprecision (5) << arrow_Y + 2.8 * P.A << " 0.7 0 360 arc 0 stroke" << '\n';

	o << "  1.0 0.0 0.0 setrgbcolor 0.7 setlinewidth newpath" << '\n';

	o << fixed << setprecision (3) << arrow_X - 0.35 * P.A<< " "
	  << fixed << setprecision (3) << arrow_Y + 1.40 * P.A
	  << " 1.5 0 360 arc 0 gsave 1.00 1.00 1.00 setrgbcolor fill grestore stroke" << '\n';
	o << "   0 0 0 setrgbcolor" << '\n';

	o << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << "  translate 20 rotate" << '\n';
	o << "  newpath normalarrow" << '\n';
	o << "  -20 rotate " << - (arrow_X - 0.3 * P.A)<< " " << - arrow_Y << "  translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X  + 4.0 << " " << arrow_Y << " moveto (Hanging wall) 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 4.0 << " " << arrow_Y - 0.18 * P.A << " moveto (movement direction) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (0.4 * P.A);
}

void PS_SYMBOLS_GROUPS (ofstream& o, INPSET inset, PAPER P) {

	double arrow_X = P.S1X + 0.6 * P.A + 6.0 * P.A;;
	double arrow_Y = P.S1Y - 3.355 * P.A;

	PS_SYMBOL_draw_plane (6.8 * P.A, 1.00 * P.A, o, inset, P, "", "A");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.20 * P.A, o, inset, P, "", "B");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.40 * P.A, o, inset, P, "", "C");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.60 * P.A, o, inset, P, "", "D");
	PS_SYMBOL_draw_plane (6.8 * P.A, 1.80 * P.A, o, inset, P, "", "E");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.00 * P.A, o, inset, P, "", "F");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.20 * P.A, o, inset, P, "", "G");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.40 * P.A, o, inset, P, "", "H");
	PS_SYMBOL_draw_plane (6.8 * P.A, 2.60 * P.A, o, inset, P, "", "I");

	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (2.5 * P.A) << " moveto (Group 'A') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (1.8 * P.A) << " moveto (Group 'B') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (1.1 * P.A) << " moveto (Group 'C') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y + (0.4 * P.A) << " moveto (Group 'D') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (0.3 * P.A) << " moveto (Group 'E') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (1.0 * P.A) << " moveto (Group 'F') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (1.7 * P.A) << " moveto (Group 'G') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (2.4 * P.A) << " moveto (Group 'H') 0 0 0 setrgbcolor show"  << '\n';
	o  << "  " << arrow_X  + 1.3 * P.A << " " << arrow_Y - (3.1 * P.A) << " moveto (Group 'I') 0 0 0 setrgbcolor show"  << '\n';
}

void PS_SYMBOLS_INVERSION (ofstream& o, PAPER P) {

	double arrow_X = P.S1X + (0.6 * P.A) + 2.8 * P.A;
	double arrow_Y = P.S1Y - (3.355 * P.A) + 2.5 * P.A - 0.4 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate " << '\n';
	o << "   newpath s1_axis" << '\n';
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << " moveto (S1 axis) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << '\n';
	o << "   newpath s2_axis" << '\n';
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << " moveto (S2 axis) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << '\n';
	o << "   newpath s3_axis" << '\n';
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.3 * P.A << " " << arrow_Y << " moveto (S3 axis) 0 0 0 setrgbcolor show"  << '\n';

	arrow_Y = arrow_Y - (0.2 * P.A) - 0.7 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate 90 rotate 0 10 translate" << '\n';
	o << "   newpath compression_arrow" << '\n';
	o << "   0 -10 translate 180 rotate 0 10 translate " << '\n';
	o << "   newpath compression_arrow" << '\n';
	o << "   0 -10 translate 180 rotate" << '\n';
	o << "   -90 rotate " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.5 * P.A);
	o  << "  " << arrow_X - P.A << " " << arrow_Y << " moveto (Maximum horizontal stress) 0 0 0 setrgbcolor show"  << '\n';

	arrow_Y = arrow_Y - (0.2 * P.A) - 0.7 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate 90 rotate 0 10 translate" << '\n';
	o << "   newpath extension_arrow" << '\n';
	o << "   0 -10 translate 180 rotate 0 10 translate " << '\n';
	o << "   newpath extension_arrow" << '\n';
	o << "   0 -10 translate 180 rotate" << '\n';
	o << "   -90 rotate " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.5 * P.A);
	o  << "  " << arrow_X - P.A << " " << arrow_Y << " moveto (Minimum horizontal stress) 0 0 0 setrgbcolor show"  << '\n';
}

void PS_SYMBOLS_BINGHAM (ofstream& o, PAPER P) {

	double arrow_X = P.S1X + (0.6 * P.A) + 2.8 * P.A;
	double arrow_Y = P.S1Y - (3.355 * P.A) + 2.5 * P.A - 0.4 * P.A;

	o << "  " << arrow_X << " " << arrow_Y << " translate " << '\n';
	o << "   newpath s1_axis" << '\n';
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.8 * P.A << " " << arrow_Y << " moveto (Maximum weight point) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << '\n';
	o << "   newpath s2_axis" << '\n';
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.8 * P.A << " " << arrow_Y << " moveto (Intermediate weight point) 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (0.4 * P.A);

	o << "  " << arrow_X << " " << arrow_Y << " translate " << '\n';
	o << "   newpath s3_axis" << '\n';
	o << "  " << - arrow_X << " " << - arrow_Y << " translate " << '\n';
	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X - 0.8 * P.A << " " << arrow_Y << " moveto (Minimum weight point) 0 0 0 setrgbcolor show"  << '\n';
}

void PS_SYMBOLS_ROSE (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P) {

	double angle = 80.0;
	double radius = 80.0;

	double arrow_X = P.S1X + (0.6 * P.A) + 2.8 * P.A;
	double arrow_Y = P.S1Y - (3.355 * P.A) + 2.5 * P.A - 1.1 * P.A;

	string outtext1 = "Plane dip direction";
	if (inGDB.at(0).DATAGROUP == "LINEATION") outtext1 = "Lineation direction";
	if (inGDB.at(0).DATAGROUP == "SC") outtext1 = "Schistosity dip";

	o
	<< "  " << fixed << setprecision (2) << arrow_X + 1.5 * P.A
	<< " "  << fixed << setprecision (2) << arrow_Y - 1.0 * P.A << " translate -30 rotate" << '\n';

	o << "0.7 setlinewidth 0.50 0.50 0.50 setrgbcolor " << '\n';
	o << " newpath 0.0 0.0 moveto "
	  << fixed << setprecision (2) << radius * SIN(10.0) << " "
	  << fixed << setprecision (2) << radius * COS(10.0) << " lineto" << '\n';
	o
	<< "  0.0 0.0 " << fixed << setprecision (3) << radius
	<< " "     << fixed << setprecision (3) << angle << " 90.0 arc" << '\n';
	o << "  0.0 0.0 moveto 0.0 " << fixed << setprecision (3) << radius << " lineto" << '\n';
	o << "  closepath " << '\n';
	if (inset.grayscale == "Y") o << " gsave 0.80 0.80 0.80 setrgbcolor fill grestore stroke" << '\n';
	else 						o << " gsave 0.00 1.00 0.00 setrgbcolor fill grestore stroke" << '\n';
	o
	<< "  30 rotate "
	<< fixed << setprecision (2) << - (arrow_X + 1.5 * P.A) << " "
	<< fixed << setprecision (2) << - (arrow_Y - 1.0 * P.A) << " translate" << '\n';

	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 1.6 * P.A << " " << arrow_Y - (2.0 * 0.7 * P.A) << " moveto (" << outtext1 << ") 0 0 0 setrgbcolor show"  << '\n';
	arrow_Y = arrow_Y - (0.4 * P.A);

	if ((inGDB.at(0).DATAGROUP == "LINEATION") || (inGDB.at(0).DATAGROUP == "PLANE")) return;

	string outtext2 = "Lineation dip direction";
	if (inGDB.at(0).DATAGROUP == "SC") outtext2 = "Cleavage dip";

	arrow_Y = arrow_Y - (3.0 * 0.7 * P.A);

	o
	<< "  " << fixed << setprecision (2) << arrow_X + 1.5 * P.A
	<< " "  << fixed << setprecision (2) << arrow_Y - 1.0 * P.A << " translate -30 rotate" << '\n';
	if (inset.grayscale == "Y") o << "1.0 setlinewidth 0.00 0.00 0.00 setrgbcolor " << '\n';
	else o << "1.0 setlinewidth 0.00 0.00 1.00 setrgbcolor " << '\n';

	o << " newpath 0.0 0.0 moveto "
			<< fixed << setprecision (2) << radius * SIN(10.0) << " "
			<< fixed << setprecision (2) << radius * COS(10.0) << " lineto" << '\n';
	o
	<< "  0.0 0.0 " << fixed << setprecision (3) << radius
	<< " "     << fixed << setprecision (3) << angle << " 90.0 arc" << '\n';

	o << "  0.0 0.0 moveto 0.0 " << fixed << setprecision (3) << radius << " lineto" << '\n';
	o << "  closepath stroke " << '\n';

	o
	<< "  30 rotate "
	<< fixed << setprecision (2) << - (arrow_X + 1.5 * P.A) << " "
	<< fixed << setprecision (2) << - (arrow_Y - 1.0 * P.A) << " translate" << '\n';

	arrow_Y = arrow_Y - (0.3 * P.A);
	o  << "  " << arrow_X + 1.5 * P.A << " " << arrow_Y - (2.0 * 0.7 * P.A)<< " moveto (" << outtext2 << ") 0 0 0 setrgbcolor show"  << '\n';
}

void PS_SYMBOLS_LABEL (ofstream& o, INPSET inset, PAPER P) {

	double arrow_X = P.S1X + 0.3 * P.A;
	double arrow_Y = P.S1Y - 1.55 * P.A;

	if (inset.labeling == "Y") {

		o
		<< "  /ArialNarrow-Italic findfont 6 scalefont setfont "
		<< "  0.5 setlinewidth 0.50 0.50 0.50 setrgbcolor" << '\n';
		o
		<< "  " << fixed << setprecision (3) << arrow_X
		<< " "  << fixed << setprecision (3) << arrow_Y  <<" translate 90 rotate" << '\n';
		o
		<< "  28.0 2.0 moveto 35.0 2.0 lineto stroke" << '\n';
		o
		<< "  0.0 0.0 moveto (OXAM_001) show"	<< '\n';
		o
		<< "  -90 rotate " << fixed << setprecision (3) << - arrow_X
		<< " "  << fixed << setprecision (3) << - arrow_Y  <<" translate" << '\n';

		arrow_X = P.S1X + 0.6 * P.A;
		arrow_Y = P.S1Y - 3.355 * P.A;

		o  << "  " << arrow_X  + 5.0 << " " << arrow_Y + (2.14 * P.A) << " moveto (with labeling) 0 0 0 setrgbcolor show"  << '\n';
	}

	else return;
}

void PS_SYMBOLS (vector <GDB> inGDB, ofstream& o, INPSET inset, PAPER P) {

	PS_SYMBOLS_border (o, P);
	PS_SYMBOLS_LABEL (o, inset, P);

	o << "/ArialNarrow findfont 8 scalefont setfont" 					<< '\n';

	o << "  " << P.S1X + 5.2 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
	o << "  (ROSE PLOT) 0 0 0 setrgbcolor show"  << '\n';
	PS_SYMBOLS_ROSE (inGDB, o, inset, P);

	o << "  " << P.S1X + 7.4 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
	o << "  (GROUPS) 0 0 0 setrgbcolor show"  << '\n';
	PS_SYMBOLS_GROUPS (o, inset, P);

	if (inGDB.at(0).DATAGROUP == "PLANE") {

		o << "  " << P.S1X + 0.8 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
		o << "  (PLANES) 0 0 0 setrgbcolor show"  << '\n';
		PS_SYMBOLS_PLANE (inGDB, o, inset, P);

		o << "  " << P.S1X + 2.5 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
		o << "  (BINGHAM STATISTICS) 0 0 0 setrgbcolor show"  << '\n';

		if ((inset.fracture == "B") && (inGDB.at(0).DATATYPE == "FRACTURE")) PS_SYMBOLS_BINGHAM (o, P);

		return;
	}

	if (inGDB.at(0).DATAGROUP == "STRIAE") {

		o << "  " << P.S1X + 0.1 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
		o << "  (FAULT AND STRIAE DATA) 0 0 0 setrgbcolor show"  << '\n';

		if (inset.plot == "A")	PS_SYMBOLS_STRIAE (o, inset, P);
		else PS_SYMBOLS_HOEPPNER (o, P);

		o << "  " << P.S1X + 2.6 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
		o << "  (STRESS INVERSION) 0 0 0 setrgbcolor show"  << '\n';
		if (inset.inversion != "N") PS_SYMBOLS_INVERSION (o, P);

		return;
	}

	if (inGDB.at(0).DATAGROUP == "SC") {

		o << "  " << P.S1X + 0.1 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
		o << "  (SCHISTOSITY, CLEAVEGE) 0 0 0 setrgbcolor show"  << '\n';
		PS_SYMBOLS_SC (o, inset, P);

		o << "  " << P.S1X + 2.6 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
		o << "  (STRESS INVERSION) 0 0 0 setrgbcolor show"  << '\n';

		return;
	}

	if (inGDB.at(0).DATAGROUP == "LINEATION") {

		o << "  " << P.S1X + 0.6 * P.A << " " << P.S1Y - 0.3 * P.A  << " moveto " << '\n';
		o << "  (LINEATION) 0 0 0 setrgbcolor show"  << '\n';
		PS_SYMBOLS_LINEATION (inGDB, o, inset, P);

		return;
	}
}

void ps_plot_densities (DENSITY dens, size_t radius, ofstream& o, INPSET inset, CENTER center) {

	const size_t min_dipdir = dens.direction.DIPDIR - radius;
	const size_t max_dipdir = dens.direction.DIPDIR + radius;

	const size_t min_dip = dens.direction.DIP - radius;
	const size_t max_dip = dens.direction.DIP + radius;

	DIPDIR_DIP act_DD;

	act_DD.DIPDIR = min_dipdir;
	act_DD.DIP = min_dip;

	XY act_xy = stereonet_coordinate_from_DIPDIR_DIP (act_DD, center, inset);

	o << fixed << setprecision (3) << '\n';

	o << "newpath" << '\n';

	o << "  "  <<  act_xy.X << " " << act_xy.Y << " moveto" << '\n';

	for (act_DD.DIPDIR = min_dipdir + 1; act_DD.DIPDIR < max_dipdir + 1; act_DD.DIPDIR++) {

		act_xy = stereonet_coordinate_from_DIPDIR_DIP (act_DD, center, inset);
		o << "  "  <<  act_xy.X << " " << act_xy.Y << " lineto" << '\n';
	}

	for (act_DD.DIP = min_dip + 1; act_DD.DIP < max_dip + 1; act_DD.DIP++) {

		act_xy = stereonet_coordinate_from_DIPDIR_DIP (act_DD, center, inset);
		o << "  "  <<  act_xy.X << " " << act_xy.Y << " lineto" << '\n';
	}

	for (act_DD.DIPDIR = max_dipdir - 1; act_DD.DIPDIR > min_dipdir - 1; act_DD.DIPDIR--) {

		act_xy = stereonet_coordinate_from_DIPDIR_DIP (act_DD, center, inset);
		o << "  "  <<  act_xy.X << " " << act_xy.Y << " lineto" << '\n';
	}

	for (act_DD.DIP = min_dip - 1; act_DD.DIP > max_dip - 1; act_DD.DIP--) {

		act_xy = stereonet_coordinate_from_DIPDIR_DIP (act_DD, center, inset);
		o << "  "  <<  act_xy.X << " " << act_xy.Y << " lineto" << '\n';
	}

	o << "  closepath" << '\n';


	o << fixed << setprecision (2) << '\n';

	o
	<< "  "
	<< (density_color_from_percentage(dens.percentage)).X << " "
	<< (density_color_from_percentage(dens.percentage)).Y << " "
	<< (density_color_from_percentage(dens.percentage)).Z << " "
	<< "setrgbcolor fill stroke" << '\n' << '\n';
}
