
// Copyright (C) 2012 - 2014 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <ctime>
#include <vector>
#include "structs.h"

using namespace std;

//void header();

void print_banner () ;

string capslock (string input);

string to_uppercase(string s);
vector<string> vec_to_uppercase(const vector<string>& v);

const string int_to_string(const int i);
const string size_t_to_string (const size_t i);
const string double_to_string(const double in, const int precision);
const string dmp_dbl (const double in, const int precision);
const string char_to_string (const char i);
double string_to_double(const string& s, bool& failed);
double string_to_int( const string& s, bool& failed);

double string_to_double(const string& s); // throws logic_error

int string_to_int(const string& s); // throws logic_error
size_t string_to_size_t (const string& s);

double SIN  (const double& in);
double COS 	(const double& in);
double ASIN (const double& in);
double ACOS (const double& in);
double ACOS_NUM (const double& in);
double TAN (const double& in);
double ATAN (const double& in);

double rounding (double in);
double mm_to_point (size_t i);

VCTR  crossproduct (const VCTR& in1, const VCTR& in2);
double dotproduct (const VCTR& in1, const VCTR& in2, const bool& normalisation=false);

double det_3 (vector <vector <double> > in);

double stresstensor_determinant (const STRESSTENSOR& st);

void check_stress_tensor_singularity(const STRESSTENSOR& st);

vector <vector <double> >  declare_3x3_matrix (const double& a, const double& b, const double& c, const double& d, const double& e, const double& f, const double& g, const double& h, const double& i);

vector <vector <double> > init_matrix (const size_t& dimension);
vector <vector <double> > init_matrix (const size_t& i, const size_t& j);

vector <vector <double> > identity_matrix (vector <vector <double> >);

vector <double> init_vector (const size_t dimension);

size_t search_max_off_diagonal_element_in_mtrx (const vector <vector <double> >& in);
size_t m_from_max_element (size_t max_element, const size_t n);
double teta (const vector <vector <double> >& in, const size_t m, const size_t n);
vector <vector <double> > init_rotation_mtrx (const double teta, const size_t m, const size_t n, const size_t dimension);
vector <vector <double> > outer_product (const vector <double>& in);
vector <vector <double> > transpose (const vector <vector <double> >& in);
vector <vector <double> > mult_mtrx (const vector <vector <double> >& in1, const vector <vector <double> >& in2);
vector <vector <double> > add_mtrx (const vector <vector <double> >& in1, const vector <vector <double> >& in2);

int return_second_eigenvalue (const vector <vector <double> >& in);
int return_first_eigenvalue (const vector <vector <double> >& in);

vector <vector <double> > jacobi (const vector <vector <double> >& in);

vector <vector <double> > gaussian_elimination (vector <vector <double> >& in);

vector <vector <double> > LU_decomposition (const vector <vector <double> >& in);
vector <vector <double> > compute_Z (const vector <vector <double> >& L, const vector <vector <double> >& c);
vector <vector <double> > compute_X (const vector <vector <double> >& U, const vector <vector <double> >& Z);


vector <vector <double> > row_division_diagonal (vector <vector <double> >& in, const size_t rownumber, const double value);
vector <vector <double> > row_addition (vector <vector <double> >& in, const size_t actual_row_number, const size_t zero_row_number, const double value);
vector <vector <double> > row_addition_LU (vector <vector <double> >& in, const size_t actual_row_number, const size_t zero_row_number, const double value);

vector < vector < double > > generate_A (const vector < vector < double > >& EVEV);
vector < vector < double > > generate_D (const vector < vector < double > >& EVEV);

bool check_correct_stressfield (const STRESSFIELD& sf);

string generate_stress_colors (const double V);

double vector_angle (const VCTR& A, const VCTR& B);
double vectorlength (const VCTR& in);
VCTR unitvector (const VCTR& in);
VCTR invert_VCTR (VCTR in);
CENTR_VECT unitvector (CENTR_VECT in);
vector <double> unitvector (vector <double>& in);

VCTR declare_vector (const double a, const double b, const double c);
CENTR_VECT declare_vector (const double a, const double b, const double c, const double d, const double e, const double f);

VCTR flip_vector (const VCTR& in);
//VCTR flip_D_vector (const VCTR& in);
//VCTR flip_N_vector (const VCTR& in);

VCTR compute_d_for_SC (const GDB& i);

VCTR DXDYDZ_from_dipdir_dip (const DIPDIR_DIP& i);
VCTR NXNYNZ_from_dipdir_dip (const DIPDIR_DIP& i);

STRESSFIELD stressvector_to_DXDYDZ (STRESSFIELD in);

VCTR DXDYDZ_from_NXNYNZ (const VCTR& i);
VCTR NXNYNZ_from_DXDYDZ (const VCTR& i);

DIPDIR_DIP dipdir_dip_from_DXDYDZ (const VCTR& i);
DIPDIR_DIP dipdir_dip_from_NXNYNZ (const VCTR& i);

VCTR ROTATE (const VCTR& ax, const VCTR& torotate, const double& A);

bool existence (const string& expression, const vector<GDB>& inGDB);
bool existence_of_group (const size_t group, const vector <size_t>& whichgroup);

vector <GDB> merge_GDB (const vector <GDB>& source, const vector <GDB>& target);

double average (const vector <double>& IN);
double stdev (const vector <double>& IN);
VCTR VCTR_average (const vector <VCTR>& IN);
double median (const vector <double>& IN);

vector <double> quadratic_solution (const double A, const double B, const double C);
vector <double> cubic_solution (const double A, const double B, const double C, const double D);
vector <double> quartic_solution (double A, double B, double C, double D, double E);

STRESSTENSOR fix_stress_tensor_singularity(STRESSTENSOR& st);
STRESSFIELD eigenvalue_eigenvector (STRESSTENSOR st);
STRESSTENSOR stresstensor_from_eigenvalue_eigenvector (STRESSFIELD sf);

STRESSFIELD stress_regime (const STRESSFIELD& in);
STRESSTENSOR convert_matrix_to_stresstensor (const vector <vector <double> >& IN);

bool bycorrDIPDIRcorrDIP(const GDB& x, const GDB& y);
bool bycorrDIPDIRcorrDIPcorrLDIPDIRcorrLDIP(const GDB& x, const GDB& y);

//double right_hand_rule_to_german (const double corrDIPDIR);
//double german_to_right_hand_rule (const double corrDIPDIR);

void output_elapsed_time (const clock_t& start_t, const clock_t& finish_t);
string build_date ();
string build_time ();
string version();
string version_id();

bool is_in_range (const double range_min, const double range_max, const double in); //ok

double points_distance (const VCTR& a, const VCTR& b);

//vector <VCTR> convert_vectors_to_S_or_W (vector <VCTR> in);

#if _MSC_VER && !__INTEL_COMPILER
#include <float.h>
#define isnan _isnan
#endif

#endif
