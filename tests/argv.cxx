#include "catch.hpp"
#include <iostream>
#include <toolbox/argv/parser.hpp>
#include <toolbox/cpp/make_ref_tuple.hpp>
#include <toolbox/gcc/type_to_string.hpp>

TEST_CASE("basic parser", "[argv][argv::parser]")
{
	namespace o = toolbox::argv::options;

	auto verbose = o::option(o::short_name('v'));
	auto xes = o::option(o::short_name('x')).multiple();
	auto number = o::option(o::short_name('n')).value<int>();
	auto path = o::option(o::short_name('p')).value<std::string>();

	auto parser = toolbox::argv::make_parser(verbose, xes, number, path);

	const char* args[] = {"app", "arg0", "-vv", "-xxx", "-n", "123", "-p", "/usr/local", "arg1"};
	parser.parse(9, args);

	REQUIRE(verbose.value() == true);
	REQUIRE(xes.value() == 3);
	REQUIRE(number.value() == 123);
	REQUIRE(path.value() == "/usr/local");
	REQUIRE(parser.non_options().size() == 2);
	REQUIRE(parser.non_options()[0] == "arg0");
	REQUIRE(parser.non_options()[1] == "arg1");
}
