#ifndef _TOOLBOX_CPP_SIZEOF_TUPLE_HPP_
#define _TOOLBOX_CPP_SIZEOF_TUPLE_HPP_

#include <type_traits>
#include <tuple>

namespace toolbox
{
namespace cpp
{

template<typename T> struct sizeof_tuple;

template<>
struct sizeof_tuple<std::tuple<>> : std::integral_constant<std::size_t, 0> {};

template<typename Head, typename... Tail>
struct sizeof_tuple<std::tuple<Head, Tail...>> :
	std::integral_constant<std::size_t,
		sizeof(Head) + sizeof_tuple<std::tuple<Tail...>>::value> {
};

} // namespace cpp
} // namespace toolbox

#endif
