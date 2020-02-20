#include "toolbox/system/popen.hpp"
#include "toolbox/string/cat.hpp"

namespace toolbox
{

Popen::Popen(std::string_view command, std::initializer_list<std::string_view> arguments, Mode mode)
{
    auto cmd = string::cat(command, arguments);
    fd = popen(cmd.c_str(), mode == Mode::Read ? "r" : "w");
}

Popen::Popen(const char *cmdline, Mode mode)
    : fd {popen(cmdline, mode == Mode::Read ? "r" : "w")}
{
}

Popen::~Popen()
{
    wait();
}

int Popen::wait()
{
    if (fd == nullptr)
        // TODO: throw exception
        return -1;

    auto r = pclose (fd);
    fd = nullptr;
    return r;
}

input_file_stream Popen::output()
{
    return input_file_stream {fd};
}

} // namespace toolbox
