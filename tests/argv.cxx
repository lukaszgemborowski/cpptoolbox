#include "catch.hpp"
#include <iostream>
#include <toolbox/argv/parser.hpp>
#include <toolbox/cpp/make_ref_tuple.hpp>
#include <toolbox/gcc/type_to_string.hpp>
#include <initializer_list>
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
	int value_from_callback = 0;
	int value_of_number = 0;

	auto verbose = o::option<void>(o::short_name('v'));
	auto opt_a = o::option<void>(o::short_name('a'));
	auto opt_b = o::option<void>(o::short_name('b'));
	auto xes = o::option<void>(o::short_name('x'), o::long_name("longX"));
	auto number = o::option<int>(o::short_name('n')).store(value_of_number);
	auto path = o::option<std::vector<std::string>>(o::short_name('p'));
	auto callback = o::option<int>(o::short_name('c')).action([&value_from_callback](int value) { value_from_callback = value * 2; });

	auto parser = toolbox::argv::make_parser(verbose, number, path, xes, opt_a, opt_b, callback);

	auto args = cmd_line_options(
	{
		"app",
		"arg0",
		"-vv",
		"-xx", "--longX",
		"-ab",
		"-n", "123",
		"-p", "/usr/local",
		"-p", "/var/run",
		"-c", "3",
		"arg1"});
	parser.parse(args.argc, args.argv);

	REQUIRE(verbose.value() == true);
	REQUIRE(xes.found() == 3);
	REQUIRE(number.value() == 123);
	REQUIRE(value_of_number == 123);
	REQUIRE(path.value()[0] == "/usr/local");
	REQUIRE(path.value()[1] == "/var/run");
	REQUIRE(parser.non_options().size() == 2);
	REQUIRE(parser.non_options()[0] == "arg0");
	REQUIRE(parser.non_options()[1] == "arg1");
	REQUIRE(opt_a.value() == true);
	REQUIRE(opt_b.value() == true);
	REQUIRE(value_from_callback == 6);
}
