#ifndef _TOOLBOX_CPP_MAKE_REF_TUPLE_HPP_
#define _TOOLBOX_CPP_MAKE_REF_TUPLE_HPP_

#include <tuple>

namespace toolbox
{
namespace cpp
{

template<typename Head, typename... Tail>
auto make_ref_tuple(Head &&head, Tail&&... tail)
{
	// head is rvalue ref
	auto head_tuple = std::tuple<Head>(std::move(head));
	auto tail_tuple = make_ref_tuple(tail...);

	return std::tuple_cat(head_tuple, tail_tuple);
}

template<typename T>
auto make_ref_tuple(T &object)
{
	return std::tuple<T &>(object);
}

template<typename Head, typename... Tail>
auto make_ref_tuple(Head &head, Tail&&... tail)
{
	// head is lvalue ref
	auto head_tuple = std::tuple<Head &>(head);
	auto tail_tuple = make_ref_tuple(tail...);

	return std::tuple_cat(head_tuple, tail_tuple);
}

}
}

#endif
