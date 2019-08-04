#ifndef TOOLBOX_SYSTEM_POPEN_HPP
#define TOOLBOX_SYSTEM_POPEN_HPP

#include <string_view>
#include <cstdio>
#include <toolbox/system/input_file_stream.hpp>

namespace toolbox
{

class Popen
{
public:
    enum class Mode {
        Read, Write
    };

    Popen(std::string_view command, std::initializer_list<std::string_view> arguments, Mode mode);
    Popen(const char *cmdline, Mode mode);
    ~Popen();

    input_file_stream output();

    int wait();


private:
    FILE *fd = nullptr;
};

} // namespace toolbox

#endif // TOOLBOX_SYSTEM_POPEN_HPP
