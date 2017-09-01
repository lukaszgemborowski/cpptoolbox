#ifndef _TOOLBOX_CURL_CURL_ERROR_HPP_
#define _TOOLBOX_CURL_CURL_ERROR_HPP_

#include <curl/curl.h>
#include <stdexcept>

namespace toolbox
{
namespace curl
{

class curl_error : public std::runtime_error
{
public:
	curl_error(CURLcode code, const std::string &message) :
		runtime_error (message),
		code_ (code)
	{
	}

	curl_error(CURLcode code, const char *message) :
		runtime_error (message),
		code_ (code)
	{
	}

	curl_error(CURLcode code) :
		runtime_error("curl error"),
		code_ (code)
	{
	}

	CURLcode code() const
	{
		return code_;
	}

private:
	CURLcode code_;
};

} // namespace curl
} // namespace toolbox

#endif
