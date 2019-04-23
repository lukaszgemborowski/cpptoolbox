#include <toolbox/testing/test.hpp>
#include <toolbox/argv/parser.hpp>
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

namespace argv = toolbox::argv;

TEST_CASE(argv_basic_parser)
{
    int value_from_callback = 0;
    int value_of_number = 0;

    auto verbose = argv::option<void>('v');
    auto opt_a = argv::option<void>('a');
    auto opt_b = argv::option<void>('b');
    auto xes = argv::option<void>('x', "longX");
    auto number = argv::option<int>('n').store(value_of_number);
    auto path = argv::option<std::vector<std::string>>('p');
    auto callback = argv::option<int>('c').action([&value_from_callback](int value) { value_from_callback = value * 2; });

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

    CHECK(verbose.value() == true);
    CHECK(xes.found() == 3);
    CHECK(number.value() == 123);
    CHECK(value_of_number == 123);
    CHECK(path.value()[0] == "/usr/local");
    CHECK(path.value()[1] == "/var/run");
    CHECK(parser.non_options().size() == 2);
    CHECK(std::strcmp(parser.non_options()[0], "arg0") == 0);
    CHECK(std::strcmp(parser.non_options()[1], "arg1") == 0);
    CHECK(opt_a.value() == true);
    CHECK(opt_b.value() == true);
    CHECK(value_from_callback == 6);
}

TEST_CASE(argv_rvalue_options)
{
    int value_from_callback = 0;

    // if you want just to perform some action on argument occurence
    // you don't need to create lvalue option object, you may just
    // pass temporary to the parser
    auto parser = argv::make_parser(
        argv::option<void>{'f'}.action(
            [&value_from_callback]() {
                value_from_callback = 42;
            }
        ));

    auto args = cmd_line_options({"foo", "-f"});

    parser.parse(args.argc, args.argv);

    CHECK(value_from_callback == 42);
}
