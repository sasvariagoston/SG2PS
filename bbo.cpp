/*
 * bbo.cpp


 *
 *  Created on: 6 Jul 2016
 *      Author: sasvariagoston_admin
 */

#include "assertions.hpp"
#include "bbo.hpp"
#include "common.h"

double return_sigma_r_tau_teta (const GDB in, const double AZIMUTH, const string MODE) {

	const bool AZ = AZIMUTH;
	const bool RAD = MODE == "RAD";
	const bool TET = MODE == "TETA";
	const bool TAU = MODE == "TAU";
	ASSERT_EXACTLY_ONE_TRUE (AZ, RAD, TET, TAU);

	const double SH = in.SH;
	const double Sh = in.Sh;
	const double R = in.N_rad;
	const double r = in.BBO_rad;
	const double dP = in.delta_p;
	ASSERT_FINITE (SH, Sh, R, r, dP);

	const double var_RRrr = (R*R)/(r*r);
	const double var_RRRRrrrr = (R*R*R*R)/(r*r*r*r);

	const double A = ((SH+Sh)/2);
	const double B = 1 + var_RRrr;
	const double C = dP * var_RRrr;
	const double D = 1 + var_RRrr;
	ASSERT_FINITE (A, B, C, D);

	double res;

	if (RAD) {

		res = (A*B) + (A * (1 - 4*var_RRrr + 3*var_RRRRrrrr) * COS (2*AZ)) + C;
	}
	else if (TET) {

		res = (A*D) - (A * (1 + 3*var_RRRRrrrr) * COS (2*AZ)) - C;
	}
	else {

		res = -A * (1 + 2*var_RRrr - 3*var_RRRRrrrr) * SIN (2*AZ);
	}
	ASSERT_FINITE (res);
	return res;
}

double return_SH (const GDB in, const BBO B, bool POSITIVE, bool MAX) {

	ASSERT_FINITE (B.a1, B.a2, B.b1, B.b2, B.c1, B.c2, B.d1, B.d2, B.e1, B.f1);

	const double TETA = in.teta;
	const double TAU = in.cohesion;
	const double dP = in.delta_p;

	ASSERT_FINITE (TETA, TAU, dP);

	const double a1 = B.a1;
	const double b1 = B.b1;
	const double c1 = B.c1;
	const double d1 = B.d1;

	double a2 = B.a2;
	double b2 = B.b2;
	double c2 = B.c2;
	double d2 = B.d2;
	double e1 = B.e1;
	double f1 = B.f1;

	if (! POSITIVE) {

		a2 = -a2;
		b2 = -b2;
		c2 = -c2;
		d2 = -d2;
		e1 = -e1;
		f1 = -f1;
	}
	double pA;

	if (! MAX) 	pA = ((a1+a2) * (TAU + (f1*dP))) - ((c1+c2) * (TAU - (e1*dP)));
	else 		pA = ((d1+d2) * (TAU - (e1*dP))) - ((b1+b2) * (TAU - (f1*dP)));

	const double pB = ((a1+a2) * (d1+d2) - (b1+b2) * (c1+c2));

	ASSERT_FINITE (pA, pB);

	return 2 * (pA/pB);
}

GDB return_shmax_shmin (const GDB in) {

	GDB OUT = in;

	BBO B;

	const double MU = OUT.mu;
	const double R = OUT.N_rad;
	const double r = OUT.BBO_rad;

	const double var_2_cos_2_teta = 2 * COS (2 * OUT.teta);
	const double var_sqrt_1_mu_mu = sqrt (1 + (MU * MU));
	const double var_RRrr = (R*R)/(r*r);
	const double var_RRRRrrrr = (R*R*R*R)/(r*r*r*r);

	B.a1 = - MU * (1 - var_2_cos_2_teta);
	B.b1 = - MU * (1 + var_2_cos_2_teta);

	B.a2 = var_sqrt_1_mu_mu * (1 - var_2_cos_2_teta);
	B.b2 = var_sqrt_1_mu_mu * (1 + var_2_cos_2_teta);

	B.c1 = - MU * (1 + (2 * var_RRrr));
	B.d1 = - MU * (1 + (2 * var_RRrr));

	B.c2 = var_sqrt_1_mu_mu * (  1     - var_RRrr + 3 * var_RRRRrrrr);
	B.d2 = var_sqrt_1_mu_mu * (- 1 + 3 * var_RRrr - 3 * var_RRRRrrrr);

	B.e1 = - var_sqrt_1_mu_mu;
	B.f1 = - var_sqrt_1_mu_mu * var_RRrr;

	ASSERT_FINITE (B.a1, B.a2, B.b1, B.b2, B.c1, B.c2, B.d1, B.d2, B.e1, B.f1);

	const double SH_1 = return_SH (in, B, true, true);
	const double SH_2 = return_SH (in, B, false, true);

	const double Sh_1 = return_SH (in, B, true, false);
	const double Sh_2 = return_SH (in, B, false, false);

	const bool SH1_GE_0 = SH_1 > 0;
	const bool SH2_GE_0 = SH_2 > 0;
	const bool Sh1_GE_0 = Sh_1 > 0;
	const bool Sh2_GE_0 = Sh_2 > 0;

	const bool SH1_GE_Sh1 = SH_1 >= Sh_1;
	const bool SH2_GE_Sh2 = SH_2 >= Sh_2;

	const bool STRESS_1_OK = (SH1_GE_0 && Sh1_GE_0 && SH1_GE_Sh1);
	const bool STRESS_2_OK = (SH2_GE_0 && Sh2_GE_0 && SH2_GE_Sh2);

	ASSERT_EXACTLY_ONE_TRUE (STRESS_1_OK, STRESS_2_OK);

	if (STRESS_1_OK) {

		OUT.SH = SH_1;
		OUT.Sh = Sh_1;
	}
	else {

		OUT.SH = SH_2;
		OUT.Sh = Sh_2;
	}
	return OUT;
}


//double N_rad, BBO_rad, mu, delta_p, cohesion, sigma_r, sigma_tete, tau;
