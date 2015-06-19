
// Copyright (C) 2012 - 2015 Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <stdexcept>
#include <string>

#include "assertions.hpp"

using namespace std;

void GDB_size_check (const vector <GDB>& inGDB) {

	ASSERT2(inGDB.size() > 0, "vector <GDB> has no records");

	return;
}

void throw_std_logic_error(const string& msg) {

	cout << flush;
	cerr << msg << endl;

	throw logic_error(msg);
}
