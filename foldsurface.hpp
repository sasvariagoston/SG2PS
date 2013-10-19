// Copyright (C) 2012, 2013 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef FOLDSURFACE_H
#define FOLDSURFACE_H

#include <vector>

/*#include "angelier.h"
#include "assertions.hpp"
#include "bingham.h"*/
#include "common.h"
/*#include "fry.h"
#include "inversion.h"
#include "michael.h"
#include "mostafa.h"
#include "nda.h"
#include "ps.h"
#include "ptn.h"
#include "rgf.h"
#include "shan.h"*/
#include "structs.h"


using namespace std;

void plot_fold_great_circle (VCTR GR_CRC, ofstream& o, INPSET inset, CENTER center);

void calculate_foldsurface (vector <GDB> inGDB, ofstream& o, INPSET inset, CENTER center);

#endif
