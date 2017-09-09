#include "catch.hpp"
#include <iostream>
#include <toolbox/argv/parser.hpp>
#include <toolbox/cpp/make_ref_tuple.hpp>
#include <toolbox/gcc/type_to_string.hpp>
#include <cstring>

struct cmd_line_options
{
	cmd_line_options(std::initializer_list<const char *> options)
	{
		argc = options.size();
		argv = new char*[options.size()];

		int index = 0;
		for (auto &arg : options) {
			argv[index++] = strdup(arg);
		}
	}

	~cmd_line_options()
	{
		for (int i = 0; i < argc; i++)
			free(argv[i]);
		delete [] argv;
	}

	int argc;
	char **argv;
};

TEST_CASE("basic parser", "[argv][argv::parser]")
{
	namespace o = toolbox::argv::options;

	auto verbose = o::option(o::short_name('v'));
	auto xes = o::option(o::short_name('x')).multiple();
	auto number = o::option(o::short_name('n')).value<int>();
	auto path = o::option(o::short_name('p')).value<std::string>();

	auto parser = toolbox::argv::make_parser(verbose, xes, number, path);

	auto args = cmd_line_options({"app", "arg0", "-vv", "-xxx", "-n", "123", "-p", "/usr/local", "arg1"});
	parser.parse(args.argc, args.argv);

	REQUIRE(verbose.value() == true);
	REQUIRE(xes.value() == 3);
	REQUIRE(number.value() == 123);
	REQUIRE(path.value() == "/usr/local");
	REQUIRE(parser.non_options().size() == 2);
	REQUIRE(parser.non_options()[0] == "arg0");
	REQUIRE(parser.non_options()[1] == "arg1");
}
