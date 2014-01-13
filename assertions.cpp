// Copyright (C) 2012, 2013 Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

void throw_std_logic_error(const string& msg) {

	cout << flush;
	cerr << msg << endl;

	throw logic_error(msg);
}
