#ifndef _CPP_TUPLE_AT_HPP_
#define _CPP_TUPLE_AT_HPP_

#include <toolbox/cpp/tuple_for_each.hpp>

namespace toolbox
{
namespace cpp
{

template<typename T, typename F>
F tuple_at(T &&tuple, std::size_t index, F &&func)
{
	std::size_t current = 0;

	tuple_for_each(
		tuple,
		[&index, &func, &current](auto &arg) {
			if (current == index) {
				func(arg);
			}

			current ++;
		}
	);

	return func;
}

} // namespace cpp
} // namespace toolbox

#endif
