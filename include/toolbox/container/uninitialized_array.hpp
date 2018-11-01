#ifndef _TOOLBOX_CONTAINER_UNINITIALIZED_ARRAY_HPP_
#define _TOOLBOX_CONTAINER_UNINITIALIZED_ARRAY_HPP_

#include <toolbox/config.h>
#include <toolbox/std/type_traits.h>
#include <toolbox/std/new.hpp>

namespace toolbox
{

template<typename T, unsigned int N>
class uninitialized_array
{
    using element_size = std::integral_constant<unsigned int, sizeof(T)>;
    using buffer_size = std::integral_constant<unsigned int, element_size::value * N>;

public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T&;
    using iterator = const T*;
    using const_iterator = const T*;
    using size_type = unsigned int;

    constexpr size_type size() const { return N; }
    reference operator[](size_type i)
    {
        return at(i);
    }

    reference at(size_type i)
    {
        return *reinterpret_cast<T *>(&buffer_[i * element_size::value]);
    }

    template<typename... Args>
    reference construct(size_type i, Args&&... args)
    {
        char *ptr = &buffer_[i * element_size::value];
        T *tptr = new(ptr) T(args...);
        return *tptr;
    }

    constexpr size_type index_of(reference obj) const
    {
        return &obj - reinterpret_cast<const T *>(&buffer_[0]);
    }

    constexpr bool own(reference obj) const
    {
        return index_of(obj) < size();
    }

    void destruct(size_type i)
    {
        char *ptr = &buffer_[i * element_size::value];
        reinterpret_cast<T *>(ptr)->~T();
    }

private:
    char buffer_[buffer_size::value];
};

} // namespace toolbox

#endif // _TOOLBOX_CONTAINER_UNINITIALIZED_ARRAY_HPP_