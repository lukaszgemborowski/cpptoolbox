#include "catch.hpp"
#include <toolbox/curl/global.hpp>
#include <toolbox/curl/easy.hpp>

TEST_CASE("curl init", "[curl]")
{
	auto global = toolbox::curl::global(toolbox::curl::flags::Default);
}

TEST_CASE("curl easy fetch", "[curl]")
{
	auto global = toolbox::curl::global(toolbox::curl::flags::Default);
	auto request = toolbox::curl::easy{};

	request.set(toolbox::curl::options::verbose(1));
	request.set(toolbox::curl::options::url("http://google.com"));

	request.perform();
}
