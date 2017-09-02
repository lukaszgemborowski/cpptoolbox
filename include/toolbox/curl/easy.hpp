#ifndef _TOOLBOX_CURL_EASY_HPP_
#define _TOOLBOX_CURL_EASY_HPP_

#include <curl/curl.h>
#include <tuple>
#include <vector>
#include <string>
#include <toolbox/cpp/tuple_for_each.hpp>

namespace toolbox
{
namespace curl
{

namespace options
{

namespace detail
{

template<typename T>
struct curl_type
{
	curl_type (const T &value) :
		value_ (value)
	{}

	using curl_type_t = T;
	using cpp_type_t = T;

	curl_type_t curl_value() const
	{
		return value_;
	}

	T value_;
};

template<> struct curl_type<std::string>
{
	curl_type (const std::string &value) :
		value_ (value)
	{
	}

	curl_type(curl_type<std::string> &&rhs) = default;
	curl_type(const curl_type<std::string> &) = delete;
	void operator=(const curl_type<std::string> &) = delete;

	using curl_type_t = const char *;
	using cpp_type_t = std::string;

	curl_type_t curl_value() const
	{
		return value_.c_str();
	}

	cpp_type_t value_;
};

template<> struct curl_type<std::vector<std::string>>
{
	using curl_type_t = curl_slist *;
	using cpp_type_t = std::vector<std::string>;

	curl_type (const std::vector<std::string> &value)
	{
		for (auto &el : value)
			value_ = curl_slist_append(value_, el.c_str());
	}

	curl_type(curl_type<cpp_type_t> &&rhs)
	{
		std::swap(value_, rhs.value_);
	}

	curl_type(const curl_type<cpp_type_t> &) = delete;
	void operator=(curl_type<cpp_type_t> &) = delete;

	~curl_type()
	{
		curl_slist_free_all(value_);
	}

	curl_type_t curl_value() const
	{
		return value_;
	}

private:
	curl_type_t value_ = nullptr;
};

} // namespacec detail

template<int E, typename T>
struct option_def
{
	using type_t = T;
	static constexpr auto option = static_cast<CURLoption>(E);

	option_def(const T &value) :
		value_ (value) {}

	option_def(option_def<E, T> &&rhs) :
		value_ (std::move(rhs.value_))
	{}

	auto value() const
	{
		return value_.curl_value();
	}

private:
	detail::curl_type<T> value_;
};

using verbose = option_def<CURLOPT_VERBOSE, long>;
using url = option_def<CURLOPT_URL, std::string>;
using postfields = option_def<CURLOPT_POSTFIELDS, std::string>;
using writefunction = option_def<CURLOPT_WRITEFUNCTION, size_t(*)(char *, size_t, size_t, void *)>;
using writedata = option_def<CURLOPT_WRITEDATA, void *>;
using httpheader = option_def<CURLOPT_HTTPHEADER, std::vector<std::string>>;

} // namespace options


// this is just sugar to give a new name for make_tuple
// unnecesary right now but may be helpful when changing
// implementation later
template<typename... Options>
auto make_options(Options&&... options)
{
	return std::make_tuple( std::forward<Options>(options)...);
}

class easy
{
public:
	easy() :
		curl_ (curl_easy_init())
	{
	}

	easy(easy &&other) :
		curl_(other.curl_)
	{
		other.curl_ = nullptr;
	}

	void operator=(easy &&other)
	{
		curl_ = other.curl_;
		other.curl_ = nullptr;
	}

	easy(const easy &) = delete;
	void operator=(const easy &) = delete;

	template<typename T>
	auto use(const T &options)
	{
		auto res = CURLE_OK;

		cpp::tuple_for_each(
			options,
			[this, &res](const auto &option) {
				if (res != CURLE_OK)
					return;

				res = curl_easy_setopt(curl_, std::decay<decltype(option)>::type::option, option.value());
			}
		);

		return res;
	}

	CURLcode perform()
	{
		return curl_easy_perform(curl_);
	}

	~easy()
	{
		if (curl_)
			curl_easy_cleanup (curl_);
	}

private:
	CURL* curl_ = nullptr;
};

} // namespace curl
} // namespace toolbox

#endif
