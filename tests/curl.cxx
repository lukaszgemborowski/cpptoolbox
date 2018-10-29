#include <toolbox/testing/test.h>
#include <toolbox/curl/global.hpp>
#include <toolbox/curl/easy.hpp>

TEST_CASE(curl_curl_init)
{
	auto global = toolbox::curl::global(toolbox::curl::flags::Default);
}

TEST_CASE(curl_easy_fetch_with_options)
{
	auto global = toolbox::curl::global(toolbox::curl::flags::Default);
	auto request = toolbox::curl::make_easy(
		toolbox::curl::options::verbose{1},
		toolbox::curl::options::url{"http://google.com"},
		toolbox::curl::options::httpheader({"Abc: test", "Def: test2"})
	);

	request.perform();
}
