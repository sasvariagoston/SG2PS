// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <sstream>

#if __linux__
#include <fenv.h>
#endif

#include "array_to_vector.hpp"
#include "common.h"
#include "data_sort.hpp"
#include "settings.hpp"

template <typename T>
T convert(const string& s, bool& failed) {

	istringstream iss(s);

	T value;

	iss >> value;

	failed = (iss.fail() || !iss.eof()) ? true : false;

	return value;
}

template <typename T>
bool equals(const string& s, const T& value) {

	bool failed = true;

	T other = convert<T>(s, failed);

	return !failed && (value==other);
}

void print_banner () {

	writeln("");
	writeln(" ------------------------------------------------------------------ ");
	writeln("|                                                                  |");
	writeln("|                             SG2PS                                |");
	writeln("|       Structural Geological data to PostScript converter         |");
	writeln("|                                                                  |");
	writeln("|                       Data processing software                   |");
	writeln("|                                                                  |");
	writeln("|            Copyright (C) 2012-2016, Agoston Sasvari.             |");
	writeln("|                        All rights reserved.                      |");
	writeln("|             This a free software, license: GNU LGPL.             |");
	writeln("|                                                                  |");
	writeln("|              This software comes with NO WARRANTY.               |");
	writeln("|                                                                  |");
	writeln("|            For further information check www.sg2ps.eu            |");
	writeln("|                                                                  |");
	writeln(" ------------------------------------------------------------------ ");
	writeln("|                                                                  |");
	cout << "|                 Built on: " << version() <<   "                  |"<<endl;
	writeln("|                                                                  |");
	writeln(" ------------------------------------------------------------------ ");
	writeln("");

	return;
}

void progress_bar (const size_t WIDTH, const double STATUS, const double MAX) {

	const double PROGRESS = STATUS / MAX;

	const double DONE_D = WIDTH * PROGRESS;

	const size_t DONE = string_to_size_t (double_to_string (DONE_D, 0));

	const size_t LEFT = WIDTH - DONE;

	cout << '\r' << flush;

	cout << "[" << flush;

	for (size_t i = 0; i < DONE; i++) cout << "-" << flush;

	for (size_t i = 0; i < LEFT; i++) cout << " " << flush;

	cout << "]" << flush;

	cout << setfill (' ') << setw (4) << setprecision (0) << PROGRESS * 100 << "%" << flush;
}

string capslock (string input) {

	for(unsigned int i = 0; i < input.length(); i++)	input.at(i) = (char) toupper(input.at(i));
	return input;
}

string to_uppercase(string s) {

	transform(s.begin(), s.end(), s.begin(), ::toupper);

	return s;
}

vector<string> vec_to_uppercase(const vector<string>& v) {

	vector<string> res;

	transform(v.begin(), v.end(), back_inserter(res), to_uppercase);

	return res;
}

const string int_to_string (const int i) {

	ostringstream os;
	os << i << flush;
	return os.str();
}

const string size_t_to_string (const size_t i) {

	ostringstream os;
	os << i << flush;
	return os.str();
}

const string double_to_string(const double in, const int precision) {

	ostringstream os;
	os << fixed << setprecision (precision) << in << flush;
	return os.str();
}

const string dmp_dbl (double in, int precision) {

	if (is_nan(in)) return "NAN";

	ostringstream os;
	os << fixed << setprecision (precision) << in << flush;
	return os.str();
}

const string char_to_string (const char i) {

	ostringstream os;
	os << i << flush;
	return os.str();
}

double string_to_double( const string& s, bool& failed) {

	return convert<double>(s, failed);
}

double string_to_int( const string& s, bool& failed) {

	return convert<int>(s, failed);
}

template <typename T>
T to_type(const string& s) {

	bool conversion_failed = true;

	T ret = convert<T>(s, conversion_failed);

	ASSERT2(!conversion_failed,"failed to convert \""<<s<<"\"");

	return ret;
}

// throws logic_error
double string_to_double(const string& s) {

	return to_type<double>(s);
}

// throws logic_error
int string_to_int(const string& s) {

	return to_type<int>(s);
}

size_t string_to_size_t (const string& s) {

	return to_type <size_t> (s);
}

double deg_to_rad (const double alpha) {

    return (alpha * 3.1415926535) / 180.0;
}

double rad_to_deg (const double alpha) {

    return (alpha * 180.0) / 3.1415926535;
}

double SIN (const double& in) {

	double out = deg_to_rad (in);

	out = sin (out);

	ASSERT_FINITE(out);

	return out;
}

double COS (const double& in) {

	double out = deg_to_rad (in);

	out = cos (out);

	ASSERT_FINITE(out);

	return out;
}

double ASIN (const double& in) {

	double out = in;

	if (out >= 1.0)  out =   (1.0 - 10e-8);
	if (out <= -1.0) out = - (1.0 - 10e-8);

	out = asin (out);

	ASSERT_FINITE(out);

	return rad_to_deg (out);
}

double ACOS (const double& in) {

	double out = in;

	if (out >= 1.0)  out =   (1.0 - 10e-8);
	if (out <= -1.0) out = - (1.0 - 10e-8);

	out = acos(out);

	ASSERT_FINITE(out);

	return rad_to_deg (out);
}

double TAN (const double& in) {

	double out = deg_to_rad (in);

	out = tan (out);

	ASSERT_FINITE(out);

	return out;
}
double ATAN (const double& in) {

	double out = in;

	out = atan(out);

	ASSERT_FINITE(out);

	return rad_to_deg (out);
}

double factorial (size_t n) {

	ASSERT (n < 168);

	if (n == 0)	return 1;

	return n * factorial (n - 1);
}

// FIXME Clean up this messy implementation!
bool is_nan(double d) {
#if __linux__ && defined ENABLE_FPE
    fedisableexcept(FE_ALL_EXCEPT);
#endif
    bool result = std::isnan(d);
#if __linux__ && defined ENABLE_FPE
    feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);
#endif
    return result;
}


VCTR crossproduct (const VCTR& in1, const VCTR& in2) {

	return declare_vector (

			  (in1.Y * in2.Z) - (in2.Y * in1.Z),
			- (in1.X * in2.Z) + (in2.X * in1.Z),
			  (in1.X * in2.Y) - (in2.X * in1.Y)
	);
}

double dotproduct (const VCTR& in1, const VCTR& in2, const bool& normalisation) {

	double out = in1.X * in2.X + in1.Y * in2.Y + in1.Z * in2.Z;

	if (normalisation) {

		double l_in1 = sqrt((in1.X * in1.X) + (in1.Y * in1.Y) + (in1.Z * in1.Z));
		double l_in2 = sqrt((in2.X * in2.X) + (in2.Y * in2.Y) + (in2.Z * in2.Z));

		out = out / (l_in1 * l_in2);

		ASSERT_FINITE(out);
	}
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

vector <vector <double> >  declare_3x3_matrix (const double& a, const double& b, const double& c, const double& d, const double& e, const double& f, const double& g, const double& h, const double& i) {

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

vector <vector <double> > init_matrix (const size_t& dimension) {

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

vector <vector <double> > init_matrix (const size_t& i, const size_t& j) {

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

vector <double> init_vector (const size_t dimension) {

	vector <double> buffer;

	for (size_t i = 0; i < dimension; i++) buffer.push_back(0.0);

	return buffer;
}

size_t m_from_max_element (size_t max_element, const size_t n) {

	size_t j = 0;

	for (j = 0; max_element > n; j++) {

		max_element = max_element - n;
	}

	return j;
}

size_t search_max_off_diagonal_element_in_mtrx (const vector <vector <double> >& in) {

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

double teta (const vector <vector <double> >& in, const size_t m, const size_t n) {

	double teta = 2.0 * in.at(m).at(n) / (in.at(n).at(n) - in.at(m).at(m));

	double ret = atan(teta) / 2.0;

	ASSERT_FINITE(ret);

	return ret;
}

vector <vector <double> > init_rotation_mtrx (const double teta, const size_t m, const size_t n, const size_t dimension) {

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

vector <vector <double> > outer_product (const vector <double>& in) {

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

vector <vector <double> > transpose (const vector <vector <double> >& in) {

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

vector <vector <double> > mult_mtrx (const vector <vector <double> >& in1, const vector <vector <double> >& in2) {

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

vector <vector <double> > add_mtrx (const vector <vector <double> >& in1, const vector <vector <double> >& in2) {

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

int return_second_eigenvalue (const vector <vector <double> >& in) {

	vector <sort_jacobi> buffer;
	sort_jacobi buf;

	size_t j = 0;

	do {

		buf.ID = j;
		buf.eigenvalue = in.at(j).at(j);
		buffer.push_back(buf);

		j++;

	} while (j < in.size());

	stable_sort (buffer.begin(), buffer.end(), byeigenvalue);

	return buffer.at(1).ID;
}

int return_first_eigenvalue (const vector <vector <double> >& in) {

	sort_jacobi buf;
	vector <sort_jacobi> buffer;

	size_t j = 0;

	do {

		buf.ID = j;
		buf.eigenvalue = in.at(j).at(j);
		buffer.push_back(buf);

		j++;

	} while (j < in.size());

	stable_sort (buffer.begin(), buffer.end(), byeigenvalue);

	return buffer.at(0).ID;
}

vector <vector <double> > jacobi (const vector <vector <double> >& in) {

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

		n = m_from_max_element (max_element_in_mtrx, A.at(0).size());

		m = max_element_in_mtrx - (n * A.at(0).size()) - 1;

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

vector <vector <double> > gaussian_elimination (vector <vector <double> >& in) {

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

vector <vector <double> > LU_decomposition (const vector <vector <double> >& in) {

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

vector <vector <double> > compute_Z (const vector <vector <double> >& L, const vector <vector <double> >& c) {

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

vector <vector <double> > compute_X (const vector <vector <double> >& U, const vector <vector <double> >& Z) {

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

vector <vector <double> > row_division_diagonal (vector <vector <double> >& in, const size_t rownumber, const double value) {

	for (size_t i = 0; i < in.at(0).size(); i++) {

		in.at(rownumber).at(i) = in.at(rownumber).at(i) / value;
	}
	return in;
}

vector <vector <double> > row_addition (vector <vector <double> >& in, const size_t actual_row_number, const size_t zero_row_number, const double value) {

	for (size_t i = 0; i < in.size(); i++) {

		in.at(actual_row_number).at(i) = in.at(actual_row_number).at(i) - (value * in.at(zero_row_number).at(i));
	}
	return in;
}

vector <vector <double> > row_addition_LU (vector <vector <double> >& in, const size_t actual_row_number, const size_t zero_row_number, const double value) {

	for (size_t i = 0; i < in.size(); i++) {

		in.at(actual_row_number).at(i) = in.at(actual_row_number).at(i) - (value * in.at(zero_row_number).at(i));
	}
	return in;
}

vector < vector < double > > generate_A (const vector < vector < double > >& EVEV) {

	vector < vector < double > > A = init_matrix (EVEV.at(0).size());
	size_t i = 0;

	do {

		A.at(i) = EVEV.at(i);
		i++;

	} 	while (i < (EVEV.size() / 2));

	return A;
}

vector < vector < double > > generate_D (const vector < vector < double > >& EVEV) {

	vector < vector < double > > D = init_matrix (EVEV.at(0).size());
	size_t i = 0;

	do {

		D.at(i) = EVEV.at(i + (EVEV.size() / 2));
		i++;

	} 	while (i < (EVEV.size() / 2));

	return D;
}

bool check_correct_stressfield (const STRESSFIELD& sf) {

	if (! is_in_range (0.0, 360.0, sf.S_1.DIPDIR)) return false;
	if (! is_in_range (0.0, 360.0, sf.S_2.DIPDIR)) return false;
	if (! is_in_range (0.0, 360.0, sf.S_3.DIPDIR)) return false;

	if (! is_in_range (0.0, 90.0, sf.S_1.DIP)) return false;
	if (! is_in_range (0.0, 90.0, sf.S_2.DIP)) return false;
	if (! is_in_range (0.0, 90.0, sf.S_3.DIP)) return false;

	const double ANG1 = vector_angle (sf.EIGENVECTOR1, sf.EIGENVECTOR2);
	const double ANG2 = vector_angle (sf.EIGENVECTOR2, sf.EIGENVECTOR3);
	const double ANG3 = vector_angle (sf.EIGENVECTOR1, sf.EIGENVECTOR3);

	if (! is_in_range (88.0, 92.0, ANG1)) return false;
	if (! is_in_range (88.0, 92.0, ANG2)) return false;
	if (! is_in_range (88.0, 92.0, ANG3)) return false;

	return true;
}

string generate_stress_colors (const double V) {

	double P = 0.0;

	if (is_GRAYSCALE_USE()) return ("0.8 0.8 0.8");

	if (is_in_range (0.0, 0.5, V)) {

		P = (V - 0.0) / 0.5;
		return 	double_to_string (0.0 * (1.0 - P) + 0.0 * P, 3) + " " +
				double_to_string (0.0 * (1.0 - P) + 0.0 * P, 3) + " " +
				double_to_string (0.0 * (1.0 - P) + 1.0 * P, 3);
	}
	else if (is_in_range (0.5, 1.0, V)) {

		P = (V - 0.5) / 0.5;
		return 	double_to_string (0.0 * (1.0 - P) + 0.0 * P, 3) + " " +
				double_to_string (0.0 * (1.0 - P) + 1.0 * P, 3) + " " +
				double_to_string (1.0 * (1.0 - P) + 1.0 * P, 3);
	}
	else if (is_in_range (1.0, 1.5, V)) {

		P = (V - 1.0) / 0.5;
		return 	double_to_string (0.0 * (1.0 - P) + 0.0 * P, 3) + " " +
				double_to_string (1.0 * (1.0 - P) + 1.0 * P, 3) + " " +
				double_to_string (1.0 * (1.0 - P) + 0.0 * P, 3);
	}
	else if (is_in_range (1.5, 2.0, V)) {

		P = (V - 1.5) / 0.5;
		return 	double_to_string (0.0 * (1.0 - P) + 1.0 * P, 3) + " " +
				double_to_string (1.0 * (1.0 - P) + 1.0 * P, 3) + " " +
				double_to_string (0.0 * (1.0 - P) + 0.0 * P, 3);
	}
	else if (is_in_range (2.0, 2.5, V)) {

		P = (V - 2.0) / 0.5;
		return 	double_to_string (1.0 * (1.0 - P) + 1.0 * P, 3) + " " +
				double_to_string (1.0 * (1.0 - P) + 0.5 * P, 3) + " " +
				double_to_string (0.0 * (1.0 - P) + 0.0 * P, 3);
	}
	else {

		P = (V - 2.5) / 0.5;
		return 	double_to_string (1.0 * (1.0 - P) + 1.0 * P, 3) + " " +
				double_to_string (0.5 * (1.0 - P) + 0.0 * P, 3) + " " +
				double_to_string (0.0 * (1.0 - P) + 0.0 * P, 3);
	}
}

double vector_angle (const VCTR& A, const VCTR& B) {

	return ACOS (dotproduct (A, B, false));
}

double vectorlength (const VCTR& in) {

	return sqrt(in.X * in.X + in.Y * in.Y + in.Z * in.Z);
}

VCTR unitvector (const VCTR& in, const bool CHECK) {

	if (CHECK) {
	    ASSERT_FINITE(in.X, in.Y, in.Z);
	}

	VCTR OUT;

	double vectorlength;

	if (CHECK) vectorlength = sqrt(in.X * in.X + in.Y * in.Y + in.Z * in.Z);
	else vectorlength = sqrt(in.X * in.X + in.Y * in.Y + in.Z * in.Z + 1.0e-6);

	if (CHECK) {

		if ((vectorlength > 10e-20) && (vectorlength < 1.0e+300)) {

			OUT.X = (in.X / vectorlength);
			OUT.Y = (in.Y / vectorlength);
			OUT.Z = (in.Z / vectorlength);
		}
		else ASSERT2(false,"Problem with vector length, [X, Y, Z] = [ "<<OUT.X<<", "<<OUT.Y<<", "<<OUT.Z<<"]");
	}
	else {

		OUT.X = (in.X / vectorlength);
		OUT.Y = (in.Y / vectorlength);
		OUT.Z = (in.Z / vectorlength);
	}
	return OUT;
}

CENTR_VECT unitvector (CENTR_VECT in) {

	double vectorlength = sqrt
			(in.U * in.U + in.V * in.V + in.W * in.W +
			 in.X * in.X + in.Y * in.Y + in.Z * in.Z);

	if (vectorlength > 0.0000000000001) {

		in.U = in.U / vectorlength;
		in.V = in.V / vectorlength;
		in.W = in.W / vectorlength;
		in.X = in.X / vectorlength;
		in.Y = in.Y / vectorlength;
		in.Z = in.Z / vectorlength;
	}
	else {
		ASSERT2(false,"[U, V, W, X, Y, Z] = [ "<<in.U<<", "<<in.V<<", "<<in.W<<", "<<in.X<<", "<<in.Y<<", "<<in.Z<<"]");
	}

	return in;
}

VCTR declare_vector (const double a, const double b, const double c) {

    ASSERT_FINITE(a, b, c);

	VCTR o;

	o.X = a;
	o.Y = b;
	o.Z = c;

	return o;
}

CENTR_VECT declare_vector (const double a, const double b, const double c, const double d, const double e, const double f) {

    ASSERT_FINITE(a, b, c, d, e, f);

	CENTR_VECT o;

	o.U = a;
	o.V = b;
	o.W = c;
	o.X = d;
	o.Y = e;
	o.Z = f;

	return o;
}

VCTR flip_vector (const VCTR& in) {

	return declare_vector(-in.X, -in.Y, -in.Z);
}

VCTR DXDYDZ_from_dipdir_dip (const DIPDIR_DIP& i) {

	return declare_vector (
			SIN (i.DIPDIR) * COS (i.DIP),
			COS (i.DIPDIR) * COS (i.DIP),
						   - SIN (i.DIP)
	);
}

VCTR NXNYNZ_from_dipdir_dip (const DIPDIR_DIP& i) {

	return declare_vector (
			SIN (i.DIPDIR) * SIN (i.DIP),
			COS (i.DIPDIR) * SIN (i.DIP),
							 COS (i.DIP)
	);
}

VCTR DXDYDZ_from_NXNYNZ (const VCTR& i) {

	return DXDYDZ_from_dipdir_dip (dipdir_dip_from_NXNYNZ (i));
}

VCTR NXNYNZ_from_DXDYDZ (const VCTR& i) {

	return NXNYNZ_from_dipdir_dip (dipdir_dip_from_DXDYDZ (i));
}

DIPDIR_DIP dipdir_dip_from_DXDYDZ (const VCTR& i) {

	DIPDIR_DIP actual;

	VCTR out = unitvector (i, true);

	actual.DIP = fabs(ACOS(out.Z) - 90.0);

	if (out.Y == 0.0) out.Y = 0.00000001;

	actual.DIPDIR = ATAN (out.X / out.Y);

	if ((out.X > 0.0) && (out.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((out.X < 0.0) && (out.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((out.X < 0.0) && (out.Y > 0.0)) actual.DIPDIR = 360.0 + actual.DIPDIR;

	return actual;
}

DIPDIR_DIP dipdir_dip_from_NXNYNZ (const VCTR& i) {

	VCTR out = unitvector (i, true);

	DIPDIR_DIP actual;

	actual.DIP = ACOS(out.Z);

	if (out.Y == 0.0) out.Y = 0.0000000000001;

	actual.DIPDIR = ATAN (out.X / out.Y);

	if ((out.X > 0.0) && (out.Y > 0.0)) actual.DIPDIR =   0.0 + actual.DIPDIR;
	if ((out.X > 0.0) && (out.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((out.X < 0.0) && (out.Y < 0.0)) actual.DIPDIR = 180.0 + actual.DIPDIR;
	if ((out.X < 0.0) && (out.Y > 0.0)) actual.DIPDIR = 360.0 + actual.DIPDIR;

	return actual;
}

VCTR ROTATE (const VCTR& ax, const VCTR& torotate, const double& A) {

	VCTR result;

	const double COS_A = COS (A);
	const double MIN_COS_A = 1.0 - COS (A);
	const double SIN_A = SIN (A);

	VCTR A_1;
	A_1.X = COS_A + MIN_COS_A * ax.X * ax.X;
	A_1.Y = MIN_COS_A * ax.Y * ax.X - SIN_A * ax.Z;
	A_1.Z = MIN_COS_A * ax.Z * ax.X + SIN_A * ax.Y;

	VCTR A_2;
	A_2.X = MIN_COS_A * ax.Y * ax.X + SIN_A * ax.Z;
	A_2.Y = COS_A + MIN_COS_A * ax.Y * ax.Y;
	A_2.Z = MIN_COS_A * ax.Y * ax.Z - SIN_A * ax.X;

	VCTR A_3;
	A_3.X = MIN_COS_A * ax.Z * ax.X - SIN_A * ax.Y;
	A_3.Y = MIN_COS_A * ax.Y * ax.Z + SIN_A * ax.X;
	A_3.Z = COS_A + MIN_COS_A * ax.Z * ax.Z;

	result.X = dotproduct (torotate, A_1, false);
	result.Y = dotproduct (torotate, A_2, false);
	result.Z = dotproduct (torotate, A_3, false);

	return unitvector (result, true);
}

bool existence (const string& expression, const vector<GDB>& inGDB) { // TODO contains? contains datatype?

	for (size_t i = 0; i < inGDB.size(); i++) {

		if (inGDB.at(i).DATATYPE == expression) return true;
	}
	return false;
}

bool existence_of_group (const size_t group, const vector <size_t>& whichgroup) {

	for (size_t i = 0; i < whichgroup.size(); i++) {

		if (whichgroup.at(i) == group) return true;
	}
	return false;
}

vector <GDB> merge_GDB (const vector <GDB>& source, const vector <GDB>& target) {

	vector <GDB> OUT = target;

	for (size_t i = 0; i < source.size(); i++) OUT.push_back(source.at(i));

	return OUT;
}

double average (const vector <double>& IN) {

	if (IN.size() == 1) return IN.at(0);

	double OUT = 0.0;

	const size_t S = IN.size();

	ASSERT_GE (S, 1);
	//if (S < 1) ASSERT_DEAD_END();

	double CNT = 0.0;

	for (size_t i = 0; i < S; i++) {

		CNT++;

		OUT = OUT + IN.at(i);
	}
	return (OUT  / CNT);
}

double stdev (const vector <double>& IN) {

	double OUT = 0.0;

	const double AV = average (IN);

	double CNT = 0.0;

	for (size_t i = 0; i < IN.size(); i++) {

		CNT++;

		double SD = (IN.at(i) - AV) * (IN.at(i) - AV);

		ASSERT_GE (SD, 0.0);

		OUT = OUT + SD;
	}
	return sqrt(OUT / CNT);
}

VCTR VCTR_average (const vector <VCTR>& IN) {

	vector <double> X, Y, Z;

	const size_t S = IN.size();

	ASSERT_GE (S,  1);

	for (size_t i = 0; i < S; i++) {

		X.push_back (IN.at(i).X);
		Y.push_back (IN.at(i).Y);
		Z.push_back (IN.at(i).Z);
	}
	return unitvector (declare_vector (average(X), average(Y), average(Z)), true);
}

double median (const vector <double>& IN) {

	if (IN.size() == 1) return IN.at(0);

	vector <double> temp = IN;

	stable_sort (temp.begin(), temp.end());

	const size_t S = temp.size();

	const bool EVEN = S%2 == 0;

	vector <double> to_process;

	for (size_t i = 0; i < S; i++) {

		to_process.push_back (temp.at(i));
		to_process.push_back (temp.at(i));
	}
	if (EVEN) return to_process.at (S);

	return (to_process.at (S) + to_process.at (S+1)) / 2.0;
}

vector <double> quadratic_solution (const double A, const double B, const double C) {

	vector <double> out;

	double DET = (B * B) - (4.0 * A * C);

	if (DET < 0.0) {

		out.push_back(999.99);
		out.push_back(999.99);
	}
	else {

		out.push_back((- B - sqrt (DET)) / (2.0 * A));
		out.push_back((- B + sqrt (DET)) / (2.0 * A));
	}

	return out;
}

vector <double> cubic_solution (const double A, const double B, const double C, const double D) {

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

		ASSERT_FINITE(K);

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




	if (fabs(X.at(3)) > 1E-8 && fabs(X.at(4)) > 1E-8) {  //if complex

		double R = sqrt (X.at(2) * X.at(2) + X.at(4) * X.at(4));
		double Y = sqrt ((R - X.at(2)) / 2.0);

		double Z = X.at(4) / (2.0 * Y);

		p = - Z;	p_c =   Y;
		q = - Z;	q_c = - Y;
	}
	else { //if not complex

		if 		(fabs(X.at(0)) > 1E-8 &&  fabs(X.at(1)) > 1E-8) {

			p = sqrt (X.at(0));
			q = sqrt (X.at(1));
		}

		else if (fabs(X.at(0)) > 1E-8 &&  fabs(X.at(2)) > 1E-8) {

			p = sqrt (X.at(0));
			q = sqrt (X.at(2));
		}

		else if (fabs(X.at(1)) > 1E-8 &&  fabs(X.at(2)) > 1E-8) {

			p = sqrt (X.at(1));
			q = sqrt (X.at(2));
		}
		else if (fabs(X.at(0)) > 1E-8 &&  fabs(X.at(1)) > 1E-8 &&  fabs(X.at(2)) > 1E-8) {

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

bool byOFFSET(const GDB& x, const GDB& y) {

	return x.OFFSET < y.OFFSET;
}

bool bycorrDIPDIRcorrDIP(const GDB& x, const GDB& y) {

	if (x.corr.DIPDIR != y.corr.DIPDIR) return x.corr.DIPDIR < y.corr.DIPDIR;
	return x.corr.DIP < y.corr.DIP;
}

bool bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP(const GDB& x, const GDB& y) {

	if (x.corr.DIPDIR 	!= y.corr.DIPDIR)	return x.corr.DIPDIR < y.corr.DIPDIR;
	if (x.corr.DIP 		!= y.corr.DIP) 		return x.corr.DIP < y.corr.DIP;
	if (x.corrL.DIPDIR 	!= y.corrL.DIPDIR) 	return x.corrL.DIPDIR < y.corrL.DIPDIR;
	return x.corrL.DIP < y.corrL.DIP;
}

void output_elapsed_time (const clock_t& start_t, const clock_t& finish_t) {

	double elapsed_time = (static_cast<double>(finish_t - start_t))/CLOCKS_PER_SEC;

	elapsed_time *= 1000;

	if (elapsed_time < 1 * 1000.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " milliseconds." << endl;
	else {
		elapsed_time = elapsed_time / 1000.0;
		if (elapsed_time < 1 * 60.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " seconds." << endl;
		else {
			elapsed_time = elapsed_time / 60.0;
			if (elapsed_time < 1 * 60.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " minutes." << endl;
			else {
				elapsed_time = elapsed_time / 60.0;
				if (elapsed_time < 1 * 60.0) cout << "  - Elapsed time: " << fixed << setprecision (2) << elapsed_time << " hours." << endl;
				else cout << "  - Elapsed time: " << fixed << setprecision (1) << elapsed_time / 60.0 << " days." << endl;
			}
		}
	}
}

string build_date () {

	vector <char> date (11, ' ');

	string DATE  = __DATE__;

	if (DATE.at(4) == ' ') 		date.at(0) = '0';
	else 						date.at(0) = DATE.at(4);

	date.at(1) = 	DATE.at(5);
	date.at(2) = 	' ';
	date.at(3) = 	DATE.at(0);
	date.at(4) = 	DATE.at(1);
	date.at(5) = 	DATE.at(2);
	date.at(6) = 	' ';
	date.at(7) = 	DATE.at(7);
	date.at(8) = 	DATE.at(8);
	date.at(9) = 	DATE.at(9);
	date.at(10) = 	DATE.at(10);

	for (size_t i = 0; i < 11; i++) DATE.at(i) = date.at(i);

	return DATE;
}

string build_time () {

	return __TIME__;
}

string version() {

	return build_date()+", "+build_time();
}

namespace {

const string DATE = __DATE__;

struct date { enum format { MON1, MON2,	MON3, SPACE1, DAY1, DAY2, SPACE2, YEAR1, YEAR2, YEAR3, YEAR4, SIZE }; };

const string month_names[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const vector<string> months = from_array(month_names);

size_t month_zero_based() {
	// TODO Find index to utility class
	string Mmm = DATE.substr(date::MON1, 3);

	size_t month = static_cast<size_t>( find(months.begin(), months.end(), Mmm) - months.begin() );

	ASSERT_LT(month, 12);

	return month;
}

int day_of_month() {

	string dd = DATE.substr(date::DAY1, 2);

	int day = string_to_int(dd);

	ASSERT_GE(day,  1);

	ASSERT_LE(day, 31);

	return day;
}

int year_since_1900() {

	string yyyy = DATE.substr(date::YEAR1, 4);

	int year = string_to_int(yyyy);

	ASSERT_GE(year, 2013);

	return year-1900;
}

const string TIME = __TIME__;

struct tformat { enum format { H1, H2, COLON1, M1, M2, COLON2, S1, S2, SIZE }; };

int hour() {

	string hh = TIME.substr(tformat::H1, 2);

	int h = string_to_int(hh);

	ASSERT_GE(h,  0);
	ASSERT_LE(h, 23);

	return h;
}

int minute() {

	string mm = TIME.substr(tformat::M1, 2);

	int min = string_to_int(mm);

	ASSERT_GE(min,  0);
	ASSERT_LE(min, 59);

	return min;
}

int second() {

	string s = TIME.substr(tformat::S1, 2);

	int sec = string_to_int(s);

	ASSERT_GE(sec,  0);
	ASSERT_LE(sec, 60);

	return sec;
}

tm build_tm() {

	tm t = { 0 };

	try {
		t.tm_year = year_since_1900();
		t.tm_mon  = month_zero_based();
		t.tm_mday = day_of_month();
		t.tm_hour = hour();
		t.tm_min  = minute();
		t.tm_sec  = second();
	}
	catch (logic_error& ) {
		cout << "Date: " << DATE << ", time: " << TIME << endl;
		throw;
	}

	return t;
}

#ifdef __linux__

string version_id_linux() {

	tm t = { 0 };

	strptime(__DATE__ " " __TIME__, "%b %e %Y %H:%M:%S", &t);

	char buff[16];

	return strftime(buff, sizeof(buff), "%Y%m%d%H%M%S", &t) ? buff : "(unknown)";
}

#endif

}

string version_id() {

	struct tm t = build_tm();

	char buff[16];

	string id = strftime(buff, sizeof(buff), "%Y%m%d%H%M%S", &t) ? buff : "(unknown)";
#ifdef __linux__
	string id2 = ::version_id_linux();
	ASSERT_EQ(id, id2);
#endif
	return id;
}

bool is_in_range (const double range_min, const double range_max, const double in) {

	ASSERT_FINITE (in);

	double SN = 10e-8;

	return (range_min - SN <= in && in <= range_max + SN);
}

bool is_in_range_exactly (const double range_min, const double range_max, const double in) {

	ASSERT_FINITE (in);

	return (range_min  < in && in < range_max);
}

bool is_in_range_UP_EQ (const double range_min, const double range_max, const double in) {

	ASSERT_FINITE (in);

	return (range_min  <= in && in < range_max);
}

bool is_in_range_LW_EQ (const double range_min, const double range_max, const double in) {

	ASSERT_FINITE (in);

	return (range_min  < in && in <= range_max);
}

double points_distance (const VCTR& a, const VCTR&  b) {

	return sqrt (
			(b.X - a.X) * (b.X - a.X) +
			(b.Y - a.Y) * (b.Y - a.Y) +
			(b.Z - a.Z) * (b.Z - a.Z)
			);
}

void write (const string MSG) {

	cout << MSG << flush;
	return;
}

void writeln (const string MSG) {

	cout << MSG << endl;
	return;
}
