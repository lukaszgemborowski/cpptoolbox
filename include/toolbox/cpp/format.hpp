#ifndef _TOOLBOX_CPP_FORMAT_HPP_
#define _TOOLBOX_CPP_FORMAT_HPP_

#include <toolbox/container/stack.hpp>
#include <toolbox/span.hpp>

namespace toolbox
{

namespace format {
namespace detail {
struct flags {
    bool hex = 0;
};
} // namespace detail
template<typename Output, typename T>
void format(Output &out, detail::flags flags, T number)
{
    // TODO: may be removed when we fix stack size
    static_assert(sizeof(number) <= 4, "does not support integer types > 4 bytes");

    if (flags.hex == false) {
        // TODO: allocate stack based on max dec representation
        // of type T, for now we assume 32 bit integer which
        // may have up to 10 digits in dec representation
        toolbox::stack<char, 10> stack;

        while (number != 0) {
            char c = number % 10;
            stack.emplace_back('0' + c);
            number = number / 10;
        }

        while (stack.size()) {
            out.put(stack.back());
            stack.pop_back();
        }
    } else if (flags.hex == true) {
        static char lt[] = {
            '0', '1', '2', '3', '4',
            '5', '6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E',
            'F'
        };

        constexpr auto hexSize = sizeof(T) * 2;
        for (int i = 0; i < hexSize; i ++) {
            out.put(
                lt[(number >> (hexSize-1-i)*4) & 0xf]
            );
        }
    }
}


template<typename Output>
void print(Output &out, const char *fmt)
{
    while (*fmt) {
        out.put(*fmt);
        fmt ++;
    }
}

template<typename Output, typename Next>
void format_argument(Output &out, span<const char> fmt, const Next &next)
{
    detail::flags flags;

    for (auto c : fmt)
    {
        if (c == 'x') {
            flags.hex = true;
        }
    }

    format(out, flags, next);
}

template<typename Output, typename Next, typename... Args>
void print(Output &out, const char *fmt, const Next &next, const Args&... args)
{
    bool format = false;
    const char *fmtBegin = nullptr;
    detail::flags flags;

    while (*fmt) {
        if (format) {
            if (*fmt == '}') {
                format_argument(out, span<const char>{fmtBegin, fmt}, next);
                print(out, fmt + 1, args...);
                return;
            }
        } else {
            if (*fmt != '{') {
                out.put(*fmt);
            } else {
                format = true;
                fmtBegin = fmt + 1;
            }
        }

        fmt ++;
    }
}

} // namespace format
} // namespace toolbox

#endif // _TOOLBOX_CPP_FORMAT_HPP_
