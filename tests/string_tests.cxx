#include <toolbox/testing/test.hpp>
#include <toolbox/cpp/string.hpp>

using namespace std::string_literals;

TEST_CASE(string_trim_left_no_whitespace)
{
    auto input = "text"s;
    auto result = std::string{toolbox::trim_left(input)};

    CHECK(input == result);
}

TEST_CASE(string_trim_left_one_space)
{
    auto input = " text"s;
    auto result = std::string{toolbox::trim_left(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_right_no_whitespace)
{
    auto input = "text"s;
    auto result = std::string{toolbox::trim_right(input)};

    CHECK(input == result);
}

TEST_CASE(string_trim_right_one_space)
{
    auto input = "text "s;
    auto result = std::string{toolbox::trim_right(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_no_whitespace)
{
    auto input = "text"s;
    auto result = std::string{toolbox::trim(input)};

    CHECK(input == result);
}

TEST_CASE(string_trim_on_space_left)
{
    auto input = " text"s;
    auto result = std::string{toolbox::trim(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_on_space_right)
{
    auto input = "text "s;
    auto result = std::string{toolbox::trim(input)};

    CHECK("text"s == result);
}

TEST_CASE(string_trim_on_space_both_sides)
{
    auto input = " text "s;
    auto result = std::string{toolbox::trim(input)};

    CHECK("text"s == result);
}
