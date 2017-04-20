// Copyright (C) 2012-2017, Ágoston Sasvári
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

const char MKDIR[] =
#ifdef _WIN32
        "mkdir ";
#else
        "mkdir -p ";
#endif

#endif
