#include <toolbox/testing/test.h>
#include <toolbox/container/array.hpp>

TEST_CASE(array_default_initialization)
{
    const int default_value = 42;
    constexpr int size = 5;
    struct test_struct {
        const int value = default_value;

        test_struct() = default;

        // unsure no copies
        test_struct(const test_struct &) = delete;
        void operator=(const test_struct &) = delete;
    };

    toolbox::array<test_struct, size> arr;

    CHECK(size == arr.size());

    // test range for loop over elements
    for (auto &el : arr) {
        CHECK(el.value == default_value);
    }

    // test at() and operator[]
    for (int i = 0; i < size; i ++) {
        CHECK(arr.at(i).value == default_value);
        CHECK(arr[i].value == default_value);
    }
}