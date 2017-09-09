#ifndef _TOOLBOX_ARGV_PARSER_HPP_
#define _TOOLBOX_ARGV_PARSER_HPP_

#include <tuple>
#include <string>
#include <sstream>
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
	explicit long_name(const char *name) :
		std::string (name)
	{}
};

struct option;
struct multi_flag;
template<typename> struct with_value;

struct option
{
	option(const option &other) = default;

	option(const short_name &name) :
		short_name_ (name),
		long_name_ {},
		description_ {},
		found_ {0}
	{}

	option(const long_name &name) :
		short_name_ (0),
		long_name_ (name),
		description_ {},
		found_ {0}
	{}

	option& description(const std::string &desc)
	{
		description_ = desc;
		return *this;
	}

	multi_flag multiple() const;

	template<typename T>
	with_value<T> value() const;

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
		return false;
	}

	bool value() const
	{
		return found_;
	}

	void set_found()
	{
		found_ ++;
	}

	void set_found(const char *)
	{
		found_ ++;
	}

protected:
	const char short_name_;
	const std::string long_name_;
	std::string description_;
	int found_;
};

struct multi_flag : public option
{
	multi_flag(const option &original) :
		option (original)
	{}

	int value() const
	{
		return found_;
	}
};

template<typename T> struct with_value : public option
{
	with_value(const option& original) :
		option (original)
	{}

	bool has_argument() const
	{
		return true;
	}

	const T& value() const
	{
		return value_;
	}

	void set_found()
	{
	}

	void set_found(const char *value)
	{
		std::stringstream ss(value);
		ss >> value_;
	}

private:
	T value_;
};

multi_flag option::multiple() const
{
	return multi_flag(*this);
}

template<typename T>
with_value<T> option::value() const
{
	return with_value<T>(*this);
}

} // namespace options

namespace detail
{

template<typename T>
struct option
{
	option(const T &definition) :
		definition_ (definition),
		found_ (false)
	{
	}

	const auto & definition() const
	{
		return definition_;
	}

	auto found() const
	{
		return found_;
	}

	void set_found()
	{
		found_ = true;
	}

private:
	T definition_;
	bool found_;
};


template<typename T> struct wrap_tuple_elements;
template<typename... Args> struct wrap_tuple_elements<std::tuple<Args...>> {
	using type = std::tuple<option<Args>...>;
};

} // namespace detail


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
						if (optarg)
							option.set_found(optarg);
						else
							option.set_found();
					}
				}
			);
		}

		for (int i = optind; i < argc; i ++)
			non_options_.push_back(std::string(argv[i]));
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
