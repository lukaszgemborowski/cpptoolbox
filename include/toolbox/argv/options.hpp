#ifndef _TOOLBOX_ARGV_OPTIONS_HPP_
#define _TOOLBOX_ARGV_OPTIONS_HPP_

#include <functional>
#include <string>
#include <vector>

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

	option& description(const std::string &desc, const std::string &arg_name = std::string())
	{
		description_ = desc;
		argument_name_ = arg_name;
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

	const std::string &get_description() const
	{
		return description_;
	}

	const std::string &get_argument_name() const
	{
		return argument_name_;
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
	std::string argument_name_;
	detail::value_container<T> value_;
	callback_type func_;
	std::function<void (const typename detail::value_container<T>::type_t &)> transfer_to_storage_;
};

} // namespace options
} // namespace argv
} // namespace toolbox

#endif
