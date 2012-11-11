// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef STRUCTS_HPP_
#define STRUCTS_HPP_

#include <string>

using namespace std;

struct VCTR {

	double X, Y, Z;};

struct DIPDIR_DIP {

	double DIPDIR, DIP;};

struct INPSET  {

	string datarule;
	string plot;
	string plottype;
	string hemisphere;
	string tilting;
	string group;
	string clusternumber;
	string labeling;
	string inversion;
	string virt_striae;
	string idealmovement;
	double angle;
	string anglest;
	string fracture;
	string linewidthst;
	double linewidth;
	string rosetype;
	string rosebinning;
	string filename;
};

struct GDB  {

	string ID;
	string GC;
	string COLOR;
	string LOC;
	double LOCX;
	double LOCY;
	string FORMATION;
	string DATATYPE;
	double DIPDIR;
	double DIP;
	double LDIR;
	double LDIP;
	double PALEON;
	string COMMENT;
	int iID;
	string DATAGROUP;
	string PSCOLOR;


	DIPDIR_DIP corr, corrL;
	DIPDIR_DIP ptnPd, ptnTd, ptnNd;
	DIPDIR_DIP avS0d, avd;

	string avS0offset;

	double LPITCH;
	string LPITCHSENSE;
	double PITCHANGLE;

	VCTR N, NC, D, DC, S, SC, SV;
	VCTR ptnP, ptnT, ptnN;
	VCTR avD, avS0D, avS0N, rotavD;

	double MISFIT;

	string LINEATION;
	bool UPWARD;

	string OFFSET;
	string corrOFFSET;

	bool UP;

	double lambda;

	VCTR SHEAR_S, NORMAL_S, UPSILON;

	double ANG, RUP;
};

struct CORRECTSTRIAE {

	double X, Y, Z, MISFIT;
};


struct PFN {

	string
	projectname,
	projectfolder,
	datetime,
	original,
	completed,
	average,
	rgfsep,
	pssep,
	tiltrgfsep;};

struct PAPER {

	double X, Y;
	double A, B, C, R;
	double O1X, O1Y, O2X, O2Y, O3X, O3Y, O4X, O4Y, O5X, O5Y, O6X, O6Y, O7X, O7Y, O8X, O8Y;
	double S1X, S1Y, S2X, S2Y, S3X, S3Y, S4X, S4Y, S5X, S5Y, S6X, S6Y;
};

struct CENTER {

	double X, Y, radius;
};

struct LOC_X_Y_FRM {

	string LOC;
	double X, Y;
	string FORMATION;
};


struct STRESSTENSOR {

	double _11, _12, _13, _22, _23, _33;
};

struct ROSENUMBER {

	double PLN_NUM, LIN_NUM;
};

struct STRESSFIELD {

	VCTR EIGENVECTOR1, EIGENVECTOR2, EIGENVECTOR3, EIGENVALUE;
	DIPDIR_DIP S_1, S_2, S_3;
	double stressratio, delvaux_str, shmax, shmin;
	string regime, delvaux_rgm;
};

struct CENTR_VECT {

	double U, V, W, X, Y, Z;
};


struct sort_jacobi {

	int ID;
	double eigenvalue;
};

struct ANG_PRM {

	double a, b, c, d, e, f, g, h, i, j, k, l, m, n, p, q, u, v, w;
};

#endif
