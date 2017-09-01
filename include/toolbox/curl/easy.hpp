#ifndef _TOOLBOX_CURL_EASY_HPP_
#define _TOOLBOX_CURL_EASY_HPP_

#include <curl/curl.h>

namespace toolbox
{
namespace curl
{

namespace options
{

template<int E, typename T>
struct option_def
{
	using type_t = T;
	static constexpr auto option = static_cast<CURLoption>(E);
	const T &value;

	option_def(const T &value) :
		value (value) {}
};

using verbose = option_def<CURLOPT_VERBOSE, long>;
using url = option_def<CURLOPT_URL, const char*>;
using postfields = option_def<CURLOPT_POSTFIELDS, const char*>;
using writefunction = option_def<CURLOPT_WRITEFUNCTION, size_t(*)(char *, size_t, size_t, void *)>;
using writedata = option_def<CURLOPT_WRITEDATA, void *>;

} // namespace options

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
	auto set(const T &value)
	{
		return curl_easy_setopt(curl_, T::option, value.value);
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
