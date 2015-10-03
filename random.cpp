// Copyright (C) 2012 - 2015 Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifdef __GNUG__
#include <tr1/random>
#include "random.hpp"

using namespace std;

using namespace std::tr1;

namespace {

	const unsigned long SEED = 5489UL;

	mt19937  mt(SEED);

	const double MAX_RAND = mt.max();
}

void uniform_generator_reset() {

	mt.seed(SEED);
}

double uniform_0_1() {

    return mt()/MAX_RAND;
}
#elif _MSC_VER && !__INTEL_COMPILER
#include <cstdlib>
using namespace std;

void uniform_generator_reset() {

	srand(31);
}

double uniform_0_1() {

    return ((double) rand() / (RAND_MAX)); // Horrible hack
}

#else
#error "Check the random number generation on your platform, try to use mt19937!"
#endif
