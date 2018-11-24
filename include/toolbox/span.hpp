#ifndef _TOOLBOX_SPAN_HPP_
#define _TOOLBOX_SPAN_HPP_

#include <toolbox/std/cstddef.hpp>

namespace toolbox
{

template<class T>
class span
{
public:
    span(T * const b, T * const e)
        : begin_ (b)
        , end_ (e)
    {}

    T* begin()
    {
        return begin_;
    }

    T* end()
    {
        return end_;
    }

    T& operator[](std::size_t index)
    {
        return begin_[index];
    }

    const T& operator[](std::size_t index) const
    {
        return begin_[index];
    }

    std::size_t size() const
    {
        return end_ - begin_;
    }

private:
    T * const begin_;
    T * const end_;
};

} // namespace toolbox

#endif // _TOOLBOX_SPAN_HPP_
