#ifndef _CPP_CALLSEQUENCE_HPP_
#define _CPP_CALLSEQUENCE_HPP_

#include <tuple>

namespace toolbox
{
namespace cpp
{

template<typename... Functors>
class call_seq
{
public:
	call_seq(Functors... functors) :
		functors_(functors...)
	{}

	template<typename... Args>
	void operator()(Args&&... args)
	{
		exec<0>(args...);
	}

private:
	template<std::size_t I, typename... Args>
	std::enable_if_t<I < sizeof...(Functors) - 1, void> exec(Args&&... args)
	{
		std::get<I>(functors_)(args...);
		exec<I + 1>(args...);
	}

	template<std::size_t I, typename... Args>
	std::enable_if_t<I == sizeof...(Functors) - 1, void> exec(Args&&... args)
	{
		std::get<I>(functors_)(args...);
	}

private:
	std::tuple<Functors...> functors_;
};

template<typename... Functors>
auto make_call_seq(Functors... functors)
{
	return call_seq<Functors...>(functors...);
}

} // namespace cpp
} // namespace toolbox

#endif
