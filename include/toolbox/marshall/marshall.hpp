#ifndef TOOLBOX_MARSHALL_MARSHALL_HPP
#define TOOLBOX_MARSHALL_MARSHALL_HPP

#include <meta/meta.hpp>
#include <toolbox/cpp/tuple_for_each.hpp>

namespace toolbox
{
namespace marshall
{

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
		f.value.visit(visitor);
	}

	template<typename Visitor, typename T>
	typename std::enable_if<!has_tag<T>::value>::type
	visit_impl(Visitor &&visitor, field<T> &f)
	{
		visitor(f);
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

	template<typename T>
	void operator()(T &f)
	{
		s.write(reinterpret_cast<const char *>(&f.value), sizeof(f.value));
	}
};

template<typename S>
struct record_deserializer
{
	record_deserializer(S &s) : s(s) {}
	S& s;

	template<typename T>
	void operator()(T &f)
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
