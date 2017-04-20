// Copyright (C) 2012-2017, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <vector>

template <typename T, size_t n>
std::vector<T> from_array(const T (&array)[n]) {

	return std::vector<T>(array, array+n);
}
