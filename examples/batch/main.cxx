#include <toolbox/argv.hpp>
#include <toolbox/system/popen.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
    using namespace toolbox;
    auto opt_to_out = argv::option<void>('o', "stdout")
        .description("redirect command output to stdout");

    auto parser = argv::make_parser(opt_to_out);
    parser.parse(argc, argv);

    if (parser.non_options().size() != 1) {
        std::cerr << "you need to provide file name\n";
        return 1;
    }

    std::ifstream ifs {parser.non_options()[0]};

    for (std::string line; std::getline(ifs, line);) {
        Popen command {line.c_str(), Popen::Mode::Read};

        if (opt_to_out.value()) {
            auto in = command.output();

            for (std::string o; std::getline(in, o);) {
                std::cout << "> " << o << '\n';
            }
        }

        std::cout << "Command \"" << line << "\" returned: " << command.wait() << std::endl;
    }
}
