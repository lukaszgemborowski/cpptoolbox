#include "toolbox/cpp/string.hpp"
#include <sstream>

namespace toolbox
{

std::string concatenate(const std::initializer_list<std::string_view> &list)
{
    std::stringstream ss;

    for (const auto &s : list) {
        ss << s;
    }

    return ss.str();
}

std::string concatenate(const std::initializer_list<std::string> &list)
{
    std::stringstream ss;

    for (const auto &s : list) {
        ss << s;
    }

    return ss.str();
}

} // namespace toolbox
