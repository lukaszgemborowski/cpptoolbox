#include "catch.hpp"
#include <iostream>
#include <toolbox/curl/global.hpp>
#include <toolbox/curl/easy.hpp>

TEST_CASE("curl init", "[curl]")
{
	auto global = toolbox::curl::global(toolbox::curl::flags::Default);
}

TEST_CASE("curl easy fetch with options", "[curl]")
{
	auto global = toolbox::curl::global(toolbox::curl::flags::Default);
	auto request = toolbox::curl::easy{};
	auto options = std::make_tuple(
		toolbox::curl::options::verbose{1},
		toolbox::curl::options::url{"http://google.com"},
		toolbox::curl::options::httpheader({"Abc: test", "Def: test2"})
	);

	request.use(options);
	request.perform();
}
