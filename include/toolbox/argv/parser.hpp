#ifndef _TOOLBOX_ARGV_PARSER_HPP_
#define _TOOLBOX_ARGV_PARSER_HPP_

#include <tuple>
#include <string>
#include <sstream>
#include <functional>
#include <getopt.h>
#include <toolbox/cpp/tuple_for_each.hpp>
#include <toolbox/cpp/make_ref_tuple.hpp>

namespace toolbox
{
namespace argv
{
namespace options
{

struct short_name
{
	explicit short_name(const char name) :
		value_ (name)
	{}

	operator char() const
	{
		return value_;
	}

private:
	char value_;
};

struct long_name : public  std::string
{
	long_name() : std::string{} {}
	explicit long_name(const char *name) :
		std::string (name)
	{}
};

struct counter {};

namespace detail
{

template<typename T>
struct value_container
{
	using type_t = T;
	using element_type_t = T;
	using is_void = std::false_type;

	T value_;
	const auto& set(const char *p)
	{
		std::stringstream ss(p);
		ss >> value_;
		return value_;
	}

	const T& get() const
	{
		return value_;
	}
};

template<typename T>
struct value_container<std::vector<T>>
{
	using type_t = std::vector<T>;
	using element_type_t = T;
	using is_void = std::false_type;

	std::vector<T> value_;
	const auto& set(const char *p)
	{
		std::stringstream ss(p);
		T temp;
		ss >> temp;
		value_.push_back(temp);
		return value_.back();
	}

	const auto& get() const
	{
		return value_;
	}
};

template<>
struct value_container<void>
{
	using type_t = bool;
	using element_type_t = bool;
	using is_void = std::true_type;

	bool value_ = false;
	const auto& set(const char *)
	{
		value_ = true;
		return value_;
	}

	const bool& get() const
	{
		return value_;
	}
};

template<>
struct value_container<counter>
{
	using type_t = int;
	using element_type_t = int;
	using is_void = std::true_type;

	type_t value_ = 0;
	auto set(const char *)
	{
		value_ ++;
		return value_;
	}

	const auto& get() const
	{
		return value_;
	}
};

template<typename T> struct function_type
{
	using have_argument_t = std::true_type;
	using type_t = std::function<void (T &)>;
};

template<> struct function_type<void>
{
	using have_argument_t = std::false_type;
	using type_t = std::function<void ()>;
};

} // namespace detail

template<typename T = void>
struct option
{
	option(const short_name &name, const long_name &lname = long_name{}) :
		short_name_ (name),
		long_name_ (lname),
		description_ {}
	{
	}

	option& description(const std::string &desc)
	{
		description_ = desc;
		return *this;
	}

	char get_short() const
	{
		return short_name_;
	}

	const std::string &get_long() const
	{
		return long_name_;
	}

	bool has_argument() const
	{
		return detail::value_container<T>::is_void::value == false;
	}

	typename detail::value_container<T>::type_t
	value() const
	{
		return value_.get();
	}

	void set_found(const char *arg)
	{
		const auto &value = value_.set(arg);
		if (func_)
			func_(value);
	}

	using callback_type = std::function<void (const typename detail::value_container<T>::element_type_t &)>;

	option<T>& action(const callback_type &func)
	{
		func_ = func;
		return *this;
	}

	option<T>& store(typename detail::value_container<T>::type_t &dest)
	{
		transfer_to_storage_ = [&dest](const typename detail::value_container<T>::type_t &src) {
			dest = src;
		};

		return *this;
	}

	void transfer_to_storage() const
	{
		if (transfer_to_storage_)
			transfer_to_storage_(value_.get());
	}

protected:
	const char short_name_;
	const std::string long_name_;
	std::string description_;
	detail::value_container<T> value_;
	callback_type func_;
	std::function<void (const typename detail::value_container<T>::type_t &)> transfer_to_storage_;
};

} // namespace options

template<typename Options>
class parser
{
public:
	parser(Options &&options) :
		options_(std::move(options))
	{
		std::stringstream ss;
		cpp::tuple_for_each(
			options_,
			[&ss, this](auto& option) {
				ss << option.get_short();
				if (option.has_argument()) ss << ":";

				if (option.get_long().size() > 0) {
					::option o;
					o.name = option.get_long().c_str();
					o.has_arg = option.has_argument() ? required_argument : no_argument;
					o.flag = nullptr;
					o.val = option.get_short();

					options_arr_.push_back(o);
				}
			}
		);

		opt_descriptor_ = ss.str();
	}

	void parse(int argc, char ** argv)
	{
		int c;
		int index;

		while (
			(c = getopt_long(
					argc,
					const_cast<char* const*>(argv),
					opt_descriptor_.c_str(),
					options_arr_.data(),
					&index))
				!= -1) {
			cpp::tuple_for_each(
				options_,
				[this, c](auto& option) {
					if (option.get_short() == c) {
						option.set_found(optarg);
					}
				}
			);
		}

		for (int i = optind; i < argc; i ++)
			non_options_.push_back(std::string(argv[i]));

		cpp::tuple_for_each(
			options_,
			[](auto &option) {
				option.transfer_to_storage();
			}
		);
	}

	const std::vector<std::string> &non_options() const
	{
		return non_options_;
	}

private:
	Options options_;
	std::vector<::option> options_arr_;
	std::string opt_descriptor_;
	std::vector<std::string> non_options_;
};

template<typename... Options>
auto make_parser(Options&&... options)
{
	auto options_tuple = cpp::make_ref_tuple(options...);
	return parser<decltype(options_tuple)>(std::move(options_tuple));
}

} // namespace argv
} // namespace toolbox

#endif
