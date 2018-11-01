#ifndef _TOOLBOX_CPP_INT_TYPE_HPP_H_
#define _TOOLBOX_CPP_INT_TYPE_HPP_H_

#include <toolbox/std/type_traits.hpp>
#include <toolbox/std/cstdint.hpp>

namespace toolbox
{
namespace cpp
{

template<std::size_t N> struct int_type
{
	template<bool C, typename T, typename F>
	using cond = typename std::conditional<C,T,F>::type;

	using signed_type = 
		cond<N <= 8, std::int8_t,
			cond<N <= 16, std::int16_t,
				cond<N <= 32, std::int32_t,
					cond<N <= 64, std::int64_t, std::false_type>
				>
			>
		>;

	static_assert(!std::is_same<signed_type, std::false_type>::value,
		"wrong N, outside [1, 64] range");

	// TODO: implement make_unsigned in type_traits
	//using unsigned_type = typename std::make_unsigned<signed_type>::type;
};

} // namespace cpp
} // namespace toolbox

#endif
