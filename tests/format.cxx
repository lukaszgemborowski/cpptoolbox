#include <toolbox/testing/test.h>
#include <toolbox/cpp/format.hpp>

// we may compile this without standard C++ library
// so let's use C version of strncmp
#include <string.h>

template<std::size_t N>
struct output_stream {
    char buffer[N] = {0};
    std::size_t pos = 0;

    void put(char c) {
        buffer[pos] = c;
        pos++;
    }
};

TEST_CASE(format_print_string)
{
    output_stream<10> os;
    toolbox::format::print(os, "test");

    CHECK(strncmp(os.buffer, "test", 4) == 0);
}

TEST_CASE(format_print_with_number)
{
    output_stream<10> os;
    toolbox::format::print(os, "test {}", 42);

    CHECK(strncmp(os.buffer, "test 42", 7) == 0);
}

TEST_CASE(format_print_with_hex_number)
{
    output_stream<10> os;
    toolbox::format::print(os, "test 0x{x}", 0x42);

    CHECK(strncmp(os.buffer, "test 0x00000042", 15) == 0);
}