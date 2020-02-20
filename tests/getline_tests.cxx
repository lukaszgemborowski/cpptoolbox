#include <toolbox/testing/test.hpp>
#include <toolbox/getline.hpp>
#include <iostream>

using namespace std::string_literals;

TEST_CASE(getline_no_newlines)
{
    auto input = "oneline"s;
    CHECK(input == toolbox::getline(input));
}

TEST_CASE(getline_one_endline)
{
    auto input = "oneline\n"s;
    CHECK("oneline"s == toolbox::getline(input));
}

TEST_CASE(getline_iterator)
{
    auto input = "one\ntwo\nthree"s;
    auto it = toolbox::string_line_iterator{input};

    CHECK("one"s == *it);
    ++ it;
    CHECK("two"s == *it);
    ++ it;
    CHECK("three"s == *it);
    ++ it;

    // check if end
    CHECK(it == toolbox::string_line_iterator{});
}

TEST_CASE(getline_iterator_empty_string)
{
    CHECK(toolbox::string_line_iterator{""s} == toolbox::string_line_iterator{});
}