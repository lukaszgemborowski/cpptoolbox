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

	auto parser = toolbox::argv::make_parser(verbose, xes);

	const char* args[] = {"app", "-vv", "-xxx"};
	parser.parse(3, args);

	REQUIRE(verbose.value() == true);
	REQUIRE(xes.value() == 3);
}
