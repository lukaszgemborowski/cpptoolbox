#ifndef _TOOLBOX_CONTAINER_ARRAY_HPP_
#define _TOOLBOX_CONTAINER_ARRAY_HPP_

#include <toolbox/std/cstddef.h>

namespace toolbox
{

template<typename T, unsigned int N>
class array
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = const T*;
    using const_iterator = const T*;
    using size_type = std::size_t;

    array() : buffer_()
    {
    }

    constexpr std::size_t size() const {
        return N;
    }

    iterator begin() {
        return &buffer_[0];
    }

    iterator end() {
        return &buffer_[N];
    }

    reference at(size_type pos) {
        return buffer_[pos];
    }

    reference operator[](size_type pos) {
        return buffer_[pos];
    }

private:
    T buffer_[N];
};

} // namespace toolbox

#endif // _TOOLBOX_CONTAINER_ARRAY_HPP_