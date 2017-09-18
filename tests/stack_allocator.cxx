#include "catch.hpp"
#include <toolbox/alloc/stack_allocator.hpp>
#include <iostream>

TEST_CASE("simple functionality", "[alloc][stack_allocator]")
{
	toolbox::alloc::stack_allocator<int, 10> alloc;

	int *p1 = alloc.allocate(1);
	int *p2 = alloc.allocate(1);
	int *p3 = alloc.allocate(8);

	REQUIRE(p2 - p1 == 1);
	REQUIRE(p3 != nullptr);
	REQUIRE_THROWS(alloc.allocate(1));
	REQUIRE(alloc.available() == 0);

	alloc.deallocate(p1, 1);
	REQUIRE(alloc.available() == 0);

	alloc.deallocate(p3, 8);
	REQUIRE(alloc.available() == 8);
}

TEST_CASE("test with vector", "[alloc][stack_allocator][vector]")
{
	toolbox::alloc::stack_allocator<float, 5> alloc;
	std::vector<float, decltype(alloc)> vec(alloc);

	vec.push_back(1);
	vec.push_back(1);
}
