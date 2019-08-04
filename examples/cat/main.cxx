#include <toolbox/system/input_file_stream.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "wrong number of arguments\n";
        return 1;
    }

    auto f = std::fopen(argv[1], "r");

    if (!f) {
        std::cout << "failed to open file: " << argv[1] << std::endl;
        return 2;
    }

    toolbox::input_file_stream ifs {f};

    for (std::string line; std::getline(ifs, line);) {
        std::cout << line << '\n';
    }

    std::fclose(f);
}
