#include <toolbox/testing/test.h>
#include <toolbox/container/stack.hpp>

TEST_CASE(stack_empty_default_init)
{
    toolbox::stack<int, 10> stack;

    CHECK(stack.size() == 0);
    CHECK(stack.capacity() == 10);
}

TEST_CASE(stack_push_back_one_value)
{
    struct copy_constructible {
        copy_constructible() = delete;
        explicit copy_constructible(int v) : value(v) {}
        copy_constructible(const copy_constructible &o) : value(o.value) {}

        int value;
    };
    toolbox::stack<copy_constructible, 10> stack;
    stack.push_back(copy_constructible(42));

    CHECK(stack.size() == 1);
    CHECK(stack.back().value == 42);
}

TEST_CASE(stack_emplace_back_one_value)
{
    struct non_copyable {
        non_copyable() = delete;
        explicit non_copyable(int v) : value(v) {}
        non_copyable(const non_copyable &) = delete;
        void operator=(const non_copyable &) = delete;

        int value;
    };
    toolbox::stack<non_copyable, 10> stack;
    stack.emplace_back(42);

    CHECK(stack.size() == 1);
    CHECK(stack.back().value == 42);
}

TEST_CASE(stack_push_and_pop)
{
    toolbox::stack<int, 10> stack;
    stack.emplace_back(42);
    stack.emplace_back(43);
    stack.emplace_back(44);

    CHECK(stack.size() == 3);
    CHECK(stack.back() == 44);
}