// Copyright (C) 2012, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef ASSERTIONS_HPP
#define ASSERTIONS_HPP

#include <stdexcept>
#include <sstream>

#ifdef __GNUG__
#define FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define FUNCTION_NAME __FUNCTION__
#endif

#define ASSERT_EQ(val_1, val_2) { \
	if (val_1!=val_2) { \
		std::ostringstream os; \
		os << #val_1 << " == " << #val_2 << " failed: "; \
		os << val_1 << " != " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw std::logic_error(os.str()); \
	} \
}

#endif
