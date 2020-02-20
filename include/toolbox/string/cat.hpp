#ifndef TOOLBOX_STRING_CAT_HPP
#define TOOLBOX_STRING_CAT_HPP

#include <initializer_list>
#include <sstream>
#include <string_view>
#include <string>

namespace toolbox::string
{

inline std::string concatenate(const std::initializer_list<std::string_view> &list)
{
    std::stringstream ss;

    for (const auto &s : list) {
        ss << s;
    }

    return ss.str();
}

inline std::string concatenate(const std::initializer_list<std::string> &list)
{
    std::stringstream ss;

    for (const auto &s : list) {
        ss << s;
    }

    return ss.str();
}

inline const std::string& concatenate(const std::string &s)
{
    return s;
}

template<class... Args>
std::string concatenate(const Args&... args)
{
    return (concatenate(args) + ...);
}

} // namespace toolbox::string

#endif