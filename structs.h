
// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef STRUCTS_HPP_
#define STRUCTS_HPP_

#include <limits>
#include <string>

using namespace std;

inline double NaN() { return numeric_limits<double>::quiet_NaN(); }

struct VCTR {

	VCTR() { X=Y=Z=NaN(); }

	double X, Y, Z;
};

struct DIPDIR_DIP {

	DIPDIR_DIP() { DIPDIR=DIP=NaN(); }

	double DIPDIR, DIP;
};

struct STRESSFIELD {

	STRESSFIELD() { stressratio=delvaux_str=shmax=shmin=NaN(); }

	VCTR EIGENVECTOR1, EIGENVECTOR2, EIGENVECTOR3, EIGENVALUE;
	DIPDIR_DIP S_1, S_2, S_3;
	double stressratio, delvaux_str, shmax, shmin;
	string regime, delvaux_rgm;
};

struct STRESSTENSOR {

	STRESSTENSOR() { _11=_12=_13=_22=_23=_33=NaN(); }

	double _11, _12, _13, _22, _23, _33;
};

struct GDB  {

	GDB() { LOCX=LOCY=DIPDIR=DIP=LDIR=LDIP=PALEON=LPITCH=DEPTH=PITCHANGLE=MISFIT=lambda=ANG=RUP=AV_MISF=NaN();
			iID=numeric_limits<int>::min();
			//UPWARD=false UP=false;
		   }

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
	size_t iID;
	string DATAGROUP;
	string PSCOLOR;
	string DASHED;
	double DEPTH;

	DIPDIR_DIP corr, corrL;
	DIPDIR_DIP avS0d, avd;

	string avS0offset;

	double LPITCH;
	string LPITCHSENSE;
	double PITCHANGLE;

	VCTR N, NC,	D, DC, S, SC;
	VCTR T;

	VCTR ptnP, ptnT, ptnN;
	VCTR avD, avS0D, avS0N;

	double MISFIT;

	string LINEATION;

	string OFFSET;

	double lambda;

	VCTR SHEAR_S, NORMAL_S, UPSILON;

	double ANG, RUP;
	double AV_MISF;

	VCTR fold_great_circle_N;

	vector <STRESSFIELD> SFV;
	vector <STRESSTENSOR> STV;
};

struct CORRECTSTRIAE {

	CORRECTSTRIAE() { X=Y=Z=MISFIT=NaN(); }

	double X, Y, Z, MISFIT;
};

/*
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
	tiltrgfsep,
	well_ps;};

*/
struct PAPER {

	double X, Y;
	double A, B, C, D, R;
	double O1X, O1Y, O2X, O2Y, O3X, O3Y, O4X, O4Y, O5X, O5Y, O6X, O6Y, O7X, O7Y, O8X, O8Y;
	double S1X, S1Y, S2X, S2Y, S3X, S3Y, S4X, S4Y, S5X, S5Y, S6X, S6Y;
};

struct CENTER {

	double X, Y, radius;
};

struct LOC_X_Y_FRM {

	LOC_X_Y_FRM() { X=Y=NaN(); }

	string LOC;
	double X, Y;
	string FORMATION;
};

struct ROSENUMBER {

	ROSENUMBER() { PLN_NUM=LIN_NUM=NaN(); }

	double PLN_NUM, LIN_NUM;
};

struct CENTR_VECT {

	CENTR_VECT() { U=V=W=X=Y=Z=NaN(); }

	double U, V, W, X, Y, Z;
};


struct sort_jacobi {

	sort_jacobi() { ID=numeric_limits<int>::min(); eigenvalue=NaN(); }

	int ID;
	double eigenvalue;
};

struct ANG_PRM {

	ANG_PRM() { a=b=c=d=e=f=g=h=i=j=k=l=m=n=p=q=u=v=w=NaN(); }

	double a, b, c, d, e, f, g, h, i, j, k, l, m, n, p, q, u, v, w;
};

struct DENSITY {

	DIPDIR_DIP direction;
	double percentage;
};

struct XY {

	double X;
	double Y;
};

struct RUP_table {

	size_t clusternumber;
	double k;
	double v;
	double delta;
	double C;
};

struct HISTOGRAM {

	double BIN_MIN;
	int COUNT;
	string GC;
};

struct VALLEY {

	size_t BIN_ID;
	double BIN_CENTER;
	string DIR;
};

struct line_RUP_ANG {

	double L_STR;
	double L_END;
	size_t COUNT;
	string GC;
};

struct BRDR {

	double border;
	string ID;
	size_t COUNT;
};


struct TRIANGLE {

	size_t GROUP;
	VCTR A;
	VCTR B;
	VCTR C;
	size_t COUNT;
};

struct GRID_CENTER {

	VCTR CENTER;
	size_t COUNT;
};

/*struct GRID_CENTER_S {

	VCTR CENTER;
	size_t COUNT;
};*/

struct LINE {

	VCTR A;
	VCTR B;
};

struct BRUTEFORCE_RESULT {

	VCTR NRM;
	double ANG;
	double PHI;
	double MISFIT;
};

struct BEZIER {

	VCTR A;
	VCTR B;
	VCTR C;
	VCTR D;
};
/*
struct WELL_AVERAGE {

	double DEPTH;
	VCTR D;
	DIPDIR_DIP DD;
	double MISFIT;
};
*/

/*
struct WELL_DERIVATE {

	double DEPTH;
	double DERIV;
};
*/

struct WELL_FREQUENCY {

	double DEPTH;
	double FREQ;

	double DERIV_DEPTH;
	double DERIV;
};

struct WELL_INTERVAL {

	double MIN;
	double MAX;

	size_t SIZE;

	double DEPTH;

	VCTR INT_AV_D;
	DIPDIR_DIP INT_AV_DD;

	double INT_AV_DD_STDEV;
	double INT_AV_D_STDEV;

	double D_DERIV;
	double DD_DERIV;
};

struct TRJ {

	double DEPTH;
	double X_AZIMUTH;
	double Y_PLUNGE;
};

struct PEAK_TO_PLOT {

	double DEPTH;
	double VALUE;
	double COUNT;
};

#endif
