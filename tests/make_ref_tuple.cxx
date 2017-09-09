#include "catch.hpp"
#include <toolbox/cpp/make_ref_tuple.hpp>

TEST_CASE("tuple of references", "[make_ref_tuple][tuple]")
{
	auto foo = 1, bar = 2;

	auto t = toolbox::cpp::make_ref_tuple(foo, bar);

	REQUIRE(std::get<0>(t) == 1);
	REQUIRE(std::get<1>(t) == 2);

	foo = 2;
	bar = 1;

	REQUIRE(std::get<0>(t) == 2);
	REQUIRE(std::get<1>(t) == 1);
}

TEST_CASE("tuple of references and copies", "[make_ref_tuple][tuple]")
{
	auto foo = 1;
	auto t = toolbox::cpp::make_ref_tuple(foo, 5);

	REQUIRE(std::get<0>(t) == 1);
	REQUIRE(std::get<1>(t) == 5);

	foo = 2;

	REQUIRE(std::get<0>(t) == 2);
}
