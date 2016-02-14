// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef MOSTAFA_H
#define MOSTAFA_H

using namespace std;

vector <STRESSTENSOR> stv_MOSTAFA ();

vector <STRESSFIELD> sfv_MOSTAFA (const vector <GDB>& inGDB);

#endif
