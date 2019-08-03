#ifndef TOOLBOX_CPP_STRING_HPP
#define TOOLBOX_CPP_STRING_HPP

#include <string_view>
#include <string>

namespace toolbox
{

std::string_view trim_left(std::string_view input);
std::string_view trim_right(std::string_view input);
std::string_view trim(std::string_view input);

std::string concatenate(const std::initializer_list<std::string_view> &list);
std::string concatenate(const std::initializer_list<std::string> &list);

inline const std::string& concatenate(const std::string &s)
{
    return s;
}

template<class... Args>
std::string concatenate(const Args&... args)
{
    return (concatenate(args) + ...);
}

} // namespace toolbox

#endif // TOOLBOX_CPP_STRING_HPP
