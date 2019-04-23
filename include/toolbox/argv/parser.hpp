#ifndef _TOOLBOX_ARGV_PARSER_HPP_
#define _TOOLBOX_ARGV_PARSER_HPP_

#include <tuple>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <getopt.h>
#include <toolbox/cpp/tuple_for_each.hpp>
#include <toolbox/cpp/for_each_param.hpp>
#include <toolbox/argv/options.hpp>
#include <toolbox/argv/exceptions.hpp>
#include <toolbox/cpp/cstring_ref.hpp>

namespace toolbox
{
namespace argv
{

// non-templated part of parser class
class base_parser
{
public:
    void updateOptions(std::stringstream &ss, const base_option &option, bool has_argument);

protected:
    std::vector<::option>       options_arr_;
    std::string                 opt_descriptor_;
    std::vector<cstring_ref>    non_options_;
    std::size_t                 longest_name_ = 0;
    std::size_t                 longest_argname_ = 0;
};

// command line argument parser class
template<typename Options>
class parser : public base_parser
{
public:
    // creates parser object from all available arguments
    parser(Options &&options) :
        options_(std::move(options))
    {
        std::stringstream ss;
        cpp::tuple_for_each(
            options_,
            [&ss, this](auto& option) {
                // on gcc 5 & 6 call to updateOptions
                // without -> fails to compile
                this->updateOptions(ss, option, option.has_argument());
            }
        );

        opt_descriptor_ = ss.str();
    }

    // disable copying for now, revise later if needed
    parser() = delete;
    parser(const parser &) = delete;
    parser& operator=(const parser &) = delete;
    parser& operator=(parser &&) = delete;

    // this is needed for make_parser utility
    // as it will return rvalue
    parser(parser &&) = default;

    // actually parse command line arguments with getopt library
    // this shall populate data to options objects
    void parse(int argc, char ** argv)
    {
        int c;
        int index;
        optind = 0;

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
            non_options_.push_back(cstring_ref(static_cast<char const *>(argv[i])));

        cpp::tuple_for_each(
            options_,
            [](auto &option) {
                option.transfer_to_storage();
            }
        );
    }

    // return vector of all remaining free arguments (ie. not in form of -f or --foo
    const auto &non_options() const
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
    Options options_;
};

// helper function for creating parser object from available options
template<typename... Options>
auto make_parser(Options&&... options)
{
    auto options_tuple { std::forward_as_tuple(options...) };
    return parser<decltype(options_tuple)>{ std::move(options_tuple) };
}

} // namespace argv
} // namespace toolbox

#endif
