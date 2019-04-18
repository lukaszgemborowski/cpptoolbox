#include <toolbox/testing/test.hpp>
#include <toolbox/cpp/named_type.hpp>

using MyInt = cpptoolbox::cpp::named_type<int, struct MyIntTag>;

int foo(MyInt a)
{
    return 2 * a;
}

TEST_CASE(named_type_call_function_with_named_type)
{
    CHECK(foo(MyInt(42)) == 84);
}
