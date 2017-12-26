#ifndef TOOLBOX_MARSHALL_MARSHALL_HPP
#define TOOLBOX_MARSHALL_MARSHALL_HPP

#include <meta/meta.hpp>
#include <toolbox/cpp/tuple_for_each.hpp>
#include <json/json.hpp>

namespace toolbox
{
namespace marshall
{

template<
	typename SizeT,
	typename ElementT,
	template<typename...> class ContainerT = std::vector>
struct dynamic_array : public ContainerT<ElementT>
{
	using array_size_t = SizeT;
	using element_t = ElementT;
};

template<typename...> struct is_dynamic_array : std::false_type {};
template<typename A, typename B, template<typename ... > class C> struct is_dynamic_array<dynamic_array<A, B, C>> : std::true_type {};

template<typename... Fields>
struct record
{
private:
	template<typename T>
	struct field_description
	{
		std::string short_name;
		std::string description;
	};

	template<typename T>
	struct field
	{
		field(const field_description<T> & desc) :
			description(desc) {}

		field_description<T> description;
		T value = T{};
		using type = T;
	};

private:
	template<typename T> struct has_tag
	{
		template<typename U> static std::true_type check(typename U::marshall_record_tag *);
		template<typename> static std::false_type check(...);

		static constexpr auto value = decltype(check<T>(nullptr))::value;
	};

	template<typename Visitor, typename T>
	typename std::enable_if<has_tag<T>::value>::type
	visit_impl(Visitor &&visitor, field<T> &f)
	{
		visitor.enter(f);
		f.value.visit(visitor);
		visitor.leave(f);
	}

	template<typename Visitor, typename T>
	typename std::enable_if<!has_tag<T>::value>::type
	visit_impl(Visitor &&visitor, field<T> &f)
	{
		visitor.visit(f);
	}

public:
	using types = meta::list<Fields...>;
	using fields_count = meta::size<types>;
	using marshall_record_tag = void;

	record(field_description<Fields>... descriptions) :
		fields(descriptions...)
	{}

	template<typename Visitor>
	void visit(Visitor &&visitor)
	{
		cpp::tuple_for_each(
			fields,
			[&](auto &f)
			{
				visit_impl(visitor, f);
			}
		);
	}

	template<std::size_t I>
	auto & get()
	{
		return std::get<I>(fields).value;
	}

	std::tuple<field<Fields>...> fields;
};

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
	void visit(T &t)
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
	void visit(T &t)
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
