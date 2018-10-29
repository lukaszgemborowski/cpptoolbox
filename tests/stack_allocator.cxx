#include <toolbox/testing/test.h>
#include <toolbox/alloc/stack_allocator.hpp>
#include <vector>

TEST_CASE(stack_allocator_simple_functionality)
{
	toolbox::alloc::stack_allocator<int, 10> alloc;

	int *p1 = alloc.allocate(1);
	int *p2 = alloc.allocate(1);
	int *p3 = alloc.allocate(8);

	CHECK(p2 - p1 == 1);
	CHECK(p3 != nullptr);
	//CHECK_THROWS(alloc.allocate(1));
	CHECK(alloc.available() == 0);

	alloc.deallocate(p1, 1);
	CHECK(alloc.available() == 0);

	alloc.deallocate(p3, 8);
	CHECK(alloc.available() == 8);
}

TEST_CASE(stack_allocator_test_with_vector)
{
	toolbox::alloc::stack_allocator<float, 5> alloc;
	std::vector<float, decltype(alloc)> vec(alloc);

	vec.push_back(1);
	vec.push_back(1);
}
