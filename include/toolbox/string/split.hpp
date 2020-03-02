#ifndef TOOLBOX_STRING_SPLIT_HPP
#define TOOLBOX_STRING_SPLIT_HPP

#include <toolbox/string/line_iterator.hpp>
#include <string>
#include <string_view>

namespace toolbox::string
{

template<template <typename...> class Container, class Iterator>
auto split(Iterator it)
{
    auto e = Iterator {};
    Container<std::string> result;

    for (; it != e; ++it) {
        result.push_back(std::string{it.get_string_view()});
    }

    return result;
}

template<
    template <typename...> class Container,
    class String
>
auto split_lines(const String &str)
{
    return split<Container>(line_iterator{str});
}

} // namespace toolbox::string

#endif // TOOLBOX_STRING_SPLIT_HPP
