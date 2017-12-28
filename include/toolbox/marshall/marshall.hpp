#ifndef TOOLBOX_MARSHALL_MARSHALL_HPP
#define TOOLBOX_MARSHALL_MARSHALL_HPP

#include <toolbox/marshall/record.hpp>
#include <toolbox/marshall/json.hpp>

namespace toolbox
{
namespace marshall
{

namespace detail
{

template<typename S>
struct record_serializer
{
	record_serializer(S &s) : s(s) {}
	S& s;

	template<typename T> void enter(T &) {}
	template<typename T> void leave(T &) {}

	template<typename T>
	typename std::enable_if<is_dynamic_array<typename T::type>::value>::type
	visit(T &f)
	{
		typename T::type::array_size_t size = f.value.size();
		s.write(reinterpret_cast<const char *>(&size), sizeof(size));

		for (const auto &e : f.value) {
			s.write(reinterpret_cast<const char*>(&e), sizeof(e));
		}
	}

	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<typename T::type>::value>::type
	visit(T &f)
	{
		s.write(reinterpret_cast<const char *>(&f.value), sizeof(f.value));
	}
};

template<typename S>
struct record_deserializer
{
	record_deserializer(S &s) : s(s) {}
	S& s;

	template<typename T> void enter(T &) {}
	template<typename T> void leave(T &) {}

	template<typename T>
	typename std::enable_if<is_dynamic_array<typename T::type>::value>::type
	visit(T &f)
	{
		using size_type = typename T::type::array_size_t;

		size_type size = f.value.size();
		s.read(reinterpret_cast<char *>(&size), sizeof(size));
		f.value.clear();

		for (size_type i = 0; i < size; i ++) {
			typename T::type::element_t el;
			s.read(reinterpret_cast<char *>(&el), sizeof(el));
			f.value.push_back(el);
		}
	}

	template<typename T>
	typename std::enable_if<std::is_trivially_copyable<typename T::type>::value>::type
	visit(T &f)
	{
		s.read(reinterpret_cast<char *>(&f.value), sizeof(f.value));
	}
};

}

template<typename S, typename T>
void serialize(S &s, T& record)
{
	record.visit(detail::record_serializer<S>{s});
}

template<typename S, typename T>
void deserialize(S &s, T& record)
{
	record.visit(detail::record_deserializer<S>{s});
}

}
}

#endif
