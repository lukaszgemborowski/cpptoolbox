#ifndef _TOOLBOX_ARGV_PARSER_HPP_
#define _TOOLBOX_ARGV_PARSER_HPP_

#include <tuple>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <getopt.h>
#include <toolbox/cpp/tuple_for_each.hpp>
#include <toolbox/cpp/make_ref_tuple.hpp>
#include <toolbox/cpp/for_each_param.hpp>
#include <toolbox/argv/options.hpp>
#include <toolbox/argv/exceptions.hpp>

namespace toolbox
{
namespace argv
{

template<typename Options>
class parser
{
public:
    parser(Options &&options) :
        options_(std::move(options))
    {
        std::stringstream ss;
        cpp::tuple_for_each(
            options_,
            [&ss, this](auto& option) {
                updateOptions(ss, option, option.has_argument());
            }
        );

        opt_descriptor_ = ss.str();
    }

    void updateOptions(std::stringstream &ss, const options::base_option &option, bool has_argument)
    {
        ss << option.get_short();
        if (has_argument)
            ss << ":";

        if (option.get_long().size() > 0) {
            ::option o = {
                option.get_long().c_str(),
                has_argument ? required_argument : no_argument,
                nullptr,
                option.get_short()
            };

            options_arr_.push_back(o);
        }

        longest_name_ = std::max(longest_name_, option.get_long().size());
        longest_argname_ = std::max(longest_argname_, option.get_argument_name().size());
    }

    void parse(int argc, char ** argv)
    {
        int c;
        int index;

        while (
            (c = getopt_long(
                    argc,
                    const_cast<char* const*>(argv),
                    opt_descriptor_.c_str(),
                    options_arr_.data(),
                    &index))
                != -1) {
            cpp::tuple_for_each(
                options_,
                [this, c](auto& option) {
                    if (option.get_short() == c) {
                        option.set_found(optarg);
                    }
                }
            );
        }

        for (int i = optind; i < argc; i ++)
            non_options_.push_back(std::string(argv[i]));

        cpp::tuple_for_each(
            options_,
            [](auto &option) {
                option.transfer_to_storage();
            }
        );
    }

    const std::vector<std::string> &non_options() const
    {
        return non_options_;
    }

    void print_options(std::ostream &os) const
    {
        /* TODO: this is just a ugly draft - refactor */
        cpp::tuple_for_each(
            options_,
            [this, &os](auto &option) {
                std::stringstream p1;

                p1 << "  -" << option.get_short();
                if (option.get_long().size() > 0)
                    p1 << ", --" << option.get_long();

                if (option.get_argument_name().size() > 0)
                    p1 << " [" << option.get_argument_name() << "]";

                std::stringstream p2;
                p2 << std::setw(longest_argname_ + longest_name_ + 10) << std::left << p1.str();

                if (option.get_description().size() > 0)
                    p2 << " - " << option.get_description();

                os << p2.str() << std::endl;
            }
        );
    }

    template<typename... RequiredOptions>
    void check_required(const RequiredOptions&... options) const
    {
        std::vector<std::pair<char, std::string>> not_found;
        cpp::for_each_param(
            [&not_found](auto &option) {
                if (option.found() == 0) {
                    not_found.push_back(std::make_pair(option.get_short(), option.get_long()));
                }
            },
            options...
        );
        if (not_found.size() > 0)
            throw exceptions::missing_required(std::move(not_found));
    }

private:
    Options                     options_;
    std::vector<::option>       options_arr_;
    std::string                 opt_descriptor_;
    std::vector<std::string>    non_options_;
    std::size_t                 longest_name_ = 0;
    std::size_t                 longest_argname_ = 0;
};

template<typename... Options>
auto make_parser(Options&&... options)
{
    auto options_tuple = cpp::make_ref_tuple(options...);
    return parser<decltype(options_tuple)>(std::move(options_tuple));
}

} // namespace argv
} // namespace toolbox

#endif
