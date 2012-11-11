// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef PLATFORM_DEP_HPP_
#define PLATFORM_DEP_HPP_


const char path_separator[] =
#ifdef _WIN32
		"\\";
#else
		"/";
#endif

#endif
