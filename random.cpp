// Copyright (C) 2012, 2013 Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

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
