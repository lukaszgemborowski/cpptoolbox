#include <toolbox/testing/test.h>
#include <sstream>
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

template<typename... Args>
auto printer(std::ostream &os, Args... args)
{
	toolbox::cpp::for_each_param(
		[&os](auto arg) {
			os << " -> " << arg << std::endl;
		},
		args...
	);
}

TEST_CASE(for_each_param_printer)
{
	std::stringstream ss;
	printer(ss, 1, "test", 1.5f);

	CHECK(ss.str() == " -> 1\n -> test\n -> 1.5\n");
}
