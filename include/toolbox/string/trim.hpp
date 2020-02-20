#ifndef TOOLBOX_STRING_TRIM_HPP
#define TOOLBOX_STRING_TRIM_HPP

#include <string_view>

namespace toolbox::string
{

template<class CharT, class Traits>
auto trim_left(std::basic_string_view<CharT, Traits> input)
{
    input.remove_prefix(
        std::min(input.find_first_not_of(" "), input.size())
    );

    return input;
}

template<class CharT, class Traits, class Allocator>
auto trim_left(std::basic_string<CharT, Traits, Allocator> &input)
{
    return trim_left(std::basic_string_view<CharT, Traits>{input.c_str(), input.size()});
}

template<class CharT, class Traits>
auto trim_right(std::basic_string_view<CharT, Traits> input)
{
    auto l = input.size() - input.find_last_not_of(" ") - 1;

    input.remove_suffix(std::min(l, input.size()));
    return input;
}

template<class CharT, class Traits, class Allocator>
auto trim_right(std::basic_string<CharT, Traits, Allocator> &input)
{
    return trim_right(std::basic_string_view<CharT, Traits>{input.c_str(), input.size()});
}

template<class CharT, class Traits>
auto trim(std::basic_string_view<CharT, Traits> input)
{
    return trim_left(trim_right(input));
}

template<class CharT, class Traits, class Allocator>
auto trim(std::basic_string<CharT, Traits, Allocator> &input)
{
    return trim(std::basic_string_view<CharT, Traits>{input.c_str(), input.size()});
}

} // namespace toolbox::string

#endif // TOOLBOX_STRING_TRIM_HPP