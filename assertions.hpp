// Copyright (C) 2012-2016, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef ASSERTIONS_HPP
#define ASSERTIONS_HPP

#include <algorithm>
#include <array>
#include <cmath>
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

#define ASSERT_NE(val_1, val_2) { \
    if (val_1==val_2) { \
        std::ostringstream os; \
        os << #val_1 << " != " << #val_2 << " failed: "; \
        os << val_1 << " == " << val_2 << '\n'; \
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
	if (val_1 <= val_2) { \
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

//------------------------------------------------------------------------------

template <size_t n>
int number_of_true_values(const bool (&array)[n]) {
    int count = 0;
    for (size_t i=0; i<n; ++i)
        if (array[i])
            ++count;
    return count;
}

template <size_t n>
int number_of_false_values(const bool (&array)[n]) {
    int count = 0;
    for (size_t i=0; i<n; ++i)
        if (!array[i])
            ++count;
    return count;
}

template <typename T, size_t n>
void print_values(std::ostream& os, const T (&array)[n]) {
    for (size_t i=0; i<n; ++i)
        os << array[i] << ", ";
}

#define ASSERT_EXACTLY_ONE_TRUE(...) { \
    bool arr[] = { __VA_ARGS__ } ; \
    int count = number_of_true_values(arr); \
    if (count!=1) { \
        std::ostringstream os; \
        os << "Expected exactly 1 true value but got " << count <<  " in \'" \
           << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ << '\n'; \
        os << "Arguments were: " << #__VA_ARGS__ << '\n' ; \
        os << "Their values:   " ; \
        print_values(os, arr); \
        throw_std_logic_error(os.str()); \
    } \
}

#define ASSERT_AT_LEAST_ONE_FALSE(...) { \
    bool arr[] = { __VA_ARGS__ } ; \
    int count = number_of_false_values(arr); \
    if (count < 1) { \
        std::ostringstream os; \
        os << "Expected at least 1 false value but got " << count <<  " in \'" \
           << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ << '\n'; \
        os << "Arguments were: " << #__VA_ARGS__ << '\n' ; \
        os << "Their values:   " ; \
        print_values(os, arr); \
        throw_std_logic_error(os.str()); \
    } \
}

#define ASSERT_AT_LEAST_ONE_TRUE(...) { \
    bool arr[] = { __VA_ARGS__ } ; \
    int count = number_of_true_values(arr); \
    if (count < 1) { \
        std::ostringstream os; \
        os << "Expected at least 1 true value but got " << count <<  " in \'" \
           << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ << '\n'; \
        os << "Arguments were: " << #__VA_ARGS__ << '\n' ; \
        os << "Their values:   " ; \
        print_values(os, arr); \
        throw_std_logic_error(os.str()); \
    } \
}

template <size_t n>
bool has_nan_or_inf(const double (&array)[n]) {
    for (size_t i=0; i<n; ++i)
        if (!std::isfinite(array[i]))
            return true;
    return false;
}

#define ASSERT_FINITE(...) { \
    double arr[] = { __VA_ARGS__ } ; \
    if (has_nan_or_inf(arr)) { \
        std::ostringstream os; \
        os << "NaN or infinity found in \'" \
           << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ << '\n'; \
        os << "Arguments were: " << #__VA_ARGS__ << '\n' ; \
        os << "Their values:   " ; \
        print_values(os, arr); \
        throw_std_logic_error(os.str()); \
    } \
}


template<typename... T>
std::array<typename std::common_type<T...>::type, sizeof...(T)>
make_array(T &&...t) {
    return {std::forward<T>(t)...};
}

#define ASSERT_ONE_OF(val, ...) { \
    auto arr = make_array( __VA_ARGS__ ); \
    bool not_found = std::find(begin(arr), end(arr), val) == end(arr); \
    if (not_found) { \
        std::ostringstream os; \
        os << "ASSERT_ONE_OF failed in \'" \
           << FUNCTION_NAME << "\' at "<< __FILE__ << ':' << __LINE__ << '\n'; \
        os << val << " not found in: " << #__VA_ARGS__ ; \
        throw_std_logic_error(os.str()); \
    } \
}

#endif
