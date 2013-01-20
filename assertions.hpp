// Copyright (C) 2012, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef ASSERTIONS_HPP
#define ASSERTIONS_HPP

#include <stdexcept>
#include <sstream>

// The FORTRAN traditions are followed
// EQ	==	EQuals
// LE   <=	Less or Equals
// LT   <	Less Than
// GE   >=	Greater or Equals


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

#define ASSERT(condition) { \
	if (!(condition)) { \
		std::ostringstream os; \
		os << #condition << " failed in \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw std::logic_error(os.str()); \
	} \
}

#define ASSERT_GE(val_1, val_2) { \
	if (val_1 < val_2) { \
		std::ostringstream os; \
		os << #val_1 << " >= " << #val_2 << " failed: "; \
		os << val_1 << " < " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw std::logic_error(os.str()); \
	} \
}

#define ASSERT_LE(val_1, val_2) { \
	if (val_1 > val_2) { \
		std::ostringstream os; \
		os << #val_1 << " <= " << #val_2 << " failed: "; \
		os << val_1 << " > " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw std::logic_error(os.str()); \
	} \
}

#define ASSERT_LT(val_1, val_2) { \
	if (val_1 >= val_2) { \
		std::ostringstream os; \
		os << #val_1 << " < " << #val_2 << " failed: "; \
		os << val_1 << " >= " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw std::logic_error(os.str()); \
	} \
}

#endif
