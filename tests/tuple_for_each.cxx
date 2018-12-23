#include <toolbox/testing/test.h>
#include <toolbox/cpp/tuple_for_each.hpp>

TEST_CASE(tuple_for_each_iterate_and_sum)
{
	auto sum = 0;

	toolbox::cpp::tuple_for_each(std::make_tuple(1, 2, 3), [&sum](auto arg) { sum += arg; });

	CHECK(sum == 6);
}

TEST_CASE(tuple_for_each_increament_elements)
{
	auto tuple = std::make_tuple(1, 2, 3);

	toolbox::cpp::tuple_for_each(tuple, [](auto &arg) { arg ++; });

	CHECK(std::get<0>(tuple) == 2);
	CHECK(std::get<1>(tuple) == 3);
	CHECK(std::get<2>(tuple) == 4);
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

TEST_CASE(tuple_for_each_functor_with_state)
{
	auto fun = toolbox::cpp::tuple_for_each(std::make_tuple(1, 2, 3), adder{});

	CHECK(fun.sum == 6);
}

struct int_doubler
{
	template<typename T, typename U, typename R = void>
	using enable_for_type_t = typename std::enable_if<std::is_same<T, U>::value>::type;
	template<typename T, typename U, typename R = void>
	using disable_for_type_t = typename std::enable_if<!std::is_same<T, U>::value>::type;

	template<typename T>
	enable_for_type_t<T, int> operator()(T &arg)
	{
		arg *= 2;
	}

	template<typename T>
	disable_for_type_t<T, int> operator()(T &) {}
};

TEST_CASE(tuple_for_each_selective_modification)
{
	auto tuple = std::make_tuple(1, 2, 3.0, 4.0, 5.f, 6.f);

	toolbox::cpp::tuple_for_each(tuple, int_doubler{});

	CHECK(std::get<0>(tuple) == 2);
	CHECK(std::get<1>(tuple) == 4);
	CHECK(std::get<2>(tuple) == 3.0);
	CHECK(std::get<3>(tuple) == 4.0);
	CHECK(std::get<4>(tuple) == 5.f);
	CHECK(std::get<5>(tuple) == 6.f);
}
