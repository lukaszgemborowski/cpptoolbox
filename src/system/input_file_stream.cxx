#include "toolbox/system/input_file_stream.hpp"

namespace toolbox
{
input_file_device::input_file_device(FILE *file)
    : file {file}
{
}

std::streamsize input_file_device::read(char_type* s, std::streamsize n)
{
    auto r = std::fread(s, sizeof(char_type), n, file);

    if (r == 0 && std::feof(file)) {
        return -1;
    } else {
        return r;
    }
}

} // namespace toolbox
