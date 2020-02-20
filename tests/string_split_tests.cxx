#include <toolbox/testing/test.hpp>
#include <toolbox/string/split.hpp>
#include <toolbox/string/line_iterator.hpp>
#include <vector>

using namespace std::string_literals;

TEST_CASE(string_split_few_lines)
{
    auto input = "one\ntwo\nthree"s;
    auto result = toolbox::string::split_lines<std::vector>(input);

    CHECK(result.size() == 3);
    CHECK(result[0] == "one"s);
    CHECK(result[1] == "two"s);
    CHECK(result[2] == "three"s);
}