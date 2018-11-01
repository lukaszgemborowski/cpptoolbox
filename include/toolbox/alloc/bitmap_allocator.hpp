#ifndef _TOOLBOX_ALLOC_BITMAP_ALLOCATOR_H_
#define _TOOLBOX_ALLOC_BITMAP_ALLOCATOR_H_

#include <toolbox/container/uninitialized_array.hpp>
#include <toolbox/container/array.hpp>

namespace toolbox
{

template<typename T, unsigned int N>
class bitmap_allocator
{
public:
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using size_type = unsigned int;

    template<typename... Args>
    reference allocate(Args&&... args)
    {
        auto index = find_and_mark();
        allocated_ ++;
        return pool_.construct(index, args...);
    }

    void deallocate(reference obj)
    {
        auto index = find_and_clear(obj);
        pool_.destruct(index);
        allocated_ --;
    }

    constexpr bool own(reference obj) const
    {
        return pool_.own(obj);
    }

    constexpr size_type allocated_count() const
    {
        return allocated_;
    }

    constexpr size_type capacity() const
    {
        return N;
    }

    constexpr bool space_available() const
    {
        return allocated_count() < capacity();
    }

private:
    unsigned int find_and_clear(reference obj)
    {
        auto index = pool_.index_of(obj);
        auto byte_index = index / 8;
        auto bit_index = index % 8;

        bits_[byte_index] = bits_[byte_index] & ~(1 << bit_index);
        return index;
    }

    unsigned int find_and_mark()
    {
        unsigned int index = 0;
        for (auto byte : bits_) {
            if (byte == 0xff) {
                index ++;
                continue;
            }

            unsigned int bit_index = 0;
            while (byte & 1 && bit_index < 8) {
                byte = byte >> 1;
                bit_index ++;
            }

            bits_[index] = bits_[index] | (1 << bit_index);
            return (index * 8) + bit_index;
        }

        return 0;
    }

private:
    toolbox::uninitialized_array<T, N> pool_;
    toolbox::array<unsigned char, (N/8)+1> bits_;
    unsigned int allocated_ = 0; 
};

} // namespace toolbox

#endif // _TOOLBOX_ALLOC_BITMAP_ALLOCATOR_H_