#ifndef _TOOLBOX_CPP_STATIC_CONFIG_HPP_
#define _TOOLBOX_CPP_STATIC_CONFIG_HPP_

#include <toolbox/std/type_traits.hpp>
#include <meta/meta.hpp>

namespace toolbox
{
namespace static_config
{

template<class... DefaultOptions>
struct defaults {
    using list = meta::list<DefaultOptions...>;
};

template<class... Options>
struct options {
    using list = meta::list<Options...>;
};

template<class A, class B>
struct same_base_template : public std::false_type {};

template<class A, class B, template<typename> typename Template>
struct same_base_template<Template<A>, Template<B>> : public std::true_type {};

template<class Defaults, class Options>
struct base {
    // search through Options and find if T
    // is not contained in it
    struct if_not_in_options {
        template<class A>
        struct same_template {
            template<class B>
            using invoke = same_base_template<A, B>;
        };

        template<class T>
        using invoke = std::integral_constant<bool,
            meta::count_if<
                typename Options::list,
                same_template<T>
            >::value == 0>;
    };

    using not_overriden = meta::filter<
        typename Defaults::list,
        if_not_in_options>;

    using result = meta::concat<not_overriden, typename Options::list>;

    template<class... Args>
    struct base_type : Args... {};

    using type = meta::apply<meta::quote<base_type>, result>;
};

} // namespace static_config
} // namespace toolbox

#endif // _TOOLBOX_CPP_STATIC_CONFIG_HPP_