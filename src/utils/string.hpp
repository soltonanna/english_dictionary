// Headers from standard library
#include <string>
#include <algorithm>
#include <iostream>

namespace StringUtils {
    inline void eraseFirstAndLastProbels (std::string& dst) {
        const auto f = std::find_if(dst.begin(), dst.end(), [](const char& a) { return a != ' '; });
        const auto l = std::find_if(dst.rbegin(), dst.rend(), [](const char& a) { return a != ' '; }).base();
        dst = (l - f > 0) ? dst = std::string(f, l) : "";
    }
};
