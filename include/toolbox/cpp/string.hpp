#ifndef TOOLBOX_CPP_STRING_HPP
#define TOOLBOX_CPP_STRING_HPP

#include <string_view>

namespace toolbox
{

std::string_view trim_left(std::string_view input);
std::string_view trim_right(std::string_view input);
std::string_view trim(std::string_view input);

} // namespace toolbox

#endif // TOOLBOX_CPP_STRING_HPP
