#ifndef TOOLBOX_GETLINE_HPP
#define TOOLBOX_GETLINE_HPP

#include <string_view>
#include <string>

namespace toolbox
{

template<
    class CharT,
    class Traits,
    class Allocator>
std::basic_string_view<CharT, Traits> getline(const std::basic_string<CharT, Traits, Allocator> &text)
{
    using string_t = std::basic_string<CharT, Traits, Allocator>;

    if (auto pos = text.find(CharT{'\n'}); pos == string_t::npos) {
        return {text};
    } else {
        return {text.c_str(), pos};
    }
}

} // namespace toolbox

#endif // TOOLBOX_GETLINE_HPP