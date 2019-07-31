#include "toolbox/cpp/string.hpp"

namespace toolbox
{

std::string_view trim_left(std::string_view input)
{
    input.remove_prefix(
        std::min(input.find_first_not_of(" "), input.size())
    );

    return input;
}

std::string_view trim_right(std::string_view input)
{
    auto l = input.size() - input.find_last_not_of(" ") - 1;

    input.remove_suffix(std::min(l, input.size()));
    return input;
}

std::string_view trim(std::string_view input)
{
    return trim_left(trim_right(input));
}

} // namespace toolbox
