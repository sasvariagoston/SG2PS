// Copyright (C) 2012 - 2015 Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef ASSERTIONS_HPP
#define ASSERTIONS_HPP

#include <sstream>
#include <vector>

// The FORTRAN traditions are followed
// EQ	==	EQuals
// LE   <=	Less or Equals
// LT   <	Less Than
// GE   >=	Greater or Equals
// GT   >	Greater Than // FIXME Finish!
// CC approx. CirCa

void throw_std_logic_error(const std::string& message);

#ifdef __GNUG__
#define FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define FUNCTION_NAME __FUNCTION__
#endif

#define ASSERT(condition) { \
	if (!(condition)) { \
		std::ostringstream os; \
		os << #condition << " failed in \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw_std_logic_error(os.str()); \
	} \
}

#define ASSERT2(condition, message) { \
	if (!(condition)) { \
		std::ostringstream os; \
		os << #condition << " failed in \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ << '\n'; \
		os << message ; \
		throw_std_logic_error(os.str()); \
	} \
}

#define ASSERT_DEAD_END() { \
		std::ostringstream os; \
		os  << " Unexpected branch in \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ << '\n'; \
		throw_std_logic_error(os.str()); \
}

#define ASSERT_EQ(val_1, val_2) { \
	if (val_1!=val_2) { \
		std::ostringstream os; \
		os << #val_1 << " == " << #val_2 << " failed: "; \
		os << val_1 << " != " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw_std_logic_error(os.str()); \
	} \
}

#define ASSERT_GE(val_1, val_2) { \
	if (val_1 < val_2) { \
		std::ostringstream os; \
		os << #val_1 << " >= " << #val_2 << " failed: "; \
		os << val_1 << " < " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw_std_logic_error(os.str()); \
	} \
}

#define ASSERT_LE(val_1, val_2) { \
	if (val_1 > val_2) { \
		std::ostringstream os; \
		os << #val_1 << " <= " << #val_2 << " failed: "; \
		os << val_1 << " > " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw_std_logic_error(os.str()); \
	} \
}

#define ASSERT_GT(val_1, val_2) { \
	if (val_1 >= val_2) { \
		std::ostringstream os; \
		os << #val_1 << " < " << #val_2 << " failed: "; \
		os << val_1 << " >= " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw_std_logic_error(os.str()); \
	} \
}

#define ASSERT_LT(val_1, val_2) { \
	if (val_1 >= val_2) { \
		std::ostringstream os; \
		os << #val_1 << " < " << #val_2 << " failed: "; \
		os << val_1 << " >= " << val_2 << '\n'; \
		os << "In \'" << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ ; \
		throw_std_logic_error(os.str()); \
	} \
}

#endif
