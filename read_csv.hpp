
// Copyright (C) 2012 - 2015 Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#ifndef READ_CSV_HPP
#define READ_CSV_HPP

#include <string>
#include <vector>

// Returns the number of lines read ( == table.size() )
std::size_t read_csv(const std::string& file_name, std::vector<std::vector<std::string> >& table);

#endif
