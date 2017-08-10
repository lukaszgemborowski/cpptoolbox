#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cpp/tuple_for_each.hpp>

TEST_CASE("Iterate and sum", "[tuple_for_each][tuple]")
{
	auto sum = 0;

	cpp::tuple_for_each(std::make_tuple(1, 2, 3), [&sum](auto arg) { sum += arg; });

	REQUIRE(sum == 6);
}

TEST_CASE("Increment elements", "[tuple_for_each][tuple]")
{
	auto tuple = std::make_tuple(1, 2, 3);

	cpp::tuple_for_each(tuple, [](auto &arg) { arg ++; });

	REQUIRE(std::get<0>(tuple) == 2);
	REQUIRE(std::get<1>(tuple) == 3);
	REQUIRE(std::get<2>(tuple) == 4);
}

struct adder
{
	template<typename T>
	void operator()(const T& arg)
	{
		sum += static_cast<int>(arg);
	}

	int sum = 0;
};

TEST_CASE("Functor with state", "[tuple_for_each][tuple]")
{
	auto fun = cpp::tuple_for_each(std::make_tuple(1, 2, 3), adder{});

	REQUIRE(fun.sum == 6);
}
