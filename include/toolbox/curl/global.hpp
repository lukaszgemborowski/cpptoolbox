#ifndef _TOOLBOX_CURL_GLOBAL_HPP_
#define _TOOLBOX_CURL_GLOBAL_HPP_

#include <curl/curl.h>
#include <toolbox/curl/curl_error.hpp>

namespace toolbox
{
namespace curl
{

enum class flags : long
{
	SSL = CURL_GLOBAL_SSL,
	Win32 = CURL_GLOBAL_WIN32,
	Nothing = CURL_GLOBAL_NOTHING,
	Default = CURL_GLOBAL_DEFAULT,
	Ack = CURL_GLOBAL_ACK_EINTR
};

class global
{
public:
	global(flags f) :
		has_ownership_ (true)
	{
		auto res = curl_global_init(static_cast<long>(f));

		if (res != CURLE_OK) {
			throw curl_error(res, "Can't initialize CURL");
		}
	}

	global(const global &) = delete;
	void operator=(const global &) = delete;

	global(global &&other) :
		has_ownership_ (true)
	{
		other.has_ownership_ = false;
	}

	void operator=(global &&other)
	{
		other.has_ownership_ = false;
		has_ownership_ = true;
	}

	~global()
	{
		if (has_ownership_)
			curl_global_cleanup();
	}

private:
	bool has_ownership_;
};

} // namespace curl
} // namespace toolbox

#endif
