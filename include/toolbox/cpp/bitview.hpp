#ifndef _CPP_BITVIEW_HPP_
#define _CPP_BITVIEW_HPP_

namespace toolbox
{
namespace cpp
{

template<typename IntT, std::size_t I>
using onebit = std::integral_constant<IntT, 1 << I>;

template<typename IntT, std::size_t N>
struct mask : public std::integral_constant<
	IntT, mask<IntT, N - 1>::value | onebit<IntT, N - 1>::value> {};

template<typename IntT>
struct mask<IntT, 0> : public std::integral_constant<IntT, 1> {};


namespace detail
{

template<typename IntT, std::size_t Offset, std::size_t Size>
class bitview_slice
{
public:
	bitview_slice(IntT &value) :
		value_ (value)
	{}

	void operator=(const IntT &v)
	{
		set(v);
	}

	void set(const IntT &v)
	{
		// clear field
		value_ = value_ & ~(mask<IntT, Size>::value << Offset);

		// set new value
		value_ |= (v & mask<IntT, Size>::value) << Offset;
	}

	IntT get() const
	{
		return (value_ >> Offset) & mask<IntT, Size>::value;
	}

private:
	IntT &value_;
};

} // namespace detail

template<typename IntRefT>
class bitview
{
public:
	bitview(IntRefT value) :
		value_ (value)
	{}

	template<std::size_t Offset, std::size_t Size>
	auto slice()
	{
		return detail::bitview_slice<
			typename std::remove_reference<IntRefT>::type, Offset, Size>(value_);
	}

	typename std::remove_reference<IntRefT>::type value() const
	{
		return value_;
	}

private:
	IntRefT value_;
};

} // namespace cpp
} // namespace toolbox

#endif
