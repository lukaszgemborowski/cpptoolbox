#include "catch.hpp"
#include <toolbox/cpp/named_type.hpp>

using MyInt = cpptoolbox::cpp::named_type<int, struct MyIntTag>;

int foo(MyInt a)
{
    return 2 * a;
}

TEST_CASE("call function with named_type", "[cpp][named_type]")
{
    REQUIRE(foo(MyInt(42)) == 84);
}