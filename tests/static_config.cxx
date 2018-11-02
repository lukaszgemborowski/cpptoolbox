#include <toolbox/testing/test.h>
#include <toolbox/cpp/static_config.hpp>
#include <meta/meta.hpp>

// library can define a set of options, eg:

template<class value>
struct FooOption {
    static constexpr int foo = value::value;

    // you can constraint it using static_assert
    static_assert(foo < 100, "foo needs to be less than 100");
};

template<class value>
struct BarOption {
    static constexpr bool bar = value::value;
};

template<class value>
struct BazOption {
    static constexpr char baz = value::value;
};

namespace sc = toolbox::static_config;


TEST_CASE(static_config_example) {
    // library define defaults:
    using lib_defauls = sc::defaults<
        FooOption<std::integral_constant<int, 42>>,
        BarOption<std::integral_constant<bool, true>>,
        BazOption<std::integral_constant<char, 'X'>>
    >;

    // user overrides some selected options:
    using my_config = sc::options<
        BazOption<std::integral_constant<char, 'Y'>>,
        FooOption<std::integral_constant<int, 24>>
    >;

    using b = sc::base<lib_defauls, my_config>::type;

    // library code (possibly via template argument) can
    // obtain configuration, final config should look like:
    // Foo -> 24 (overriden)
    // Bar -> true (default)
    // Baz -> 'Y' (overriden)
    CHECK(b::foo == 24);
    CHECK(b::bar == true);
    CHECK(b::baz == 'Y');
}
