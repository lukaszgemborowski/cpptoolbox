#ifndef _CPP_TUPLE_FOR_EACH_HPP_
#define _CPP_TUPLE_FOR_EACH_HPP_

#include <tuple>
#include <type_traits>

namespace toolbox
{
namespace cpp
{

namespace detail
{
	template<typename Idx, typename Size, typename T, typename F>
	typename std::enable_if<Idx::value == Size::value, void>::type
	tuple_for_each(T &&, F &&) {}

	template<typename Idx, typename Size, typename T, typename F>
	typename std::enable_if<Idx::value < Size::value, void>::type
	tuple_for_each(T &&tuple, F &&func)
	{
		func(std::get<Idx::value>(tuple));
		detail::tuple_for_each<
			std::integral_constant<std::size_t, Idx::value + 1>,
			Size>
				(tuple, func);
	}
} // namespace detail

template<typename T, typename F>
F tuple_for_each(T &&tuple, F &&func)
{
	detail::tuple_for_each<
		std::integral_constant<std::size_t, 0>,
		typename std::tuple_size<typename std::decay<T>::type>::type>
			(tuple, func);

	return func;
}

} // namespace cpp
} // namespace toolbox

#endif
