#ifndef _TOOLBOX_ARGV_EXCEPTIONS_HPP_
#define _TOOLBOX_ARGV_EXCEPTIONS_HPP_

#include <stdexcept>
#include <vector>

namespace toolbox
{
namespace argv
{
namespace exceptions
{

struct missing_required : public std::invalid_argument
{
    missing_required(std::vector<std::pair<char, std::string>> &&options) :
        invalid_argument("Missing command line arguments"),
        missing(std::move(options))
    {}

    std::vector<std::pair<char, std::string>> missing;
};

} // namespace exceptions
} // namespace argv
} // namespace toolbox

#endif
