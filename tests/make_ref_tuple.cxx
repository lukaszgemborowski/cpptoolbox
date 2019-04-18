#include <toolbox/testing/test.hpp>
#include <toolbox/cpp/make_ref_tuple.hpp>

TEST_CASE(make_ref_tuple_tuple_of_references)
{
	auto foo = 1, bar = 2;

	auto t = toolbox::cpp::make_ref_tuple(foo, bar);

	CHECK(std::get<0>(t) == 1);
	CHECK(std::get<1>(t) == 2);

	foo = 2;
	bar = 1;

	CHECK(std::get<0>(t) == 2);
	CHECK(std::get<1>(t) == 1);
}

TEST_CASE(make_ref_tuple_tuple_of_references_and_copies)
{
	auto foo = 1;
	auto t = toolbox::cpp::make_ref_tuple(foo, 5);

	CHECK(std::get<0>(t) == 1);
	CHECK(std::get<1>(t) == 5);

	foo = 2;

	CHECK(std::get<0>(t) == 2);
}
