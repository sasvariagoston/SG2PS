// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <cmath>
#include <algorithm>

#include "common.h"
#include "rgf.h"

using namespace std;

string capslock (string input) {

	for(unsigned int i = 0; i < input.length(); i++)	input.at(i) = toupper(input.at(i));
	return input;
}

const string int_to_string (int i) {

	ostringstream os;
	os << i << flush;
	return os.str();
}

const string char_to_string (char i) {

	ostringstream os;
	os << i << flush;
	return os.str();
}

double SIGNUM (double in) {

	if (in < 0.0) return -1.0;
	else if (in == 0.0) return  0.0;
	else return  1.0;
}

double SIN (double in) {

	in = (in * 3.1415926535) / 180.0;

	return sin(in);
}

double COS (double in) {

	in = (in * 3.1415926535) / 180.0;

	return cos(in);
}

double ASIN (double in) {

	if (in >= 1.0) 	in =   (1.0 - 10e-8);
	if (in <= -1.0) in = - (1.0 - 10e-8);

	in = asin(in);

	return (in * 180.0) / 3.1415926535;
}

double ACOS (double in) {

	if (in >= 1.0)  in =   (1.0 - 10e-8);
	if (in <= -1.0) in = - (1.0 - 10e-8);

	in = acos(in);

	return (in * 180.0) / 3.1415926535;
}

double ATAN (double in) {

	in = atan(in);

	return (in * 180.0) / 3.1415926535;
}

double rounding (double in) {

	double fract_part, int_part;

	if (in == 0.0) return 0;
	fract_part = modf (in , &int_part);

	if (in > 0.0) {

		if (fract_part >= 0.5) return ceil (in);
		else return floor (in);
	}

	else {

		if (fract_part >= 0.5) return floor (in);
		else return ceil (in);
	}
}

double mm_to_point (int i) {

	return i * 0.03937 * 72.0;
}

VCTR crossproduct (VCTR in1, VCTR in2) {

	VCTR out;

	out.X =	  (in1.Y * in2.Z) - (in2.Y * in1.Z);
	out.Y = - (in1.X * in2.Z) + (in2.X * in1.Z);
	out.Z =	  (in1.X * in2.Y) - (in2.X * in1.Y);

	return out;
}

double dotproduct (VCTR in1, VCTR in2, bool normalisation) {

	double l_in1 = sqrt((in1.X * in1.X) + (in1.Y * in1.Y) + (in1.Z * in1.Z));
	double l_in2 = sqrt((in2.X * in2.X) + (in2.Y * in2.Y) + (in2.Z * in2.Z));

	double out = in1.X * in2.X + in1.Y * in2.Y + in1.Z * in2.Z;

	if (normalisation) out = out / (l_in1 * l_in2);

	return out;
}

double det_3 (vector <vector <double> > in) {

	return

			(in.at(0).at(0) * in.at(1).at(1) * in.at(2).at(2)) +
			(in.at(0).at(1) * in.at(1).at(2) * in.at(2).at(0)) +
			(in.at(0).at(2) * in.at(1).at(0) * in.at(2).at(1)) -
			(in.at(0).at(2) * in.at(1).at(1) * in.at(2).at(0)) -
			(in.at(0).at(1) * in.at(1).at(0) * in.at(2).at(2)) -
			(in.at(0).at(0) * in.at(1).at(2) * in.at(2).at(1));
}

vector <vector <double> >  declare_3x3_matrix (double a, double b, double c, double d, double e, double f, double g, double h, double i) {

	vector < vector < double > > o;
	vector <double> buffer;

	buffer.push_back(a);
	buffer.push_back(b);
	buffer.push_back(c);

	o.push_back(buffer);
	buffer.clear();

	buffer.push_back(d);
	buffer.push_back(e);
	buffer.push_back(f);

	o.push_back(buffer);
	buffer.clear();

	buffer.push_back(g);
	buffer.push_back(h);
	buffer.push_back(i);

	o.push_back(buffer);

	return o;
}

vector <vector <double> > init_matrix (const size_t dimension) {

	vector <vector <double> > o;
	vector <double> buffer;

	size_t m = 0;
	size_t n = 0;

	do {
		do {

			buffer.push_back(0.0);
			n++;

		} while (n < dimension);

		o.push_back(buffer);
		buffer.clear();

		n = 0;
		m++;

	} while (m < dimension);

	return o;
}

vector <vector <double> > init_matrix (const size_t i, const size_t j) {

	vector <vector <double> > o;
	vector <double> buffer;

	size_t m = 0;
	size_t n = 0;

	do {
		do {

			buffer.push_back(0.0);
			n++;

		} while (n < j);

		o.push_back(buffer);
		buffer.clear();

		n = 0;
		m++;

	} while (m < i);

	return o;
}

vector <vector <double> > identity_matrix (vector <vector <double> > in) {

	size_t m = 0;
	size_t n = 0;

	do {
		do {

			if (m == n) in.at(m).at(n) = 1.0;
			else 		in.at(m).at(n) = 0.0;

			m++;

		} while (m < in.at(0).size());

		m = 0;
		n++;

	} while (n < in.size());

	return in;
}

vector <double> init_vector (int dimension) {

	vector <double> buffer;
	int i = 0;

	do {

		buffer.push_back(0.0);
		i++;

	} while (i < dimension);

	return buffer;
}

size_t m_from_max_element (size_t max_element, size_t n) {

	int j = 0;

	while (max_element > 0)  {

		max_element = max_element - n;
		j++;
	}

	j = j - 1;

	return j;
}

size_t search_max_off_diagonal_element_in_mtrx (vector <vector <double> > in) {

	size_t m = 0;
	size_t n = 0;
	size_t  out = 0;
	double max_element_size = 0.0;

	do {
		do {
			if ((fabs(in.at(m).at(n)) > max_element_size) && (m != n)) {

				max_element_size = fabs(in.at(m).at(n));
				out = (in.at(0).size() * m) + (n + 1);
			}

			n++;

		} while (n < in.at(0).size());

		n = 0;
		m++;

	} while (m < in.size());

	if (max_element_size < 10e-8) out = 9999;

	return out;
}

double teta (vector <vector <double> > in, size_t m, size_t n) {

	double teta = 2.0 * in.at(m).at(n) / (in.at(n).at(n) - in.at(m).at(m));

	return atan(teta) / 2.0;
}

vector <vector <double> > init_rotation_mtrx (double teta, size_t m, size_t n, size_t dimension) {

	vector <vector <double> > o = init_matrix (dimension);

	size_t i = 0;
	size_t j = 0;

	do {
		do {

			if ((i == m) && (j == n)) 		o.at(i).at(j) = - sin (teta);

			else if ((i == n) && (j == m)) 	o.at(i).at(j) =   sin (teta);

			else if ((i == m) && (j == m)) 	o.at(i).at(j) =   cos (teta);

			else if ((i == n) && (j == n)) 	o.at(i).at(j) =   cos (teta);

			else if (i == j) 				o.at(i).at(j) =   1.0;

			else 							o.at(i).at(j) =   0.0;

			i++;

		} while (i < dimension);

		i = 0;
		j++;

	} while (j < dimension);

	return o;
}

vector <vector <double> > outer_product (vector <double> in) {

	vector <vector <double> > o = init_matrix (in.size());
	size_t m = 0;
	size_t n = 0;

	do {
		do {

			o.at(m).at(n) = in.at(m) * in.at(n);
			n++;

		} while (n < in.size());

		n = 0;
		m++;

	} while (m < in.size());

	return o;
}

vector <vector <double> > transpose (vector <vector <double> > in) {

	vector <vector <double> > o = in;

	size_t m = 0;
	size_t n = 0;

	const size_t j = in.size();
	const size_t i = in.at(0).size();

	o = init_matrix (i, j);

	do {
		do {

			o.at(m).at(n) = in.at(n).at(m);
			n++;

		} while (n < j);

		n = 0;
		m++;

	} while (m < i);

	return o;
}

vector <vector <double> > mult_mtrx (vector <vector <double> > in1, vector <vector <double> > in2) {

	size_t m = in1.size();
	size_t n = in1.at(0).size();
	size_t M = 0;
	size_t N = 0;

	size_t l = in2.at(0).size();
	size_t L = 0;

	vector <vector <double> > out = init_matrix (m, l);

	do {
		do {
			do {

				out.at(M).at(L) = out.at(M).at(L) + in1.at(M).at(N) * in2 .at(N).at(L);

				N++;

			} while (N < n);

			N = 0;
			M++;

		} while (M < m);

		M = 0;
		L++;

	} while (L < l);

	return out;
}

vector <vector <double> > add_mtrx (vector <vector <double> > in1, vector <vector <double> > in2) {

	vector <vector <double> > out = init_matrix (in1.size(), in1.at(0).size());
	size_t m = 0;
	size_t n = 0;

	do {
		do {

			out.at(m).at(n) = in1.at(m).at(n) + in2.at(m).at(n);
			n++;

		} while (n < in1.at(0).size());

		n = 0;
		m++;

	} while (m < in1.size());

	return out;
}


int return_second_eigenvalue (vector <vector< double > > in) {

	vector <sort_jacobi> buffer;
	sort_jacobi buf;

	size_t j = 0;

	do {

		buf.ID = j;
		buf.eigenvalue = in.at(j).at(j);
		buffer.push_back(buf);

		j++;

	} while (j < in.size());

	sort(buffer.begin(), buffer.end(), byeigenvalue);

	return buffer.at(1).ID;
}

int return_first_eigenvalue (vector <vector< double > > in) {

	sort_jacobi buf;
	vector <sort_jacobi> buffer;

	size_t j = 0;

	do {

		buf.ID = j;
		buf.eigenvalue = in.at(j).at(j);
		buffer.push_back(buf);

		j++;

	} while (j < in.size());

	sort(buffer.begin(), buffer.end(), byeigenvalue);

	return buffer.at(0).ID;
}

vector <vector <double> > jacobi (vector <vector <double> > in) {

	vector <vector <double> > out;

	vector <vector <double> > A = in;
	vector <vector <double> > R1 = init_matrix (in.size());
	vector <vector <double> > R2 = init_matrix (in.size());
	vector <vector <double> > D  = init_matrix (in.size());

	size_t max_element_in_mtrx = 10;

	double teta_angle = 0.0;

	size_t m = 0;
	size_t n = 0;
	size_t i = 0;

	do {

		max_element_in_mtrx = search_max_off_diagonal_element_in_mtrx (A);

		if (max_element_in_mtrx > 9000) break;

		m = m_from_max_element (max_element_in_mtrx, A.at(0).size());

		n = max_element_in_mtrx - (m * A.at(0).size()) - 1;

		teta_angle = teta (A, m, n);

		R1 = init_rotation_mtrx (teta_angle, m, n, in.size());

		R2 = transpose (R1);

		A = mult_mtrx (R1, A);
		A = mult_mtrx (A, R2);

		if (i == 0) D = R2;
		else D = mult_mtrx (D, R2);

		i++;

	} while (i < 40);

	i = 0;

	do {

		out.push_back(A.at(i));
		i++;

	} 	while (i < A.size());

	i = 0;

	D = transpose (D);

	do {

		out.push_back(D.at(i));
		i++;

	} 	while (i < D.size());

	return out;
}

vector <vector <double> > gaussian_elimination (vector <vector <double> > in) {

	size_t m = 0;
	size_t n = 0;

	double value;

	vector <vector <double> > o = init_matrix (in.size());
	o = identity_matrix (o);

	do {

		o  = row_division_diagonal (o,  n, in.at(n).at(n));

		in = row_division_diagonal (in, n, in.at(n).at(n));

		do {

			if (n != m) {

				value = in.at(m).at(n);

				o  = row_addition (o,  m, n, value);
				in = row_addition (in, m, n, value);
			}

			m++;

		} while (m < in.size());

		m = 0;
		n++;

	} while (n < in.at(0).size());

	return o;
}

vector <vector <double> > LU_decomposition (vector <vector <double> > in) {

	vector <vector <double> > U = in;
	vector <vector <double> > L = init_matrix (in.size());

	size_t m = 1;
	size_t n = 0;

	double value = 0;

	L = identity_matrix (L);

	do {

		do {

			value = U.at(m).at(n) / U.at(n).at(n);

			U  = row_addition_LU (U,  m, n, value);

			L.at(m).at(n) = value;

			m++;

		} while (m < in.size());

		n++;
		m = n + 1;

	} while ((n < in.at(0).size()) && (m < in.size()));

	n = 0;

	do {

		U.push_back(L.at(n));

		n++;

	} while (n < L.size());

	return U;
}

vector <vector <double> > compute_Z (vector <vector <double> > L, vector <vector <double> > c) {

	vector <vector <double> > out = init_matrix (L.size(), 1);

	size_t i = 0;
	size_t j = 0;

	do {

		out.at(i).at(0) = c.at(i).at(0);

		do {

			if (!(i == j)) out.at(i).at(0) = out.at(i).at(0) - (L.at(i).at(j) * out.at(j).at(0));

			j++;

		} while (j <= i);

		j = 0;
		i++;

	} while (i < c.size());

	return out;
}

vector <vector <double> > compute_X (vector <vector <double> > U, vector <vector <double> > Z) {

	vector <vector <double> > out = init_matrix (U.size(), 1);

	size_t i = U.size();
	size_t j = U.size();

	do {

		out.at(i-1).at(0) = Z.at(i-1).at(0);

		do {

			if (i == j) 	out.at(i-1).at(0) = out.at(i-1).at(0) /  U.at(i-1).at(j-1);
			else 			out.at(i-1).at(0) = out.at(i-1).at(0) - (U.at(i-1).at(j-1) * out.at(j-1).at(0));

			j--;

		} while (j > i-1);

		j = U.size();
		i--;

	} while (i > 0);

	return out;
}

vector <vector <double> > row_division_diagonal (vector <vector <double> > in,  size_t rownumber, double value) {

	size_t i = 0;

	do {

		in.at(rownumber).at(i) = in.at(rownumber).at(i) / value;

		i++;

	} while (i < in.at(0).size());

	return in;
}

vector <vector <double> > row_addition (vector <vector <double> > in, size_t actual_row_number, size_t zero_row_number, double value) {

	size_t i = 0;

	do {

		in.at(actual_row_number).at(i) = in.at(actual_row_number).at(i) - (value * in.at(zero_row_number).at(i));

		i++;

	} while (i < in.size());

	return in;
}


vector <vector <double> > row_addition_LU (vector <vector <double> > in, size_t actual_row_number, size_t zero_row_number, double value) {

	size_t i = 0;

	do {

		in.at(actual_row_number).at(i) = in.at(actual_row_number).at(i) - (value * in.at(zero_row_number).at(i));

		i++;

	} while (i < in.size());

	return in;
}

vector < vector < double > > generate_A (vector < vector < double > > EVEV) {

	vector < vector < double > > A = init_matrix (EVEV.at(0).size());
	size_t i = 0;

	do {

		A.at(i) = EVEV.at(i);
		i++;

	} 	while (i < (EVEV.size() / 2));

	return A;
}

vector < vector < double > > generate_D (vector < vector < double > > EVEV) {

	vector < vector < double > > D = init_matrix (EVEV.at(0).size());
	size_t i = 0;

	do {

		D.at(i) = EVEV.at(i + (EVEV.size() / 2));
		i++;

	} 	while (i < (EVEV.size() / 2));

	return D;
}

bool check_fry_matrix (size_t first_eigenvalue, vector <vector <double> > in_eigenvector) {

	vector <sort_jacobi> buffer;
	sort_jacobi buf;
	size_t j = 0;

	do {

		buf.ID = j;
		buf.eigenvalue = in_eigenvector.at(first_eigenvalue).at(j);
		buffer.push_back(buf);

		j++;

	} while (j < in_eigenvector.size());

	sort(buffer.begin(), buffer.end(), byeigenvalue);

	if (	((buffer.at(0).eigenvalue < 0.005) && (buffer.at(0).eigenvalue > -0.005)) &&
			((buffer.at(1).eigenvalue < 0.005) && (buffer.at(1).eigenvalue > -0.005)) &&
			((buffer.at(2).eigenvalue < 0.005) && (buffer.at(2).eigenvalue > -0.005))

		&&
			((buffer.at(3).eigenvalue > 0.57) && (buffer.at(3).eigenvalue < 0.58)) &&
			((buffer.at(4).eigenvalue > 0.57) && (buffer.at(4).eigenvalue < 0.58)) &&
			((buffer.at(5).eigenvalue > 0.57) && (buffer.at(5).eigenvalue < 0.58))	) return true;

	else return false;
}

bool check_correct_stressfield (STRESSFIELD sf) {

	if ((sf.S_1.DIPDIR > 0.0) && (sf.S_1.DIPDIR < 360.0) &&
		(sf.S_1.DIP > 0.0) && (sf.S_1.DIP < 90.0) &&
		(sf.S_2.DIPDIR > 0.0) && (sf.S_2.DIPDIR < 360.0) &&
		(sf.S_2.DIP > 0.0) && (sf.S_2.DIP < 90.0) &&
		(sf.S_3.DIPDIR > 0.0) && (sf.S_3.DIPDIR < 360.0) &&
		(sf.S_3.DIP > 0.0) && (sf.S_3.DIP < 90.0))  return true;

	else return false;
}

VCTR generarte_stress_colors (double value) {

	double percentage = 0.0;

	VCTR out;

	if ((value > 0.0) && (value <= 0.5)) {

		percentage = (value - 0.0) / 0.5;

		out.X = 0.00 * (1.0 - percentage) + 0.00 * (percentage);
		out.Y = 0.00 * (1.0 - percentage) + 0.00 * (percentage);
		out.Z = 0.00 * (1.0 - percentage) + 1.00 * (percentage);
	}

	else if ((value > 0.5) && (value <= 1.0)) {

		percentage = (value - 0.5) / 0.5;

		out.X = 0.00 * (1.0 - percentage) + 0.00 * (percentage);
		out.Y = 0.00 * (1.0 - percentage) + 1.00 * (percentage);
		out.Z = 1.00 * (1.0 - percentage) + 1.00 * (percentage);
	}

	else if ((value > 1.0) && (value <= 1.5)) {

		percentage = (value - 1.0) / 0.5;

		out.X = 0.00 * (1.0 - percentage) + 0.00 * (percentage);
		out.Y = 1.00 * (1.0 - percentage) + 1.00 * (percentage);
		out.Z = 1.00 * (1.0 - percentage) + 0.00 * (percentage);
	}

	else if ((value > 1.5) && (value <= 2.0)) {

		percentage = (value - 1.5) / 0.5;

		out.X = 0.00 * (1.0 - percentage) + 1.00 * (percentage);
		out.Y = 1.00 * (1.0 - percentage) + 1.00 * (percentage);
		out.Z = 0.00 * (1.0 - percentage) + 0.00 * (percentage);
	}

	else if ((value > 2.0) && (value <= 2.5)) {

		percentage = (value - 2.0) / 0.5;

		out.X = 1.00 * (1.0 - percentage) + 1.00 * (percentage);
		out.Y = 1.00 * (1.0 - percentage) + 0.50 * (percentage);
		out.Z = 0.00 * (1.0 - percentage) + 0.00 * (percentage);
	}

	else {

		percentage = (value - 2.5) / 0.5;

		out.X = 1.00 * (1.0 - percentage) + 1.00 * (percentage);
		out.Y = 0.50 * (1.0 - percentage) + 0.00 * (percentage);
		out.Z = 0.00 * (1.0 - percentage) + 0.00 * (percentage);
	}

	return out;
}

VCTR unitvector (VCTR in) {

	double vectorlength = sqrt(in.X * in.X + in.Y * in.Y + in.Z * in.Z);

	if (vectorlength > 10e-8) {

		in.X = (in.X / vectorlength);
		in.Y = (in.Y / vectorlength);
		in.Z = (in.Z / vectorlength);
	}

	return in;
}

vector < double > unitvector (vector < double > in) {

	size_t j = 0;
	double vectorlength = 0.0;

	do {

		vectorlength = vectorlength + (in.at(j) * in.at(j));
		j++;

	} while (j < in.size());

	vectorlength = sqrt (vectorlength);
	j = 0;

	if (vectorlength < 10e-8) return in;

	else {

		do {

			in.at(j) = in.at(j) / vectorlength;
			j++;

		} while (j < in.size());
	}

	return in;
}


CENTR_VECT unitvector (CENTR_VECT in) {

	double vectorlength = sqrt
			(in.U * in.U + in.V * in.V + in.W * in.W +
			 in.X * in.X + in.Y * in.Y + in.Z * in.Z);

	if (vectorlength > 0.0000000000001) {

		in.U = (in.U / vectorlength);
		in.V = (in.V / vectorlength);
		in.W = (in.W / vectorlength);
		in.X = (in.X / vectorlength);
		in.Y = (in.Y / vectorlength);
		in.Z = (in.Z / vectorlength);
	}

	return in;
}

VCTR declare_vector (double a, double b, double c) {

	VCTR o;

	o.X = a;
	o.Y = b;
	o.Z = c;

	return o;
}


CENTR_VECT declare_vector (double a, double b, double c, double d, double e, double f) {

	CENTR_VECT o;

	o.U = a;
	o.V = b;
	o.W = c;
	o.X = d;
	o.Y = e;
	o.Z = f;

	return o;
}


VCTR flip_D_vector (VCTR in) {

	if (in.Z > 0.0) {

		in.X = - in.X;
		in.Y = - in.Y;
		in.Z = - in.Z;
	}

	return in;
}

VCTR flip_N_vector (VCTR in) {

	if (in.Z < 0.0) {

		in.X = - in.X;
		in.Y = - in.Y;
		in.Z = - in.Z;
	}

	return in;
}

VCTR flip_ptn_vector (VCTR in) {

	if (in.Z < 0.0) in.Z = - in.Z;

	return in;
}

VCTR compute_d_for_SC (GDB i) {

	VCTR temp1;
	VCTR temp2;
	VCTR d;
	VCTR n;

	temp1 = i.NC;
	temp2 = i.N;

	n = crossproduct (temp1, temp2);
	n = unitvector (n);
	n = flip_D_vector (n);

	temp1 = n;
	temp2 = i.NC;

	d = crossproduct (temp1, temp2);
	d = unitvector (d);
	d = flip_D_vector (d);

	return d;
}

VCTR DXDYDZ_from_dipdir_dip (DIPDIR_DIP i) {

	VCTR out;

	out.X = ((SIN (i.DIPDIR)) * (COS(i.DIP)));
	out.Y = ((COS (i.DIPDIR)) * (COS(i.DIP)));
	out.Z = ((                -  SIN(i.DIP)));

	return out;
}

VCTR NXNYNZ_from_dipdir_dip (DIPDIR_DIP i) {

	VCTR out;

	out.X = ((SIN (i.DIPDIR)) * (SIN(i.DIP)));
	out.Y = ((COS (i.DIPDIR)) * (SIN(i.DIP)));
	out.Z = ((                   COS(i.DIP)));

	return out;
}

VCTR inversion_DXDYDZ_from_DXDYDZ (VCTR i) {

	DIPDIR_DIP dd;

	dd = dipdir_dip_from_DXDYDZ (i);

	dd.DIP = dd.DIP - 90.0;

	return DXDYDZ_from_dipdir_dip (dd);
}

STRESSFIELD stressvector_to_DXDYDZ (STRESSFIELD in) {

	STRESSFIELD out = in;

	if (in.EIGENVECTOR1.Z > 0.0) out.EIGENVECTOR1 = flip_D_vector (out.EIGENVECTOR1);

	if (in.EIGENVECTOR2.Z > 0.0) out.EIGENVECTOR2 = flip_D_vector (out.EIGENVECTOR2);

	if (in.EIGENVECTOR3.Z > 0.0) out.EIGENVECTOR3 = flip_D_vector (out.EIGENVECTOR3);

	return out;
}

VCTR DXDYDZ_from_NXNYNZ (VCTR i) {

	VCTR out;

	DIPDIR_DIP temp = dipdir_dip_from_NXNYNZ (i);

	out = DXDYDZ_from_dipdir_dip (temp);

	return out;
}

VCTR NXNYNZ_from_DXDYDZ (VCTR i) {

	VCTR out;

	DIPDIR_DIP temp = dipdir_dip_from_DXDYDZ (i);

	out = NXNYNZ_from_dipdir_dip (temp);

	return out;
}

DIPDIR_DIP dipdir_dip_from_DXDYDZ (VCTR i) {

	DIPDIR_DIP actual;

	i = unitvector (i);

	actual.DIP = fabs(ACOS(i.Z) - 90.0);

	if (i.Y == 0.0) i.Y = 0.00000001;

	actual.DIPDIR = ATAN (i.X / i.Y);

	if ((i.X > 0.0) && (i.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((i.X < 0.0) && (i.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((i.X < 0.0) && (i.Y > 0.0)) actual.DIPDIR = 360.0 + actual.DIPDIR;

	return actual;
}

DIPDIR_DIP dipdir_dip_from_NXNYNZ (VCTR i) {

	i = unitvector (i);

	DIPDIR_DIP actual;

	actual.DIP = ACOS(i.Z);

	if (i.Y == 0.0) i.Y = 0.0000000000001;

	actual.DIPDIR = ATAN (i.X / i.Y);

	if ((i.X > 0.0) && (i.Y > 0.0)) actual.DIPDIR =   0.0 + actual.DIPDIR;
	if ((i.X > 0.0) && (i.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((i.X < 0.0) && (i.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((i.X < 0.0) && (i.Y > 0.0)) actual.DIPDIR = 360.0 + actual.DIPDIR;

	return actual;
}

VCTR ROTATE (VCTR ax, VCTR torotate, double angle) {

	double A = angle;

	VCTR result;
	VCTR A_1, A_2, A_3;

	A_1.X =      COS(A)         + (1.0 -  COS(A)) * ax.X * ax.X;
	A_1.Y = (1.0-COS(A)) * ax.Y * ax.X - (SIN(A))        * ax.Z;
	A_1.Z = (1.0-COS(A)) * ax.Z * ax.X + (SIN(A))        * ax.Y;

	A_2.X = (1.0-COS(A)) * ax.Y * ax.X + (SIN(A))        * ax.Z;
	A_2.Y =      COS(A)         + (1.0 -  COS(A)) * ax.Y * ax.Y;
	A_2.Z = (1.0-COS(A)) * ax.Y * ax.Z - (SIN(A))        * ax.X;

	A_3.X = (1.0-COS(A)) * ax.Z * ax.X - (SIN(A))        * ax.Y;
	A_3.Y = (1.0-COS(A)) * ax.Y * ax.Z + (SIN(A))        * ax.X;
	A_3.Z =      COS(A)         + (1.0 -  COS(A)) * ax.Z * ax.Z;

	result.X = dotproduct (torotate, A_1, false);
	result.Y = dotproduct (torotate, A_2, false);
	result.Z = dotproduct (torotate, A_3, false);

	return unitvector (result);
}

bool existence (string expression, vector<GDB> inGDB) {

	bool presence = false;

	size_t i = 0;

	while (i < inGDB.size()) {

		if (inGDB.at(i).DATATYPE == expression) presence = true;
		i++;
	}

	return presence;
}

bool existence_of_group (int expression, vector <int> whichgroup) {

	bool presence = false;

	size_t i = 0;

	while (i < whichgroup.size()) {

		if (whichgroup.at(i) == expression) presence = true;
		i++;
	}

	return presence;
}

bool existence_of_group_GDB (string expression, vector <GDB> inGDB) {

	bool presence = false;

	size_t i = 0;

	while (i < inGDB.size()) {

		if (inGDB.at(i).GC == expression) presence = true;
		i++;
	}

	return presence;
}

bool existence_of_groupcodes (vector <GDB> inGDB) {

	bool presence = false;

	size_t i = 0;

	do {

		if (!(inGDB.at(i).GC == "X")) return true;

		i++;
	}

	while (i < inGDB.size());

	return presence;
}




vector <double> cubic_solution (double A, double B, double C, double D) {

	vector <double> out;

	double F = (((3.0 * C) / A) - ((B * B) / (A * A))) / 3.0;

	double G = (((2.0 * B * B * B) / (A * A * A)) - ((9.0 * B * C) / (A * A)) + ((27 * D) / A)) / 27.0;

	double H = ((G * G) / 4.0) + ((F * F * F) / 27.0);

	if ((fabs(F) <= 1E-8) && (fabs(G) <= 1E-8) && (fabs(H) <= 1E-8)) {

		double V = -1.0 * pow(D / A , 1.0 / 3.0);

		out.push_back(V);
		out.push_back(V);
		out.push_back(V);
		out.push_back(0.0);
		out.push_back(0.0);
	}

	else if (H > 0.0) {

		double R = - (G / 2.0) + sqrt (H);

		double S = 0.0;

		if (R < 0.0) S = - pow (-R, 1.0 / 3.0);
		else  		 S =   pow ( R, 1.0 / 3.0);

		double T = - (G / 2.0) - sqrt (H);

		double U = 0.0;

		if (T < 0.0) U = - pow (-T, 1.0 / 3.0);
		else 		 U =   pow ( T, 1.0 / 3.0);

		out.push_back(		S + U         - (B / (3.0 * A))			);
		out.push_back(	- ((S + U) / 2.0) - (B / (3.0 * A))			);
		out.push_back(	- ((S + U) / 2.0) - (B / (3.0 * A))			);
		out.push_back(     (S - U) * (sqrt(3.0) / 2.0)				);
		out.push_back(	 - (S - U) * (sqrt(3.0) / 2.0)  			);
	}

	else {

		double I = sqrt(((G * G) / 4.0) - H);

		double J = 0.0;

		if (I < 0.0) J = - pow (-I, 1.0 / 3.0);
		else 		 J =   pow ( I, 1.0 / 3.0);

		double K = acos(-(G / (2.0 * I)));

		double L = -J;

		double M = cos (K / 3.0);

		double N = sqrt(3.0) * sin(K / 3.0);

		double P =  - (B / (3.0 * A));

		out.push_back (2.0 * J * cos (K / 3.0) - (B / (3.0 * A)));
		out.push_back (L * (M + N) + P);
		out.push_back (L * (M - N) + P);
		out.push_back (0.0);
		out.push_back (0.0);
	}

	return out;
}

vector <double>  quartic_solution (double A, double B, double C, double D, double E) {

	vector <double> result;
	vector <double> X;
	double y1, y2, y3, y4;
	double p, q;
	double p_c = 0.0;
	double q_c = 0.0;

	E = E / A;
	D = D / A;
	C = C / A;
	B = B / A;
	A = 1.0;

	double f = C - ((3.0 * B * B) / 8.0);

	double g = D + ((B * B * B) / 8.0) - (B * (C / 2.0));

	double h = E - ((3.0 * B * B * B * B) / 256.0) + ((B * B) * (C / 16.0)) - (B * (D / 4.0));

	y1 = 1.0;

	y2 = f / 2.0;

	y3 = ((f * f) - (4.0 * h)) / 16.0;

	y4 = (- g * g) / 64.0;



	X = cubic_solution (y1, y2, y3, y4);




	if ((fabs(X.at(3) > 0.0)) && (fabs(X.at(4) > 0.0))) {

		double R = sqrt (X.at(2) * X.at(2) + X.at(4) * X.at(4));
		double Y = sqrt ((R - X.at(2)) / 2.0);

		double Z = X.at(4) / (2.0 * Y);

		p = - Z;	p_c =   Y;
		q = - Z;	q_c = - Y;
	}

	else {

		if 		((fabs(X.at(0) > 1E-8)) &&  (fabs(X.at(1) > 1E-8))) {

			p = sqrt (X.at(0));
			q = sqrt (X.at(1));
		}

		else if ((fabs(X.at(0) > 1E-8)) &&  (fabs(X.at(2) > 1E-8))) {

			p = sqrt (X.at(0));
			q = sqrt (X.at(2));
		}

		else if ((fabs(X.at(1) > 1E-8)) &&  (fabs(X.at(2) > 1E-8))) {

			p = sqrt (X.at(1));
			q = sqrt (X.at(2));
		}
		else if ((fabs(X.at(0) > 1E-8)) &&  (fabs(X.at(1) > 1E-8)) &&  (fabs(X.at(2) > 1E-8))) {

			p = sqrt (X.at(0));
			q = sqrt (X.at(1));
		}

		else {

			result.push_back (999.99);
			result.push_back (999.99);
			result.push_back (999.99);
			result.push_back (999.99);
			result.push_back (999.99);
			result.push_back (999.99);

			return result;
		}
	}

	double r = -g / (8.0 * (p * q + p * q_c + q * p_c - p_c * q_c));
	double s = B / (4.0 * A);

 	result.push_back (  p + q + r - s);
	result.push_back (  p - q - r - s);
	result.push_back (- p + q - r - s);
	result.push_back (- p - q + r - s);
	result.push_back (  p_c   -   q_c);
	result.push_back (- p_c   +   q_c);

	return result;
}

STRESSFIELD eigenvalue_eigenvector (STRESSTENSOR st) {

	STRESSFIELD sf;

	double A, B, C, D;
	double a1, a2, b1, b2, c1, c2;

	vector < double > X;

	A =   1.0;

	B = - (st._11 + st._22 + st._33);

	C =   (st._11 * st._22) + (st._22 * st._33) + (st._11 * st._33) - (st._12 * st._12) - (st._23 * st._23) - (st._13 * st._13);

	D = - ((st._11 * st._22 * st._33) + (2.0 * st._12 * st._23 * st._13) - (st._12 * st._12 * st._33) - (st._23 * st._23 * st._11) - (st._13 * st._13 * st._22));

	X = cubic_solution (A, B, C, D);

	if ((X.at(0) >= X.at(1)) && (X.at(1) >= X.at(2))) {

		sf.EIGENVALUE.X = X.at(0);
		sf.EIGENVALUE.Y = X.at(1);
		sf.EIGENVALUE.Z = X.at(2);
	}

	else if ((X.at(0) >= X.at(2)) && (X.at(2) >= X.at(1))) {

		sf.EIGENVALUE.X = X.at(0);
		sf.EIGENVALUE.Y = X.at(2);
		sf.EIGENVALUE.Z = X.at(1);
	}

	else if ((X.at(1) >= X.at(0)) && (X.at(0) >= X.at(2))) {

		sf.EIGENVALUE.X = X.at(1);
		sf.EIGENVALUE.Y = X.at(0);
		sf.EIGENVALUE.Z = X.at(2);
	}

	else if ((X.at(1) >= X.at(2)) && (X.at(2) >= X.at(0))) {

		sf.EIGENVALUE.X = X.at(1);
		sf.EIGENVALUE.Y = X.at(2);
		sf.EIGENVALUE.Z = X.at(0);
	}

	else if ((X.at(2) >= X.at(0)) && (X.at(0) >= X.at(1))) {

		sf.EIGENVALUE.X = X.at(2);
		sf.EIGENVALUE.Y = X.at(0);
		sf.EIGENVALUE.Z = X.at(1);
	}

	else  {

		sf.EIGENVALUE.X = X.at(2);
		sf.EIGENVALUE.Y = X.at(1);
		sf.EIGENVALUE.Z = X.at(0);
	}

	a1 = st._11 - sf.EIGENVALUE.X;
	b1 = st._12;
	c1=  st._13;
	a2 = st._12;
	b2 = st._22 - sf.EIGENVALUE.X;
	c2 = st._23;

	sf.EIGENVECTOR1.Z = 1.0;
	sf.EIGENVECTOR1.X = ((b1 * c2) - (b2 * c1)) / ((b2 * a1) - (a2 * b1));
	sf.EIGENVECTOR1.Y = - ((a1 * sf.EIGENVECTOR1.X) + c1) / b1;
	sf.EIGENVECTOR1 = unitvector (sf.EIGENVECTOR1);


	a1 = st._11 - sf.EIGENVALUE.Y;
	b2 = st._22 - sf.EIGENVALUE.Y;

	sf.EIGENVECTOR2.Z = 1.0;
	sf.EIGENVECTOR2.X = ((b1 * c2) - (b2 * c1)) / ((b2 * a1) - (a2 * b1));
	sf.EIGENVECTOR2.Y = - ((a1 * sf.EIGENVECTOR2.X) + c1) / b1;
	sf.EIGENVECTOR2 = unitvector (sf.EIGENVECTOR2);


	a1 = st._11 - sf.EIGENVALUE.Z;
	b2 = st._22 - sf.EIGENVALUE.Z;

	sf.EIGENVECTOR3.Z = 1.0;
	sf.EIGENVECTOR3.X = ((b1 * c2) - (b2 * c1)) / ((b2 * a1) - (a2 * b1));
	sf.EIGENVECTOR3.Y = - ((a1 * sf.EIGENVECTOR3.X) + c1) / b1;
	sf.EIGENVECTOR3 = unitvector (sf.EIGENVECTOR3);

	return sf;
}

STRESSTENSOR stresstensor_from_eigenvalue_eigenvector (STRESSFIELD sf) {

	vector < vector <double> >  D = init_matrix (3);
	vector < vector <double> >  T = init_matrix (3);

	STRESSTENSOR out;

	D.at(0).at(0) = sf.EIGENVECTOR1.X;
	D.at(0).at(1) = sf.EIGENVECTOR1.Y;
	D.at(0).at(2) = sf.EIGENVECTOR1.Z;

	D.at(1).at(0) = sf.EIGENVECTOR2.X;
	D.at(1).at(1) = sf.EIGENVECTOR2.Y;
	D.at(1).at(2) = sf.EIGENVECTOR2.Z;

	D.at(2).at(0) = sf.EIGENVECTOR3.X;
	D.at(2).at(1) = sf.EIGENVECTOR3.Y;
	D.at(2).at(2) = sf.EIGENVECTOR3.Z;


	T.at(0).at(0) = sf.EIGENVALUE.X;
	T.at(0).at(1) = 0.0;
	T.at(0).at(2) = 0.0;

	T.at(1).at(0) = 0.0;
	T.at(1).at(1) = sf.EIGENVALUE.Y;
	T.at(1).at(2) = 0.0;

	T.at(2).at(0) = 0.0;
	T.at(2).at(1) = 0.0;
	T.at(2).at(2) = sf.EIGENVALUE.Z;


	T = mult_mtrx (transpose(D), T);

	T = mult_mtrx (T, D);


	out._11 = T.at(0).at(0);
	out._12 = T.at(0).at(1);
	out._13 = T.at(0).at(2);
	out._22 = T.at(1).at(1);
	out._23 = T.at(1).at(2);
	out._33 = T.at(2).at(2);

	return out;
}

STRESSFIELD computestressfield_DXDYDZ (STRESSFIELD in) {

	STRESSFIELD sf = in;

	sf.EIGENVECTOR1 = flip_D_vector (in.EIGENVECTOR1);
	sf.S_1 = dipdir_dip_from_DXDYDZ (sf.EIGENVECTOR1);

	sf.EIGENVECTOR2 = flip_D_vector (in.EIGENVECTOR2);
	sf.S_2 = dipdir_dip_from_DXDYDZ (sf.EIGENVECTOR2);

	sf.EIGENVECTOR3 = flip_D_vector (in.EIGENVECTOR3);
	sf.S_3 = dipdir_dip_from_DXDYDZ (sf.EIGENVECTOR3);

	return sf;
}

STRESSFIELD computestressfield_NXNYNZ (STRESSFIELD in) {

	STRESSFIELD sf = in;

	sf.EIGENVECTOR1 = flip_N_vector (in.EIGENVECTOR1);
	sf.S_1 = dipdir_dip_from_NXNYNZ (unitvector(in.EIGENVECTOR1));

	sf.EIGENVECTOR2 = flip_N_vector (in.EIGENVECTOR2);
	sf.S_2 = dipdir_dip_from_NXNYNZ (unitvector(in.EIGENVECTOR2));

	sf.EIGENVECTOR3 = flip_N_vector (in.EIGENVECTOR3);
	sf.S_3 = dipdir_dip_from_NXNYNZ (unitvector(in.EIGENVECTOR3));

	return sf;
}

STRESSTENSOR invert_stress_tensor (STRESSTENSOR st) {

	STRESSTENSOR out = st;

	out._11 = - out._11;
	out._12 = - out._12;
	out._13 = - out._13;
	out._22 = - out._22;
	out._23 = - out._23;
	out._33 = - out._33;

	return out;
}

VCTR return_stressvector (STRESSTENSOR st, GDB inGDB, bool compression_positive) {

	VCTR N = inGDB.N;

	VCTR out = declare_vector (
			(st._11 * N.X + st._12 * N.Y + st._13 * N.Z),
			(st._12 * N.X + st._22 * N.Y + st._23 * N.Z),
			(st._13 * N.X + st._23 * N.Y + st._33 * N.Z));

	if (! compression_positive)

	out = declare_vector (-out.X, -out.Y, -out.Z);

	return out;
}

VCTR return_normalstress (STRESSTENSOR st, GDB inGDB, bool compression_positive) {

	VCTR N = inGDB.N;
	VCTR stressvector = return_stressvector (st, inGDB, compression_positive);

	double stress = (N.X * stressvector.X) + (N.Y * stressvector.Y) + (N.Z * stressvector.Z);

	VCTR out = declare_vector (N.X * stress, N.Y * stress, N.Z * stress);

	return out;
}

VCTR return_shearstress (STRESSTENSOR st, GDB inGDB, bool compression_positive) {

	VCTR stressvector = return_stressvector (st, inGDB, compression_positive);
	VCTR normalstress = return_normalstress (st, inGDB, compression_positive);

	VCTR out = declare_vector(
			stressvector.X - normalstress.X,
			stressvector.Y - normalstress.Y,
			stressvector.Z - normalstress.Z);

	return out;
}

VCTR return_upsilon (STRESSTENSOR st, GDB inGDB, string method, bool compression_positive) {

	VCTR shearstress = return_shearstress (st, inGDB, compression_positive);
	VCTR out;

	if (method == "ANGELIER")

		out = declare_vector(
				(inGDB.SV.X * inGDB.lambda) - shearstress.X,
				(inGDB.SV.Y * inGDB.lambda) - shearstress.Y,
				(inGDB.SV.Z * inGDB.lambda) - shearstress.Z);

	else

		out = declare_vector(
				(inGDB.UPSILON.X * inGDB.lambda) - shearstress.X,
				(inGDB.UPSILON.Y * inGDB.lambda) - shearstress.Y,
				(inGDB.UPSILON.Z * inGDB.lambda) - shearstress.Z);

	return out;
}

double return_ANG (STRESSTENSOR st, GDB inGDB, bool compression_positive) {

	VCTR slipvector = inGDB.SV;
	VCTR shearstress = return_shearstress (st, inGDB, compression_positive);

	return ACOS (dotproduct (slipvector, shearstress, true));
}

double return_RUP (STRESSTENSOR st, GDB inGDB, bool compression_positive) {

	VCTR shearstress  = return_shearstress (st, inGDB, compression_positive);
	VCTR stressvector = return_stressvector (st, inGDB, compression_positive);
	VCTR slipvector = inGDB.SV;

	double out = inGDB.lambda * inGDB.lambda;

	out = out +
			(shearstress.X * shearstress.X) +
			(shearstress.Y * shearstress.Y) +
			(shearstress.Z * shearstress.Z);

	out = out - 2.0 * inGDB.lambda * dotproduct (slipvector, stressvector, false);

	return ((sqrt(out * out)) / inGDB.lambda) * 100.0;
}

double return_average_misfit (STRESSTENSOR st, vector <GDB> inGDB, bool compression_positive) {

	double misfit = 0.0;
	double ang = 0.0;
	size_t i = 0;

	do {

		ang = return_ANG (st, inGDB.at(i), compression_positive);
		misfit = misfit + ang;

		i++;

	} while (i < inGDB.size());

	return misfit / inGDB.size();
}


STRESSFIELD stress_regime (STRESSFIELD in) {

	STRESSFIELD out;
	out = in;

	out.stressratio = (out.EIGENVALUE.Y - out.EIGENVALUE.Z) / (out.EIGENVALUE.X - out.EIGENVALUE.Z);

	out.shmax = 999.99;
	out.shmin = 999.99;

	if ((out.S_1.DIP <= out.S_3.DIP) && (out.S_2.DIP <= out.S_3.DIP)) {

		out.regime = "COMPRESSIONAL";
		out.shmax = out.S_1.DIPDIR;
		out.delvaux_str = 2.0 + out.stressratio;
	}

	else if ((out.S_1.DIP <= out.S_2.DIP) && (out.S_3.DIP <= out.S_2.DIP)) {

		out.regime = "STRIKE-SLIP";
		out.shmax = out.S_1.DIPDIR;
		out.shmin = out.shmax + 90.0;
		out.delvaux_str = 2.0 - out.stressratio;
	}

	else {

		out.regime = "EXTENSIONAL";
		out.shmin = out.S_3.DIPDIR;
		out.delvaux_str = out.stressratio;
	}

	if 		((out.delvaux_str >= 0.00) && (out.delvaux_str < 0.25)) out.delvaux_rgm = "RADIAL EXTENSIVE";
	else if ((out.delvaux_str >= 0.25) && (out.delvaux_str < 0.75)) out.delvaux_rgm = "PURE EXTENSIVE";
	else if ((out.delvaux_str >= 0.75) && (out.delvaux_str < 1.25)) out.delvaux_rgm = "TRANSTENSIVE";
	else if ((out.delvaux_str >= 1.25) && (out.delvaux_str < 1.75)) out.delvaux_rgm = "PURE STRIKE SLIP";
	else if ((out.delvaux_str >= 1.75) && (out.delvaux_str < 2.25)) out.delvaux_rgm = "TRANSPRESSIVE";
	else if ((out.delvaux_str >= 2.25) && (out.delvaux_str < 2.75)) out.delvaux_rgm = "PURE COMPRESSIVE";
	else 															out.delvaux_rgm = "RADIAL COMPRESSIVE";

	return out;
}

vector <double> hyperplane_from_GDB (GDB inGDB)  {

	vector <double>  out = init_vector (6);
	CENTR_VECT o;

	VCTR n = inGDB.N;
	VCTR b = inGDB.SV;

	b = crossproduct (b, n);

	b = unitvector (b);

	o.U = - (n.X * b.X);
	o.V = - (n.Y * b.Y);
	o.W = - (n.Z * b.Z);

	o.X = - ((b.X * n.Y) + (b.Y * n.X));
	o.Y = - ((b.Y * n.Z) + (b.Z * n.Y));
	o.Z = - ((b.Z * n.X) + (b.X * n.Z));

	o = unitvector (o);

	out.at(0) = o.U;
	out.at(1) = o.V;
	out.at(2) = o.W;
	out.at(3) = o.X;
	out.at(4) = o.Y;
	out.at(5) = o.Z;

	return out;
}

vector < vector <double> > shan_matrix_from_GDB (GDB inGDB)  {

	vector <double>  o = init_vector (5);
	vector < vector <double> > out = init_matrix (5);

	VCTR n = inGDB.N;
	VCTR s = inGDB.DC;

	s = crossproduct (s, n);
	s = unitvector (s);

	o.at(0) = n.X * s.X - n.Z * s.Z;
	o.at(1) = n.Y * s.Y - n.Z * s.Z;
	o.at(2) = n.X * s.Y + n.Y * s.X;
	o.at(3) = n.X * s.Z + n.Z * s.X;
	o.at(4) = n.Y * s.Z + n.Z * s.Y;

	out = outer_product (o);

	return out;
}
