#include <toolbox/testing/test.h>
#include <toolbox/container/queue.h>
#include <toolbox/alloc/bitmap_allocator.hpp>

TEST_CASE(queue_basic_scenario)
{
    toolbox::queue<int, toolbox::bitmap_allocator<toolbox::queue_node<int>, 10>> q;

    q.push_back(1);
    q.push_back(2);
    q.push_back(3);

    CHECK(q.front() == 1);
    q.pop_front();
    CHECK(q.front() == 2);
    q.pop_front();
    CHECK(q.front() == 3);
    q.pop_front();
}

TEST_CASE(queue_rotate)
{
    toolbox::queue<int, toolbox::bitmap_allocator<toolbox::queue_node<int>, 10>> q;

    q.push_back(1);
    q.push_back(2);
    q.push_back(3);
    // 3 2 1
    q.rotate();
    // 1 3 2

    CHECK(q.front() == 2);
    q.pop_front();
    CHECK(q.front() == 3);
    q.pop_front();
    CHECK(q.front() == 1);
    q.pop_front();
}

TEST_CASE(queue_variadic_init)
{
    toolbox::queue<int, toolbox::bitmap_allocator<toolbox::queue_node<int>, 10>> q(1, 2, 3);

    CHECK(q.front() == 1);
    q.pop_front();
    CHECK(q.front() == 2);
    q.pop_front();
    CHECK(q.front() == 3);
    q.pop_front();
}

TEST_CASE(queue_remove_element)
{
    toolbox::queue<int, toolbox::bitmap_allocator<toolbox::queue_node<int>, 10>> q(1, 2, 3);

    q.remove(2);
    CHECK(q.front() == 1);
    q.remove(1);
    CHECK(q.front() == 3);
    q.remove(3);
    CHECK(q.size() == 0);
}