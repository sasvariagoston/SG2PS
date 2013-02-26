// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <cmath>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "ps.h"
#include "structs.h"


XY stereonet_coordinate_from_DIPDIR_DIP (DIPDIR_DIP in, CENTER center, INPSET inset) {

	XY out;

	VCTR D = DXDYDZ_from_dipdir_dip(in);

	if (D.Z > 0.0) D.Z = - D.Z;

	if (inset.plottype == "S") {

		out.X = (D.X / sqrt (1.00 - D.Z)) * center.radius;
		out.Y = (D.Y / sqrt (1.00 - D.Z)) * center.radius;
	}

	else {

		out.X = (D.X / (1.00 - D.Z)) * center.radius;
		out.Y = (D.Y / (1.00 - D.Z)) * center.radius;
	}

	if (inset.hemisphere == "U") {

		out.X = - out.X;
		out.Y = - out.Y;
	}

	else {} // that's OK

	out.X = out.X + center.X;
	out.Y = out.Y + center.Y;

	return out;
}

VCTR density_color_from_percentage (double percentage) {

	//ASSERT_GE(1.00, percentage);

	VCTR minimum = declare_vector(1.00, 1.00, 1.00);
	VCTR maximum = declare_vector(1.00, 0.00, 1.00);

	return declare_vector(
	minimum.X * (1.0 - percentage) + maximum.X * percentage,
	minimum.Y * (1.0 - percentage) + maximum.Y * percentage,
	minimum.Z * (1.0 - percentage) + maximum.Z * percentage);
}

DENSITY density_in_cell (vector <GDB> in, size_t search_dipdir, size_t search_dip, size_t radius) {

	//ASSERT_GE(radius, search_dip);

	DENSITY out;

	size_t counter = 0;

	double min_dipdir = search_dipdir - radius;
	double max_dipdir = search_dipdir + radius;
	double min_dip = search_dip - radius;
	double max_dip = search_dip + radius;

	for (size_t i = 0; i < in.size(); i++) {

		double DD = in.at(i).corr.DIPDIR;
		double D = in.at(i).corr.DIP;

		if ((DD >= min_dipdir && DD < max_dipdir) && (D >= min_dip && D < max_dip)) counter++;
		else {};
	}

	out.direction.DIPDIR = search_dipdir;
	out.direction.DIP = search_dip;
	out.percentage = counter;

	return out;
}

vector <DENSITY> normalize_density (vector <DENSITY> in) {

	size_t max = 0;

	for (size_t i = 0; i < in.size(); i++ ) {

		if (in.at(i).percentage > max) max = in.at(i).percentage;
	}


	for (size_t i = 0; i < in.size(); i++ ) {

		in.at(i).percentage = in.at(i).percentage / max;
	}

	return in;
}

vector <DENSITY> generate_density_vector (vector <GDB> in, size_t radius) {

	vector <DENSITY> out;

	size_t dipdir_step = 2 * radius;
	size_t    dip_step = 2 * radius;

	for (size_t search_dip = radius; search_dip < 90; search_dip += dip_step) {

		for (size_t search_dipdir = radius; search_dipdir < 360; search_dipdir += dipdir_step) {

			/*if 		((search_dip >   0) && (search_dip < 40)) 	dipdir_step =  2 * radius;
			else if ((search_dip >= 40) && (search_dip < 60)) 	dipdir_step =  3 * radius;
			else if ((search_dip >= 60) && (search_dip < 80)) 	dipdir_step =  5 * radius;
			else 												dipdir_step = 10 * radius;*/

			DENSITY buffer = density_in_cell (in, search_dipdir, search_dip, radius);

			//cout << search_dipdir << " / " << search_dip << " / " << buffer.percentage << endl;

			if (buffer.percentage > 10e-4) out.push_back(buffer);


		}
	}

	out = normalize_density (out);

	return out;
}

void plot_densities (vector <GDB> inGDB, vector <GDB> tiltinGDB, ofstream& o, INPSET inset, CENTER center, PAPER P) {

	vector <DENSITY> dens;

	size_t radius = 5;

	center.X = P.O1X;
	center.Y = P.O1Y;
	dens = generate_density_vector (inGDB, radius);

	cout << fixed << setprecision (3) << endl;
	cout << dens.size() << endl;





	for (size_t i = 0; i < dens.size(); i++) {


		cout << dens.at(i).percentage / dens.size() << endl;

		ps_plot_densities (dens.at(i), radius, o, inset, center, P);
	}


	center.X = P.O2X;
	center.Y = P.O2Y;
	dens = generate_density_vector (tiltinGDB, radius);

	for (size_t i = 0; i < dens.size(); i++) {

		//ps_plot_densities (dens.at(i), radius, o, inset, center, P);
	}
}
