#include <toolbox/testing/test.h>
#include <toolbox/alloc/bitmap_allocator.hpp>

TEST_CASE(bitmap_allocatorallocate_one_object)
{
    struct T {
        bool initialized = true;
    };

    toolbox::bitmap_allocator<T, 1> alloc;

    CHECK(alloc.space_available());

    auto &obj = alloc.allocate();

    CHECK(obj.initialized == true);
    CHECK(alloc.space_available() == false);
}

TEST_CASE(bitmap_allocator_unique_addresses)
{
    constexpr auto number = 20;
    int* addrs[number] = {0};
    toolbox::bitmap_allocator<int, number> alloc;

    for (int i = 0; i < number; i ++) {
        addrs[i] = &alloc.allocate(42);

        for (int j = 0; j < i; j ++) {
            CHECK(addrs[j] != addrs[i]);
        }
    }

    CHECK(alloc.space_available() == false);
}

TEST_CASE(bitmap_allocator_deallocate_all)
{
    constexpr auto number = 20;
    static int count = 0;
    struct T {
        T() { count ++; }
        ~T() { count --; }
    };
    toolbox::bitmap_allocator<T, number> alloc;
    T* arr[number];

    for(int i = 0; i < number; i ++) {
        arr[i] = &alloc.allocate();
    }

    CHECK(count == 20);

    for (int i = 0; i < number; i ++) {
        alloc.deallocate(*arr[i]);
    }

    CHECK(count == 0);
}

TEST_CASE(bitmap_allocator_deallocate_random)
{
    constexpr auto number = 20;
    static int count = 0;
    struct T {
        T() { count ++; }
        ~T() { count --; }
    };
    toolbox::bitmap_allocator<T, number> alloc;
    T* arr[number];

    for(int i = 0; i < number; i ++) {
        arr[i] = &alloc.allocate();
    }

    CHECK(count == 20);
    CHECK(alloc.allocated_count() == 20);

    // bitmap_allocator is mapping allocations on 8bit variables
    // therefore we can check deallocation on 8bit boundary

    for (int i = 0; i < 8; i ++) {
        alloc.deallocate(*arr[i]);
    }

    CHECK(count == 12);
    CHECK(alloc.allocated_count() == 12);

    // remove last 4
    for (int i = 16; i < 20; i++)
        alloc.deallocate(*arr[i]);

    CHECK(count == 8);
    CHECK(alloc.allocated_count() == 8);

    // we should be able to allocate 12 more objects
    for (int i = 0; i < 12; i ++) {
        auto &ptr = alloc.allocate();
        CHECK(alloc.own(ptr));
        CHECK(alloc.allocated_count() == (9 + i));
    }
}
