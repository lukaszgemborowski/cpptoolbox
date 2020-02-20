#ifndef TOOLBOX_STRING_CAT_HPP
#define TOOLBOX_STRING_CAT_HPP

#include <initializer_list>
#include <sstream>
#include <string_view>
#include <string>

namespace toolbox::string
{

template<class CharT, class Traits>
std::basic_string<CharT, Traits> concatenate(
    const std::initializer_list<std::basic_string_view<CharT, Traits>> &list)
{
    std::basic_stringstream<CharT, Traits> ss;

    for (const auto &s : list) {
        ss << s;
    }

    return ss.str();
}

template<class CharT, class Traits, class Allocator>
std::basic_string<CharT, Traits> concatenate(
    const std::initializer_list<std::basic_string<CharT, Traits, Allocator>> &list)
{
    std::basic_stringstream<CharT, Traits> ss;

    for (const auto &s : list) {
        ss << s;
    }

    return ss.str();
}

template<class CharT, class Traits, class Allocator>
const std::basic_string<CharT, Traits>& concatenate(
    const std::basic_string<CharT, Traits, Allocator> &s)
{
    return s;
}

// TODO: result type will be wrong for any other type than char.
template<class... Args>
std::string concatenate(const Args&... args)
{
    return (concatenate(args) + ...);
}

} // namespace toolbox::string

#endif