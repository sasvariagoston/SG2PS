// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#include <iostream>
#include <stdexcept>
#include "exceptions.hpp"

using namespace std;

void real_main(int argument_number, char *argv[]);

int main (int argc, char *argv[]) {

	enum error_codes { SUCCESS, RUNTIME_ERROR };

	try {

		real_main(argc, argv);
	}
	catch(exit_requested& ) {
		// User requested exit
		// I believe it is the normal behavior so nothing to do
	}
	catch(std::runtime_error& e) {

		cout << "Runtime error: " << e.what() << endl;
		return RUNTIME_ERROR;
	}
	// TODO In the release build, there will be more to catch

	return SUCCESS;
}
