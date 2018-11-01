#include <toolbox/testing/test.h>
#include <toolbox/cpp/for_each_param.hpp>

template<typename... Args>
auto sum_args(Args... args)
{
	auto result = 0;
	toolbox::cpp::for_each_param(
		[&result](auto arg) {
			result += arg;
		},
		args...
	);

	return result;
}

TEST_CASE(for_each_param_example_usage)
{
	auto a = sum_args(1, 2, 3);
	auto b = sum_args(4, 5, 6);

	CHECK(a == 6);
	CHECK(b == 15);
}
