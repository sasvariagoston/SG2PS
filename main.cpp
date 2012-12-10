// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#include <iostream>
#include <stdexcept>
#include "exceptions.hpp"
#include "ExitStatus.hpp"

using namespace std;

void real_main(int argument_number, char *argv[]);

int main (int argc, char *argv[]) {

	try {

		real_main(argc, argv);
	}
	catch(exit_requested& ) {
		// User requested exit
		// Normal behavior, nothing to do
	}
	catch(rgf_error& ) {
		cout << "RGF error error: " << endl;
		return ExitStatus::RGF_ERROR;
	}
	catch(out_of_range& ) {
		cout << "This is a bug, please report it!" << endl;
		return ExitStatus::BUG;
	}
	catch(runtime_error& e) {
		cout << "Runtime error: " << e.what() << endl;
		return ExitStatus::RUNTIME_ERROR;
	}
	catch(...) {
		cout << "Unknown error, please report it!" << endl;
		return ExitStatus::UNKNOWN_ERROR;
	}

	return ExitStatus::OK;
}
