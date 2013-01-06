#include <string>
#include <vector>
#include "ReservedColumnNames.hpp"

using namespace std;

namespace {

// TODO If changes -> update the enum of columns appropriately
const string headers[] = {
		"DATA_ID",
		"GROUP_CODE",
		"COLOR_CODE",
		"LOCATION",
		"LOC_X",
		"LOC_Y",
		"FORMATION",
		"DATATYPE",
		"DIP_DIR",
		"DIP",
		"L_DIP_DIR",
		"L_DIP",
		"SENSE",
		"PALEONORTH",
		"COMMENT"
};

template <typename T, size_t n>
vector<T> from_array(const T (&array)[n]) {

	return vector<T>(array, array+n);
}

const vector<string> reserved_headers = from_array(headers);

}

const std::vector<std::string>& reserved_column_names() {

	return reserved_headers;
}
