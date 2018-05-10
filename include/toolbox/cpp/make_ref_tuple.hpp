#ifndef _TOOLBOX_CPP_MAKE_REF_TUPLE_HPP_
#define _TOOLBOX_CPP_MAKE_REF_TUPLE_HPP_

#include <tuple>

namespace toolbox
{
namespace cpp
{

template<typename T>
auto make_ref_tuple(T &&element)
{
	return std::tuple<T>(std::forward<T>(element));
}

template<typename Head, typename... Tail>
auto make_ref_tuple(Head &&head, Tail&&... tail)
{
	auto head_tuple = std::tuple<Head>(std::forward<Head>(head));
	auto tail_tuple = make_ref_tuple(std::forward<Tail>(tail)...);

	return std::tuple_cat(head_tuple, tail_tuple);
}

}
}

#endif
