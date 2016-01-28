// Copyright (C) 2012-2016, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <iostream>
#include <stdexcept>

#include "exceptions.hpp"
#include "ExitStatus.hpp"

using namespace std;

void enable_fpe();
void real_main(int argc, char *argv[]);

int main (int argc, char *argv[]) {

    enable_fpe(); // does nothing if ENABLE_FPE is not defined

    try {
        real_main(argc, argv);
    }
    catch(exit_requested& ) {
        return ExitStatus::OK;
    }
    catch(rgf_error& ) {
        return ExitStatus::RGF_ERROR;
    }
    catch(set_error& ) {
        return ExitStatus::SET_ERROR;
    }
    catch(xy_error& ) {
        return ExitStatus::XY_ERROR;
    }
    catch(trj_error& ) {
        return ExitStatus::TRJ_ERROR;
    }
    catch (arg_error& e) {
        cout << "Wrong number of arguments"  << endl;
        return ExitStatus::ARG_ERROR;
    }
    catch(out_of_range& e) {
        cout << "This is a bug, please report it (sg2ps@sg2ps.eu) together with the input files you used!\n";
        cout << e.what() << endl;
        return ExitStatus::BUG;
    }
    catch(logic_error& e) {
        cout << "A bug has been detected, please report it (sg2ps@sg2ps.eu)!\n";
        cout << e.what() << endl;
        return ExitStatus::LOGIC_ERROR;
    }
    catch(runtime_error& e) {
        cout << "Runtime error: " << e.what() << endl;
        return ExitStatus::RUNTIME_ERROR;
    }
    catch(exception& e) {
        cout << "Something went wrong, please report it (sg2ps@sg2ps.eu)!\n";
        cout << "std::exception: " << e.what() << endl;
        return ExitStatus::STD_EXCEPTION;
    }
    catch(...) {
        cout << "Unknown error, please report it (sg2ps@sg2ps.eu)!" << endl;
        return ExitStatus::UNKNOWN_ERROR;
    }

    return ExitStatus::OK;
}
