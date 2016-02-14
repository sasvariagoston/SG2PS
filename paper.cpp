// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include "paper.hpp"

namespace {

PAPER PPR;

}

void INIT_PAPER (const bool WELL) {

	PAPER P;

	if (WELL) {

		P.Y = mm_to_point (420);
		P.X = P.Y * 0.707143;
	}
	else {

		P.X = mm_to_point (420);
		P.Y = P.X * 0.707143;
	}

	P.A = P.X * 0.033670;
	P.B = P.A / 2.0;
	P.C = P.B / 2.0;
	P.D = P.C / 10.0122;

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

	PPR = P;
}

double mm_to_point (const size_t i) {

	return i * 0.03937 * 72.0;
}

PAPER RETURN_PAPER () {

	return PPR;
}
