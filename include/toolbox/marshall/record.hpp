#ifndef TOOLBOX_MARSHALL_RECORD_HPP
#define TOOLBOX_MARSHALL_RECORD_HPP

#include <tuple>
#include <string>
#include <vector>
#include <toolbox/cpp/tuple_for_each.hpp>
#include <meta/meta.hpp>

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
		T value;
		using type = T;
	};

private:
	template<typename T> struct has_tag
	{
		template<typename U> static std::true_type check(typename U::marshall_record_tag *);
		template<typename> static std::false_type check(...);

		static constexpr auto value = decltype(check<T>(nullptr))::value;
	};

	template<typename Visitor>
	struct visitor_impl
	{
		Visitor &visitor_;

		visitor_impl(Visitor &visitor) : visitor_ (visitor) {}

		template<typename T>
		typename std::enable_if<has_tag<T>::value>::type
		operator()(field<T> &f)
		{
			visitor_.enter(f);
			f.value.visit(visitor_);
			visitor_.leave(f);
		}

		template<typename T>
		typename std::enable_if<!has_tag<T>::value>::type
		operator()(field<T> &f)
		{
			visitor_.visit(f);
		}
	};

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
		visitor_impl<Visitor> vis(visitor);
		cpp::tuple_for_each(
			fields,
			vis
		);
	}

	template<std::size_t I>
	auto & get()
	{
		return std::get<I>(fields).value;
	}

	std::tuple<field<Fields>...> fields;
};

} // namespace marshall
} // namespace toolbox

#endif // TOOLBOX_MARSHALL_RECORD_HPP
