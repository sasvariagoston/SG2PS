#ifndef BRUTE_FORCE_HPP_
#define BRUTE_FORCE_HPP_

#include <vector>

#include "structs.h"

using namespace std;

vector <vector <double> > DIR_MX1_from_n1 (VCTR n1, double angle);
vector <vector <double> >  st_from_reduced_stresstensor (vector <vector <double> > DIR_MX1, double fi);
vector <VCTR> net_to_centroids (vector <vector <vector <VCTR> > > NET);

void BRUTEFORCE (vector <GDB> inGDB, INPSET inset);

void dbg_cout_matrix (vector <vector <double> > IN);



#endif
