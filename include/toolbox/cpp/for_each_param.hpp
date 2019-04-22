#ifndef _TOOLBOX_CPP_FOR_EACH_PARAM_HPP_
#define _TOOLBOX_CPP_FOR_EACH_PARAM_HPP_

namespace toolbox
{
namespace cpp
{

namespace detail
{

template<typename Func, typename Last>
void for_each_param(Func &&func, Last &&last)
{
    func(last);
}

template<typename Func, typename First, typename... Params>
void for_each_param(Func &&func, First &&first, Params&&... params)
{
    func(first);
    for_each_param(func, params...);
}

} // namespace detail

template<typename Func, typename... Params>
auto for_each_param(Func &&func, Params&&... params)
{
    detail::for_each_param(func, params...);
    return func;
}

} // namespace cpp
} // namespace toolbox

#endif
