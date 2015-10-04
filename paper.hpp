// Copyright (C) 2012-2015, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef PAPER_HPP_
#define PAPER_HPP_

#include "structs.h"

using namespace std;

void INIT_PAPER (const bool WELL);

double mm_to_point (const size_t i);

PAPER RETURN_PAPER ();

#endif /* PAPER_HPP_ */
