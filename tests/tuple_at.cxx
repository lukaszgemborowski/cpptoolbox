#include "catch.hpp"
#include <cpp/tuple_at.hpp>

TEST_CASE("Manipulate element at index", "[tuple_at][tuple]")
{
	auto tuple = std::make_tuple(1, 2, 3);
	cpp::tuple_at(
		tuple,
		1,
		[](auto &arg) {
			arg = arg * 2;
		}
	);

	REQUIRE(std::get<1>(tuple) == 4);
}
