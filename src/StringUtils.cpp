#include "StringUtils.h"
#include <algorithm>

namespace StringUtils {
	std::string ToLower(std::string str) {

		std::transform(std::begin(str), std::end(str), std::begin(str),
			[](unsigned char c) { return std::tolower(c);});

		return str;
	}
}
