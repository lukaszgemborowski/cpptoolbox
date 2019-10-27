#ifndef _TOOLBOX_CPP_STACK_ALLOCATOR_HPP_
#define _TOOLBOX_CPP_STACK_ALLOCATOR_HPP_

#include <cstddef>
#include <type_traits>
#include <new>

namespace toolbox
{
namespace alloc
{

template<typename T, std::size_t N>
struct stack_allocator
{
	using value_type = T;
	using pointer = T *;
	using const_pointer = const T *;
	using reference = T&;
	using const_reference = const T &;

	template<typename U> struct rebind
	{
		using other = stack_allocator<U, N>;
	};

	stack_allocator()
	{
	}

	~stack_allocator()
	{
	}

	stack_allocator(const stack_allocator &) :
		size_ (0)
	{
	}

	stack_allocator(stack_allocator &&) = delete;
	void operator=(const stack_allocator &) = delete;
	void operator=(stack_allocator &&) = delete;

	T* allocate(std::size_t n)
	{
		if (size_ + n > N)
		#ifdef TOOLBOX_HAS_EXCEPTIONS
			throw std::bad_alloc {};
		#else
			return nullptr;
		#endif

		auto res = reinterpret_cast<T *>(&storage_[size_]);
		size_ += n;

		return res;
	}

	void deallocate(T* ptr, std::size_t n)
	{
		if (ptr + n == reinterpret_cast<T *>(&storage_[size_])) {
			size_ -= n;
		}
	}

	std::size_t available() const
	{
		return N - size_;
	}

	std::size_t used() const
	{
		return size_;
	}

private:
	typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_[N];
	std::size_t size_ = 0;
};

} // namespace alloc
} // namespace toolbox

#endif
