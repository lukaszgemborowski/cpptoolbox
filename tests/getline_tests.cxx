#include <toolbox/testing/test.hpp>
#include <toolbox/string/line_iterator.hpp>
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
    auto it = toolbox::string::line_iterator{input};

    CHECK("one"s == it.get_string_view());
    ++ it;
    CHECK("two"s == it.get_string_view());
    ++ it;
    CHECK("three"s == it.get_string_view());
    ++ it;

    // check if end
    CHECK(it == toolbox::string::line_iterator{});
}

TEST_CASE(getline_iterator_empty_string)
{
    auto input = ""s;
    CHECK(toolbox::string::line_iterator{input} == toolbox::string::line_iterator{});
}
