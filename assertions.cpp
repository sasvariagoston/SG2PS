// Copyright (C) 2012-2016, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <stdexcept>
#include <string>

#include "assertions.hpp"

using namespace std;

void throw_std_logic_error(const string& msg, const string& func, const string& loc) {

    cout << flush;
    string location = "In \'" + func + "\' at " + loc;
    cerr << msg << endl;
    cerr << location << endl;

    throw logic_error(msg + '\n' + location);
}
