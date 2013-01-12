// Copyright (C) 2013, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.
#include <fstream>
#include <sstream>
#include "read_csv.hpp"

using namespace std;

typedef vector<vector<string> > str_table;

namespace {

void push_to_table(str_table& table, const string& line) {

	vector<string> row;

	istringstream iss(line);

	string cell;

	while (getline(iss, cell, '\t')) {

		row.push_back(cell);
	}

	table.push_back(row);
}

}

size_t read_csv(const string& file_name, str_table& table) {

	table.clear();

	size_t lines_read = 0;

	ifstream in(file_name.c_str());

	string line;

	while (getline(in, line)) {

		++lines_read;

		push_to_table(table, line+'\t'); // so that we don't loose trailing tabs

	}

	return lines_read;
}
