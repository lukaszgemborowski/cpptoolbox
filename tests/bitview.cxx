#include <toolbox/testing/test.hpp>
#include <toolbox/cpp/bitview.hpp>
#include <cstddef>

TEST_CASE(bitview_set_one_bit)
{
	using first_bit = toolbox::cpp::onebit<int, 0>;
	using second_bit = toolbox::cpp::onebit<int, 1>;
	using third_bit = toolbox::cpp::onebit<int, 2>;

	CHECK(first_bit::value == 1);
	CHECK(second_bit::value == 2);
	CHECK(third_bit::value == 4);
}

TEST_CASE(bitview_bit_mask)
{
	using mask_three = toolbox::cpp::mask<int, 3>;
	using mask_four = toolbox::cpp::mask<int, 4>;

	CHECK(mask_three::value == 7);
	CHECK(mask_four::value == 0xf);
}

TEST_CASE(bitview_slicing_an_int)
{
	int value = 0;
	toolbox::cpp::bitview<int &> bv(value);
	auto slice = bv.slice<4, 4>();
	slice = 5;

	CHECK(value == 0x50);

	bv.slice<16, 8>() = 0xab;

	CHECK(value == 0xab0050);

	auto slice2 = bv.slice<8, 16>();
	CHECK(slice2.get() == 0xab00);
}
