#include <toolbox/testing/test.hpp>
#include <toolbox/container/tuple.hpp>

TEST_CASE(tuple_default_initialize)
{
    toolbox::tuple<int, float> t;

    CHECK(toolbox::get<0>(t) == 0);
    CHECK(toolbox::get<1>(t) == 0.f);
}

TEST_CASE(tuple_ctor_initialize)
{
    toolbox::tuple<int, float> t(42, 42.f);

    CHECK(toolbox::get<0>(t) == 42);
    CHECK(toolbox::get<1>(t) == 42.f);
}

TEST_CASE(tuple_ctor_does_one_copy)
{
    struct cpy {
        int cnt = 0;
        cpy() = delete;
        explicit cpy(int value)
            : cnt (value) {}
        cpy(const cpy &c)
            : cnt (c.cnt + 1) {}
    };

    toolbox::tuple<cpy> e(cpy(0));

    CHECK(toolbox::get<0>(e).cnt == 1);
}
