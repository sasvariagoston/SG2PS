// Copyright (C) 2012- 2017 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef BBO_HPP_
#define BBO_HPP_

#include "structs.h"

struct BBO {

	BBO () {a1=a2=b1=b2=c1=c2=d1=d2=e1=f1 = NaN(); }

	double a1, a2, b1, b2, c1, c2, d1, d2, e1, f1;
};

double return_SH (const GDB in, const BBO B, bool POSITIVE, bool MAX);
GDB return_shmax_shmin (const GDB in);

#endif /* BBO_HPP_ */
