#include "toolbox/argv/parser.hpp"

namespace toolbox
{
namespace argv
{

void base_parser::updateOptions(std::stringstream &ss, const base_option &option, bool has_argument)
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

} // namespace argv
} // namespace toolbox
