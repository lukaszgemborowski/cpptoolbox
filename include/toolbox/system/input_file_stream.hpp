#ifndef TOOLBOX_SYSTEM_INPUT_FILE_STREAM_HPP
#define TOOLBOX_SYSTEM_INPUT_FILE_STREAM_HPP

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/stream.hpp>
#include <ios>
#include <cstdio>

namespace toolbox
{

class input_file_device
{
public:
    using char_type = char;
    using category = boost::iostreams::source_tag;

    explicit input_file_device(FILE *file);
    std::streamsize read(char_type* s, std::streamsize n);

private:
    FILE *file = nullptr;
};

using input_file_stream = boost::iostreams::stream<input_file_device>;

} // namespace toolbox

#endif // TOOLBOX_SYSTEM_INPUT_FILE_STREAM_HPP
