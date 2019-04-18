#include <toolbox/testing/test.hpp>
#include <toolbox/cpp/tuple_at.hpp>

TEST_CASE(tuple_at_manipulate_element_at)
{
	auto tuple = std::make_tuple(1, 2, 3);
	toolbox::cpp::tuple_at(
		tuple,
		1,
		[](auto &arg) {
			arg = arg * 2;
		}
	);

	CHECK(std::get<1>(tuple) == 4);
}
