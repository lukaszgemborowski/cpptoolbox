#ifndef TOOLBOX_MARSHALL_MARSHALL_HPP
#define TOOLBOX_MARSHALL_MARSHALL_HPP

#include <json/json.hpp>
#include <toolbox/marshall/record.hpp>

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

namespace json
{
namespace detail
{
struct json_serializer
{
	nlohmann::json j;
	nlohmann::json *current;
	nlohmann::json *previous;

	json_serializer() :
		j {},
		current {&j}
	{}

	json_serializer(const json_serializer &) = delete;
	void operator=(const json_serializer &) = delete;

	template<typename T> void enter(T &t)
	{
		previous = current;
		current = &(*current)[t.description.short_name];
	}

	template<typename T> void leave(T &t)
	{
		current = previous;
	}

	template<typename T>
	typename std::enable_if<std::is_array<typename T::type>::value>::type
	visit(T &t)
	{
		static_assert(std::rank<typename T::type>::value == 1, "can only serialize 1-dimensional array");

		auto arr = nlohmann::json::array();

		for (const auto &e : t.value)
			arr.push_back(e);

		(*current)[t.description.short_name] = arr;
	}

	template<typename T>
	typename std::enable_if<!std::is_array<typename T::type>::value>::type
	visit(T &t)
	{
		(*current)[t.description.short_name] = t.value;
	}
};

struct json_deserializer
{
	nlohmann::json &j;
	nlohmann::json *current;
	nlohmann::json *previous;

	json_deserializer(nlohmann::json &j) : j(j), current(&j) {}

	template<typename T> void enter(T &t)
	{
		previous = current;
		current = &(*current)[t.description.short_name];
	}

	template<typename T> void leave(T &t)
	{
		current = previous;
	}

	template<typename T>
	typename std::enable_if<std::is_array<typename T::type>::value>::type
	visit(T &t)
	{
		using array_type = typename T::type;
		using value_type = typename std::remove_extent<array_type>::type;

		static_assert(std::rank<typename T::type>::value == 1, "can only deserialize 1-dimensional array");

		for (auto i = 0u; i < std::extent<typename T::type>::value; i ++)
			t.value[i] = (*current)[t.description.short_name][i].template get<value_type>();
	}

	template<typename T>
	typename std::enable_if<!std::is_array<typename T::type>::value>::type
	visit(T &t)
	{
		t.value = (*current)[t.description.short_name];
	}

};

} // namespace detail

template<typename T>
nlohmann::json serialize(T& record)
{
	detail::json_serializer visitor{};
	record.visit(visitor);
	return visitor.j;
}

template<typename T>
void deserialize(nlohmann::json &j, T &record)
{
	detail::json_deserializer visitor {j};
	record.visit(visitor);
}

} // namespace json


}
}

#endif
