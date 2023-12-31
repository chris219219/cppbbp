#include "../inc/util.hpp"

#include <string>

static bool ends_with(const std::string& str, const std::string& suffix)
{
    return
        str.size() >= suffix.size()
        && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}