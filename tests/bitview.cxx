#include "catch.hpp"
#include <toolbox/cpp/bitview.hpp>

TEST_CASE("set one bit", "[onebit]")
{
	using first_bit = toolbox::cpp::onebit<int, 0>;
	using second_bit = toolbox::cpp::onebit<int, 1>;
	using third_bit = toolbox::cpp::onebit<int, 2>;

	REQUIRE(first_bit::value == 1);
	REQUIRE(second_bit::value == 2);
	REQUIRE(third_bit::value == 4);
}

TEST_CASE("bit mask", "[mask]")
{
	using mask_three = toolbox::cpp::mask<int, 3>;
	using mask_four = toolbox::cpp::mask<int, 4>;

	REQUIRE(mask_three::value == 7);
	REQUIRE(mask_four::value == 0xf);
}

TEST_CASE("Slicing an int", "[bitview]")
{
	int value = 0;
	toolbox::cpp::bitview<int &> bv(value);
	auto slice = bv.slice<4, 4>();
	slice = 5;

	REQUIRE(value == 0x50);

	bv.slice<16, 8>() = 0xab;

	REQUIRE(value == 0xab0050);

	auto slice2 = bv.slice<8, 16>();
	REQUIRE(slice2.get() == 0xab00);
}
