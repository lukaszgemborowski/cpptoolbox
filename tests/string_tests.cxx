#include <toolbox/testing/test.hpp>
#include <toolbox/string/cat.hpp>
#include <toolbox/string/trim.hpp>
#include <string>

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST_CASE(string_trim_left_no_whitespace)
{
    auto input = "text"s;
    auto result = std::string{toolbox::string::trim_left(input)};

    CHECK(input == result);
}

TEST_CASE(string_trim_left_one_space)
{
    auto input = " text"s;
    auto result = std::string{toolbox::string::trim_left(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_right_no_whitespace)
{
    auto input = "text"s;
    auto result = std::string{toolbox::string::trim_right(input)};

    CHECK(input == result);
}

TEST_CASE(string_trim_right_one_space)
{
    auto input = "text "s;
    auto result = std::string{toolbox::string::trim_right(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_no_whitespace)
{
    auto input = "text"s;
    auto result = std::string{toolbox::string::trim(input)};

    CHECK(input == result);
}

TEST_CASE(string_trim_on_space_left)
{
    auto input = " text"s;
    auto result = std::string{toolbox::string::trim(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_on_space_right)
{
    auto input = "text "s;
    auto result = std::string{toolbox::string::trim(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_on_space_both_sides)
{
    auto input = " text "s;
    auto result = std::string{toolbox::string::trim(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_concatenate_strings)
{
    auto r = toolbox::string::cat("a"s, "b"s, "c"s);
    CHECK("abc"s == r);
}

TEST_CASE(string_concatenate_init_list_strings)
{
    auto r = toolbox::string::cat( {"a"s, "b"s, "c"s} );
    CHECK("abc"s == r);
}

TEST_CASE(string_concatenate_init_list_str_view)
{
    auto r = toolbox::string::cat( {"a"sv, "b"sv, "c"sv} );
    CHECK("abc"s == r);
}

TEST_CASE(string_concatenate_several_types)
{
    auto ils = std::initializer_list<std::string> {
        "c"s, "d"s
    };

    auto ilsv = std::initializer_list<std::string_view> {
        "e"sv, "f"sv
    };

    auto r = toolbox::string::cat( "a"s, "b"s, ils, ilsv );
    CHECK("abcdef"s == r);
}
