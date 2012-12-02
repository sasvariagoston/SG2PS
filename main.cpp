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
		// I believe it is the normal behavior so nothing to do
	}

	catch (logic_error& e) {

		cout << "Logic error: " << e.what() << endl;
		return ExitStatus::BUG;
	}

	catch(std::runtime_error& e) {

		cout << "Runtime error: " << e.what() << endl;
		return ExitStatus::RUNTIME_ERROR;
	}

	catch(rgf_error&) {

		cout << "RGF error error: " << endl;
		return ExitStatus::RGF_ERROR;
	}

	// TODO In the release build, there will be more to catch

	return ExitStatus::OK;
}
